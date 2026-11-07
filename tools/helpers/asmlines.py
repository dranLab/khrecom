import sys,re
sys.path.insert(0,'tools')
from pathlib import Path
from mdebug import read_mdebug
unit,func=sys.argv[1:3]
marks=[]
for f in read_mdebug(Path('iso/SLUS_217.99')):
    if unit.split('/')[-1] not in f.name: continue
    on=False; cur=f.name
    for s in f.symbols:
        if s.st in (6,14): on=(s.name==func)
        if on and s.st==5:
            if s.name.startswith('$LM'): marks.append((s.value,cur.split('/')[-1],s.index))
            elif s.index==586628: cur=s.name
import subprocess
asm=subprocess.run(['.venv/bin/python','tools/asm.py',unit,func],capture_output=True,text=True).stdout.split('\n')
start=marks[0][0]
addr=start; mi=0; cur=''
for l in asm[1:]:
    if not l.startswith('  ') or (l.strip().startswith(('.','jlabel')) and not l.strip().startswith('.word')) : print(l); continue
    while mi<len(marks) and marks[mi][0]<=addr: cur='%s:%d'%(marks[mi][1],marks[mi][2]); mi+=1
    print('%-22s %s'%(cur,l.replace('_61_GLOBAL_$N$c__KHCOM_MainApp_System_Graphic_WorldEnv.cppUPaaaa$','')))
    addr+=4
