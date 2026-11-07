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
constants) that only its code (or its .data pointers) references. In .rodata it starts even earlier:
gcc writes the string literals of included headers' inline functions first,
used or not, so unreferenced strings (ASCII or Shift-JIS) before an object's
first known item are its own; other unreferenced data there (e.g. the vtable of a
template class) is the previous object's. Uninitialized globals the linker put
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
DATA_REF = re.compile(r"\.word ([A-Za-z_][\w.$]*)")  # pointers in an object's .data (e.g. a table of names)
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
    """link position -> addresses of anonymous labels referenced by that object's code or .data pointers."""
    anon = lambda names: {int(m.group(1), 16) for name in names if (m := ANON.match(name))}
    refs = {}
    for i, o in enumerate(order):
        # Pointers in .data count too: an object without code (a table of names) owns the strings it points to.
        data = asm_dir / "data" / f"{o.name}.data.s"
        mine = anon(DATA_REF.findall(data.read_text(encoding="utf-8"))) if data.exists() else set()
        if o.text is not None:
            path = asm_dir / f"{o.name}.s"
            if not path.exists():
                sys.exit(f"error: {path} missing - run configure.py after the .text split first (see --help)")
            mine |= anon(REF.findall(path.read_text(encoding="utf-8")))
        if mine or o.text is not None:
            refs[i] = mine
    return refs


def item_end(addr: int, sizes: dict[int, int], read) -> int:
    """End of the data item at addr: the symbol's size, or a NUL-terminated string's length, else one word."""
    if sizes.get(addr):
        return addr + sizes[addr]
    data = read(addr, 0x400)
    end = data.find(b"\0")
    if end > 0 and is_text(data[:end]):
        return addr + end + 1
    return addr + 4


def starts_text(addr: int, limit: int, read) -> bool:
    """Whether a NUL-terminated text string starts at addr and ends before limit."""
    data = read(addr, max(0, limit - addr))
    end = data.find(b"\0")
    return end > 0 and is_text(data[:end])


def is_text(data: bytes) -> bool:
    """Printable text, ASCII or Shift-JIS (the game's messages, e.g. TRACE output, are in Japanese). Shift-JIS
    text must be at least 4 bytes: shorter runs are as likely binary data (e.g. 9C 75 in a float constant)."""
    if all(0x20 <= c < 0x7F or c in (9, 10, 13) for c in data):
        return True
    try:
        text = data.decode("cp932")
    except UnicodeDecodeError:
        return False
    return len(data) >= 4 and all(c.isprintable() or c in "\t\n\r" for c in text)


def split_data(section, lo, hi, order, files, owner, refs, elf_named, sizes=None, read=None, text_range=(0, 0)
               ) -> tuple[list[tuple[int, str]], dict]:
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

    # gcc writes the string literals of a header's inline functions when it reads the class definition, i.e. at
    # the start of the including file's .rodata, used or not (assert/allocation messages like "Gprim.h(613)").
    # Unreferenced data between one object's last known item and the next object's first one is therefore the
    # next object's leading data.
    if section == ".rodata" and read is not None:
        named = sorted(a for a, _ in elf_named if lo <= a < hi and a not in claimed)
        for k in range(1, len(cuts)):
            end = item_end(cuts[k - 1][2], sizes or {}, read)
            # Named data nobody claims belongs to an object without debug info (handled below): stop there.
            end = max([end] + [item_end(a, sizes or {}, read) for a in named if end <= a < cuts[k][0]])
            # Words that are code addresses continue the previous object's jump table (a table of an object
            # without debug info is only known by its first entry; "8:8\0" = 0x00383A38 is no string).
            end = (end + 3) & ~3
            while end < cuts[k][0] and text_range[0] <= int.from_bytes(read(end, 4), "little") < text_range[1]:
                end += 4
            # The leading data are string literals; anything else before the first one (e.g. the vtable of a
            # template class, which has no symbol) is the previous object's trailing data.
            # A code address is no string, even when its bytes read as one (0x00226538 = "8e\"": a vtable entry).
            is_code = lambda p: text_range[0] <= int.from_bytes(read(p, 4), "little") < text_range[1]
            first = next((p for p in range((end + 3) & ~3, cuts[k][0], 4)
                          if not is_code(p) and starts_text(p, cuts[k][0], read)), None)
            if first is not None:
                cuts[k] = (first, cuts[k][1], cuts[k][2])
                stats["leading data moved to the next object"] += 1

        # Objects that only include a header (its inline functions' strings, unused) have no reference into
        # .rodata, so their blocks land in the next object's leading data. gcc merges identical strings within one
        # object: when the leading data repeat their first string, each repeat starts a new object. If the repeats
        # match the objects of the link order between the two cuts that have no data of their own, give each of
        # those its block, in order (CollUtilA and CollUtilF: CollUtil.h's strings, before CollUtilO's).
        pos = {o.name: i for i, o in enumerate(order)}
        for k in range(1, len(cuts)):
            start, name, last = cuts[k]
            prev_name = cuts[k - 1][1]
            if name not in pos or prev_name not in pos:
                continue
            between = [order[i].name for i in range(pos[prev_name] + 1, pos[name])
                       if order[i].text is not None and order[i].fdr is not None]
            if not between or not starts_text(start, cuts[k][2] + 1, read):
                continue
            head = read(start, 0x400)
            head = head[:head.find(b"\0") + 1]  # the block's first string, with its NUL
            limit = min(a for a in refs.get(pos[name], ()) if a >= start) if refs.get(pos[name]) else cuts[k][2]
            repeats = [p for p in range(start + 4, limit + 1, 4) if read(p, len(head)) == head]  # incl. its own block
            if len(repeats) != len(between):
                continue
            blocks = [start] + repeats
            for nm, b in zip(between, blocks[:-1]):
                cuts.append((b, nm, b))
            cuts[k] = (blocks[-1], name, last)
            stats["header-only objects given their strings"] += len(between)
        cuts.sort()

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


