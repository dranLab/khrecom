#!/usr/bin/env python3
"""Split the game into its original object files using the ELF's .mdebug info.

.text: object boundaries come from the gcc2_compiled. label gcc puts at the
start of every object (also those built without debug info) plus asm files
with procedure entries; names come from the .mdebug file descriptors at those
addresses. Code after an object's last function is hand-written asm (e.g.
expand.s).

.data / .rodata / .bss: objects keep their link order in every section. Each
object's data starts at the lowest of its own symbols (statics and globals from
.mdebug) and the anonymous labels (string literals, jump tables, float
constants) that only its code references. Uninitialized globals the linker put
after all objects (COMMON) form one final .bss unit. .sndata, .sdata and .sbss
(all small COMMON) stay single files.

Objects without debug info are named lib/nodebug/<first symbol>. Paths:
c:/KHCOM/MainApp/<path>.cpp -> <path>, everything else (Sony SDK, newlib,
libgcc) -> lib/<path without ../ prefixes>.

The data split reads the code's references from the split .text assembly, so
run configure.py once after the .text split:
    python3 tools/split_objects.py SLUS_217.99 --text-only && python3 configure.py
    python3 tools/split_objects.py SLUS_217.99 --dry-run    # report only
    python3 tools/split_objects.py SLUS_217.99
"""

import argparse
import re
import sys
from collections import Counter
from dataclasses import dataclass
from pathlib import Path, PurePosixPath
from typing import Optional

import yaml
from elftools.elf.elffile import ELFFile

import mdebug

ROOT = Path(__file__).resolve().parent.parent
GAME_ROOT = "c:/KHCOM/MainApp/"
COMPILER_MARKERS = {"gcc2_compiled.", "__gnu_compiled_c", "__gnu_compiled_cplusplus"}
PROC_TYPES = {mdebug.ST_PROC, mdebug.ST_STATICPROC}
DATA_SECTIONS = {".data": mdebug.SC_DATA, ".rodata": mdebug.SC_RDATA, ".bss": mdebug.SC_BSS}
SINGLE_LINES = {  # the subsegment splat's create_config wrote for each section
    ".text": r"      - \[0x000000, asm, [^\]]+\] # \.text\n",
    ".data": r"      - \[0x[0-9A-F]+, data, [^\]]+\] # \.data\n",
    ".rodata": r"      - \[0x[0-9A-F]+, rodata, [^\]]+\] # \.rodata\n",
    ".bss": r"      - \{ type: bss, vram: 0x[0-9A-F]+, name: [^}]+\} # \.bss\n",
}
REF = re.compile(r"%(?:hi|lo|gp_rel)\(([A-Za-z_][\w.$]*)")
ANON = re.compile(r"^[A-Za-z]+_([0-9A-F]{8})$")  # spimdisasm's D_xxxxxxxx, jtbl_xxxxxxxx, ...


@dataclass
class Unit:
    start: int
    source: str  # source path from .mdebug, or "" for code linked without debug info
    first_symbol: str
    fdr: Optional[int] = None  # .mdebug file descriptor index
    name: str = ""


@dataclass
class Obj:
    """One object file in link order."""
    name: str
    fdr: Optional[int]
    text: Optional[Unit]


def defined_procs(sf: mdebug.SourceFile, lo: int, hi: int) -> list[int]:
    return [s.value for s in sf.symbols if s.sc == mdebug.SC_TEXT and s.st in PROC_TYPES and lo <= s.value < hi]


def source_name(source: str, first_symbol: str) -> str:
    if not source:
        return "lib/nodebug/" + re.sub(r"[^\w.+-]", "_", first_symbol)
    path = source.replace("\\", "/")
    if path.lower().startswith(GAME_ROOT.lower()):
        rel = path[len(GAME_ROOT):]
    else:
        rel = "lib/" + "/".join(p for p in PurePosixPath(re.sub(r"^[a-zA-Z]:/", "", path)).parts
                                if p not in ("..", ".", "/"))
    return re.sub(r"[^\w/.+-]", "_", str(PurePosixPath(rel).with_suffix("")))


