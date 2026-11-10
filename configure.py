#!/usr/bin/env python3
"""Split the game with splat and generate build.ninja.

Run inside WSL/Linux from the repo root:
    python3 configure.py              # uses the only version in config/, or pass one
    python3 configure.py SLUS_217.99 --clean
    ninja                             # build and verify the checksum
"""

import argparse
import fnmatch
import json
import os
import re
import shutil
import subprocess
import sys
from collections import Counter
from pathlib import Path

import ninja_syntax
import yaml
from splat.scripts import split

ROOT = Path(__file__).resolve().parent
CROSS = "mips-linux-gnu-"
AS_FLAGS = "-EL -march=r5900 -mabi=eabi -no-pad-sections -I include"


def pick_version(requested: str | None) -> str:
    versions = sorted(p.name for p in (ROOT / "config").iterdir() if (p / f"{p.name}.yaml").exists())
    if requested:
        if requested not in versions:
            sys.exit(f"error: unknown version {requested!r}; available: {versions or 'none'}")
        return requested
    if len(versions) != 1:
        sys.exit(f"error: pass a version explicitly; available: {versions or 'none (run tools/init_version.py)'}")
    return versions[0]


def ensure_target(target: Path, elf: Path) -> None:
    """splat splits an objcopy'd image of the ELF, not the ELF itself."""
    if target.exists():
        return
    if not elf.exists():
        sys.exit(f"error: neither {target} nor the original ELF {elf} exist (see iso/README.md)")
    target.parent.mkdir(parents=True, exist_ok=True)
    subprocess.run([f"{CROSS}objcopy", "-O", "binary", "--gap-fill=0x00", str(elf), str(target)], check=True)


# mips-linux-gnu-as aligns every section to 16; the original (ee-elf) toolchain only used what the contents asked
# for, and Sony's linker script starts each output section at ALIGN(128). Each object's sections get the alignment
# their original start address implies, capped at gcc's 8 for code and at 128 for data. Any value between the
# original alignment and that address's alignment places the object correctly, now and once neighbours become C.
TYPE_SECTION = {"asm": ".text", "hasm": ".text", "c": ".text", "cpp": ".text", "data": ".data", "rodata": ".rodata",
                "sdata": ".sdata", "bss": ".bss", "sbss": ".sbss"}
SECTION_ALIGN_CAP = {".text": 8}
DEFAULT_ALIGN_CAP = 128


def object_fixups(entries: list, compiled: bool, text_cap: int = SECTION_ALIGN_CAP[".text"]) -> str:
    """objcopy flags for one object (see the comment above TYPE_SECTION for the alignments).

    splat links every object under every section ("auto_link_sections"), interleaved with the real data. In
    assembled objects the sections without a subsegment are empty, but their default alignment of 16 would still
    move things, so it is set to 1. Compiled objects only contribute the sections they have a subsegment for
    (`cpp` for code, `.data`/`.rodata`/`.bss` for data): the others are removed, so a file's code can come from
    C++ while its data still comes from the original assembly. Their .mdebug (from -g) goes too: ld crashes on it.
    """
    owned = {}
    for entry in entries:
        section = TYPE_SECTION.get(entry.segment.type.lstrip("."))
        start = entry.segment.vram_start
        # Skip the auto-linked entries, e.g. a code object listed under .data.
        if section is None or section != entry.section_order_type or not start:
            continue
        cap = text_cap if section == ".text" else SECTION_ALIGN_CAP.get(section, DEFAULT_ALIGN_CAP)
        owned[section] = min(cap, start & -start)
    flags = [f"--set-section-alignment {s}={a}" for s, a in sorted(owned.items())]
    for section in sorted(set(TYPE_SECTION.values()) - set(owned)):
        flags += [f"-R {section} -R .rel{section}"] if compiled else [f"--set-section-alignment {section}=1"]
    if compiled:
        flags.append("-R .mdebug")
    return " ".join(flags)


def align_section_starts(ld_script: Path) -> None:
    """Sony's linker script starts each output section at ALIGN(128) (see TYPE_SECTION). splat's script sets each
    section's start label (`cod_SDATA_START = .;`) right after the previous section's last object, so when that
    object comes from source (no padding in its data) the label lands on the padding instead: `_gp` is computed
    from cod_SDATA_START and every gp-relative offset shifts (libgcc2___umoddi3, the last .rodata object). Align the
    location counter before each data section's label; where the section already starts aligned this is a no-op."""
    text = ld_script.read_text(encoding="utf-8")
    text, n = re.subn(r"^(\s+)(cod_(?:DATA|RODATA|SDATA|SBSS|BSS)_START = \.;)", r"\1. = ALIGN(., 128);\n\1\2",
                      text, flags=re.M)
    if n != 5:
        sys.exit(f"error: expected 5 data section start labels in {ld_script}, found {n}")
    ld_script.write_text(text, encoding="utf-8")


