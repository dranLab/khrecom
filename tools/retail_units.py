#!/usr/bin/env python3
"""Bring units over to the retail build (docs/retail-plan.md, Phase 3): which units' sources reproduce the retail ELF,
switch them to source there, and verify.

    try <unit> [--show F ...] [--data]   compile the unit as retail does (-DKH_RETAIL, no -g) and compare each function
                                         (masked: relocated fields, lui-based addresses) with its retail pair
                                         (tools/version_map.py's functions.tsv); --data: each data section's original
                                         start, voted by the relocations of the matching functions, and its bytes
    scan [-j N] [units...]               `try --data` for every complete unit (no INCLUDE_ASM; default: all of them) and
                                         whether it can switch: every .text function matches, its own linkonce
                                         functions match, its code and linkonce code have retail's sizes, every data
                                         section the code references is voted unanimously and (except .bss) has
                                         retail's bytes. Writes build/<VERSION>/retail_units/scan.tsv
    switch [--eligible | units...]       switch units (from the last scan): record their data starts
                                         (config/<VERSION>/unit_starts.txt), give the source its retail data marker
                                         (// SOURCE DATA <VERSION>: ...), list it in config/<VERSION>/sources.txt,
                                         regenerate the split (tools/split_version.py)
    verify [--max N]                     build the retail image; while it differs, switch back the units whose objects
                                         tools/imagediff.py names (at most N rounds), then report

The version is SLUS_217.99_retail (--version for another non-reference version).
"""

import argparse
import csv
import re
import struct
import subprocess
import sys
from collections import Counter, defaultdict
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

import yaml
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

sys.path.insert(0, str(Path(__file__).resolve().parent))
from version_map import Image, ROOT, decode  # noqa: E402

CC = "tools/wibo/wibo tools/compilers/ee-gcc2.95.3-136/bin/ee-gcc.exe"
AS = "tools/wibo/wibo tools/compilers/ee-gcc2.95.3-136/lib/gcc-lib/ee/2.95.3/Ps2EeAs.exe --stdout -G0"
DATA = (".data", ".rodata", ".bss", ".sdata", ".sbss")


class Version:
    def __init__(self, name: str):
        self.name = name
        self.cfg = ROOT / "config" / name
        self.build_cfg = yaml.safe_load((self.cfg / "build.yaml").read_text(encoding="utf-8"))
        self.cflags = self.build_cfg["cflags"]
        self.image = Image(ROOT / self.build_cfg["elf"])
        self.out = ROOT / "build" / name / "retail_units"
        vmap = ROOT / "build" / Path(self.build_cfg["elf"]).name / "version_map" / "functions.tsv"
        self.pairs = {r["name"]: r for r in csv.DictReader(vmap.open(encoding="utf-8"), delimiter="\t")}
        # file-local names the beta's symbol file de-duplicated (randf_0016A2C0): per unit, by the bare name
        self.local_pairs = defaultdict(list)
        for fname, r in self.pairs.items():
            if m := re.fullmatch(r"(.+)_([0-9A-F]{8})", fname):
                self.local_pairs[(r["unit"], m.group(1))].append(r)
        self.symbols = {}
        for line in (self.cfg / "elf_symbol_addrs.txt").read_text(encoding="utf-8").splitlines():
            if m := re.match(r"^(\S+) = 0x([0-9A-F]+);", line):
                self.symbols[m.group(1)] = int(m.group(2), 16)
        # each unit's subsegment ranges in this version's split: (unit, kind) -> (start, end)
        self.ranges = {}
        rows = []
        text = (self.cfg / f"{name}.yaml").read_text(encoding="utf-8")
        for m in re.finditer(r"^      - (?:\[0x([0-9A-F]+), ([.\w]+), ([^\]]+)\]|\{ type: ([.\w]+), vram: 0x([0-9A-F]+), "
                             r"name: ([^ }]+) \})", text, re.M):
            if m.group(1):
                rows.append((0x100000 + int(m.group(1), 16), m.group(2).lstrip("."), m.group(3).strip()))
            else:
                rows.append((int(m.group(5), 16), m.group(4).lstrip("."), m.group(6).strip()))
        ends = [a for a, _, _ in rows[1:]] + [max(a + n for a, n in self.image.sections.values())]
        section_end = {}
        for sec, (a, n) in self.image.sections.items():
            section_end[sec] = a + n
        for (a, t, u), e in zip(rows, ends):
            kind = {"asm": ".text", "c": ".text", "cpp": ".text", "data": ".data", "rodata": ".rodata",
                    "bss": ".bss", "sdata": ".sdata", "sbss": ".sbss"}.get(t, t)
            sec_end = next((section_end[s] for s, (sa, sn) in self.image.sections.items() if sa <= a < sa + sn), e)
            self.ranges[(u, kind)] = (a, min(e, sec_end))


