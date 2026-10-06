#!/usr/bin/env python3
"""Generate a C++ reference tree from the ELF's .mdebug debug info with ccc.

Writes (gitignored, regenerate any time):
    ref/<VERSION>/types.h   every type as C++ (stdump types)
    ref/<VERSION>/src/      one .cpp/.h per original game source file (uncc):
                            class layouts, function signatures with parameter
                            and local variable names, globals with their data

The output is a reference for writing the real headers and code in include/
and src/, not something to compile: e.g. compiler-generated members appear
without parameters and types uncc can't place end up in lost+found.h.

If you export Ghidra's decompilation with ccc's CCCDecompileAllFunctions.java
(in tools/ccc/), pass it with --functions and uncc fills in the function bodies.

Usage:
    python3 tools/gen_reference.py SLUS_217.99 [--functions FUNCTIONS.txt]
"""

import argparse
import io
import re
import shutil
import subprocess
import sys
import urllib.request
import zipfile
from collections import Counter
from pathlib import Path, PurePosixPath

import yaml

ROOT = Path(__file__).resolve().parent.parent
CCC_VERSION = "v2.1"
CCC_DIR = ROOT / "tools" / "ccc" / f"ccc_{CCC_VERSION}_linux-musl"
CCC_URL = f"https://github.com/chaoticgd/ccc/releases/download/{CCC_VERSION}/ccc_{CCC_VERSION}_linux-musl.zip"
GAME_ROOT = "c:/khcom/mainapp/"


def ensure_ccc() -> None:
    if (CCC_DIR / "uncc").exists():
        return
    print(f"downloading {CCC_URL}")
    with urllib.request.urlopen(CCC_URL) as resp:
        zipfile.ZipFile(io.BytesIO(resp.read())).extractall(CCC_DIR.parent)
    for tool in ("stdump", "uncc", "demangle", "objdump"):
        (CCC_DIR / tool).chmod(0o755)


def run(tool: str, *args: str) -> str:
    result = subprocess.run([str(CCC_DIR / tool), *args], capture_output=True, text=True)
    if result.returncode != 0:
        sys.exit(f"error: {tool} failed:\n{result.stderr[-2000:]}")
    return result.stdout


def source_list(elf: Path) -> list[str]:
    """SOURCES.txt for uncc: one path per translation unit, named like the split (System/Coord.cpp, lib/...)."""
    paths = []
    for line in run("stdump", "files", str(elf)).splitlines():
        path = re.sub(r"^/\* [0-9a-f]+ \*/ ", "", line).replace("\\", "/")
        if path.lower().startswith(GAME_ROOT):
            paths.append(path[len(GAME_ROOT):])
        else:
            parts = PurePosixPath(re.sub(r"^[a-zA-Z]:/", "", path)).parts
            paths.append("lib/" + "/".join(p for p in parts if p not in ("..", ".", "/")))
    counts, seen, out = Counter(paths), Counter(), []
    for path in paths:
        if counts[path] > 1:  # e.g. libgcc2.c is compiled once per helper function
            seen[path] += 1
            p = PurePosixPath(path)
            path = str(p.with_name(f"{p.stem}_{seen[path]}{p.suffix}"))
        out.append(path)
    return out


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("version", help="folder name under config/")
    parser.add_argument("--functions", type=Path, help="FUNCTIONS.txt exported from Ghidra with CCCDecompileAllFunctions.java")
    args = parser.parse_args()

    build_cfg = yaml.safe_load((ROOT / "config" / args.version / "build.yaml").read_text(encoding="utf-8"))
    elf = ROOT / build_cfg["elf"]
    out = ROOT / "ref" / args.version
    ensure_ccc()

    shutil.rmtree(out, ignore_errors=True)
    (out / "src").mkdir(parents=True)
    run("stdump", "types", str(elf), "--output", str(out / "types.h"))

    sources = source_list(elf)
    (out / "src" / "SOURCES.txt").write_text("\n".join(sources) + "\n", encoding="utf-8")
    if args.functions:
        shutil.copy(args.functions, out / "src" / "FUNCTIONS.txt")
    run("uncc", str(elf), str(out / "src"))

    files = [p for p in (out / "src").rglob("*") if p.suffix in (".c", ".cpp", ".h")]
    print(f"wrote {out.relative_to(ROOT)}/types.h and {len(files)} files for {len(sources)} sources "
          f"in {out.relative_to(ROOT)}/src/")


if __name__ == "__main__":
    main()
