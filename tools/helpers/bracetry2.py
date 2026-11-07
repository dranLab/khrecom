"""tools/helpers/bracetry2.py <items>: generalized brace-init sweep: X._q = 0; [X.f = expr;]* -> FVECTOR X = {...}.
items: unit TAB mangled TAB first-line [TAB apply-index]. With apply-index the rewrite stays applied (draft un-parked)."""
import re, subprocess, sys
from pathlib import Path
ITEMS = [l.rstrip('\n').split('\t') for l in open(sys.argv[1]) if l.strip()]

def enable(t, first):
    L = t.split('\n')
    k = next(i for i, l in enumerate(L) if l.startswith(first))
    a = max(i for i in range(k) if L[i].startswith('#if 0'))
    el = next(i for i in range(k, len(L)) if L[i].startswith('#else'))
    en = next(i for i in range(el, len(L)) if L[i].startswith('#endif'))
    s = a
    j = a - 1
    if L[j].rstrip().endswith('*/'):
        while '/*' not in L[j]:
            j -= 1
        if L[j].lstrip().startswith('/* Parked'):
            s = j
    return L, s, a, el, en

def score(u, name, src, obj, text):
    src.write_text(text, encoding='latin-1'); obj.unlink(missing_ok=True)
    r = subprocess.run(['ninja', str(obj)], capture_output=True, text=True)
    if r.returncode:
        return 'build failed'
    o = subprocess.run(['.venv/bin/python', 'tools/funcdiff.py', u, name], capture_output=True, text=True).stdout.split('\n')
    open('build/last.diff', 'w').write('\n'.join(o))
    if o[0].startswith('MATCH'):
        return 'MATCH'
    return o[1].split(' of ')[0] + ' ' + o[1].split('(')[0].split()[2] + ' sz' + o[0].split('ours ')[1].split(' ')[0] if len(o) > 1 and ' of ' in o[1] else o[0][:50]

Q = re.compile(r'^( *)(\w+)\._q = 0;\n((?:\s*\2\.[xyzw] = [^;\n]+;\n)*)', re.M)
for item in ITEMS:
    u, name, first = item[:3]
    apply_i = int(item[3]) if len(item) > 3 else None
    src = next(p for p in (Path('src')/(u+'.cpp'), Path('src')/(u+'.c')) if p.exists()); obj = Path(f'build/SLUS_217.99/src/{u}.o')
    orig = src.read_text(encoding='latin-1')
    keep = None
    try:
        L, s, a, el, en = enable(orig, first)
        d = '\n'.join(L[a+1:el]) + '\n'
        head = '\n'.join(L[:a]) + '\n'; tail = '\n'.join(L[en+1:])
        print(name[:50], 'base', score(u, name, src, obj, head + d + tail), flush=True)
        for i, m in enumerate(Q.finditer(d)):
            ind, v, rest = m.group(1), m.group(2), m.group(3)
            vals = {'x': '0.0f', 'y': '0.0f', 'z': '0.0f', 'w': '0.0f'}
            for f, e in re.findall(r'%s\.([xyzw]) = ([^;\n]+);' % v, rest):
                vals[f] = e
            br = f"{ind}FVECTOR {v} = {{ {vals['x']}, {vals['y']}, {vals['z']}, {vals['w']} }};\n"
            d2 = d[:m.start()] + br + d[m.end():]
            d3, n = re.subn(r'^ *FVECTOR %s;\n' % v, '', d2, count=1, flags=re.M)
            r = score(u, name, src, obj, head + d3 + tail)
            print('  ', i, v, n, br.strip()[:60], r, flush=True)
            if i == apply_i:
                keep = '\n'.join(L[:s]) + '\n' + d3 + tail
    except StopIteration:
        print(name[:50], 'draft not found')
    finally:
        src.write_text(keep if keep else orig, encoding='latin-1'); obj.unlink(missing_ok=True); subprocess.run(['ninja', str(obj)], capture_output=True)