def source_of(unit: str) -> Path | None:
    return next((p for p in (ROOT / f"src/{unit}.cpp", ROOT / f"src/{unit}.c") if p.exists()), None)


def masked(words, base):
    return [x[0] for x in decode(words, base)], decode(words, base)


def try_unit(v: Version, unit: str, show=(), data=True) -> dict:
    """Compile and compare; returns a result dict (see scan)."""
    src = source_of(unit)
    res = {"unit": unit, "functions": [], "data": {}, "problems": []}
    if src is None:
        res["problems"].append("no source")
        return res
    out = v.out / "obj" / (unit.replace("/", "_") + ".o")
    out.parent.mkdir(parents=True, exist_ok=True)
    src = src.relative_to(ROOT)  # as the build passes it: gcc's _GLOBAL_$I$<path> names come from it
    cmd = (f"{CC} -S {v.cflags} -I include -I src -o {out}.gcc.s {src} && "
           f"{sys.executable} tools/ee_asm_compat.py {out}.gcc.s {out}.s --source {src} "
           f"--symbols {v.cfg}/elf_symbol_addrs.txt --own-data 0x0-0xFFFFFFFF --unit-data none "
           f"--source-root c:/KHCOM/MainApp/ --assembler snas --asm-path asm/{v.name} && {AS} -o {out} {out}.s")
    r = subprocess.run(cmd, shell=True, capture_output=True, text=True, cwd=ROOT)
    if r.returncode:
        err = [l for l in (r.stdout + r.stderr).splitlines() if "rror" in l or ": parse" in l]
        res["problems"].append("compile: " + (err[0] if err else "failed")[:160])
        return res
    lo_t = v.ranges.get((unit + ".linkonce", ".text"), (0, 0))
    lo_d = v.ranges.get((unit + ".linkonce", ".data"), (0, 0))
    with open(out, "rb") as fh:
        elf = ELFFile(fh)
        symtab = elf.get_section_by_name(".symtab")
        relocated, target_of = defaultdict(set), {}
        for rs in elf.iter_sections():
            if isinstance(rs, RelocationSection):
                tsec = elf.get_section(rs["sh_info"]).name
                for rel in rs.iter_relocations():
                    relocated[tsec].add(rel["r_offset"])
                    sym = symtab.get_symbol(rel["r_info_sym"])
                    if isinstance(sym["st_shndx"], int) and sym["st_shndx"]:
                        dsec = elf.get_section(sym["st_shndx"])
                        if not dsec["sh_flags"] & 4:
                            target_of[(tsec, rel["r_offset"])] = (dsec.name, sym["st_value"])
        secs = {s.name: s for s in elf.iter_sections()}
        funcs = []
        for sym in symtab.iter_symbols():
            if sym["st_info"]["type"] == "STT_FUNC" and isinstance(sym["st_shndx"], int) and sym["st_shndx"]:
                funcs.append((elf.get_section(sym["st_shndx"]).name, sym["st_value"], sym["st_size"], sym.name))
        secdata = {n: (s.data() if s["sh_type"] != "SHT_NOBITS" else None, s["sh_size"], s["sh_addralign"])
                   for n, s in secs.items()}
    votes = defaultdict(Counter)
    text_size = secdata.get(".text", (None, 0, 1))[1]
    own_linkonce = 0
    for sec, off, size, name in sorted(funcs):
        linkonce = sec.startswith(".gnu.linkonce.t.")
        if linkonce:
            addr = v.symbols.get(name)
            if addr is None:
                res["functions"].append((name, "extra", "linkonce copy unknown to this version"))
                continue  # stays in the build (ee_asm_compat --linkonce keeps unknown copies): extra code
            if not lo_t[0] <= addr < lo_t[1]:
                continue  # kept from another object in the original: removed in the build
            own_linkonce += secdata[sec][1]
        pair = v.pairs.get(name)
        if not pair:
            local = v.local_pairs.get((unit, name), [])
            if len(local) == 1:
                pair = local[0]
            elif len(local) > 1:  # several statics of that name in the unit: in address order, like ours
                same = sorted((r for r in local), key=lambda r: int(r["beta"], 16))
                k = sum(1 for f in res["functions"] if re.sub(r"_[0-9A-F]{8}$", "", f[0]) == name)
                pair = same[k] if k < len(same) else None
        if not pair or not pair["other"]:
            res["functions"].append((name, "extra", ""))
            continue
        ow = list(struct.unpack_from(f"<{size // 4}I", secdata[sec][0], off))
        ra, rsz = int(pair["other"], 16), int(pair["other_size"], 16)
        rw = v.image.words(ra, rsz)
        da, db = decode(ow, 0), decode(rw, ra)
        a, b = [x[0] for x in da], [x[0] for x in db]
        for k in range(min(len(a), len(b))):
            if off + 4 * k in relocated[sec]:
                field = 0x3FFFFFF if a[k] >> 26 in (2, 3) else 0xFFFF
                a[k] &= ~field & 0xFFFFFFFF
                b[k] &= ~field & 0xFFFFFFFF
        same = a == b
        res["functions"].append((name, "OK" if same else "DIFF", pair["class"] or pair["status"]))
        if same:
            for k in range(len(da)):
                t = target_of.get((sec, off + 4 * k))
                if t and da[k][3] is not None and db[k][3] is not None:
                    votes[t[0]][db[k][3] - da[k][3] - t[1]] += 1
        if name in show:
            from rabbitizer import Instruction
            for k in range(max(len(ow), len(rw))):
                x = Instruction(ow[k], 4 * k).disassemble() if k < len(ow) else ""
                y = Instruction(rw[k], ra + 4 * k).disassemble() if k < len(rw) else ""
                mark = " " if k < len(a) and k < len(b) and a[k] == b[k] else "*"
                print(f"    {mark} {x[:44]:44s} | {y[:44]}")
    # code sizes: ours vs the unit's ranges in this version's split (alignment padding at the end ignored)
    def code_size(start, end):
        n = end - start
        while n > 0 and v.image.word(start + n - 4) == 0:
            n -= 4
        return n
    t_range = v.ranges.get((unit, ".text"))
    if t_range and text_size:
        theirs = code_size(*t_range)
        if not (theirs <= text_size <= t_range[1] - t_range[0]):
            res["problems"].append(f".text size {text_size:#x}, retail range {theirs:#x}")
    elif text_size:
        res["problems"].append("no .text range in the split")
    if lo_t[1] > lo_t[0]:
        theirs = code_size(*lo_t)
        if not (theirs <= own_linkonce <= lo_t[1] - lo_t[0]):
            res["problems"].append(f"linkonce code {own_linkonce:#x}, retail range {theirs:#x}")
    own_ld = sum(sz for n, (_, sz, al) in secdata.items() if n.startswith(".gnu.linkonce.d.")
                 and lo_d[0] <= v.symbols.get(n[len(".gnu.linkonce.d."):], -1) < lo_d[1])
    if lo_d[1] > lo_d[0] and not (lo_d[1] - lo_d[0] - 8 < own_ld <= lo_d[1] - lo_d[0]):
        res["problems"].append(f"linkonce data {own_ld:#x}, retail range {lo_d[1] - lo_d[0]:#x}")
    if data:
        referenced = {t[0] for t in target_of.values()}
        for name, (blob, size, align) in sorted(secdata.items()):
            if not size or not (name in DATA or name.startswith(".gnu.linkonce.d.")):
                continue
            entry = {"size": size, "start": None, "votes": "", "bytes": None, "referenced": name in referenced}
            if name.startswith(".gnu.linkonce.d."):
                addr = v.symbols.get(name[len(".gnu.linkonce.d."):])
                if addr is None:
                    res["problems"].append(f"{name}: unknown to this version (extra data)")
                    continue
                if not lo_d[0] <= addr < lo_d[1]:
                    continue  # another object's copy: removed in the build
                entry["start"] = addr
            elif votes.get(name):
                start, n = votes[name].most_common(1)[0]
                entry["votes"] = f"{n}/{sum(votes[name].values())}"
                if n == sum(votes[name].values()):
                    entry["start"] = start
            if entry["start"] is not None and blob is not None:
                try:
                    theirs = v.image.data[v.image.off(entry["start"]):v.image.off(entry["start"]) + size]
                except KeyError:
                    theirs = b""
                diff = [i for i in range(0, size, 4) if i not in relocated[name] and blob[i:i + 4] != theirs[i:i + 4]]
                entry["bytes"] = "OK" if not diff and len(theirs) == size else f"DIFF@{diff[0]:#x}" if diff else "DIFF"
            res["data"][name] = entry
    return res


