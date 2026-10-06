#!/usr/bin/env python3
"""Post-process EE-GCC's assembly output before assembling it with modern binutils.

- cvt.w.s: the EE only truncates, so binutils rejects the instruction for r5900; Sony's assembler
  accepted it. Emit the standard encoding as a .word instead.
- Anonymous namespaces: gcc names them after the source path plus six random characters
  (_58_GLOBAL_$N$c__KHCOM_MainApp_System_PlaySpeedCtrl.cppM1aaaa). With --symbols, the name is
  replaced by the original one for this source file, so objects get the original symbol names
  (objdiff can pair them) and builds are reproducible.

Usage (from configure.py's cc rule):
    ee_asm_compat.py IN.s OUT.s [--source src/X.cpp --symbols config/<VERSION>/elf_symbol_addrs.txt]
"""

import argparse
import re
from pathlib import PurePosixPath

CVT_W_S = re.compile(r"^(\s*)cvt\.w\.s\s+\$f(\d+)\s*,\s*\$f(\d+)\s*$")
ANON_NAMESPACE = re.compile(r"_\d+(_GLOBAL_\$N\$[\w.]*?\.(?:cpp|cc|c)[A-Za-z0-9]{6})(?=\$|\W|$)")


def fix_cvt_w_s(line: str) -> str:
    if m := CVT_W_S.match(line):
        fd, fs = int(m.group(2)), int(m.group(3))
        word = 0x46000024 | (fs << 11) | (fd << 6)  # COP1, fmt=S, funct=CVT.W
        return f"{m.group(1)}.word 0x{word:08X} # cvt.w.s $f{fd},$f{fs}"
    return line


def original_namespace(source: str, symbols_path: str) -> str | None:
    """The mangled anonymous namespace name the original object used for this source file."""
    rel = PurePosixPath(source)
    rel = rel.relative_to("src") if rel.parts[0] == "src" else rel
    stem = re.escape("_" + "_".join(rel.parts))
    pattern = re.compile(rf"_GLOBAL_\$N\$[\w.]*{stem}[A-Za-z0-9]{{6}}(?=\$)")
    with open(symbols_path, encoding="utf-8") as f:
        for line in f:
            if m := pattern.search(line):
                return m.group(0)
    return None


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("input")
    parser.add_argument("output")
    parser.add_argument("--source", help="the compiled source file (for the anonymous namespace name)")
    parser.add_argument("--symbols", help="symbol file with the original names")
    args = parser.parse_args()

    with open(args.input, encoding="latin-1") as f:
        text = f.read()
    if args.source and args.symbols and "_GLOBAL_$N$" in text:
        name = original_namespace(args.source, args.symbols)
        if name:
            text = ANON_NAMESPACE.sub(lambda _: f"_{len(name)}{name}", text)
    lines = [fix_cvt_w_s(line) for line in text.splitlines()]
    with open(args.output, "w", encoding="latin-1", newline="\n") as f:
        f.write("\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
