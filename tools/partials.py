#!/usr/bin/env python3
"""Inventory of the PARTIAL pseudo-implementations in src/ (docs/plan.md, "Header cleanup"): PARTIAL and MATCH comments,
padding members, local stand-in copies of other units' classes, duplicate class definitions, gates and other
matching mechanisms, each class's completeness against the reference (ref/), and which headers' strings each
original unit's .rodata contains. Read-only: it changes nothing but the generated part of the docs.

Inputs: the sources (comments and literals tokenized, `#if 0` blanked), the class blocks of the ref headers
(fields, methods, `/* vtable[n] */` slots; implicit members and template instances skipped), the unit status
(tools/leftovers.py), the original .rodata of every unit (addresses from the expected .rodata.s, bytes from
iso/<VERSION>), and the includers of each header (`ninja -t deps`, else a static include closure).

Terms:
- owner of a class: the unit of the ref header that defines it (ref .cpp types: that unit); for types the ref
  only has in lost+found.h, the src/ header that defines it. Names match by their last component.
- stand-in: a type defined in a .cpp; own (owner is the .cpp's unit), foreign, or unattributed (no ref class
  and no src/ header defines it: overlays and invented helpers).
- readiness of a class: complete (one ungated definition equal to the ref: fields by name and order, no padding,
  methods by name and parameter count, virtuals in slot order, no PARTIAL inside), superset (has every ref
  member, plus padding/extras/PARTIAL), partial (lacks ref members), gated (behind a feature macro or defined
  twice in its header), missing (no src/ header defines it), unattributed (no ref class).
- string compatibility (a heuristic): the literals of a header's non-template code with __FILE__ resolved through
  `#line`/`*_H_PATH`, under the includer's macros, compared with the original unit's .rodata strings: ok, adds
  (literals the original lacks), n/a (the unit's .rodata is still asm; missing literals = hidden conflicts).

Usage:
    python3 tools/partials.py [--summary]                 # full report, or only the counts
    python3 tools/partials.py --update docs/partials.md   # regenerate the part between the markers
    python3 tools/partials.py --unit Form/Menu/TechHelp   # start-hook view of one unit
    python3 tools/partials.py --header src/System/RsrcDB.h  # finish-hook view of one header
    python3 tools/partials.py --class CRsrcDB             # every definition and stand-in of one class
    python3 tools/partials.py --queue [-n 20]             # between-units view
    python3 tools/partials.py --strings [src/System/RsrcData.h]  # header strings vs the originals
"""

import argparse
import re
import struct
import subprocess
import sys
from collections import Counter, defaultdict
from functools import lru_cache
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import leftovers  # noqa: E402
from progress import ROOT, active_lines, source, units, version_dir  # noqa: E402

START, END = "<!-- partials:start -->", "<!-- partials:end -->"
SRC = ROOT / "src"
ORIG_PREFIX = "c:/KHCOM/MainApp/"
LEX = re.compile(r"//[^\n]*|/\*.*?\*/|\"(?:\\.|[^\"\\\n])*\"|'(?:\\.|[^'\\\n])*'", re.S)
TOKEN = re.compile(r"\"(?:\\.|[^\"\\\n])*\"|'(?:\\.|[^'\\\n])*'|[A-Za-z_]\w*|\S")
DIRECTIVE = re.compile(r"^\s*#\s*(\w+)\s*(.*?)\s*$")
DEFINE = re.compile(r"^\s*#\s*define\s+(\w+)(?:\s+(.*?))?\s*$")


def define_value(value):
    """A #define's value without a trailing comment (outside string literals)."""
    value = value or ""
    out, i, q = [], 0, None
    while i < len(value):
        c = value[i]
        if q:
            out.append(c)
            if c == "\\" and i + 1 < len(value):
                out.append(value[i + 1])
                i += 1
            elif c == q:
                q = None
        elif c in "\"'":
            q = c
            out.append(c)
        elif value.startswith("/*", i) or value.startswith("//", i):
            break
        else:
            out.append(c)
        i += 1
    return "".join(out).strip()


UNIT_IN_TEXT = re.compile(r"\bfor ((?:Form|Game|System)/[\w/]+)")
PADDING = re.compile(r"^(_m_0x[0-9A-Fa-f]+|_pad\w*)$")
PADDING_REF = re.compile(r"\b(_m_0x[0-9A-Fa-f]+|_pad\w*)\b")
STRING_CALLS = re.compile(r"\b(AssertApp|TRACE\w*|com_malloc_d|printf|PRINTF)\b")
PLAN_UNIT = re.compile(r"^[ \t]*\| \d+ \| `([^`]+)` \|", re.M)  # docs/plan.md's work table (may be indented)
CATEGORIES = ("method", "data member", "type", "fwd decl", "#include", "changed in place", "padding",
              "path override", "gate", "guessed", "header-level", "stand-in", "other")
READINESS = ("complete", "superset", "partial", "gated", "missing", "unattributed")


def unit_of(rel):
    """'System/RsrcDB' for 'System/RsrcDB.h' or '.cpp' (rel to src/ or to the ref's src/)."""
    return re.sub(r"\.(h|hpp|cpp|c)$", "", rel)


def last(name):
    return name.split("::")[-1]


def decode(literal):
    body = literal[1:-1]
    try:
        return body.encode("latin-1").decode("unicode_escape").encode("latin-1", "replace").decode("latin-1")
    except (UnicodeDecodeError, UnicodeEncodeError):
        return body


def bisect_right(a, x):
    lo, hi = 0, len(a)
    while lo < hi:
        mid = (lo + hi) // 2
        if x < a[mid]:
            hi = mid
        else:
            lo = mid + 1
    return lo


def blank(text, keep_strings=False):
    """(text with comments blanked - and literals emptied unless keep_strings -, [(pos, literal)])"""
    out, strings, pos = [], [], 0
    for m in LEX.finditer(text):
        out.append(text[pos:m.start()])
        s = m.group(0)
        if s[0] == "/":
            out.append(re.sub(r"[^\n]", " ", s))
        else:
            if s[0] == '"':
                strings.append((m.start(), s))
            out.append(s if keep_strings else s[0] + " " * (len(s) - 2) + s[-1])
        pos = m.end()
    out.append(text[pos:])
    return "".join(out), strings


def blank_pp(code):
    lines, cont = code.split("\n"), False
    for i, line in enumerate(lines):
        if cont or line.lstrip().startswith("#"):
            cont = line.rstrip().endswith("\\")
            lines[i] = " " * len(line)
    return "\n".join(lines)


def parse_cond(kind, expr):
    """[(macro, must be defined)] of a conditional; [] when not a simple one."""
    expr = expr.split("//")[0].split("/*")[0].strip()
    if kind == "ifdef":
        return [(expr, True)] if re.fullmatch(r"\w+", expr) else []
    if kind == "ifndef":
        return [(expr, False)] if re.fullmatch(r"\w+", expr) else []
    m = re.fullmatch(r"(!?)\s*defined\s*\(?\s*(\w+)\s*\)?", expr)
    return [(m.group(2), not m.group(1))] if m else []


class Scope:
    __slots__ = ("kind", "name", "keyword", "template", "start", "end", "head", "parent", "bases", "children")

    def __init__(self, kind, parent, start, name="", keyword="", template=False, head="", bases=()):
        self.kind, self.parent, self.start, self.end = kind, parent, start, None
        self.name, self.keyword, self.template, self.head, self.bases = name, keyword, template, head, bases
        self.children = []


def classify(head):
    h = re.sub(r"^\s*(?:(?:public|private|protected)\s*:\s*)+", "", head).strip()
    template = False
    if re.match(r"template\s*<", h):
        depth, i = 0, h.index("<")
        for i in range(i, len(h)):
            depth += {"<": 1, ">": -1}.get(h[i], 0)
            if depth == 0:
                break
        h, template = h[i + 1:].strip(), True
    if re.fullmatch(r"namespace(?:\s+\w+)?", h):
        return "ns", h.split()[1] if " " in h else "", "", template, ()
    if re.match(r'extern\s*"', h):
        return "ns", "", "", template, ()
    m = re.fullmatch(r"(?:typedef\s+)?(class|struct|union|enum)\b\s*(\w*)\s*(<.*>)?\s*(?::(.*))?", h, re.S)
    if m and "(" not in h and "=" not in h.split(":")[0]:
        bases = []
        if m.group(4):
            for b in re.split(r",(?![^<]*>)", m.group(4)):
                b = re.sub(r"\b(public|private|protected|virtual)\b", "", b).strip()
                if b:
                    bases.append(last(re.sub(r"<.*", "", b).strip()))
        return "type", m.group(2), m.group(1), template or bool(m.group(3)), tuple(bases)
    return "body", "", "", template, ()


