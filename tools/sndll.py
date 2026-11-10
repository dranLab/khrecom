#!/usr/bin/env python3
"""Generate the SN Systems DLL table (.sndata, sn_dll_header_root) from the link's objects (docs/port-step1-shiftable.md
S4/S5).

The main program carries a symbol table for libsn's DLL loader (src/lib/nodebug/check_dll.c): a header, the
relocations of the main program's uses of DLL imports, the symbols (name, value, hash, type) and a zero area the
loader fills. splat's disassembly of it has most pointers as plain numbers (name pointers into the middle of the
strings, the import sites in .text) and took string bytes for references, so it only works at the original
addresses. This writes it again: the original bytes, with every pointer field symbolic.

- header pointers and symbol name pointers: sn_dll_header_root + offset;
- symbol values: the symbol the entry names, where an object defines it at that address (else the object's
  symbol at that address, or the nearest global before it + offset); _gp, _end, end, _stack_size by name;
  imports (type 1) and other absolutes as numbers;
- import relocations: the site as <global symbol before it> + offset, after checking that the object at that
  address has a relocation of the same type there against a symbol of config/<VERSION>/dll_imports.ld.

Addresses are the original's: every object sits at its original address in the matching link (the manifest gives
each object section's original start), and the matching build's SHA-1 checks the result byte for byte. The
symbolic table then moves correctly in a shifted link.

Usage (from build.ninja): tools/sndll.py --rom iso/SLUS_217.99.rom --manifest <objects.txt> --imports <dll_imports.ld>
                                          --start 0x3C2B00 --end 0x45AB00 -o <sndata.s>
"""

import argparse
import bisect
import re
import struct
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

CROSS = "mips-linux-gnu-"
BASE = 0x100000  # the .rom image starts here
LINKER_SYMBOLS = {"_gp", "_end", "end", "_stack_size"}  # defined by the linker scripts
RELOC_TYPES = {"R_MIPS_32": 2, "R_MIPS_26": 4, "R_MIPS_HI16": 5, "R_MIPS_LO16": 6}
HEADER_POINTERS = (0x04, 0x0C, 0x14, 0x18, 0x1C, 0x20, 0x30, 0x34, 0x38)  # check_dll.c's SN_DLL_HEADER
ROOT_NAME = "sn_dll_header_root"
IMAGE = (0x100000, 0x2000000)  # EE addresses of the program and its data (32 MB RAM)