def own_data_ranges(entries: list) -> str:
    """ee_asm_compat.py's --own-data argument for a compiled object: the address ranges of the data sections that
    come from its source. Weak data (vtables of classes whose virtual functions are all inline) that lies outside
    them is dropped: the original linker kept one copy of it, in some other object."""
    ranges = []
    for entry in entries:
        section = TYPE_SECTION.get(entry.segment.type.lstrip("."))
        start, end = entry.segment.vram_start, entry.segment.vram_end
        if section in (None, ".text") or section != entry.section_order_type or not start or end is None:
            continue
        ranges.append(f"0x{start:X}-0x{end:X}")
    return ",".join(ranges) or "none"


def unit_data_ranges(entries: list) -> dict[str, str]:
    """Per unit (segment name): the address ranges of all its data sections, from source or still assembly, for
    ee_asm_compat.py's --unit-data (which original function-local statics are this unit's)."""
    ranges: dict[str, list[str]] = {}
    for entry in entries:
        section = TYPE_SECTION.get(entry.segment.type.lstrip("."))
        start, end = entry.segment.vram_start, entry.segment.vram_end
        if section in (None, ".text") or not start or end is None:
            continue
        ranges.setdefault(entry.segment.name, []).append(f"0x{start:X}-0x{end:X}")
    return {name: ",".join(r) for name, r in ranges.items()}


def compiler_commands(compiler: str) -> tuple[str, str | None]:
    """(compiler command, original assembler command or None) for a package in tools/compilers/."""
    cc_dir = ROOT / "tools" / "compilers" / compiler
    rel = cc_dir.relative_to(ROOT).as_posix()
    if not (cc_dir / "bin" / "ee-gcc.exe").exists():
        return f"{rel}/bin/ee-gcc -B {rel}/bin/ee-", None
    snas = next((p for p in cc_dir.glob("lib/gcc-lib/ee/*/*") if p.name.lower() == "ps2eeas.exe" and not p.is_symlink()),
                None)
    wibo = "tools/wibo/wibo"
    return f"{wibo} {rel}/bin/ee-gcc.exe", f"{wibo} {snas.relative_to(ROOT).as_posix()}" if snas else None


SOURCE_DATA = re.compile(r"^\s*(?://|#)\s*SOURCE DATA:\s*(.*?)\s*$", re.MULTILINE)
SOURCE_SUFFIXES = (".c", ".cpp", ".s")


def unit_toolchain(build_cfg: dict, unit: str) -> dict:
    """compiler, cflags, source_root and assembler for one unit: build.yaml's defaults, overridden by the first entry
    of its `overrides:` list whose `match` (an fnmatch pattern, e.g. lib/graphdev_*) matches the unit name.
    `assembler` names a package in tools/compilers/ whose bin/ee-as or ee/bin/as (GNU as) assembles the compiler's output instead
    of the compiler package's own assembler."""
    tc = {key: build_cfg.get(key) for key in ("compiler", "cflags", "source_root", "assembler")}
    for override in build_cfg.get("overrides") or []:
        if fnmatch.fnmatchcase(unit, override["match"]):
            tc.update({key: override[key] for key in tc if key in override})
            break
    return tc
DATA_TYPES = {".data": "data", ".rodata": "rodata", ".bss": "bss", ".sdata": "sdata", ".sbss": "sbss"}