def eligible(res: dict) -> tuple[bool, list[str], str]:
    """(can switch, data sections from source, reason)."""
    if res["problems"]:
        return False, [], "; ".join(res["problems"])
    bad = [f"{n} {s}" for n, s, _ in res["functions"] if s != "OK"]
    if bad:
        return False, [], f"{len(bad)} functions: " + ", ".join(bad[:3])
    sections = []
    for name, e in res["data"].items():
        if name.startswith(".gnu.linkonce.d."):
            if e["bytes"] != "OK":
                return False, [], f"{name} bytes {e['bytes']}"
            continue
        if not e["referenced"]:
            continue  # stays assembly; nothing in the code needs gcc's copy
        if e["start"] is None:
            return False, [], f"{name}: votes {e['votes'] or 'none'}"
        if e["bytes"] not in (None, "OK"):
            return False, [], f"{name}: bytes {e['bytes']}"
        sections.append(name)
    return True, sections, ""


def image_culprits(v: Version) -> set[str]:
    """The compiled units to blame for the differences of the last link: per output section, the compiled object
    placed last at or before the section's first differing address (it is wrong itself, or its size moved what
    follows). Differences in data come first: moved data also change the code that addresses it."""
    from imagediff import read_map
    opts = yaml.safe_load((v.cfg / f"{v.name}.yaml").read_text(encoding="utf-8"))["options"]
    original = (ROOT / opts["target_path"]).read_bytes()
    ours = (ROOT / Path(opts["elf_path"]).with_suffix(".rom")).read_bytes()
    sections, _ = read_map(ROOT / Path(opts["elf_path"]).with_suffix(".map"))
    base = 0x100000
    found = {}
    for name, (lo, size) in v.image.sections.items():
        if name not in (".text", ".data", ".rodata", ".sdata") or not size:
            continue
        first = next((a for a in range(lo, lo + size) if a - base < len(ours) and a - base < len(original)
                      and ours[a - base] != original[a - base]), None)
        if first is None:
            continue
        before = [s for s in sections if lo <= s[0] <= first and "/src/" in s[3]]
        if before:
            obj = max(before)[3]
            found[name] = re.sub(r"^build/[^/]+/src/(.*)\.o$", r"\1", obj)
    data = {k: u for k, u in found.items() if k != ".text"}
    return set((data or found).values())


