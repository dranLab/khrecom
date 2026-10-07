#!/usr/bin/env python3
"""Compare a file compiled from source with the original, function by function (non-interactive objdiff).

Builds the unit's object and its target first. Address fields filled in at link time are ignored, but where
both sides name the symbol they refer to (a called function, a global), it must be the same.

Usage:
    python3 tools/funcdiff.py System/Spad                      # status of every function
    python3 tools/funcdiff.py System/Spad init__5CSpad         # original | ours: the differences, with context
    python3 tools/funcdiff.py System/Spad init__5CSpad --full  # original | ours: every line
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
            line = re.sub(r"^\s+[0-9a-f]+: (R_)", r"   \1", line)       # and relocations' (they follow their insn)
            line = re.sub(r"\b[0-9a-f]+ <([^>+]*)(\+0x[0-9a-f]+)?>", r"<\1\2>", line)  # branch targets
            line = line.replace(f"<{name}+", "<+")                     # branches within the function
            lines.append(line.replace("\t", " ").rstrip())
    # An unlinked jal shows a meaningless target (offset 0 in its section); the relocation below names the real one.
    for i in range(len(lines) - 1):
        if lines[i + 1].lstrip().startswith("R_MIPS_26"):
            lines[i] = re.sub(r"^(jal) <[^>]*>$", r"\1", lines[i])
    return lines


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("unit", help="e.g. System/Spad")
    parser.add_argument("function", nargs="?", help="mangled name; omit for an overview")
    parser.add_argument("--version", help="folder name under config/ (default: the only one)")
    parser.add_argument("--full", action="store_true", help="side by side: print every line, not just the differences")
    parser.add_argument("-C", "--context", type=int, default=3, metavar="N",
                        help="side by side: matching lines shown around each difference (default 3)")
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
            diff = t[name].difference(b[name]) if name in b else None
            state = "missing" if name not in b else "MATCH" if diff is None else "differs"
            print(f"{state:8} {name} ({len(t[name].data)} bytes)" + (f": {diff}" if diff else ""))
        extra = [n for n in b if n not in t]
        if extra:
            print("only in ours (not in the original):", ", ".join(extra))
        return

    if args.function not in t:
        sys.exit(f"{args.function} not in the original {args.unit}")
    name = args.function
    diff = t[name].difference(b[name]) if name in b else "missing in ours"
    left = disassemble(ROOT / target, name, len(t[name].data))
    right = disassemble(ROOT / base, name, len(b[name].data)) if name in b else []
    # Lines are aligned on a key without branch-target offsets, so code shifted by an earlier difference still
    # lines up. Such lines are marked "~" (they differ only in an offset); "|" marks a real difference.
    # Display only: MATCH / differs above comes from comparing the bytes.
    def key(line: str) -> str:
        return re.sub(r"<\+0x[0-9a-f]+>", "<+?>", line)
    rows = []  # (mark, original, ours)
    keys_l, keys_r = [key(line) for line in left], [key(line) for line in right]
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(a=keys_l, b=keys_r, autojunk=False).get_opcodes():
        for k in range(max(i2 - i1, j2 - j1)):
            a = left[i1 + k] if i1 + k < i2 else ""
            c = right[j1 + k] if j1 + k < j2 else ""
            rows.append(("|" if tag != "equal" else "~" if a != c else " ", a, c))
    real = sum(m == "|" for m, _, _ in rows)
    offsets = sum(m == "~" for m, _, _ in rows)
    print(f"{'MATCH' if diff is None else 'differs'} {name}: original {len(t[name].data)} bytes, "
          f"ours {len(b[name].data) if name in b else 0} bytes" + (f"; {diff}" if diff else ""))
    if diff is None and not args.full:
        return
    print(f"{real} of {len(rows)} lines differ (|), {offsets} only in a branch offset (~); original | ours")
    # Show rows within --context of a real difference; elide the rest (nothing with --full).
    keep = [args.full or any(rows[j][0] == "|" for j in range(max(0, i - args.context), min(len(rows), i + args.context + 1)))
            for i in range(len(rows))]
    width = max(len(line) for line in left) + 2
    skipped = []
    def flush() -> None:
        if skipped:
            n = sum(m == "~" for m in skipped)
            print(f"   ... {len(skipped)} lines" + (f" ({n} differ only in a branch offset)" if n else " equal") + " ...")
            skipped.clear()
    for (mark, a, c), shown in zip(rows, keep):
        if not shown:
            skipped.append(mark)
            continue
        flush()
        print(f"{a:<{width}}{mark} {c}")
    flush()


if __name__ == "__main__":
    main()
