#!/usr/bin/env python3
"""Decompilation progress: how many of each version's functions are matched C/C++, hand-written assembly,
still INCLUDE_ASM, not adapted to the version yet, or not started, as a Markdown table for the README.

Every version of config/versions.yaml that is configured gets its rows (a version without build/<VERSION>/expected/
is left out, with a note). A non-reference version (the retail ELF) builds only the units its build.yaml `sources:`
selects from src/ (docs/versions.md); a unit with a source that it doesn't select yet is "not adapted": its code is
still that version's own assembly. Its `.linkonce` units (weak functions, vtables) count with their unit.

A function counts as matched when its file is compiled from source and it isn't INCLUDE_ASM'd: the build only
passes when the rebuilt image is byte-identical (tools/check.sh), so every compiled function matches. Code
written in assembly originally (a `.s` source path, or a `src/<unit>.s`) is hand-written assembly. Files
without a source are "not started". INCLUDE_ASM inside `#if 0` doesn't count, the one in its `#else` does. The function list and sizes come from the original assembly, so the counts
don't depend on objdiff.

Needs a configured tree (configure.py) for the original assembly in build/<VERSION>/expected/.

Usage:
    python3 tools/progress.py                   # print the table (all configured versions)
    python3 tools/progress.py --version V       # one version
    python3 tools/progress.py --update README.md  # replace the part between the progress markers
"""

import argparse
import fnmatch
import re
import sys
from pathlib import Path

import yaml

sys.path.insert(0, str(Path(__file__).resolve().parent))
from versions import default_version, versions  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
START, END = "<!-- progress:start -->", "<!-- progress:end -->"
FUNC = re.compile(r"^nonmatching (\S+), 0x([0-9A-Fa-f]+)", re.M)
INCLUDE_ASM = re.compile(r"INCLUDE_ASM\([^,]+,\s*([^)\s]+)\s*\)")
GROUPS = (
    ("game", "Game code (`System/`, `Game/`, `Form/`, `crt0`)"),
    ("lib", "Libraries (`lib/`: Sony SDK, C library, libgcc)"),
)
STATES = ("matched", "handasm", "asm", "notyet", "todo")
VERSION_LABELS = {
    "SLUS_217.99": "US translation beta (the reference)",
    "SLUS_217.99_retail": "US retail",
}
CONDITIONAL = re.compile(r"^\s*#\s*(if|ifdef|ifndef|elif|else|endif)\b(.*)$")


def version_dir(version):
    return default_version(version)


def units(version):
    """[(unit, original source path)] of the .text segments, in address order."""
    text = (ROOT / "config" / version / f"{version}.yaml").read_text(encoding="utf-8").split("# end .text")[0]
    return re.findall(r"- \[0x[0-9A-F]+, asm, ([^\]]+)\](?: # (\S+))?", text)


def active_lines(text):
    """The lines of a source with the code under `#if 0` (and `#else` of `#if 1`) blanked, so line numbers stay.
    Other conditions count as true for every branch: only literal 0/1 conditions are known."""
    out = []
    stack = []  # per open #if: (enclosing active, this branch active, a branch was taken)
    active = True
    for line in text.split("\n"):
        m = CONDITIONAL.match(line)
        if m:
            kind, cond = m.group(1), m.group(2).split("//")[0].split("/*")[0].strip()
            if kind in ("if", "ifdef", "ifndef"):
                value = {"0": False, "1": True}.get(cond) if kind == "if" else None
                stack.append((active, value is not False, value is True))
                active = active and value is not False
            elif kind in ("elif", "else") and stack:
                outer, _, taken = stack[-1]
                value = {"0": False, "1": True}.get(cond) if kind == "elif" else True
                branch = not taken and value is not False
                stack[-1] = (outer, branch, taken or value is True)
                active = outer and branch
            elif kind == "endif" and stack:
                active = stack.pop()[0]
            out.append("")
            continue
        out.append(line if active else "")
    return out


def asm_functions(text):
    """Names INCLUDE_ASM'd in the active code of a source."""
    return set(INCLUDE_ASM.findall("\n".join(active_lines(text))))


def source_patterns(version):
    """The version's build.yaml `sources:` patterns (configure.py's source_selected), or None: every source."""
    path = ROOT / "config" / version / "build.yaml"
    cfg = yaml.safe_load(path.read_text(encoding="utf-8")) if path.exists() else {}
    patterns = (cfg or {}).get("sources")
    if isinstance(patterns, str):
        patterns = [l.strip() for l in (ROOT / patterns).read_text(encoding="utf-8").splitlines()
                    if l.strip() and not l.startswith("#")]
    return patterns


def selected(patterns, unit):
    if patterns is None:
        return True
    if any(p.startswith("!") and fnmatch.fnmatchcase(unit, p[1:]) for p in patterns):
        return False
    return any(fnmatch.fnmatchcase(unit, p) for p in patterns if not p.startswith("!"))


