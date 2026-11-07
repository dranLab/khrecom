import sys
# usage: applyv.py file name  (applies a variant from build/variants.py)
p,k=sys.argv[1:3]
V=eval(open('build/variants.py').read()); s=open(p).read(); a,b=V[k]; assert a in s,k; open(p,'w').write(s.replace(a,b,1))