def source_config(splat_yaml: Path, opts: dict) -> tuple[Path, dict[str, list[str]]]:
    """The splat config with every file that has a source in src/ switched to C/C++.

    config/<VERSION>/<VERSION>.yaml stays as tools/split_objects.py writes it (all asm), so parallel branches never
    edit it. A file's code comes from src/<unit>.cpp (or .c, or .s for hand-written assembly: its subsegment stays
    asm, write_ninja() assembles the source instead of splat's output) as soon as that file exists; its data sections come
    from the compiled object only when the source lists them in a comment, e.g. as its last line
        // SOURCE DATA: .data .rodata
    (until then they stay assembly, e.g. while INCLUDE_ASM'd functions still reference them). At the end of the
    file it shifts no line numbers, which matter wherever __LINE__ ends up in assert or allocation messages.
    Returns (the generated yaml, unit -> data sections from source; every unit with a source is a key).
    """
    text = splat_yaml.read_text(encoding="utf-8")
    sources: dict[str, list[str]] = {}
    for path in sorted((ROOT / "src").rglob("*")):
        if path.suffix not in SOURCE_SUFFIXES:
            continue
        unit = path.relative_to(ROOT / "src").with_suffix("").as_posix()
        kind = "asm" if path.suffix == ".s" else path.suffix.lstrip(".")
        name = re.escape(unit)
        text, count = re.subn(rf"^(      - \[0x[0-9A-F]+, )(?:asm|c|cpp)(, {name}\])", rf"\g<1>{kind}\2", text,
                              flags=re.MULTILINE)
        if count != 1:
            sys.exit(f"error: {path.relative_to(ROOT)}: no code subsegment named {unit} in {splat_yaml.name}")
        marker = SOURCE_DATA.search(path.read_text(encoding="latin-1"))
        sections = marker.group(1).split() if marker else []
        for section in sections:
            if section not in DATA_TYPES:
                sys.exit(f"error: {path.relative_to(ROOT)}: unknown section {section!r} in SOURCE DATA")
            t = DATA_TYPES[section]
            text, n1 = re.subn(rf"^(      - \[0x[0-9A-F]+, )\.?{t}(, {name}\])", rf"\g<1>.{t}\2", text,
                               flags=re.MULTILINE)
            text, n2 = re.subn(rf"^(      - \{{ type: )\.?{t}(, vram: 0x[0-9A-F]+, name: {name} \}})", rf"\g<1>.{t}\2",
                               text, flags=re.MULTILINE)
            if n1 + n2 != 1:
                sys.exit(f"error: {path.relative_to(ROOT)}: SOURCE DATA lists {section}, but {unit} has no such data")
        sources[unit] = sections
    # build/<VERSION>/ is as deep as config/<VERSION>/, so base_path stays the same.
    out = ROOT / opts["build_path"] / splat_yaml.name
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(text, encoding="utf-8", newline="\n")
    return out, sources


def expected_config(splat_yaml: Path, opts: dict) -> tuple[Path, Path]:
    """A copy of the splat config with every file as assembly, writing to build/<VERSION>/expected/.

    Its objects are objdiff's targets: they always come from the original ELF, also for files that are
    already decompiled (splat no longer writes their complete .s in the normal tree). Returns (yaml, asm dir).
    """
    exp = f"{opts['build_path']}/expected"
    text = splat_yaml.read_text(encoding="utf-8")
    text = re.sub(r", (c|cpp), ", ", asm, ", text)
    text = re.sub(r", \.(data|rodata|bss|sdata|sbss), ", r", \1, ", text)
    text = re.sub(r"type: \.(bss|sbss)\b", r"type: \1", text)
    for option, value in {
        "base_path": "../../..",  # the copy lives one level deeper than config/<VERSION>/
        "asm_path": f"{exp}/asm",
        "build_path": f"{exp}/obj",
        "elf_path": f"{exp}/expected.elf",
        "ld_script_path": f"{exp}/expected.ld",
        "undefined_funcs_auto_path": f"{exp}/undefined_funcs_auto.txt",
        "undefined_syms_auto_path": f"{exp}/undefined_syms_auto.txt",
    }.items():
        text, count = re.subn(rf"^  {option}: .*$", f"  {option}: {value}", text, count=1, flags=re.MULTILINE)
        if count != 1:
            sys.exit(f"error: option {option} not found in {splat_yaml}")
    text = text.replace("options:\n", "options:\n  generate_asm_macros_files: False\n", 1)
    path = ROOT / exp / "expected.yaml"
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text, encoding="utf-8", newline="\n")
    return path, ROOT / exp / "asm"


def deduplicated_names(symbols: Path) -> dict[str, str]:
    """name_<ADDR> -> name for the names the symbol file de-duplicated (see tools/import_symbols.py)."""
    names = {}
    if symbols.exists():
        for line in symbols.read_text(encoding="utf-8").splitlines():
            m = re.match(r"^(\S+)_([0-9A-F]{8}) = 0x([0-9A-F]{8});", line)
            if m and m.group(2) == m.group(3):
                names[f"{m.group(1)}_{m.group(2)}"] = m.group(1)
    return names


def expected_renames(sources: list[str], dedup: dict[str, str]) -> str:
    """objcopy options giving a unit's de-duplicated names their original name back (when that stays unique)."""
    found = set()
    for src in sources:
        found |= {w for w in re.findall(r"[\w$.]+_[0-9A-F]{8}\b", (ROOT / src).read_text(encoding="utf-8"))
                  if w in dedup}
    bases = Counter(dedup[w] for w in found)
    # $ in names ("_11BattleEnemy$BadStatusEffectName") is a ninja variable: escape it.
    return " ".join(ninja_syntax.escape(f"--redefine-sym '{w}={dedup[w]}'")
                    for w in sorted(found) if bases[dedup[w]] == 1)


