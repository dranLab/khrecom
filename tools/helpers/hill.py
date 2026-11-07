import subprocess, sys, re, itertools
path, unit, func = sys.argv[1:4]
block=open('build/perm_block.txt').read()
orig=open(path).read()
assert block in orig, 'block not found'
lines=block.rstrip('\n').split('\n')
def score(ls):
    s=orig.replace(block,'\n'.join(ls)+'\n',1)
    open(path,'w').write(s)
    r=subprocess.run(f'ninja build/SLUS_217.99/src/{unit}.o >/dev/null 2>&1; .venv/bin/python tools/funcdiff.py {unit} {func} | head -2',shell=True,capture_output=True,text=True).stdout
    if r.startswith('MATCH'): return 0
    m=re.search(r'(\d+) of (\d+) lines differ',r)
    return int(m.group(1)) if m else 9999
try:
    cur=lines[:]; best=score(cur); print('start',best)
    improved=True
    while improved and best>0:
        improved=False
        for i,j in itertools.combinations(range(len(cur)),2):
            cand=cur[:]; cand[i],cand[j]=cand[j],cand[i]
            sc=score(cand)
            if sc<best:
                best=sc; cur=cand; improved=True; print('improve',best); break
            # also try moving i to j
            cand=cur[:]; x=cand.pop(i); cand.insert(j,x)
            sc=score(cand)
            if sc<best:
                best=sc; cur=cand; improved=True; print('improve(move)',best); break
    print('BEST',best); print('\n'.join(cur))
finally:
    open(path,'w').write(orig)
