"""tools/helpers/lineorder.py <lines-file> <first> <last>: compress asmlines output: per marker run, marker + instructions joined."""
import sys, re
L = open(sys.argv[1], encoding='latin-1').read().split('\n')[int(sys.argv[2]):int(sys.argv[3])]
cur = None; buf = []
def flush():
    if cur is not None:
        print(cur, '; '.join(buf)[:150])
for l in L:
    if l.startswith('.L') or 'jlabel' in l:
        flush(); cur = None; buf = []; print(l.strip()); continue
    m = re.match(r'(\S+:\d+)\s+(.*)', l)
    if not m:
        continue
    k, ins = m.groups()
    ins = re.sub(r'\s+', ' ', ins.strip())
    if k != cur:
        flush(); cur = k; buf = []
    buf.append(ins)
flush()
