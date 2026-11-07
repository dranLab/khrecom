"""tools/helpers/hillstmt.py <unit> <mangled> <first-line-prefix> <last-line-prefix> <minutes>: hill-climb the order of the single-line
statements between two draft lines: move one statement to each other position, keep any improvement. Scored by showdraft."""
import re, subprocess, sys, time
from pathlib import Path
import os; os.makedirs('build/jm', exist_ok=True)  # scratch output (a fresh worktree has no build/jm)
u, m, first, last, mins = sys.argv[1:6]
end = time.time() + float(mins) * 60
p = next(q for q in (Path('src') / (u + '.cpp'), Path('src') / (u + '.c')) if q.exists())
L = p.read_text(encoding='latin-1').split('\n')
a = next(i for i, l in enumerate(L) if l.startswith(first))
b = next(i for i in range(a, len(L)) if L[i].startswith(last))
base = L[a:b]
vf = 'build/jm/_hill.txt'
def score(lines):
    if lines == base:
        Path(vf).write_text('', encoding='latin-1')
        args = ['.venv/bin/python', 'tools/helpers/showdraft.py', u, m]
    else:
        Path(vf).write_text('\n'.join(base) + '\n-----\n' + '\n'.join(lines), encoding='latin-1')
        args = ['.venv/bin/python', 'tools/helpers/showdraft.py', u, m, vf, '0']
    o = subprocess.run(args, capture_output=True, text=True).stdout.split('\n')
    if o and o[0].startswith('MATCH'):
        return 0
    g = re.match(r'(\d+) of (\d+)', o[1]) if len(o) > 1 else None
    return int(g[1]) if g else 9999
cur = list(base)
best = score(cur)
print('start', best, 'statements', len(cur), flush=True)
improved = True
while improved and time.time() < end and best > 0:
    improved = False
    for i in range(len(cur)):
        for j in range(len(cur)):
            if i == j or time.time() > end:
                continue
            t = list(cur)
            x = t.pop(i)
            t.insert(j, x)
            s = score(t)
            if s < best:
                best, cur, improved = s, t, True
                print(best, 'moved', x.strip()[:44], '->', j, flush=True)
                Path('build/jm/hill_best.txt').write_text('\n'.join(base) + '\n-----\n' + '\n'.join(cur), encoding='latin-1')
                break
        if improved or best == 0:
            break
print('done', best, flush=True)
