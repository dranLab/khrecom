"""Wrap source lines START..END (1-based, inclusive) one by one in KH_DEBUG(...): line-neutral. A statement whose
parentheses don't balance on its line (a macro argument must) is joined onto its first line from the following lines
of the range, which stay empty (the beta's line note stays on the statement's first line). Already wrapped lines
are left alone.
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
    in_comment = False
    for i in range(start - 1, end):
        l = lines[i]
        if in_comment:  # a comment's continuation line: nothing to wrap
            if "*/" in l:
                in_comment = False
                if l.split("*/", 1)[1].strip():
                    sys.exit(f"line {i + 1}: code after a block comment: {l}")
            continue
        if not l.strip() or l.lstrip().startswith(("KH_DEBUG(", "#")):
            continue  # a preprocessor directive stays as it is (#line, #define: the same in both versions)
        if "/*" in l and "*/" not in l.split("/*", 1)[1]:
            if l.split("/*", 1)[0].strip():
                sys.exit(f"line {i + 1}: code before a multi-line block comment: {l}")
            in_comment = True
            continue
        code, comment = split_comment(l)
        k = i
        while not balanced(code) and k + 1 < end and not comment:
            k += 1
            nxt, comment = split_comment(lines[k])
            if lines[k].lstrip().startswith("#") or "/*" in nxt:
                break
            code = code.rstrip() + " " + nxt.strip()
            lines[k] = ""
        if not balanced(code):
            sys.exit(f"line {i + 1}: unbalanced parentheses: {l}")
        ind = len(code) - len(code.lstrip())
        if code.strip():
            lines[i] = code[:ind] + "KH_DEBUG(" + code[ind:].rstrip() + ")" + (" " + comment if comment else "")
    print(f"{path}: lines {start}-{end} wrapped")
path.write_text("\n".join(lines), encoding="latin-1")
