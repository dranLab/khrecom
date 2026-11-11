#!/usr/bin/env python3
"""Write the splat config and symbol file of another version of the game from the beta's split and the beta ->
version map (docs/retail-plan.md, Phase 1).

The other version (the US retail ELF) has no .mdebug, so tools/split_objects.py can't split it. Its objects are the
beta's, in the same link order (tools/version_map.py pairs 94 % of the functions and keeps the order), so the beta's
units are carried over: each unit starts where the first of its functions (data items) that keep the link order
lies in the other version. Units with nothing in the other version (debug code) are left out.

Built without -g, the other version has gcc's .gnu.linkonce sections: every object's weak (inline) functions sit in
one block after all .text, and its vtables (and other weak data) in one block at the end of .data, both in link
order. A unit gets a second subsegment there, named `<unit>.linkonce` (configure.py builds it from the same source
once the unit is compiled).

Writes config/<VERSION>/<VERSION>.yaml (the beta's options and section layout, this version's subsegments) and
config/<VERSION>/elf_symbol_addrs.txt (the names of this version's SN DLL table, plus the beta's names of the paired
functions and data the table doesn't name). Run tools/version_map.py first.

config/<VERSION>/unit_starts.txt (optional, `<unit> <.data|.rodata|.bss> 0x<address> [0x<size>]` per line) overrides
a unit's data start with the exact one measured from its compiled object (tools/retail_units.py switch); with
the size, the next unit starts at its end.

Usage: .venv/bin/python tools/split_version.py SLUS_217.99_retail [--dry-run]
"""

import argparse
import bisect
import csv
import hashlib
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

import yaml

sys.path.insert(0, str(Path(__file__).resolve().parent))
from version_map import BETA, Image, ROOT, anon_key  # noqa: E402

SUBSEGMENT = re.compile(r"^      - (?:\[0x([0-9A-F]+), ([.\w]+), ([^\]]+)\]|\{ type: ([.\w]+), vram: 0x([0-9A-F]+), "
                        r"name: ([^ }]+) \})")
LINKONCE = ".linkonce"


def lis(seq: list[tuple[int, object]]) -> set[int]:
    """Indices of a longest strictly increasing subsequence of seq's values (seq: [(value, key)] in beta order)."""
    tails, tails_idx, prev = [], [], [-1] * len(seq)
    for i, (v, _) in enumerate(seq):
        k = bisect.bisect_left(tails, v)
        if k == len(tails):
            tails.append(v)
            tails_idx.append(i)
        else:
            tails[k] = v
            tails_idx[k] = i
        prev[i] = tails_idx[k - 1] if k else -1
    out, i = set(), tails_idx[-1] if tails_idx else -1
    while i >= 0:
        out.add(i)
        i = prev[i]
    return out


def beta_blocks(text: str) -> dict[str, list[tuple[int, str, str]]]:
    """The beta yaml's generated blocks: section -> [(address, type, unit)] (rom offsets as addresses)."""
    blocks = {}
    for section in (".text", ".data", ".rodata", ".bss"):
        body = text.split(f"# begin {section},")[1].split(f"# end {section}")[0]
        rows = []
        for line in body.splitlines():
            m = SUBSEGMENT.match(line)
            if not m:
                continue
            if m.group(1):
                rows.append((0x100000 + int(m.group(1), 16), m.group(2), m.group(3).strip()))
            else:
                rows.append((int(m.group(5), 16), m.group(4), m.group(6).strip()))
        blocks[section] = rows
    return blocks