def split_text(elf_syms, files, text_start, text_end):
    """Returns (units, included source names, problems)."""
    # Functions plus other code labels (hand-written asm such as ExpandData__FPUcT0 or md0_* labels).
    text_syms = sorted((s["st_value"], s["st_size"], s.name) for s in elf_syms
                       if text_start <= s["st_value"] < text_end and s.name.isprintable()
                       and s["st_info"]["type"] in ("STT_FUNC", "STT_NOTYPE")
                       and s.name not in COMPILER_MARKERS and not s.name.startswith("$"))
    size_of = {a: size for a, size, _ in text_syms}
    # gcc emits a gcc2_compiled. label at the start of every object it compiles: the ground truth for C/C++ units.
    object_starts = {s["st_value"] for s in elf_syms if s.name == "gcc2_compiled." and text_start <= s["st_value"] < text_end}

    defined = {}  # function address -> index of the file descriptor defining it (headers included)
    for sf in files:
        for a in defined_procs(sf, text_start, text_end):
            defined.setdefault(a, sf.index)

    # Objects start at gcc2_compiled. markers (also for objects without debug info) and at asm files with procedure
    # entries. A source with code but no marker at its address was #included into another file (mm.c into mpc.c).
    by_start: dict[int, mdebug.SourceFile] = {}
    included = []
    for sf in files:
        if sf.name.endswith(".h") or not defined_procs(sf, text_start, text_end):
            continue
        if sf.adr in object_starts or sf.name.endswith(".s"):
            by_start.setdefault(sf.adr, sf)
        else:
            included.append(sf.index)
    starters = {sf.index for sf in by_start.values()}

    # Data-only objects have their marker in the previous object's padding: only cut where code follows.
    cuts = sorted(object_starts | set(by_start) | {text_start})
    has_code = lambda lo, hi: any(lo <= a < hi for a, _, _ in text_syms)
    cuts = [c for c, nxt in zip(cuts, cuts[1:] + [text_end]) if c == text_start or has_code(c, nxt)]

    units: list[Unit] = []
    bounds = cuts + [text_end]
    for lo, hi in zip(bounds, bounds[1:]):
        sf = by_start.get(lo)
        first = next((n for a, _, n in text_syms if lo <= a < hi), "")
        units.append(Unit(lo, sf.name if sf else "", first, sf.index if sf else None))
        if sf is None:
            continue
        # Code after the last function of a debug-info object comes from an asm file without procedure entries
        # (expand.s). Its descriptor's address is unreliable, so start right after the last function, 8-aligned.
        end = max(a + size_of.get(a, 0) for a in defined if lo <= a < hi)
        gap = [(a, n) for a, _, n in text_syms if end <= a < hi and a not in defined]
        if gap:
            following = next((i for i in range(sf.index + 1, len(files)) if i in starters), len(files))
            asm = [f for f in files[sf.index + 1:following] if f.name.endswith(".s")]
            units.append(Unit((end + 7) & ~7, asm[0].name if asm else "", gap[0][1], asm[0].index if asm else None))

    # Cross-check: functions of each object's own descriptor must lie inside that object's unit.
    unit_bounds = [u.start for u in units] + [text_end]
    unit_at = lambda a: next(u for u, lo, hi in zip(units, unit_bounds, unit_bounds[1:]) if lo <= a < hi)
    problems = [(hex(a), files[i].name) for a, i in defined.items() if i in starters and unit_at(a).fdr != i]
    return units, included, problems


def unique_names(names: list[str], suffixes: list[str]) -> list[str]:
    counts = Counter(names)
    out = [f"{n}_{s}" if counts[n] > 1 else n for n, s in zip(names, suffixes)]  # e.g. libgcc2.c, once per helper
    if len(set(out)) != len(out):
        sys.exit("error: duplicate subsegment names remain")
    return out


def link_order(files, units, included) -> list[Obj]:
    """All objects in link order: debug-info objects follow the file descriptors, objects without debug info sit
    where their code is."""
    text_by_fdr = {u.fdr: u for u in units if u.fdr is not None}
    nodebug = [u for u in units if u.fdr is None]
    skip = set(included) | {sf.index for sf in files if sf.name.endswith(".h")}
    order, n = [], 0
    for sf in files:
        if sf.index in skip:
            continue
        unit = text_by_fdr.get(sf.index)
        if unit is not None:
            while n < len(nodebug) and nodebug[n].start < unit.start:
                order.append(Obj(nodebug[n].name, None, nodebug[n]))
                n += 1
        order.append(Obj("", sf.index, unit))
    order += [Obj(u.name, None, u) for u in nodebug[n:]]
    # Data-only objects need names of their own, unique among all objects.
    data_only = [o for o in order if o.text is None]
    taken = {u.name for u in units}
    for o in data_only:
        base = source_name(files[o.fdr].name, "")
        o.name = base if base not in taken else f"{base}_{files[o.fdr].adr:08X}"
        taken.add(o.name)
    for o in order:
        if o.text is not None:
            o.name = o.text.name
    return order


def owner_map(files, order) -> dict[int, int]:
    """file descriptor index -> position in link order (headers / #included sources belong to their includer)."""
    pos = {o.fdr: i for i, o in enumerate(order) if o.fdr is not None}
    owner, current = {}, None
    for sf in files:
        current = pos.get(sf.index, current)
        if current is not None:
            owner[sf.index] = current
    return owner


