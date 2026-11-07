import re,subprocess,sys
sys.path.insert(0,'tools/helpers')
from coord_conv import fix
def body(fn):
    out=subprocess.run(['.venv/bin/python','tools/asm.py','System/Graphic/Gprim',fn],capture_output=True,text=True).stdout
    ls=[l.strip() for l in out.split('\n')[1:] if l.strip()]
    # strip trailing 'jr $ra' + 'nop' (+ padding nops)
    while ls and ls[-1]=='nop': ls.pop()
    assert ls[-1].startswith('jr'), ls[-1]
    ls.pop()
    res=[];labels={}
    for i,l in enumerate(ls):
        if l.endswith(':'):
            labels[l[:-1]]=str(len(labels)); continue
    out=[]
    i=0
    while i<len(ls):
        l=ls[i]
        if l.endswith(':'):
            out.append(labels[l[:-1]]+':'); i+=1; continue
        mn=l.split()[0]
        if mn.startswith('b'):
            # branch with explicit delay slot
            lab=l.split(',')[-1].strip()
            l2=re.sub(re.escape(lab),labels[lab]+'f',l)
            out.append('.set noreorder'); out.append(fix(l2)); out.append(fix(ls[i+1])); out.append('.set reorder'); i+=2; continue
        out.append(fix(l)); i+=1
    return out
for fn in sys.argv[1:]:
    print('//@',fn)
    print('\n'.join('        "%s\\n"'%x for x in body(fn)))
