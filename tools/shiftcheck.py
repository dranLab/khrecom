#!/usr/bin/env python3
"""Check a shifted link against the matching one, word by word (docs/port-step1-shiftable.md S6).

`ninja shift-<preset>` links the matching build's objects with padding inserted. Both link maps give every input
section's address, so each word of the matching image has a counterpart in the shifted image. For every word
with content:

- relocated in the shifted link (its --emit-relocs relocations): an R_MIPS_32 word must hold the matching value
  moved like its target, an R_MIPS_26 jump must reach the moved target, HI16/LO16 instructions must keep their
  opcode and registers;
- not relocated: the word must be identical. If its value is an image address that moved (exactly, as a DMA tag's
  address field, or as a j/jal target in code), it is a SUSPECT: a pointer the link can't see, which a shifted
  program would follow to the old place;
- R_MIPS_32 words in splat's data objects that point into the middle of data (no symbol starts there, not code)
  are listed for review: string or instruction bytes splat took for a reference move with the link and get
  corrupted;
- symbols that lose alignment (16 or more in the matching layout, less in the shifted one): qword data (VU0
  lqc2/sqc2, DMA) would fault or read the wrong place.

Suspects can be cleared one by one in config/<VERSION>/shift_allow.txt: `<object> <section> 0x<offset> <reason>`
(no ranges). Exit status 1 for errors, unexplained suspects or lost alignment.

Usage: .venv/bin/python tools/shiftcheck.py [--preset text128] [--details 40]
"""

import argparse
import bisect
import re
import struct
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parent.parent
CROSS = "mips-linux-gnu-"
# Instructions that take the low half of an address after a lui (opcode -> name).
LOW_HALF_OPS = {0x09: "addiu", 0x20: "lb", 0x21: "lh", 0x23: "lw", 0x24: "lbu", 0x25: "lhu",
                0x27: "lwu", 0x28: "sb", 0x29: "sh", 0x2B: "sw", 0x1E: "lq", 0x1F: "sq", 0x37: "ld", 0x3F: "sd",
                0x31: "lwc1", 0x39: "swc1", 0x36: "lqc2", 0x3E: "sqc2"}


def writes_reg(w: int):
    """The GPR an instruction writes (rd for SPECIAL/MMI R-type, rt for immediates and loads), else None."""
    op = w >> 26
    if op in (0x00, 0x1C):  # SPECIAL, MMI
        return (w >> 11) & 31
    if op in (0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x18, 0x19) or 0x20 <= op <= 0x27 or op in (0x1E, 0x37):
        return (w >> 16) & 31
    return None


MAP_LINE = re.compile(r"^ (\.\w+|COMMON)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+\.o)$", re.M)


def read_map(path: Path) -> dict[tuple[str, str], tuple[int, int]]:
    out = {}
    for m in MAP_LINE.finditer(path.read_text(encoding="utf-8")):
        size = int(m.group(3), 16)
        if size:
            out[(m.group(4), m.group(1))] = (int(m.group(2), 16), size)
    return out


class Image:
    def __init__(self, elf: Path):
        self.elf = elf
        self.data, self.base = b"", 0
        out = subprocess.run([f"{CROSS}readelf", "-SW", str(elf)], capture_output=True, text=True, check=True).stdout
        for line in out.splitlines():
            parts = line.replace("[ ", "[").split()
            if len(parts) > 6 and parts[1] == ".cod" and parts[2] == "PROGBITS":
                self.base, off, size = int(parts[3], 16), int(parts[4], 16), int(parts[5], 16)
                self.data = elf.read_bytes()[off: off + size]

    def word(self, addr: int):
        i = addr - self.base
        return struct.unpack_from("<I", self.data, i)[0] if 0 <= i <= len(self.data) - 4 else None


def relocations(elf: Path) -> tuple[dict[int, str], dict[int, tuple[str, int, str]]]:
    """offset -> type, and offset -> (type, symbol value, symbol name)."""
    out = subprocess.run([f"{CROSS}readelf", "-rW", str(elf)], capture_output=True, text=True, check=True).stdout
    relocs, detail = {}, {}
    for line in out.splitlines():
        parts = line.split()
        if len(parts) >= 3 and parts[2].startswith("R_MIPS_"):
            relocs[int(parts[0], 16)] = parts[2]
            if len(parts) >= 5:
                detail[int(parts[0], 16)] = (parts[2], int(parts[3], 16), parts[4])
    return relocs, detail


