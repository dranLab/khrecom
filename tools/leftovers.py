#!/usr/bin/env python3
"""What is not decompiled yet, generated from the sources: per function still INCLUDE_ASM'd, whether a C draft
is parked under `#if 0`, whether it is hand-written assembly, or not attempted; plus the data sections still
taken from the original assembly and the units without a source file.

The state of an INCLUDE_ASM'd function:
- parked: a definition of it is in a disabled block (`#if 0`, or the `#else` of `#if 1`);
- hand-written: the note that applies to it starts with "Hand-written:" (no compiler output to match);
- not attempted: otherwise.
A note is a comment starting with "Parked:", "Hand-written:" or "Not attempted:". It applies to the functions
named after it (INCLUDE_ASM or a draft) up to the next compiled function definition or the next note. The
parked note goes above the draft's `#if 0` or above the INCLUDE_ASM line, e.g.

    /* Parked: the loop keeps the IPU_CMD address in a saved register (the original rebuilds it each pass) */
    #if 0
    int _sceMpegMbAddressIncrement(_sceMpegDecoder *d) { ... }
    #else
    INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/mpc", _sceMpegMbAddressIncrement);
    #endif

Library units are listed per function (with the notes), game units per unit (counts) followed by their parked
functions. Needs a configured tree (configure.py).

Usage:
    python3 tools/leftovers.py lib|game                 # print the list
    python3 tools/leftovers.py lib --update docs/sdk-leftovers.md   # replace the part between the markers
    python3 tools/leftovers.py game --unit Game/Player/PlayerSora   # one unit, every function left
"""

import argparse
import re
import sys
from pathlib import Path

import yaml

sys.path.insert(0, str(Path(__file__).resolve().parent))
from progress import CONDITIONAL, FUNC, INCLUDE_ASM, ROOT, active_lines, source, units, version_dir  # noqa: E402

NOTE = re.compile(r"^\s*(?:/\*+|//)\s*(Parked|Hand-written|Not attempted)\s*:\s*", re.I)
SOURCE_DATA = re.compile(r"^\s*(?://|#)\s*SOURCE DATA:\s*(.*?)\s*$", re.M)
# a function definition's head at column 0 (the name is the last identifier before the parameter list)
DEF_HEAD = re.compile(r"^[A-Za-z_][^;{}()#=]*?\b((?:\w+::)*~?\w+|(?:\w+::)*operator\s*\S+?)\s*\([^;]*$")
DATA_SECTIONS = (".data", ".rodata", ".bss", ".sdata", ".sbss")


def plain_name(mangled):
    """(class, function) of a GCC 2 mangled name, enough to recognize its definition; class '' if none."""
    m = re.match(r"^_\$_(.*)$", mangled)  # destructor _$_7CPlayer, _$_Q22PL7CPlayer
    if m:
        cls = mangled_class(m.group(1))
        return cls, "~" + cls
    m = re.match(r"^(.*?_*)__(C?[Q\d].*)$", mangled)
    if not m:
        return "", re.sub(r"__F.*$", "", mangled)
    cls = mangled_class(re.sub(r"^C(?=[Q\d])", "", m.group(2)))
    return cls, m.group(1) or cls  # constructor: __7CPlayeri


def mangled_class(text):
    """The innermost class name of a mangled class at the start of text (7CPlayer..., Q22PL7CPlayer...)."""
    count = 1
    m = re.match(r"^Q(\d)", text)
    if m:
        count, text = int(m.group(1)), text[2:]
    name = ""
    for _ in range(count):
        m = re.match(r"^(\d+)", text)
        if not m:
            return name
        n = int(m.group(1))
        name, text = text[m.end(): m.end() + n], text[m.end() + n:]
    return name


def defined_name(line):
    """(class, function) of the definition starting on this line, or None."""
    m = DEF_HEAD.match(line.rstrip())
    if not m or re.match(r"^(typedef|struct|union|enum|class|return|extern|static\s+const)\b", line):
        return None
    parts = re.sub(r"\s+", "", m.group(1)).split("::")
    return (parts[-2] if len(parts) > 1 else ""), parts[-1]


def comment_text(lines, i):
    """The comment starting at line i, as one line of text, and the index after it."""
    first = lines[i]
    if first.lstrip().startswith("//"):
        out = []
        while i < len(lines) and lines[i].lstrip().startswith("//"):
            out.append(lines[i].lstrip()[2:].strip())
            i += 1
        return " ".join(out), i
    out = []
    while i < len(lines):
        out.append(lines[i])
        i += 1
        if "*/" in out[-1]:
            break
    text = " ".join(s.strip() for s in out)
    text = re.sub(r"^/\*+\s*|\s*\*+/.*$", "", text)
    return re.sub(r"\s*\n?\s*\*\s+", " ", text).strip(), i


