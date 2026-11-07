import sys,subprocess,re
obj,start,end=sys.argv[1],int(sys.argv[2],16),int(sys.argv[3],16)
subprocess.run(['mips-linux-gnu-objcopy','-O','binary','--only-section=.rodata',obj,'build/ro.bin'],check=True)
rel=subprocess.run(['mips-linux-gnu-readelf','-r',obj],capture_output=True,text=True).stdout
relocs=set()
sec=None
for line in rel.split('\n'):
    if line.startswith('Relocation section'):
        sec=line.split("'")[1]
    elif sec=='.rel.rodata' and re.match(r'^[0-9a-f]{8}',line):
        relocs.add(int(line.split()[0],16))
d=open('iso/SLUS_217.99.rom','rb').read()
o=d[start-0x100000:end-0x100000]
m=open('build/ro.bin','rb').read()
print('orig',hex(len(o)),'ours',hex(len(m)))
i=0
while i<min(len(o),len(m)):
    if i not in relocs and o[i:i+4]!=m[i:i+4]:
        print('first diff at',hex(i),hex(start+i)); print(o[max(0,i-32):i+48]); print(m[max(0,i-32):i+48]); break
    i+=4
else: print('no diff in common part')
