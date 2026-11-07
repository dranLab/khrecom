"""tools/helpers/banddiff.py <lo> <hi> [skip-substrings...]: compact current diffs of parked game drafts with lo..hi differing lines."""
import re, subprocess, sys
import sys as _sys; _sys.path.insert(0, 'tools/helpers')  # draftlib
lo, hi = int(sys.argv[1]), int(sys.argv[2])
skip = sys.argv[3:]
rows = []
for _u, _s, _n in __import__('draftlib').rows():
    c = [_u, _s, str(_n)]
    if len(c) >= 3 and c[2].isdigit() and lo <= int(c[2]) <= hi and not c[0].startswith('lib/'):
        rows.append(c)
for c in rows:
    u, short = c[0], c[1]
    if any(s in short for s in skip):
        continue
    src = next((p for p in (f'src/{u}.cpp', f'src/{u}.c') if __import__('os').path.exists(p)), None)
    if not src:
        continue
    t = open(src, encoding='latin-1').read()
    m = re.search(r'INCLUDE_ASM\("[^"]+", (' + re.escape(short) + r'\w*)\);', t)
    if not m:
        print('---', u, short, 'gone'); continue
    o = subprocess.run(['.venv/bin/python', 'tools/helpers/showdraft.py', u, m.group(1)], capture_output=True, text=True).stdout.split('\n')
    head = o[1][:34] if len(o) > 1 else ''
    print('---', u.split('/')[-1], m.group(1)[:46], '|', head)
    n = 0
    for x in o[2:]:
        if ' | ' in x or x.rstrip().endswith('|') or x.startswith('  | '):
            print('   ', x[:64]); n += 1
            if n >= 16:
                print('    ...'); break
