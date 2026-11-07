import sys,re
p,cls,inc=sys.argv[1:4]
s=open(p,encoding='latin-1').read()
i=re.search(r"class %s( : [^{]*)? \{[^\n]*\n"%cls, s).start()
j=s.index("\n};\n",i)+4
n=s[i:j].count("\n")
s=s[:i]+'#include "%s"\n'%inc+"\n"*(n-1)+s[j:]
open(p,"w",encoding='latin-1').write(s)