def expected_units(asm_dir: Path) -> dict[str, list[str]]:
    """Unit name -> its .s files (code, data, rodata, bss) in the expected tree."""
    units: dict[str, list[str]] = {}
    for path in sorted(asm_dir.rglob("*.s")):
        rel = path.relative_to(asm_dir).as_posix()
        name = re.sub(r"\.(data|rodata|bss|sdata|sbss)\.s$", "", rel[len("data/"):]) if rel.startswith("data/") \
            else rel[:-len(".s")]
        units.setdefault(name, []).append(path.relative_to(ROOT).as_posix())
    return units


def write_objdiff(opts: dict, build_cfg: dict, units: dict[str, list[str]], compiled: dict[str, tuple[str, str]]):
    """objdiff.json: one unit per object; compiled = unit name -> (object, source) of the decompiled ones."""
    exp = f"{opts['build_path']}/expected/units"
    config = {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "min_version": "2.0.0",
        "custom_make": "ninja",
        "build_target": True,
        "build_base": True,
        "watch_patterns": ["*.c", "*.cpp", "*.h", "*.hpp", "*.inc", "*.s"],
        "units": [],
    }
    for name in sorted(units):
        unit = {"name": name, "target_path": f"{exp}/{name}.o"}
        if name in compiled:
            unit["base_path"], source = compiled[name]
            unit["metadata"] = {"source_path": source}
        tc = unit_toolchain(build_cfg, name)
        unit["scratch"] = {"platform": "ps2", "compiler": tc["compiler"], "c_flags": tc["cflags"]}
        config["units"].append(unit)
    (ROOT / "objdiff.json").write_text(json.dumps(config, indent=2) + "\n", encoding="utf-8", newline="\n")


# A label whose name ends in its address: splat's D_/func_/jtbl_<ADDR>, or <name>_<ADDR> for names it renamed
# (file-local statics like get_st_001828C8, _2PL$g_szDpxFriend_003A814C).
ADDRESS_LABEL = re.compile(r"(?<![\w$.])([A-Za-z_$][\w$.]*?_([0-9A-F]{8}))(?![\w$.])")


def write_original_labels(opts: dict, labels: dict[str, int]) -> str:
    """A linker script that PROVIDEs, at its original address, every address-named label that INCLUDE_ASM'd
    functions or the data's assembly reference. Compiled code names a static, string literal or jump table its own
    way (get_st, $LC12), and with the unit's data from source the original data's labels are gone; Ps2EeAs crashes on aliases (.set, =), so
    the source can't define them either. PROVIDE only defines a label nothing else defines, and the checksum proves
    the data really is at that address. (Labels inside a compiled object's data from source are defined by the
    object itself since port S3: data_label_lines(), tools/name_data_labels.py.)"""
    out = Path(opts["build_path"]) / "original_labels.ld"
    (ROOT / out).parent.mkdir(parents=True, exist_ok=True)
    text = "/* Generated by configure.py (write_original_labels) - do not edit. */\n" + "".join(
        f'PROVIDE("{name}" = 0x{addr:08X});\n' for name, addr in sorted(labels.items()))
    if not (ROOT / out).exists() or (ROOT / out).read_text(encoding="utf-8") != text:
        (ROOT / out).write_text(text, encoding="utf-8")
    return out.as_posix()


def data_label_lines(entries: list, labels_by_addr: list[tuple[int, str]]) -> list[str]:
    """For tools/name_data_labels.py: the address-named labels inside a compiled object's data sections from
    source, as "section offset name" (offset from the section's original start)."""
    import bisect
    addrs = [a for a, _ in labels_by_addr]
    lines = []
    for entry in entries:
        section = TYPE_SECTION.get(entry.segment.type.lstrip("."))
        start, end = entry.segment.vram_start, entry.segment.vram_end
        if section in (None, ".text") or section != entry.section_order_type or not start or end is None:
            continue
        for i in range(bisect.bisect_left(addrs, start), bisect.bisect_left(addrs, end)):
            addr, name = labels_by_addr[i]
            lines.append(f"{section} {addr - start:X} {name}")
    return lines


def address_labels(opts: dict) -> dict[str, int]:
    """Every address-named label (D_004E26D8, jtbl_..., cardDataTbl_004E5840) that the original's assembly, its
    data or the sources mention, with its address."""
    labels: dict[str, int] = {}
    # The data's assembly too: where splat read string bytes as label + addend (.word D_0063696C + 0x2), that label
    # may be defined nowhere once the data around its address comes from source.
    # And the C/C++ sources: compiled code names the original's strings and tables `extern D_xxxxxxxx` while the
    # unit's data is still asm, and a fresh split has no .s file of an already matched function that mentions them.
    for path in sorted([*(ROOT / opts["asm_path"] / "nonmatchings").rglob("*.s"),
                        *(ROOT / opts["asm_path"] / "data").rglob("*.s"),
                        *[p for p in (ROOT / "src").rglob("*") if p.suffix in (".c", ".cpp", ".h", ".s")]]):
        for m in ADDRESS_LABEL.finditer(path.read_text(encoding="latin-1")):
            labels[m.group(1)] = int(m.group(2), 16)
    return labels