class Objects:
    """Where each object section starts in the original, and the objects' global symbols and import relocations."""

    def __init__(self, manifest: Path, imports: set[str]):
        self.ranges = []  # (start, end, object, section)
        self.globals = defaultdict(dict)  # (object, section) -> {offset: [names]}
        self.relocs = defaultdict(dict)  # (object, section) -> {offset: (type, symbol)}
        sections = defaultdict(dict)
        for line in manifest.read_text(encoding="utf-8").splitlines():
            obj, section, start = line.split()
            sections[obj][section] = int(start, 16)
        for obj, starts in sections.items():
            sizes = self._sizes(obj)
            for section, start in starts.items():
                if sizes.get(section):
                    self.ranges.append((start, start + sizes[section], obj, section))
            self._symbols(obj, set(starts))
            self._relocations(obj, set(starts), imports)
        self.ranges.sort()
        self.starts = [r[0] for r in self.ranges]

    @staticmethod
    def _run(*args) -> str:
        return subprocess.run(args, capture_output=True, text=True, check=True).stdout

    def _sizes(self, obj: str) -> dict[str, int]:
        sizes = {}
        for line in self._run(f"{CROSS}readelf", "-SW", obj).splitlines():
            parts = line.replace("[ ", "[").split()
            if len(parts) > 6 and parts[0].startswith("[") and parts[1].startswith("."):
                sizes[parts[1]] = int(parts[5], 16)
        return sizes

    def _symbols(self, obj: str, sections: set[str]) -> None:
        # nm can't tell the section of a symbol; readelf -s gives its section index, -S the names.
        names = {}
        for line in self._run(f"{CROSS}readelf", "-SW", obj).splitlines():
            m = re.match(r"\s*\[\s*(\d+)\]\s+(\S+)", line)
            if m:
                names[m.group(1)] = m.group(2)
        for line in self._run(f"{CROSS}readelf", "-sW", obj).splitlines():
            parts = line.split()
            if len(parts) < 8 or parts[4] not in ("GLOBAL", "WEAK") or not parts[6].isdigit():
                continue
            section = names.get(parts[6])
            if section in sections:
                self.globals[(obj, section)].setdefault(int(parts[1], 16), []).append(parts[7])

    def _relocations(self, obj: str, sections: set[str], imports: set[str]) -> None:
        current = None
        for line in self._run(f"{CROSS}readelf", "-rW", obj).splitlines():
            m = re.match(r"Relocation section '\.rel(\.\w+)'", line)
            if m:
                current = m.group(1)
                continue
            parts = line.split()
            if current in sections and len(parts) >= 5 and parts[2] in RELOC_TYPES and parts[4] in imports:
                self.relocs[(obj, current)][int(parts[0], 16)] = (RELOC_TYPES[parts[2]], parts[4])

    def locate(self, addr: int):
        i = bisect.bisect_right(self.starts, addr) - 1
        if i >= 0 and addr < self.ranges[i][1]:
            start, _, obj, section = self.ranges[i]
            return obj, section, addr - start
        return None

    def expression(self, addr: int, prefer: str | None = None) -> str | None:
        """A symbolic expression for an original address: a global symbol there (the preferred name if it is one),
        or the nearest global before it in the same object section + offset."""
        where = self.locate(addr)
        if not where:
            return None
        obj, section, offset = where
        table = self.globals.get((obj, section), {})
        here = table.get(offset, [])
        if here:
            return prefer if prefer in here else sorted(here)[0]
        before = [o for o in table if o < offset]
        if not before:
            return None
        o = max(before)
        return f"{sorted(table[o])[0]} + 0x{offset - o:X}"


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--rom", required=True)
    parser.add_argument("--manifest", required=True, help="lines `object section original_start`")
    parser.add_argument("--imports", required=True, help="dll_imports.ld")
    parser.add_argument("--start", required=True, type=lambda s: int(s, 0))
    parser.add_argument("--end", required=True, type=lambda s: int(s, 0))
    parser.add_argument("-o", "--output", required=True)
    parser.add_argument("--names", action="store_true",
                        help="also print each import whose source name differs from the table's (original) name")
    args = parser.parse_args()

    rom = Path(args.rom).read_bytes()
    start, end = args.start, args.end
    data = rom[start - BASE: end - BASE]
    word = lambda a: struct.unpack_from("<I", data, a - start)[0]
    imports = set(re.findall(r'^"([^"]+)" = 0;', Path(args.imports).read_text(encoding="utf-8"), re.M))
    objects = Objects(Path(args.manifest), imports)

    if data[:3] != b"SNR":
        sys.exit("error: no SN DLL header at the start")
    fields: dict[int, str] = {}  # offset in .sndata -> symbolic word
    errors = []

    def inside(value: int) -> str | None:
        return f"{ROOT_NAME} + 0x{value - start:X}" if start <= value < end else None

    for off in HEADER_POINTERS:
        value = word(start + off)
        if value:
            fields[off] = inside(value) or objects.expression(value) or errors.append(f"header +{off:#x}: {value:#x}")

    relocs, nrelocs, syms, nsyms = word(start + 4), word(start + 8), word(start + 12), word(start + 16)

    def cstr(addr: int) -> str:
        e = data.index(b"\0", addr - start)
        return data[addr - start: e].decode("latin-1")

    names = []
    for i in range(nsyms):
        entry = syms + 12 * i
        name_ptr, value = word(entry), word(entry + 4)
        typ = data[entry + 10 - start]
        name = cstr(name_ptr) if start <= name_ptr < end else None
        names.append(name)
        if name_ptr:
            fields[entry - start] = inside(name_ptr) or errors.append(f"symbol {i}: name pointer {name_ptr:#x}")
        if name in LINKER_SYMBOLS:
            fields[entry + 4 - start] = name
        elif typ in (1, 4) or not IMAGE[0] <= value < IMAGE[1]:
            pass  # imports (0), absolutes and values outside the image (VU micro memory: VU0_cbbox = 0x20) stay numbers
        else:
            expr = inside(value) or objects.expression(value, prefer=name)
            if expr is None:
                errors.append(f"symbol {i} {name}: no object symbol for {value:#x}")
            else:
                fields[entry + 4 - start] = expr

    renames = set()
    for i in range(nrelocs):
        entry = relocs + 12 * i
        site, info = word(entry), word(entry + 4)
        rtype, import_name = info & 0xFF, names[info >> 8]
        where = objects.locate(site)
        if not where:
            errors.append(f"reloc {i}: site {site:#x} ({import_name}) is in no object")
            continue
        obj, section, offset = where
        found = objects.relocs.get((obj, section), {}).get(offset)
        if not found or found[0] != rtype:
            errors.append(f"reloc {i}: {obj} {section}+{offset:#x} has no type-{rtype} relocation against a DLL "
                          f"import (table: {import_name}; object: {found})")
            continue
        expr = objects.expression(site)
        if expr is None:
            errors.append(f"reloc {i}: no global symbol before {obj} {section}+{offset:#x}")
            continue
        fields[entry - start] = expr
        if args.names and found[1] != import_name:
            renames.add((found[1], import_name, obj))

    for ours, original, obj in sorted(renames):
        print(f"name: {ours} -> {original}  ({obj})")
    if errors:
        for e in errors[:40]:
            print(f"error: {e}", file=sys.stderr)
        sys.exit(f"error: {len(errors)} fields of the DLL table could not be made symbolic")

    out = [f"# Generated by tools/sndll.py from {args.rom} and the link's objects - do not edit.",
           '.section .data, "wa"', f".globl {ROOT_NAME}", f"{ROOT_NAME}:"]
    off, size = 0, len(data)
    last_nonzero = max(i for i in range(size) if data[i]) + 1
    tail = size - ((last_nonzero + 3) & ~3)
    body_end = size - tail
    raw = []

    def flush():
        for k in range(0, len(raw), 8):
            out.append(".word " + ",".join(raw[k: k + 8]))
        raw.clear()

    while off < body_end:
        if off in fields:
            flush()
            out.append(f".word {fields[off]}")
        else:
            raw.append(f"0x{struct.unpack_from('<I', data, off)[0]:08X}")
        off += 4
    flush()
    if tail:
        out.append(f".space 0x{tail:X}")
    Path(args.output).parent.mkdir(parents=True, exist_ok=True)
    Path(args.output).write_text("\n".join(out) + "\n", encoding="utf-8")
    print(f"{args.output}: {len(fields)} symbolic words ({nsyms} symbols, {nrelocs} import relocations)")


if __name__ == "__main__":
    main()
