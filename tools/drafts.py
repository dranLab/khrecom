#!/usr/bin/env python3
"""Measure every parked draft: each INCLUDE_ASM'd function whose C sits under #if 0 is built with its draft switched on
(in a copy: src/ is never touched) and compared with the original.

The draft is found by the function's symbol (GCC 2 demangling for C++), wherever it sits relative to its INCLUDE_ASM:
in the #else, after the #endif, or further away past a placeholder. A note on the `#if 0` line itself counts. So no
layout hides a draft, and a draft that no longer compiles reports its first error instead of disappearing.

The number is funcdiff's "N of M lines differ" (offset- and address-only lines left out), with the size difference.

Usage:
    python3 tools/drafts.py                         # every unit, closest first
    python3 tools/drafts.py --part game -j 12       # or lib
    python3 tools/drafts.py --unit System/Rcf       # one unit (a prefix works: --unit Game/Enemy)
    python3 tools/drafts.py --failed                # only the drafts that do not build, with their first error
"""

import argparse
import multiprocessing as mp
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from compare_compilers import functions  # noqa: E402
from funcdiff import compare_rows, disassemble  # noqa: E402
from permuter import (Scorer, build_command, definition_pattern, demangle, disabled, enable_draft,  # noqa: E402
                      select_definition)
from versions import default_version  # noqa: E402

INCLUDE_ASM = re.compile(r'^INCLUDE_ASM\("[^"]*",\s*([^)\s]+)\s*\);', re.M)


def version_and_build() -> tuple[str, str]:
    version = default_version()
    opts = yaml.safe_load((ROOT / "config" / version / f"{version}.yaml").read_text())["options"]
    return version, opts["build_path"]


def jobs_for(unit: str, source: Path) -> list[tuple[str, str, str]]:
    """(unit, symbol, state) per INCLUDE_ASM'd function: state "draft" when a disabled definition exists."""
    text = source.read_text(encoding="latin-1")
    out = []
    for m in INCLUDE_ASM.finditer(text):
        if disabled(text, m.start()):
            continue  # an INCLUDE_ASM in a disabled region is not compiled
        symbol = m.group(1)
        name, cls = demangle(symbol)
        pattern = definition_pattern(name, cls)
        k, _ = select_definition(text, pattern, symbol)  # overloads: by parameter count, then the parked one
        d = list(re.finditer(pattern, text, re.M))[k] if k is not None else None
        out.append((unit, symbol, "draft" if d and disabled(text, d.start()) else "no draft"))
    return out


def lines_differ(scorer: Scorer, obj: Path, symbol: str) -> tuple[int, int]:
    """(lines, size difference in bytes) by funcdiff's own measure (compare_rows): the "N of M lines differ" count,
    offset- and address-only lines left out. (0, 0) = MATCH. The permuter's score also counts relocations and
    address differences, which suits ranking variants but not a report."""
    ours = functions(obj).get(symbol)
    if ours is None:
        return 1 << 30, 0
    if scorer.target.difference(ours) is None:
        return 0, 0
    rows = compare_rows(disassemble(scorer.expected, symbol, len(scorer.target.data)),
                        disassemble(obj, symbol, len(ours.data)))
    return max(1, sum(m == "|" for m, _, _ in rows)), len(ours.data) - len(scorer.target.data)


def measure(job: tuple[str, str, str, str]) -> tuple[str, str, str, int | None, str]:
    unit, symbol, state, build = job
    if state != "draft":
        return unit, symbol, state, None, ""
    source = next(ROOT / "src" / f"{unit}{e}" for e in (".c", ".cpp") if (ROOT / "src" / f"{unit}{e}").exists())
    expected = ROOT / build / "expected" / "units" / f"{unit}.o"
    workdir = ROOT / "build" / "drafts" / unit.replace("/", "_") / re.sub(r"[^\w.-]", "_", symbol)  # shell: no $
    shutil.rmtree(workdir, ignore_errors=True)
    workdir.mkdir(parents=True)
    name, cls = demangle(symbol)
    text = enable_draft(source.read_text(encoding="latin-1"), definition_pattern(name, cls), symbol)
    Scorer.build = build
    try:
        scorer = Scorer(unit, symbol, source, build_command(f"{build}/src/{unit}.o"), expected, workdir)
    except KeyError:
        return unit, symbol, "no symbol", None, "not in the original object"
    score, err = scorer.score(text, "draft")
    size = 0
    if score < 1 << 30:
        score, size = lines_differ(scorer, workdir / "draft" / "out.o", symbol)
    shutil.rmtree(workdir, ignore_errors=True)
    if score >= 1 << 30:
        first = next((l.strip() for l in err.split("\n") if re.search(r":\d+(:\d+)?: (error|parse error|syntax)", l)
                      or "Error:" in l or "undeclared" in l or "conflicting" in l), "")
        if not first:
            first = next((l.strip() for l in err.split("\n") if re.search(r"\.(c|cpp|h):\d+:", l)
                          and "warning" not in l), err.strip().split("\n")[-1] if err.strip() else "")
        first = first.replace(str(workdir / "draft") + "/", "")  # the copy's path: just the file name
        if "Segmentation fault" in first:
            first += " (Ps2EeAs crashed: an assembler error it cannot print, see CLAUDE.md's Pitfalls)"
        return unit, symbol, "build failed", None, first[-200:]
    info = f"{abs(size)} bytes {'long' if size > 0 else 'short'}" if size else ""
    return unit, symbol, "MATCH" if score == 0 else "differs", score, info


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--unit", help="only this unit, or units under this prefix")
    parser.add_argument("--part", choices=("game", "lib"), help="only game units or only lib/ units")
    parser.add_argument("--failed", action="store_true", help="only report drafts that do not build")
    parser.add_argument("-j", "--jobs", type=int, default=max(1, (os.cpu_count() or 2) - 2))
    args = parser.parse_args()

    version, build = version_and_build()
    sources = sorted(list((ROOT / "src").rglob("*.c")) + list((ROOT / "src").rglob("*.cpp")))
    jobs = []
    for s in sources:
        unit = str(s.relative_to(ROOT / "src")).rsplit(".", 1)[0]
        if args.unit and not (unit == args.unit or unit.startswith(args.unit.rstrip("/") + "/")):
            continue
        if args.part == "lib" and not unit.startswith("lib/") or args.part == "game" and unit.startswith("lib/"):
            continue
        if "INCLUDE_ASM" not in s.read_text(encoding="latin-1"):
            continue
        jobs += [j + (build,) for j in jobs_for(unit, s)]
    units = sorted({j[0] for j in jobs})
    for u in units:  # the original objects, once
        subprocess.run(["ninja", f"{build}/expected/units/{u}.o"], cwd=ROOT, capture_output=True)
    with mp.Pool(args.jobs) as pool:
        rows = pool.map(measure, jobs)
    order = {"MATCH": 0, "differs": 1, "build failed": 2, "no draft": 3, "no symbol": 4}
    rows.sort(key=lambda r: (order[r[2]], r[3] if r[3] is not None else 0, r[0], r[1]))
    counts = {}
    for unit, symbol, state, score, info in rows:
        counts[state] = counts.get(state, 0) + 1
        if args.failed and state != "build failed":
            continue
        shown = "MATCH" if state == "MATCH" else str(score) if state == "differs" else state
        print(f"{shown:>13}  {unit}  {symbol}" + (f"   {info}" if info else ""))
    print("  ".join(f"{k}: {v}" for k, v in sorted(counts.items(), key=lambda kv: order[kv[0]])), file=sys.stderr)


if __name__ == "__main__":
    main()