def layout_culprits(v: Version) -> set[str]:
    """From the linker map (written even when the link's checks fail): the first input section, by address, that
    doesn't start where this version's split puts its unit; blamed: the compiled object placed last at or before it
    (wrong itself, or its size moved what follows)."""
    from imagediff import read_map
    opts = yaml.safe_load((v.cfg / f"{v.name}.yaml").read_text(encoding="utf-8"))["options"]
    sections, _ = read_map(ROOT / Path(opts["elf_path"]).with_suffix(".map"))
    out_section = {}
    for name, (lo, size) in v.image.sections.items():
        out_section[name] = (lo, lo + size)
    compiled_before = None
    for start, end, sec, obj in sorted(sections):
        kind = next((k for k, (lo, hi) in out_section.items() if lo <= start < hi), None)
        if kind is None:
            continue
        if m := re.match(r"^build/[^/]+/src/(.*)\.o$", obj):
            unit = m.group(1)
            if sec.startswith(".gnu.linkonce."):
                unit += ".linkonce"
                first = v.ranges.get((unit, kind))
                if first and not first[0] <= start < first[1]:
                    return {m.group(1)}
                compiled_before = m.group(1)
                continue
        elif m := re.match(r"^build/[^/]+/asm/[^/]+/data/(.*)\.(?:data|rodata|bss|sdata|sbss)\.o$", obj):
            unit = m.group(1)
        elif m := re.match(r"^build/[^/]+/asm/[^/]+/(.*)\.o$", obj):
            unit = m.group(1)
        else:
            continue
        want = v.ranges.get((unit, kind))
        if want and want[0] != start and start != (want[0] & ~0x7F):
            return {compiled_before} if compiled_before else set()
        if obj.startswith(f"build/{v.name}/src/"):
            compiled_before = unit
    return set()


