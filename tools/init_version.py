#!/usr/bin/env python3
"""Create config/<VERSION>/ for a game ELF using `splat create_config`.

splat writes its files next to the ELF with paths relative to the current
directory; this script runs it in a temporary directory, rewrites the paths to
this repo's layout and moves the results into place:

    config/<VERSION>/<VERSION>.yaml     splat config (segments, options)
    config/<VERSION>/symbol_addrs.txt   known symbols (grows as you decompile)
    config/<VERSION>/reloc_addrs.txt    manual relocation overrides
    config/<VERSION>/linker_script_extra.ld
    config/<VERSION>/build.yaml         compiler + flags used by configure.py
    iso/<VERSION>.rom                   objcopy'd code image splat splits (gitignored)

Needs mips-linux-gnu-objcopy on PATH, so run it inside WSL/Linux.

Usage:
    python tools/init_version.py iso/SLUS_217.99
"""

import argparse
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

DEFAULT_COMPILERS = {
    "EEGCC": "ee-gcc2.96",
    "MWCCPS2": "mwcps2-3.0.1b145-050209",
}


def rewrite(yaml: str, version: str, name: str) -> str:
    cfg = f"config/{version}"
    replacements = [
        (r"^# name: .*$", f"name: Kingdom Hearts Re:Chain of Memories ({version})"),
        (r"^  base_path: \.$", "  base_path: ../.."),
        (r"^  target_path: .*$", f"  target_path: iso/{name}.rom"),
        (r"^  elf_path: .*$", f"  elf_path: build/{version}/{name}.elf"),
        (r"^  # asm_path: asm$", f"  asm_path: asm/{version}"),
        (r"^  # src_path: src$", "  src_path: src"),
        (r"^  # build_path: build$", f"  build_path: build/{version}"),
        (r"^  ld_script_path: .*$", f"  ld_script_path: build/{version}/{name}.ld"),
        (r"^    - symbol_addrs\.txt$", f"    - {cfg}/symbol_addrs.txt"),
        (r"^    - reloc_addrs\.txt$", f"    - {cfg}/reloc_addrs.txt"),
        (r"^  # undefined_funcs_auto_path: .*$", f"  undefined_funcs_auto_path: {cfg}/undefined_funcs_auto.txt"),
        (r"^  # undefined_syms_auto_path: .*$", f"  undefined_syms_auto_path: {cfg}/undefined_syms_auto.txt"),
        # Padding after the last loaded section would grow the image; configure.py handles section alignment.
        (r"^  ld_bss_contains_common: True$", "  ld_bss_contains_common: True\n  ld_align_section_vram_end: False"),
    ]
    for pattern, replacement in replacements:
        yaml, count = re.subn(pattern, replacement, yaml, count=1, flags=re.MULTILINE)
        if count != 1:
            sys.exit(f"error: splat's generated yaml changed; pattern {pattern!r} not found")
    return yaml


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("elf", type=Path, help="the game's boot ELF, e.g. iso/SLUS_217.99")
    parser.add_argument("--version", help="version id (default: ELF file name)")
    parser.add_argument("--force", action="store_true", help="overwrite an existing config/<VERSION>/")
    args = parser.parse_args()

    elf = args.elf.resolve()
    name = elf.name.replace(" ", "")
    version = args.version or name
    cfg_dir = ROOT / "config" / version
    if cfg_dir.exists() and not args.force:
        sys.exit(f"error: {cfg_dir.relative_to(ROOT)} exists (use --force to overwrite)")

    with tempfile.TemporaryDirectory() as tmp:
        tmp_dir = Path(tmp)
        subprocess.run([sys.executable, "-m", "splat", "create_config", str(elf)], cwd=tmp_dir, check=True)

        yaml = (tmp_dir / f"{name}.yaml").read_text(encoding="utf-8")
        compiler = re.search(r"^  compiler: (\w+)$", yaml, re.MULTILINE).group(1)

        cfg_dir.mkdir(parents=True, exist_ok=True)
        (cfg_dir / f"{version}.yaml").write_text(rewrite(yaml, version, name), encoding="utf-8", newline="\n")
        for extra in ("symbol_addrs.txt", "linker_script_extra.ld"):
            shutil.copy(tmp_dir / extra, cfg_dir / extra)
        (ROOT / "iso").mkdir(exist_ok=True)
        shutil.copy(tmp_dir / f"{name}.rom", ROOT / "iso" / f"{name}.rom")

    (cfg_dir / "reloc_addrs.txt").touch()
    build_yaml = cfg_dir / "build.yaml"
    if not build_yaml.exists():
        elf_rel = elf.relative_to(ROOT).as_posix() if elf.is_relative_to(ROOT) else elf.as_posix()
        build_yaml.write_text(
            "# Read by configure.py.\n"
            f"elf: {elf_rel}            # original ELF (not committed)\n"
            f"compiler: {DEFAULT_COMPILERS.get(compiler, 'ee-gcc2.96')}   # folder under tools/compilers/ (splat guessed {compiler})\n"
            "cflags: -O2 -G0\n",
            encoding="utf-8",
            newline="\n",
        )

    print(f"\ncreated {cfg_dir.relative_to(ROOT)}/ - next: python3 configure.py {version}")


if __name__ == "__main__":
    main()
