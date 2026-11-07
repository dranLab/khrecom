"""tools/helpers/vtry.py <unit> <func> <variants>: enable the draft (tools/permuter.enable_draft), apply each variant's pairs
(`OLD\n-----\nNEW`, pairs split by `\n+++++\n`, variants by `\n=====\n`; an empty variant = the draft), funcdiff it."""
import subprocess, sys
sys.path.insert(0, 'tools'); import permuter
from pathlib import Path
u, f, vf = sys.argv[1:4]
src = next(p for p in (Path('src')/(u+'.c'), Path('src')/(u+'.cpp')) if p.exists())
obj = f'build/SLUS_217.99/src/{u}.o'
orig = src.read_text(encoding='latin-1'); base = permuter.enable_draft(orig, permuter.definition_pattern(*permuter.demangle(f)), f)
try:
    for n, var in enumerate(open(vf).read().split('\n=====\n')):
        t = base
        for pair in var.split('\n+++++\n'):
            if '\n-----\n' in pair:
                a, b = pair.split('\n-----\n'); assert a in t, (n, a[:60]); t = t.replace(a, b, 1)
        src.write_text(t, encoding='latin-1'); Path(obj).unlink(missing_ok=True)
        r = subprocess.run(['ninja', obj], capture_output=True, text=True)
        if r.returncode: print(n, 'build failed'); continue
        out = subprocess.run(['.venv/bin/python', 'tools/funcdiff.py', u, f], capture_output=True, text=True).stdout
        print(n, ' / '.join(out.splitlines()[:2]))
finally:
    src.write_text(orig, encoding='latin-1'); Path(obj).unlink(missing_ok=True); subprocess.run(['ninja', obj], capture_output=True)