def scan_scopes(code):
    top = Scope("ns", None, -1)
    stack, head_start, paren = [top], 0, 0
    for m in re.finditer(r"[{};()]", code):
        c, i = m.group(), m.start()
        if c == "(":
            paren += 1
        elif c == ")":
            paren = max(0, paren - 1)
        elif paren:
            continue
        elif c == ";":
            head_start = i + 1
        elif c == "{":
            parent = stack[-1]
            if parent.kind in ("ns", "type"):
                kind, name, keyword, template, bases = classify(code[head_start:i])
                sc = Scope(kind, parent, i, name, keyword, template or parent.template, code[head_start:i], bases)
            else:
                sc = Scope("body", parent, i, template=parent.template)
            parent.children.append(sc)
            stack.append(sc)
            head_start = i + 1
        elif len(stack) > 1:
            sc = stack.pop()
            sc.end = i
            if sc.kind == "type" and not sc.name and re.match(r"\s*typedef\b", sc.head):
                m = re.match(r"\s*(\w+)\s*;", code[i + 1:i + 200])
                sc.name = m.group(1) if m else ""
            head_start = i + 1
    for sc in stack[1:]:
        sc.end = len(code)
    top.end = len(code)
    return top


class Member:
    __slots__ = ("kind", "name", "params", "virtual", "static", "body", "line", "text", "vslot")

    def __init__(self, kind, name, line, text, params=None, virtual=False, static=False, body=None, vslot=None):
        self.kind, self.name, self.line, self.text, self.params = kind, name, line, text, params
        self.virtual, self.static, self.body, self.vslot = virtual, static, body, vslot


def count_params(text):
    text = text.strip()
    if text in ("", "void") or re.fullmatch(r"\w+\s*\*\s*,\s*int\s*,\s*void", text):
        return 0  # the ref writes some inline methods as f(X*, int, void): parameters unknown
    depth, n = 0, 1
    for c in text:
        if c in "(<[":
            depth += 1
        elif c in ")>]":
            depth -= 1
        elif c == "," and depth == 0:
            n += 1
    return n


def parse_member(text, cls, line, body, vslot):
    s = re.sub(r"^\s*(?:(?:public|private|protected)\s*:\s*)+", "", text).strip()
    s = re.sub(r"\s+", " ", s)
    s = re.sub(r"\s*__attribute__\s*\(\((?:[^()]|\([^()]*\))*\)\)", "", s).strip()
    if not s or s.startswith(("friend ", "using ", "typedef ")) or re.match(r"(class|struct|union|enum) \w+ ?$", s):
        return []
    if re.match(r"(static )?(class|struct|union|enum)\b", s) and "{}" in s:
        rest = s.split("{}", 1)[1]
        return [Member("field", n, line, s, static=s.startswith("static ")) for n in re.findall(r"(\w+)\s*(?:\[[^\]]*\])?\s*(?:,|$)", rest)]
    p = s.find("(")
    eq = s.find("=")
    is_op = re.search(r"\boperator\b", s[:p] if p >= 0 else "")
    if p >= 0 and (is_op or eq < 0 or eq > p) and not re.match(r"\(\s*(?:\w+::)*[*&]", s[p:]):
        prefix = s[:p].rstrip()
        m = re.search(r"(operator\s*\S+|~?\w+)$", prefix)
        if not m:
            return []
        name = m.group(1).replace(" ", "")
        depth, q = 0, p
        for q in range(p, len(s)):
            depth += {"(": 1, ")": -1}.get(s[q], 0)
            if depth == 0:
                break
        params = s[p + 1:q]
        if name == cls and re.match(r"\s*" + re.escape(cls) + r"\b[^,]*\*\s*,", params):
            name, params = "~" + cls, ""  # the ref writes destructors as X(X*, int)
        return [Member("method", name, line, s, count_params(params), bool(re.search(r"\bvirtual\b", prefix)),
                       bool(re.search(r"\bstatic\b", prefix)), body, vslot)]
    static = bool(re.search(r"\bstatic\b", s))
    out = []
    depth, start, parts = 0, 0, []
    for i, c in enumerate(s):
        if c in "(<[{":
            depth += 1
        elif c in ")>]}":
            depth -= 1
        elif c == "," and depth == 0:
            parts.append(s[start:i])
            start = i + 1
    parts.append(s[start:])
    for part in parts:
        part = part.split("=")[0]
        part = re.sub(r"\[[^\]]*\]", "", part)
        part = re.sub(r":\s*\w+\s*$", "", part)  # bit field
        # pointers to arrays/functions/members: T (*name)[n], T (C::*name)()
        m = (re.search(r"\(\s*(?:\w+::)*[*&]+\s*([A-Za-z_]\w*)\s*\)", part)
             or re.search(r"([A-Za-z_$]\w*)\s*\)?\s*(?:\([^)]*\))?\s*$", part))
        if m and not m.group(1).startswith("$"):
            out.append(Member("field", m.group(1), line, s, static=static))
    return out


class TypeDef:
    def __init__(self, src, sc, gate):
        self.src, self.sc, self.gate = src, sc, gate
        self.name, self.keyword, self.template, self.bases = sc.name, sc.keyword, sc.template, sc.bases
        self.line, self.end_line = src.line(sc.start), src.line(sc.end)
        self._members = None

    @property
    def file(self):
        return self.src.rel

    @property
    def members(self):
        if self._members is None:
            self._members = members_of(self.src, self.sc)
        return self._members


def members_of(src, sc):
    code, out = src.code, []
    pos, cur, cur_start = sc.start + 1, "", sc.start + 1
    kids = sorted(sc.children, key=lambda k: k.start)

    def emit(text, start, body):
        m = re.search(r"\S", code[start:start + 400])
        line = src.line(start + (m.start() if m else 0))
        out.extend(parse_member(text, sc.name, line, body, src.vslots.get(line)))

    for kid in kids + [None]:
        stop = kid.start if kid else sc.end
        pieces = code[pos:stop].split(";")
        offset = pos
        for piece in pieces[:-1]:
            emit(cur + piece, cur_start, None)
            offset += len(piece) + 1
            cur, cur_start = "", offset
        cur += pieces[-1]
        if kid is None:
            if cur.strip():
                emit(cur, cur_start, None)
            break
        if re.search(r"(?:^|[\s;:{}])(?:struct|union)\s*$", cur) and re.match(r"\s*;", code[kid.end + 1:]):
            # an anonymous struct/union member: its fields are the class's own (C++ anonymous union, gcc's
            # anonymous struct extension)
            text = re.sub(r"(?:struct|union)\s*$", "", cur)
            if text.strip():
                emit(text, cur_start, None)
            out.extend(m for m in members_of(src, kid) if m.kind == "field")
            cur, cur_start = "", kid.end + 1
        elif kid.kind == "type" or re.search(r"(?:^|[\s;:])(?:static\s+)?(?:struct|union|class)\s*$", cur):
            cur += " {} "  # also anonymous struct members: static struct {...} m_pStateTbl[n]
        else:
            emit(cur, cur_start, (kid.start, kid.end))
            cur, cur_start = "", kid.end + 1
        pos = kid.end + 1
    return out