SUBSEGMENT = re.compile(r"^      - (?:\[(0x[0-9A-F]+), ([.\w]+), ([^\]]+)\]|\{ type: ([.\w]+), vram: (0x[0-9A-F]+), name: ([^ }]+) \})")


def keep_types(old_block: str, lines: list[str]) -> list[str]:
    """Keep the types of subsegments the user changed (asm -> cpp, data -> .data, ...) if name and start are the same."""
    types = {}
    for line in old_block.splitlines():
        if m := SUBSEGMENT.match(line):
            start, kind, name = (m[1], m[2], m[3]) if m[1] else (m[5], m[4], m[6])
            types[(start, name)] = kind
    out = []
    for line in lines:
        m = SUBSEGMENT.match(line)
        if m and m[1] and (m[1], m[3]) in types:
            line = line.replace(f", {m[2]}, ", f", {types[(m[1], m[3])]}, ", 1)
        elif m and m[4] and (m[5], m[6]) in types:
            line = line.replace(f"type: {m[4]},", f"type: {types[(m[5], m[6])]},", 1)
        out.append(line)
    return out


def replace_block(text_yaml: str, section: str, lines: list[str]) -> str:
    begin = f"      # begin {section}, generated by tools/split_objects.py"
    end = f"      # end {section}"
    block = re.compile(rf"^      # begin {re.escape(section)},.*?^{re.escape(end)}\n", re.MULTILINE | re.DOTALL)
    if old := block.search(text_yaml):
        new_block = "\n".join([begin] + keep_types(old.group(0), lines) + [end]) + "\n"
        return block.sub(lambda _: new_block, text_yaml, count=1)
    new_block = "\n".join([begin] + lines + [end]) + "\n"
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
        # configure.py's all-asm tree (objdiff's targets) still has complete .s files for decompiled units.
        refs = code_refs(order, ROOT / opts["build_path"] / "expected" / "asm")
        image = (ROOT / opts["target_path"]).read_bytes()
        read = lambda vram, size: image[rom(vram):rom(vram) + size]
        sizes = {s["st_value"]: s["st_size"] for s in elf_syms if s["st_size"]}
        for section in DATA_SECTIONS:
            lo, hi = ranges[section]
            common = common_start(files, owner, lo, hi) if section == ".bss" else hi
            cuts, stats = split_data(section, lo, common, order, files, owner, refs, elf_named, sizes, read,
                                     (text_start, text_end))
            if common < hi:
                cuts.append((common, "common"))
                stats["units"] += 1
            print(f"{section}: {dict(stats)}" + (f", COMMON from {common:#x}" if common < hi else ""))
            if section == ".bss":
                blocks[section] = [f"      - {{ type: bss, vram: 0x{a:08X}, name: {n} }}" for a, n in cuts]
            else:
                kind = section.lstrip(".")
                blocks[section] = [f"      - [0x{rom(a):06X}, {kind}, {n}]" for a, n in cuts]
            if section == ".data":
                # Objects without debug info and without code (newlib's impure.o) only show up here, as named
                # .data. They can own data in the later sections too (impure.o's "C" locale string in .rodata,
                # pointed at from its .data), so they join the link order: between the .data objects around
                # them, before the first object there whose references in a later section come after theirs.
                section_of = lambda a: next((n for n, (l, h) in ranges.items() if l <= a < h), None)
                known = {o.name for o in order}
                for k, (_, name) in enumerate(cuts):
                    if name in known or name == "common":
                        continue
                    pos = {o.name: i for i, o in enumerate(order)}
                    prv = next((n for _, n in reversed(cuts[:k]) if n in known), None)
                    nxt = next((n for _, n in cuts[k + 1:] if n in known), None)
                    lo_i, hi_i = (pos[prv] + 1 if prv else 0), (pos[nxt] if nxt else len(order))
                    mine = [a for a in code_refs([Obj(name, None, None)], ROOT / opts["build_path"] / "expected" / "asm")
                            .get(0, ()) if section_of(a) not in (None, ".data")]
                    at = hi_i
                    if mine:
                        first, sec = min(mine), section_of(min(mine))
                        for i in range(lo_i, hi_i):
                            theirs = [a for a in refs.get(i, ()) if section_of(a) == sec]
                            if theirs and min(theirs) > first:
                                at = i
                                break
                    order.insert(at, Obj(name, None, None))
                    refs = {(j + 1 if j >= at else j): v for j, v in refs.items()}
                    known.add(name)
                owner = owner_map(files, order)
                refs = code_refs(order, ROOT / opts["build_path"] / "expected" / "asm")
            elif section == ".rodata" and any(o.fdr is None and o.text is None for o in order):
                # Only for .rodata: impure.o's .data also points at two .bss words that _vfwrite's code references
                # too; which object owns them is not settled, so .bss keeps the split it had without these objects.
                order[:] = [o for o in order if o.fdr is not None or o.text is not None]
                owner = owner_map(files, order)
                refs = code_refs(order, ROOT / opts["build_path"] / "expected" / "asm")

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
