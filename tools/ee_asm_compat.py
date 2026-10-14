#!/usr/bin/env python3
"""Post-process EE-GCC's assembly output before assembling it.

Always:
- Anonymous namespaces: gcc names them after the source path plus six random characters
  (_58_GLOBAL_$N$c__KHCOM_MainApp_System_PlaySpeedCtrl.cppM1aaaa). With --symbols, the name is
  replaced by the original one for this source file, so objects get the original symbol names
  (objdiff can pair them) and builds are reproducible. Likewise the global constructor/destructor that
  runs a file's static initializers (_GLOBAL_$I$c__KHCOM_MainApp_main.cpp00aaaa, _GLOBAL_$D$...), which
  the original __CTOR_LIST__/__DTOR_LIST__ reference by name.
- Source paths in string literals (__FILE__ in assert/allocation messages): with --source-root, the
  "src/" prefix becomes the original one (e.g. "c:/KHCOM/MainApp/").
- Weak data (vtables of classes whose virtual functions are all inline): with --symbols and --own-data, a copy the
  original object doesn't contain is dropped (see drop_foreign_weak_data).

--assembler gas (mips-linux-gnu-as):
- cvt.w.s: the EE only truncates, so binutils rejects the instruction for r5900. Emit its encoding.

--assembler snas (SN Systems' Ps2EeAs, the original assembler):
- Short loops in compiled code: our Ps2EeAs pads them (EE erratum) to 6 instructions label..branch, the
  one that built the game to 5. fix_short_loops inserts the original padding and hides the branch from
  Ps2EeAs (see there).
- INCLUDE_ASM'd splat assembly is inlined, because Ps2EeAs can't read splat's macro file: its macros
  (glabel, endlabel, dlabel, ...) are expanded into plain directives.
- In that inlined code, instructions are emitted as .word with their original encoding (from
  splat's comments), except those that need a relocation (%hi/%lo, jal/j to symbols). Ps2EeAs pads
  divisions near labels and short loops with nops (EE erratum workarounds) even in noreorder code,
  but the original code already contains those nops. Branches are PC-relative, so their encodings
  stay valid as long as nothing moves.

Usage (from configure.py's cc rule):
    ee_asm_compat.py IN.s OUT.s [--assembler gas|snas] [--source src/X.cpp --symbols elf_symbol_addrs.txt]
                   [--own-data 0xSTART-0xEND,...]
"""

import argparse
import re
from pathlib import Path, PurePosixPath

CVT_W_S = re.compile(r"^(\s*)cvt\.w\.s\s+\$f(\d+)\s*,\s*\$f(\d+)\s*$")
ANON_NAMESPACE = re.compile(r"_\d+(_GLOBAL_\$N\$[\w.]*?\.(?:cpp|cc|c)[A-Za-z0-9]{6})(?=\$|\W|$)")
GLOBAL_CDTOR = re.compile(r"_GLOBAL_\$([ID])\$[\w.]*?\.(?:cpp|cc|c)[A-Za-z0-9]{6}(?=\$|\W|$)")
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


LABEL = re.compile(r"^(\$L\d+):\s*$")
INSN = re.compile(r"^\t([a-z][\w.]*)\b(.*)$")  # an instruction line of gcc's output (not a directive)
SET_REORDER = re.compile(r"^\t\.set\t(no)?reorder\s*$")
BRANCH = re.compile(r"^\t(b|beqz?l?|bnez?l?|blezl?|bgtzl?|bltzl?|bgezl?|bc1[tf]l?)\t(?:(.*),)?(\$L\d+)\s*$")
REGS = {"zero": 0, "at": 1, "gp": 28, "sp": 29, "fp": 30, "ra": 31}
BRANCH_OPCODES = {  # mnemonic: (opcode, fixed rt or None, number of register operands)
    "beq": (4, None, 2), "bne": (5, None, 2), "beql": (20, None, 2), "bnel": (21, None, 2),
    "beqz": (4, 0, 1), "bnez": (5, 0, 1), "beqzl": (20, 0, 1), "bnezl": (21, 0, 1),
    "blez": (6, 0, 1), "bgtz": (7, 0, 1), "blezl": (22, 0, 1), "bgtzl": (23, 0, 1),
    "bltz": (1, 0, 1), "bgez": (1, 1, 1), "bltzl": (1, 2, 1), "bgezl": (1, 3, 1),
}
COP1_BRANCHES = {"bc1f": 0x45000000, "bc1t": 0x45010000, "bc1fl": 0x45020000, "bc1tl": 0x45030000}
SHORT_LOOP = 5  # the original pads label..branch to 5 instructions