class Source:
    """One source file: comments, literals, conditionals, type definitions."""

    def __init__(self, path, rel, ref=False):
        raw = path.read_text(encoding="latin-1")
        if ref:
            raw = raw.replace("{anonymous}", "_anonymous_")
        self.path, self.rel, self.ref = path, rel, ref
        self.unit = unit_of(rel)
        self.is_header = rel.endswith((".h", ".hpp"))
        self.raw_lines = raw.split("\n")
        self.active = raw if ref else "\n".join(active_lines(raw))
        self._starts = [0] + [m.end() for m in re.finditer("\n", self.active)]
        rawcode, _ = blank(raw)
        self.rawcode_lines = rawcode.split("\n")
        self.comments = []  # (line, text), disabled code included
        starts = [0] + [m.end() for m in re.finditer("\n", raw)]
        for m in LEX.finditer(raw):
            if m.group(0)[0] == "/":
                self.comments.append((bisect_right(starts, m.start()), m.group(0)))
        code, self.strings = blank(self.active)
        self.code = blank_pp(code)
        self.vslots = {}
        if ref:
            for line, text in self.comments:
                m = re.search(r"vtable\[(\d+)\]", text)
                if m:
                    self.vslots[line] = int(m.group(1))
        self.guard = None
        self.conds = self._conditions()
        self.top = scan_scopes(self.code)
        self.types = []
        self._collect(self.top)
        self._defines = self._literals = None

    def line(self, pos):
        return bisect_right(self._starts, pos)

    def _conditions(self):
        """Per line (index = line number): the [(macro, defined)] terms that must hold; not the include guard."""
        per_line, stack, cur = [()], [], ()
        directives = []
        for line in self.raw_lines:
            m = DIRECTIVE.match(line)
            if m and m.group(1) in ("if", "ifdef", "ifndef", "elif", "else", "endif"):
                directives.append((m.group(1), m.group(2)))
        if self.is_header and directives and directives[0][0] == "ifndef" and directives[0][1]:
            g = directives[0][1].split()[0]
            if any(re.match(r"\s*#\s*define\s+" + re.escape(g) + r"\b", l) for l in self.raw_lines[:60]):
                self.guard = g
        self.cond_sites = []  # (line, kind, terms)
        for n, line in enumerate(self.raw_lines, 1):
            m = DIRECTIVE.match(line)
            kind = m.group(1) if m else None
            if kind in ("if", "ifdef", "ifndef"):
                terms = parse_cond(kind, m.group(2))
                if terms and terms[0][0] == self.guard and not stack:
                    terms = []
                stack.append([cur, [terms], terms])
                cur = cur + tuple(terms)
                self.cond_sites.append((n, kind, terms))
            elif kind in ("elif", "else") and stack:
                outer, prev, _ = stack[-1]
                neg = tuple((mac, not d) for t in prev if len(t) == 1 for mac, d in t)
                terms = parse_cond("if", m.group(2)) if kind == "elif" else []
                stack[-1] = [outer, prev + [terms], terms]
                cur = outer + neg + tuple(terms)
                self.cond_sites.append((n, kind, list(neg) + terms))
            elif kind == "endif" and stack:
                cur = stack.pop()[0]
            per_line.append(cur)
        per_line.append(cur)
        return per_line

    def _collect(self, scope):
        for sc in scope.children:
            if sc.kind == "type" and sc.name:
                self.types.append(TypeDef(self, sc, self.conds[min(self.line(sc.start), len(self.conds) - 1)]))
            if sc.kind == "ns":
                self._collect(sc)

    def code_at(self, n):
        """The code of raw line n (1-based), or of the next lines when n holds only a comment."""
        for k in range(n, min(n + 4, len(self.rawcode_lines) + 1)):
            text = self.rawcode_lines[k - 1].strip()
            if text:
                return text
        return ""

    def defines(self):
        if self._defines is None:
            self._defines = {}
            for line in self.active.split("\n"):
                m = DEFINE.match(line)
                if m:
                    self._defines.setdefault(m.group(1), define_value(m.group(2)))
        return self._defines

    def template_ranges(self):
        out = []

        def walk(scope):
            for sc in scope.children:
                if sc.template:
                    out.append((sc.start, sc.end))
                else:
                    walk(sc)
        walk(self.top)
        return out

    def literals(self):
        """[(line, parts, terms)]: the string literal sequences of the active non-template code; parts are
        ('s', text) and ('m', macro) - __FILE__ becomes the file of the last #line (a macro or a string)."""
        if self._literals is not None:
            return self._literals
        strcode, _ = blank(self.active, keep_strings=True)
        cands = [((), ("s", ORIG_PREFIX + self.rel))]  # (terms, value) of each #line so far
        files = []
        for n, line in enumerate(self.raw_lines, 1):
            m = re.match(r"\s*#\s*line\s+\d+\s*(.*?)\s*$", line)
            if m and m.group(1):
                v = m.group(1)
                cands = cands + [(self.conds[n], ("s", decode(v)) if v.startswith('"') else ("m", v))]
            files.append(tuple(cands))
        code = blank_pp(strcode)
        tranges = self.template_ranges()
        offsets = [0]
        for line in code.split("\n"):
            offsets.append(offsets[-1] + len(line) + 1)
        out, run, run_line = [], [], 0

        def flush():
            if run and any(k in ("s", "f") for k, _ in run):
                parts = []
                for k, v in run:
                    parts.append(("f", files[run_line - 1]) if k == "f" else (k, v))
                out.append((run_line, tuple(parts), self.conds[min(run_line, len(self.conds) - 1)]))

        depth, asm_depth, prev = 0, None, ""
        for m in TOKEN.finditer(code):
            tok = m.group(0)
            if tok == "(":
                depth += 1
            elif tok == ")":
                depth -= 1
                if asm_depth is not None and depth <= asm_depth:
                    asm_depth = None
            elif tok in ("asm", "__asm__", "__asm") and asm_depth is None:
                asm_depth = depth
            if asm_depth is not None or (prev == "extern" and tok.startswith('"')):
                prev = tok
                continue  # inline asm operands, extern "C"
            prev = tok
            if tok.startswith('"'):
                item = ("s", decode(tok))
            elif tok == "__FILE__":
                item = ("f", None)
            elif re.fullmatch(r"\w+_PATH", tok):
                item = ("m", tok)
            else:
                flush()
                run = []
                continue
            if any(a <= m.start() <= b for a, b in tranges):
                continue
            if not run:
                run_line = bisect_right(offsets, m.start())
            run.append(item)
        flush()
        self._literals = out
        return out


def norm(s):
    s = s.replace("\\", "/").lower()
    s = re.sub(r"^c:/khcom/mainapp/", "", s)
    while True:
        t = re.sub(r"[^/]+/\.\./", "", s, count=1)
        if t == s:
            return s
        s = t


def gate_kind(mac):
    if mac.endswith("_CPP"):
        return "unit gate"
    if mac.endswith("_PATH"):
        return "path override"
    if "_NO_" in mac:
        return "opt-out"
    if "FULL" in mac:
        return "full-class"
    return "feature"


def placeholders(src):
    """Uncalled `_name` inline functions holding only strings (TRACE, AssertApp, com_malloc_d, literals)."""
    out = []
    for sc in src.top.children:
        if sc.kind != "body":
            continue
        m = re.search(r"\binline\b[^;{}]*?\b(_\w+)\s*\(\s*\)\s*$", sc.head)
        if not m:
            continue
        name = m.group(1)
        body = src.active[sc.start:sc.end]
        if not ('"' in body or STRING_CALLS.search(body)):
            continue
        if len(re.findall(r"\b" + re.escape(name) + r"\b", src.code)) > 1:
            continue
        out.append((name, src.line(sc.start)))
    return out


def categorize(src, line, text, first_code):
    code = src.code_at(line)
    low = text.lower()
    if src.is_header and line < first_code:
        return "header-level"
    if "_H_PATH" in code or "path override" in low:
        return "path override"
    if re.match(r"#\s*(if|ifdef|ifndef|elif|else)\b", code) or re.search(
            r"declares the class only|only for its own unit|\bgate", low):
        return "gate"
    if re.search(r"\b(_m_0x|_pad)", code) or "padding" in low:
        return "padding"
    if re.search(r"guess|\bnames? unknown|unknown name", low):
        return "guessed"
    if not src.is_header and ("stand-in" in low or re.match(r"(class|struct|union|enum)\s+\w+[^;]*\{", code)):
        return "stand-in"
    if code.startswith("#include") or code.startswith("# include"):
        return "#include"
    if re.match(r"(namespace\s+\w+\s*\{\s*)?(class|struct|union|enum)\s+[\w:]+\s*;", code):
        return "fwd decl"
    if not re.search(r"\b(added|from)\b", low):
        return "changed in place"
    if re.match(r"(typedef\b|template\s*<|(class|struct|union|enum)\b)", code):
        return "type"
    if re.match(r"#\s*define\b", code):
        return "other"
    if "(" in code:
        return "method"
    if code.endswith((";", ",")):
        return "data member"
    return "other"


def extract_strings(blob):
    out = []
    i, n = 0, len(blob)
    while i < n:
        if i % 4 or (i and blob[i - 1] != 0):
            i += 1
            continue
        j = blob.find(b"\0", i)
        if j < 0:
            break
        s = blob[i:j]
        if s and valid_text(s):
            out.append(s.decode("latin-1"))
            i = j + 1
        else:
            i += 4 if not s else 1
    return out


def valid_text(s):
    k = 0
    while k < len(s):
        b = s[k]
        if 0x20 <= b < 0x7F or b in (9, 10, 13, 27) or 0xA1 <= b <= 0xDF:
            k += 1
        elif (0x81 <= b <= 0x9F or 0xE0 <= b <= 0xFC) and k + 1 < len(s) and (
                0x40 <= s[k + 1] <= 0x7E or 0x80 <= s[k + 1] <= 0xFC):
            k += 2
        else:
            return False
    return True


