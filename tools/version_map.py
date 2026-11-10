#!/usr/bin/env python3
"""Map the beta ELF's functions and data onto another version of the game (docs/retail-plan.md, Phase 0).

The other version (the US retail ELF) is stripped, but carries SN Systems' DLL table (`sn_dll_header_root`): names
with that version's addresses. This pairs the beta's functions with the other version's:

1. by name, from the two DLL tables (anonymous-namespace names `_N_GLOBAL_$N$<file>XXXXXX$...` pair by the name
   without the per-compile 6-character suffix, static constructors `_GLOBAL_$I$...`/`$D$` likewise);
2. by the call graph: in every paired function, the `jal` targets at the same positions (identical instructions, or
   the equal blocks of an instruction alignment) pair the callees, also the unnamed (file-local) ones;
3. by masked search: an unpaired beta function whose masked instructions occur exactly once in the gap between its
   paired neighbours (link order is kept).

"Masked" compares instructions with their address fields cleared: `j`/`jal` targets, `lui` immediates and the
immediates of instructions based on a register loaded by `lui` (or on $gp). Every paired function is then compared:
`same` (masked-identical, sizes equal) or `differs`, and the differing game functions are classified against the
beta's debug calls (AssertApp, com_malloc_d, TRACE*, printf, ...):

- `debug`:  every beta-only block of the alignment holds a debug call of the unit's own file (asserts, debug
            allocations, prints compiled out), branch offsets ignored;
- `header`: as `debug`, but at least one removed call is an assert of a header (an inlined header function);
- `regs`:   the rest of the instructions match once registers are masked too (allocation/scheduling changes only);
- `other`:  anything else (real retail changes, or a different shape of the debug forms).

Data: in identical or aligned code, the full `lui`+`addiu`/load/store addresses at the same positions pair beta
data addresses with retail ones.

Output (build/<VERSION>/version_map/): functions.tsv (beta address, size, name, unit, retail address, retail size,
how paired, status, class, removed debug sites), data.tsv (beta address, beta symbol, retail address, evidence count,
conflicts), units.tsv (per unit: functions, same, differs, missing, retail range); `--report docs/retail.md` writes the
summary.

Usage: .venv/bin/python tools/version_map.py [--other iso/SLUS_217.99_retail] [--report docs/retail.md]
"""

import argparse
import bisect
import difflib
import re
import struct
from collections import Counter, defaultdict
from pathlib import Path

from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parent.parent
BETA = "SLUS_217.99"
DEBUG_FUNCS = {"AssertApp", "com_malloc_d", "com_memalign_d", "mspace_malloc_d", "TRACE", "TRACE_COL", "TRACE_USER", "TRACE_USER_COL",
               "printf", "scePrintf", "kprintf", "sceVprintf", "vprintf", "puts"}
ANON = re.compile(r"(?:_\d+)?_GLOBAL_\$([NID])\$(.*?\.(?:cpp|c|s))[0-9A-Za-z_]{6}(\$|$)")


def anon_key(name: str) -> str:
    return ANON.sub(lambda m: f"<{m.group(1)}:{m.group(2)}>{m.group(3)}", name)


# ---------------------------------------------------------------------------------------------------------------
class Image:
    def __init__(self, path: Path):
        self.path = path
        self.data = path.read_bytes()
        with path.open("rb") as f:
            elf = ELFFile(f)
            self.segs = [(p["p_vaddr"], p["p_offset"], p["p_filesz"]) for p in elf.iter_segments()
                         if p["p_type"] == "PT_LOAD"]
            self.sections = {s.name: (s["sh_addr"], s["sh_size"]) for s in elf.iter_sections() if s["sh_addr"]}
        a, n = self.sections[".text"]
        self.text = (a, a + n)

    def off(self, addr: int) -> int:
        for v, o, n in self.segs:
            if v <= addr < v + n:
                return o + addr - v
        raise KeyError(hex(addr))

    def word(self, addr: int) -> int:
        return struct.unpack_from("<I", self.data, self.off(addr))[0]

    def words(self, addr: int, size: int) -> list[int]:
        o = self.off(addr)
        return list(struct.unpack_from(f"<{size // 4}I", self.data, o))

    def cstr(self, addr: int) -> str | None:
        try:
            o = self.off(addr)
        except KeyError:
            return None
        e = self.data.find(b"\0", o)
        return self.data[o:e].decode("latin-1") if 0 <= e - o < 256 else None

    def dll_table(self) -> list[tuple[str, int, int]]:
        i = self.data.find(b"SNR2")
        root = next(v + (i - o) for v, o, n in self.segs if o <= i < o + n)
        syms, nsyms = self.word(root + 12), self.word(root + 16)
        out = []
        for k in range(nsyms):
            e = syms + 12 * k
            out.append((self.cstr(self.word(e)), self.word(e + 4), self.data[self.off(e + 10)]))
        return out


