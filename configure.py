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


def object_fixups(entries: list, compiled: bool) -> str:
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
        owned[section] = min(SECTION_ALIGN_CAP.get(section, DEFAULT_ALIGN_CAP), start & -start)
    flags = [f"--set-section-alignment {s}={a}" for s, a in sorted(owned.items())]
    for section in sorted(set(TYPE_SECTION.values()) - set(owned)):
        flags += [f"-R {section} -R .rel{section}"] if compiled else [f"--set-section-alignment {section}=1"]
    if compiled:
        flags.append("-R .mdebug")
    return " ".join(flags)


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
    """compiler, cflags and source_root for one unit: build.yaml's defaults, overridden by the first entry of its
    `overrides:` list whose `match` (an fnmatch pattern, e.g. lib/graphdev_*) matches the unit name."""
    tc = {key: build_cfg.get(key) for key in ("compiler", "cflags", "source_root")}
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


def write_original_labels(opts: dict) -> str:
    """A linker script that PROVIDEs, at its original address, every address-named label that INCLUDE_ASM'd
    functions or the data's assembly reference. Compiled code names a static, string literal or jump table its own
    way (get_st, $LC12), and with the unit's data from source the original data's labels are gone; Ps2EeAs crashes on aliases (.set, =), so
    the source can't define them either. PROVIDE only defines a label nothing else defines, and the checksum proves
    the data really is at that address."""
    labels: dict[str, int] = {}
    # The data's assembly too: where splat read string bytes as label + addend (.word D_0063696C + 0x2), that label
    # may be defined nowhere once the data around its address comes from source.
    for path in sorted([*(ROOT / opts["asm_path"] / "nonmatchings").rglob("*.s"),
                        *(ROOT / opts["asm_path"] / "data").rglob("*.s")]):
        for m in ADDRESS_LABEL.finditer(path.read_text(encoding="utf-8")):
            labels[m.group(1)] = int(m.group(2), 16)
    out = Path(opts["build_path"]) / "original_labels.ld"
    (ROOT / out).parent.mkdir(parents=True, exist_ok=True)
    text = "/* Generated by configure.py (write_original_labels) - do not edit. */\n" + "".join(
        f'PROVIDE("{name}" = 0x{addr:08X});\n' for name, addr in sorted(labels.items()))
    if not (ROOT / out).exists() or (ROOT / out).read_text(encoding="utf-8") != text:
        (ROOT / out).write_text(text, encoding="utf-8")
    return out.as_posix()


def toolchain_variables(tc: dict) -> dict[str, str]:
    """The cc rule's per-toolchain variables (see unit_toolchain()): compiler command, flags, assembler command
    (without -o) and the extra arguments for ee_asm_compat.py."""
    gp = re.search(r"-G\s*(\d+)", tc["cflags"])
    cc, snas = compiler_commands(tc["compiler"])
    extra = f"--source-root {tc['source_root']}" if tc.get("source_root") else ""
    if snas:
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

    extra_scripts = [f"{cfg_dir}/linker_script_extra.ld"] + [
        f"{cfg_dir}/{name}" for name in ("undefined_funcs_auto.txt", "undefined_syms_auto.txt")
        if (ROOT / cfg_dir / name).exists()
    ] + [write_original_labels(opts)]

    with (ROOT / "build.ninja").open("w", encoding="utf-8", newline="\n") as f:
        n = ninja_syntax.Writer(f, width=120)
        n.comment(f"Generated by configure.py for {version} - do not edit.")
        n.variable("asflags", f"{AS_FLAGS} -G0")
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
            f" && {assemble} && {set_align}",
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
            if suffix == ".s" and (ROOT / "src" / f"{unit}.s").exists():  # hand-written assembly as source
                src = f"src/{unit}.s"
            align = {"align_flags": object_fixups(entries, compiled=suffix in (".c", ".cpp", ".cc"))}
            if suffix in (".c", ".cpp", ".cc"):  # ee-gcc picks cc1 / cc1plus from the extension
                tc = toolchain_variables(unit_toolchain(build_cfg, unit))
                n.build(obj, "cc", src, variables={**align, "own_data": own_data_ranges(entries),
                                                 "unit_data": unit_data.get(unit, "none"),
                                                 **{k: v for k, v in tc.items() if v != default_tc[k]}},
                        implicit=["tools/ee_asm_compat.py"])
                compiled[entries[0].segment.name] = (obj, src)
            elif suffix == ".s":
                n.build(obj, "as", src, variables=align)
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

    units = expected_units(exp_asm)
    compiled = write_ninja(version, splat_yaml, opts, splat_cfg["sha1"], build_cfg, units)
    write_objdiff(opts, build_cfg, units, compiled)
    with_data = sum(1 for sections in sources.values() if sections)
    print(f"\nwrote build.ninja and objdiff.json ({len(units)} units, {len(compiled)} from source, "
          f"{with_data} with data from source) for {version} - run `ninja`")


if __name__ == "__main__":
    main()