def compare(ours, ref):
    rf = [m for m in ref.members if m.kind == "field"]
    of = [m for m in ours.members if m.kind == "field"]
    ref_fields = [m.name for m in rf if not m.static]
    our_all = [m.name for m in of if not m.static]
    padding = [n for n in our_all if PADDING.match(n)]
    our_fields = [n for n in our_all if not PADDING.match(n)]
    ref_static = {m.name for m in rf if m.static}
    our_static = {m.name for m in of if m.static}
    cls = ref.name

    def methods(t):
        out = defaultdict(set)
        for m in t.members:
            if m.kind != "method" or m.name.startswith("$"):
                continue
            if (m.name == cls and m.params == 0) or (m.name == "operator=" and m.params == 0):
                continue  # implicit members
            out[m.name].add(m.params)
        return out
    rm, om = methods(ref), methods(ours)
    ref_names = {m.name for m in ref.members if m.kind == "method"}
    # A destructor right after the implicit operator=/X() block is the implicit one (gcc 2.95 chains the
    # implicitly declared members in front of the user's): not required unless our class declares it.
    dtor = "~" + cls
    lead = [m.name for m in ref.members if m.kind == "method" and not m.name.startswith("$")
            and not (m.name == "operator=" or (m.name == cls and m.params == 0))][:1]
    names = [(m.name, m.params) for m in ref.members if m.kind == "method" and not m.name.startswith("$")]
    last_x = max((i for i, (n, ps) in enumerate(names) if n == cls and ps == 0), default=-1)
    # ... also one listed inside that block (before its last X()): op=, X(<user params>), ~X, X()
    in_block = any(n == dtor for n, _ in names[:last_x])
    implicit_dtor = (lead == [dtor] or in_block) and dtor not in om
    if implicit_dtor:
        rm.pop(dtor, None)
    common_f = [f for f in our_fields if f in ref_fields]
    # Overriders keep their slot from the base: compare the declaration order (the ref's member order).
    rvirt = [m.name for m in ref.members if m.kind == "method" and m.vslot is not None
             and not (implicit_dtor and m.name == dtor)]
    ovirt = [m.name for m in ours.members if m.kind == "method" and m.virtual]
    vcommon = list(dict.fromkeys(v for v in ovirt if v in rvirt))  # overloads: by name, first declaration
    return {
        "missing_fields": [f for f in ref_fields if f not in our_fields] + sorted(ref_static - our_static),
        "extra_fields": [f for f in our_fields if f not in ref_fields],
        "padding": padding,
        "order_ok": common_f == [f for f in ref_fields if f in common_f],
        "missing_methods": sorted(n for n in rm if n not in om),
        # a name the ref lists at all is not extra (its parameterless operator= entries are dropped above)
        "extra_methods": sorted(n for n in om if n not in rm and n not in ref_names and n not in ("~" + cls, cls)),
        # the ref lists only named parameters (unnamed/unused ones are dropped): fewer than the ref is wrong
        "param_mismatch": sorted(n for n, ps in rm.items() if n in om and any(ps)
                                 and all(o < p for o in om[n] for p in ps if p)),
        "vorder_ok": vcommon == [v for v in dict.fromkeys(rvirt) if v in vcommon],
        "ref_file": ref.file,
    }


