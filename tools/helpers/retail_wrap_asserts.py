"""Wrap one-line assert statements `if (...) AssertApp(...);` in KH_DEBUG(...) (line-neutral)."""
import re, sys
from pathlib import Path
SITE = re.compile(r"^(\s*)(if \(.*\) AssertApp\(.*\);)(\s*(?://.*|/\*.*\*/)?)$")
def dangling(prev: str) -> bool:
    p = re.sub(r"//.*$", "", prev).rstrip()
    return bool(p) and (p.endswith(")") and re.match(r"^\s*(?:\}\s*)?(?:else\s+)?(?:if|for|while)\b", p)) or p.endswith("else")
n = skipped = 0
for path in sys.argv[1:]:
    p = Path(path); lines = p.read_text(encoding="latin-1").split("\n")
    changed = False
    for i, l in enumerate(lines):
        m = SITE.match(l)
        if not m or "KH_DEBUG(" in l:
            continue
        prev = next((lines[j] for j in range(i - 1, -1, -1) if lines[j].strip()), "")
        if dangling(prev):
            skipped += 1; continue
        lines[i] = f"{m.group(1)}KH_DEBUG({m.group(2)}){m.group(3)}"; n += 1; changed = True
    if changed:
        p.write_text("\n".join(lines), encoding="latin-1")
print(n, "wrapped,", skipped, "skipped (dangling)")