def symbols(elf: Path, absolute: bool = False) -> list[tuple[int, str]]:
    out = subprocess.run([f"{CROSS}nm", "-n", str(elf)], capture_output=True, text=True, check=True).stdout
    syms = []
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[1] != "U" and (absolute or parts[1] not in "aA"):
            syms.append((int(parts[0], 16), parts[2]))
    return syms


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--preset", default="text128")
    parser.add_argument("--details", type=int, default=40, help="lines per category in the report")
    args = parser.parse_args()

    version = next(p.name for p in (ROOT / "config").iterdir() if (p / "build.yaml").exists())
    opts = yaml.safe_load((ROOT / "config" / version / f"{version}.yaml").read_text(encoding="utf-8"))["options"]
    build = ROOT / opts["build_path"]
    m_elf, s_elf = build / f"{version}.elf", build / "shift" / args.preset / f"{version}.elf"
    m_map, s_map = read_map(m_elf.with_suffix(".map")), read_map(s_elf.with_suffix(".map"))
    allow = {}
    allow_file = ROOT / "config" / version / "shift_allow.txt"
    if allow_file.exists():
        for line in allow_file.read_text(encoding="utf-8").splitlines():
            if line.strip() and not line.startswith("#"):
                obj, sec, off, reason = line.split(None, 3)
                allow[(obj, sec, int(off, 16))] = reason

    pieces = []  # (matching start, size, shifted start, object, section)
    for key, (m_start, size) in m_map.items():
        if key in s_map:
            s_start, s_size = s_map[key]
            if s_size != size:
                sys.exit(f"error: {key} has size {size:#x} in the matching link, {s_size:#x} in the shifted one")
            pieces.append((m_start, size, s_start, *key))
    pieces.sort()
    starts = [p[0] for p in pieces]

    def where(addr: int):
        i = bisect.bisect_right(starts, addr) - 1
        if i >= 0 and addr < pieces[i][0] + pieces[i][1]:
            return pieces[i]
        return None

    ends = {p[0] + p[1]: p for p in pieces}

    def moved(addr: int):
        p = where(addr) or ends.get(addr)  # one past the end (e.g. _end) moves with the section that ends there
        return p[2] + (addr - p[0]) if p else None

    mi, si = Image(m_elf), Image(s_elf)
    relocs, reloc_detail = relocations(s_elf)
    m_syms, s_syms = symbols(m_elf), symbols(s_elf)
    sym_starts = {a for a, _ in m_syms}
    linker_names = {"_gp", "_end", "end"}  # defined by the linker scripts, not by an object
    m_linker = {n: a for a, n in symbols(m_elf, absolute=True) if n in linker_names}
    s_linker = {n: a for a, n in symbols(s_elf, absolute=True) if n in linker_names}
    code = [(p[0], p[0] + p[1]) for p in pieces if p[4] == ".text"]

    def in_code(addr: int) -> bool:
        return any(a <= addr < b for a, b in code)

    # Objects generated with every pointer checked (tools/sndll.py's .sndata): not splat's guesses.
    generated = set(re.findall(r"^build (\S+\.o): as \S+/sndll/sndata\.s$",
                               (ROOT / "build.ninja").read_text(encoding="utf-8"), re.M))
    errors, suspects, review, cleared, compiled_hits = [], [], Counter(), 0, []
    code_hits = []  # addresses built in code without relocation (any object: code is never "a constant")

    def add_code_hit(obj: str, sec: str, off: int, text: str) -> None:
        nonlocal cleared
        if (obj.replace(f"build/{version}/", ""), sec, off) in allow:
            cleared += 1
        else:
            code_hits.append(f"{obj} {sec} 0x{off:X}: {text}")
    review_lines = []
    checked = Counter()
    for m_start, size, s_start, obj, sec in pieces:
        if sec in (".bss", ".sbss", "COMMON"):
            continue
        for off in range(0, size - 3, 4):
            a, b = m_start + off, s_start + off
            wm, ws = mi.word(a), si.word(b)
            if wm is None or ws is None:
                continue
            checked[sec] += 1
            rel = relocs.get(b)
            where_s = f"{obj} {sec} 0x{off:X}"
            if rel is None:
                if wm != ws:
                    errors.append(f"{where_s}: not relocated, but {wm:#010x} -> {ws:#010x}")
                    continue
                hit = None
                # In code only jumps can hide an address (gcc puts no data in .text; instruction words often look
                # like addresses: 0x00431024 is "and $v0,$v0,$v1").
                if sec != ".text" and moved(wm) not in (None, wm) and wm % 4 == 0:
                    hit = f"value {wm:#010x} is an address that moved"
                elif wm & 0x7FFFFFF0 != wm and moved(wm & 0x7FFFFFF0) not in (None, wm & 0x7FFFFFF0) \
                        and (wm >> 28) & 7 and sec != ".text":
                    hit = f"DMA-tag-like {wm:#010x}: address {wm & 0x7FFFFFF0:#x} moved"
                elif sec == ".text" and (wm >> 26) in (2, 3):
                    target = (a & 0xF0000000) | ((wm & 0x3FFFFFF) << 2)
                    if moved(target) not in (None, target):
                        hit = f"{'jal' if wm >> 26 == 3 else 'j'} {target:#x} without relocation"
                        add_code_hit(obj, sec, off, hit)
                        continue
                elif sec == ".text" and (wm >> 26) == 0x0F:  # lui without relocation: an address built by hand?
                    rt, hi = (wm >> 16) & 31, (wm & 0xFFFF) << 16
                    for k in range(1, 9):  # its use: addiu/ori/load/store with rt as base, also unrelocated
                        w2 = mi.word(a + 4 * k)
                        if w2 is None:
                            break
                        op, rs = w2 >> 26, (w2 >> 21) & 31
                        if relocs.get(b + 4 * k) or rs != rt or op not in LOW_HALF_OPS:
                            if writes_reg(w2) == rt:
                                break  # rt redefined before any use as an address
                            continue
                        lo = w2 & 0xFFFF
                        value = (hi + (lo - 0x10000 if lo & 0x8000 else lo)) & 0xFFFFFFFF
                        if moved(value) not in (None, value):
                            add_code_hit(obj, sec, off, f"lui + {LOW_HALF_OPS[op]} at +{4 * k:#x} build "
                                                        f"{value:#010x}, an address that moved, without relocation")
                        break
                    continue
                if hit and obj in generated:
                    hit = None  # every pointer field of the generated table is symbolic by construction
                if hit and "/asm/" not in obj:
                    # Compiled C: a pointer initializer always carries a relocation, so an unrelocated word is a
                    # constant unless the source writes a numeric address; counted, not failed (S8 boots it).
                    compiled_hits.append(f"{where_s}: {hit}")
                elif hit:
                    key = (obj.replace(f"build/{version}/", ""), sec, off)
                    if key in allow:
                        cleared += 1
                    else:
                        suspects.append(f"{where_s}: {hit}")
                continue
            if rel == "R_MIPS_32":
                expect = moved(wm)
                if expect is None:
                    expect = wm
                if ws != expect and any(m_linker.get(n) == wm and s_linker.get(n) == ws for n in m_linker):
                    pass  # a linker-script symbol (_gp = cod_SDATA_START + 0x7FF0): it moves its own way
                elif ws != expect:
                    errors.append(f"{where_s}: R_MIPS_32 {wm:#010x} -> {ws:#010x}, expected {expect:#010x}")
                elif "/asm/" in obj and "/data/" in obj and obj not in generated and wm not in sym_starts \
                        and not in_code(wm) and wm != 0 and where(wm):
                    review[obj] += 1
                    review_lines.append(f"{where_s}: {wm:#010x} points into the middle of {where(wm)[3]}")
            elif rel == "R_MIPS_26":
                tm = ((a + 4) & 0xF0000000) | ((wm & 0x3FFFFFF) << 2)
                ts = ((b + 4) & 0xF0000000) | ((ws & 0x3FFFFFF) << 2)
                expect = moved(tm)
                if (expect if expect is not None else tm) != ts or (wm >> 26) != (ws >> 26):
                    errors.append(f"{where_s}: jump to {tm:#x} became {ts:#x}")
            elif rel in ("R_MIPS_HI16", "R_MIPS_LO16", "R_MIPS_GPREL16"):
                if (wm >> 16) != (ws >> 16):
                    errors.append(f"{where_s}: {rel} changed the instruction {wm:#010x} -> {ws:#010x}")

    # symbol + offset that lands in another input section than the symbol: the two can drift apart in a layout
    # (splat expressed _gp as "<a .bss variable> + 0x2130"). Checked in the shifted image, by the final values.
    s_pieces = sorted((p[2], p[1], p[3], p[4]) for p in pieces)
    s_starts = [p[0] for p in s_pieces]

    def s_piece(addr: int):
        i = bisect.bisect_right(s_starts, addr) - 1
        return s_pieces[i][2:] if i >= 0 and addr < s_pieces[i][0] + s_pieces[i][1] else None

    crossing = []
    lo_by_sym = defaultdict(list)
    for off_s, (rtype, value, name) in reloc_detail.items():
        if rtype == "R_MIPS_LO16":
            lo_by_sym[name].append(off_s)
    for name in lo_by_sym:
        lo_by_sym[name].sort()
    for off_s, (rtype, value, name) in sorted(reloc_detail.items()):
        if rtype == "R_MIPS_32":
            target = si.word(off_s)
        elif rtype == "R_MIPS_HI16":
            los = lo_by_sym.get(name, [])
            j = bisect.bisect_right(los, off_s)
            if j >= len(los) or los[j] - off_s > 0x400:
                continue
            lo = si.word(los[j]) & 0xFFFF
            target = (((si.word(off_s) & 0xFFFF) << 16) + (lo - 0x10000 if lo & 0x8000 else lo)) & 0xFFFFFFFF
        else:
            continue
        if target is None or name in linker_names or not value or name.startswith("."):
            continue  # section symbols (.cod): --emit-relocs' form of references to local symbols
        a, b = s_piece(value), s_piece(target)
        if a and b and a != b:
            i = bisect.bisect_right(s_starts, off_s) - 1
            obj_r, sec_r, off_r = s_pieces[i][2], s_pieces[i][3], off_s - s_pieces[i][0]
            if (obj_r.replace(f"build/{version}/", ""), sec_r, off_r) in allow:
                cleared += 1
                continue
            crossing.append(f"{obj_r} {sec_r} 0x{off_r:X}: {rtype} {name} ({value:#x}) + "
                            f"{target - value:#x} lands in {b[0]} {b[1]}, not in {a[0]} {a[1]}")

    misaligned = []
    for addr, name in m_syms:
        p = where(addr)
        if p is None or p[4] == ".text":
            continue  # code needs 4-byte alignment only
        new = moved(addr)
        need = min(addr & -addr if addr else 128, 128)
        have = new & -new if new else 128
        if need >= 16 and have < 16:
            misaligned.append(f"{name}: {addr:#x} (aligned {need}) -> {new:#x} (aligned {have})")

    report = build / "shift" / args.preset / "shiftcheck.txt"
    lines = [f"preset {args.preset}: {sum(checked.values())} words checked ({dict(checked)})",
             f"errors: {len(errors)}", f"suspects: {len(suspects)} (+{cleared} cleared by shift_allow.txt)",
             f"compiled constants in the image range (not failed): {len(compiled_hits)}",
             f"lost alignment: {len(misaligned)}",
             f"addresses built in code without relocation: {len(code_hits)}",
             f"symbol + offset into another section: {len(crossing)}",
             f"review: {sum(review.values())} R_MIPS_32 words in splat data pointing into the middle of data "
             f"({dict(review.most_common(8))})"]
    detail = []
    for title, items in (("errors", errors), ("suspects", suspects), ("lost alignment", misaligned),
                         ("crossing", crossing), ("code", code_hits), ("review", review_lines),
                         ("compiled constants", compiled_hits)):
        if items:
            detail += [f"\n== {title}"] + items
    report.write_text("\n".join(lines + detail) + "\n", encoding="utf-8")
    print("\n".join(lines))
    for title, items in (("errors", errors), ("suspects", suspects), ("lost alignment", misaligned),
                         ("crossing", crossing), ("code", code_hits)):
        for item in items[: args.details]:
            print(f"  [{title}] {item}")
    print(f"full report: {report.relative_to(ROOT)}")
    sys.exit(1 if errors or suspects or misaligned or crossing or code_hits else 0)


if __name__ == "__main__":
    main()
