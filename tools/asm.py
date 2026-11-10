#!/usr/bin/env python3
"""Print a unit's original assembly compactly (about a third of the raw splat .s), to read it cheaply.

Usage:
    python3 tools/asm.py System/Spad                  # functions in original order: size, binding; data sections
    python3 tools/asm.py System/Spad init__5CSpad ... # those functions' assembly
    python3 tools/asm.py System/Spad --all            # every function's assembly
    python3 tools/asm.py System/Spad --data [rodata]  # the unit's data (all sections, or the ones named)

Removed: the "/* rom vram bytes */" column, blank lines, .align/nonmatching/endlabel, the hex bytes after
strings. Float constants loaded through $at get their value as a comment ("# 1.78f").
"""

import argparse
import re
import struct
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from versions import default_version  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
PREFIX = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]+)(?: [0-9A-F]+)? \*/\s*")
SKIP = (".align", ".include", ".set", ".section", "/* Generated", "nonmatching", "endlabel", "enddlabel", ".size")
LUI_AT = re.compile(r"lui\s+\$at, \(0x([0-9A-F]{8}) >> 16\)")


def version_dir(version):
    return default_version(version)


def short_float(hexbits):
    f = struct.unpack(">f", bytes.fromhex(hexbits))[0]
    for p in range(1, 10):
        if struct.pack(">f", float(f"{f:.{p}g}")) == bytes.fromhex(hexbits):
            return f"{f:.{p}g}"
    return repr(f)


def parse_functions(path):
    """Ordered {name: [compact lines]} of a unit's text .s."""
    funcs, cur = {}, None
    for raw in path.read_text().splitlines():
        if raw.startswith("glabel "):
            cur = funcs.setdefault(raw.split()[1], [])
            continue
        if cur is None or not raw.strip() or raw.lstrip().startswith(SKIP):
            continue
        m = PREFIX.match(raw)
        line = raw[m.end() if m else 0:].rstrip()
        if line.strip().startswith(".L"):
            cur.append(line.strip())
            continue
        slot = " " if m and raw[:m.end()].endswith("*/   ") else ""  # branch delay slot
        op, _, args = line.strip().partition(" ")
        line = f"  {slot}{op:<8}{args.strip()}" if args else f"  {slot}{op}"
        m = LUI_AT.search(line)
        if m:
            line += f"  # {short_float(m.group(1))}f"
        cur.append(line)
    return funcs


def bindings(version, names):
    try:
        out = subprocess.run(["mips-linux-gnu-readelf", "-sW", str(ROOT / "iso" / version)],
                             capture_output=True, text=True, check=True).stdout
    except (OSError, subprocess.CalledProcessError):
        return {}
    result = {}
    for line in out.splitlines():
        f = line.split()
        if len(f) >= 8 and f[3] == "FUNC" and f[7] in names:
            result[f[7]] = f[4].lower()
    return result


def compact_data(path):
    out = []
    for raw in path.read_text().splitlines():
        s = raw.strip()
        if not s or s.startswith(SKIP):
            continue
        if re.fullmatch(r"/\* [0-9A-F]+ \*/", s):  # bytes after a string
            continue
        m = PREFIX.match(raw)
        out.append(f"{m.group(1)} {raw[m.end():].rstrip()}" if m else s)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("unit")
    ap.add_argument("functions", nargs="*", help="mangled names (or, with --data, section names)")
    ap.add_argument("--all", action="store_true", help="every function")
    ap.add_argument("--data", action="store_true", help="the unit's data sections")
    ap.add_argument("--version")
    args = ap.parse_intermixed_args()

    version = version_dir(args.version)
    base = ROOT / "build" / version / "expected" / "asm"
    text = base / f"{args.unit}.s"
    data = sorted((base / "data").glob(f"{args.unit}.*.s"))

    if args.data:
        for p in data:
            sec = p.name[len(Path(args.unit).name) + 1:-2]
            if args.functions and sec not in args.functions:
                continue
            print(f"## .{sec}")
            print("\n".join(compact_data(p)))
        return

    if not text.exists():
        sys.exit(f"no {text.relative_to(ROOT)} (run configure.py?)")
    funcs = parse_functions(text)

    if args.all or args.functions:
        for name in funcs if args.all else args.functions:
            if name not in funcs:
                sys.exit(f"{name}: not in {args.unit}; functions: {' '.join(funcs)}")
            print(f"{name}:")
            print("\n".join(funcs[name]))
        return

    bind = bindings(version, set(funcs))
    for name, lines in funcs.items():
        n = sum(1 for l in lines if not l.startswith(".L"))
        print(f"{n:5} {bind.get(name, '?'):6} {name}")
    for p in data:
        print(f"data: {p.relative_to(ROOT)} ({len(compact_data(p))} lines)")


if __name__ == "__main__":
    main()