def reg(name: str) -> int:
    name = name.strip().lstrip("$")
    return int(name) if name.isdigit() else REGS[name]


def branch_word(mnemonic: str, operands: str | None) -> int:
    """Encoding of a branch without its offset (the low 16 bits)."""
    if mnemonic in COP1_BRANCHES:
        return COP1_BRANCHES[mnemonic]
    if mnemonic == "b":
        return 4 << 26  # beq $0,$0
    opcode, rt, nregs = BRANCH_OPCODES[mnemonic]
    regs = [reg(r) for r in (operands or "").split(",")]
    if len(regs) != nregs:
        raise ValueError(f"unexpected operands for {mnemonic}: {operands}")
    return (opcode << 26) | (regs[0] << 21) | ((regs[1] if nregs == 2 else rt) << 16)


def fix_short_loops(lines: list[str]) -> list[str]:
    """Pad short loops the way the original assembler did.

    The EE needs short loops padded with nops (an erratum). The Ps2EeAs we have pads the label..branch
    span to 6 instructions; the one that built the game padded it to 5 (all 1867 short loops of the game's
    own code agree). For each backward branch that closes a short loop, insert the original padding before
    the branch (counted by the assembler, so macro instructions are sized correctly) and emit the branch as
    a .word whose offset the assembler computes from labels, so Ps2EeAs doesn't recognize the loop and adds
    no padding of its own. A branch in reorder code (gcc left its delay slot to the assembler) gets the
    delay slot nop the assembler would have added."""
    out: list[str] = []
    labels: dict[str, int] = {}  # label -> index in out
    noreorder = False
    n = 0
    for i, line in enumerate(lines):
        if m := SET_REORDER.match(line):
            noreorder = m.group(1) == "no"
        elif line.startswith("\t.ent\t"):
            labels.clear()
        elif m := LABEL.match(line):
            labels[m.group(1)] = len(out)
        elif (m := BRANCH.match(line)) and m.group(3) in labels:
            body = [l for l in out[labels[m.group(3)] + 1:] if INSN.match(l)]
            if len(body) + 1 <= SHORT_LOOP + 1:  # macros only make loops longer
                label, pad, at = m.group(3), f"$LSLp{n}", f"$LSLb{n}"
                count = f"{SHORT_LOOP} - (({pad} - {label}) >> 2) - 1"
                word = f"\t.word 0x{branch_word(m.group(1), m.group(2)):08X} | ((({label} - {at} - 4) >> 2) & 0xFFFF)"
                out += ([] if noreorder else ["\t.set\tnoreorder"]) + [
                    f"{pad}:", f"\t.if {count} > 0", f"\t.rept {count}", "\tnop", "\t.endr", "\t.endif",
                    f"{at}:", f"{word} # {line.strip()}"] + ([] if noreorder else ["\tnop", "\t.set\treorder"])
                n += 1
                continue
        out.append(line)
    return out


WEAK = re.compile(r"^\t\.weak\t(\S+)\s*$")
DATA_SECTION = re.compile(r"^\t\.(?:rdata|data|sdata|bss|sbss)\s*$|^\t\.section\s+\.(?:rodata|data|sdata|bss|sbss)\b")
OTHER_SECTION = re.compile(r"^\t\.(?:text|section)\b")
DATA_DIRECTIVE = re.compile(r"^\t\.(?:byte|half|word|dword|float|double|space|ascii|asciz)\b")


def symbol_addresses(symbols_path: str, names: set[str]) -> dict[str, int]:
    addrs = {}
    with open(symbols_path, encoding="utf-8") as f:
        for line in f:
            if (m := re.match(r"^(\S+) = 0x([0-9A-Fa-f]+);", line)) and m.group(1) in names:
                addrs[m.group(1)] = int(m.group(2), 16)
    return addrs


