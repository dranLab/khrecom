#!/usr/bin/env python3
"""Compare a file compiled from source with the original, function by function (non-interactive objdiff).

Builds the unit's object and its target first. Address fields filled in at link time are ignored.

Usage:
    python3 tools/funcdiff.py System/Spad                      # status of every function
    python3 tools/funcdiff.py System/Spad init__5CSpad         # original | ours, side by side
"""

import argparse
import difflib
import re
import subprocess
import sys
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from compare_compilers import functions  # noqa: E402


def disassemble(obj: Path, name: str, size: int) -> list[str]:
    """The function's instructions (and their relocations), without the padding after it."""
    out = subprocess.run(["mips-linux-gnu-objdump", "-dr", "-z", "--no-show-raw-insn", str(obj)],
                         capture_output=True, text=True, check=True).stdout
    lines, inside, count = [], False, 0
    for line in out.splitlines():
        if line.endswith(f"<{name}>:"):
            inside = True
            continue
        if inside and (not line.strip() or re.match(r"^[0-9a-f]+ <", line)):
            break
        if inside:
            is_insn = re.match(r"^\s+[0-9a-f]+:\t", line) is not None
            if is_insn and count == size // 4:
                break
            count += is_insn
            line = re.sub(r"^\s+[0-9a-f]+:\t", "", line)               # drop addresses
            line = re.sub(r"\b[0-9a-f]+ <([^>+]*)(\+0x[0-9a-f]+)?>", r"<\1\2>", line)  # branch targets
            lines.append(line.replace("\t", " ").rstrip())
    return lines


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("unit", help="e.g. System/Spad")
    parser.add_argument("function", nargs="?", help="mangled name; omit for an overview")
    parser.add_argument("--version", help="folder name under config/ (default: the only one)")
    args = parser.parse_args()

    versions = sorted(p.name for p in (ROOT / "config").iterdir() if (p / f"{p.name}.yaml").exists())
    version = args.version or versions[0]
    opts = yaml.safe_load((ROOT / "config" / version / f"{version}.yaml").read_text(encoding="utf-8"))["options"]
    build = Path(opts["build_path"])
    target, base = build / "expected" / "units" / f"{args.unit}.o", build / "src" / f"{args.unit}.o"
    result = subprocess.run(["ninja", str(target), str(base)], cwd=ROOT, capture_output=True, text=True)
    if result.returncode != 0:
        sys.exit(result.stdout[-3000:] + result.stderr[-1000:])

    t, b = functions(ROOT / target), functions(ROOT / base)
    if not args.function:
        for name in t:
            state = "missing" if name not in b else "MATCH" if b[name] == t[name] else "differs"
            print(f"{state:8} {name} ({len(t[name].data)} bytes)")
        extra = [n for n in b if n not in t]
        if extra:
            print("only in ours (not in the original):", ", ".join(extra))
        return

    if args.function not in t:
        sys.exit(f"{args.function} not in the original {args.unit}")
    left = disassemble(ROOT / target, args.function, len(t[args.function].data))
    right = disassemble(ROOT / base, args.function, len(b[args.function].data)) if args.function in b else []
    width = max(len(line) for line in left) + 2
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(a=left, b=right, autojunk=False).get_opcodes():
        for k in range(max(i2 - i1, j2 - j1)):
            a = left[i1 + k] if i1 + k < i2 else ""
            c = right[j1 + k] if j1 + k < j2 else ""
            mark = " " if tag == "equal" else "|"
            print(f"{a:<{width}}{mark} {c}")


if __name__ == "__main__":
    main()