# Shifted links (docs/port-step1-shiftable.md): the same objects as the matching build, linked with padding objects
# inserted and without any label pinned to its original address, to prove that nothing depends on where it is.
# config/<VERSION>/shift.yaml lists the presets; each one is linked by `ninja shift-<preset>` (`ninja shift`: all).
SHIFT_SECTIONS = {".text": "TEXT", ".data": "DATA", ".rodata": "RODATA", ".sdata": "SDATA", ".sbss": "SBSS",
                  ".bss": "BSS"}
# Absolute addresses a shifted link may assign: none inside the image area (load address up to the end of RAM).
SHIFT_PINNED_RANGE = (0x00100000, 0x02000000)


def shift_script(base: str, pads: list[tuple[str, str, str]]) -> str:
    """splat's linker script with padding objects inserted. pads: (section, after, object); `after` is a unit name
    (the padding follows that unit's input line of the section) or "start" (first in the section)."""
    lines = base.splitlines()
    for section, after, obj in pads:
        label = SHIFT_SECTIONS[section]
        start = next(i for i, line in enumerate(lines) if line.strip() == f"cod_{label}_START = .;")
        end = next(i for i, line in enumerate(lines) if line.strip() == f"cod_{label}_END = .;")
        if after == "start":
            pos = start + 1
        else:
            unit = re.compile(rf"/(?:src|asm/[^/]+(?:/data)?)/{re.escape(after)}(?:\.\w+)?\.o\(")
            hits = [i for i in range(start, end) if unit.search(lines[i])]
            if len(hits) != 1:
                sys.exit(f"error: shift.yaml: {after} has {len(hits)} input lines in {section} (needs exactly 1)")
            pos = hits[0] + 1
        indent = re.match(r"\s*", lines[start]).group(0)
        lines.insert(pos, f"{indent}{obj}({section});")
    return "\n".join(lines) + "\n"


def assert_no_pins(scripts: dict[str, str]) -> None:
    """A shifted link must not place anything at a fixed address: no PROVIDE, no assignment of an address in the
    image area. Stops configure instead of producing a link that only works because of a pin."""
    for name, text in scripts.items():
        if re.search(r"\bPROVIDE(?:_HIDDEN)?\s*\(", text):
            sys.exit(f"error: shifted link script {name} uses PROVIDE")
        for m in re.finditer(r"([^\s;{}()]+)\s*=\s*(0x[0-9A-Fa-f]+|\d+)\s*;", text):
            value = int(m.group(2), 0)
            if SHIFT_PINNED_RANGE[0] <= value < SHIFT_PINNED_RANGE[1]:
                sys.exit(f"error: shifted link script {name} pins {m.group(1)} = {m.group(2)}")


def write_shift_links(n, cfg_dir: str, opts: dict, objects: list[str]) -> None:
    shift_cfg = ROOT / cfg_dir / "shift.yaml"
    if not shift_cfg.exists():
        return
    presets = yaml.safe_load(shift_cfg.read_text(encoding="utf-8"))["presets"]
    build_dir = opts["build_path"]
    base = (ROOT / opts["ld_script_path"]).read_text(encoding="utf-8")
    extra = f"{cfg_dir}/linker_script_extra.ld"
    n.rule(
        "ld_shift",
        f"{CROSS}ld -EL -nostdlib --no-check-sections --emit-relocs -Map $mapfile -T $ldscript -T {extra} -o $out",
        description="LD (shifted) $out",
    )
    # Padding objects only contribute the section they are listed under; alignment 1, so the padding is exact.
    pad_align = " ".join(f"--set-section-alignment {s}=1" for s in sorted(SHIFT_SECTIONS))
    elves = []
    for preset, entries in presets.items():
        out_dir = f"{build_dir}/shift/{preset}"
        (ROOT / out_dir).mkdir(parents=True, exist_ok=True)
        pads, pad_objects = [], []
        for i, entry in enumerate(entries):
            section, after, size = entry["section"], str(entry["after"]), int(entry["size"])
            if section not in SHIFT_SECTIONS or size <= 0:
                sys.exit(f"error: shift.yaml: preset {preset}: bad entry {entry}")
            src, obj = f"{out_dir}/pad{i}.s", f"{out_dir}/pad{i}.o"
            text = f"# shift.yaml preset {preset}: {size:#x} bytes in {section} after {after}\n" \
                   f".section {section}\n.space {size}\n"
            if not (ROOT / src).exists() or (ROOT / src).read_text(encoding="utf-8") != text:
                (ROOT / src).write_text(text, encoding="utf-8")
            n.build(obj, "as", src, variables={"align_flags": pad_align})
            pads.append((section, after, obj))
            pad_objects.append(obj)
        script = f"{out_dir}/link.ld"
        script_text = shift_script(base, pads)
        assert_no_pins({script: script_text, extra: (ROOT / extra).read_text(encoding="utf-8")})
        if not (ROOT / script).exists() or (ROOT / script).read_text(encoding="utf-8") != script_text:
            (ROOT / script).write_text(script_text, encoding="utf-8")
        elf = f"{out_dir}/{Path(opts['elf_path']).name}"
        n.build(elf, "ld_shift", objects + pad_objects, implicit=[script, extra],
                variables={"ldscript": script, "mapfile": str(Path(elf).with_suffix(".map"))})
        n.build(f"shift-{preset}", "phony", elf)
        elves.append(elf)
    n.build("shift", "phony", elves)
    n.newline()


