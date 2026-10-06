#!/usr/bin/env python3
"""Extract the boot ELF (and optionally every file) from a PS2 disc image.

The boot ELF is found via the BOOT2 line in SYSTEM.CNF, e.g.
    BOOT2 = cdrom0:\\SLUS_217.99;1

Usage:
    python tools/extract_iso.py iso/game.iso
    python tools/extract_iso.py iso/game.iso --all iso/files
"""

import argparse
import hashlib
import re
import sys
from io import BytesIO
from pathlib import Path

import pycdlib


def read_file(iso: pycdlib.PyCdlib, iso_path: str) -> bytes:
    buf = BytesIO()
    iso.get_file_from_iso_fp(buf, iso_path=iso_path)
    return buf.getvalue()


def find_boot_elf(system_cnf: str) -> str:
    match = re.search(r"BOOT2\s*=\s*cdrom0:\\?([^;\r\n]+)", system_cnf, re.IGNORECASE)
    if match is None:
        sys.exit("error: no BOOT2 line in SYSTEM.CNF")
    return match.group(1).replace("\\", "/").lstrip("/")


def extract_all(iso: pycdlib.PyCdlib, out_dir: Path) -> None:
    for dirname, _, filenames in iso.walk(iso_path="/"):
        for filename in filenames:
            iso_path = f"{dirname.rstrip('/')}/{filename}"
            dest = out_dir / iso_path.lstrip("/").split(";")[0]
            dest.parent.mkdir(parents=True, exist_ok=True)
            iso.get_file_from_iso(str(dest), iso_path=iso_path)
            print(f"  {iso_path}")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("iso", type=Path, help="path to the disc image")
    parser.add_argument("--out", type=Path, default=Path("iso"), help="where to write the ELF (default: iso/)")
    parser.add_argument("--all", type=Path, metavar="DIR", help="also extract every file on the disc into DIR")
    args = parser.parse_args()

    iso = pycdlib.PyCdlib()
    iso.open(str(args.iso))
    try:
        system_cnf = read_file(iso, "/SYSTEM.CNF;1")
        boot_name = find_boot_elf(system_cnf.decode("ascii", errors="replace"))
        elf = read_file(iso, f"/{boot_name.upper()};1")

        args.out.mkdir(parents=True, exist_ok=True)
        elf_name = boot_name.split("/")[-1]
        (args.out / "SYSTEM.CNF").write_bytes(system_cnf)
        (args.out / elf_name).write_bytes(elf)

        print("SYSTEM.CNF:")
        print("  " + system_cnf.decode("ascii", errors="replace").strip().replace("\n", "\n  "))
        print(f"boot ELF:   {args.out / elf_name}")
        print(f"size:       {len(elf):#x} bytes")
        print(f"sha1:       {hashlib.sha1(elf).hexdigest()}")

        if args.all:
            print(f"extracting all files to {args.all}:")
            extract_all(iso, args.all)
    finally:
        iso.close()


if __name__ == "__main__":
    main()
