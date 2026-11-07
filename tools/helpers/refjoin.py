"""tools/helpers/refjoin.py [max-lines]: join build/jm/reflocals.log with build/drafts.log (tools/drafts.py) (differing lines), noise names removed."""
import sys
import sys as _sys; _sys.path.insert(0, 'tools/helpers')  # draftlib
lim = int(sys.argv[1]) if len(sys.argv) > 1 else 40
cnt = {}
for _u, _s, _n in __import__('draftlib').rows():
    c = [_u, _s, str(_n)]
    if len(c) >= 3 and c[2].isdigit():
        cnt[(c[0], c[1][:44])] = int(c[2])
noise = {'this', 'nRA', 'r', 'v', 'x', 'y', 'z', 'w', '_x', '_y', '_z', '_w', '__pn', 'ret', 'id', 'flag'}
rows = []
for l in open('build/jm/reflocals.log'):
    c = l.split()
    if len(c) < 4 or not c[0].isdigit():
        continue
    u, m, names = c[1], c[2], [x for x in c[3:] if x not in noise]
    n = next((v for (a, b), v in cnt.items() if a.startswith(u[:34]) and (b.startswith(m) or m.startswith(b))), None)
    if names and n is not None and n <= lim:
        rows.append((n, u, m, ' '.join(names)))
rows.sort()
for n, u, m, ms in rows:
    print(f'{n:4d} {u[:30]:30s} {m[:40]:40s} {ms[:60]}')
print(len(rows))
