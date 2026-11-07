"""tools/helpers/hillqueue.py <minutes-per-draft> <unit:mangled-prefix>...: for each parked draft, hill-climb the order of every run of
adjacent simple statements (same indent, no braces, labels, returns or preprocessor lines). Best variants go to build/jm/hq_<name>.txt."""
import re, subprocess, sys, time
from pathlib import Path
import os; os.makedirs('build/jm', exist_ok=True)  # scratch output (a fresh worktree has no build/jm)
mins = float(sys.argv[1])
items = [x.split(':', 1) for x in sys.argv[2:]]
sys.argv = sys.argv[:1]
sys.path.insert(0, 'tools/helpers')
from draftlib import blocks  # every draft layout, as tools/drafts.py
SIMPLE = re.compile(r'^\s+[^\s#/].*;\s*(/\*.*\*/|//.*)?$')
BAD = re.compile(r'\b(return|goto|break|continue|case|default)\b|[{}]')
def runs(lines):
    out, i = [], 0
    while i < len(lines):
        ind = len(lines[i]) - len(lines[i].lstrip())
        j = i
        while j < len(lines) and SIMPLE.match(lines[j]) and not BAD.search(lines[j]) and len(lines[j]) - len(lines[j].lstrip()) == ind:
            j += 1
        if j - i >= 3:
            out.append((i, j))
        i = max(j, i + 1)
    return out
IDENT = re.compile(r'[A-Za-z_]\w*')
def lhs(line):
    """The base names a statement writes, or None when it is a call or anything else with unknown effects."""
    g = re.match(r'\s*([^=;(]*?)\s*(?:[-+*/|&^]|<<|>>)?=(?!=)', line)
    if not g or '(' in line.split('=')[0]:
        return None
    return set(IDENT.findall(g.group(1))) - {'_q', 'w'}   # field names shared by every FVECTOR, not a dependency
def conflict(a, b):
    """True if the two statements may not swap: a shared written name, or one is a call and the other writes what it reads."""
    wa, wb = lhs(a), lhs(b)
    ia, ib = set(IDENT.findall(a)), set(IDENT.findall(b))
    if wa is None and wb is None:
        return True          # two calls keep their order
    if wa is None:
        return bool(wb & ia) or not wb.isdisjoint(ia) or 'this' in ia
    if wb is None:
        return bool(wa & ib) or 'this' in ib
    return bool(wa & ib) or bool(wb & ia)
def legal(seq, base):
    """Every pair whose order changed from base must be free to swap."""
    pos = {id(x): i for i, x in enumerate(base)}
    idx = [base.index(x) for x in seq]
    for i in range(len(seq)):
        for j in range(i + 1, len(seq)):
            if idx[i] > idx[j] and conflict(seq[i], seq[j]):
                return False
    return True
def score(u, m, vf=None):
    a = ['.venv/bin/python', 'tools/helpers/showdraft.py', u, m] + ([vf, '0'] if vf else [])
    env = dict(__import__('os').environ, VARLAST='1')
    o = subprocess.run(a, capture_output=True, text=True, env=env).stdout.split('\n')
    if o and o[0].startswith('MATCH'):
        return 0
    g = re.match(r'(\d+) of (\d+)', o[1]) if len(o) > 1 else None
    return int(g[1]) if g else 9999
for u, short in items:
    p = next(q for q in (Path('src') / (u + '.cpp'), Path('src') / (u + '.c')) if q.exists())
    L = p.read_text(encoding='latin-1').split('\n')
    full = re.search(r'INCLUDE_ASM\("[^"]+", (' + re.escape(short) + r'\w*)\);', '\n'.join(L))
    if not full:
        print('gone', short, flush=True); continue
    m = full.group(1)
    b = [x for x in blocks(L) if x[3] == m]
    if not b:
        print('no draft', m, flush=True); continue
    a, el = b[0][0], b[0][1]
    draft = L[a + 1:el]
    rs = runs(draft)
    start = best = score(u, m)
    bestvar = None
    end = time.time() + mins * 60
    vf = f'build/jm/_hq.txt'
    pairs = []
    for ra, rb in sorted(rs, key=lambda r: r[0] - r[1]):
        base = draft[ra:rb]
        cur = list(base)
        improved = True
        while improved and time.time() < end and best > 0:
            improved = False
            for i in range(len(cur)):
                for j in range(len(cur)):
                    if i == j or time.time() > end:
                        continue
                    t = list(cur); x = t.pop(i); t.insert(j, x)
                    if t == base or not legal(t, base):
                        continue
                    var = list(pairs) + ['\n'.join(base) + '\n-----\n' + '\n'.join(t)]
                    Path(vf).write_text('\n+++++\n'.join(var), encoding='latin-1')
                    sc = score(u, m, vf)
                    if sc < best:
                        best, cur, improved = sc, t, True
                        bestvar = var
                        print(f'  {m[:34]} {best} moved {x.strip()[:40]} -> {j}', flush=True)
                        Path(f'build/jm/hq_{m[:40]}.txt').write_text('\n+++++\n'.join(var), encoding='latin-1')
                        break
                if improved or best == 0:
                    break
        if cur != base:
            pairs.append('\n'.join(base) + '\n-----\n' + '\n'.join(cur))
    print(f'{u.split("/")[-1]} {m[:40]} {start} -> {best} ({len(rs)} runs)', flush=True)
print('queue done', flush=True)
