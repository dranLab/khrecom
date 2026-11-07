#!/usr/bin/env python3
"""Search source variants of one function for the original's code (a decomp permuter).

Takes a function that is already close (same structure, wrong registers or scheduling), applies random source
mutations to it, compiles every candidate with the unit's own build command and scores it against the original
function. Improvements are kept (hill climbing; equal scores are accepted too, so it can walk plateaus); the
best variant so far is written to build/permuter/<function>/best.c (the whole source file) and the diff of the
function to best.diff. The real source file is never modified.

A parked function (its draft under #if 0, the INCLUDE_ASM in the #else) is switched on in the copies; the real
source file is never modified.

Usage:
    python3 tools/permuter.py lib/pack sceMpegDemuxPssRing
    python3 tools/permuter.py lib/pack sceMpegDemuxPssRing -j 16 --minutes 30
    python3 tools/permuter.py lib/mpc _getRef0 --unsafe          # also mutations that can change behaviour
    python3 tools/permuter.py System/Rcf _atolen                 # C++: the source name, if unique in the unit,
    python3 tools/permuter.py Game/Collision/CollDisp setBodyLines__9CCollDispP14CCollisionList   # or the symbol

Mutations (all keep the program's meaning unless --unsafe):
    swap two adjacent declarations              swap two adjacent independent statements
    commute + * & | ^ == != operands            a = a OP b <-> a OP= b,  x++ <-> ++x <-> x += 1
    a < b <-> b > a (and <=, >=)                put a statement in its own { } block
    evaluate a subexpression into a temporary   (--unsafe) move a statement past a dependent one
The score is the number of differing instruction lines (relocations included, branch offsets ignored), plus 4 per
instruction of size difference; 0 is a match. A candidate that does not compile is skipped. Read the result
before adopting it: statement moves are only checked with a simple identifier test.
"""

import argparse
import difflib
import multiprocessing as mp
import os
import random
import re
import shutil
import subprocess
import sys
import tempfile
import time
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from compare_compilers import functions  # noqa: E402
from funcdiff import disassemble  # noqa: E402

KEYWORDS = {"if", "else", "for", "while", "do", "switch", "case", "default", "return", "break", "continue", "goto",
            "sizeof", "int", "unsigned", "signed", "char", "short", "long", "float", "double", "void", "struct",
            "union", "const", "volatile", "static", "register", "inline", "typedef", "enum", "extern"}
IDENT = re.compile(r"\b[A-Za-z_]\w*\b")


# ---------------------------------------------------------------------------------------------------------------
# names: the symbol (GCC 2 mangled for C++) and what the source spells

def demangle(symbol: str) -> tuple[str, str | None]:
    """(name in the source, class or None) of a GCC 2 symbol: `getAttackCardNum__C10CHumanBossUi` ->
    ("getAttackCardNum", "CHumanBoss"), `main__Q25Gauge8CNaviMap` -> ("main", "CNaviMap"), `__ct__5CTaskFv` ->
    ("CTask", "CTask"), `_$_5CTask` -> ("~CTask", "CTask"). A C name comes back unchanged with no class."""
    if symbol.startswith(("_$_", "_._")):
        cls = _mangled_class(symbol[3:])
        return ("~" + cls, cls) if cls else (symbol, None)
    i = symbol.find("__", 1)
    if i <= 0:
        return symbol, None
    base, rest = symbol[:i], symbol[i + 2:]
    if base == "__ct" or symbol.startswith("__ct__"):
        cls = _mangled_class(symbol[6:])
        return (cls, cls) if cls else (symbol, None)
    cls = _mangled_class(rest[1:] if rest.startswith("C") else rest)
    return base, cls


