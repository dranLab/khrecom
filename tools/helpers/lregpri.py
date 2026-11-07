"""tools/helpers/lregpri.py <fn.lreg> [limit] [greg?]: registers with their global-alloc priority (floor_log2(refs)*refs/live*size), sorted."""
import re, sys, math
t = open(sys.argv[1], encoding='latin-1').read()
lim = int(sys.argv[2]) if len(sys.argv) > 2 else 30
rows = []
for m in re.finditer(r'Register (\d+) used (\d+) times? across (\d+) insns(.*?)\.\n', t):
    r, n, l, rest = int(m[1]), int(m[2]), int(m[3]), m[4]
    p = (math.floor(math.log2(n)) * n / l * 10000) if n > 0 and l > 0 else 0
    rows.append((p, r, n, l, 'crosses' in rest, 'user' in rest, re.search(r'set (\d+)', rest)))
rows = [x for x in rows if x[4]]
rows.sort(key=lambda x: -x[0])
for p, r, n, l, c, u, s in rows[:lim]:
    print(f'{r:5d} pri {p:9.1f} refs {n:4d} live {l:5d} set {s.group(1) if s else "?"} {"user" if u else ""}')
