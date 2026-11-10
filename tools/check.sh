#!/usr/bin/env bash
# Gate before committing (workers) and after merging (coordinator):
#   1. configure.py (picks up new/changed sources and SOURCE DATA comments)
#   2. full build, which only succeeds if the rebuilt image matches the original byte for byte
#   3. objdiff overview of every file compiled from source (informational)
#   4. whether this branch contains the current main, and uncommitted changes
# Exit code 0 only if 1 and 2 pass.
#
# The overview of all files is written to build/check.overview.txt. The terminal shows the files that are not
# complete (INCLUDE_ASM or not matching) and those changed on this branch or uncommitted, plus the totals;
# --full prints every file. This only changes what is printed, never the checks or the exit code.
#
#   tools/check.sh [--full] [--shift]
#
# --shift (docs/port-step1-shiftable.md): also link the shifted presets of config/<VERSION>/shift.yaml and check
# them (tools/shiftpins.py, tools/shiftcheck.py); their failure fails the gate.
set -uo pipefail
cd "$(git rev-parse --show-toplevel)"
PY=.venv/bin/python
status=0
full=0
shift_check=0
for arg in "$@"; do
    case "$arg" in
        --full) full=1 ;;
        --shift) shift_check=1 ;;
        *) echo "usage: tools/check.sh [--full] [--shift]" >&2; exit 2 ;;
    esac
done
# Source files changed on this branch (since it left main) or not committed yet.
base=$(git merge-base main HEAD 2>/dev/null || git rev-parse HEAD)
changed=$( { git diff --name-only "$base"; git ls-files --others --exclude-standard; } | sort -u | tr '\n' ' ')

echo "== configure"
if ! $PY configure.py > build.configure.log 2>&1; then
    tail -20 build.configure.log
    exit 1
fi
tail -1 build.configure.log

echo "== build"
if ninja > build.ninja.log 2>&1; then
    echo "OK: the rebuilt image matches the original"
else
    grep -E "^FAILED|: [Ee]rror|[Ee]rror:|did NOT match" build.ninja.log | head -20
    if grep -q "did NOT match" build.ninja.log; then
        echo "-- where the image differs (tools/imagediff.py):"
        $PY tools/imagediff.py -n 5
    fi
    echo "FAIL: see build.ninja.log"
    status=1
fi

echo "== files from source (objdiff)"
if ninja expected > /dev/null 2>&1 && tools/objdiff.sh report generate -o build/report.json > /dev/null 2>&1; then
    CHECK_FULL=$full CHECK_CHANGED="$changed" $PY - <<'PY'
import json, os, re, sys
from pathlib import Path
sys.path.insert(0, "tools")
from compare_compilers import functions, pair_unnamed
from progress import active_lines
full, changed = os.environ["CHECK_FULL"] == "1", set(os.environ["CHECK_CHANGED"].split())
lines, shown, complete = [], [], 0
config = [u for u in json.load(open("objdiff.json"))["units"] if "base_path" in u]
units = {u["name"]: u["metadata"]["source_path"] for u in config}
objects = {u["name"]: (u["target_path"], u["base_path"]) for u in config}

def objdiff_only(unit, names):
    """The names objdiff counts as not matching that are byte-identical by funcdiff's comparison: unnamed copies of
    inline functions (func_<ADDR>, compiled under their real names) and relocations against splat's symbols for
    constants (D_FFFF), which compiled code doesn't have."""
    try:
        t, b = (functions(Path(p)) for p in objects[unit])
    except (OSError, KeyError, StopIteration):
        return set()
    pairs = pair_unnamed(t, b)
    return {n for n in names if n in t and pairs.get(n, n) in b and t[n].difference(b[pairs.get(n, n)]) is None}
