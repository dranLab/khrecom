import subprocess,sys
# usage: tryv.py unit func file  (variants read from build/variants.py: list of (old,new))
unit,func,path=sys.argv[1:4]
V=eval(open('build/variants.py').read())
orig=open(path).read()
try:
    for k,(a,b) in V.items():
        assert a in orig,k
        open(path,'w').write(orig.replace(a,b))
        r=subprocess.run(f'ninja build/SLUS_217.99/src/{unit}.o 2>&1 | grep -iE " error|cpp:[0-9]+:" | head -3; .venv/bin/python tools/funcdiff.py {unit} \'{func}\' | head -2',shell=True,capture_output=True,text=True)
        print(k,r.stdout.strip())
finally:
    open(path,'w').write(orig)
