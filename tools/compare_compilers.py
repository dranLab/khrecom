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
from configure import compiler_commands  # noqa: E402  (the build's exact commands)
# R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16, R_MIPS_PC16 (branches to global labels)
RELOC_MASKS = {4: 0x03FFFFFF, 5: 0xFFFF, 6: 0xFFFF, 10: 0xFFFF}


class Function:
    """A function's bytes plus the fields that are only filled in at link time."""

    def __init__(self, data: bytes, masks: dict[int, int]):
        self.data, self.masks = data, masks

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, Function) or len(self.data) != len(other.data):
            return False
        masks = {**self.masks, **other.masks}
        for i in range(0, len(self.data), 4):
            a = int.from_bytes(self.data[i:i + 4], "little")
            b = int.from_bytes(other.data[i:i + 4], "little")
            if (a ^ b) & ~masks.get(i, 0) & 0xFFFFFFFF:
                return False
        return True


def functions(obj: Path) -> dict[str, Function]:
    """Function name -> its .text bytes and relocated fields."""
    with obj.open("rb") as f:
        elf = ELFFile(f)
        text_index = next(i for i, s in enumerate(elf.iter_sections()) if s.name == ".text")
        text = elf.get_section(text_index).data()
        masks: dict[int, int] = {}
        for section in elf.iter_sections():
            if isinstance(section, RelocationSection) and section["sh_info"] == text_index:
                for reloc in section.iter_relocations():
                    masks[reloc["r_offset"]] = RELOC_MASKS.get(reloc["r_info_type"], 0xFFFFFFFF)
        out = {}
        for sym in elf.get_section_by_name(".symtab").iter_symbols():
            if sym["st_info"]["type"] == "STT_FUNC" and sym["st_shndx"] == text_index:
                start, size = sym["st_value"], sym["st_size"]
                out[sym.name] = Function(text[start:start + size],
                                         {o - start: m for o, m in masks.items() if start <= o < start + size})
        return out


def compile_with(compiler: Path, source: Path, cflags: str, symbols: Path, out: Path) -> str | None:
    """Same pipeline as configure.py's cc rule. Returns an error message, or None on success."""
    gcc_s, compat_s = out.with_suffix(".gcc.s"), out.with_suffix(".s")
    cc, snas = compiler_commands(compiler.name)
    compat = [sys.executable, "tools/ee_asm_compat.py", str(gcc_s), str(compat_s), "--source", str(source),
              "--symbols", str(symbols)]
    if snas:
        assemble = snas.split() + ["--stdout", "-G0", "-o", str(out), str(compat_s)]
        compat += ["--assembler", "snas"]
    else:
        assemble = [f"{CROSS}as", "-EL", "-march=r5900", "-mabi=eabi", "-no-pad-sections", "-I", "include",
                    "-G0", "-o", str(out), str(compat_s)]
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
    parser.add_argument("--cflags", help="override the flags from build.yaml")
    args = parser.parse_args()

    cfg_dir = ROOT / "config" / args.version
    opts = yaml.safe_load((cfg_dir / f"{args.version}.yaml").read_text(encoding="utf-8"))["options"]
    build_cfg = yaml.safe_load((cfg_dir / "build.yaml").read_text(encoding="utf-8"))
    cflags = args.cflags or build_cfg["cflags"]
    unit = args.source.relative_to("src").with_suffix("").as_posix()
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
            error = compile_with(compiler, args.source, cflags, cfg_dir / "elf_symbol_addrs.txt", obj)
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
    print(f"(flags: {cflags})")


if __name__ == "__main__":
    main()
