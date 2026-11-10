#!/usr/bin/env python3
"""Put the built ELF into a copy of the original disc image, for boot tests.

The new ELF replaces the boot ELF in place: it is written into the original
file's sectors (the rest of them is zeroed) and the file size is updated in
both the ISO9660 directory record (read by the PS2 and PCSX2) and the UDF
file entry (read by most PC tools). Every other file keeps its sector, so
games that address data by sector number still work.

The boot copy of the ELF gets PhysAddr = VirtAddr in its program headers
(splat's linker script leaves them at the ROM offset), and its loadable image
is compared with the original's.

Usage:
    python3 tools/build_iso.py SLUS_217.99 [--iso iso/game.iso] [--elf path] [--out path]
    ninja iso        # same, via the build
"""

import argparse
import shutil
import struct
import sys
from pathlib import Path

import yaml
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parent.parent
SECTOR = 2048


# --- ELF -------------------------------------------------------------------------------------------------------------

def load_image(path: Path) -> tuple[int, list[tuple[int, int, bytes]]]:
    """(entry, [(vaddr, memsz, file bytes)]) of the PT_LOAD segments."""
    with path.open("rb") as f:
        elf = ELFFile(f)
        segments = [(s["p_vaddr"], s["p_memsz"], s.data()) for s in elf.iter_segments() if s["p_type"] == "PT_LOAD"]
        return elf["e_entry"], segments


def boot_elf(built: Path, out: Path) -> None:
    """Copy of the built ELF with p_paddr = p_vaddr for every PT_LOAD segment."""
    data = bytearray(built.read_bytes())
    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 1:
        sys.exit(f"error: {built} is not a 32-bit little-endian ELF")
    phoff, = struct.unpack_from("<I", data, 0x1C)
    phentsize, phnum = struct.unpack_from("<HH", data, 0x2A)
    for i in range(phnum):
        ph = phoff + i * phentsize
        p_type, _offset, vaddr = struct.unpack_from("<III", data, ph)
        if p_type == 1:  # PT_LOAD
            struct.pack_into("<I", data, ph + 12, vaddr)
    out.write_bytes(data)


def compare_images(original: Path, built: Path) -> bool:
    entry_a, segs_a = load_image(original)
    entry_b, segs_b = load_image(built)
    same = entry_a == entry_b and [(v, m) for v, m, _ in segs_a] == [(v, m) for v, m, _ in segs_b]
    diffs = sum(sum(x != y for x, y in zip(a[2], b[2])) + abs(len(a[2]) - len(b[2])) for a, b in zip(segs_a, segs_b))
    if same and diffs == 0:
        print("loadable image: identical to the original (entry point, segments, bytes)")
        return True
    print(f"loadable image: DIFFERS from the original - entry {entry_b:#x} vs {entry_a:#x}, "
          f"segments {[(hex(v), hex(m)) for v, m, _ in segs_b]} vs {[(hex(v), hex(m)) for v, m, _ in segs_a]}, "
          f"{diffs} differing bytes")
    return False


# --- ISO9660 ---------------------------------------------------------------------------------------------------------

def read_sector(f, lba: int, count: int = 1) -> bytes:
    f.seek(lba * SECTOR)
    return f.read(count * SECTOR)