def toolchain_variables(tc: dict) -> dict[str, str]:
    """The cc rule's per-toolchain variables (see unit_toolchain()): compiler command, flags, assembler command
    (without -o) and the extra arguments for ee_asm_compat.py."""
    gp = re.search(r"-G\s*(\d+)", tc["cflags"])
    cc, snas = compiler_commands(tc["compiler"])
    extra = f"--source-root {tc['source_root']}" if tc.get("source_root") else ""
    if tc.get("assembler"):
        package = f"tools/compilers/{tc['assembler']}"
        as_path = f"{package}/bin/ee-as" if (ROOT / package / "bin/ee-as").exists() else f"{package}/ee/bin/as"
        assemble = f"{as_path} -EL -G{gp.group(1) if gp else 0} -I include"
    elif snas:
        assemble = f"{snas} --stdout -G{gp.group(1) if gp else 0}"
        extra += " --assembler snas" if extra else "--assembler snas"
    else:
        assemble = f"{CROSS}as {AS_FLAGS} -G{gp.group(1) if gp else 0}"
    return {"cc": cc, "cflags": tc["cflags"], "assemble": assemble, "compat_extra": extra}


def write_ninja(version: str, splat_yaml: Path, opts: dict, sha1: str, build_cfg: dict,
                units: dict[str, list[str]]) -> dict[str, str]:
    cfg_dir = f"config/{version}"
    build_dir = opts["build_path"]
    elf_out = opts["elf_path"]
    rom_out = str(Path(elf_out).with_suffix(".rom"))
    ld_script = opts["ld_script_path"]

    symbols = f"{cfg_dir}/elf_symbol_addrs.txt"
    default_tc = toolchain_variables(unit_toolchain({**build_cfg, "overrides": []}, ""))

    # linker_script_matching.ld: checks that only hold for the original layout (e.g. where _end lands); the shifted
    # links (write_shift_links) leave it out.
    labels = address_labels(opts)
    labels_by_addr = sorted((addr, name) for name, addr in labels.items())
    extra_scripts = [f"{cfg_dir}/linker_script_extra.ld", f"{cfg_dir}/linker_script_matching.ld"] + [
        f"{cfg_dir}/{name}" for name in ("undefined_funcs_auto.txt", "undefined_syms_auto.txt")
        if (ROOT / cfg_dir / name).exists()
    ] + [write_original_labels(opts, labels)]

    with (ROOT / "build.ninja").open("w", encoding="utf-8", newline="\n") as f:
        n = ninja_syntax.Writer(f, width=120)
        n.comment(f"Generated by configure.py for {version} - do not edit.")
        n.variable("asflags", f"{AS_FLAGS} -G0")
        n.variable("name_labels", "true")  # per object: tools/name_data_labels.py (see data_label_lines)
        # The default toolchain (build.yaml); units matched by its `overrides:` set their own (see unit_toolchain).
        for key, value in default_tc.items():
            n.variable(key, value)
        n.newline()

        # See alignment_flags().
        set_align = f"{CROSS}objcopy $align_flags $out"
        n.rule("as", f"{CROSS}as $asflags -o $out $in && {set_align}", description="AS $in")
        # Compile to assembly with the unit's compiler, post-process it with tools/ee_asm_compat.py and
        # assemble it with the original assembler when the package has one (SN Systems' Ps2EeAs: it inserts
        # the EE's hazard and erratum nops, guided by -g's line labels), otherwise with modern binutils.
        # Windows-only tools (SN Systems' ee-gcc 2.95.x) run under wibo.
        compat_args = f" --source $in --symbols {symbols} --own-data $own_data --unit-data $unit_data" if (ROOT / symbols).exists() else ""
        compat_args += " $compat_extra"
        assemble = "$assemble -o $out $out.s"
        # The preprocessor writes the included headers to $out.d (gcc 2.95 has no -MT: its target is the bare
        # object name, so it is replaced by $out), which ninja reads to rebuild the object when a header changes.
        n.rule(
            "cc",
            f"$cc -S $cflags -I include -I src -Wp,-MD,$out.d -o $out.gcc.s $in"
            f" && sed -i '1s|^[^:]*:|$out:|' $out.d"
            f" && {sys.executable} tools/ee_asm_compat.py $out.gcc.s $out.s{compat_args}"
            f" && {assemble} && {set_align} && $name_labels",
            description="CC $in",
            depfile="$out.d",
            deps="gcc",
        )
        n.rule("bin", f"{CROSS}ld -EL -r -b binary -o $out $in", description="BIN $in")
        n.rule(
            "ld",
            f"{CROSS}ld -EL -nostdlib --no-check-sections -Map $mapfile -T $ldscript $extra_scripts -o $out",
            description="LD $out",
        )
        n.rule("objcopy", f"{CROSS}objcopy -O binary --gap-fill=0x00 $in $out", description="OBJCOPY $out")
        n.rule("sha1", "echo '$sha1  $in' | sha1sum --quiet -c - && touch $out", description="SHA1 $in")
        n.rule(
            "configure",
            f"{sys.executable} configure.py {version}",
            description="CONFIGURE (re-running splat)",
            generator=True,
        )
        n.newline()

        # splat emits one linker entry per section of the same file; group them per object.
        by_object: dict[str, list] = {}
        for entry in split.linker_writer.entries:
            if entry.object_path is not None and entry.src_paths:
                by_object.setdefault(entry.object_path.as_posix(), []).append(entry)

        unit_data = unit_data_ranges(split.linker_writer.entries)
        objects = list(by_object)
        compiled: dict[str, tuple[str, str]] = {}
        for obj, entries in by_object.items():
            src = entries[0].src_paths[0].as_posix()
            suffix = entries[0].src_paths[0].suffix
            unit = entries[0].segment.name
            text_cap = SECTION_ALIGN_CAP[".text"]
            if suffix == ".s" and (ROOT / "src" / f"{unit}.s").exists():  # hand-written assembly as source
                src = f"src/{unit}.s"
                text_cap = DEFAULT_ALIGN_CAP  # its code may be aligned further (tlbtrap.s: 128), unlike gcc's
            align = {"align_flags": object_fixups(entries, compiled=suffix in (".c", ".cpp", ".cc"), text_cap=text_cap)}
            if suffix in (".c", ".cpp", ".cc"):  # ee-gcc picks cc1 / cc1plus from the extension
                tc = toolchain_variables(unit_toolchain(build_cfg, unit))
                naming, implicit = {}, ["tools/ee_asm_compat.py"]
                label_lines = data_label_lines(entries, labels_by_addr)
                if label_lines:
                    label_file = ROOT / f"{obj}.labels"
                    label_file.parent.mkdir(parents=True, exist_ok=True)
                    text = "\n".join(label_lines) + "\n"
                    if not label_file.exists() or label_file.read_text(encoding="utf-8") != text:
                        label_file.write_text(text, encoding="utf-8")
                    # ($out is not expanded in a build's own variables: the paths are written out)
                    naming = {"name_labels": f"{sys.executable} tools/name_data_labels.py {obj} {obj}.labels"}
                    implicit += ["tools/name_data_labels.py", f"{obj}.labels"]
                n.build(obj, "cc", src, variables={**align, **naming, "own_data": own_data_ranges(entries),
                                                 "unit_data": unit_data.get(unit, "none"),
                                                 **{k: v for k, v in tc.items() if v != default_tc[k]}},
                        implicit=implicit)
                compiled[entries[0].segment.name] = (obj, src)
            elif suffix == ".s":
                n.build(obj, "as", src, variables=align)
                if src.startswith("src/"):
                    compiled[unit] = (obj, src)
            elif suffix == ".bin":
                n.build(obj, "bin", src)
            else:
                sys.exit(f"error: don't know how to build {src}")
        n.newline()

        n.build(
            elf_out, "ld", objects,
            implicit=[ld_script, *extra_scripts],
            variables={"ldscript": ld_script, "extra_scripts": " ".join(f"-T {s}" for s in extra_scripts),
                       "mapfile": str(Path(elf_out).with_suffix(".map"))},
        )
        n.build(rom_out, "objcopy", elf_out)
        n.build(f"{build_dir}/{Path(rom_out).name}.ok", "sha1", rom_out, variables={"sha1": sha1})
        n.build("check", "phony", f"{build_dir}/{Path(rom_out).name}.ok")
        n.default("check")
        # Not built by default: copies the 4+ GB disc image on first use.
        n.rule("iso", f"{sys.executable} tools/build_iso.py {version}", description="ISO $out")
        n.build(f"{build_dir}/{version}.iso", "iso", elf_out, implicit=["tools/build_iso.py"])
        n.build("iso", "phony", f"{build_dir}/{version}.iso")
        n.newline()

        write_shift_links(n, cfg_dir, opts, objects)

        # objdiff targets (see expected_config()): every unit's code and data assembled from the original, merged
        # into one object like a compiled source file. Built on demand by objdiff or with `ninja expected`.
        n.rule("as_expected", f"{CROSS}as $asflags -o $out $in", description="AS $in")
        # Names that occur more than once in the game (file-local statics like randf) carry an _<ADDR> suffix in
        # the symbol file; in the merged objects they get their original name back, as in compiled objects, so
        # objdiff pairs them.
        exp = f"{build_dir}/expected"
        # symbol_addrs.txt (by hand) names e.g. weak copies whose names the original linker dropped the same way.
        dedup = {**deduplicated_names(ROOT / cfg_dir / "elf_symbol_addrs.txt"),
                 **deduplicated_names(ROOT / cfg_dir / "symbol_addrs.txt")}
        n.rule("merge", f"{CROSS}ld -EL -r -o $out $in && {CROSS}objcopy $renames $out", description="MERGE $out")
        unit_objects = []
        for name, sources in units.items():
            parts = []
            for src in sources:
                part = f"{exp}/obj/{Path(src).relative_to(f'{exp}/asm').with_suffix('.o').as_posix()}"
                n.build(part, "as_expected", src)
                parts.append(part)
            unit_objects.append(f"{exp}/units/{name}.o")
            n.build(unit_objects[-1], "merge", parts, variables={"renames": expected_renames(sources, dedup)})
        n.build("expected", "phony", unit_objects)
        n.newline()

        n.build(
            "build.ninja", "configure",
            implicit=["configure.py", splat_yaml.relative_to(ROOT).as_posix(), f"{cfg_dir}/build.yaml",
                      *(p.relative_to(ROOT).as_posix() for p in (ROOT / cfg_dir).glob("*_addrs.txt")),
                      *(p.relative_to(ROOT).as_posix() for p in (ROOT / cfg_dir).glob("shift.yaml")),
                      # new sources switch files to C/C++ (see source_config); a directory changes when one is added
                      *(p.relative_to(ROOT).as_posix() for p in [ROOT / "src", *(ROOT / "src").rglob("*")]
                        if p.is_dir())],
        )
    return compiled


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("version", nargs="?", help="folder name under config/")
    parser.add_argument("--clean", action="store_true", help="delete generated asm and build output first")
    args = parser.parse_args()

    os.chdir(ROOT)  # splat and ninja both work with paths relative to the repo root
    version = pick_version(args.version)
    cfg_dir = ROOT / "config" / version
    splat_yaml = cfg_dir / f"{version}.yaml"
    splat_cfg = yaml.safe_load(splat_yaml.read_text(encoding="utf-8"))
    build_cfg = yaml.safe_load((cfg_dir / "build.yaml").read_text(encoding="utf-8"))
    opts = splat_cfg["options"]

    if args.clean:
        for path in (opts["asm_path"], opts["build_path"]):
            shutil.rmtree(ROOT / path, ignore_errors=True)

    ensure_target(ROOT / opts["target_path"], ROOT / build_cfg["elf"])
    build_yaml, sources = source_config(splat_yaml, opts)

    # The all-asm split for objdiff's targets runs in parallel with the real one.
    exp_yaml, exp_asm = expected_config(splat_yaml, opts)
    shutil.rmtree(exp_asm, ignore_errors=True)
    exp_log = exp_yaml.with_suffix(".log")
    with exp_log.open("w") as log:
        exp_split = subprocess.Popen([sys.executable, "-m", "splat", "split", str(exp_yaml)], stdout=log, stderr=log)
        # No cache: a cached run writes incomplete undefined_*_auto.txt files and the link fails.
        split.main([build_yaml], modes=["all"], verbose=False, use_cache=False)
        if exp_split.wait() != 0:
            sys.exit(f"error: the expected split failed, see {exp_log.relative_to(ROOT)}")
    align_section_starts(ROOT / opts["ld_script_path"])

    units = expected_units(exp_asm)
    compiled = write_ninja(version, splat_yaml, opts, splat_cfg["sha1"], build_cfg, units)
    write_objdiff(opts, build_cfg, units, compiled)
    with_data = sum(1 for sections in sources.values() if sections)
    print(f"\nwrote build.ninja and objdiff.json ({len(units)} units, {len(compiled)} from source, "
          f"{with_data} with data from source) for {version} - run `ninja`")


if __name__ == "__main__":
    main()
