"""tools/helpers/reflocals.py [unit-prefix]: for every parked draft, list the ref's locals missing from the draft's text.
A missing local usually means the draft's source shape differs from the original's (invented loop, lost temporary)."""
import re, subprocess, sys
from pathlib import Path
want = sys.argv[1] if len(sys.argv) > 1 else ''
sys.argv = sys.argv[:1]
sys.path.insert(0, 'tools/helpers')
from draftlib import blocks  # every draft layout, as tools/drafts.py
rows = []
for p in sorted(Path('src').rglob('*.cpp')):
    u = str(p.relative_to('src'))[:-4]
    if not u.startswith(want):
        continue
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
        draft = '\n'.join(L[a + 1:el])
        m = re.search(r'^[^\n/#][^\n]*?(\w+)\([^\n]*\{\s*$', draft, re.M)
        if not m:
            continue
        name = m.group(1)
        hit = None
        for x in re.finditer(r'^[^\n\t/#][^\n]*\b' + re.escape(name) + r'\([^\n]*\{\s*$', ref, re.M):
            e = ref.find('\n}', x.start())
            hit = ref[x.end():e]
            # several overloads: take the first whose owner matches the mangled name
            own = x.group(0)
            if '::' in own and own.split('::')[0].split()[-1] in mangled:
                break
        if hit is None:
            continue
        names = []
        for line in hit.split('\n'):
            q = re.match(r'\s*(?:[\w:<>]+[\s*&]+)+(\w+)(?:\[[^\]]*\])*\s*(?:=.*)?;', line)
            if q:
                names.append(q.group(1))
        miss = sorted({n for n in names if not re.search(r'\b' + re.escape(n) + r'\b', draft)})
        if miss:
            rows.append((len(miss), u, mangled[:44], ' '.join(miss)[:70]))
rows.sort(key=lambda x: (-x[0], x[1]))
for n, u, m, ms in rows:
    print(f'{n:3d} {u[:34]:34s} {m:44s} {ms}')
print(len(rows), 'drafts with missing locals')