# ---------------------------------------------------------------------------------------------------------------
LOADSTORE = set(range(0x20, 0x40)) | {0x1A, 0x1B, 0x1E, 0x1F, 0x31, 0x35, 0x39, 0x3D}  # incl. ldl/ldr/lq/sq, lwc1..


def decode(words: list[int], base: int):
    """Per instruction: (masked word, word with registers masked too, jal target or None, data address or None,
    branch flag). Branch offsets stay in the masked word; `loose` clears them as well."""
    hi = {}  # register -> value of its lui
    luiregs = {28}
    out = []
    for w in words:
        op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
        imm = w & 0xFFFF
        simm = imm - 0x10000 if imm & 0x8000 else imm
        target = addr = None
        branch = False
        m = w
        if op in (2, 3):
            m = w & 0xFC000000
            if op == 3:
                target = ((base & 0xF0000000) | ((w & 0x3FFFFFF) << 2))
        elif op == 15:  # lui
            m = w & 0xFFFF0000
            hi[rt] = imm << 16
            luiregs.add(rt)
        elif op in (4, 5, 6, 7, 0x14, 0x15, 0x16, 0x17) or (op == 1 and rt in (0, 1, 2, 3, 0x10, 0x11, 0x12, 0x13)) \
                or (op == 0x11 and rs == 8):
            branch = True
        elif rs in luiregs and (op in LOADSTORE or op in (8, 9, 13)):
            m = w & 0xFFFF0000
            if rs in hi and op != 13:
                addr = (hi[rs] + simm) & 0xFFFFFFFF
        if op == 0 and (w & 0x3F) in (0x08, 0x09):
            pass
        loose = m & 0xFC00003F if op == 0 else (m & 0xFC000000 if branch or op in (2, 3) else
                                                (m & 0xFC00FFFF) if op not in (0x10, 0x11, 0x12, 0x1C) else m & 0xFE00003F)
        out.append((m, loose, target, addr, branch))
        base += 4
    return out


def class_of(name: str) -> str | None:
    """The class of a GCC 2 mangled member function name (`method__5CTaski`, `_$_5CTask`, `__5CTask`, nested
    `__Q22PL7CPlayer...`)."""
    m = re.match(r"^(?:_\$_|.+?__)(Q(\d)(.*)|(\d+)(.*))$", name)
    if not m:
        return None
    if m.group(4):
        n = int(m.group(4))
        return m.group(5)[:n] if len(m.group(5)) >= n else None
    count, rest, parts = int(m.group(2)), m.group(3), []
    for _ in range(count):
        mm = re.match(r"(\d+)", rest)
        if not mm:
            return None
        n = int(mm.group(1))
        parts.append(rest[len(mm.group(1)):len(mm.group(1)) + n])
        rest = rest[len(mm.group(1)) + n:]
    return "::".join(parts)


# ---------------------------------------------------------------------------------------------------------------
def load_beta_symbols():
    funcs, data = {}, {}
    text = (ROOT / f"config/{BETA}/elf_symbol_addrs.txt").read_text(encoding="utf-8")
    for m in re.finditer(r"^(\S+) = 0x([0-9A-F]+);(?: // (type:func)?\s*(?:size:0x([0-9A-F]+))?)?", text, re.M):
        name, addr = m.group(1), int(m.group(2), 16)
        size = int(m.group(4), 16) if m.group(4) else 0
        (funcs if m.group(3) else data)[addr] = (name, size)
    return funcs, data


def load_units():
    units = []
    y = (ROOT / f"config/{BETA}/{BETA}.yaml").read_text(encoding="utf-8")
    block = y.split("# begin .text")[1].split("# end .text")[0]
    for m in re.finditer(r"- \[0x([0-9A-F]+), \w+, ([^\]]+)\]", block):
        units.append((0x100000 + int(m.group(1), 16), m.group(2).strip()))
    return units


