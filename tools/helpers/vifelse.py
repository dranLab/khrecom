"""tools/helpers/vifelse.py <unit> <mangled> <draft-first-line> <variants (pairs as vold)>: enable an `#if 0 draft #else ... #endif` block
whose first draft line starts with <draft-first-line>, drop its #else branch, apply pairs, score."""
import subprocess, sys
from pathlib import Path
u, name, first, vf = sys.argv[1:5]
src = next(p for p in (Path('src')/(u+'.cpp'), Path('src')/(u+'.c')) if p.exists()); obj = Path(f'build/SLUS_217.99/src/{u}.o')
orig = src.read_text(encoding='latin-1'); L = orig.split('\n')
k = next(i for i,l in enumerate(L) if l.startswith(first) and not l.rstrip().endswith(';')); a = max(i for i in range(k) if L[i].startswith('#if 0'))
el = next(i for i in range(k, len(L)) if L[i].startswith('#else')); en = next(i for i in range(el, len(L)) if L[i].startswith('#endif'))
base = '\n'.join(L[:a] + L[a+1:el] + L[en+1:])
try:
    for n, var in enumerate(open(vf).read().split('\n=====\n')):
        t = base
        for pair in var.split('\n+++++\n'):
            if '\n-----\n' in pair:
                x, y = pair.split("\n-----\n"); assert x in t, x[:50]; t = t.replace(x, y, 1)
        src.write_text(t, encoding='latin-1'); obj.unlink(missing_ok=True)
        r = subprocess.run(['ninja', str(obj)], capture_output=True, text=True)
        if r.returncode: print(n, 'build failed', [l for l in r.stdout.splitlines() if 'rror' in l or 'Segm' in l][:2]); continue
        f = subprocess.run(['.venv/bin/python', 'tools/funcdiff.py', u, name], capture_output=True, text=True).stdout
        import os; print(n, "\n".join(f.splitlines() if os.environ.get("SHOW")==str(n) else f.splitlines()[:2]))
finally:
    src.write_text(orig, encoding='latin-1'); obj.unlink(missing_ok=True); subprocess.run(['ninja', str(obj)], capture_output=True)
