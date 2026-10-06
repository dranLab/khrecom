#!/usr/bin/env python3
"""Post-process EE-GCC's assembly output before assembling it.

Always:
- Anonymous namespaces: gcc names them after the source path plus six random characters
  (_58_GLOBAL_$N$c__KHCOM_MainApp_System_PlaySpeedCtrl.cppM1aaaa). With --symbols, the name is
  replaced by the original one for this source file, so objects get the original symbol names
  (objdiff can pair them) and builds are reproducible.
- Source paths in string literals (__FILE__ in assert/allocation messages): with --source-root, the
  "src/" prefix becomes the original one (e.g. "c:/KHCOM/MainApp/").

--assembler gas (mips-linux-gnu-as):
- cvt.w.s: the EE only truncates, so binutils rejects the instruction for r5900. Emit its encoding.

--assembler snas (SN Systems' Ps2EeAs, the original assembler):
- INCLUDE_ASM'd splat assembly is inlined, because Ps2EeAs can't read splat's macro file: its macros
  (glabel, endlabel, dlabel, ...) are expanded into plain directives.
- In that inlined code, instructions are emitted as .word with their original encoding (from
  splat's comments), except those that need a relocation (%hi/%lo, jal/j to symbols). Ps2EeAs pads
  divisions near labels and short loops with nops (EE erratum workarounds) even in noreorder code,
  but the original code already contains those nops. Branches are PC-relative, so their encodings
  stay valid as long as nothing moves.

Usage (from configure.py's cc rule):
    ee_asm_compat.py IN.s OUT.s [--assembler gas|snas] [--source src/X.cpp --symbols elf_symbol_addrs.txt]
"""

import argparse
import re
from pathlib import Path, PurePosixPath

CVT_W_S = re.compile(r"^(\s*)cvt\.w\.s\s+\$f(\d+)\s*,\s*\$f(\d+)\s*$")
ANON_NAMESPACE = re.compile(r"_\d+(_GLOBAL_\$N\$[\w.]*?\.(?:cpp|cc|c)[A-Za-z0-9]{6})(?=\$|\W|$)")
INCLUDE = re.compile(r'^\s*\.include\s+"([^"]+)"\s*$')
MACRO = re.compile(r'^\s*(glabel|alabel|ehlabel|jlabel|dlabel|endlabel|enddlabel|nonmatching)\s+"?([^",\s]+)"?\s*(?:,\s*(\S+))?\s*$')
SPLAT_INSN = re.compile(r"^\s*/\* [0-9A-F]+ [0-9A-F]+ ([0-9A-F]{8}) \*/\s+(\S+)")
SECTION = re.compile(r'^\s*\.section\s+(\.\w+)\s*,\s*"[a-z]*"\s*$')
ABSOLUTE_JUMPS = {"j", "jal"}  # J-type: the target is relocated (R_MIPS_26)


def needs_relocation(mnemonic: str, line: str) -> bool:
    operands = line.split("*/", 1)[1].split(None, 1)[1] if len(line.split("*/", 1)[1].split(None, 1)) > 1 else ""
    return "%" in operands or mnemonic in ABSOLUTE_JUMPS or mnemonic.startswith(".")


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


def expand_macro(kind: str, name: str, arg: str | None) -> list[str]:
    """Plain directives for one of splat's macros. No .size: Ps2EeAs crashes on it inside .ent/.end
    and derives function sizes from .ent/.end itself."""
    visible = arg != "local"
    if kind in ("glabel", "alabel"):
        return ([f"\t.globl {name}"] if visible else []) + [f"{name}:", f"\t.{'ent' if kind == 'glabel' else 'aent'} {name}"]
    if kind in ("ehlabel", "jlabel", "dlabel"):
        return ([f"\t.globl {name}"] if visible else []) + [f"{name}:"]
    if kind == "endlabel":
        return [f"\t.end {name}"]
    return []  # enddlabel, and nonmatching (only a marker symbol for splat's tooling)


def inline_splat_asm(path: Path) -> list[str]:
    """splat's assembly in a form Ps2EeAs accepts (see the module docstring)."""
    out = []
    in_delay_slot = False  # Ps2EeAs crashes on a .word in the delay slot of a real jump
    for line in path.read_text(encoding="utf-8").splitlines():
        if m := INCLUDE.match(line):
            if m.group(1).endswith(".s"):
                out += inline_splat_asm(Path(m.group(1)))
            continue  # macro.inc / labels.inc
        if m := MACRO.match(line):
            out += expand_macro(*m.groups())
        elif m := SPLAT_INSN.match(line):
            if in_delay_slot or needs_relocation(m.group(2), line):
                out.append(line)
                in_delay_slot = m.group(2) in ABSOLUTE_JUMPS
            else:
                word = int.from_bytes(bytes.fromhex(m.group(1)), "little")
                out.append(f"\t.word 0x{word:08X} # {line.split('*/', 1)[1].strip()}")
                in_delay_slot = False
        elif m := SECTION.match(line):
            out.append(f"\t.section {m.group(1)}")
        else:
            out.append(line)
    return out


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("input")
    parser.add_argument("output")
    parser.add_argument("--assembler", choices=("gas", "snas"), default="gas")
    parser.add_argument("--source", help="the compiled source file (for the anonymous namespace name)")
    parser.add_argument("--symbols", help="symbol file with the original names")
    parser.add_argument("--source-root", help='original path prefix for "src/" in string literals')
    args = parser.parse_args()

    with open(args.input, encoding="latin-1") as f:
        text = f.read()
    if args.source_root:
        text = re.sub(r'^(\s*\.ascii\s+")src/', lambda m: m.group(1) + args.source_root, text, flags=re.MULTILINE)
    if args.source and args.symbols and "_GLOBAL_$N$" in text:
        name = original_namespace(args.source, args.symbols)
        if name:
            text = ANON_NAMESPACE.sub(lambda _: f"_{len(name)}{name}", text)

    lines = []
    for line in text.splitlines():
        if args.assembler == "gas":
            lines.append(fix_cvt_w_s(line))
        elif m := INCLUDE.match(line):
            if m.group(1).endswith(".s"):
                lines += inline_splat_asm(Path(m.group(1)))
            # else: splat's macro.inc / labels.inc, expanded above instead
        else:
            lines.append(line)
    with open(args.output, "w", encoding="latin-1", newline="\n") as f:
        f.write("\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
