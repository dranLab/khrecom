"""tools/helpers/refextra.py [max-lines]: parked drafts declaring locals the ref does not list (invented variables). Sorted by differing lines."""
import re, sys
from pathlib import Path
lim = int(sys.argv[1]) if len(sys.argv) > 1 else 60
sys.argv = sys.argv[:1]
sys.path.insert(0, 'tools/helpers')
from draftlib import blocks  # every draft layout, as tools/drafts.py
cnt = {}
for _u, _s, _n in __import__('draftlib').rows():
    c = [_u, _s, str(_n)]
    if len(c) >= 3 and c[2].isdigit():
        cnt[(c[0], c[1])] = int(c[2])
DECL = re.compile(r'^\s+(?:const\s+|volatile\s+|register\s+|static\s+)*(?:unsigned\s+|struct\s+)?([A-Za-z_]\w*(?:::\w+)*)[\s*&]+(?:const\s+)?([a-zA-Z_]\w*)\s*(?:\[[^\]]*\])*\s*(?:=[^=].*|asm.*)?;')
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
    if not B:
        continue
    ref = r.read_text(encoding='latin-1')
    for a, el, en, mangled, inc in B:
        draft = L[a + 1:el]
        text = '\n'.join(draft)
        m = re.search(r'^[^\n/#][^\n]*?(\w+)\([^\n]*\{\s*$', text, re.M)
        if not m:
            continue
        name = m.group(1)
        hit = None
        for x in re.finditer(r'^[^\n\t/#][^\n]*\b' + re.escape(name) + r'\([^\n]*\{\s*$', ref, re.M):
            e = ref.find('\n}', x.start())
            hit = ref[x.start():e]
            own = x.group(0)
            if '::' in own and own.split('::')[0].split()[-1] in mangled:
                break
        if hit is None:
            continue
        names = set(re.findall(r'\b([A-Za-z_]\w*)\s*(?:\[[^\]]*\])*\s*[;,)]', hit))
        extra = []
        for l in draft[1:]:
            d = DECL.match(l)
            if d and d.group(1) not in KW and d.group(2) not in names and d.group(2) not in extra:
                extra.append(d.group(2))
        if extra:
            n = next((v for (uu, mm), v in cnt.items() if uu == u and mangled.startswith(mm)), None)
            if n is not None and n <= lim:
                rows.append((n, u, mangled, ' '.join(extra)))
rows.sort()
for n, u, m, e in rows:
    print(f'{n:4d} {u[:30]:30s} {m[:40]:40s} {e[:56]}')
print(len(rows))
