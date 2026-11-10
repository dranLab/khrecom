"""Compile one unit with other flags and compare each function (masked: address fields cleared) with its pair in the
retail ELF (docs/retail-plan.md Phase 0/2; the pairs come from tools/version_map.py's functions.tsv, run it first).

Usage (repo root): .venv/bin/python tools/helpers/retail_try.py <unit> "<cflags>" [-D...] [--show=<function>...]
  e.g. tools/helpers/retail_try.py System/Math3D "-O2 -G0 -fno-exceptions -fno-rtti"     (the beta flags minus -g)
Objects go to build/retail_try/obj/. Functions are read from their own section: without -g gcc 2.95 puts weak
functions into .gnu.linkonce.t.<name>. Prints OK/DIFF per function with the version_map class, and a total.
"""
import csv, subprocess, sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
from version_map import Image, decode, ROOT
args = [a for a in sys.argv[1:] if not a.startswith("--show=")]
show = {a[len("--show="):] for a in sys.argv[1:] if a.startswith("--show=")}  # functions to print side by side
unit, cflags = args[0], args[1]
extra = args[2:]  # e.g. -DKH_RETAIL
src = next(p for p in (ROOT / f"src/{unit}.cpp", ROOT / f"src/{unit}.c") if p.exists())
out = ROOT / "build/retail_try/obj" / (unit.replace("/", "_") + ".o")
out.parent.mkdir(parents=True, exist_ok=True)
cc = "tools/wibo/wibo tools/compilers/ee-gcc2.95.3-136/bin/ee-gcc.exe"
asm = "tools/wibo/wibo tools/compilers/ee-gcc2.95.3-136/lib/gcc-lib/ee/2.95.3/Ps2EeAs.exe --stdout -G0"
cmd = (f"{cc} -S {cflags} {' '.join(extra)} -I include -I src -o {out}.gcc.s {src} && "
       f".venv/bin/python tools/ee_asm_compat.py {out}.gcc.s {out}.s --source {src} "
       f"--symbols config/SLUS_217.99/elf_symbol_addrs.txt --own-data none --unit-data none "
       f"--source-root c:/KHCOM/MainApp/ --assembler snas && {asm} -o {out} {out}.s")
r = subprocess.run(cmd, shell=True, capture_output=True, text=True)
if r.returncode:
    print(r.stdout[-2000:], r.stderr[-2000:]); sys.exit(1)
# functions of the object, each from its own section (.text or .gnu.linkonce.t.<name>)
from elftools.elf.elffile import ELFFile
syms, secdata = {}, {}
with open(out, "rb") as fh:
    elf = ELFFile(fh)
    for sym in elf.get_section_by_name(".symtab").iter_symbols():
        if sym["st_info"]["type"] == "STT_FUNC" and isinstance(sym["st_shndx"], int):
            sec = elf.get_section(sym["st_shndx"])
            secdata.setdefault(sec.name, sec.data())
            syms[sym.name] = (sec.name, sym["st_value"], sym["st_size"])
    relocated = {}  # section -> offsets of relocated words (their address fields are masked on both sides)
    from elftools.elf.relocation import RelocationSection
    for rs in elf.iter_sections():
        if isinstance(rs, RelocationSection):
            target = elf.get_section(rs["sh_info"]).name
            relocated.setdefault(target, set()).update(r["r_offset"] for r in rs.iter_relocations())
import struct
retail = Image(ROOT / "iso/SLUS_217.99_retail")
rows = {x["name"]: x for x in csv.DictReader(open(ROOT / "build/SLUS_217.99_retail/version_map/functions.tsv"), delimiter="\t")}
ok = bad = 0
for name, (sec, off, size) in sorted(syms.items(), key=lambda kv: (kv[1][0] != ".text", kv[1][1])):
    text = secdata[sec]
    row = rows.get(name)
    if not row or not row["other"]:
        print(f"  ----  {name}: no retail pair ({row['status'] if row else 'not in beta'})"); continue
    ow = list(struct.unpack_from(f"<{size // 4}I", text, off))
    ra, rs = int(row["other"], 16), int(row["other_size"], 16)
    rw = retail.words(ra, rs)
    a = [x[0] for x in decode(ow, 0)]; b = [x[0] for x in decode(rw, ra)]
    for k in range(min(len(a), len(b))):
        if off + 4 * k in relocated.get(sec, ()):  # an address field: compare opcode and registers only
            field = 0x3FFFFFF if a[k] >> 26 in (2, 3) else 0xFFFF
            a[k] &= ~field & 0xFFFFFFFF
            b[k] &= ~field & 0xFFFFFFFF
    same = a == b
    ok += same; bad += not same
    diff = sum(1 for x, y in zip(a, b) if x != y) + abs(len(a) - len(b))
    if name in show:
        from rabbitizer import Instruction
        for k in range(max(len(ow), len(rw))):
            x = Instruction(ow[k], 4 * k).disassemble() if k < len(ow) else ""
            y = Instruction(rw[k], ra + 4 * k).disassemble() if k < len(rw) else ""
            mark = " " if k < len(a) and k < len(b) and a[k] == b[k] else "*"
            print(f"    {mark} {x[:44]:44s} | {y[:44]}")
    print(f"  {'OK  ' if same else 'DIFF'}  {name} ({row['class'] or row['status']}) ours {size:#x} retail {rs:#x}"
          + ("" if same else f" ~{diff} words"))
print(f"{unit}: {ok} match retail, {bad} differ")
