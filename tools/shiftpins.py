#!/usr/bin/env python3
"""What keeps a shifted link from resolving (docs/port-step1-shiftable.md, S1).

The matching link resolves address-named labels through files that pin them at their original address
(build/<VERSION>/original_labels.ld, config/<VERSION>/undefined_*_auto.txt). The shifted links of shift.yaml use
none of them. This lists every symbol the objects of a shifted link need that neither an object nor the shifted
link's own scripts define: per symbol the objects that use it and the pin file that resolves it in the matching
link ("-" if none does). Exit status 1 while anything is listed.

Usage (after configure.py, which writes the shifted link scripts):
    .venv/bin/python tools/shiftpins.py [--preset text128] [--by-object] [--all]
"""

import argparse
import re
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parent.parent
NM = "mips-linux-gnu-nm"
ASSIGNED = re.compile(r"(?<![\w$.])([A-Za-z_$.][\w$.]*)\s*=\s*[^=;]+;")


def pick_version() -> str:
    versions = [p.name for p in (ROOT / "config").iterdir() if (p / "build.yaml").exists()]
    if len(versions) != 1:
        sys.exit(f"error: {len(versions)} versions under config/")
    return versions[0]


def pin_files(version: str, build_path: str) -> dict[str, str]:
    """symbol -> the matching link's file that pins it."""
    pins = {}
    labels = ROOT / build_path / "original_labels.ld"
    for m in re.finditer(r'PROVIDE\("([^"]+)" = ', labels.read_text(encoding="utf-8")):
        pins[m.group(1)] = labels.name
    for name in ("undefined_funcs_auto.txt", "undefined_syms_auto.txt"):
        path = ROOT / "config" / version / name
        if path.exists():
            for m in re.finditer(r"^(\S+) = ", path.read_text(encoding="utf-8"), re.M):
                pins.setdefault(m.group(1), name)
    return pins


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--preset", help="shift.yaml preset (default: the first)")
    parser.add_argument("--by-object", action="store_true", help="group by the object that needs the symbols")
    parser.add_argument("--all", action="store_true", help="list every symbol (default: the first 40 per group)")
    args = parser.parse_args()

    version = pick_version()
    opts = yaml.safe_load((ROOT / "config" / version / f"{version}.yaml").read_text(encoding="utf-8"))["options"]
    presets = yaml.safe_load((ROOT / "config" / version / "shift.yaml").read_text(encoding="utf-8"))["presets"]
    preset = args.preset or next(iter(presets))
    script = ROOT / opts["build_path"] / "shift" / preset / "link.ld"
    if not script.exists():
        sys.exit(f"error: {script.relative_to(ROOT)} missing: run configure.py")
    text = script.read_text(encoding="utf-8")
    extra = (ROOT / "config" / version / "linker_script_extra.ld").read_text(encoding="utf-8")

    objects = sorted(set(re.findall(r"^\s*(\S+\.o)\(", text, re.M)))
    missing = [o for o in objects if not (ROOT / o).exists()]
    if missing:
        sys.exit(f"error: {len(missing)} objects not built (run ninja first), e.g. {missing[0]}")

    defined = {m.group(1) for m in ASSIGNED.finditer(text + extra)}
    users = defaultdict(set)
    for obj in objects:
        out = subprocess.run([NM, obj], cwd=ROOT, capture_output=True, text=True, check=True).stdout
        for line in out.splitlines():
            parts = line.split()
            if len(parts) == 2 and parts[0] in "Uw":
                if parts[0] == "U":
                    users[parts[1]].add(obj)
            elif len(parts) == 3 and (parts[1].isupper() or parts[1] in "vw"):
                defined.add(parts[2])  # global or weak; a local symbol of another object resolves nothing

    pins = pin_files(version, opts["build_path"])
    needed = {sym: objs for sym, objs in users.items() if sym not in defined}
    short = lambda o: re.sub(r"^build/[^/]+/(?:src/|asm/[^/]+/)", "", o)

    print(f"preset {preset}: {len(objects)} objects, {len(needed)} undefined symbols")
    if needed:
        print("by pin file: " + ", ".join(f"{k} {v}" for k, v in Counter(pins.get(s, "-") for s in needed).items()))
    limit = None if args.all else 40
    if args.by_object:
        per_object = defaultdict(list)
        for sym, objs in needed.items():
            for obj in objs:
                per_object[short(obj)].append(sym)
        for obj, syms in sorted(per_object.items(), key=lambda kv: (-len(kv[1]), kv[0])):
            print(f"\n{obj}: {len(syms)}")
            for sym in sorted(syms)[:limit]:
                print(f"    {sym}  [{pins.get(sym, '-')}]")
    else:
        for sym in sorted(needed)[:limit]:
            where = ", ".join(sorted(short(o) for o in needed[sym]))
            print(f"    {sym}  [{pins.get(sym, '-')}]  {where}")
        if limit and len(needed) > limit:
            print(f"    ... {len(needed) - limit} more (--all)")
    sys.exit(1 if needed else 0)


if __name__ == "__main__":
    main()