def code_refs(order, asm_dir: Path) -> dict[int, set[int]]:
    """link position -> addresses of anonymous labels referenced by that object's code."""
    refs = {}
    for i, o in enumerate(order):
        if o.text is None:
            continue
        path = asm_dir / f"{o.name}.s"
        if not path.exists():
            sys.exit(f"error: {path} missing - run configure.py after the .text split first (see --help)")
        refs[i] = {int(m.group(1), 16) for name in REF.findall(path.read_text(encoding="utf-8"))
                   if (m := ANON.match(name))}
    return refs


def split_data(section, lo, hi, order, files, owner, refs, elf_named) -> tuple[list[tuple[int, str]], dict]:
    """Returns ([(start, name)], stats)."""
    sc = DATA_SECTIONS[section]
    own: dict[int, set[int]] = {}
    for sf in files:
        for s in sf.symbols + sf.externals:
            if s.sc == sc and s.st in (mdebug.ST_GLOBAL, mdebug.ST_STATIC) and lo <= s.value < hi and sf.index in owner:
                own.setdefault(owner[sf.index], set()).add(s.value)
    claimed = set().union(*own.values()) if own else set()

    # A referenced label can only be the object's own if it lies before every later object's first symbol
    # (code also references other objects' unnamed data, e.g. VU microcode tables linked at the end of .data).
    later_min = [hi] * (len(order) + 1)
    for i in range(len(order) - 1, -1, -1):
        later_min[i] = min(later_min[i + 1], min(own[i]) if i in own else hi)

    stats = Counter()
    cuts: list[tuple[int, str, int]] = []  # (start, name, last known address)
    running = lo - 1
    for i, o in enumerate(order):
        mine = own.get(i, set())
        if mine and min(mine) <= running:
            stats["order problems"] += 1
            print(f"  {section}: {o.name} starts at {min(mine):#x}, before the previous object's data ends")
        anon = {a for a in refs.get(i, ()) if lo <= a < hi and a not in claimed}
        foreign = {a for a in anon if a <= running or a >= later_min[i + 1]}
        stats["refs dropped"] += len(foreign)
        anon -= foreign
        stats["refs used"] += len(anon)
        addrs = mine | anon
        if not addrs:
            continue
        if anon and min(anon) < (min(mine) if mine else hi):
            stats["starts moved by refs"] += 1
        cuts.append((min(addrs), o.name, max(addrs)))
        running = max(addrs)

    # Named symbols nobody claims come from objects without debug info: give them a unit of their own when they
    # come after everything known about the object in front of them.
    unclaimed = sorted((a, n) for a, n in elf_named if lo <= a < hi and a not in claimed)
    bounds = [c[0] for c in cuts] + [hi]
    extra = []
    for (start, name, last), nxt in zip(cuts, bounds[1:]):
        tail = [(a, n) for a, n in unclaimed if last < a < nxt]
        if tail:
            extra.append((tail[0][0], "lib/nodebug/" + re.sub(r"[^\w.+-]", "_", tail[0][1]), tail[0][0]))
    before_first = [(a, n) for a, n in unclaimed if a < (cuts[0][0] if cuts else hi)]
    if before_first:
        extra.append((before_first[0][0], "lib/nodebug/" + re.sub(r"[^\w.+-]", "_", before_first[0][1]), 0))
    stats["nodebug units"] = len(extra)
    result = sorted([(s, n) for s, n, _ in cuts + extra])
    result[0] = (lo, result[0][1])  # the first unit starts at the section start
    stats["units"] = len(result)
    return result, stats


def common_start(files, owner, lo, hi) -> int:
    """Start of the COMMON area: the first .bss symbol that breaks link order (only globals may follow)."""
    syms = sorted((s.value, owner[sf.index], s.st) for sf in files for s in sf.symbols + sf.externals
                  if s.sc == mdebug.SC_BSS and s.st in (mdebug.ST_GLOBAL, mdebug.ST_STATIC)
                  and lo <= s.value < hi and sf.index in owner)
    highest = -1
    for i, (a, pos, _) in enumerate(syms):
        if pos < highest:
            if any(st == mdebug.ST_STATIC for _, _, st in syms[i:]):
                sys.exit(f"error: static .bss symbols after the presumed COMMON start {a:#x}")
            return a
        highest = pos
    return hi