total = decompiled = 0
lib = {"files": 0, "complete": 0, "total": 0, "decompiled": 0}  # library units (lib/...), counted separately too
for u in sorted(json.load(open("build/report.json"))["units"], key=lambda u: u["name"]):
    if u["name"] not in units:
        continue
    m = u.get("measures", {})
    # INCLUDE_ASM'd functions are the original assembly: objdiff counts them as matching, but they aren't done.
    # Only the compiled lines: a parked draft's INCLUDE_ASM sits in the #else of its `#if 0`, which a plain
    # `#if 0.*?#endif` strip would remove too (it counted every parked function as done).
    source = "\n".join(active_lines(Path(units[u["name"]]).read_text(encoding="latin-1")))
    asm = set(re.findall(r"INCLUDE_ASM\([^,]+,\s*([^)\s]+)\s*\)", source))
    fns = u.get("functions", [])
    open_fns = [f["name"] for f in fns if f.get("fuzzy_match_percent", 0) < 100]
    if open_fns:
        same = objdiff_only(u["name"], open_fns)
        open_fns = [n for n in open_fns if n not in same]
    done = [f for f in fns if f["name"] not in open_fns and f["name"] not in asm]
    total += len(fns); decompiled += len(done)
    line = (f"  {u['name']}: {len(done)}/{len(fns)} functions decompiled and matching"
            + (f", {len(asm)} still INCLUDE_ASM" if asm else "")
            + (f", NOT matching: {', '.join(open_fns)}" if open_fns else ""))
    lines.append(line)
    is_complete = len(done) == len(fns)
    complete += is_complete
    if u["name"].startswith("lib/"):
        lib["files"] += 1; lib["complete"] += is_complete; lib["total"] += len(fns); lib["decompiled"] += len(done)
    if full or not is_complete or units[u["name"]] in changed:
        shown.append(line)
summary = f"  total: {decompiled} functions decompiled in {len(units)} files"
Path("build/check.overview.txt").write_text("\n".join(lines + [summary]) + "\n")
if shown:
    print("\n".join(shown))
if len(shown) < len(lines):
    print(f"  ({len(lines) - len(shown)} more files, all complete, not listed; every file: build/check.overview.txt)")
print(f"{summary}; {complete} files complete, {len(units) - complete} not")
if lib["files"]:
    print(f"  of these, libraries (lib/): {lib['decompiled']}/{lib['total']} functions in {lib['files']} files;"
          f" {lib['complete']} complete, {lib['files'] - lib['complete']} not")
PY
else
    echo "  (objdiff report failed)"
fi

if [[ $shift_check == 1 ]]; then
    echo "== shifted links (docs/port-step1-shiftable.md)"
    if ! ninja shift > build/shift.log 2>&1; then
        grep -E "^FAILED|undefined reference|[Ee]rror" build/shift.log | head -10
        echo "FAIL: shifted links, see build/shift.log"
        status=1
    elif ! $PY tools/shiftpins.py > build/shiftpins.log 2>&1; then
        head -12 build/shiftpins.log
        echo "FAIL: pinned or undefined symbols in the shifted link"
        status=1
    else
        for preset in $($PY -c 'import yaml,glob; print(" ".join(yaml.safe_load(open(glob.glob("config/*/shift.yaml")[0]))["presets"]))'); do
            if $PY tools/shiftcheck.py --preset "$preset" --details 5 > "build/shiftcheck.$preset.log" 2>&1; then
                echo "OK: $preset ($(sed -n 2,3p "build/shiftcheck.$preset.log" | tr '\n' ' '))"
            else
                cat "build/shiftcheck.$preset.log"
                echo "FAIL: shiftcheck $preset"
                status=1
            fi
        done
    fi
fi

echo "== branch"
if git rev-parse -q --verify main > /dev/null && ! git merge-base --is-ancestor main HEAD; then
    echo "WARNING: $(git branch --show-current) does not contain the current main - rebase before merging"
else
    echo "$(git branch --show-current): up to date with main"
fi
if [[ -n "$(git status --porcelain)" ]]; then
    echo "uncommitted changes:"
    git status --short | head -20
fi
exit $status
