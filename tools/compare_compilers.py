#!/usr/bin/env python3
"""Compile a source file with every installed EE-GCC and compare each function with the original.

Uses the build's pipeline (compiler -> ee_asm_compat.py -> mips-linux-gnu-as) and the flags from
config/<VERSION>/build.yaml, and compares against objdiff's target object of the unit. Address fields
that are relocated are masked, so only the generated code is compared.

Usage:
    python3 tools/compare_compilers.py SLUS_217.99 src/System/Graphic/ColorFader.cpp
    python3 tools/compare_compilers.py SLUS_217.99 src/X.cpp --cflags "-O2 -G0"
"""

import argparse
import re
import subprocess
import sys
import tempfile
from pathlib import Path

import yaml
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

ROOT = Path(__file__).resolve().parent.parent
CROSS = "mips-linux-gnu-"
sys.path.insert(0, str(ROOT))
from configure import compiler_commands, unit_toolchain  # noqa: E402  (the build's exact commands)
# R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16, R_MIPS_PC16 (branches to global labels)
RELOC_MASKS = {4: 0x03FFFFFF, 5: 0xFFFF, 6: 0xFFFF, 10: 0xFFFF}


# Relocation targets that name the same thing differently in the original and in a compiled object: section
# symbols (a compiled file's own data: .rodata+offset) and splat's labels for unnamed data and code.
LOCAL_STATIC = re.compile(r"\.\d+$")
# splat's/the symbol file's suffix for a name that occurs more than once in the game (cardDataTbl_004F0B48)
ADDRESS_SUFFIX = re.compile(r"_[0-9A-F]{8}$")
UNNAMED_FUNCTION = re.compile(r"^func_[0-9A-F]{8}$")
ANONYMOUS = re.compile(r"^(\.|D_[0-9A-F]{8}$|jtbl_|L[0-9A-F]{8}$|\$L|func_[0-9A-F]{8}$)")


class Function:
    """A function's bytes plus the fields that are only filled in at link time, and what they refer to."""

    def __init__(self, data: bytes, masks: dict[int, int], targets: dict[int, str | None] | None = None):
        self.data, self.masks, self.targets = data, masks, targets or {}

    def __eq__(self, other: object) -> bool:
        return isinstance(other, Function) and self.difference(other) is None

    def difference(self, other: "Function") -> str | None:
        """None if equal; else what differs first. Masked fields must refer to the same symbol where both sides
        name one (e.g. a call to com_free, not __builtin_delete), since the linker fills them differently."""
        if len(self.data) != len(other.data):
            return f"size {len(self.data):#x} vs {len(other.data):#x}"
        masks = {**self.masks, **other.masks}
        for i in range(0, len(self.data), 4):
            a = int.from_bytes(self.data[i:i + 4], "little")
            b = int.from_bytes(other.data[i:i + 4], "little")
            if (a ^ b) & ~masks.get(i, 0) & 0xFFFFFFFF:
                return f"instruction at +{i:#x}"
            x, y = self.targets.get(i), other.targets.get(i)
            if x and y and x != y:
                return f"+{i:#x} refers to {x} vs {y}"
        return None


def target_name(name: str) -> str:
    """A referenced symbol's name without a local static's .<n> or a de-duplicated name's _<ADDR>."""
    return LOCAL_STATIC.sub("", ADDRESS_SUFFIX.sub("", name))


def pair_unnamed(original: dict[str, Function], ours: dict[str, Function]) -> dict[str, str]:
    """Original func_<ADDR> -> our function with the same bytes. The original object's unnamed copies of inline
    functions (the linker kept another copy's name) are compiled under their real names, which only our object has;
    each pairs with the first equal one of those, in order."""
    unpaired = [n for n in ours if n not in original]
    pairs = {}
    for name, fn in original.items():
        if UNNAMED_FUNCTION.match(name) and name not in ours and fn.data:
            match = next((n for n in unpaired if fn.difference(ours[n]) is None), None)
            if match:
                pairs[name] = match
                unpaired.remove(match)
    return pairs


def functions(obj: Path) -> dict[str, Function]:
    """Function name -> its .text bytes and relocated fields."""
    with obj.open("rb") as f:
        elf = ELFFile(f)
        text_index = next(i for i, s in enumerate(elf.iter_sections()) if s.name == ".text")
        text = elf.get_section(text_index).data()
        masks: dict[int, int] = {}
        targets: dict[int, str | None] = {}  # offset -> referenced symbol name (None: anonymous)
        for section in elf.iter_sections():
            if isinstance(section, RelocationSection) and section["sh_info"] == text_index:
                symtab = elf.get_section(section["sh_link"])
                for reloc in section.iter_relocations():
                    masks[reloc["r_offset"]] = RELOC_MASKS.get(reloc["r_info_type"], 0xFFFFFFFF)
                    sym = symtab.get_symbol(reloc["r_info_sym"])
                    # _gp: the original names that address after whatever symbol lies there.
                    named = (sym["st_info"]["type"] != "STT_SECTION" and not ANONYMOUS.match(sym.name)
                             and sym.name != "_gp")
                    # Function-local statics are named name.<n> after a gcc-internal counter, and names the game
                    # has more than once carry an _<ADDR> suffix in the original: compare the name only.
                    targets[reloc["r_offset"]] = target_name(sym.name) if named else None
        out = {}
        for sym in elf.get_section_by_name(".symtab").iter_symbols():
            if sym["st_info"]["type"] == "STT_FUNC" and sym["st_shndx"] == text_index:
                start, size = sym["st_value"], sym["st_size"]
                inside = lambda o: start <= o < start + size  # noqa: E731
                out[sym.name] = Function(text[start:start + size],
                                         {o - start: m for o, m in masks.items() if inside(o)},
                                         {o - start: n for o, n in targets.items() if inside(o)})
        return out


