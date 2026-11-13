"""Wrap a function definition (from the line starting with PATTERN to its closing `}` at column 0) line by line in
KH_DEBUG(...): line-neutral. Usage: wrap_func.py FILE REGEX"""
import re, sys
from pathlib import Path
path, pattern = Path(sys.argv[1]), re.compile(sys.argv[2])
lines = path.read_text(encoding="latin-1").split("\n")
start = next(i for i, l in enumerate(lines) if pattern.search(l) and not l.startswith((" ", "\t", "/")))
end = next(i for i in range(start, len(lines)) if lines[i].startswith("}"))
def split_comment(l):
    q = None
    for k, c in enumerate(l):
        if q:
            if c == "\\": continue
            if c == q and l[k - 1] != "\\": q = None
        elif c in "\"'": q = c
        elif l.startswith("//", k): return l[:k], l[k:]
    return l, ""
for i in range(start, end + 1):
    l = lines[i]
    if not l.strip(): continue
    if l.lstrip().startswith("#") or "/*" in l and "*/" not in l:
        sys.exit(f"line {i + 1}: directive or block comment: {l}")
    code, comment = split_comment(l)
    ind = len(code) - len(code.lstrip())
    if code.strip():
        lines[i] = code[:ind] + "KH_DEBUG(" + code[ind:].rstrip() + ")" + (" " + comment if comment else "")
path.write_text("\n".join(lines), encoding="latin-1")
print(f"{path}: lines {start + 1}-{end + 1} wrapped")