def scan(path, asm_names):
    """{function: (state, note)} for the INCLUDE_ASM'd functions of one source. A parked function without a note
    gets the comment next to its draft or INCLUDE_ASM instead (only blank lines, conditionals and INCLUDE_ASM
    lines in between), marked "(comment)"."""
    raw = path.read_text(encoding="latin-1").split("\n")
    active = active_lines("\n".join(raw))
    drafts = set()  # (class, function) defined in disabled code
    notes = {}  # function or (class, function) of a draft -> (kind, text) of the note before it
    near = {}  # the same -> the comment next to it
    note = comment = None
    i = 0
    while i < len(raw):
        line = raw[i]
        stripped = line.strip()
        if stripped.startswith(("/*", "//")):
            text, i = comment_text(raw, i)
            m = NOTE.match(line)
            if m:
                note = (m.group(1).lower(), re.sub(r"^[^:]*:\s*", "", text))
            else:
                comment = text
            continue
        name = defined_name(line)
        if name and active[i]:
            if line.rstrip().endswith("{") or (i + 1 < len(raw) and raw[i + 1].startswith("{")):
                note = None  # a compiled function ends the note's reach
        elif name and not active[i] and stripped:
            drafts.add(name)
            if note:
                notes.setdefault(name, note)
            if comment:
                near.setdefault(name, comment)
        for asm_name in INCLUDE_ASM.findall(active[i]):
            if note:
                notes.setdefault(asm_name, note)
            if comment:
                near.setdefault(asm_name, comment)
        if stripped and not (INCLUDE_ASM.search(line) or CONDITIONAL.match(line)):
            comment = None
        i += 1
    result = {}
    for asm_name in asm_names:
        cls, func = plain_name(asm_name)
        draft = (cls, func) if (cls, func) in drafts else ("", func) if not cls and ("", func) in drafts else None
        note = notes.get(asm_name) or notes.get(draft)
        if draft:
            text = note[1] if note and note[0] == "parked" else ""
            if not text and (near.get(draft) or near.get(asm_name)):
                text = "(comment) " + (near.get(draft) or near.get(asm_name))
            result[asm_name] = ("parked", text)
        elif note and note[0] == "hand-written":
            result[asm_name] = ("hand-written", note[1])
        else:
            result[asm_name] = ("not attempted", note[1] if note else "")
    return result


