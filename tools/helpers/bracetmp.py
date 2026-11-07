"""tools/helpers/bracetmp.py <items> : in each draft replace the hand-written temporary block by a real brace init.
items: unit TAB mangled TAB first-line. Prints base, form A (t = {...}; v = t; setDir), form B (t = {...}; setDir; v = t)."""
import re, subprocess, sys
from pathlib import Path
exec(open('tools/helpers/bracetry2.py').read().split("Q = re.compile")[0].split('ITEMS = ')[0])
ITEMS = [l.rstrip('\n').split('\t') for l in open(sys.argv[1]) if l.strip()]
exec("def enable" + open('tools/helpers/bracetry2.py').read().split("def enable")[1].split("Q = re.compile")[0])
BLK = re.compile(r'^( *)\{\n(?: */\*(?:.|\n)*?\*/\n)? *FVECTOR (\w+);\n *\2\._q = 0;\n *\2\.y = ([^;\n]+);\n *(\w+) = \2;\n *\}\n(?: *(setDir\([^;\n]*\);)\n)?', re.M)
def form(m, b):
    ind, t, e, v, sd = m.group(1), m.group(2), m.group(3), m.group(4), m.group(5)
    s = f'{ind}{{\n{ind}    FVECTOR {t} = {{ 0.0f, {e}, 0.0f, 0.0f }};\n'
    if b and sd:
        s += f'{ind}    {sd}\n{ind}    {v} = {t};\n'
    else:
        s += f'{ind}    {v} = {t};\n' + (f'{ind}    {sd}\n' if sd else '')
    return s + f'{ind}}}\n'
for item in ITEMS:
    u, name, first = item[:3]
    apply_f = item[3] if len(item) > 3 else None
    src = next(p for p in (Path('src')/(u+'.cpp'), Path('src')/(u+'.c')) if p.exists()); obj = Path(f'build/SLUS_217.99/src/{u}.o')
    orig = src.read_text(encoding='latin-1'); keep = None
    try:
        L, s, a, el, en = enable(orig, first)
        d = '\n'.join(L[a+1:el]) + '\n'
        head = '\n'.join(L[:a]) + '\n'; tail = '\n'.join(L[en+1:])
        print(name[:50], 'sites', len(BLK.findall(d)), 'base', score(u, name, src, obj, head + d + tail), flush=True)
        for fm, b in (('A', 0), ('B', 1)):
            d2 = BLK.sub(lambda m: form(m, b), d)
            r = score(u, name, src, obj, head + d2 + tail)
            print('  ', fm, r, flush=True)
            if apply_f == fm:
                keep = '\n'.join(L[:s]) + '\n' + d2 + tail
    except StopIteration:
        print(name[:50], 'draft not found')
    finally:
        src.write_text(keep if keep else orig, encoding='latin-1'); obj.unlink(missing_ok=True); subprocess.run(['ninja', str(obj)], capture_output=True)
