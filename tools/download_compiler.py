#!/usr/bin/env python3
"""Download a PS2 compiler package from decomp.me's compiler archive.

The packages are unpacked into tools/compilers/<name>/ (gitignored).

Usage:
    python tools/download_compiler.py --list
    python tools/download_compiler.py ee-gcc2.96
"""

import argparse
import json
import sys
import tarfile
import urllib.request
from pathlib import Path

RELEASE_API = "https://api.github.com/repos/decompme/compilers/releases/tags/compilers"
ROOT = Path(__file__).resolve().parent.parent
COMPILERS_DIR = ROOT / "tools" / "compilers"
SUFFIXES = (".tar.xz", ".tar.gz")


def ps2_packages() -> dict[str, str]:
    with urllib.request.urlopen(RELEASE_API) as resp:
        release = json.load(resp)
    packages = {}
    for asset in release["assets"]:
        name = asset["name"]
        if not name.startswith(("ee-gcc", "mwcps2")):
            continue
        for suffix in SUFFIXES:
            if name.endswith(suffix):
                packages[name[: -len(suffix)]] = asset["browser_download_url"]
    return packages


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("name", nargs="?", help="package name, e.g. ee-gcc2.96 or mwcps2-3.0.1b145-050209")
    parser.add_argument("--list", action="store_true", help="list available PS2 compilers")
    args = parser.parse_args()

    packages = ps2_packages()
    if args.list or not args.name:
        print("\n".join(sorted(packages)))
        return
    if args.name not in packages:
        sys.exit(f"error: unknown compiler {args.name!r}; run with --list")

    dest = COMPILERS_DIR / args.name
    if dest.exists():
        print(f"{dest} already exists")
        return

    url = packages[args.name]
    archive = COMPILERS_DIR / url.rsplit("/", 1)[-1]
    COMPILERS_DIR.mkdir(parents=True, exist_ok=True)
    print(f"downloading {url}")
    urllib.request.urlretrieve(url, archive)

    dest.mkdir()
    # The archives keep their executable bits; the "data" filter (when available) guards against path tricks.
    extract_kwargs = {"filter": "data"} if hasattr(tarfile, "data_filter") else {}
    with tarfile.open(archive) as tar:
        tar.extractall(dest, **extract_kwargs)
    archive.unlink()
    print(f"installed to {dest}")


if __name__ == "__main__":
    main()
