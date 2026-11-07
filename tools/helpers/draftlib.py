"""Shared by the helpers in tools/helpers/: where the parked drafts are and how far off they measure.

blocks(L)  every parked draft of a unit, given its source lines: (if0, end, endif, symbol, inc) with `#if 0` at L[if0],
           the draft text in L[if0 + 1:end] (end = its `#else` or `#endif`), the `#endif` at L[endif] and the
           function's INCLUDE_ASM at L[inc]. Found like tools/drafts.py does: by the function's symbol, wherever the
           draft sits relative to its INCLUDE_ASM (in the #else, after the #endif, past a placeholder).
rows()     (unit, symbol, lines) for every measured draft, from tools/drafts.py's output in build/drafts.log:
               .venv/bin/python tools/drafts.py -j 12 > build/drafts.log
           (lines = funcdiff's differing lines; drafts that do not build or have no draft are left out).

Run the helpers from the repository root.
"""
import re
import sys
from pathlib import Path

sys.path.insert(0, 'tools')
from permuter import definition_pattern, demangle  # noqa: E402

INC = re.compile(r'^INCLUDE_ASM\("[^"]*",\s*([^)\s]+)\s*\);')
COND = re.compile(r'^\s*#\s*(if|ifdef|ifndef|else|elif|endif)\b(.*)$')
DRAFTS_LOG = Path('build/drafts.log')


def _disabled_if0(L, d):
    """Index of the `#if 0` line whose region contains line d (nesting counted), or None."""
    stack = []  # (line index, is #if 0 and still in its first branch)
    for i in range(d):
        m = COND.match(L[i])
        if not m:
            continue
        kind = m.group(1)
        if kind.startswith('if'):
            stack.append((i, kind == 'if' and m.group(2).strip().split('/')[0].strip() == '0'))
        elif kind in ('else', 'elif') and stack:
            stack[-1] = (stack[-1][0], False)
        elif kind == 'endif' and stack:
            stack.pop()
    for i, off in reversed(stack):
        if off:
            return i
    return None


def _block_end(L, a):
    """(end of the first branch: its #else/#elif or #endif, the #endif) of the #if at L[a]."""
    depth, first_end = 0, None
    for i in range(a, len(L)):
        m = COND.match(L[i])
        if not m:
            continue
        kind = m.group(1)
        if kind.startswith('if'):
            depth += 1
        elif kind in ('else', 'elif') and depth == 1 and first_end is None:
            first_end = i
        elif kind == 'endif':
            depth -= 1
            if depth == 0:
                return (first_end if first_end is not None else i), i
    return None, None


def blocks(L):
    text = '\n'.join(L)
    starts = [0]
    for line in L:
        starts.append(starts[-1] + len(line) + 1)
    line_of = lambda pos: next(i for i in range(len(starts) - 1) if starts[i] <= pos < starts[i + 1])
    out = []
    for inc, line in enumerate(L):
        m = INC.match(line)
        if not m or _disabled_if0(L, inc) is not None:
            continue
        symbol = m.group(1)
        name, cls = demangle(symbol)
        for d in re.finditer(definition_pattern(name, cls), text, re.M):
            dl = line_of(d.start())
            a = _disabled_if0(L, dl)
            if a is None:
                continue
            end, endif = _block_end(L, a)
            if end is not None and a < dl < end:
                out.append((a, end, endif, symbol, inc))
                break
    return out


def rows():
    if not DRAFTS_LOG.exists():
        sys.exit(f'{DRAFTS_LOG} missing: run `.venv/bin/python tools/drafts.py -j 12 > {DRAFTS_LOG}` first')
    out = []
    for line in DRAFTS_LOG.read_text(errors='replace').split('\n'):
        c = line.split()
        if len(c) >= 3 and c[0].isdigit():
            out.append((c[1], c[2], int(c[0])))
    return out
