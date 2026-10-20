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
import json
import os
import re
import subprocess
import sys
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from compare_compilers import ANONYMOUS, functions, pair_unnamed, target_name  # noqa: E402


def disassemble(obj: Path, name: str, size: int) -> list[str]:
    """The function's instructions (and their relocations), without the padding after it.

    Labels inside the function (the original's jump-table targets, `.L...`) are skipped, and branches within the
    function are shown as offsets from its start (`<+0x40>`), whether they go to a label (the original: an
    R_MIPS_PC16 relocation) or are resolved already (ours)."""
    out = subprocess.run(["mips-linux-gnu-objdump", "-dr", "-z", "--no-show-raw-insn", str(obj)],
                         capture_output=True, text=True, check=True).stdout
    labels = {m.group(2): int(m.group(1), 16)
              for m in re.finditer(r"^([0-9a-f]+) <([^>]+)>:$", out, re.M)}
    start = labels.get(name)
    end = None if start is None else start + size
    def local(addr: int) -> str:
        return f"<+0x{addr - start:x}>" if addr != start else "<+0x0>"
    lines, inside, count = [], False, 0
    for line in out.splitlines():
        if line.endswith(f"<{name}>:"):
            inside = True
            continue
        if not inside:
            continue
        label = re.match(r"^([0-9a-f]+) <[^>]+>:$", line)
        if not line.strip() or label:
            if count == size // 4 or (label and int(label.group(1), 16) >= end):
                break
            continue                                                  # a label within the function
        is_insn = re.match(r"^\s+[0-9a-f]+:\t", line) is not None
        if is_insn and count == size // 4:
            break
        count += is_insn
        reloc = re.match(r"^\s+[0-9a-f]+: (R_MIPS_PC16)\s+(\S+?)(\+0x[0-9a-f]+|-0x[0-9a-f]+)?$", line)
        if reloc and reloc.group(2) in labels and lines:
            # a branch to a label: the instruction shows no real target; put the label's offset there
            target = labels[reloc.group(2)]
            if start <= target < end:
                lines[-1] = re.sub(r"<[^>]*>$", local(target), lines[-1])
                continue
        line = re.sub(r"^\s+[0-9a-f]+:\t", "", line)               # drop addresses
        line = re.sub(r"^\s+[0-9a-f]+: (R_)", r"   \1", line)       # and relocations' (they follow their insn)
        def target(m: re.Match) -> str:
            addr = int(m.group(1), 16)
            return local(addr) if start is not None and start <= addr < end else f"<{m.group(2)}{m.group(3) or ''}>"
        line = re.sub(r"\b([0-9a-f]+) <([^>+]*)(\+0x[0-9a-f]+)?>", target, line)  # branch targets
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
    # A hand-written .s source builds splat's object path instead: objdiff.json knows it.
    unit = next((u for u in json.loads((ROOT / "objdiff.json").read_text())["units"] if u["name"] == args.unit), {})
    base = Path(unit.get("base_path", base))
    result = subprocess.run(["ninja", str(target), str(base)], cwd=ROOT, capture_output=True, text=True)
    if result.returncode != 0:
        sys.exit(result.stdout[-3000:] + result.stderr[-1000:])

    t, b = functions(ROOT / target), functions(ROOT / base)
    pairs = pair_unnamed(t, b)  # the original's unnamed copies of inline functions -> ours
    if not args.function:
        for name in t:
            if not t[name].data and name not in b:
                continue  # a 0-byte label (splat's func_ at a jump target inside a function), not a function
            ours = pairs.get(name, name)
            diff = t[name].difference(b[ours]) if ours in b else None
            state = "missing" if ours not in b else "MATCH" if diff is None else "differs"
            print(f"{state:8} {name} ({len(t[name].data)} bytes)" + (f" = ours {ours}" if ours != name else "")
                  + (f": {diff}" if diff else ""))
        extra = [n for n in b if n not in t and n not in pairs.values()]
        if extra:
            print("only in ours (not in the original):", ", ".join(extra))
        return

    if args.function not in t:
        sys.exit(f"{args.function} not in the original {args.unit}")
    name = args.function
    ours = pairs.get(name, name)
    diff = t[name].difference(b[ours]) if ours in b else "missing in ours"
    left = disassemble(ROOT / target, name, len(t[name].data))
    right = disassemble(ROOT / base, ours, len(b[ours].data)) if ours in b else []
    # Lines are aligned on a key without branch-target offsets and data addresses (see key()), so code shifted by
    # an earlier difference still lines up. Such lines are marked "~" (they differ only in an offset or address);
    # "|" marks a real difference.
    # Display only: MATCH / differs above comes from comparing the bytes.
    # Data references are aligned whatever they name: the original's anonymous labels (jtbl_..., D_... for statics
    # whose names are lost) vs. our names or section + offset. Different names on both sides are still a difference.
    data_reloc = re.compile(r"^\s+(R_MIPS_(?:HI16|LO16|32|GPREL16))\s+(\S+?)([+-]0x[0-9a-f]+)?$")
    any_reloc = re.compile(r"^\s+(R_MIPS_\w+)\s+(\S+?)([+-]0x[0-9a-f]+)?$")

    def key(line: str) -> str:
        line = re.sub(r"<\+0x[0-9a-f]+>", "<+?>", line)
        line = data_reloc.sub(r"   \1 <data>", line)
        # a jump to a local label (inline asm): splat's func_ label in the original, the section in ours
        return re.sub(r"^(\s+R_MIPS_26)\s+(\S+)$", lambda m: f"{m.group(1)} <local>" if ANONYMOUS.match(m.group(2))
                      else m.group(0), line)

    def real_difference(a: str, c: str) -> bool:
        """For lines with equal keys: do they name different (non-anonymous) data?"""
        x, y = data_reloc.match(a), data_reloc.match(c)
        names = [m.group(2) for m in (x, y) if m and not ANONYMOUS.match(m.group(2)) and m.group(2) != "_gp"]
        return len(names) == 2 and target_name(names[0]) != target_name(names[1])

    def keys(lines: list[str]) -> list[str]:
        k = [key(line) for line in lines]
        for i in range(len(k) - 1):  # the address part a data relocation fills in (%lo(...) offset)
            if k[i + 1].endswith(" <data>"):
                k[i] = re.sub(r"(,|^\S+ )-?(?:0x)?[0-9a-f]+(\(\w+\))?$", r"\1?\2", k[i])
        return k
    rows = []  # (mark, original, ours)
    keys_l, keys_r = keys(left), keys(right)
    def address_only(a: list[str], c: list[str]) -> bool:
        """Do two differing blocks only differ in addresses? E.g. an address load splat left unrelocated in the
        original (lui s5,0x63) and relocated in ours (lui s5,0x0 + R_MIPS_HI16 ppvEnv)."""
        def insns(lines: list[str]) -> list[tuple[str, bool]]:  # (key, relocated)
            out: list[tuple[str, bool]] = []
            for line in lines:
                if line.lstrip().startswith("R_"):
                    if out:
                        out[-1] = (out[-1][0], True)
                else:
                    out.append((key(line), False))
            return out
        x, y = insns(a), insns(c)
        if len(x) != len(y):
            return False
        mask = lambda k: re.sub(r"(,|^\S+ )-?(?:0x)?[0-9a-f]+(\(\w+\))?$", r"\1?\2", k)  # noqa: E731
        if any((mask(p) != mask(q)) if (r or t) else p != q for (p, r), (q, t) in zip(x, y)):
            return False  # the immediate only counts as an address where a side relocates the instruction
        def named(lines: list[str]) -> list[str]:  # the symbols the block's relocations name (calls too)
            return [target_name(m.group(2)) for line in lines if (m := any_reloc.match(line))
                    and not ANONYMOUS.match(m.group(2)) and m.group(2) != "_gp"]
        return not (named(a) and named(c) and named(a) != named(c))
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(a=keys_l, b=keys_r, autojunk=False).get_opcodes():
        moved = tag != "equal" and address_only(left[i1:i2], right[j1:j2])
        for k in range(max(i2 - i1, j2 - j1)):
            a = left[i1 + k] if i1 + k < i2 else ""
            c = right[j1 + k] if j1 + k < j2 else ""
            mark = ("~" if moved else "|") if tag != "equal" or real_difference(a, c) else "~" if a != c else " "
            rows.append((mark, a, c))
    real = sum(m == "|" for m, _, _ in rows)
    offsets = sum(m == "~" for m, _, _ in rows)
    print(f"{'MATCH' if diff is None else 'differs'} {name}: original {len(t[name].data)} bytes, "
          f"ours {len(b[ours].data) if ours in b else 0} bytes" + (f" ({ours})" if ours != name else "")
          + (f"; {diff}" if diff else ""))
    if diff is None and not args.full:
        return
    print(f"{real} of {len(rows)} lines differ (|), {offsets} only in an offset or address (~); original | ours")
    # Show rows within --context of a real difference; elide the rest (nothing with --full).
    keep = [args.full or any(rows[j][0] == "|" for j in range(max(0, i - args.context), min(len(rows), i + args.context + 1)))
            for i in range(len(rows))]
    width = max(len(line) for line in left) + 2
    skipped = []
    def flush() -> None:
        if skipped:
            n = sum(m == "~" for m in skipped)
            print(f"   ... {len(skipped)} lines" + (f" ({n} differ only in an offset or address)" if n else " equal") + " ...")
            skipped.clear()
    for (mark, a, c), shown in zip(rows, keep):
        if not shown:
            skipped.append(mark)
            continue
        flush()
        print(f"{a:<{width}}{mark} {c}")
    flush()


if __name__ == "__main__":
    try:
        main()
    except BrokenPipeError:  # output piped into head etc.: stop quietly (the Python docs' recipe)
        os.dup2(os.open(os.devnull, os.O_WRONLY), sys.stdout.fileno())
        sys.exit(1)