def find_iso9660_record(f, name: str) -> tuple[int, int, int]:
    """(byte offset of the directory record, extent LBA, data length) of a file in the root directory."""
    pvd = read_sector(f, 16)
    if pvd[1:6] != b"CD001" or pvd[0] != 1:
        sys.exit("error: no ISO9660 primary volume descriptor at sector 16")
    root_lba, root_len = struct.unpack_from("<I", pvd, 156 + 2)[0], struct.unpack_from("<I", pvd, 156 + 10)[0]
    wanted = f"{name};1".upper().encode()
    for s in range((root_len + SECTOR - 1) // SECTOR):
        sector = read_sector(f, root_lba + s)
        pos = 0
        while pos < SECTOR and sector[pos]:
            length = sector[pos]
            ident = sector[pos + 33:pos + 33 + sector[pos + 32]]
            if ident.upper() in (wanted, wanted[:-2]):
                extent, size = struct.unpack_from("<I", sector, pos + 2)[0], struct.unpack_from("<I", sector, pos + 10)[0]
                return (root_lba + s) * SECTOR + pos, extent, size
            pos += length
    sys.exit(f"error: {name} not found in the ISO9660 root directory")


def boot_name(f) -> str:
    _, extent, size = find_iso9660_record(f, "SYSTEM.CNF")
    cnf = read_sector(f, extent)[:size].decode("ascii", errors="replace")
    for line in cnf.splitlines():
        if line.strip().upper().startswith("BOOT2"):
            return line.split("\\")[-1].split(";")[0].strip()
    sys.exit("error: no BOOT2 line in SYSTEM.CNF")


def disc_boot_elf(path: Path) -> bytes | None:
    """The boot ELF's bytes on a disc image (None when it isn't a PS2 disc with ISO9660)."""
    with path.open("rb") as f:
        pvd = read_sector(f, 16)
        if pvd[1:6] != b"CD001":
            return None
        _, extent, size = find_iso9660_record(f, boot_name(f))
        f.seek(extent * SECTOR)
        return f.read(size)


def find_disc(original_elf: Path) -> Path:
    """The disc image in iso/ whose boot ELF is the version's original ELF (build.yaml `elf:`): iso/ may hold the
    discs of several versions (the beta's, the retail one), all booting an ELF of the same name."""
    if not original_elf.exists():
        sys.exit(f"error: {original_elf.relative_to(ROOT)} (build.yaml `elf:`) doesn't exist: pass --iso")
    wanted = original_elf.read_bytes()
    isos = sorted((ROOT / "iso").glob("*.iso"))
    hits = [p for p in isos if disc_boot_elf(p) == wanted]
    if len(hits) != 1:
        sys.exit(f"error: {len(hits)} of the disc images {[p.name for p in isos]} boot "
                 f"{original_elf.relative_to(ROOT)}: pass --iso")
    return hits[0]


# --- UDF (ECMA-167 / UDF 1.02) ---------------------------------------------------------------------------------------

def crc_itu(data: bytes) -> int:
    crc = 0
    for byte in data:
        crc ^= byte << 8
        for _ in range(8):
            crc = ((crc << 1) ^ 0x1021) if crc & 0x8000 else crc << 1
            crc &= 0xFFFF
    return crc


def fix_tag(desc: bytearray) -> None:
    crc_len, = struct.unpack_from("<H", desc, 10)
    struct.pack_into("<H", desc, 8, crc_itu(bytes(desc[16:16 + crc_len])))
    desc[4] = sum(desc[i] for i in range(16) if i != 4) & 0xFF


def tag_id(data: bytes, offset: int = 0) -> int:
    return struct.unpack_from("<H", data, offset)[0]


def read_extent(f, partition: int, ad: bytes, long_ad: bool) -> bytes:
    length, = struct.unpack_from("<I", ad, 0)
    lbn, = struct.unpack_from("<I", ad, 4)
    length &= 0x3FFFFFFF
    return read_sector(f, partition + lbn, (length + SECTOR - 1) // SECTOR)[:length]


def file_entry_ads(fe: bytes) -> tuple[bool, int, int]:
    """(long_ad?, offset of the allocation descriptors, their total length)."""
    flags, = struct.unpack_from("<H", fe, 16 + 18)
    l_ea, l_ad = struct.unpack_from("<II", fe, 168)
    return (flags & 7) == 1, 176 + l_ea, l_ad


def find_udf_file_entry(f, name: str) -> tuple[int, int] | None:
    """(LBA of the file's File Entry, LBA of its data) in the UDF root directory, or None without UDF."""
    avdp = read_sector(f, 256)
    if tag_id(avdp) != 2:
        return None
    vds_len, vds_loc = struct.unpack_from("<II", avdp, 16)
    partition = fsd_lbn = None
    for i in range(vds_len // SECTOR):
        desc = read_sector(f, vds_loc + i)
        tid = tag_id(desc)
        if tid == 5:
            partition, = struct.unpack_from("<I", desc, 188)
        elif tid == 6:
            if struct.unpack_from("<I", desc, 212)[0] != SECTOR:
                sys.exit("error: unexpected UDF logical block size")
            fsd_lbn, = struct.unpack_from("<I", desc, 252)
        elif tid == 8:
            break
    if partition is None or fsd_lbn is None:
        sys.exit("error: incomplete UDF volume descriptor sequence")
    fsd = read_sector(f, partition + fsd_lbn)
    root_fe = read_sector(f, partition + struct.unpack_from("<I", fsd, 404)[0])
    long_ad, ad_off, ad_len = file_entry_ads(root_fe)
    ad_size = 16 if long_ad else 8
    directory = b"".join(read_extent(f, partition, root_fe[ad_off + i:ad_off + i + ad_size], long_ad)
                         for i in range(0, ad_len, ad_size))
    pos = 0
    while pos + 38 <= len(directory):
        if tag_id(directory, pos) != 257:
            break
        l_fi, = struct.unpack_from("<B", directory, pos + 19)
        icb_lbn, = struct.unpack_from("<I", directory, pos + 24)
        l_iu, = struct.unpack_from("<H", directory, pos + 36)
        ident = directory[pos + 38 + l_iu:pos + 38 + l_iu + l_fi]
        text = ident[1:].decode("latin-1") if ident[:1] == b"\x08" else ident[1:].decode("utf-16-be", errors="replace")
        if l_fi and text.upper() == name.upper():
            fe = read_sector(f, partition + icb_lbn)
            long_ad, ad_off, _ = file_entry_ads(fe)
            data_lbn, = struct.unpack_from("<I", fe, ad_off + 4)
            return partition + icb_lbn, partition + data_lbn
        pos += (38 + l_iu + l_fi + 3) & ~3
    sys.exit(f"error: {name} not found in the UDF root directory")


def patch_udf_file_entry(f, fe_lba: int, size: int) -> None:
    fe = bytearray(read_sector(f, fe_lba))
    if tag_id(fe) != 261:
        sys.exit("error: UDF file entry has an unexpected tag")
    long_ad, ad_off, ad_len = file_entry_ads(fe)
    if ad_len != (16 if long_ad else 8):
        sys.exit("error: the UDF file entry has more than one extent")
    struct.pack_into("<Q", fe, 56, size)                                # information length
    struct.pack_into("<Q", fe, 64, (size + SECTOR - 1) // SECTOR)       # logical blocks recorded
    kind = struct.unpack_from("<I", fe, ad_off)[0] & 0xC0000000
    struct.pack_into("<I", fe, ad_off, kind | size)                     # extent length
    fix_tag(fe)
    f.seek(fe_lba * SECTOR)
    f.write(fe)


# --- main ------------------------------------------------------------------------------------------------------------

def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("version", help="folder name under config/")
    parser.add_argument("--iso", type=Path, help="original disc image (default: the *.iso in iso/ whose boot ELF is build.yaml's `elf:`)")
    parser.add_argument("--elf", type=Path, help="ELF to put on the disc (default: the build output)")
    parser.add_argument("--out", type=Path, help="output image (default: build/<VERSION>/<VERSION>.iso)")
    parser.add_argument("--fresh", action="store_true", help="copy the original image again instead of patching the last output")
    parser.add_argument("--shifted", action="store_true",
                        help="the ELF is a shifted link (docs/port-step1-shiftable.md): don't compare its image")
    args = parser.parse_args()

    cfg_dir = ROOT / "config" / args.version
    opts = yaml.safe_load((cfg_dir / f"{args.version}.yaml").read_text(encoding="utf-8"))["options"]
    build_cfg = yaml.safe_load((cfg_dir / "build.yaml").read_text(encoding="utf-8"))
    original_iso = args.iso or find_disc(ROOT / build_cfg["elf"])
    built_elf = ROOT / args.elf if args.elf else ROOT / opts["elf_path"]  # relative paths: from the repo root
    out_iso = ROOT / args.out if args.out else ROOT / opts["build_path"] / f"{args.version}.iso"
    boot_copy = out_iso.with_suffix(".boot.elf")

    boot_elf(built_elf, boot_copy)
    if args.shifted:
        print("loadable image: NOT compared - a shifted build (docs/port-step1-shiftable.md) differs from the "
              "original by design; tools/shiftcheck.py checks it against the matching build")
    else:
        compare_images(ROOT / build_cfg["elf"], boot_copy)
    new_elf = boot_copy.read_bytes()

    # Locations come from the untouched original, so repeated runs can patch the same output again.
    with original_iso.open("rb") as f:
        name = boot_name(f)
        record_offset, extent, original_size = find_iso9660_record(f, name)
        udf = find_udf_file_entry(f, name)
    allocated = (original_size + SECTOR - 1) // SECTOR * SECTOR
    if len(new_elf) > allocated:
        sys.exit(f"error: the new ELF ({len(new_elf):#x} bytes) doesn't fit the original's {allocated:#x} bytes")
    if udf is not None and udf[1] != extent:
        sys.exit(f"error: UDF and ISO9660 disagree on where {name} is stored")

    if args.fresh or not out_iso.exists() or out_iso.stat().st_size != original_iso.stat().st_size:
        print(f"copying {original_iso.relative_to(ROOT)} -> {out_iso.relative_to(ROOT)}")
        out_iso.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(original_iso, out_iso)

    with out_iso.open("r+b") as f:
        f.seek(extent * SECTOR)
        f.write(new_elf + bytes(allocated - len(new_elf)))
        f.seek(record_offset + 10)
        f.write(struct.pack("<I", len(new_elf)) + struct.pack(">I", len(new_elf)))  # both-endian data length
        if udf is not None:
            patch_udf_file_entry(f, udf[0], len(new_elf))

    print(f"{name}: {len(new_elf):#x} bytes at sector {extent} (original {original_size:#x} bytes)"
          f"{', ISO9660 + UDF updated' if udf else ', ISO9660 updated (no UDF)'}")
    print(f"wrote {out_iso.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
