"""tools/helpers/permlines.py <unit> <mangled> <file with adjacent lines of the draft, one statement per line> [max]:
score every permutation of those lines; stops at a MATCH; prints the best orders."""
import itertools, re, subprocess, sys
import os; os.makedirs('build/jm', exist_ok=True)  # scratch output (a fresh worktree has no build/jm)
u, m, f = sys.argv[1:4]
mx = int(sys.argv[4]) if len(sys.argv) > 4 else 120
lines = open(f, encoding='latin-1').read().rstrip('\n').split('\n')
old = '\n'.join(lines)
res = []
vf = 'build/jm/_perm.txt'
for i, p in enumerate(itertools.permutations(lines)):
    if i >= mx:
        break
    new = '\n'.join(p)
    if new == old:
        continue
    open(vf, 'w', encoding='latin-1').write(old + "\n-----\n" + new)
    env = dict(__import__('os').environ, VARLAST='1')
    o = subprocess.run(['.venv/bin/python', 'tools/helpers/showdraft.py', u, m, vf, '0'], capture_output=True, text=True, env=env).stdout
    h = o.split('\n')
    if h and h[0].startswith('MATCH'):
        print('MATCH', [l.strip()[:40] for l in p]); res = [];
        open('build/jm/_perm_best.txt', 'w', encoding='latin-1').write(old + "\n-----\n" + new)
        sys.exit(0)
    g = re.match(r'(\d+) of (\d+)', h[1]) if len(h) > 1 else None
    size = ' '.join(h[0].split()[2:8]) if h else ''
    if g:
        res.append((int(g[1]), new, h[0][-40:]))
res.sort(key=lambda x: x[0])
for n, new, s in res[:3]:
    print(n, [l.strip()[:34] for l in new.split('\n')])
if res:
    open('build/jm/_perm_best.txt', 'w', encoding='latin-1').write(old + "\n-----\n" + res[0][1])