def _mangled_class(s: str) -> str | None:
    """The (innermost) class at the start of a mangled qualifier: `10CHumanBossUi` -> CHumanBoss,
    `Q25Gauge8CNaviMap` -> CNaviMap; None for a plain function's argument list (`FPCc`)."""
    m = re.match(r"^Q(\d)", s)
    if m:
        s, n, name = s[2:], int(m.group(1)), None
        for _ in range(n):
            m = re.match(r"^(\d+)", s)
            if not m:
                return None
            k = int(m.group(1))
            name, s = s[len(m.group(1)):len(m.group(1)) + k], s[len(m.group(1)) + k:]
        return name
    m = re.match(r"^(\d+)", s)
    if not m:
        return None
    k = int(m.group(1))
    return s[len(m.group(1)):len(m.group(1)) + k] or None


def resolve(arg: str, symbols) -> tuple[str, str, str | None]:
    """(symbol, source name, class) for the function named on the command line: a symbol of the unit as it stands,
    or a plain source name that exactly one symbol of the unit demangles to."""
    if arg in symbols:
        name, cls = demangle(arg)
        return arg, name, cls
    hits = [s for s in symbols if demangle(s)[0] == arg or (demangle(s)[1] and
                                                             f"{demangle(s)[1]}::{demangle(s)[0]}" == arg)]
    if len(hits) == 1:
        name, cls = demangle(hits[0])
        return hits[0], name, cls
    if hits:
        sys.exit(f"{arg} is ambiguous in this unit: " + ", ".join(sorted(hits)) + " (pass the symbol)")
    sys.exit(f"{arg}: no such function in the unit's original object")


def definition_pattern(name: str, cls: str | None) -> str:
    """Regex for the start of the function's definition line, up to and including its opening brace."""
    q = r"(?:\b" + re.escape(cls) + r"\s*::\s*)" if cls else ""
    n = r"\b" + re.escape(name) if not name.startswith("~") else r"~\s*" + re.escape(name[1:])
    # an optional return type and qualifiers (none for a constructor or destructor), then the name
    return r"^(?:[^;{}#]*?[\s*&])?" + q + n + r"\s*\([^;{}]*\)\s*(?:const\s*)?\{"


# ---------------------------------------------------------------------------------------------------------------
# locating the function in the source

def find_function(text: str, pattern: str) -> tuple[int, int]:
    """(start, end) of the function's body braces, end exclusive (just after the closing brace). pattern:
    definition_pattern() of the function."""
    for m in re.finditer(pattern, text, re.M):
        start = m.end() - 1
        depth = 0
        i = start
        while i < len(text):
            c = text[i]
            if c == "{":
                depth += 1
            elif c == "}":
                depth -= 1
                if depth == 0:
                    return start, i + 1
            elif c == '"':
                i = text.index('"', i + 1)
            elif text.startswith("/*", i):
                i = text.index("*/", i) + 1
            elif text.startswith("//", i):
                i = text.index("\n", i)
            i += 1
    raise SystemExit(f"function not found (as a definition) in the source: {pattern}")


def enable_draft(text: str, pattern: str, symbol: str) -> str:
    """Switch a parked draft on: the #if 0 block around the function's definition becomes #if 1, and the
    function's INCLUDE_ASM (by its symbol: in the #else, after the #endif, or further away past a placeholder) goes
    away, with those of the other functions defined in the same block."""
    d = re.search(pattern, text, re.M)
    if not d:
        return text
    depth, opened = 0, None
    for m in re.finditer(r"^\s*#\s*(if|ifdef|ifndef|endif)\b(.*)$", text[:d.start()], re.M):
        if m.group(1).startswith("if"):
            depth += 1
            if m.group(1) == "if" and m.group(2).strip() == "0":
                opened = (m.start(), m.end(), depth)
        else:
            if opened and depth == opened[2]:
                opened = None
            depth -= 1
    names = {symbol}
    if opened:
        text = text[:opened[0]] + re.sub(r"#\s*if\s+0", "#if 1", text[opened[0]:opened[1]]) + text[opened[1]:]
        depth, end = 0, len(text)  # the block's end: its #else or #endif
        for m in re.finditer(r"^\s*#\s*(if|ifdef|ifndef|else|elif|endif)\b", text[opened[0]:], re.M):
            if m.group(1).startswith("if"):
                depth += 1
            elif depth == 1:
                end = opened[0] + m.start()
                break
            elif m.group(1) == "endif":
                depth -= 1
        names |= {m.group(1) for m in re.finditer(r"^[A-Za-z_][^;{}]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{",
                                                   text[opened[0]:end], re.M)}
    for n in names:
        text = re.sub(r"^\s*INCLUDE_ASM\([^)]*,\s*" + re.escape(n) + r"\s*\);\s*$", "", text, flags=re.M)
    return text


