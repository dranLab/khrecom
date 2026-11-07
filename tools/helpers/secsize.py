"""tools/helpers/secsize.py <unit>...: section sizes of the unit's object vs the split in the yaml."""
import re, subprocess, sys
Y = open('config/SLUS_217.99/SLUS_217.99.yaml').read().split('\n')
for u in sys.argv[1:]:
    exp = {}
    for i, l in enumerate(Y):
        m = re.match(r'\s*- \[(0x[0-9A-Fa-f]+), (\w+), (.+?)\]', l)
        if m and m.group(3) == u:
            n = re.match(r'\s*- \[(0x[0-9A-Fa-f]+)', Y[i+1])
            if n:
                exp[{'asm': '.text', 'c': '.text', 'cpp': '.text', 'data': '.data', 'rodata': '.rodata'}.get(m.group(2), m.group(2))] = int(n.group(1), 16) - int(m.group(1), 16)
    o = subprocess.run(['mips-linux-gnu-readelf', '-SW', f'build/SLUS_217.99/src/{u}.o'], capture_output=True, text=True).stdout
    got = {m.group(1): int(m.group(2), 16) for m in re.finditer(r'\] (\.\w+)\s+\w+\s+\w+\s+\w+\s+(\w+)', o)}
    print(u, {k: (hex(v), hex(got.get(k, -1))) for k, v in exp.items() if got.get(k) != v} or 'sizes ok')