def replace_block(text_yaml: str, section: str, lines: list[str]) -> str:
    begin = f"      # begin {section}, generated by tools/split_objects.py"
    end = f"      # end {section}"
    new_block = "\n".join([begin] + lines + [end]) + "\n"
    block = re.compile(rf"^      # begin {re.escape(section)},.*?^{re.escape(end)}\n", re.MULTILINE | re.DOTALL)
    if block.search(text_yaml):
        return block.sub(lambda _: new_block, text_yaml, count=1)
    single = re.compile(rf"^{SINGLE_LINES[section]}", re.MULTILINE)
    if single.search(text_yaml):
        return single.sub(lambda _: new_block, text_yaml, count=1)
    sys.exit(f"error: can't find the {section} subsegment in the yaml")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("version", help="folder name under config/")
    parser.add_argument("--dry-run", action="store_true", help="print the split without changing the yaml")
    parser.add_argument("--text-only", action="store_true", help="only split .text")
    args = parser.parse_args()

    cfg_dir = ROOT / "config" / args.version
    splat_yaml = cfg_dir / f"{args.version}.yaml"
    build_cfg = yaml.safe_load((cfg_dir / "build.yaml").read_text(encoding="utf-8"))
    opts = yaml.safe_load(splat_yaml.read_text(encoding="utf-8"))["options"]
    elf_path = ROOT / build_cfg["elf"]

    with elf_path.open("rb") as f:
        elf = ELFFile(f)
        sections = {s.name: s for s in elf.iter_sections()}
        text = sections[".text"]
        text_start, text_end = text["sh_addr"], text["sh_addr"] + text["sh_size"]
        first_load = min(s["sh_offset"] for s in elf.iter_sections() if s["sh_flags"] & 2 and s["sh_type"] != "SHT_NOBITS")
        rom = lambda vram: text["sh_offset"] - first_load + (vram - text_start)
        ranges = {n: (sections[n]["sh_addr"], sections[n]["sh_addr"] + sections[n]["sh_size"]) for n in DATA_SECTIONS}
        elf_syms = [s for s in sections[".symtab"].iter_symbols() if s.name]
    elf_named = [(s["st_value"], s.name) for s in elf_syms
                 if s["st_info"]["type"] in ("STT_OBJECT", "STT_NOTYPE") and s.name.isprintable()
                 and s.name not in COMPILER_MARKERS and not isinstance(s["st_shndx"], str)]

    files = mdebug.read_mdebug(elf_path)
    units, included, problems = split_text(elf_syms, files, text_start, text_end)
    names = unique_names([source_name(u.source, u.first_symbol) for u in units],
                         [u.first_symbol or f"{u.start:08X}" for u in units])
    for u, n in zip(units, names):
        u.name = n

    nodebug = [u for u in units if u.fdr is None]
    print(f".text: {len(files)} file descriptors -> {len(units)} objects with code: "
          f"{sum(not u.name.startswith('lib/') for u in units)} game, "
          f"{sum(u.name.startswith('lib/') for u in units) - len(nodebug)} library, {len(nodebug)} without debug info")
    print(f"  included sources (no object of their own): {[files[i].name for i in included]}")
    print(f"  {len(problems)} functions outside their object")
    for item in problems[:10]:
        print("    outside:", item)
    if problems:
        sys.exit("error: refusing to split with functions outside their object")

    blocks = {".text": [f"      - [0x{rom(u.start):06X}, asm, {u.name}] # {u.source or 'no debug info'}" for u in units]}

    if not args.text_only:
        order = link_order(files, units, included)
        owner = owner_map(files, order)
        refs = code_refs(order, ROOT / opts["asm_path"])
        for section in DATA_SECTIONS:
            lo, hi = ranges[section]
            common = common_start(files, owner, lo, hi) if section == ".bss" else hi
            cuts, stats = split_data(section, lo, common, order, files, owner, refs, elf_named)
            if common < hi:
                cuts.append((common, "common"))
                stats["units"] += 1
            print(f"{section}: {dict(stats)}" + (f", COMMON from {common:#x}" if common < hi else ""))
            if section == ".bss":
                blocks[section] = [f"      - {{ type: bss, vram: 0x{a:08X}, name: {n} }}" for a, n in cuts]
            else:
                kind = section.lstrip(".")
                blocks[section] = [f"      - [0x{rom(a):06X}, {kind}, {n}]" for a, n in cuts]

    if args.dry_run:
        for section, lines in blocks.items():
            print(f"{section}:\n" + "\n".join(lines[:4] + ["      ..."] + lines[-2:]))
        return

    text_yaml = splat_yaml.read_text(encoding="utf-8")
    for section, lines in blocks.items():
        text_yaml = replace_block(text_yaml, section, lines)
    splat_yaml.write_text(text_yaml, encoding="utf-8", newline="\n")
    print(f"wrote {', '.join(f'{len(v)} {k}' for k, v in blocks.items())} subsegments to {splat_yaml.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