def drop_foreign_weak_data(lines: list[str], symbols_path: str, own_data: str) -> list[str]:
    """Remove weak data definitions the original object doesn't contain.

    gcc emits the vtable of a class whose virtual functions are all inline (and other weak data) into every object
    that needs it. The original linker kept one copy, at the address the symbol file gives; in every other object
    it is extra data, and in a section that doesn't come from the source (removed by objcopy) a missing symbol. So a
    weak object whose original address lies outside this object's data from source (own_data: "0xSTART-0xEND,...",
    or "none") loses its definition and the references resolve to the original copy."""
    own = [tuple(int(x, 16) for x in r.split("-")) for r in own_data.split(",") if r != "none"]
    weak = {m.group(1) for line in lines if (m := WEAK.match(line))}
    if not weak:
        return lines
    addrs = symbol_addresses(symbols_path, weak)
    foreign = {name for name, addr in addrs.items() if not any(lo <= addr < hi for lo, hi in own)}
    in_data, data_labels = False, set()
    for line in lines:
        if DATA_SECTION.match(line):
            in_data = True
        elif OTHER_SECTION.match(line):
            in_data = False
        elif in_data and line.endswith(":") and line[:-1] in foreign:
            data_labels.add(line[:-1])
    if not data_labels:
        return lines
    out: list[str] = []
    dropping = False
    for line in lines:
        if dropping:
            if DATA_DIRECTIVE.match(line):
                continue
            dropping = False
        if (m := WEAK.match(line)) and m.group(1) in data_labels:
            continue
        if (m := re.match(r"^\t\.(?:type|size)\t\s*([^,\s]+),", line)) and m.group(1) in data_labels:
            if line.startswith("\t.type"):  # the object's alignment comes just before it
                for i in range(len(out) - 1, -1, -1):
                    if out[i].startswith("\t.align"):
                        del out[i]
                        break
                    if not out[i].lstrip().startswith(".stabs"):
                        break
            continue
        if line.endswith(":") and line[:-1] in data_labels:
            dropping = True
            continue
        out.append(line)
    return out


def original_global(kind: str, source: str, symbols_path: str) -> str | None:
    """The name of gcc's per-file global _GLOBAL_$<kind>$ (N: anonymous namespace, I/D: global
    constructor/destructor) in the original object of this source file."""
    rel = PurePosixPath(source)
    rel = rel.relative_to("src") if rel.parts[0] == "src" else rel
    stem = re.escape("_" + "_".join(rel.parts))
    # A namespace name is followed by its member's name ($...), a constructor's name ends there.
    end = r"(?=\$)" if kind == "N" else r"(?![\w.$])"
    pattern = re.compile(rf"_GLOBAL_\${kind}\$[\w.]*{stem}[A-Za-z0-9]{{6}}{end}")
    with open(symbols_path, encoding="utf-8") as f:
        for line in f:
            if m := pattern.search(line):
                return m.group(0)
    return None


def expand_macro(kind: str, name: str, arg: str | None) -> list[str]:
    """Plain directives for one of splat's macros. No .size: Ps2EeAs crashes on it inside .ent/.end
    and derives function sizes from .ent/.end itself. No .aent for alabel either (splat's func_ labels at
    jump targets inside a function): Ps2EeAs crashes on it too."""
    visible = arg != "local"
    if kind == "glabel":
        return ([f"\t.globl {name}"] if visible else []) + [f"{name}:", f"\t.ent {name}"]
    if kind in ("alabel", "ehlabel", "jlabel", "dlabel"):
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
    parser.add_argument("--own-data", help='address ranges of the data from source ("0xSTART-0xEND,..." or "none")')
    args = parser.parse_args()

    with open(args.input, encoding="latin-1") as f:
        text = f.read()
    if args.source_root:
        text = re.sub(r'^(\s*\.ascii\s+")src/', lambda m: m.group(1) + args.source_root, text, flags=re.MULTILINE)
    if args.source and args.symbols and "_GLOBAL_$N$" in text:
        name = original_global("N", args.source, args.symbols)
        if name:
            text = ANON_NAMESPACE.sub(lambda _: f"_{len(name)}{name}", text)
    if args.source and args.symbols and ("_GLOBAL_$I$" in text or "_GLOBAL_$D$" in text):
        names = {kind: original_global(kind, args.source, args.symbols) for kind in "ID"}
        text = GLOBAL_CDTOR.sub(lambda m: names[m.group(1)] or m.group(0), text)

    lines = []
    source_lines = text.splitlines()
    if args.symbols and args.own_data:
        source_lines = drop_foreign_weak_data(source_lines, args.symbols, args.own_data)
    if args.assembler == "snas":
        source_lines = fix_short_loops(source_lines)  # compiled code only: INCLUDE_ASM'd code is inlined below
    for line in source_lines:
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
