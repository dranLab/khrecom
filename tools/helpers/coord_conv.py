import re,subprocess,sys
REG={'zero':0,'at':1,'v0':2,'v1':3,'a0':4,'a1':5,'a2':6,'a3':7,'t0':8,'t1':9,'t2':10,'t3':11,'t4':12,'t5':13,'t6':14,'t7':15,
 's0':16,'s1':17,'s2':18,'s3':19,'s4':20,'s5':21,'s6':22,'s7':23,'t8':24,'t9':25,'k0':26,'k1':27,'gp':28,'sp':29,'fp':30,'ra':31}
def asm_of(fn):
    out=subprocess.run(['.venv/bin/python','tools/asm.py','System/Coord',fn],capture_output=True,text=True).stdout
    return [l for l in out.split('\n')[1:] if l.strip()]
def straight(lines):
    ins=[l.strip() for l in lines]
    js=[i for i,l in enumerate(ins) if l.startswith('jr ')]
    if len(js)!=1: return None
    j=js[0]
    if j+1>=len(ins): return None
    if any(l!='nop' for l in ins[j+2:]): return None
    delay=ins[j+1]; body=ins[:j]
    for l in body:
        m=l.split()[0]
        if l.endswith(':') or m.startswith('b') or m in('j','jal','jr','jalr') or '$sp' in l or '%' in l or 'func_' in l: return None
    if delay!='nop': body=body+[delay]  # delay slot instr: original had it after jr; can't express -> reject
    if delay!='nop': return None
    return body
def fix(l):
    if l.startswith('.word'):
        return l.split('#')[0].strip()
    m=re.match(r'(\S+?)(\$\S.*)?$',l.replace('\t',' '))
    parts=l.split(None,1)
    if len(parts)==1: mn,ops=parts[0],''
    else: mn,ops=parts
    # rabbitizer glues operand to mnemonic when long: e.g. "vmulax.xyzw$ACC, ..."
    mm=re.match(r'([a-z0-9.]+?)(\$.*)$',mn)
    if mm: mn,ops=mm.group(1),mm.group(2)+(' '+ops if ops else '')
    ops=re.sub(r'\$([a-z][a-z0-9]?)\b',lambda x:'$%d'%REG[x.group(1)] if x.group(1) in REG else x.group(0),ops)
    if mn.startswith('vadda') or mn.startswith('vmsuba'):
        o=[x.strip() for x in ops.split(',')]
        if len(o)==3 and not re.search(r'[xyzw]$',o[2]):
            o=[o[0],o[2],o[1]]
        ops=', '.join(o)
    return (mn+' '+ops).strip()
if __name__=='__main__':
    for fn in sys.argv[1:]:
        b=straight(asm_of(fn))
        print(fn, 'OK' if b else 'NO')
        if b:
            for l in b: print('   ',fix(l))

def parse_params(s, method=False):
    out=[]; i=0; off=1 if method else 0
    def one(s,i):
        pre=''
        if s[i]=='R':
            t,i=one(s,i+1); return t+'&',i
        if s[i]=='P':
            t,i=one(s,i+1); return t+'*',i
        if s[i]=='C':
            t,i=one(s,i+1); return 'const '+t,i
        if s[i]=='U':
            t,i=one(s,i+1); return 'unsigned '+t,i
        if s[i].isdigit():
            j=i
            while s[j].isdigit(): j+=1
            n=int(s[i:j]); return s[j:j+n],j+n
        return {'f':'float','i':'int','v':'void','s':'short','c':'char'}[s[i]],i+1
    while i<len(s):
        if s[i]=='T':
            n=int(s[i+1]); out.append(out[n-off]); i+=2; continue
        if s[i]=='N':
            cnt=int(s[i+1]); n=int(s[i+2]); out+= [out[n-off]]*cnt; i+=3; continue
        t,i=one(s,i); out.append(t)
    return out
