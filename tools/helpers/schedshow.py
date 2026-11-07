import sys,re
f,fn,key=sys.argv[1:4]
t=open(f,errors='replace').read()
a=t.index(';; Function '+fn); b=t.find('\n;; Function ',a+10); s=t[a:b]
# rtl
rtl={}
for m in re.finditer(r'\n\((insn|call_insn|jump_insn) (\d+) \d+ \d+ (.*?)\n\n', s, re.S):
    rtl[m.group(2)]=re.sub(r'\s+',' ',m.group(3))[:120]
keyn=[k for k,v in rtl.items() if key in v]
print('key insns',keyn)
bbs=[m.start() for m in re.finditer(r'-- basic block', s)]
for i,st in enumerate(bbs):
    e=bbs[i+1] if i+1<len(bbs) else len(s)
    blk=s[st:e]
    if any(re.search(r'^;;\s+%s\s+\d+\s+0 '%k, blk, re.M) for k in keyn) and 'forward dep' in blk:
        print(blk.split('\n')[0])
        for l in blk.split('\n'):
            g=re.match(r';;\s+(\d+)\s+(-?\d+)\s+0\s+(\d+)\s+(\d+)\s+(\d+)\s+.*?:(.*)',l)
            if g: print(g[1],'p'+g[4],'c'+g[5],'->',g[6].strip(),'|',rtl.get(g[1],'')[:90])
            elif 'Ready list (t' in l or '--> scheduling' in l: print(re.sub(r'^;;\s+','  ',l)[:70])
        break
