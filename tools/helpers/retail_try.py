"""Compile one unit with other flags and compare each function (masked: address fields cleared) with its pair in the
retail ELF (docs/retail-plan.md Phase 0/2; the pairs come from tools/version_map.py's functions.tsv, run it first).

--data: also the unit's data sections: each one's original start, voted by the relocations of the functions that
match (retail address - our addend at the same instruction), and whether our bytes equal the retail image's there
(relocated words compared without their value).

Usage (repo root): .venv/bin/python tools/helpers/retail_try.py <unit> "<cflags>" [-D...] [--show=<function>...] [--data] [--record]
  e.g. tools/helpers/retail_try.py System/Math3D "-O2 -G0 -fno-exceptions -fno-rtti"     (the beta flags minus -g)
Objects go to build/retail_try/obj/. Functions are read from their own section: without -g gcc 2.95 puts weak
functions into .gnu.linkonce.t.<name>. Prints OK/DIFF per function with the version_map class, and a total.
"""
import csv, subprocess, sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
from version_map import Image, decode, ROOT
want_data = "--data" in sys.argv or "--record" in sys.argv
record = "--record" in sys.argv  # write unanimous .data/.rodata/.bss starts to config/SLUS_217.99_retail/unit_starts.txt
args = [a for a in sys.argv[1:] if not a.startswith("--show=") and a not in ("--data", "--record")]
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
       f"--symbols config/SLUS_217.99_retail/elf_symbol_addrs.txt --own-data 0x0-0xFFFFFFFF --unit-data none "
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
    reloc_target = {}  # (section, offset) -> (data section, symbol value) of relocations against our own data
    from elftools.elf.relocation import RelocationSection
    symtab = elf.get_section_by_name(".symtab")
    data_secs = {}
    for rs in elf.iter_sections():
        if isinstance(rs, RelocationSection):
            target = elf.get_section(rs["sh_info"]).name
            relocated.setdefault(target, set()).update(r["r_offset"] for r in rs.iter_relocations())
            for r in rs.iter_relocations():
                sym = symtab.get_symbol(r["r_info_sym"])
                if isinstance(sym["st_shndx"], int) and sym["st_shndx"]:
                    dsec = elf.get_section(sym["st_shndx"])
                    if not dsec["sh_flags"] & 4:  # not code
                        reloc_target[(target, r["r_offset"])] = (dsec.name, sym["st_value"])
    for sec in elf.iter_sections():
        if sec["sh_addr"] == 0 and sec["sh_flags"] & 2 and not sec["sh_flags"] & 4 and sec["sh_size"] and \
                not sec.name.startswith(".rel"):
            data_secs[sec.name] = (sec["sh_size"], sec.data() if sec["sh_type"] != "SHT_NOBITS" else None,
                                   relocated.get(sec.name, set()))
import struct
retail = Image(ROOT / "iso/SLUS_217.99_retail")
rows = {x["name"]: x for x in csv.DictReader(open(ROOT / "build/SLUS_217.99_retail/version_map/functions.tsv"), delimiter="\t")}
ok = bad = 0
from collections import Counter
votes = {}
for name, (sec, off, size) in sorted(syms.items(), key=lambda kv: (kv[1][0] != ".text", kv[1][1])):
    text = secdata[sec]
    row = rows.get(name)
    if not row or not row["other"]:
        print(f"  ----  {name}: no retail pair ({row['status'] if row else 'not in beta'})"); continue
    ow = list(struct.unpack_from(f"<{size // 4}I", text, off))
    ra, rs = int(row["other"], 16), int(row["other_size"], 16)
    rw = retail.words(ra, rs)
    da, db = decode(ow, 0), decode(rw, ra)
    a = [x[0] for x in da]; b = [x[0] for x in db]
    for k in range(min(len(a), len(b))):
        if off + 4 * k in relocated.get(sec, ()):  # an address field: compare opcode and registers only
            field = 0x3FFFFFF if a[k] >> 26 in (2, 3) else 0xFFFF
            a[k] &= ~field & 0xFFFFFFFF
            b[k] &= ~field & 0xFFFFFFFF
    same = a == b
    ok += same; bad += not same
    if same and want_data:
        for k in range(len(da)):
            t = reloc_target.get((sec, off + 4 * k))
            if t and da[k][3] is not None and db[k][3] is not None:
                votes.setdefault(t[0], Counter())[db[k][3] - da[k][3] - t[1]] += 1
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
if want_data:
    for name, (size, data, rel) in sorted(data_secs.items()):
        v = votes.get(name)
        if not v:
            print(f"  DATA  {name}: size {size:#x}, no vote (no matching function addresses it)")
            continue
        start, n = v.most_common(1)[0]
        line = f"  DATA  {name}: start 0x{start:08X} ({n}/{sum(v.values())} votes), size {size:#x}"
        if data is not None:
            try:
                theirs = retail.data[retail.off(start):retail.off(start) + size]
            except KeyError:
                theirs = b""
            diff = [i for i in range(0, size, 4) if i not in rel and data[i:i + 4] != theirs[i:i + 4]]
            line += ": bytes " + ("OK" if not diff and len(theirs) == size else f"DIFF at {', '.join(hex(i) for i in diff[:6])}")
        print(line)
        if record and name in (".data", ".rodata", ".bss") and n == sum(v.values()):
            path = ROOT / "config/SLUS_217.99_retail/unit_starts.txt"
            lines = path.read_text(encoding="utf-8").splitlines() if path.exists() else [
                "# Exact data starts of retail units, measured from their compiled objects (tools/helpers/retail_try.py",
                "# --record); tools/split_version.py applies them. <unit> <section> <address>"]
            lines = [l for l in lines if l.split()[:2] != [unit, name]] + [f"{unit} {name} 0x{start:08X} 0x{size:X}"]
            path.write_text("\n".join(lines[:2] + sorted(lines[2:])) + "\n", encoding="utf-8")
