"""tools/helpers/refcount.py [max-lines]: parked drafts declaring a local fewer times than the ref does (shared variable where the
original has one per scope). Inline parameters are noise: only names the draft itself declares are compared."""
import re, sys
from pathlib import Path
lim = int(sys.argv[1]) if len(sys.argv) > 1 else 80
sys.argv = sys.argv[:1]
sys.path.insert(0, 'tools/helpers')
from draftlib import blocks  # every draft layout, as tools/drafts.py
cnt = {}
for _u, _s, _n in __import__('draftlib').rows():
    c = [_u, _s, str(_n)]
    if len(c) >= 3 and c[2].isdigit():
        cnt[(c[0], c[1])] = int(c[2])
DECL = re.compile(r'(?:^|[({;])\s*(?:const\s+|volatile\s+|register\s+|static\s+)*(?:unsigned\s+|struct\s+)?[A-Za-z_]\w*(?:::\w+)*[\s*&]+(?:const\s+)?([a-zA-Z_]\w*)\s*(?:\[[^\]]*\])*\s*(?:=[^=;]*)?[;,]')
KW = {'return', 'goto', 'delete', 'new', 'else', 'case', 'break', 'continue', 'typedef', 'sizeof'}
rows = []
for p in sorted(Path('src').rglob('*.cpp')):
    u = str(p.relative_to('src'))[:-4]
    r = Path('ref/SLUS_217.99/src') / (u + '.cpp')
    if not r.exists():
        continue
    L = p.read_text(encoding='latin-1').split('\n')
    try:
        B = blocks(L)
    except Exception:
        continue
    ref = r.read_text(encoding='latin-1')
    for a, el, en, mangled, inc in B:
        text = '\n'.join(L[a + 1:el])
        m = re.search(r'^[^\n/#][^\n]*?(\w+)\(([^\n]*)\)[^\n]*\{\s*$', text, re.M)
        if not m:
            continue
        name = m.group(1)
        nparams = len([x for x in m.group(2).split(',') if x.strip()])
        best = None
        for x in re.finditer(r'^[^\n\t/#][^\n]*\b' + re.escape(name) + r'\(([^\n]*)\)[^\n]*\{\s*$', ref, re.M):
            np = len([q for q in x.group(1).split(',') if q.strip()])
            if np == nparams and ('::' not in x.group(0) or x.group(0).split('::')[0].split()[-1] in mangled):
                best = ref[x.end():ref.find('\n}', x.start())]
                break
        if best is None:
            continue
        rc = {}
        for line in best.split('\n'):
            q = re.match(r'\s*[\w:<>]+(?:\s+[\w:<>]+)*[\s*&]+(\w+)(?:\[[^\]]*\])*;', line)
            if q:
                rc[q.group(1)] = rc.get(q.group(1), 0) + 1
        dc = {}
        for d in DECL.finditer(text[m.end():]):
            n = d.group(1)
            if n not in KW:
                dc[n] = dc.get(n, 0) + 1
        short = [f'{n} {dc[n]}<{rc[n]}' for n in dc if n in rc and n not in ('this',) and dc[n] < rc[n]]
        if short:
            k = next((v for (uu, mm), v in cnt.items() if uu == u and mangled.startswith(mm)), None)
            if k is not None and k <= lim:
                rows.append((k, u, mangled, ' '.join(short)))
rows.sort()
for k, u, m, e in rows:
    print(f'{k:4d} {u[:30]:30s} {m[:40]:40s} {e[:50]}')
print(len(rows))
