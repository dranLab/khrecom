#!/usr/bin/env python3
"""List game files that are not decompiled yet, as work for (sub-)agents.

Per file: code size, functions (and how many are weak, i.e. inline functions emitted here), vtables, and the
headers whose inline functions left strings in its .rodata (assert/allocation messages). Such headers must be
reconstructed with their original line numbers before the file's .rodata can match, so files without them are
the easiest. Files touching the same headers should not be worked on in parallel.

Needs a configured tree (configure.py) for the original assembly in build/<VERSION>/expected/.

Usage:
    python3 tools/candidates.py SLUS_217.99                     # smallest first
    python3 tools/candidates.py SLUS_217.99 --no-headers --max-size 0x400 --limit 20
"""

import argparse
import re
from pathlib import Path

import yaml
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parent.parent
HEADER_STRING = re.compile(rb"c:/KHCOM/MainApp/([\w/]+\.h)(?:\(\d+\))?\x00")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("version")
    parser.add_argument("--max-size", type=lambda s: int(s, 0), default=None, help="largest code size")
    parser.add_argument("--no-headers", action="store_true", help="only files without header strings in .rodata")
    parser.add_argument("--limit", type=int, default=40)
    args = parser.parse_args()

    cfg_dir = ROOT / "config" / args.version
    text_yaml = (cfg_dir / f"{args.version}.yaml").read_text(encoding="utf-8")
    opts = yaml.safe_load(text_yaml)["options"]
    build_cfg = yaml.safe_load((cfg_dir / "build.yaml").read_text(encoding="utf-8"))
    image = (ROOT / opts["target_path"]).read_bytes()
    expected = ROOT / opts["build_path"] / "expected" / "asm"

    with (ROOT / build_cfg["elf"]).open("rb") as f:
        binding = {s.name: s["st_info"]["bind"] for s in ELFFile(f).get_section_by_name(".symtab").iter_symbols()
                   if s["st_info"]["type"] == "STT_FUNC"}

    code_block = text_yaml.split("# end .text")[0]
    code = [(int(a, 16), n) for a, n in re.findall(r"- \[0x([0-9A-F]+), asm, ([^\]]+)\]", code_block)]
    language = {n: Path(src).suffix.lstrip(".") or "?" for n, src in re.findall(r", asm, ([^\]]+)\] # (\S+)", code_block)}
    code_end = int(re.search(r"- \[0x([0-9A-F]+), \.?data, ", text_yaml).group(1), 16)
    rodata = sorted((int(a, 16), n) for a, n in re.findall(r"- \[0x([0-9A-F]+), \.?rodata, ([^\]]+)\]", text_yaml))
    rodata_range = {n: (a, b) for (a, n), (b, _) in zip(rodata, rodata[1:] + [(len(image), "")])}

    rows = []
    for (start, unit), (end, _) in zip(code, code[1:] + [(code_end, "")]):
        if unit.startswith("lib/") or (ROOT / "src" / f"{unit}.cpp").exists() or (ROOT / "src" / f"{unit}.c").exists():
            continue
        size = end - start
        if args.max_size is not None and size > args.max_size:
            continue
        asm = expected / f"{unit}.s"
        funcs = re.findall(r"^glabel (\S+)", asm.read_text(encoding="utf-8"), re.M) if asm.exists() else []
        weak = sum(1 for fn in funcs if binding.get(fn) == "STB_WEAK")
        headers, vtables = [], 0
        if unit in rodata_range:
            a, b = rodata_range[unit]
            headers = sorted({m.group(1).decode() for m in HEADER_STRING.finditer(image[a:b])})
            vtables = len(re.findall(r"^dlabel _vt\$", (expected / "data" / f"{unit}.rodata.s").read_text(encoding="utf-8"), re.M)) \
                if (expected / "data" / f"{unit}.rodata.s").exists() else 0
        if args.no_headers and headers:
            continue
        rows.append((size, unit, language.get(unit, "?"), len(funcs), weak, vtables, headers))

    rows.sort()
    print(f"{'size':>7} {'lang':>4} {'fns':>4} {'weak':>4} {'vt':>3}  file / header strings in .rodata")
    for size, unit, lang, nfuncs, weak, vtables, headers in rows[:args.limit]:
        print(f"{size:#7x} {lang:>4} {nfuncs:4} {weak:4} {vtables:3}  {unit}"
              + (f"   [{', '.join(headers)}]" if headers else ""))
    print(f"({len(rows)} files match, {min(len(rows), args.limit)} shown)")


if __name__ == "__main__":
    main()