# ---------------------------------------------------------------------------------------------------------------
# mutations: each takes the body's text and a random generator, returns new text or None

def lines_of(body: str) -> list[str]:
    return body.split("\n")


def is_simple_statement(line: str) -> bool:
    s = line.strip()
    if not s.endswith(";") or s.count(";") != 1 or "{" in s or "}" in s:
        return False
    first = IDENT.match(s)
    return not (first and first.group(0) in {"return", "break", "continue", "goto", "case", "default"})


def is_declaration(line: str) -> bool:
    s = line.strip()
    return bool(re.match(r"^(?:(?:const|volatile|unsigned|signed|register|static|struct|union)\s+)*"
                         r"[A-Za-z_]\w*(?:\s*\*)*\s+\**\s*[A-Za-z_]\w*\s*(?:\[[^\]]*\])*\s*(?:=[^;]*)?;$", s)) \
        and IDENT.match(s).group(0) not in {"return", "goto"}


LOCALS: set[str] = set()  # the function's parameters and locals (main() fills it in)


def accesses(stmt: str) -> tuple[set[str], set[str], set[str], set[str], bool]:
    """(locals written, locals read, memory written, memory read, calls). Memory is a non-local name (a global) or
    "*" for anything through a pointer, an array or a member."""
    s = stmt.strip().rstrip(";")
    m = re.match(r"^(.*?)(?<![=!<>])([-+*/%&|^]|<<|>>)?=(?!=)(.*)$", s)
    lhs, op, rhs = (m.group(1), m.group(2), m.group(3)) if m else ("", None, s)
    calls = bool(re.search(r"\b[A-Za-z_]\w*\s*\(", s)) or "asm" in s
    lw, lr, mw, mr = set(), set(), set(), set()

    def names(text: str) -> list[str]:  # variables, not called functions, keywords or macro constants
        return [w.group(0) for w in IDENT.finditer(text) if w.group(0) not in KEYWORDS
                and not re.match(r"\s*\(", text[w.end():]) and not w.group(0).isupper()]
    if lhs:
        if re.search(r"[\[*]|->|\.", lhs):
            mw.add("*")
            for n in names(lhs):  # the index or base pointer is read
                (lr if n in LOCALS else mr).add(n)
        else:
            base = names(lhs)
            if base:
                (lw if base[-1] in LOCALS else mw).add(base[-1])
    for a, b in re.findall(r"(\b[A-Za-z_]\w*)\s*(?:\+\+|--)|(?:\+\+|--)\s*([A-Za-z_]\w*)", s):
        n = a or b
        (lw if n in LOCALS else mw).add(n)
    for n in names(rhs):
        (lr if n in LOCALS else mr).add(n)
    if re.search(r"[\[*]|->", rhs):
        mr.add("*")
    if op:
        lr |= lw
        mr |= mw
    return lw, lr, mw, mr, calls


def independent(a: str, b: str) -> bool:
    if "asm" in a or "asm" in b or "volatile" in a or "volatile" in b:
        return False
    lwa, lra, mwa, mra, ca = accesses(a)
    lwb, lrb, mwb, mrb, cb = accesses(b)
    if lwa & (lrb | lwb) or lwb & lra:
        return False

    def clash(x: set[str], y: set[str]) -> bool:
        return bool(x & y) or ("*" in x and bool(y)) or ("*" in y and bool(x))
    if clash(mwa, mwb | mrb) or clash(mwb, mra):
        return False
    if (ca and (cb or mwb or mrb)) or (cb and (mwa or mra)):
        return False
    return True


