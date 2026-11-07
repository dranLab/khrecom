"""tools/helpers/en.py <unit> <mangled> <draft-first-line> <lines> [variants n]: enable a draft that sits apart from its INCLUDE_ASM (dropping the
INCLUDE_ASM line and the #if 0/#endif pair), optionally apply a variant, build the object, print the diff; restores the file."""
import re, subprocess, sys
from pathlib import Path
u, m, first = sys.argv[1:4]
n = int(sys.argv[4])
vf = sys.argv[5] if len(sys.argv) > 5 else None
k = int(sys.argv[6]) if len(sys.argv) > 6 else 0
p = next(q for q in (Path('src') / (u + '.cpp'), Path('src') / (u + '.c')) if q.exists())
raw = p.read_bytes(); s = raw.decode('latin-1')
try:
    L = s.split('\n')
    i = next(j for j, l in enumerate(L) if l.startswith('INCLUDE_ASM') and ', ' + m + ')' in l)
    L[i] = ''
    c = [j for j, l in enumerate(L) if l.startswith(first)]; d = next((j for j in c if L[j - 1].startswith('#if 0')), c[0])
    a = max(j for j in range(d) if L[j].startswith('#if 0'))
    e = next(j for j in range(d, len(L)) if L[j].startswith('#endif'))
    L[a] = ''; L[e] = ''
    s = '\n'.join(L)
    if vf:
        for pair in open(vf, encoding='latin-1').read().split('\n=====\n')[k].split('\n+++++\n'):
            x, y = pair.split('\n-----\n')
            assert x in s, x[:50]
            s = s.replace(x, y, 1)
    p.write_bytes(s.encode('latin-1'))
    r = subprocess.run(['ninja', f'build/SLUS_217.99/src/{u}.o'], capture_output=True, text=True)
    if r.returncode:
        print('build failed', [l[:150] for l in r.stdout.split('\n') if ('.cpp:' in l or '.c:' in l) and 'warning' not in l and 'In ' not in l][:4], r.stdout[-200:])
    o = subprocess.run(['.venv/bin/python', 'tools/funcdiff.py', u, m], capture_output=True, text=True).stdout
    print('\n'.join(re.sub(r'   +', '  ', l)[:78] for l in o.split('\n')[:n * 2] if 'R_MIPS' not in l))
finally:
    p.write_bytes(raw)