class Project:
    def __init__(self):
        self.version = version_dir(None)
        self.ref_root = ROOT / "ref" / self.version / "src"
        self.files = {}
        for path in sorted(SRC.rglob("*")):
            rel = path.relative_to(SRC).as_posix()
            if path.suffix in (".h", ".hpp", ".cpp", ".c") and not rel.startswith("lib/"):
                self.files[rel] = Source(path, rel)
        self.headers = {r: s for r, s in self.files.items() if s.is_header}
        self.sources = {r: s for r, s in self.files.items() if not s.is_header}
        self.ref = defaultdict(list)  # last name -> [(TypeDef, owner unit or None)]
        self.methods_in = defaultdict(Counter)  # class -> {ref .cpp unit: methods defined there}
        if self.ref_root.exists():
            for path in sorted(self.ref_root.rglob("*")):
                if path.suffix not in (".h", ".cpp"):
                    continue
                rel = path.relative_to(self.ref_root).as_posix()
                src = Source(path, rel, ref=True)
                owner = None if rel == "lost+found.h" else unit_of(rel)
                for t in src.types:
                    if not t.template:
                        self.ref[t.name].append((t, owner))
                if rel.endswith(".cpp"):
                    for cls in re.findall(r"^[\w*& <>:,]*?\b(\w+)::~?\w+\s*\(", src.code, re.M):
                        self.methods_in[cls][unit_of(rel)] += 1
        self.defs = defaultdict(list)  # last name -> [TypeDef] in src/ (non-template)
        for src in self.files.values():
            for t in src.types:
                if not t.template:
                    self.defs[t.name].append(t)
        self.guards = {s.guard: r for r, s in self.headers.items() if s.guard}
        self._status = self._deps = self._orig = None
        self._cache = {}

    def cached(self, key, fn):
        if key not in self._cache:
            self._cache[key] = fn()
        return self._cache[key]

    # ---- units, ownership

    @property
    def status(self):
        """unit -> (INCLUDE_ASM, parked, data sections from source, has a source)"""
        if self._status is None:
            rows, _, _ = leftovers.collect(self.version)
            st = defaultdict(lambda: [0, 0])
            for unit, _, _, state, _ in rows:
                st[unit][0] += 1
                st[unit][1] += state == "parked"
            self._status = {}
            for unit, _ in units(self.version):
                src = source(unit)
                sd = set()
                if src is not None and src.suffix != ".s":
                    listed = leftovers.SOURCE_DATA.findall(src.read_text(encoding="latin-1"))
                    sd = set(listed[-1].split()) if listed else set()
                self._status[unit] = (st[unit][0], st[unit][1], sd, src is not None)
        return self._status

    def plan_order(self):
        def read():
            plan = ROOT / "docs" / "plan.md"
            return PLAN_UNIT.findall(plan.read_text(encoding="utf-8")) if plan.exists() else []
        return self.cached("plan", read)

    def in_progress(self, unit):
        """A unit of docs/plan.md's work table with functions neither matched nor parked."""
        if unit not in self.plan_order():
            return False
        asm, parked, _, _ = self.status.get(unit, (0, 0, set(), False))
        return asm > parked

    def ref_owner(self, name):
        """The ref header's unit; for lost+found.h classes, the ref .cpp defining most of their methods."""
        owners = [o for _, o in self.ref.get(name, ()) if o]
        if owners:
            return owners[0]
        if name in self.ref and self.methods_in.get(name):
            return self.methods_in[name].most_common(1)[0][0]
        return None

    def home(self, name):
        """The primary src/ header definition: the one in the ref owner's header, else the first."""
        homes = [t for t in self.defs.get(name, ()) if t.src.is_header]
        if not homes:
            return None
        owner = self.ref_owner(name)
        return next((t for t in homes if t.src.unit == owner), homes[0])

    def owner(self, name):
        o = self.ref_owner(name)
        if o:
            return o
        h = self.home(name)
        return h.src.unit if h else None

    def owner_header(self, name):
        h = self.home(name)
        if h:
            return h.file
        o = self.ref_owner(name)
        return f"{o}.h" if o else None

    def standins(self):
        """[(TypeDef, kind)] for the types defined in .cpp files: own, foreign, unattributed."""
        def build():
            out = []
            for src in self.sources.values():
                for t in src.types:
                    if t.template:
                        continue
                    owner = self.owner(t.name)
                    if owner is None:
                        kind = "unattributed"
                    elif owner == src.unit or any(o == src.unit for _, o in self.ref.get(t.name, ())):
                        kind = "own"
                    else:
                        kind = "foreign"
                    out.append((t, kind))
            return out
        return self.cached("standins", build)

    def standin_kind(self, t):
        kinds = self.cached("standin_kinds", lambda: {id(x): k for x, k in self.standins()})
        return kinds.get(id(t))

    def duplicates(self):
        def build():
            out = {}
            for name, ts in self.defs.items():
                hs = sorted({t.file for t in ts if t.src.is_header})
                if len(hs) > 1:
                    out[name] = hs
            return out
        return self.cached("dups", build)

    # ---- readiness

    def readiness(self, name):
        """(state, details)"""
        return self.cached(("ready", name), lambda: self._readiness(name))

    def _readiness(self, name):
        home = self.home(name)
        refs = self.ref.get(name)
        if home is None:
            return "missing", {}
        if not refs:
            return "unattributed", {}
        owner = self.ref_owner(name)
        ref = next((t for t, o in refs if o == home.src.unit), None)
        if ref is None:
            # several ref types share the name (e.g. CARD_DATA): the one with the most of our field names
            ours_f = {m.name for m in home.members if m.kind == "field"}
            by_fields = max(refs, key=lambda r: len(ours_f & {m.name for m in r[0].members if m.kind == "field"}))
            if len(refs) > 1 and ours_f & {m.name for m in by_fields[0].members if m.kind == "field"}:
                ref = by_fields[0]
            else:
                ref = next((t for t, o in refs if o == owner), refs[0][0])
        d = compare(home, ref)
        d["partial_marks"] = sum(text.count("PARTIAL") for line, text in home.src.comments
                                 if home.line <= line <= home.end_line)
        same_header = [t for t in self.defs[name] if t.file == home.file]
        if home.gate or len(same_header) > 1:
            return "gated", d
        if d["missing_fields"] or d["missing_methods"]:
            return "partial", d
        if (d["padding"] or not d["order_ok"] or not d["vorder_ok"] or d["extra_fields"] or d["extra_methods"]
                or d["partial_marks"] or d["param_mismatch"]):
            return "superset", d
        return "complete", d

    def class_names(self):
        """Classes with a src/ header home or a foreign stand-in."""
        def build():
            names = {t.name for ts in self.defs.values() for t in ts if t.src.is_header}
            names |= {t.name for t, k in self.standins() if k == "foreign"}
            return tuple(sorted(names))
        return self.cached("class_names", build)

    # ---- includes and strings

    @property
    def deps(self):
        """unit -> [header rel paths] in include order (ninja -t deps; else the static closure)."""
        if self._deps is None:
            self._deps = {}
            try:
                out = subprocess.run(["ninja", "-t", "deps"], cwd=ROOT, capture_output=True, text=True).stdout
            except OSError:
                out = ""
            cur = None
            prefix = f"build/{self.version}/src/"
            for line in out.split("\n"):
                if line and not line.startswith(" "):
                    obj = line.split(":")[0]
                    cur = unit_of(obj[len(prefix):-2]) if obj.startswith(prefix) and obj.endswith(".o") else None
                    if cur is not None:
                        self._deps[cur] = []
                elif cur is not None and line.strip().startswith("src/"):
                    rel = line.strip()[4:]
                    if rel in self.headers and rel not in self._deps[cur]:
                        self._deps[cur].append(rel)
            for rel, src in self.sources.items():
                if src.unit not in self._deps or not self._deps[src.unit]:
                    self._deps[src.unit] = self.closure(rel)[1:]
        return self._deps

    def closure(self, rel):
        """rel and the headers it includes, depth first, in order."""
        def build():
            seen, out = set(), []

            def visit(r):
                if r in seen or r not in self.files:
                    return
                seen.add(r)
                out.append(r)
                base = Path(r).parent
                for line in self.files[r].active.split("\n"):
                    m = re.match(r'\s*#\s*include\s*"([^"]+)"', line)
                    if m:
                        cand = (base / m.group(1)).as_posix()
                        visit(cand if cand in self.files else m.group(1))
            visit(rel)
            return out
        return self.cached(("closure", rel), build)

    def includers(self, header):
        return sorted(u for u, hs in self.deps.items() if header in hs)

    def unit_source(self, unit):
        return next((f"{unit}{e}" for e in (".cpp", ".c") if f"{unit}{e}" in self.sources), None)

    def macros(self, unit, extra=()):
        """The macros a unit's code sees: its headers' defines, overridden by the .cpp's own."""
        out = {}
        for h in list(self.deps.get(unit, ())) + list(extra):
            for k, v in self.files[h].defines().items():
                out.setdefault(k, v)
        rel = self.unit_source(unit)
        if rel:
            out.update(self.sources[rel].defines())
        return out

    def header_strings(self, header, macros):
        out = []
        for _, parts, terms in self.files[header].literals():
            if not all((m in macros) == d for m, d in terms):
                continue
            text = []
            for k, v in parts:
                if k == "f":  # the last #line whose conditions hold
                    k, v = next(val for t, val in reversed(v) if all((m in macros) == d for m, d in t))
                if k == "s":
                    text.append(v)
                else:
                    val = macros.get(v, "")
                    if not val.startswith('"'):
                        text = None
                        break
                    text.append(decode(val))
            if text is not None and "".join(text):
                out.append("".join(text))
        return out

    @property
    def originals(self):
        """unit -> [strings of its original .rodata, in address order]"""
        if self._orig is None:
            self._orig = {}
            expected = ROOT / "build" / self.version / "expected" / "asm" / "data"
            elf = ROOT / "iso" / self.version
            if not expected.exists() or not elf.exists():
                return self._orig
            starts = []
            for path in expected.rglob("*.rodata.s"):
                m = re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ", path.read_text(encoding="utf-8"))
                if m:
                    starts.append((int(m.group(1), 16), path.relative_to(expected).as_posix()[:-len(".rodata.s")]))
            starts.sort()
            data = elf.read_bytes()
            phoff = struct.unpack_from("<I", data, 0x1C)[0]
            phentsize, phnum = struct.unpack_from("<HH", data, 0x2A)
            segs = []
            for k in range(phnum):
                p_type, p_offset, p_vaddr, _, p_filesz = struct.unpack_from("<5I", data, phoff + k * phentsize)
                if p_type == 1:
                    segs.append((p_vaddr, p_offset, p_filesz))
            for k, (addr, unit) in enumerate(starts):
                end = starts[k + 1][0] if k + 1 < len(starts) else addr + 0x1000
                seg = next((s for s in segs if s[0] <= addr < s[0] + s[2]), None)
                if seg:
                    lo, hi = seg[1] + addr - seg[0], seg[1] + min(end, seg[0] + seg[2]) - seg[0]
                    self._orig[unit] = extract_strings(data[lo:hi])
        return self._orig

    def orig_set(self, unit):
        return self.cached(("orig", unit), lambda: frozenset(self.originals.get(unit, ())))

    def default_macros(self, header):
        def build():
            out = {}
            for h in self.closure(header):
                for k, v in self.files[h].defines().items():
                    out.setdefault(k, v)
            return out
        return self.cached(("defmac", header), build)

    def fingerprint(self, header):
        """The header's literals that name it (normalized paths): its trace in an original .rodata."""
        base = Path(header).name.lower()
        return {norm(s) for s in self.header_strings(header, self.default_macros(header)) if base in s.lower()}

    def orig_with(self, header):
        def build():
            fp = self.fingerprint(header)
            if not fp:
                return None
            return sorted(u for u, ss in self.originals.items() if any(norm(s) in fp for s in ss))
        return self.cached(("origwith", header), build)

    def string_status(self, unit, headers, extra=()):
        """(ok | adds | n/a | no source, [literals the original lacks]) of the given headers in a unit."""
        st = self.status.get(unit)
        macros = self.macros(unit, extra)
        rel = self.unit_source(unit)
        own = self.sources[rel].defines() if rel else {}
        headers = [h for h in headers if self.files[h].guard not in own]  # include guard pre-defined
        lits = [s for h in headers for s in self.header_strings(h, macros)]
        orig = self.orig_set(unit)
        missing = list(dict.fromkeys(s for s in lits if s not in orig))
        if st is None or not st[3]:
            return "no source", missing
        if ".rodata" in st[2]:
            return ("adds" if missing else "ok"), missing
        return "n/a", missing

    def include_adds(self, unit, header):
        """String status if the unit included header: its closure minus what the unit includes already."""
        have = set(self.deps.get(unit, ()))
        new = [h for h in self.closure(header) if h not in have]
        return self.string_status(unit, new, extra=new)

    # ---- comments and mechanisms

    def partials(self):
        """[(file, line, category, unit, text)] per PARTIAL occurrence."""
        def build():
            out = []
            for rel, src in self.files.items():
                first_code = next((n for n, l in enumerate(src.rawcode_lines, 1) if l.strip()), 1)
                for line, text in src.comments:
                    n = text.count("PARTIAL")
                    if n:
                        cat = categorize(src, line, text, first_code)
                        m = UNIT_IN_TEXT.search(text)
                        out.extend([(rel, line, cat, m.group(1) if m else "", text)] * n)
            return out
        return self.cached("partials", build)

    def match_lines(self):
        """{(file, line): reason} of MATCH: comments (a comment tags its lines and the next one)."""
        def build():
            out = {}
            for rel, src in self.files.items():
                for line, text in src.comments:
                    if "MATCH:" in text:
                        reason = re.sub(r"\s*\*/\s*$", "", text.split("MATCH:", 1)[1]).strip()
                        for k in range(line, line + text.count("\n") + 2):
                            out[(rel, k)] = reason
            return out
        return self.cached("match", build)

    def tagged(self, rel, line):
        return self.match_lines().get((rel, line))

    def mechanisms(self):
        """[(kind, key, file, line)]: every site of a matching mechanism."""
        return self.cached("mech", self._mechanisms)

    def _mechanisms(self):
        out = []
        header_macros = defaultdict(list)  # macro -> [(header, line)] conditional sites
        all_defined = defaultdict(list)  # macro -> [(file, line, value)]
        for rel, src in self.files.items():
            for n, line in enumerate(src.raw_lines, 1):
                m = DEFINE.match(line)
                if m:
                    all_defined[m.group(1)].append((rel, n, m.group(2) or ""))
                m = re.match(r"\s*#\s*pragma\s+(interface|implementation)\b", line)
                if m:
                    out.append(("pragma", f"#pragma {m.group(1)}", rel, n))
            if src.is_header:
                for n, kind, terms in src.cond_sites:
                    for mac, _ in terms:
                        if mac != src.guard and kind in ("if", "ifdef", "ifndef", "elif"):
                            header_macros[mac].append((rel, n))
        for mac, sites in header_macros.items():
            sites = list(dict.fromkeys(sites))
            if mac in self.guards:
                out.extend(("guard skip", mac, rel, n) for rel, n in sites if self.guards[mac] != rel)
                continue
            kind = gate_kind(mac)
            users = [(f, n) for f, n, _ in all_defined.get(mac, ()) if not self.files[f].is_header
                     or f not in {r for r, _ in sites}]
            if not users or kind == "path override":
                continue
            out.extend((kind + " site", mac, rel, n) for rel, n in sites)
            out.extend((kind, mac, f, n) for f, n in users)
        for mac, defs in all_defined.items():
            if mac in self.guards:
                out.extend(("guard pre-define", mac, f, n) for f, n, _ in defs
                           if f != self.guards[mac] and not self.files[f].is_header)
            elif gate_kind(mac) == "path override":
                out.extend(("path override", f"{mac} {v}", f, n) for f, n, v in defs if not self.files[f].is_header)
        for rel, src in self.files.items():
            out.extend(("placeholder", name, rel, n) for name, n in placeholders(src))
        return sorted(out)

    def line_directives(self):
        return self.cached("lines", lambda: {rel: sum(1 for l in s.raw_lines if re.match(r"\s*#\s*line\b", l))
                                             for rel, s in self.files.items()})

    def padding(self):
        """[(header, class, member, array | accessor)]"""
        def build():
            out = []
            for rel, src in self.headers.items():
                for t in src.types:
                    for m in t.members:
                        if m.kind == "field" and PADDING.match(m.name):
                            out.append((rel, t.name, m.name, "array"))
                        elif m.kind == "method" and m.body and PADDING_REF.search(src.code[m.body[0]:m.body[1]]):
                            out.append((rel, t.name, m.name, "accessor"))
            return out
        return self.cached("padding", build)

    def summary(self):
        parts = self.partials()
        hdr = [x for x in parts if self.files[x[0]].is_header]
        cpp = [x for x in parts if not self.files[x[0]].is_header]
        foreign = [t for t, k in self.standins() if k == "foreign"]
        mech = self.mechanisms()
        return {
            "PARTIAL": len(parts), "hdr": len(hdr), "hdr files": len({x[0] for x in hdr}),
            "cpp": len(cpp), "cpp files": len({x[0] for x in cpp}),
            "MATCH": sum(1 for s in self.files.values() for _, t in s.comments if "MATCH:" in t),
            "foreign": len(foreign), "foreign files": len({t.file for t in foreign}),
            "foreign names": len({t.name for t in foreign}),
            "unattributed": sum(1 for _, k in self.standins() if k == "unattributed"),
            "own": sum(1 for _, k in self.standins() if k == "own"),
            "dups": len(self.duplicates()),
            "padding arrays": sum(1 for x in self.padding() if x[3] == "array"),
            "padding accessors": sum(1 for x in self.padding() if x[3] == "accessor"),
            "opt-out": Counter(m[1] for m in mech if m[0] == "opt-out" and not self.files[m[2]].is_header),
            "mechanisms": len(mech), "untagged": sum(1 for m in mech if not self.tagged(m[2], m[3])),
            "#line": sum(self.line_directives().values()),
            "classes": Counter(self.readiness(n)[0] for n in self.class_names()),
            "categories": Counter(x[2] for x in parts),
        }

    # ---- queue

    def queue(self):
        sti = defaultdict(list)
        for t, k in self.standins():
            if k == "foreign":
                sti[t.name].append(t)
        dups = self.duplicates()
        mech = self.mechanisms()
        site_header = {}
        for kind, key, rel, n in mech:
            if kind in ("full-class site", "feature site", "opt-out site"):
                site_header.setdefault(key, rel)
        gate_users, gate_macros = defaultdict(set), defaultdict(set)
        partitioned = {"TASK_H_NO_SLC_OVERFLOW_MSG"}  # a named partition item
        for kind, key, rel, n in mech:
            if kind in ("full-class", "feature", "opt-out") and key in site_header and key not in partitioned:
                gate_users[site_header[key]].add(rel)
                gate_macros[site_header[key]].add(key)
            elif kind in ("guard pre-define", "guard skip"):
                gate_users[self.guards[key]].add(rel)
                gate_macros[self.guards[key]].add(key)
        by_header = defaultdict(set)
        for name in set(sti) | set(dups):
            h = self.owner_header(name)
            if h:
                by_header[h].add(name)
        for h in gate_users:
            by_header.setdefault(h, set())
        items = []
        for header, names in by_header.items():
            names = sorted(names)
            states = {n: self.readiness(n)[0] for n in names}
            if names and all(s in ("missing", "unattributed") for s in states.values()):
                continue
            files = sorted({t.file for n in names for t in sti.get(n, ())})
            dup_h = sorted({d for n in names for d in dups.get(n, ()) if d != header})
            users = sorted(gate_users.get(header, ()))
            marks = sum(self.readiness(n)[1].get("partial_marks", 0) for n in names)
            owners = [unit_of(header)] if unit_of(header) in self.status else []
            owners += [self.owner(n) for n in names if self.owner(n)]
            owner = owners[0] if owners else unit_of(header)
            busy = next((o for o in owners if self.in_progress(o)), None)
            if header in self.headers:
                risk, why = self.risk(header, names, files, states, gate_macros.get(header, ()))
            else:
                risk, why = "R3", "new header from the ref"
            items.append({"item": header, "classes": names, "files": files, "dups": dup_h, "users": users,
                          "states": states, "marks": marks, "risk": risk, "why": why, "owner": owner,
                          "benefit": 2 * len(files) + 3 * len(dup_h) + len(users) + 0.5 * marks,
                          "deferred": f"deferred: {busy} hook" if busy else ""})
        items.extend(self.partition_items())
        items.sort(key=lambda i: (bool(i["deferred"]), i["risk"], -i["benefit"], i["item"]))
        return items

    def risk(self, header, names, files, states, macros=()):
        gated_strings = [mac for mac in macros if any(mac in {m for m, _ in terms}
                                                      for _, _, terms in self.files[header].literals())]
        if gated_strings:
            return "R2", "gated strings: " + fmt_list(sorted(gated_strings), 3)
        adds = []
        for f in files:
            st, missing = self.include_adds(self.files[f].unit, header)
            if missing:
                adds.append(f"{self.files[f].unit} ({len(missing)})")
        hint = self.weak_hint(header, files)
        if adds or hint:
            why = []
            if adds:
                why.append("adds strings: " + fmt_list(adds, 3))
            if hint:
                why.append("weak hint: " + fmt_list(hint, 2))
            return "R2", "; ".join(why)
        inline = any(m.kind == "method" and (m.body or m.virtual)
                     for n in names if self.home(n) for m in self.home(n).members)
        gated = "gated" in states.values() or bool(macros)
        if inline or gated:
            return "R1", ("inline bodies or virtuals" if inline else "") + (" (gate)" if gated else "")
        return "R0", ""

    def weak_hint(self, header, files):
        """Template instances with virtuals in the header whose weak functions an original includer lacks."""
        inst = set(re.findall(r"\b(\w+T)\s*<\s*(\w+)\s*>", self.files[header].code))
        out = []
        expected = ROOT / "build" / self.version / "expected" / "asm"
        for name, arg in sorted(inst):
            tmpl = [t for s in self.headers.values() for t in s.types if t.template and t.name == name]
            if not any(m.kind == "method" and m.virtual for t in tmpl for m in t.members):
                continue
            frag = f"t{len(name)}{name}1Z{len(arg)}{arg}"
            for f in files:
                asm = expected / f"{self.files[f].unit}.s"
                if asm.exists() and frag not in asm.read_text(encoding="utf-8", errors="replace"):
                    out.append(f"{name}<{arg}> not in {self.files[f].unit}")
                    break
        return out

    def partition_items(self):
        """The named partition fixes of the plan, while they are pending."""
        out = []
        foreign = [t for t, k in self.standins() if k == "foreign"]
        base = {"dups": [], "users": [], "states": {}, "marks": 0, "risk": "R3", "deferred": "", "files": []}
        if "System/RsrcDB.h" in self.headers and (self.owner_header("NAME_CASE") != "System/RsrcDB.h"
                                                  or "System/RsrcData.h" in self.closure("System/RsrcDB.h")):
            files = sorted({t.file for t in foreign if t.name in ("CRsrcDB", "NAME_CASE")})
            out.append({**base, "item": "partition: System/RsrcDB.h (NAME_CASE in, RsrcData.h/Task.h out)",
                        "classes": ["CRsrcDB", "NAME_CASE"], "files": files, "benefit": 2 * len(files),
                        "why": "partition change", "owner": "System/RsrcDB"})
        users = sorted({m[2] for m in self.mechanisms() if m[1] == "TASK_H_NO_SLC_OVERFLOW_MSG" and m[0] == "opt-out"})
        if users:
            out.append({**base, "item": "partition: task-slc-message (CTaskSLCManager to TaskMan.h)",
                        "classes": ["CTaskSLCManager"], "users": users, "benefit": len(users),
                        "why": "partition change", "owner": "System/TaskMan"})
        return out


