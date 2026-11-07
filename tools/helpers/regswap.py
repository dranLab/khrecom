"""tools/helpers/regswap.py [max-lines]: classify parked game drafts: diff lines that are a pure register permutation vs other. Sorted, pure first."""
import os, re, subprocess, sys
import sys as _sys; _sys.path.insert(0, 'tools/helpers')  # draftlib
lim = int(sys.argv[1]) if len(sys.argv) > 1 else 60
REG = re.compile(r'^\$?(?:zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|fp|ra|gp|sp|f\d+)$')
def toks(s):
    return re.findall(r'[\w$.]+', s)
rows = []
for _u, _s, _n in __import__('draftlib').rows():
    c = [_u, _s, str(_n)]
    if not (len(c) >= 3 and c[2].isdigit()) or c[0].startswith('lib/') or int(c[2]) > lim:
        continue
    u, short = c[0], c[1]
    src = next((p for p in (f'src/{u}.cpp', f'src/{u}.c') if os.path.exists(p)), None)
    if not src:
        continue
    t = open(src, encoding='latin-1').read()
    m = re.search(r'INCLUDE_ASM\("[^"]+", (' + re.escape(short) + r'\w*)\);', t)
    if not m:
        continue
    o = subprocess.run(['.venv/bin/python', 'tools/helpers/showdraft.py', u, m.group(1)], capture_output=True, text=True).stdout.split('\n')
    pure, other = 0, 0
    pairs = {}
    size = 'size' if len(o) > 0 and 'size 0x' in o[0] else ''
    for x in o[2:]:
        if ' | ' not in x and not x.startswith('  | ') and not x.rstrip().endswith('|'):
            continue
        a, _, b = x.partition(' | ')
        ta, tb = toks(a), toks(b)
        if not a.strip() or not b.strip() or len(ta) != len(tb):
            other += 1
            continue
        ok = True
        loc = []
        for p, q in zip(ta, tb):
            if p == q:
                continue
            if REG.match(p) and REG.match(q):
                loc.append((p, q))
            else:
                ok = False
        if ok:
            pure += 1
            for p, q in loc:
                pairs[p] = pairs.get(p, 0) + 1
                pairs[p + '>' + q] = pairs.get(p + '>' + q, 0) + 1
        else:
            other += 1
    mp = ' '.join(sorted(k for k in pairs if '>' in k))
    rows.append((other, pure, u.split('/')[-1], m.group(1)[:40], size, mp[:60]))
    print(other, pure, u.split('/')[-1], m.group(1)[:40], size, mp[:60], flush=True)
print('=== sorted')
for r in sorted(rows):
    print(*r)
