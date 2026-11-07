"""tools/helpers/showdraft.py <unit> <mangled> [variants-file n]: enable one parked draft (any layout), apply a variant, print the funcdiff (compact)."""
import re, subprocess, sys
from pathlib import Path
sys.path.insert(0, 'tools')
from permuter import definition_pattern, demangle, enable_draft  # finds the draft in any layout, as tools/drafts.py
sys.argv, args = sys.argv[:1], sys.argv[1:]
u, name = args[0], args[1]
vf = args[2] if len(args) > 2 else None; n = int(args[3]) if len(args) > 3 else 0
p = next(q for q in (Path('src')/(u+'.cpp'), Path('src')/(u+'.c')) if q.exists()); obj = Path(f'build/SLUS_217.99/src/{u}.o')
raw = p.read_bytes(); text = raw.decode('latin-1')
T = enable_draft(text, definition_pattern(*demangle(name)), name)
if T == text:
    sys.exit(f'{name}: no parked draft found in {p}')
if vf:
    for pair in open(vf).read().split('\n=====\n')[n].split('\n+++++\n'):
        x, y = pair.split('\n-----\n'); assert x in T, x[:60]; i = T.rindex(x) if __import__('os').environ.get('VARLAST') else T.index(x); T = T[:i] + y + T[i + len(x):]
try:
    p.write_bytes(T.encode('latin-1')); obj.unlink(missing_ok=True)
    r = subprocess.run(['ninja', str(obj)], capture_output=True, text=True)
    if r.returncode:
        print('build failed', [l[:170] for l in r.stdout.split('\n') if '.cpp:' in l and 'warning' not in l or '.c:' in l and 'warning' not in l][:4]); print(r.stdout[-500:])
    o = subprocess.run(['.venv/bin/python', 'tools/funcdiff.py', u, name] + (['--full'] if __import__('os').environ.get('FULL') else []), capture_output=True, text=True).stdout
    for l in o.split('\n'):
        if 'R_MIPS' in l:
            continue
        print(re.sub(r'   +', '  ', l)[:80])
finally:
    p.write_bytes(raw); obj.unlink(missing_ok=True); subprocess.run(['ninja', str(obj)], capture_output=True)
