#!/usr/bin/env python3
"""Print facts about a PS2 ELF that matter when starting a decompilation.

- sections and program headers
- whether symbols / debug info survived (a huge head start if they did)
- compiler and SDK fingerprints (GCC version strings, Metrowerks, SN Systems, libsce versions)

Usage:
    python tools/elf_info.py iso/SLUS_217.99
"""

import argparse
import hashlib
import re
from collections import Counter
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.sections import SymbolTableSection

FINGERPRINTS = [
    ("GCC version string", rb"GCC: \(GNU\) [\w.\- ()]+"),
    ("EE-GCC version", rb"\d\.\d+[\w.]*-ee-\d{6}[\w-]*"),
    ("SN Systems / ProDG", rb"SN (?:Systems|Compiler)[^\x00]{0,60}|ProDG[^\x00]{0,60}"),
    ("Metrowerks / CodeWarrior", rb"Metrowerks[^\x00]{0,60}|CodeWarrior[^\x00]{0,60}|MWCC[^\x00]{0,40}"),
    ("Sony libsce version", rb"PsIIlib\w+\s+\d{4}"),
    ("Source file name", rb"[\w/\\.-]{2,80}\.(?:c|cc|cpp|cxx|s|S|vsm|dsm)\x00"),
]


def show_fingerprints(data: bytes) -> None:
    print("\n== Compiler / SDK fingerprints ==")
    found_any = False
    for label, pattern in FINGERPRINTS:
        hits = Counter(m.group(0).rstrip(b"\x00").decode("latin-1") for m in re.finditer(pattern, data))
        if not hits:
            continue
        found_any = True
        print(f"{label}:")
        for text, count in hits.most_common(15):
            print(f"  {text!r}" + (f"  (x{count})" if count > 1 else ""))
        if len(hits) > 15:
            print(f"  ... and {len(hits) - 15} more")
    if not found_any:
        print("  none found - compare codegen patterns on decomp.me instead")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("elf", type=Path)
    args = parser.parse_args()

    data = args.elf.read_bytes()
    print(f"file:  {args.elf}")
    print(f"size:  {len(data):#x}")
    print(f"sha1:  {hashlib.sha1(data).hexdigest()}")

    with args.elf.open("rb") as f:
        elf = ELFFile(f)
        print(f"entry: {elf.header.e_entry:#010x}")
        print(f"flags: {elf.header.e_flags:#010x}")

        print("\n== Program headers ==")
        print(f"  {'type':<10} {'offset':>10} {'vaddr':>10} {'filesz':>10} {'memsz':>10}")
        for seg in elf.iter_segments():
            h = seg.header
            print(f"  {h.p_type:<10} {h.p_offset:>#10x} {h.p_vaddr:>#10x} {h.p_filesz:>#10x} {h.p_memsz:>#10x}")

        print("\n== Sections ==")
        print(f"  {'name':<20} {'type':<16} {'addr':>10} {'offset':>10} {'size':>10}")
        for sec in elf.iter_sections():
            if not sec.name:
                continue
            h = sec.header
            print(f"  {sec.name:<20} {h.sh_type:<16} {h.sh_addr:>#10x} {h.sh_offset:>#10x} {h.sh_size:>#10x}")

        print("\n== Symbols / debug info ==")
        symtabs = [s for s in elf.iter_sections() if isinstance(s, SymbolTableSection)]
        if not symtabs:
            print("  no symbol table (stripped)")
        for symtab in symtabs:
            funcs = sum(1 for s in symtab.iter_symbols() if s["st_info"]["type"] == "STT_FUNC")
            print(f"  {symtab.name}: {symtab.num_symbols()} symbols, {funcs} functions")
        for name in (".mdebug", ".debug", ".debug_info", ".comment"):
            sec = elf.get_section_by_name(name)
            if sec is not None:
                print(f"  {name} present ({sec.header.sh_size:#x} bytes)")
        gp = next((s for t in symtabs for s in t.iter_symbols() if s.name == "_gp"), None)
        if gp is not None:
            print(f"  _gp = {gp['st_value']:#010x}")

    show_fingerprints(data)


if __name__ == "__main__":
    main()
