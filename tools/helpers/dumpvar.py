"""tools/helpers/dumpvar.py <unit> <mangled> <draft-first-line> <variants-file> <n> <outdir> <dflags...>: write the enabled draft with
variant n applied to <outdir>/<unit-base>.cpp and compile it there with the dump flags. Layouts: #if 0 draft #else asm #endif;
#if 0 draft #endif asm; asm note #if 0 draft #endif."""
import subprocess, sys, os
from pathlib import Path
u, name, first, vf, n, out = sys.argv[1:7]; flags = sys.argv[7:]
src = next(p for p in (Path('src')/(u+'.cpp'), Path('src')/(u+'.c')) if p.exists())
L = src.read_text(encoding='latin-1').split('\n')
k = next(i for i,l in enumerate(L) if l.startswith(first)); a = max(i for i in range(k) if L[i].startswith('#if 0'))
e = next(i for i in range(k, len(L)) if L[i].startswith('#else') or L[i].startswith('#endif'))
if L[e].startswith('#else'):
    en = next(i for i in range(e, len(L)) if L[i].startswith('#endif'))
    keep = L[:a] + L[a+1:e] + L[en+1:]
else:
    keep = L[:a] + L[a+1:e] + L[e+1:]
    keep = [l for l in keep if not (l.startswith('INCLUDE_ASM') and ', ' + name + ')' in l)]
t = '\n'.join(keep)
if int(n) >= 0:
    var = open(vf, encoding='latin-1').read().split('\n=====\n')[int(n)]
    for pair in var.split('\n+++++\n'):
        if '\n-----\n' in pair:
            x, y = pair.split('\n-----\n'); assert x in t, x[:50]; t = t.replace(x, y, 1)
os.makedirs(out, exist_ok=True); f = Path(out)/src.name; f.write_text(t, encoding='latin-1')
R = os.path.abspath('.')
subprocess.run([R+'/tools/wibo/wibo', R+'/tools/compilers/ee-gcc2.95.3-136/bin/ee-gcc.exe', '-S', '-O2', '-G0', '-fno-exceptions', '-fno-rtti', '-g',
                '-I', R+'/include', '-I', R+'/src', *flags, '-o', 'out.s', src.name], cwd=out)