def m_swap_adjacent(body: str, rnd: random.Random, unsafe: bool) -> str | None:
    lines = lines_of(body)
    idx = [i for i in range(len(lines) - 1)
           if lines[i].strip() and lines[i + 1].strip()
           and indent(lines[i]) == indent(lines[i + 1])
           and ((is_declaration(lines[i]) and is_declaration(lines[i + 1]))
                or (is_simple_statement(lines[i]) and is_simple_statement(lines[i + 1])
                    and not is_declaration(lines[i]) and not is_declaration(lines[i + 1])
                    and (unsafe or independent(lines[i], lines[i + 1]))))]
    if not idx:
        return None
    i = rnd.choice(idx)
    lines[i], lines[i + 1] = lines[i + 1], lines[i]
    return "\n".join(lines)


def indent(line: str) -> int:
    return len(line) - len(line.lstrip())


OPERAND = r"(?:[A-Za-z_]\w*(?:(?:->|\.)[A-Za-z_]\w*|\[[^\[\]]*\])*|\d+|0x[0-9A-Fa-f]+)"


def delimited(body: str, start: int, end: int) -> bool:
    """Is body[start:end] a whole operand: between ( , = ; ? : [ && || return and ) , ; ? : ] && ||?"""
    before = body[:start].rstrip()
    after = body[end:].lstrip()
    ok_before = (before.endswith(("(", ",", ";", "?", ":", "[", "&&", "||", "{")) or re.search(r"\breturn$", before)
                 or (before.endswith("=") and not before.endswith(("==", "!=", "<=", ">="))))
    ok_after = after.startswith((")", ",", ";", "?", ":", "]", "&&", "||"))
    return bool(ok_before and ok_after)


def m_commute(body: str, rnd: random.Random, unsafe: bool) -> str | None:
    pat = re.compile(r"(?<![\w\]\)>.])(" + OPERAND + r")\s*(\+|\*|&(?!&)|\|(?!\|)|\^|==|!=)\s*(" + OPERAND
                     + r")(?![\w\[(.]|->)")
    found = [m for m in pat.finditer(body) if delimited(body, m.start(), m.end())]
    if not found:
        return None
    m = rnd.choice(found)
    return body[:m.start()] + f"{m.group(3)} {m.group(2)} {m.group(1)}" + body[m.end():]


def m_compound(body: str, rnd: random.Random, unsafe: bool) -> str | None:
    """a = a OP b <-> a OP= b; x++ <-> ++x <-> x += 1 (as statements)."""
    lines = lines_of(body)
    cands = []
    for i, line in enumerate(lines):
        s = line.strip()
        m = re.match(r"^([A-Za-z_][\w\[\]\->.]*)\s*=\s*\1\s*([-+*&|^]|<<|>>)\s*(.+);$", s)
        if m:
            cands.append((i, f"{m.group(1)} {m.group(2)}= {m.group(3)};"))
        m = re.match(r"^([A-Za-z_][\w\[\]\->.]*)\s*([-+*&|^]|<<|>>)=\s*(.+);$", s)
        if m and m.group(2) in "+*&|^" and not re.search(r"[-+*/%&|^<>?:]", m.group(3)):
            cands.append((i, f"{m.group(1)} = {m.group(1)} {m.group(2)} {m.group(3)};"))
        m = re.match(r"^([A-Za-z_]\w*)(\+\+|--);$", s)
        if m:
            cands.append((i, f"{m.group(2)}{m.group(1)};"))
            cands.append((i, f"{m.group(1)} {m.group(2)[0]}= 1;"))
        m = re.match(r"^(\+\+|--)([A-Za-z_]\w*);$", s)
        if m:
            cands.append((i, f"{m.group(2)}{m.group(1)};"))
    if not cands:
        return None
    i, new = rnd.choice(cands)
    lines[i] = " " * indent(lines[i]) + new
    return "\n".join(lines)