def collect(version):
    opts = yaml.safe_load((ROOT / "config" / version / f"{version}.yaml").read_text(encoding="utf-8"))["options"]
    expected = ROOT / opts["build_path"] / "expected" / "asm"
    if not expected.exists():
        sys.exit(f"{expected} is missing: run configure.py first")
    data = {}  # unit -> sections in the original
    for path in (expected / "data").rglob("*.s"):
        m = re.match(r"(.*)(\.(?:data|rodata|bss|sdata|sbss))\.s$", path.relative_to(expected / "data").as_posix())
        if m:
            data.setdefault(m.group(1), set()).add(m.group(2))
    code = {}
    for unit, src_path in units(version):
        asm = expected / f"{unit}.s"
        if asm.exists():
            code[unit] = ([(n, int(s, 16) // 4) for n, s in FUNC.findall(asm.read_text(encoding="utf-8"))],
                          src_path or "")
    rows = []  # (unit, function or None, instructions, state, note)
    nosource = []  # (unit, instructions or 0, sections)
    data_asm = []  # (unit, sections)
    for unit in [*code, *sorted(set(data) - set(code))]:
        funcs, src_path = code.get(unit, ([], ""))
        src = source(unit)
        if src_path.endswith(".s") or (src is not None and src.suffix == ".s"):
            continue  # hand-written assembly source
        if src is None:
            nosource.append((unit, funcs, sorted(data.get(unit, ()))))
            continue
        text = src.read_text(encoding="latin-1")
        listed = SOURCE_DATA.findall(text)
        from_src = set(listed[-1].split()) if listed else set()
        missing = sorted(data.get(unit, set()) - from_src)
        if missing:
            data_asm.append((unit, missing))
        asm_names = set(INCLUDE_ASM.findall("\n".join(active_lines(text))))
        states = scan(src, asm_names)
        for name, size in funcs:
            if name in states:
                rows.append((unit, name, size, *states[name]))
    return rows, nosource, data_asm


def md(text, limit=300):
    if len(text) > limit:
        text = text[: limit - 1].rsplit(" ", 1)[0] + " …"
    return text.replace("|", "\\|")


def lib_report(rows, nosource, data_asm):
    rows = [r for r in rows if r[0].startswith("lib/")]
    nosource = [r for r in nosource if r[0].startswith("lib/")]
    data_asm = [r for r in data_asm if r[0].startswith("lib/")]
    out = ["### Units with no source file", "",
           "Their code or data comes entirely from the original assembly.", "",
           "| unit | functions (instr) | data |", "|---|---|---|"]
    for unit, funcs, sections in nosource:
        f = ", ".join(f"`{n}` ({s})" for n, s in funcs) or "-"
        out.append(f"| `{unit}` | {f} | {' '.join(sections) or '-'} |")
    out += ["", "### Functions still INCLUDE_ASM", "",
            "Sorted by unit, then size. `instr`: instructions in the original.", "",
            "| unit | function | instr | status | what differs |", "|---|---|---:|---|---|"]
    order = {"parked": 0, "not attempted": 1, "hand-written": 2}
    by_unit = {}
    for r in rows:
        by_unit.setdefault(r[0], []).append(r)
    for unit in by_unit:
        for _, name, size, state, note in sorted(by_unit[unit], key=lambda r: (order[r[3]], -r[2])):
            out.append(f"| `{unit}` | `{name}` | {size} | {state} | {md(note)} |")
    out += ["", "### Data sections from the original assembly", "",
            "Not listed in the unit's `// SOURCE DATA:` line.", "", "| unit | sections |", "|---|---|"]
    out += [f"| `{u}` | {' '.join(s)} |" for u, s in data_asm]
    counts = {s: sum(1 for r in rows if r[3] == s) for s in order}
    out += ["", f"Totals: {len(nosource)} units without a source file "
            f"({sum(1 for r in nosource if r[1])} with code); {len(by_unit)} units with INCLUDE_ASM functions: "
            f"{counts['parked']} parked, {counts['not attempted']} not attempted, {counts['hand-written']} "
            f"hand-written; {len(data_asm)} units take a data section from assembly.",
            "", "Generated by `tools/leftovers.py lib`."]
    return "\n".join(out)


def game_report(rows, nosource, data_asm):
    rows = [r for r in rows if not r[0].startswith("lib/")]
    data = {u: s for u, s in data_asm if not u.startswith("lib/")}
    nosource = [r for r in nosource if not r[0].startswith("lib/")]
    by_unit = {}
    for r in rows:
        by_unit.setdefault(r[0], []).append(r)
    total = sum(r[2] for r in rows)
    out = ["### Units with INCLUDE_ASM functions", "",
           "Sorted by the instructions left. `parked`: functions with a C draft under `#if 0`.", "",
           "| unit | functions left | instr left | parked | data from asm |", "|---|---:|---:|---:|---|"]
    for unit, rs in sorted(by_unit.items(), key=lambda kv: -sum(r[2] for r in kv[1])):
        parked = sum(1 for r in rs if r[3] == "parked")
        out.append(f"| `{unit}` | {len(rs)} | {sum(r[2] for r in rs)} | {parked or ''} | "
                   f"{' '.join(data.get(unit, ())) or ''} |")
    only_data = sorted(set(data) - set(by_unit))
    if only_data:
        out += ["", "Units with all functions in C but data from asm: "
                + ", ".join(f"`{u}` ({' '.join(data[u])})" for u in only_data) + "."]
    if nosource:
        out += ["", "Units without a source file: " + ", ".join(f"`{u}`" for u, _, _ in nosource) + "."]
    parked = [r for r in rows if r[3] == "parked"]
    out += ["", "### Parked functions", "",
            "INCLUDE_ASM'd functions with a C draft in a disabled block. Note: the source's `Parked:` note, or else",
            "the comment next to the draft, marked (comment); shortened to 300 characters.", "",
            "| unit | function | instr | note |", "|---|---|---:|---|"]
    out += [f"| `{u}` | `{n}` | {s} | {md(note)} |" for u, n, s, _, note in parked]
    out += ["", f"Totals: {len(rows)} functions ({total} instructions) in {len(by_unit)} units, "
            f"{len(parked)} of them parked; {len(data)} units take a data section from assembly.",
            "", "Generated by `tools/leftovers.py game`."]
    return "\n".join(out)


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("part", choices=("lib", "game"))
    parser.add_argument("--version", help="folder name under config/ (default: the only one)")
    parser.add_argument("--update", metavar="FILE", help="replace the text between the part's markers in FILE")
    parser.add_argument("--unit", help="only this unit: every INCLUDE_ASM'd function (instructions, state, note)")
    args = parser.parse_args()
    collected = collect(version_dir(args.version))
    if args.unit:
        rows = [r for r in collected[0] if r[0] == args.unit]
        for _, name, size, state, note in rows:
            print(f"{state:13} {name} ({size} instr)" + (f": {note}" if note else ""))
        data = [s for u, s in collected[2] if u == args.unit]
        print(f"{len(rows)} functions still INCLUDE_ASM; data from asm: {' '.join(data[0]) if data else 'none'}")
        return
    report = (lib_report if args.part == "lib" else game_report)(*collected)
    if not args.update:
        print(report)
        return
    start, end = f"<!-- leftovers:{args.part}:start -->", f"<!-- leftovers:{args.part}:end -->"
    path = Path(args.update)
    text = path.read_text(encoding="utf-8")
    if start not in text or end not in text:
        sys.exit(f"{path}: add the markers {start} and {end} where the list should go")
    head, rest = text.split(start, 1)
    _, tail = rest.split(end, 1)
    path.write_text(f"{head}{start}\n{report}\n{end}{tail}", encoding="utf-8")
    print(f"updated {path}")


if __name__ == "__main__":
    main()