class Mapper:
    def __init__(self, beta: Image, other: Image):
        self.beta, self.other = beta, other
        self.funcs, self.bdata = load_beta_symbols()
        self.units = load_units()
        self.ustarts = [u[0] for u in self.units]
        self.fstarts = sorted(self.funcs)
        self.pair: dict[int, int] = {}  # beta function -> other address
        self.how: dict[int, str] = {}
        self.otab = other.dll_table()
        self.btab = beta.dll_table()
        self.bynamebeta = {n: a for a, (n, s) in self.funcs.items()}
        self.debug_addrs = {a for a, (n, s) in self.funcs.items() if n in DEBUG_FUNCS}
        # other version's code addresses known as function starts (named in its table, or found)
        self.ostarts: set[int] = set()
        lo, hi = other.text
        for n, v, t in self.otab:
            if lo <= v < hi and t in (0, 2, 3):
                self.ostarts.add(v)
        self.data_pairs: dict[int, Counter] = defaultdict(Counter)
        # data addresses: from the end of .text to the end of the last section (.bss)
        self.beta_end = max(a + n for a, n in beta.sections.values())
        self.other_end_addr = max(a + n for a, n in other.sections.values())
        # likely function starts in the other version: after `jr $ra`/`j` and its delay slot, either alignment
        # padding or a frame prologue (`addiu $sp, $sp, -N`) follows
        ws = other.words(lo, hi - lo)
        self.cands = set()
        for k in range(2, len(ws)):
            if ws[k - 2] == 0x03E00008 or ws[k - 2] >> 26 == 2:
                j = k
                while j < len(ws) and ws[j] == 0:
                    j += 1
                if j < len(ws) and (j > k or (ws[j] >> 16 == 0x27BD and ws[j] & 0x8000)):
                    self.cands.add(lo + 4 * j)
        self.absent: set[int] = set()
        self.call_conflicts = 0
        self.layout: dict[str, Counter] = defaultdict(Counter)

    def unit_of(self, addr: int) -> str:
        return self.units[bisect.bisect_right(self.ustarts, addr) - 1][1]

    # -- 1: names --------------------------------------------------------------------------------------------
    def by_name(self):
        lo, hi = self.other.text
        exact = {n: v for n, v, t in self.otab if lo <= v < hi}
        keyed = defaultdict(list)
        for n, v, t in self.otab:
            if lo <= v < hi:
                keyed[anon_key(n)].append(v)
        for a, (n, s) in self.funcs.items():
            if n in exact:
                self.pair[a], self.how[a] = exact[n], "name"
            else:
                k = anon_key(n)
                if k != n and len(keyed.get(k, ())) == 1:
                    self.pair[a], self.how[a] = keyed[k][0], "anon"

    # -- sizes in the other version: up to the next known start ----------------------------------------------
    def other_end(self, addr: int) -> int:
        starts = self._sorted_ostarts
        i = bisect.bisect_right(starts, addr)
        return starts[i] if i < len(starts) else self.other.text[1]

    def refresh(self):
        self.ostarts |= set(self.pair.values())
        self._sorted_ostarts = sorted(self.ostarts | self.cands)
        self._sorted_known = sorted(self.ostarts)

    def body(self, img: Image, addr: int, size: int):
        return decode(img.words(addr, size), addr)

    def other_size(self, a: int) -> int:
        """Size of the other version's function paired with beta function a: the beta size when the masked bodies
        are identical, else up to the next known start (trailing alignment nops dropped)."""
        o = self.pair[a]
        bs = self.funcs[a][1]
        known = self._sorted_known
        i = bisect.bisect_right(known, o)
        if (known[i] if i < len(known) else self.other.text[1]) - o >= bs:
            b = [x[0] for x in self.body(self.beta, a, bs)]
            r = [x[0] for x in self.body(self.other, o, bs)]
            if b == r:
                return bs
        end = self.other_end(o)
        size = end - o
        ws = self.other.words(o, size)
        # alignment padding: zero words after the last `jr $ra`/`j` and its delay slot
        last = max((k for k, w in enumerate(ws) if w == 0x03E00008 or w >> 26 == 2), default=None)
        if last is not None and last + 2 < len(ws) and not any(ws[last + 2:]):
            size = 4 * (last + 2)
        return size

    # -- 2: call graph and data ------------------------------------------------------------------------------
    def align(self, a: int):
        """Equal blocks between the beta function and its pair: list of (beta insn index, other insn index, n),
        with the decoded bodies."""
        bs = self.funcs[a][1]
        o = self.pair[a]
        b = self.body(self.beta, a, bs)
        r = self.body(self.other, o, self.other_size(a))
        if [x[0] for x in b] == [x[0] for x in r]:
            return b, r, [(0, 0, len(b))]
        sm = difflib.SequenceMatcher(None, [x[0] for x in b], [x[0] for x in r], autojunk=False)
        return b, r, [blk for blk in sm.get_matching_blocks() if blk[2]]

    def propagate(self) -> int:
        new = 0
        votes: dict[int, Counter] = defaultdict(Counter)
        for a in list(self.pair):
            b, r, blocks = self.align(a)
            for i, j, n in blocks:
                for k in range(n):
                    bt, rt = b[i + k][2], r[j + k][2]
                    if bt is not None and rt is not None and bt in self.funcs:
                        votes[bt][rt] += 1
                    ba, ra = b[i + k][3], r[j + k][3]
                    if ba is not None and ra is not None and self.beta.text[1] <= ba < self.beta_end and \
                            self.other.text[1] <= ra < self.other_end_addr:
                        self.data_pairs[ba][ra] += 1
        taken = set(self.pair.values())
        for bt, c in votes.items():
            rt, _ = c.most_common(1)[0]
            if bt not in self.pair:
                if len(c) == 1 and rt not in taken:
                    self.pair[bt], self.how[bt] = rt, "call"
                    new += 1
            elif self.pair[bt] != rt and len(c) == 1:
                self.call_conflicts += 1
        return new

    # -- 3: masked search between paired neighbours ------------------------------------------------------------
    def search(self) -> int:
        new = 0
        paired = sorted(self.pair)
        for a in self.fstarts:
            if a in self.pair or not self.funcs[a][1]:
                continue
            i = bisect.bisect_left(paired, a)
            lo = self.pair[paired[i - 1]] if i else self.other.text[0]
            hi = self.pair[paired[i]] if i < len(paired) else self.other.text[1]
            if hi <= lo or hi - lo > 0x40000:
                continue
            size = self.funcs[a][1]
            pat = [x[0] for x in self.body(self.beta, a, size)]
            n = size // 4
            hay = [x[0] for x in self.body(self.other, lo, hi - lo)]
            hits = [k for k in range(0, len(hay) - n + 1) if hay[k] == pat[0] and hay[k:k + n] == pat]
            if len(hits) == 1:
                self.pair[a], self.how[a] = lo + 4 * hits[0], "search"
                new += 1
        return new

    # -- 4: gaps: unpaired beta functions vs unpaired code between paired neighbours ---------------------------
    def gaps(self) -> int:
        new = 0
        paired = sorted(self.pair)
        taken = set(self.pair.values())
        cands = sorted(self.cands | self.ostarts)
        for A, B in zip(paired, paired[1:]):
            ra, rb = self.pair[A], self.pair[B]
            if rb <= ra:
                continue
            U = [f for f in self.fstarts[bisect.bisect_right(self.fstarts, A):bisect.bisect_left(self.fstarts, B)]
                 if self.funcs[f][1] and f not in self.pair]
            if not U:
                continue
            G = [c for c in cands[bisect.bisect_right(cands, ra):bisect.bisect_left(cands, rb)] if c not in taken]
            if not G:
                self.absent.update(U)
            elif len(G) == len(U):
                ends = G[1:] + [rb]
                sizes = [e - c for c, e in zip(G, ends)]
                if all(abs(sz - self.funcs[f][1]) <= max(0x40, self.funcs[f][1] // 2) for f, sz in zip(U, sizes)):
                    for f, c in zip(U, G):
                        self.pair[f], self.how[f] = c, "gap"
                        new += 1
        return new

    # -- classification ---------------------------------------------------------------------------------------
    def classify(self, a: int):
        """'same', or the kinds of the differing blocks of an alignment (branch offsets ignored): `debug` (the beta
        block calls a debug function), `offset` (same instructions, other immediates: struct/vtable/stack offsets,
        constants), `regs` (same instructions with other registers), `other`; plus whether the call sequences agree
        once the beta's debug calls are dropped (com_malloc_d counts as the other version's com_malloc)."""
        bs = self.funcs[a][1]
        o = self.pair[a]
        b = self.body(self.beta, a, bs)
        r = self.body(self.other, o, self.other_size(a))
        if [x[0] for x in b] == [x[0] for x in r]:
            return "same", set(), [], True
        kinds = set()
        if len(r) <= 2 < len(b):
            kinds.add("stub")  # the other version's body is `jr $ra` + delay slot: compiled out
        # nops (assembler hazard padding, empty delay slots) compared apart
        nb, nr = sum(1 for x in b if x[0] == 0), sum(1 for x in r if x[0] == 0)
        if nb != nr:
            kinds.add("nops")
            b, r = [x for x in b if x[0]], [x for x in r if x[0]]
        key = lambda x: x[0] & 0xFFFF0000 if x[4] else x[0]
        sm = difflib.SequenceMatcher(None, [key(x) for x in b], [key(x) for x in r], autojunk=False)
        for tag, i1, i2, j1, j2 in sm.get_opcodes():
            if tag == "equal":
                continue
            if any(b[k][2] in self.debug_addrs for k in range(i1, min(i2 + 1, len(b)))):
                kinds.add("debug")
            elif i2 - i1 == j2 - j1 and all(b[i1 + k][0] & 0xFFFF0000 == r[j1 + k][0] & 0xFFFF0000
                                             for k in range(i2 - i1)):
                kinds.add("offset")
                self.layout_votes(a, b[i1:i2], r[j1:j2])
            elif i2 - i1 == j2 - j1 and all(b[i1 + k][1] == r[j1 + k][1] for k in range(i2 - i1)):
                kinds.add("regs")
            else:
                kinds.add("other")
        sites = [(self.funcs[x[2]][0], self.arg_string(b, k)) for k, x in enumerate(b) if x[2] in self.debug_addrs]
        bcalls = []
        for x in b:
            t = x[2]
            if t is None:
                continue
            if t in self.debug_addrs:
                if self.funcs[t][0].endswith("_d"):
                    t = self.bynamebeta[self.funcs[t][0][:-2]]
                else:
                    continue
            bcalls.append(t)
        rcalls = [x[2] for x in r if x[2] is not None]
        if [self.pair.get(t) for t in bcalls] == rcalls:
            return "differs", kinds, sites, True
        # calls that differ: explained when every callee missing in the other version is debug code (calls a debug
        # function: no longer called but inlined, or gone) or not in the other version at all, and every added one
        # is an allocation function (com_malloc_d -> com_malloc)
        missing = Counter(self.pair.get(t, ("beta", t)) for t in bcalls) - Counter(rcalls)
        added = Counter(rcalls) - Counter(self.pair.get(t, ("beta", t)) for t in bcalls)
        inv = {v: k for k, v in self.pair.items()}
        ok = all((t[1] if isinstance(t, tuple) else inv.get(t)) in self.debugish for t in missing) and \
            all(inv.get(t) in self.alloc_addrs for t in added)
        return "differs", kinds, sites, ("inline" if ok else False)

    def layout_votes(self, a, bb, rr):
        """Member offset changes (load/store/addiu on a register that isn't $sp, $gp or an address) of the class
        the function belongs to."""
        cls = class_of(self.funcs[a][0])
        if not cls:
            return
        for x, y in zip(bb, rr):
            w, v = x[0], y[0]
            op, base = w >> 26, (w >> 21) & 31
            if w != v and (op in LOADSTORE or op == 9) and base not in (28, 29) and (w & 0xFFFF) and \
                    (w & 0xFFFF) < 0x2000 and (v & 0xFFFF) < 0x2000:
                self.layout[cls][(w & 0xFFFF, v & 0xFFFF)] += 1

    def category(self, kinds: set[str], sites, calls_same: bool, unit: str) -> str:
        """The plan's categories. Same calls once the debug calls are dropped: (a) `debug` (the beta has debug
        calls of the unit's own file), (b) `header` (one of them an inlined header assert), `layout` (only offsets
        and registers differ, no debug calls), `regs` (only registers), `shape` (no debug calls, the same calls,
        other instruction differences), `nops` (only the number of nops differs), `stub` (the other version's body
        is empty), `inline` (calls of debug code - functions that call a debug function - are
        missing: inlined or gone). (c) `calls`: the call sequences differ otherwise."""
        if "stub" in kinds:
            return "stub"
        if calls_same == "inline":
            return "inline"
        if not calls_same:
            return "calls"
        if sites:
            unit_file = unit.split("/")[-1] + "."
            if any(c == "AssertApp" and f and "/" in f and unit_file not in f.split("/")[-1] + "." for c, f in sites):
                return "header"
            return "debug"
        if kinds <= {"offset", "regs", "nops"}:
            return "layout" if "offset" in kinds else ("regs" if "regs" in kinds else "nops")
        return "shape"

    def arg_string(self, b, k) -> str | None:
        """The file string of an AssertApp/com_malloc_d call: the address put into $a0 (assert) or $a1 (alloc)
        before the call at index k (or in its delay slot)."""
        reg = 4 if self.funcs[b[k][2]][0] == "AssertApp" else 5
        for j in range(min(k + 1, len(b) - 1), max(-1, k - 12), -1):
            w = self.beta_word_cache(b, j)
            op, rt = w >> 26, (w >> 16) & 31
            if op == 9 and rt == reg and b[j][3] is not None:
                s = self.beta.cstr(b[j][3])
                if s and "(" in s and reg == 5:
                    s = s.split("(")[0]
                return s
        return None

    def beta_word_cache(self, b, j):
        return b[j][0] | 0  # masked word keeps opcode/registers; the address comes from b[j][3]


# ---------------------------------------------------------------------------------------------------------------
def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--beta", default=f"iso/{BETA}")
    ap.add_argument("--other", default="iso/SLUS_217.99_retail")
    ap.add_argument("--out", default=None, help="output directory (default build/<other>/version_map)")
    ap.add_argument("--report", default=None, help="write the summary (Markdown) here")
    args = ap.parse_args()

    beta, other = Image(ROOT / args.beta), Image(ROOT / args.other)
    out = Path(args.out) if args.out else ROOT / "build" / Path(args.other).name / "version_map"
    out.mkdir(parents=True, exist_ok=True)
    m = Mapper(beta, other)
    m.by_name()
    print(f"by name: {sum(1 for h in m.how.values() if h == 'name')}, by anonymous key: "
          f"{sum(1 for h in m.how.values() if h == 'anon')} of {len(m.funcs)} beta functions")
    m.refresh()
    for rnd in range(6):
        n1 = m.propagate()
        m.refresh()
        n2 = m.search()
        m.refresh()
        print(f"round {rnd + 1}: +{n1} by calls, +{n2} by search (paired {len(m.pair)}; call conflicts "
              f"{m.call_conflicts})")
        m.call_conflicts = 0
        if not n1 and not n2:
            break

    n3 = m.gaps()
    m.refresh()
    n1 = m.propagate()
    m.refresh()
    print(f"gaps: +{n3}, then +{n1} by calls; absent (no code between the paired neighbours): "
          f"{len(m.absent - set(m.pair))}")

    # debug code: the debug functions and the functions the other version doesn't have, and their callers
    m.alloc_addrs = {m.bynamebeta[n] for n in ("com_malloc", "com_memalign", "mspace_malloc") if n in m.bynamebeta}
    m.debugish = set(m.debug_addrs) | {a for a in m.fstarts if a not in m.pair}
    level1 = set(m.debugish)
    for a, (n, size) in m.funcs.items():
        if size and any(x[2] in level1 for x in m.body(beta, a, size)):
            m.debugish.add(a)

    # order check
    pairs = sorted(m.pair.items())
    inc = sum(1 for (_, x), (_, y) in zip(pairs, pairs[1:]) if y > x)
    print(f"order kept for {inc} of {len(pairs) - 1} consecutive pairs")

    rows = []
    per_unit = defaultdict(Counter)
    urange = defaultdict(list)
    for a in m.fstarts:
        name, size = m.funcs[a]
        unit = m.unit_of(a)
        if a in m.pair:
            status, kinds, sites, calls_same = m.classify(a)
            osize = m.other_size(a)
            cls = m.category(kinds, sites, calls_same, unit) if status == "differs" else ""
            urange[unit].append(m.pair[a])
            rows.append((a, size, name, unit, m.pair[a], osize, m.how[a], status, cls, ",".join(sorted(kinds)),
                         ";".join(f"{c}@{' '.join((f or '?').split())}" for c, f in sites)))
        else:
            status, cls = ("absent" if a in m.absent else "unresolved"), ""
            rows.append((a, size, name, unit, None, None, "", status, "", "", ""))
        per_unit[unit][status] += 1
        per_unit[unit]["functions"] += 1
        if cls:
            per_unit[unit][cls] += 1
    with (out / "functions.tsv").open("w", encoding="utf-8") as f:
        f.write("beta\tsize\tname\tunit\tother\tother_size\thow\tstatus\tclass\tkinds\tdebug_sites\n")
        for r in rows:
            f.write("\t".join("" if x is None else (f"0x{x:08X}" if i in (0, 4) else (f"0x{x:X}" if i in (1, 5)
                                                                                         else str(x)))
                              for i, x in enumerate(r)) + "\n")
    # data
    bnames = {a: n for a, (n, s) in m.bdata.items()}
    # data named in both DLL tables (anonymous-namespace names by their key)
    lo, hi = other.text
    onames, okeyed = {}, defaultdict(list)
    for n, v, t in m.otab:
        if not lo <= v < hi and t in (0, 2, 3):
            onames[n] = v
            okeyed[anon_key(n)].append(v)
    named = {}
    for ba, n in bnames.items():
        if n in onames:
            named[ba] = onames[n]
        elif anon_key(n) != n and len(okeyed.get(anon_key(n), ())) == 1:
            named[ba] = okeyed[anon_key(n)][0]
    disagree = 0
    with (out / "data.tsv").open("w", encoding="utf-8") as f:
        f.write("beta\tbeta_symbol\tother\tevidence\tconflicts\n")
        for ba in sorted(set(m.data_pairs) | set(named)):
            c = m.data_pairs.get(ba, Counter())
            if ba in named:
                ra, ev = named[ba], "name"
                disagree += bool(c) and c.most_common(1)[0][0] != ra
                conf = sum(n for x, n in c.items() if x != ra)
            else:
                ra, n = c.most_common(1)[0]
                ev, conf = str(n), sum(c.values()) - n
            f.write(f"0x{ba:08X}\t{bnames.get(ba, '')}\t0x{ra:08X}\t{ev}\t{conf}\n")
    print(f"data named in both tables: {len(named)}; code evidence disagreeing with a name: {disagree}")
    with (out / "units.tsv").open("w", encoding="utf-8") as f:
        f.write("unit\tfunctions\tsame\tdiffers\tabsent\tunresolved\tdebug\theader\tlayout\tregs\tnops\tstub\tshape\tinline\tcalls\tother_start\tother_end\n")
        for _, unit in m.units:
            c = per_unit[unit]
            rr = urange.get(unit)
            f.write(f"{unit}\t{c['functions']}\t{c['same']}\t{c['differs']}\t{c['absent']}\t{c['unresolved']}\t{c['debug']}\t"
                    f"{c['header']}\t{c['layout']}\t{c['regs']}\t{c['nops']}\t{c['stub']}\t{c['shape']}\t{c['inline']}\t{c['calls']}\t"
                    + (f"0x{min(rr):08X}\t0x{max(rr):08X}" if rr else "\t") + "\n")
    with (out / "layout.tsv").open("w", encoding="utf-8") as f:
        f.write("class\tchanges\toffsets (beta->other: count)\n")
        for cls, c in sorted(m.layout.items(), key=lambda kv: -sum(kv[1].values())):
            f.write(f"{cls}\t{sum(c.values())}\t" + " ".join(f"0x{x:X}->0x{y:X}:{n}" for (x, y), n in
                                                             sorted(c.items(), key=lambda kv: -kv[1])[:12]) + "\n")
    tot = Counter()
    for r in rows:
        tot[r[7]] += 1
        if r[8]:
            tot[r[8]] += 1
    print("functions:", dict(tot))
    print(f"data addresses paired: {len(m.data_pairs)} "
          f"(conflicting: {sum(1 for c in m.data_pairs.values() if len(c) > 1)})")
    print(f"written: {out}/functions.tsv, data.tsv, units.tsv")


if __name__ == "__main__":
    main()
