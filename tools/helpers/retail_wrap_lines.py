"""Wrap source lines START..END (1-based, inclusive) one by one in KH_DEBUG(...): line-neutral. Each line's
parentheses must balance (a macro argument). Already wrapped lines are left alone.
Usage: retail_wrap_lines.py FILE START END [START END ...]"""
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).parent))
path = Path(sys.argv[1])
lines = path.read_text(encoding="latin-1").split("\n")


def split_comment(l):
    q = None
    for k, c in enumerate(l):
        if q:
            if c == q and l[k - 1] != "\\":
                q = None
        elif c in "\"'":
            q = c
        elif l.startswith("//", k):
            return l[:k], l[k:]
    return l, ""


def balanced(code):
    depth, q = 0, None
    for k, c in enumerate(code):
        if q:
            if c == q and code[k - 1] != "\\":
                q = None
        elif c in "\"'":
            q = c
        elif c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
            if depth < 0:
                return False
    return depth == 0


args = [int(a) for a in sys.argv[2:]]
for start, end in zip(args[::2], args[1::2]):
    for i in range(start - 1, end):
        l = lines[i]
        if not l.strip() or l.lstrip().startswith("KH_DEBUG("):
            continue
        if l.lstrip().startswith("#") or "/*" in l and "*/" not in l:
            sys.exit(f"line {i + 1}: directive or block comment: {l}")
        code, comment = split_comment(l)
        if not balanced(code):
            sys.exit(f"line {i + 1}: unbalanced parentheses: {l}")
        ind = len(code) - len(code.lstrip())
        if code.strip():
            lines[i] = code[:ind] + "KH_DEBUG(" + code[ind:].rstrip() + ")" + (" " + comment if comment else "")
    print(f"{path}: lines {start}-{end} wrapped")
path.write_text("\n".join(lines), encoding="latin-1")