def source(unit):
    for ext in (".cpp", ".c", ".s"):
        path = ROOT / "src" / f"{unit}{ext}"
        if path.exists():
            return path
    return None


def collect(version):
    """Per group, state -> [functions, bytes]; None when the version isn't configured (no expected assembly)."""
    opts = yaml.safe_load((ROOT / "config" / version / f"{version}.yaml").read_text(encoding="utf-8"))["options"]
    expected = ROOT / opts["build_path"] / "expected" / "asm"
    if not expected.exists():
        return None
    patterns = source_patterns(version)
    # per group: state -> [functions, bytes]
    stats = {g: {s: [0, 0] for s in STATES} for g, _ in GROUPS}
    for unit, src_path in units(version):
        asm = expected / f"{unit}.s"
        if not asm.exists():
            continue
        funcs = [(name, int(size, 16)) for name, size in FUNC.findall(asm.read_text(encoding="utf-8"))]
        group = "lib" if unit.startswith("lib/") else "game"
        base = unit.removesuffix(".linkonce")
        src = source(base)
        if (src_path or "").endswith(".s") or (src is not None and src.suffix == ".s"):
            states = dict.fromkeys((name for name, _ in funcs), "handasm")
        elif src is None:
            states = dict.fromkeys((name for name, _ in funcs), "todo")
        elif not selected(patterns, base):
            states = dict.fromkeys((name for name, _ in funcs), "notyet")
        else:
            asm_names = asm_functions(src.read_text(encoding="latin-1"))
            states = {name: "asm" if name in asm_names else "matched" for name, _ in funcs}
        for name, size in funcs:
            state = states[name]
            stats[group][state][0] += 1
            stats[group][state][1] += size
    return stats


def pct(part, whole):
    return f"{100 * part / whole:.1f}%" if whole else "-"


def table(results):
    """results: [(version, stats)] -> the Markdown table, one block of rows per version."""
    rows = [
        "Functions per version by state (percentages: share of the row's code size).",
        "",
        "| Version | Code | Functions | Matched C/C++ | Hand-written assembly | INCLUDE_ASM (to resolve) "
        "| Not adapted to the version | Not started |",
        "|---|---|---:|---:|---:|---:|---:|---:|",
    ]
    for version, stats in results:
        name = f"`{version}`<br>{VERSION_LABELS[version]}" if version in VERSION_LABELS else f"`{version}`"
        totals = {s: [0, 0] for s in STATES}
        first = True
        for key, label in GROUPS:
            st = stats[key]
            n = sum(v[0] for v in st.values())
            size = sum(v[1] for v in st.values())
            if not n:
                continue
            for s in totals:
                totals[s][0] += st[s][0]
                totals[s][1] += st[s][1]
            cells = [f"{st[s][0]} ({pct(st[s][1], size)})" for s in STATES]
            rows.append(f"| {name if first else ''} | {label} | {n} | " + " | ".join(cells) + " |")
            first = False
        n = sum(v[0] for v in totals.values())
        size = sum(v[1] for v in totals.values())
        cells = [f"**{totals[s][0]}** ({pct(totals[s][1], size)})" for s in STATES]
        rows.append(f"| | **Total** | **{n}** | " + " | ".join(cells) + " |")
    rows += [
        "",
        "Matched: compiled from `src/` and byte-identical (the build checks each whole image). Hand-written assembly:",
        "written in assembly originally (no compiler output to match; stays `.s`). INCLUDE_ASM: the file is compiled",
        "from source but the function is still the original assembly. Not adapted: the source exists, but this version",
        "still builds the file from its own assembly (retail: `config/SLUS_217.99_retail/sources.txt`,",
        "`docs/versions.md`). Not started: no source file yet. Generated by `tools/progress.py`.",
    ]
    return "\n".join(rows)


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--version", help="one version (folder name under config/; default: every configured one)")
    parser.add_argument("--update", metavar="FILE", help=f"replace the text between {START} and {END} in FILE")
    args = parser.parse_args()
    results = []
    for version in ([version_dir(args.version)] if args.version else versions()):
        stats = collect(version)
        if stats is None:
            if args.version or version == versions()[0]:
                sys.exit(f"build/{version}/expected/asm is missing: run configure.py first")
            print(f"note: {version} is not configured (no build/{version}/expected/): left out", file=sys.stderr)
            continue
        results.append((version, stats))
    report = table(results)
    if not args.update:
        print(report)
        return
    path = Path(args.update)
    text = path.read_text(encoding="utf-8")
    if START not in text or END not in text:
        sys.exit(f"{path}: add the markers {START} and {END} where the table should go")
    head, rest = text.split(START, 1)
    _, tail = rest.split(END, 1)
    path.write_text(f"{head}{START}\n{report}\n{END}{tail}", encoding="utf-8")
    print(f"updated {path}")


if __name__ == "__main__":
    main()
