#!/usr/bin/env python3
"""Split the game with splat and generate build.ninja.

Run inside WSL/Linux from the repo root:
    python3 configure.py              # uses the only version in config/, or pass one
    python3 configure.py SLUS_217.99 --clean
    ninja                             # build and verify the checksum
"""

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
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
        unit["scratch"] = {"platform": "ps2", "compiler": build_cfg["compiler"], "c_flags": build_cfg["cflags"]}
        config["units"].append(unit)
    (ROOT / "objdiff.json").write_text(json.dumps(config, indent=2) + "\n", encoding="utf-8", newline="\n")


def write_ninja(version: str, splat_yaml: Path, opts: dict, sha1: str, build_cfg: dict,
                units: dict[str, list[str]]) -> dict[str, str]:
    cfg_dir = f"config/{version}"
    build_dir = opts["build_path"]
    elf_out = opts["elf_path"]
    rom_out = str(Path(elf_out).with_suffix(".rom"))
    ld_script = opts["ld_script_path"]

    cflags = build_cfg["cflags"]
    gp_match = re.search(r"-G\s*(\d+)", cflags)
    cc_dir = f"tools/compilers/{build_cfg['compiler']}"

    extra_scripts = [f"{cfg_dir}/linker_script_extra.ld"] + [
        f"{cfg_dir}/{name}" for name in ("undefined_funcs_auto.txt", "undefined_syms_auto.txt")
        if (ROOT / cfg_dir / name).exists()
    ]

    with (ROOT / "build.ninja").open("w", encoding="utf-8", newline="\n") as f:
        n = ninja_syntax.Writer(f, width=120)
        n.comment(f"Generated by configure.py for {version} - do not edit.")
        n.variable("asflags", f"{AS_FLAGS} -G0")
        n.variable("cc_asflags", f"{AS_FLAGS} -G{gp_match.group(1) if gp_match else 0}")
        n.variable("cflags", cflags)
        n.variable("cc_dir", cc_dir)
        n.newline()

        # See alignment_flags().
        set_align = f"{CROSS}objcopy $align_flags $out"
        n.rule("as", f"{CROSS}as $asflags -o $out $in && {set_align}", description="AS $in")
        # Compile to assembly with the original compiler, post-process it with tools/ee_asm_compat.py and
        # assemble it with the original assembler when the package has one (SN Systems' Ps2EeAs: it inserts
        # the EE's hazard and erratum nops, guided by -g's line labels), otherwise with modern binutils.
        # Windows-only tools (SN Systems' ee-gcc 2.95.x) run under wibo.
        cc, snas = compiler_commands(build_cfg["compiler"])
        symbols = f"{cfg_dir}/elf_symbol_addrs.txt"
        compat_args = f" --source $in --symbols {symbols}" if (ROOT / symbols).exists() else ""
        if build_cfg.get("source_root"):
            compat_args += f" --source-root {build_cfg['source_root']}"
        if snas:
            assemble = f"{snas} --stdout -G{gp_match.group(1) if gp_match else 0} -o $out $out.s"
            compat_args += " --assembler snas"
        else:
            assemble = f"{CROSS}as $cc_asflags -o $out $out.s"
        n.rule(
            "cc",
            f"{cc} -S $cflags -I include -I src -o $out.gcc.s $in"
            f" && {sys.executable} tools/ee_asm_compat.py $out.gcc.s $out.s{compat_args}"
            f" && {assemble} && {set_align}",
            description="CC $in",
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

        objects = list(by_object)
        compiled: dict[str, tuple[str, str]] = {}
        for obj, entries in by_object.items():
            src = entries[0].src_paths[0].as_posix()
            suffix = entries[0].src_paths[0].suffix
            align = {"align_flags": object_fixups(entries, compiled=suffix in (".c", ".cpp", ".cc"))}
            if suffix in (".c", ".cpp", ".cc"):  # ee-gcc picks cc1 / cc1plus from the extension
                n.build(obj, "cc", src, variables=align)
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
        n.rule("merge", f"{CROSS}ld -EL -r -o $out $in", description="MERGE $out")
        exp = f"{build_dir}/expected"
        unit_objects = []
        for name, sources in units.items():
            parts = []
            for src in sources:
                part = f"{exp}/obj/{Path(src).relative_to(f'{exp}/asm').with_suffix('.o').as_posix()}"
                n.build(part, "as_expected", src)
                parts.append(part)
            unit_objects.append(f"{exp}/units/{name}.o")
            n.build(unit_objects[-1], "merge", parts)
        n.build("expected", "phony", unit_objects)
        n.newline()

        n.build(
            "build.ninja", "configure",
            implicit=["configure.py", splat_yaml.relative_to(ROOT).as_posix(), f"{cfg_dir}/build.yaml",
                      *(p.relative_to(ROOT).as_posix() for p in (ROOT / cfg_dir).glob("*_addrs.txt"))],
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

    # The all-asm split for objdiff's targets runs in parallel with the real one.
    exp_yaml, exp_asm = expected_config(splat_yaml, opts)
    shutil.rmtree(exp_asm, ignore_errors=True)
    exp_log = exp_yaml.with_suffix(".log")
    with exp_log.open("w") as log:
        exp_split = subprocess.Popen([sys.executable, "-m", "splat", "split", str(exp_yaml)], stdout=log, stderr=log)
        # No cache: a cached run writes incomplete undefined_*_auto.txt files and the link fails.
        split.main([splat_yaml], modes=["all"], verbose=False, use_cache=False)
        if exp_split.wait() != 0:
            sys.exit(f"error: the expected split failed, see {exp_log.relative_to(ROOT)}")

    units = expected_units(exp_asm)
    compiled = write_ninja(version, splat_yaml, opts, splat_cfg["sha1"], build_cfg, units)
    write_objdiff(opts, build_cfg, units, compiled)
    print(f"\nwrote build.ninja and objdiff.json ({len(units)} units, {len(compiled)} from source) for {version}"
          " - run `ninja`")


if __name__ == "__main__":
    main()
