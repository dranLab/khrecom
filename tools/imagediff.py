#!/usr/bin/env python3
"""Where does the rebuilt image differ from the original? The first differing ranges, with the object, section and
nearest symbol each falls in (from the linker map). check.sh runs it when the checksum fails.

funcdiff compares code only (and ignores what relocations fill in), so wrong data from source - a constant table, a
string - shows up here first.

Usage:
    python3 tools/imagediff.py [--version SLUS_217.99] [-n 10]
"""

import argparse
import bisect
import re
import sys
from pathlib import Path

import yaml

sys.path.insert(0, str(Path(__file__).resolve().parent))
from versions import default_version  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
SECTION = re.compile(r"^ (\.\w+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+) (\S+)$")
SYMBOL = re.compile(r"^\s+0x([0-9a-f]+)\s+([^\s=]+)$")


def read_map(path: Path) -> tuple[list[tuple[int, int, str, str]], list[tuple[int, str]]]:
    """(start, end, section, object) of every non-empty input section, and (address, name) of the symbols."""
    sections, symbols = [], []
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        if m := SECTION.match(line):
            start, size = int(m.group(2), 16), int(m.group(3), 16)
            if size and start:
                sections.append((start, start + size, m.group(1), m.group(4)))
        elif (m := SYMBOL.match(line)) and not m.group(2).endswith(".NON_MATCHING"):  # splat's marker labels
            symbols.append((int(m.group(1), 16), m.group(2)))
    return sorted(sections), sorted(symbols)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--version", help="folder name under config/ (default: the only one)")
    parser.add_argument("-n", type=int, default=10, help="differing ranges to show (default 10)")
    args = parser.parse_args()

    version = default_version(args.version)
    cfg = yaml.safe_load((ROOT / "config" / version / f"{version}.yaml").read_text(encoding="utf-8"))
    opts, first = cfg["options"], cfg["segments"][0]
    base = first["vram"] - first["start"]  # image offset -> address
    ours_path = ROOT / Path(opts["elf_path"]).with_suffix(".rom")
    original, ours = (ROOT / opts["target_path"]).read_bytes(), ours_path.read_bytes() if ours_path.exists() else b""
    if not ours:
        sys.exit(f"{ours_path.relative_to(ROOT)} not built (the link failed?)")
    sections, symbols = read_map(ROOT / Path(opts["elf_path"]).with_suffix(".map"))
    starts, sym_addrs = [s[0] for s in sections], [s[0] for s in symbols]

    if len(ours) != len(original):
        print(f"size: ours {len(ours):#x} bytes, original {len(original):#x} (everything after the first difference "
              f"is shifted)")
    ranges, i, n = [], 0, min(len(ours), len(original))
    while i < n and len(ranges) < args.n:
        if ours[i] != original[i]:
            j = i
            while j < n and ours[j] != original[j] and j - i < 0x100:
                j += 1
            ranges.append((i, j))
            i = j
        i += 1
    if not ranges:
        print("no differing bytes" + (" in the common part" if len(ours) != len(original) else ""))
    for lo, hi in ranges:
        addr = base + lo
        k = bisect.bisect_right(starts, addr) - 1
        where = f"{sections[k][3]} ({sections[k][2]}+{addr - sections[k][0]:#x})" \
            if k >= 0 and addr < sections[k][1] else "no input section"
        s = bisect.bisect_right(sym_addrs, addr) - 1
        sym = f", {symbols[s][1]}+{addr - symbols[s][0]:#x}" if s >= 0 else ""
        print(f"{addr:08X} ({hi - lo} bytes): {where}{sym}")
        print(f"    original {original[lo:min(hi, lo + 16)].hex(' ')}")
        print(f"    ours     {ours[lo:min(hi, lo + 16)].hex(' ')}")


if __name__ == "__main__":
    main()