# ---- reports


def fmt_list(items, limit=8):
    items = list(items)
    s = ", ".join(items[:limit])
    return s + (f", ... (+{len(items) - limit})" if len(items) > limit else "")


def detail_line(d):
    out = []
    for key, label in (("missing_fields", "missing fields"), ("missing_methods", "missing methods"),
                       ("param_mismatch", "parameter count differs"), ("padding", "padding"),
                       ("extra_fields", "extra fields"), ("extra_methods", "extra methods")):
        if d.get(key):
            out.append(f"{label}: {fmt_list(d[key], 6)}")
    if d and not d.get("order_ok", True):
        out.append("field order differs")
    if d and not d.get("vorder_ok", True):
        out.append("virtual order differs")
    if d.get("partial_marks"):
        out.append(f"{d['partial_marks']} PARTIAL")
    return "; ".join(out)


def summary_lines(s):
    return [
        f"PARTIAL: {s['PARTIAL']} ({s['hdr']} in {s['hdr files']} headers, {s['cpp']} in {s['cpp files']} .cpp); "
        f"MATCH: {s['MATCH']}",
        "  by category: " + ", ".join(f"{c} {s['categories'][c]}" for c in CATEGORIES if s["categories"].get(c)),
        f"stand-ins: {s['foreign']} foreign in {s['foreign files']} files ({s['foreign names']} names), "
        f"{s['unattributed']} unattributed, {s['own']} own",
        f"duplicate class names across headers: {s['dups']}",
        f"padding: {s['padding arrays']} arrays, {s['padding accessors']} accessors",
        f"mechanisms: {s['mechanisms']} sites ({s['untagged']} untagged); #line: {s['#line']}",
        "opt-out defines in .cpp: " + (", ".join(f"{k} {v}" for k, v in s["opt-out"].most_common()) or "none"),
        "classes: " + ", ".join(f"{r} {s['classes'].get(r, 0)}" for r in READINESS),
    ]