def m_flip_compare(body: str, rnd: random.Random, unsafe: bool) -> str | None:
    pat = re.compile(r"(?<![\w\])>.])(" + OPERAND + r")\s*(<=|>=|<(?![<=])|>(?![>=]))\s*(" + OPERAND
                     + r")(?![\w\[(.]|->)")
    found = [m for m in pat.finditer(body) if delimited(body, m.start(), m.end())]
    if not found:
        return None
    m = rnd.choice(found)
    flip = {"<": ">", ">": "<", "<=": ">=", ">=": "<="}[m.group(2)]
    return body[:m.start()] + f"{m.group(3)} {flip} {m.group(1)}" + body[m.end():]


def m_block(body: str, rnd: random.Random, unsafe: bool) -> str | None:
    lines = lines_of(body)
    idx = [i for i, line in enumerate(lines) if is_simple_statement(line) and not is_declaration(line)]
    if not idx:
        return None
    i = rnd.choice(idx)
    lines[i] = " " * indent(lines[i]) + "{ " + lines[i].strip() + " }"
    return "\n".join(lines)


def m_unblock(body: str, rnd: random.Random, unsafe: bool) -> str | None:
    lines = lines_of(body)
    idx = [i for i, line in enumerate(lines) if re.match(r"^\s*\{ [^{}]* \}$", line)]
    if not idx:
        return None
    i = rnd.choice(idx)
    lines[i] = " " * indent(lines[i]) + lines[i].strip()[2:-2]
    return "\n".join(lines)


def m_move_far(body: str, rnd: random.Random, unsafe: bool) -> str | None:
    """Move a statement up or down by several independent statements at the same level."""
    lines = lines_of(body)
    idx = [i for i, line in enumerate(lines) if is_simple_statement(line) and not is_declaration(line)]
    if not idx:
        return None
    i = rnd.choice(idx)
    step = rnd.choice((-1, 1))
    j = i
    while True:
        k = j + step
        if not (0 <= k < len(lines)) or indent(lines[k]) != indent(lines[i]) or not is_simple_statement(lines[k]) \
                or is_declaration(lines[k]) or not (unsafe or independent(lines[i], lines[k])):
            break
        j = k
        if rnd.random() < 0.5:
            break
    if j == i:
        return None
    line = lines.pop(i)
    lines.insert(j, line)
    return "\n".join(lines)


def m_temp(body: str, rnd: random.Random, unsafe: bool) -> str | None:
    """Evaluate a subexpression of a statement into a temporary first: { __typeof__(e) t = e; ...t...; }."""
    lines = lines_of(body)
    idx = [i for i, line in enumerate(lines) if is_simple_statement(line) and not is_declaration(line)]
    rnd.shuffle(idx)
    for i in idx:
        s = lines[i].strip()
        m = re.match(r"^(.*?[^=!<>])=(?!=)(.*);$", s)
        rhs_at = m.start(2) if m else 0
        exprs = [e for e in re.finditer(r"(?:" + OPERAND + r"|\([^()]*\))", s[rhs_at:])
                 if ("->" in e.group(0) or "[" in e.group(0) or e.group(0).startswith("("))
                 and "(" not in s[rhs_at + e.end():rhs_at + e.end() + 1]
                 and not re.search(r"\+\+|--|=", e.group(0))]
        if not exprs:
            continue
        e = rnd.choice(exprs)
        a, b = rhs_at + e.start(), rhs_at + e.end()
        if s[a - 1:a] in (".",) or s[a - 2:a] == "->" or s[a - 1:a] == "&":
            continue
        n = f"pt{rnd.randrange(1000)}"
        lines[i] = (" " * indent(lines[i]) + "{ __typeof__(" + e.group(0) + ") " + n + " = " + e.group(0) + "; "
                    + s[:a] + n + s[b:] + " }")
        return "\n".join(lines)
    return None


MUTATIONS = [(m_swap_adjacent, 4), (m_commute, 3), (m_compound, 1), (m_flip_compare, 1), (m_block, 1),
             (m_unblock, 1), (m_move_far, 2), (m_temp, 2)]


def mutate(body: str, rnd: random.Random, unsafe: bool) -> str:
    funcs, weights = zip(*MUTATIONS)
    for _ in range(20):
        new = rnd.choices(funcs, weights)[0](body, rnd, unsafe)
        if new and new != body:
            return new
    return body


