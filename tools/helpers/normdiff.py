"""tools/helpers/normdiff.py <file-with-funcdiff---full-output>: register-blind diff of the two columns."""
import re, sys, difflib
lines = open(sys.argv[1], encoding='latin-1').read().split('\n')
A = []; B = []
for l in lines[2:]:
    if 'R_MIPS' in l or l.startswith('   ...'):
        continue
    m = re.match(r'^(.{0,}?)\s{2,}([|~] )?(.*)$', l)
    if not m:
        continue
    a, b = m.group(1).strip(), m.group(3).strip()
    if '|' in l[:0]:
        pass
    if a: A.append(a)
    if b: B.append(b)
def N(s):
    s = re.sub(r'\b(s\d|s8|fp|v\d|a\d|t\d)\b', 'R', s)
    s = re.sub(r'\$f\d+', 'F', s)
    s = re.sub(r'<\+0x[0-9a-f]+>', 'L', s)
    return s
na = [N(x) for x in A]; nb = [N(x) for x in B]
sm = difflib.SequenceMatcher(None, na, nb, autojunk=False)
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal':
        continue
    print(f'{tag} orig[{i1}:{i2}] ours[{j1}:{j2}]')
    for x in A[max(0, i1-2):i2+2][:14]: print('   o', x)
    for x in B[max(0, j1-2):j2+2][:14]: print('   u', x)