def queue_rows(p, n):
    rows = []
    for it in p.queue()[:n]:
        st = Counter(it["states"].values())
        counts = (f"{len(it['files'])} stand-in files, {len(it['dups'])} dup headers, {len(it['users'])} gate users"
                  + (f", {it['marks']} PARTIAL" if it["marks"] else ""))
        rows.append((it["risk"], it["benefit"], it["item"], it["deferred"] or "ready", counts,
                     ", ".join(f"{c} {k}" for k, c in st.items()), it["why"], fmt_list(it["classes"], 5)))
    return rows


def print_queue(p, n):
    for risk, benefit, item, state, counts, ready, why, classes in queue_rows(p, n):
        print(f"{risk} {benefit:6.1f}  {item}  [{state}]")
        print(f"            {counts}; classes: {classes}" + (f" ({ready})" if ready else "")
              + (f"; {why}" if why else ""))


def print_unit(p, unit):
    st = p.status.get(unit)
    rel = p.unit_source(unit)
    if st is None or rel is None:
        sys.exit(f"{unit}: no such unit with a source file")
    asm, parked, sd, _ = st
    order = p.plan_order()
    pos = f"plan #{order.index(unit) + 1}" if unit in order else "not in the plan's work table"
    print(f"{unit} ({rel}): {asm} INCLUDE_ASM ({parked} parked); SOURCE DATA: {' '.join(sorted(sd)) or 'none'}; {pos}")
    idx = {}
    for k, s in enumerate(p.originals.get(unit, [])):
        idx.setdefault(s, k)
    macros = p.macros(unit)
    print("\nIncludes (@ = index of the header's first string in the original .rodata):")
    positions = []
    for h in p.deps.get(unit, ()):
        first = min((idx[s] for s in p.header_strings(h, macros) if s in idx), default=None)
        if first is not None:
            positions.append((first, h))
        print(f"  {h}" + (f"  @{first}" if first is not None else ""))
    print("\nForeign stand-ins:")
    for t in p.sources[rel].types:
        if p.standin_kind(t) != "foreign":
            continue
        name, oh = t.name, p.owner_header(t.name)
        state, d = p.readiness(name)
        home = p.home(name)
        owner = p.owner(name)
        print(f"  {name} (line {t.line}) -> {oh} [{state}], owner {owner}"
              + (" (in progress)" if p.in_progress(owner) else ""))
        if home:
            have = {x.name for x in home.members}
            lacking = [m.name for m in t.members if m.kind in ("method", "field") and m.name not in have]
            if lacking:
                print(f"      owner lacks: {fmt_list(lacking)}")
        if oh in p.headers:
            st2, missing = p.include_adds(unit, oh)
            msg = {"ok": "strings ok", "adds": "adds strings", "n/a": "strings: .rodata still asm",
                   "no source": "strings: ?"}[st2]
            if missing:
                msg += (" (hidden conflict)" if st2 == "n/a" else "") + ": " + fmt_list((repr(s) for s in missing), 4)
            first = min((idx[s] for s in p.header_strings(oh, p.macros(unit, p.closure(oh))) if s in idx),
                        default=None)
            if first is not None:
                after = [h for f, h in sorted(positions) if f < first]
                msg += f"; its strings come after {after[-1]}'s" if after else "; its strings come first"
            print(f"      {msg}")
    un = [t.name for t, k in p.standins() if k == "unattributed" and t.file == rel]
    print("\nUnattributed stand-ins: " + (fmt_list(un) or "none"))
    own = sorted({n for n, ts in p.ref.items() if any(o == unit for _, o in ts)})
    print("\nOwn classes (ref owner = this unit):")
    for n in own:
        state, d = p.readiness(n)
        print(f"  {n}: {state}" + (f" ({detail_line(d)})" if detail_line(d) else ""))
    marks = Counter(f for f, _, _, u, _ in p.partials() if u == unit)
    print("\nPARTIAL 'for this unit' elsewhere: " + (fmt_list(f"{f} ({c})" for f, c in marks.most_common()) or "none"))
    mech = [m for m in p.mechanisms() if m[2] == rel]
    print("Mechanisms in its source: " + (fmt_list(f"{k} {key} (line {n})" for k, key, _, n in mech) or "none"))


def print_header(p, header):
    header = re.sub(r"^src/", "", header)
    src = p.headers.get(header)
    if src is None:
        sys.exit(f"{header}: not a header under src/")
    marks = [x for x in p.partials() if x[0] == header]
    print(f"{header}: guard {src.guard}; {len(marks)} PARTIAL; {p.line_directives()[header]} #line")
    sti = defaultdict(list)
    for t, k in p.standins():
        if k == "foreign":
            sti[t.name].append(t.file)
    dups = p.duplicates()
    print("\nClasses:")
    for t in src.types:
        if t.template:
            print(f"  {t.name} (template, line {t.line})")
            continue
        state, d = p.readiness(t.name)
        home = p.home(t.name)
        where = "" if home is t else f" (home: {home.file}:{home.line})" if home else ""
        print(f"  {t.name} (line {t.line}): {state}{where}" + (f" - {detail_line(d)}" if detail_line(d) else ""))
        if sti.get(t.name) and home is t:
            print(f"      stand-ins: {fmt_list(sti[t.name])}")
        if t.name in dups:
            print(f"      also defined in: {fmt_list(h for h in dups[t.name] if h != header)}")
    keys = {m[1] for m in p.mechanisms() if m[2] == header}
    mech = [m for m in p.mechanisms() if m[2] == header or (m[1] in keys and not m[0].endswith(" site"))]
    if mech:
        print("\nMechanisms:")
        for kind, key, rel, n in mech:
            tag = p.tagged(rel, n)
            print(f"  {kind} {key}: {rel}:{n}" + (f"  MATCH: {tag}" if tag else "  untagged"))
    incl = p.includers(header)
    counts, adds, hidden = Counter(), [], []
    for u in incl:
        st, missing = p.string_status(u, [header])
        counts[st] += 1
        if st == "adds":
            adds.append(f"{u} ({fmt_list((repr(s) for s in missing), 3)})")
        elif st == "n/a" and missing:
            hidden.append(f"{u} ({len(missing)})")
    print(f"\nIncluders: {len(incl)}; strings ok {counts['ok']}, adds strings {counts['adds']}, "
          f".rodata asm {counts['n/a']} (hidden conflicts {len(hidden)})")
    for a in adds:
        print(f"  adds: {a}")
    if hidden:
        print(f"  hidden conflicts: {fmt_list(hidden, 20)}")