# ---------------------------------------------------------------------------------------------------------------
# compiling and scoring

def build_command(obj: str) -> str:
    out = subprocess.run(["ninja", "-t", "commands", obj], cwd=ROOT, capture_output=True, text=True, check=True)
    return out.stdout.strip().splitlines()[-1]


class Scorer:
    def __init__(self, unit: str, func: str, source: Path, command: str, expected: Path, workdir: Path):
        """source: the unit's source file as the build command names it (the text compiled comes with score())."""
        self.unit, self.func, self.source, self.workdir = unit, func, source, workdir
        self.obj = f"{self.build}/src/{unit}.o"
        self.command = command
        self.expected = expected
        self.target = functions(expected)[func]
        self.target_lines = normalize(disassemble(expected, func, len(self.target.data)))

    build = ""

    def score(self, text: str, slot: str) -> tuple[int, str]:
        d = self.workdir / slot
        d.mkdir(parents=True, exist_ok=True)
        src = d / self.source.name
        src.write_text(text)
        obj = d / "out.o"
        cmd = self.command.replace(self.obj, str(obj))
        cmd = re.sub(r"(?<=\s)" + re.escape(str(self.source.relative_to(ROOT))) + r"(?=\s|$)", str(src), cmd)
        # quoted includes of the original directory still resolve (the copy lives elsewhere)
        cmd = cmd.replace(" -I src", f" -I {self.source.parent.relative_to(ROOT)} -I src", 1)
        # keep the data sections: a draft's string literals in a unit whose .rodata still comes from the original
        # would otherwise leave relocations to a stripped section (only the function's code is compared)
        cmd = re.sub(r" -R \S+", "", cmd)
        env = dict(os.environ, TMPDIR=str(d))
        r = subprocess.run(cmd, shell=True, cwd=ROOT, capture_output=True, text=True, env=env)
        if r.returncode != 0 or not obj.exists():
            return 1 << 30, (r.stdout + r.stderr)[-2000:]
        ours = functions(obj).get(self.func)
        if ours is None:
            return 1 << 30, "function missing"
        if self.target.difference(ours) is None:
            return 0, ""
        lines = normalize(disassemble(obj, self.func, len(ours.data)))
        sm = difflib.SequenceMatcher(None, self.target_lines, lines, autojunk=False)
        diff = sum(max(i2 - i1, j2 - j1) for t, i1, i2, j1, j2 in sm.get_opcodes() if t != "equal")
        size = abs(len(ours.data) - len(self.target.data)) // 4
        return max(1, diff + 4 * size), ""


def normalize(lines: list[str]) -> list[str]:
    out = []
    for line in lines:
        line = re.sub(r"<\+0x[0-9a-f]+>", "<+?>", line.strip())
        line = re.sub(r"^(R_MIPS_\w+)\s+\S+", r"\1", line)  # the original names data after splat's labels
        out.append(line)
    return out


# ---------------------------------------------------------------------------------------------------------------
# search

_scorer: Scorer | None = None


def _init(scorer: Scorer) -> None:
    global _scorer
    _scorer = scorer