def compile_with(compiler: Path, source: Path, cflags: str, symbols: Path, out: Path,
                 assembler: str | None = None) -> str | None:
    """Same pipeline as configure.py's cc rule (assembler: a package whose bin/ee-as to use, as build.yaml's
    `assembler:`). Returns an error message, or None on success."""
    gcc_s, compat_s = out.with_suffix(".gcc.s"), out.with_suffix(".s")
    gp = re.search(r"-G\s*(\d+)", cflags)
    gp = f"-G{gp.group(1) if gp else 0}"
    cc, snas = compiler_commands(compiler.name)
    compat = [sys.executable, "tools/ee_asm_compat.py", str(gcc_s), str(compat_s), "--source", str(source),
              "--symbols", str(symbols)]
    if assembler:
        assemble = [f"tools/compilers/{assembler}/bin/ee-as", "-EL", gp, "-I", "include", "-o", str(out), str(compat_s)]
    elif snas:
        assemble = snas.split() + ["--stdout", gp, "-o", str(out), str(compat_s)]
        compat += ["--assembler", "snas"]
    else:
        assemble = [f"{CROSS}as", "-EL", "-march=r5900", "-mabi=eabi", "-no-pad-sections", "-I", "include",
                    gp, "-o", str(out), str(compat_s)]
    steps = [cc.split() + ["-S", *cflags.split(), "-I", "include", "-I", "src", "-o", str(gcc_s), str(source)],
             compat, assemble]
    for step in steps:
        result = subprocess.run(step, cwd=ROOT, capture_output=True, text=True)
        if result.returncode != 0 or (step is assemble and not out.exists()):
            message = (result.stderr or result.stdout).strip().splitlines()
            return message[-1][:120] if message else f"{Path(step[-6] if snas else step[0]).name} failed ({result.returncode})"
    return None


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("version")
    parser.add_argument("source", type=Path, help="e.g. src/System/Graphic/ColorFader.cpp")
    parser.add_argument("--cflags", help="override the unit's flags from build.yaml (its default or `overrides:`)")
    parser.add_argument("--assembler", help="package whose ee-as assembles every compiler's output (default: the unit's "
                        "`assembler:` from build.yaml; 'none': each compiler's own)")
    args = parser.parse_args()

    cfg_dir = ROOT / "config" / args.version
    opts = yaml.safe_load((cfg_dir / f"{args.version}.yaml").read_text(encoding="utf-8"))["options"]
    build_cfg = yaml.safe_load((cfg_dir / "build.yaml").read_text(encoding="utf-8"))
    unit = args.source.relative_to("src").with_suffix("").as_posix()
    tc = unit_toolchain(build_cfg, unit)
    cflags = args.cflags or tc["cflags"]
    assembler = None if args.assembler == "none" else args.assembler or tc.get("assembler")
    target_obj = ROOT / opts["build_path"] / "expected" / "units" / f"{unit}.o"
    if not target_obj.exists():
        subprocess.run(["ninja", target_obj.relative_to(ROOT).as_posix()], cwd=ROOT, check=True,
                       stdout=subprocess.DEVNULL)
    target = functions(target_obj)

    compilers = sorted(p for p in (ROOT / "tools" / "compilers").iterdir() if p.name.startswith("ee-gcc"))
    results = {}
    with tempfile.TemporaryDirectory() as tmp:
        for compiler in compilers:
            obj = Path(tmp) / f"{compiler.name}.o"
            error = compile_with(compiler, args.source, cflags, cfg_dir / "elf_symbol_addrs.txt", obj, assembler)
            results[compiler.name] = error or functions(obj)

    width = max(len(n) for n in target)
    print(f"{'function':<{width}}  " + "  ".join(f"{c.name[6:]:>14}" for c in compilers))
    for name in target:
        cells = []
        for compiler in compilers:
            got = results[compiler.name]
            cells.append("error" if isinstance(got, str) else "missing" if name not in got
                         else "MATCH" if got[name] == target[name] else "differs")
        print(f"{name:<{width}}  " + "  ".join(f"{c:>14}" for c in cells))
    for compiler in compilers:
        if isinstance(results[compiler.name], str):
            print(f"{compiler.name}: {results[compiler.name]}")
    print(f"(flags: {cflags}" + (f", assembler: {assembler} ee-as)" if assembler else ")"))


if __name__ == "__main__":
    main()