def print_class(p, name):
    name = last(name)
    state, d = p.readiness(name)
    print(f"{name}: {state}; ref owner {p.ref_owner(name) or '(lost+found or none)'}; home {p.owner_header(name)}")
    if detail_line(d):
        print(f"  vs ref ({d.get('ref_file')}): {detail_line(d)}")
    defs = p.defs.get(name, [])
    hs = [t for t in defs if t.src.is_header]
    cs = [t for t in defs if not t.src.is_header]
    print(f"\nHeader definitions ({len(hs)}):")
    for t in hs:
        gate = " gated by " + ", ".join(("" if dd else "!") + m for m, dd in t.gate) if t.gate else ""
        print(f"  {t.file}:{t.line}{gate}")
    print(f"\n.cpp definitions ({len(cs)} in {len({t.file for t in cs})} files):")
    for t in cs:
        print(f"  {t.file}:{t.line} [{p.standin_kind(t)}]")
    for t, o in p.ref.get(name, ()):
        print(f"\nref: {t.file}:{t.line} ({o or 'lost+found'})")


def string_rows(p):
    rows = []
    with_src = {u for u, st in p.status.items() if st[3]}
    for h in sorted(p.headers):
        orig = p.orig_with(h)
        if orig is None:
            continue
        ours = set(p.includers(h))
        only_o = sorted(set(orig) & with_src - ours)
        only_u = sorted(ours - set(orig))
        rows.append((h, len(orig), len(ours), only_o, only_u))
    return rows


def print_strings(p, header=None):
    if not header:
        for h, no, nu, oo, ou in string_rows(p):
            print(f"{h}: originals {no}, ours {nu}, only originals {len(oo)}, only ours {len(ou)}")
        return
    header = re.sub(r"^src/", "", header)
    if header not in p.headers:
        sys.exit(f"{header}: not a header under src/")
    orig = p.orig_with(header) or []
    ours = p.includers(header)
    with_src = {u for u, st in p.status.items() if st[3]}
    print(f"{header}: fingerprint {fmt_list(sorted(p.fingerprint(header)), 6) or '(none: no literal names it)'}")
    expected = ROOT / "build" / p.version / "expected" / "asm" / "data"
    fp = p.fingerprint(header)
    asciz = [u for u in orig if any(norm(m) in fp for m in re.findall(
        r'\.asciz "((?:\\.|[^"\\])*)"', (expected / f"{u}.rodata.s").read_text(encoding="utf-8")))]
    print(f"originals with it ({len(orig)}; {len(asciz)} as .asciz in the expected .rodata.s, "
          f"{len(orig) - len(asciz)} only in the image's bytes): {fmt_list(orig, 300)}")
    if len(asciz) < len(orig):
        print(f"  only in the bytes (splat wrote .word): {fmt_list(sorted(set(orig) - set(asciz)), 300)}")
    print(f"our includers ({len(ours)}): {fmt_list(ours, 300)}")
    print(f"only in originals (units with source): {fmt_list(sorted(set(orig) & with_src - set(ours)), 300)}")
    print(f"only ours: {fmt_list(sorted(set(ours) - set(orig)), 300)}")


def md(text):
    return str(text).replace("|", "\\|")


def markdown(p):
    s = p.summary()
    out = ["## Summary", "", "```", *summary_lines(s), "```", "",
           "## Queue (top 20)", "",
           "Risk: R0 no strings, inlines or virtuals; R1 inline bodies or virtuals; R2 adds strings to an includer or",
           "the weak hint fires; R3 partition change. Benefit = 2 x stand-in files + 3 x duplicate headers + gate",
           "users + 0.5 x the header's PARTIAL. Items whose owner unit is in progress are deferred to its hook.", "",
           "| risk | benefit | item | state | counts | classes | why |", "|---|---:|---|---|---|---|---|"]
    for risk, benefit, item, state, counts, ready, why, classes in queue_rows(p, 20):
        out.append(f"| {risk} | {benefit:.1f} | `{item}` | {state} | {counts} | {md(classes)} ({ready}) | {md(why)} |")
    sti = defaultdict(set)
    for t, k in p.standins():
        if k == "foreign":
            sti[t.name].add(t.file)
    dups = p.duplicates()
    out += ["", "## Classes with foreign copies", "",
            "| class | readiness | home | stand-in files | also defined in | gaps vs ref |", "|---|---|---|---:|---|---|"]
    for n in sorted(set(sti) | set(dups), key=lambda n: (-len(sti.get(n, ())), n)):
        state, d = p.readiness(n)
        home = p.owner_header(n) or "-"
        others = [h for h in dups.get(n, []) if h != home]
        out.append(f"| `{n}` | {state} | {home} | {len(sti.get(n, ()))} | {', '.join(others)} | "
                   f"{md(detail_line(d))[:220]} |")
    marks = Counter(x[0] for x in p.partials())
    out += ["", "## Headers", "",
            "| header | PARTIAL | classes complete/superset/partial/gated | includers |", "|---|---:|---|---:|"]
    for h in sorted(p.headers):
        st = Counter(p.readiness(t.name)[0] for t in p.headers[h].types if not t.template and p.home(t.name) is t)
        if marks.get(h) or st["superset"] or st["partial"] or st["gated"]:
            out.append(f"| `{h}` | {marks.get(h, 0)} | {st['complete']}/{st['superset']}/{st['partial']}/"
                       f"{st['gated']} | {len(p.includers(h))} |")
    groups = defaultdict(list)
    for kind, key, rel, n in p.mechanisms():
        if kind == "placeholder":
            key = re.sub(r"\d+$", "", key) + "*"
        elif kind == "path override":
            key = key.split()[0]
        groups[(kind, key)].append((rel, n))
    out += ["", "## Mechanisms", "",
            "Sites per mechanism (placeholders grouped by name, path overrides by macro), with the `MATCH:` reason",
            "or `untagged`.", "",
            "| kind | key | sites | tagged | reason, or an untagged site |", "|---|---|---:|---:|---|"]
    for (kind, key), sites in sorted(groups.items()):
        tags = [p.tagged(r, n) for r, n in sites]
        reason = next((t for t in tags if t), None)
        ex = reason or f"untagged, e.g. {sites[0][0]}:{sites[0][1]}"
        out.append(f"| {kind} | `{md(key)}` | {len(sites)} | {sum(1 for t in tags if t)} | {md(ex)[:160]} |")
    out += ["", "## String evidence", "",
            "Headers whose fingerprint (their literals that name the header) is in another set of original units",
            "than our includers (units with a source only). A heuristic: stand-ins and opt-outs show up here.", "",
            "| header | originals | ours | only originals | only ours |", "|---|---:|---:|---|---|"]
    for h, no, nu, oo, ou in string_rows(p):
        if oo or ou:
            out.append(f"| `{h}` | {no} | {nu} | {len(oo)}: {fmt_list(oo, 4)} | {len(ou)}: {fmt_list(ou, 4)} |")
    out += ["", "Generated by `tools/partials.py --update docs/partials.md`."]
    return "\n".join(out)


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--summary", action="store_true", help="only the counts")
    parser.add_argument("--update", metavar="FILE", help="replace the text between the markers in FILE")
    parser.add_argument("--unit", help="start-hook view of one unit")
    parser.add_argument("--header", help="finish-hook view of one header")
    parser.add_argument("--class", dest="cls", metavar="NAME", help="every definition and stand-in of one class")
    parser.add_argument("--queue", action="store_true", help="the between-units queue")
    parser.add_argument("-n", type=int, default=20, help="queue length (default 20)")
    parser.add_argument("--strings", nargs="?", const="", default=None, metavar="HEADER",
                        help="header string sets vs the originals' fingerprints")
    args = parser.parse_args()
    p = Project()
    if args.unit:
        print_unit(p, args.unit)
    elif args.header:
        print_header(p, args.header)
    elif args.cls:
        print_class(p, args.cls)
    elif args.queue:
        print_queue(p, args.n)
    elif args.strings is not None:
        print_strings(p, args.strings or None)
    elif args.update:
        path = Path(args.update)
        text = path.read_text(encoding="utf-8")
        if START not in text or END not in text:
            sys.exit(f"{path}: add the markers {START} and {END} where the report should go")
        head, rest = text.split(START, 1)
        _, tail = rest.split(END, 1)
        path.write_text(f"{head}{START}\n{markdown(p)}\n{END}{tail}", encoding="utf-8")
        print(f"updated {path}")
    elif args.summary:
        print("\n".join(summary_lines(p.summary())))
    else:
        print("\n".join(summary_lines(p.summary())))
        print()
        print(markdown(p))


if __name__ == "__main__":
    main()