def unit_of(rows, addr):
    starts = [r[0] for r in rows]
    i = bisect.bisect_right(starts, addr) - 1
    return rows[i][2] if i >= 0 else None


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version")
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()

    cfg = ROOT / "config" / args.version
    build_cfg = yaml.safe_load((cfg / "build.yaml").read_text(encoding="utf-8"))
    other = Image(ROOT / build_cfg["elf"])
    beta = Image(ROOT / "iso" / BETA)
    vmap = ROOT / "build" / Path(build_cfg["elf"]).name / "version_map"
    funcs = list(csv.DictReader((vmap / "functions.tsv").open(encoding="utf-8"), delimiter="\t"))
    datas = list(csv.DictReader((vmap / "data.tsv").open(encoding="utf-8"), delimiter="\t"))
    beta_yaml = (ROOT / "config" / BETA / f"{BETA}.yaml").read_text(encoding="utf-8")
    blocks = beta_blocks(beta_yaml)
    sec = {name: (a, a + n) for name, (a, n) in other.sections.items()}
    bsec = {name: (a, a + n) for name, (a, n) in beta.sections.items()}
    in_sec = lambda s, a: s in sec and sec[s][0] <= a < sec[s][1]
    in_bsec = lambda s, a: s in bsec and bsec[s][0] <= a < bsec[s][1]
    report = Counter()

    # beta weak functions (their kept copies) from the beta ELF's symbol table
    from elftools.elf.elffile import ELFFile
    with (ROOT / "iso" / BETA).open("rb") as f:
        weak = {s["st_value"] for s in ELFFile(f).get_section_by_name(".symtab").iter_symbols()
                if s["st_info"]["bind"] == "STB_WEAK" and s["st_info"]["type"] == "STT_FUNC"}

    # ---- .text: strong code, then the linkonce block -------------------------------------------------------------
    paired = sorted((int(r["beta"], 16), int(r["other"], 16)) for r in funcs if r["other"])
    game_weak = [(b, o) for b, o in paired if b in weak and not unit_of(blocks[".text"], b).startswith("lib/")]
    weak_start = min(o for _, o in game_weak)
    strong = [(o, b) for b, o in paired if o < weak_start]
    linkonce = [(o, b) for b, o in paired if o >= weak_start]
    text_lines = []
    for kind, seq, suffix in (("strong", strong, ""), ("linkonce", linkonce, LINKONCE)):
        keep = lis(seq)
        report[f".text {kind}: pairs dropped (out of order)"] = len(seq) - len(keep)
        first = {}
        for i, (o, b) in enumerate(seq):
            if i in keep:
                first.setdefault(unit_of(blocks[".text"], b), o)
        for unit, o in first.items():
            text_lines.append((o, "asm", unit + suffix))
        report[f".text {kind}: units"] = len(first)
    text_lines.sort()
    if text_lines[0][2] != blocks[".text"][0][2] or text_lines[0][0] - sec[".text"][0] > 0x10:
        sys.exit(f"error: the first unit is {text_lines[0][2]} at {text_lines[0][0]:#x}, not the beta's first at "
                 f".text's start")
    text_lines[0] = (sec[".text"][0],) + text_lines[0][1:]  # crt0: its first function follows a header
    left_out = sorted({u for _, _, u in blocks[".text"]} - {u for _, _, u in text_lines})
    report[".text: beta units with no code here"] = len(left_out)

    # ---- data sections -----------------------------------------------------------------------------------------
    # beta data address -> other address: names first, code evidence when it is consistent (no conflicts) and stays
    # in the corresponding section (beta .rodata vtables -> .data's linkonce block)
    dpairs = []
    for r in datas:
        b, o = int(r["beta"], 16), int(r["other"], 16)
        if r["evidence"] != "name" and (int(r["conflicts"]) or int(r["evidence"]) < 2):
            continue
        if b % 4 or o % 4:
            continue  # inside an item (a merged string's tail): no object starts there
        dpairs.append((b, o, r["beta_symbol"]))
    vt_start = min(o for b, o, n in dpairs if n.startswith("_vt$") and in_sec(".data", o))
    data_lines = defaultdict(list)
    for bsect, osect, rows_key in ((".data", ".data", ".data"), (".rodata", ".rodata", ".rodata"),
                                   (".rodata", ".data", ".rodata"), (".bss", ".bss", ".bss")):
        linkonce_block = bsect == ".rodata" and osect == ".data"
        seq = sorted((o, b) for b, o, _ in dpairs if in_bsec(bsect, b) and in_sec(osect, o)
                     and (not linkonce_block or o >= vt_start) and (bsect != ".data" or o < vt_start))
        seq.sort(key=lambda x: x[1])  # beta order
        keep = lis(seq)
        report[f"{bsect}->{osect}: pairs used / dropped"] = f"{len(keep)} / {len(seq) - len(keep)}"
        first = {}
        for i, (o, b) in enumerate(seq):
            if i in keep:
                first.setdefault(unit_of(blocks[rows_key], b), o)
        if not linkonce_block:
            # The libraries are the same objects: their data keeps its layout. Each library unit starts at its beta
            # start + the offset of its own name-paired items (else the previous library unit's offset).
            offsets = defaultdict(Counter)
            for b, o, n in dpairs:
                if n and in_bsec(bsect, b) and in_sec(osect, o):
                    u = unit_of(blocks[rows_key], b)
                    if u.startswith("lib/"):
                        offsets[u][o - b] += 1
            libs = [(a, u) for a, _, u in blocks[rows_key] if u.startswith("lib/") and u != "common"]
            if bsect == ".bss":  # the libraries' .bss ends where COMMON starts, in both builds
                common_beta = next(a for a, _, u in blocks[".bss"] if u == "common")
                common_other = min(o for b, o, n in dpairs if in_bsec(".bss", b)
                                   and unit_of(blocks[".bss"], b) == "common")
                offset = common_other - common_beta
            else:
                total = sum(offsets.values(), Counter())
                offset = total.most_common(1)[0][0] if total else None
            if offset is not None:
                first = {u: o for u, o in first.items() if not u.startswith("lib/")}
                # a unit without pairs of its own takes the offset of the nearest library unit (by beta address)
                # that has them (lib/nodebug/__FRAME_BEGIN__0 sits 0x1c before default_terminate, not after)
                paired = [(a, offsets[u].most_common(1)[0][0]) for a, u in libs if offsets.get(u)]
                for a, u in libs:
                    if bsect != ".bss" and paired:
                        offset = offsets[u].most_common(1)[0][0] if offsets.get(u) else \
                            min(paired, key=lambda p: abs(p[0] - a))[1]
                    first[u] = a + offset
                report[f"{bsect}: library units placed by offset"] = len(libs)
        for unit, o in first.items():
            if unit == "common":
                continue
            name = unit + (LINKONCE if linkonce_block else "")
            data_lines[osect].append((o, osect.lstrip("."), name))
    # the section starts belong to the first unit; COMMON: the beta's COMMON symbols that the table names
    common = [o for b, o, n in dpairs if in_bsec(".bss", b) and unit_of(blocks[".bss"], b) == "common"]
    common_start = min(common)
    data_lines[".bss"] = [x for x in data_lines[".bss"] if x[0] < common_start] + [(common_start, "bss", "common")]
    # exact starts measured from compiled units (tools/retail_units.py switch): config/<VERSION>/unit_starts.txt
    starts_file = cfg / "unit_starts.txt"
    pushed = set()  # (section, unit) moved to the end of the unit before it: yields when it lands on a real start
    # unit_starts.manual.txt: starts measured by hand (same format; units the scan can't measure, e.g. a data-only
    # unit's .rodata or a unit still in assembly whose data starts before its first named symbol)
    manual_file = cfg / "unit_starts.manual.txt"
    if starts_file.exists() or manual_file.exists():
        recorded = []
        text = (starts_file.read_text(encoding="utf-8") if starts_file.exists() else "") + "\n" + \
            (manual_file.read_text(encoding="utf-8") if manual_file.exists() else "")
        for line in text.splitlines():
            if line.strip() and not line.startswith("#"):
                unit, section, addr, *size = line.split()
                recorded.append((unit, section, int(addr, 16), int(size[0], 16) if size else None))
        fixed = {(u, sec) for u, sec, _, _ in recorded}
        for unit, section, addr, _ in recorded:
            data_lines[section] = [x for x in data_lines[section] if x[2] != unit and x[0] != addr] + \
                [(addr, section.lstrip("."), unit)]
            report["starts from unit_starts.txt"] += 1
        for unit, section, addr, size in recorded:
            if size is None:
                continue
            # the next unit starts where this one ends (the gap is its leading data or alignment), unless its start
            # is recorded too (then the gap is its alignment)
            rows = sorted(data_lines[section])
            k = next(i for i, x in enumerate(rows) if x[2] == unit)
            nxt = rows[k + 1][0] if k + 1 < len(rows) else None
            if nxt is not None and nxt < addr + size and (rows[k + 1][2], section) not in fixed:
                rows[k + 1] = (addr + size,) + rows[k + 1][1:]  # it started inside this unit: it follows it
                pushed.add((section, rows[k + 1][2]))
                nxt = addr + size
            align = min(128, nxt & -nxt) if nxt else 1  # configure.py gives an object its start's alignment
            gap_data = nxt and section != ".bss" and any(
                other.data[other.off(addr + size):other.off(addr + size) + (nxt - addr - size)])
            if nxt and nxt > addr + size and ((addr + size + align - 1) & -align != nxt or gap_data):
                # the gap isn't the next object's alignment padding (or it holds data). A unit between the two in link order that has
                # nothing of its own in this section here owns it (PackMan's .rodata between MotData and PathCamera);
                # else it is the next object's leading data
                order = [u for _, _, u in blocks[section]]
                present = {x[2] for x in rows}
                between = []
                if unit in order and rows[k + 1][2] in order:
                    between = [u for u in order[order.index(unit) + 1:order.index(rows[k + 1][2])]
                               if u not in present and u not in left_out]
                if between:
                    rows.insert(k + 1, (addr + size, section.lstrip("."), between[0]))
                elif (rows[k + 1][2], section) not in fixed:
                    rows[k + 1] = (addr + size,) + rows[k + 1][1:]
                    pushed.add((section, rows[k + 1][2]))
            data_lines[section] = rows
    for s in (".data", ".rodata", ".bss"):
        # at one address a unit that starts there wins over one pushed there (retail: WorldWarp's .data takes the
        # beta's __FRAME_BEGIN__0 bytes, which then lands on __do_global_dtors' start with nothing of its own)
        rows = sorted(data_lines[s], key=lambda x: (x[0], (s, x[2]) in pushed, x[1], x[2]))
        if any(a == sec[s][0] for a, _, _ in rows) and rows[0][0] < sec[s][0]:
            # a library unit placed by offset before the section start is not in this version when another unit
            # starts the section (retail: lib/nodebug/_GLOBAL__F_0build.cpp's frame data; System/Coord's .data
            # starts .data)
            report[f"{s}: units dropped (before the section start)"] += sum(a < sec[s][0] for a, _, _ in rows)
            rows = [x for x in rows if x[0] >= sec[s][0]]
        if rows[0][0] != sec[s][0]:
            # the leading part without a pair: the beta's first unit of the section, if this version has it (crt0's
            # .bss: _args), else the first unit found
            first = blocks[s][0][2]
            if rows[0][2] != first and first not in left_out and first not in {n for _, _, n in rows}:
                rows.insert(0, (sec[s][0], rows[0][1], first))
            else:
                rows[0] = (sec[s][0], rows[0][1], rows[0][2])
        # several units can't start at the same address: keep the first in link order
        out, seen = [], set()
        for a, t, n in rows:
            if a in seen:
                report[f"{s}: units dropped (same start)"] += 1
                continue
            seen.add(a)
            out.append((a, t, n))
        data_lines[s] = out
        report[f"{s}: units"] = len(out)

    rom = lambda a: a - 0x100000

    def block(section, rows):
        lines = [f"      # begin {section}, generated by tools/split_version.py"]
        for a, t, n in rows:
            if t in ("bss", "sbss"):
                lines.append(f"      - {{ type: {t}, vram: 0x{a:08X}, name: {n} }}")
            else:
                lines.append(f"      - [0x{rom(a):06X}, {t}, {n}]")
        lines.append(f"      # end {section}")
        return "\n".join(lines)

    # ---- the yaml: the beta's, with this version's values and blocks ------------------------------------------
    text = beta_yaml
    image = (ROOT / "iso" / f"{Path(build_cfg['elf']).name}.rom").read_bytes()
    gen = (cfg / f"{args.version}.yaml").read_text(encoding="utf-8") if (cfg / f"{args.version}.yaml").exists() else ""
    gp = re.search(r"^  gp_value: (0x[0-9A-F]+)$", gen, re.M) if gen else None
    bss = re.search(r"^    bss_size: (0x[0-9A-F]+)$", gen, re.M) if gen else None
    if not gp or not bss:
        sys.exit(f"error: run tools/init_version.py first (gp_value / bss_size from splat's create_config)")
    text = text.replace(f"({BETA})", f"({args.version})", 1)
    text = re.sub(r"^sha1: \w+$", f"sha1: {hashlib.sha1(image).hexdigest()}", text, count=1, flags=re.M)
    text = re.sub(r"^  gp_value: 0x[0-9A-F]+$", f"  gp_value: {gp.group(1)}", text, count=1, flags=re.M)
    text = re.sub(r"^    bss_size: 0x[0-9A-F]+$", f"    bss_size: {bss.group(1)}", text, count=1, flags=re.M)
    text = re.sub(rf"\b{re.escape(BETA)}(?!_)", args.version, text)
    for section in (".text", ".data", ".rodata", ".bss"):
        rows = text_lines if section == ".text" else data_lines[section]
        text = re.sub(rf"^      # begin {re.escape(section)},.*?^      # end {re.escape(section)}$",
                      lambda _: block(section, rows), text, count=1, flags=re.M | re.S)
    # the single subsegments: .sndata, .sdata, .sbss, and the end of the image
    text = re.sub(r"^      - \[0x[0-9A-F]+, data, cod/[0-9A-F]+\] # \.sndata$",
                  f"      - [0x{rom(sec['.sndata'][0]):06X}, data, cod/{rom(sec['.sndata'][0]):06X}] # .sndata",
                  text, count=1, flags=re.M)
    text = re.sub(r"^      - \[0x[0-9A-F]+, sdata, cod/[0-9A-F]+\] # \.sdata$",
                  f"      - [0x{rom(sec['.sdata'][0]):06X}, sdata, cod/{rom(sec['.sdata'][0]):06X}] # .sdata",
                  text, count=1, flags=re.M)
    text = re.sub(r"^      - \{ type: sbss, vram: 0x[0-9A-F]+, name: cod/[0-9A-F]+ \} # \.sbss$",
                  f"      - {{ type: sbss, vram: 0x{sec['.sbss'][0]:08X}, name: cod/{sec['.sbss'][0]:08X} }} # .sbss",
                  text, count=1, flags=re.M)
    text = re.sub(r"^  - \[0x[0-9A-F]+\]$", f"  - [0x{len(image):X}]", text, count=1, flags=re.M)

    # ---- symbols ------------------------------------------------------------------------------------------------
    syms: dict[int, tuple[str, str]] = {}  # address -> (name, comment)
    table = other.dll_table()
    lo, hi = sec[".text"]
    sizes = {int(r["other"], 16): int(r["other_size"], 16) for r in funcs if r["other"] and r["status"] == "same"}
    image_end = max(b for _, b in sec.values())
    for name, value, typ in table:
        if typ in (1, 4) or not 0x100000 <= value < image_end:
            continue  # imports, absolutes, values outside the image
        if value in syms:
            continue
        if lo <= value < hi:
            syms[value] = (name, "type:func" + (f" size:0x{sizes[value]:X}" if value in sizes else ""))
        else:
            syms[value] = (name, "")
    # anonymous-namespace / static constructor names carry a per-compile suffix: beta names added below get this
    # version's suffix for their file (from its table's names) so that every name of a file agrees
    # (in a mangled type the namespace is followed by the nested name's length: ...cppWHaaaa13_MainTaskInfo)
    suffix_re = re.compile(r"((?:_\d+)?_GLOBAL_\$[NID]\$)(.*?\.(?:cpp|c))([0-9A-Za-z_]{6})(\$|\d|$)")
    version_suffix = {}
    for n, _ in syms.values():
        if m := suffix_re.search(n):
            version_suffix.setdefault((m.group(1).lstrip("_0123456789"), m.group(2)), m.group(3))

    def own_suffix(n: str) -> str:
        def sub(m):
            suf = version_suffix.get((m.group(1).lstrip("_0123456789"), m.group(2)))
            return m.group(1) + m.group(2) + (suf or m.group(3)) + m.group(4)
        return suffix_re.sub(sub, n)
    for r in funcs:
        if r["other"] and int(r["other"], 16) not in syms and r["how"] != "gap":
            o = int(r["other"], 16)
            syms[o] = (own_suffix(r["name"]), "type:func" + (f" size:0x{sizes[o]:X}" if o in sizes else ""))
    for b, o, n in dpairs:
        if n and o not in syms and not re.fullmatch(r"D_[0-9A-F]{8}", n):
            syms[o] = (own_suffix(n), "")
    syms.setdefault(sec[".sndata"][0], ("sn_dll_header_root", ""))
    # names that occur more than once get an _<ADDR> suffix (as tools/import_symbols.py does)
    counts = Counter(n for n, _ in syms.values())
    out = [f"// Generated by tools/split_version.py from {build_cfg['elf']}'s SN DLL table and the beta -> "
           f"{args.version} map - do not edit, add manual symbols to symbol_addrs.txt."]
    for a in sorted(syms):
        n, c = syms[a]
        if counts[n] > 1:
            n = f"{n}_{a:08X}"
        out.append(f"{n} = 0x{a:08X};" + (f" // {c}" if c else ""))
    report["symbols"] = len(syms)
    report["symbols renamed (duplicate names)"] = sum(1 for n, _ in syms.values() if counts[n] > 1)

    for k, v in report.items():
        print(f"{k}: {v}")
    print("beta units with no code here:", ", ".join(left_out[:40]) + (" ..." if len(left_out) > 40 else ""))
    if args.dry_run:
        return
    (cfg / f"{args.version}.yaml").write_text(text, encoding="utf-8", newline="\n")
    (cfg / "elf_symbol_addrs.txt").write_text("\n".join(out) + "\n", encoding="utf-8", newline="\n")
    print(f"wrote {cfg.relative_to(ROOT)}/{args.version}.yaml and elf_symbol_addrs.txt")


if __name__ == "__main__":
    main()
