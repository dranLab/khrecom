#!/usr/bin/env bash
# Gate before committing (workers) and after merging (coordinator):
#   1. configure.py (picks up new/changed sources and SOURCE DATA comments)
#   2. full build, which only succeeds if the rebuilt image matches the original byte for byte
#   3. objdiff overview of every file compiled from source (informational)
#   4. whether this branch contains the current main, and uncommitted changes
# Exit code 0 only if 1 and 2 pass.
#
#   tools/check.sh
set -uo pipefail
cd "$(git rev-parse --show-toplevel)"
PY=.venv/bin/python
status=0

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
    echo "FAIL: see build.ninja.log"
    status=1
fi

echo "== files from source (objdiff)"
if ninja expected > /dev/null 2>&1 && tools/objdiff.sh report generate -o build/report.json > /dev/null 2>&1; then
    $PY - <<'PY'
import json, re
from pathlib import Path
units = {u["name"]: u["metadata"]["source_path"] for u in json.load(open("objdiff.json"))["units"] if "base_path" in u}
total = decompiled = 0
for u in sorted(json.load(open("build/report.json"))["units"], key=lambda u: u["name"]):
    if u["name"] not in units:
        continue
    m = u.get("measures", {})
    # INCLUDE_ASM'd functions are the original assembly: objdiff counts them as matching, but they aren't done.
    source = re.sub(r"#if 0.*?#endif", "", Path(units[u["name"]]).read_text(encoding="latin-1"), flags=re.S)
    asm = set(re.findall(r"INCLUDE_ASM\([^,]+,\s*([^)\s]+)\s*\)", source))
    fns = u.get("functions", [])
    done = [f for f in fns if f.get("fuzzy_match_percent", 0) >= 100 and f["name"] not in asm]
    open_fns = [f["name"] for f in fns if f.get("fuzzy_match_percent", 0) < 100]
    total += len(fns); decompiled += len(done)
    print(f"  {u['name']}: {len(done)}/{len(fns)} functions decompiled and matching"
          + (f", {len(asm)} still INCLUDE_ASM" if asm else "")
          + (f", NOT matching: {', '.join(open_fns)}" if open_fns else ""))
print(f"  total: {decompiled} functions decompiled in {len(units)} files")
PY
else
    echo "  (objdiff report failed)"
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
