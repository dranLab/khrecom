import re,subprocess
F={0x1B0:'next_address',0x1B2:'wait_timer',0x1B4:'wait_key',0x1BC:'status'}
sigs={
'acWaitLoadRsrc':['nAddr'],'acWaitCardWind':['nAddr'],'acWaitTechWind':['nAddr'],'acWaitWorldCardWind':['nAddr'],
'acWaitCrossFade':['nAddr'],'acWaitLoadEnemy':['nAddr'],
'acWaitPad':['nKey','nAddr'],'acWaitMes':['nKey','nAddr'],'acWaitCam':['nKey','nAddr'],'acWaitBusy':['nKey','nAddr'],
'acWaitRsrc':['nKey','nAddr'],'acWaitSnatch':['nKey','nAddr'],'acWaitEquip':['nKey','nAddr'],'acWaitDelLeftSword':['nKey','nAddr'],
'acWaitBusyActor':['nKey','nWin','nAddr'],'acWaitActor':['nKey','nWin','nAddr'],'acWaitSelectPad':['nKey','nWin','nAddr'],
'acWaitSelectPadEx':['nKey','nWin','nAddr'],'acWaitChangeWork':['nKey','nVal','nAddr'],
}
asm=open('build/SLUS_217.99/expected/asm/Game/Atel/VSME.s').read()
out=[]
for name,params in sigs.items():
    mang=name+'__Q24ATEL4CVSMRQ24ATEL5Actor'+'i'*len(params)
    i=asm.index('glabel '+mang); j=asm.find('glabel ',i+10)
    body=asm[i:j]
    lines=[l.split('*/')[-1].strip() for l in body.split('\n')]
    reg={'$a2':params[0],'$a3':params[1] if len(params)>1 else None,'$t0':params[2] if len(params)>2 else None,'$zero':'0'}
    stores=[];extra=[]
    D=re.search(r'%hi\((D_\w+)\)',body).group(1)
    for l in lines:
        m=re.match(r'addiu\s+(\$\w+), \$zero, (-?0x[0-9A-Fa-f]+)',l)
        if m: reg[m.group(1)]=str(int(m.group(2),16)); continue
        m=re.match(r'daddu\s+(\$\w+), (\$\w+), \$zero',l)
        if m: reg[m.group(1)]=reg.get(m.group(2)); continue
        m=re.match(r'sh\s+(\$\w+), (0x[0-9A-F]+)\((\$\w+)\)',l)
        if m and int(m.group(2),16) in F:
            stores.append('    atp.%s = %s;'%(F[int(m.group(2),16)],reg[m.group(1)])); continue
        m=re.match(r'sw\s+(\$\w+), 0x0\(\$a0\)',l)
        if m: stores.append('    m_PadWaitTime = %s;'%reg[m.group(1)])
        if l.startswith('jal'): break
    targs=', '.join(['atp.id']+params)
    sig=', '.join(['Actor& act']+['INT '+p for p in params])
    out.append('//@ %s\nvoid CVSM::%s(%s) {\n    ATP& atp = act._ATP;\n%s\n    _TRACE(m_bTrace, %s, %s);\n}\n'%(mang,name,sig,'\n'.join(stores),D,targs))
    out.append('')
open('build/vs1.txt','w').write('\n'.join(out))
print(open('build/vs1.txt').read()[:1500])
