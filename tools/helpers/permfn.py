import itertools,sys,subprocess
# usage: permfn.py unit func file  <<< block (statements one per line, exact text in file, contiguous)
unit,func,path=sys.argv[1:4]
blk=sys.stdin.read()
lines=blk.splitlines(keepends=True)
V={}
for i,p in enumerate(itertools.permutations(lines)):
    if i==0: continue
    V['q%d'%i]=(blk,''.join(p))
open('build/variants.py','w').write(repr(V))
r=subprocess.run(['.venv/bin/python','tools/helpers/tryv.py',unit,func,path],capture_output=True,text=True).stdout
m=[l for l in r.splitlines() if ' MATCH ' in l]
print('\n'.join(m) if m else 'no match')
if m:
    k=m[0].split()[0]
    subprocess.run(['.venv/bin/python','tools/helpers/applyv.py',path,k])