def _evaluate(job: tuple[str, str]) -> tuple[int, str, str]:
    text, slot = job
    s, err = _scorer.score(text, slot)
    return s, text, err


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("unit", help="e.g. lib/pack")
    parser.add_argument("function")
    parser.add_argument("--source", help="start from this file's text instead of src/<unit>.c (e.g. an earlier best.c)")
    parser.add_argument("-j", "--jobs", type=int, default=max(1, (os.cpu_count() or 2) - 2))
    parser.add_argument("--minutes", type=float, default=20, help="stop after this long (default 20)")
    parser.add_argument("--seed", type=int, default=None)
    parser.add_argument("--unsafe", action="store_true", help="also reorder statements that look dependent")
    parser.add_argument("--depth", type=int, default=3, help="up to this many mutations per candidate (default 3)")
    args = parser.parse_args()

    version = sorted(p.name for p in (ROOT / "config").iterdir() if (p / f"{p.name}.yaml").exists())[0]
    opts = yaml.safe_load((ROOT / "config" / version / f"{version}.yaml").read_text())["options"]
    build = opts["build_path"]
    source = next((ROOT / "src" / f"{args.unit}{e}" for e in (".c", ".cpp")
                   if (ROOT / "src" / f"{args.unit}{e}").exists()))
    start_text = Path(args.source).read_text() if args.source else source.read_text()
    expected = ROOT / build / "expected" / "units" / f"{args.unit}.o"
    subprocess.run(["ninja", str(expected.relative_to(ROOT))], cwd=ROOT, check=True, capture_output=True)

    symbol, name, cls = resolve(args.function, functions(expected))
    pattern = definition_pattern(name, cls)
    if symbol != args.function or cls:
        print(f"{args.function}: symbol {symbol}, defined as {cls + '::' if cls else ''}{name}", flush=True)
    text = enable_draft(start_text, pattern, symbol)
    start, end = find_function(text, pattern)
    head, body, tail = text[:start], text[start:end], text[end:]
    header = head[head.rfind(name):]
    LOCALS.update(w for w in IDENT.findall(header[header.find("("):]) if w not in KEYWORDS)
    LOCALS.update(m.group(1) for line in body.split("\n") if is_declaration(line)
                  for m in [re.search(r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\])*\s*(?:=[^;]*)?;$", line.strip())] if m)
    workdir = ROOT / "build" / "permuter" / symbol
    shutil.rmtree(workdir, ignore_errors=True)
    workdir.mkdir(parents=True)

    Scorer.build = build
    scorer = Scorer(args.unit, symbol, source, build_command(f"{build}/src/{args.unit}.o"), expected, workdir)
    if re.search(r"INCLUDE_ASM\([^)]*,\s*" + re.escape(symbol) + r"\s*\)", head + tail):
        sys.exit(f"{symbol} is still INCLUDE_ASM'd outside its draft")
    base, err = scorer.score(text, "base")
    if base >= 1 << 30:
        sys.exit("the unmodified source does not compile here:\n" + err)
    print(f"start: score {base}" + (" (MATCH)" if base == 0 else ""), flush=True)
    if base == 0:
        return

    rnd = random.Random(args.seed)
    best_score, best_body = base, body
    cur_score, cur_body = base, body
    seen = {body}
    deadline = time.time() + args.minutes * 60
    tried = 0
    with mp.Pool(args.jobs, initializer=_init, initargs=(scorer,)) as pool:
        while time.time() < deadline:
            jobs = []
            for k in range(args.jobs):
                b = cur_body
                for _ in range(rnd.randint(1, args.depth)):
                    b = mutate(b, rnd, args.unsafe)
                if b in seen:
                    continue
                seen.add(b)
                jobs.append((head + b + tail, f"w{k}"))
            if not jobs:
                cur_score, cur_body = best_score, best_body  # stuck: restart from the best
                continue
            for s, cand, _ in pool.imap_unordered(_evaluate, jobs):
                tried += 1
                b = cand[len(head):len(cand) - len(tail)]
                if s < best_score:
                    best_score, best_body = s, b
                    (workdir / "best.c").write_text(cand)
                    diff = difflib.unified_diff(body.split("\n"), b.split("\n"), "original", "best", lineterm="", n=2)
                    (workdir / "best.diff").write_text("\n".join(diff) + "\n")
                    print(f"[{tried}] score {s}" + (" MATCH" if s == 0 else ""), flush=True)
                if s <= cur_score:
                    cur_score, cur_body = s, b
            if best_score == 0:
                break
            if rnd.random() < 0.02:  # occasionally restart from the best to leave dead-end plateaus
                cur_score, cur_body = best_score, best_body
    print(f"done: {tried} candidates, best score {best_score} (start {base})")
    if best_score < base:
        print(f"best: {workdir / 'best.c'}\ndiff: {workdir / 'best.diff'}")


if __name__ == "__main__":
    main()