def complete_units() -> list[str]:
    units = []
    for p in sorted((ROOT / "src").rglob("*")):
        if p.suffix in (".c", ".cpp") and not p.relative_to(ROOT / "src").as_posix().startswith("lib/"):
            if "INCLUDE_ASM(" not in p.read_text(encoding="latin-1"):
                units.append(p.relative_to(ROOT / "src").with_suffix("").as_posix())
    return units


def _scan_one(args):
    vname, unit = args
    v = Version(vname)
    res = try_unit(v, unit)
    ok, sections, why = eligible(res)
    funcs = Counter(s for _, s, _ in res["functions"])
    return unit, ok, sections, why, funcs, res["data"]


def sources_list(v: Version) -> list[str]:
    path = v.cfg / "sources.txt"
    return [l.strip() for l in path.read_text(encoding="utf-8").splitlines()
            if l.strip() and not l.startswith("#")] if path.exists() else []


def write_sources(v: Version, units: list[str]) -> None:
    head = ("# Units this version builds from src/ (configure.py: build.yaml `sources:` names this file); written by\n"
            "# tools/retail_units.py switch/verify. One fnmatch pattern per line, `!` excludes.\n")
    (v.cfg / "sources.txt").write_text(head + "".join(f"{u}\n" for u in sorted(set(units))), encoding="utf-8")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("command", choices=("try", "scan", "switch", "verify"))
    ap.add_argument("units", nargs="*")
    ap.add_argument("--version", default="SLUS_217.99_retail")
    ap.add_argument("--show", action="append", default=[])
    ap.add_argument("--data", action="store_true")
    ap.add_argument("--eligible", action="store_true")
    ap.add_argument("-j", type=int, default=16)
    ap.add_argument("--max", type=int, default=10)
    args = ap.parse_args()
    v = Version(args.version)
    v.out.mkdir(parents=True, exist_ok=True)

    if args.command == "try":
        for unit in args.units:
            res = try_unit(v, unit, set(args.show), args.data)
            for n, s, c in res["functions"]:
                print(f"  {s:5s} {n} ({c})")
            for name, e in res["data"].items():
                start = f"0x{e['start']:08X}" if e["start"] is not None else "?"
                print(f"  DATA  {name}: start {start} votes {e['votes'] or '-'} size {e['size']:#x} bytes {e['bytes']}"
                      + ("" if e["referenced"] else " (not referenced by the code)"))
            ok, sections, why = eligible(res)
            print(f"{unit}: {sum(1 for _, s, _ in res['functions'] if s == 'OK')}/{len(res['functions'])} functions; "
                  + (f"eligible, data from source: {' '.join(sections) or 'none'}" if ok else f"not eligible: {why}"))
        return

    scan_path = v.out / "scan.tsv"
    if args.command == "scan":
        units = args.units or complete_units()
        with ProcessPoolExecutor(args.j) as pool:
            results = list(pool.map(_scan_one, [(v.name, u) for u in units]))
        with scan_path.open("w", encoding="utf-8") as f:
            f.write("unit\teligible\tsections\treason\tok\tdiff\textra\tdata\n")
            for unit, ok, sections, why, funcs, data in results:
                dtxt = ";".join(f"{n}={'0x%08X' % e['start'] if e['start'] is not None else '?'}/{e['size']:#x}"
                                for n, e in data.items() if not n.startswith(".gnu"))
                f.write(f"{unit}\t{int(ok)}\t{' '.join(sections)}\t{why}\t{funcs['OK']}\t{funcs['DIFF']}\t"
                        f"{funcs['extra']}\t{dtxt}\n")
        n_ok = sum(1 for r in results if r[1])
        print(f"{n_ok} of {len(results)} units eligible; functions OK {sum(r[4]['OK'] for r in results)}, "
              f"DIFF {sum(r[4]['DIFF'] for r in results)}, extra {sum(r[4]['extra'] for r in results)} -> {scan_path}")
        reasons = Counter(re.sub(r"[0-9a-fx]+", "N", r[3].split(":")[0]) for r in results if not r[1])
        for k, c in reasons.most_common(12):
            print(f"  {c:4d}  {k}")
        return

    rows = {r["unit"]: r for r in csv.DictReader(scan_path.open(encoding="utf-8"), delimiter="\t")}
    if args.command == "switch":
        units = [u for u, r in rows.items() if r["eligible"] == "1"] if args.eligible else args.units
        current = sources_list(v)
        if args.eligible:  # a fresh list: exactly the eligible units (the others lose marker and starts)
            marker = f"// SOURCE DATA {v.name}:"
            for unit in set(current) - set(units):
                src = source_of(unit)
                if src:
                    text = src.read_text(encoding="latin-1")
                    src.write_text("\n".join(l for l in text.rstrip("\n").split("\n") if not l.startswith(marker))
                                   + "\n", encoding="latin-1")
            starts = v.cfg / "unit_starts.txt"
            if starts.exists():
                keep = set(units)
                lines = starts.read_text(encoding="utf-8").splitlines()
                starts.write_text("\n".join(l for l in lines if not l or l.startswith("#") or l.split()[0] in keep)
                                  + "\n", encoding="utf-8")
            current = []
        starts = v.cfg / "unit_starts.txt"
        lines = starts.read_text(encoding="utf-8").splitlines() if starts.exists() else []
        head = [l for l in lines if l.startswith("#")] or [
            "# Exact data starts of retail units, measured from their compiled objects (tools/retail_units.py);",
            "# tools/split_version.py applies them. <unit> <section> <address> <size>"]
        body = [l for l in lines if l and not l.startswith("#")]
        for unit in units:
            r = rows[unit]
            if r["eligible"] != "1":
                print(f"skip {unit}: not eligible ({r['reason']})")
                continue
            sections = r["sections"].split()
            for item in filter(None, r["data"].split(";")):
                name, rest = item.split("=")
                start, size = rest.split("/")
                if name in sections and start != "?":
                    body = [l for l in body if l.split()[:2] != [unit, name]] + [f"{unit} {name} {start} {size}"]
            src = source_of(unit)
            text = src.read_text(encoding="latin-1")
            marker = f"// SOURCE DATA {v.name}:"
            text = "\n".join(l for l in text.rstrip("\n").split("\n") if not l.startswith(marker)) + "\n"
            if sections:
                text += f"{marker} {' '.join(sections)}\n"
            src.write_text(text, encoding="latin-1")
            current.append(unit)
        starts.write_text("\n".join(head + sorted(body)) + "\n", encoding="utf-8")
        write_sources(v, current)
        subprocess.run([sys.executable, "tools/split_version.py", v.name], cwd=ROOT, check=True,
                       stdout=subprocess.DEVNULL)
        print(f"{len(sources_list(v))} units from source in {v.name}")
        return

    if args.command == "verify":
        for rnd in range(args.max):
            r = subprocess.run(["ninja", f"check-{v.name}"], cwd=ROOT, capture_output=True, text=True)
            log = r.stdout + r.stderr
            if r.returncode == 0:
                print(f"OK: {v.name} matches with {len(sources_list(v))} units from source")
                return
            culprits = set()
            for m in re.finditer(r"FAILED: build/[^/]+/src/(\S+)\.o\b", log):
                culprits.add(m.group(1))
            for m in re.finditer(r"build/[^/]+/src/(\S+?)\.o[:(]", log):
                culprits.add(m.group(1))
            v = Version(v.name)  # the split changes every round
            if not culprits:
                culprits = layout_culprits(v)
            if not culprits:
                culprits = image_culprits(v)
            if not culprits:
                print(log[-3000:])
                sys.exit("error: no culprit found")
            current = [u for u in sources_list(v) if u not in culprits]
            print(f"round {rnd + 1}: switching back {len(culprits)}: {', '.join(sorted(culprits)[:8])}")
            for unit in culprits:
                src = source_of(unit)
                if src:
                    marker = f"// SOURCE DATA {v.name}:"
                    text = src.read_text(encoding="latin-1")
                    src.write_text("\n".join(l for l in text.rstrip("\n").split("\n") if not l.startswith(marker))
                                   + "\n", encoding="latin-1")
            write_sources(v, current)
            starts = v.cfg / "unit_starts.txt"
            if starts.exists():
                lines = starts.read_text(encoding="utf-8").splitlines()
                starts.write_text("\n".join(l for l in lines if not l or l.startswith("#") or l.split()[0] not in culprits)
                                  + "\n", encoding="utf-8")
            subprocess.run([sys.executable, "tools/split_version.py", v.name], cwd=ROOT, check=True,
                           stdout=subprocess.DEVNULL)
        print(f"FAIL: {v.name} still differs after {args.max} rounds")
        sys.exit(1)


if __name__ == "__main__":
    main()
