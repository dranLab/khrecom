#!/usr/bin/env python3
"""Raw data blocks in the sources that hide addresses (docs/port-step1-shiftable.md, S2/S6).

Units whose data comes from source but that still have INCLUDE_ASM'd functions emit those functions' data as
top-level asm blocks (`asm(".section .rodata\n" ".byte ...")`). The bytes are the original's, so the matching
build is right, but where the original had a pointer (a jump table entry `.word .L00315FC8`, a `.word D_...`)
the block holds the old address as plain bytes, and the labels the asm'd function uses (`D_004E3628`,
`jtbl_004E3800`) are only defined by original_labels.ld. Neither survives a shifted link.

This finds every such block, locates its bytes in the original image, and compares the range with the original
data listing (build/<VERSION>/expected/asm/data): words that are symbolic there, and labels defined there. With
--fix it rewrites the blocks: the labels the build needs by name are defined in place, and symbolic words are
written as `.word <symbol>`. The bytes stay the same, so the matching build's SHA-1 checks the rewrite.

Usage:
    .venv/bin/python tools/shiftblocks.py [--unit Form/Menu/Camp] [-v]
    .venv/bin/python tools/shiftblocks.py --fix --unit Form/Menu/Camp
"""

import argparse
import re
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
VERSION = "SLUS_217.99"
BASE = 0x100000  # the .rom image starts at this address
ROM = ROOT / "iso" / f"{VERSION}.rom"
LISTING = ROOT / "build" / VERSION / "expected" / "asm" / "data"

ASM_CALL = re.compile(r"\b(?:__asm__|asm)\s*\(((?:\s*\"(?:[^\"\\\n]|\\.)*\")+)\s*\)\s*;")
STRING = re.compile(r"\"((?:[^\"\\\n]|\\.)*)\"")
LISTED_WORD = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]+ \*/ \.word (\S+)")
LISTED_LINE = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ")
DLABEL = re.compile(r"^dlabel (\S+)")


def c_unescape(s: str) -> str:
    return re.sub(r"\\(n|t|\\|\"|')", lambda m: {"n": "\n", "t": "\t"}.get(m.group(1), m.group(1)), s)


class Block:
    """One asm() call: its lines and, per data run, the bytes and where they came from."""

    def __init__(self, path: Path, start: int, end: int, line: int, text: str):
        self.path, self.start, self.end, self.line, self.text = path, start, end, line, text
        self.runs: list[dict] = []  # {section, bytes, rows: [(line index, byte offset in run)]}

    def parse(self) -> bool:
        section, run = None, None
        lines = self.text.split("\n")
        self.lines = lines
        for i, raw in enumerate(lines):
            line = raw.split("#")[0].strip()
            if not line:
                continue
            m = re.match(r"\.section\s+([.\w]+)", line) or re.match(r"\.(text|data|rdata|rodata)\b$", line)
            if m:
                section = "." + m.group(1).lstrip(".")
                run = None
                continue
            if line.startswith(".align") or line.startswith(".balign"):
                run = None  # alignment: the next bytes start a new run (padding unknown)
                continue
            if line.endswith(":") or line.split()[0] in (".globl", ".local", ".type", ".size", ".weak"):
                continue  # labels and symbol attributes add no bytes
            m = re.match(r"\.(byte|half|short|word|space|ascii|asciz)\s+(.*)$", line)
            if not m or section in (None, ".text"):
                run = None  # another directive, or not data: ends the run
                continue
            kind, args = m.groups()
            data = self._bytes(kind, args)
            if data is None:
                run = None  # symbolic word: its bytes are not known here
                continue
            if run is None:
                run = {"section": section, "bytes": bytearray(), "rows": []}
                self.runs.append(run)
            run["rows"].append((i, len(run["bytes"])))
            run["bytes"] += data
        return bool(self.runs)

    @staticmethod
    def _bytes(kind: str, args: str):
        if kind in ("ascii", "asciz"):
            s = c_unescape(args.strip()[1:-1]).encode("latin-1")
            return s + (b"\0" if kind == "asciz" else b"")
        if kind == "space":
            return bytes(int(args.split(",")[0], 0))
        size = {"byte": 1, "half": 2, "short": 2, "word": 4}[kind]
        out = bytearray()
        for item in args.split(","):
            item = item.strip()
            try:
                out += (int(item, 0) & ((1 << (8 * size)) - 1)).to_bytes(size, "little")
            except ValueError:
                return None
        return bytes(out)


def find_blocks(paths: list[Path]) -> list[Block]:
    blocks = []
    for path in paths:
        source = path.read_text(encoding="latin-1")
        for m in ASM_CALL.finditer(source):
            first = source.count("\n", 0, m.start())
            text, char_line = "", []  # char_line: the source line (relative) each character of text came from
            for s in STRING.finditer(m.group(1)):
                src_line = source.count("\n", 0, m.start(1) + s.start()) - first
                chunk = c_unescape(s.group(1))
                text += chunk
                char_line += [src_line] * len(chunk)
            if ".byte" not in text and ".word" not in text:
                continue
            line_of, pos = [], 0
            for piece in text.split("\n"):  # each asm line belongs to the source line its first character is on
                line_of.append(char_line[min(pos, len(char_line) - 1)])
                pos += len(piece) + 1
            block = Block(path, m.start(), m.end(), first + 1, text)
            block.line_of = line_of
            block.source_lines = source.count("\n", m.start(), m.end()) + 1
            if block.parse():
                blocks.append(block)
    return blocks


def load_listing():
    """address -> listed .word token; label address -> name; and the listing's line addresses."""
    words, labels = {}, {}
    for path in LISTING.rglob("*.s"):
        pending = None
        for line in path.read_text(encoding="latin-1").splitlines():
            m = DLABEL.match(line)
            if m:
                pending = m.group(1)
                continue
            m = LISTED_LINE.match(line)
            if m:
                addr = int(m.group(1), 16)
                if pending:
                    labels[addr] = pending
                    pending = None
                w = LISTED_WORD.match(line)
                if w:
                    words[addr] = w.group(2)
    return words, labels


def locate(rom: bytes, data: bytes) -> list[int]:
    hits, at = [], rom.find(data)
    while at >= 0 and len(hits) < 3:
        hits.append(BASE + at)
        at = rom.find(data, at + 1)
    return hits


def needed_labels() -> dict[str, set[str]]:
    """Names the shifted link needs, with the objects that need them (tools/shiftpins.py --all, saved output)."""
    path = ROOT / "build" / "shiftpins.txt"
    if not path.exists():
        sys.exit("error: build/shiftpins.txt missing: run tools/shiftpins.py --all > build/shiftpins.txt first")
    needed = {}
    for m in re.finditer(r"^    (\S+)  \[[^]]*\]  (.*)$", path.read_text(encoding="utf-8"), re.M):
        needed[m.group(1)] = {u.strip() for u in m.group(2).split(",")}
    return needed


# Zero words splat wrote as a relocation against func_0: plain zeros, not pointers.
NOT_A_POINTER = {"func_0"}


def analyse(block: Block, rom: bytes, words: dict, labels: dict):
    """Per run: its address, the symbolic words and all labels inside it."""
    result = []
    for run in block.runs:
        data = bytes(run["bytes"])
        hits = locate(rom, data) if len(data) >= 8 else []
        if len(hits) != 1:
            result.append({"run": run, "addr": None, "hits": len(hits), "symbolic": [], "labels": []})
            continue
        addr = hits[0]
        symbolic = [(a, words[a]) for a in range(addr, addr + len(data), 4)
                    if a in words and not words[a].startswith("0x") and words[a] not in NOT_A_POINTER]
        inside = [(a, labels[a]) for a in sorted(labels) if addr <= a < addr + len(data)]
        result.append({"run": run, "addr": addr, "hits": 1, "symbolic": symbolic, "labels": inside})
    return result


ADDRESS_NAME = re.compile(r"(?:D|jtbl|func)_[0-9A-F]{6,8}$")


def select_labels(runs: list[dict], needed: dict, unit_object: str) -> None:
    """Keep the labels something needs by name: the shifted link (shiftpins), or a restored word of this file.
    A label another object needs becomes global. A word can only be restored if its target can have that name:
    a code label of the pasted asm (.L...), a label inside one of this file's blocks, or a real symbol name.
    Words pointing at an address-named target elsewhere (a string literal the compiled C emits under gcc's own
    $LC label) stay bytes and are listed in r["unnamed"]: they go away when the asm'd function becomes C."""
    in_blocks = {name for r in runs for _, name in r["labels"]}
    for r in runs:
        keep, unnamed = [], []
        for a, sym in r["symbolic"]:
            base = re.split(r"[+-]", sym)[0]
            (unnamed if ADDRESS_NAME.match(base) and base not in in_blocks else keep).append((a, sym))
        r["symbolic"], r["unnamed"] = keep, unnamed
    targets = {re.split(r"[+-]", s)[0] for r in runs for _, s in r["symbolic"]}
    for r in runs:
        keep = []
        for a, name in r["labels"]:
            if name in needed or name in targets:
                keep.append((a, name, bool(needed.get(name, set()) - {unit_object})))
        r["labels"] = keep


def emit_row(data: bytes, kind: str, start: int) -> str:
    """A piece of a split row, in the row's own unit where it fits (start: its offset inside the row)."""
    size = {"word": 4, "half": 2, "short": 2}.get(kind, 1)
    if size > 1 and start % size == 0 and len(data) % size == 0:
        items = [int.from_bytes(data[i: i + size], "little") for i in range(0, len(data), size)]
        return f".{kind} " + ",".join(f"0x{v:0{2 * size}X}" for v in items)
    return ".byte " + ",".join(f"0x{v:02X}" for v in data)


def rewrite(block: Block, runs: list[dict]) -> str:
    """The block's asm text with the selected labels defined and symbolic words restored (same bytes)."""
    out_lines = list(block.lines)
    replace = {}  # line index -> replacement lines
    for info in runs:
        if info["addr"] is None or not (info["symbolic"] or info["labels"]):
            continue
        run, addr = info["run"], info["addr"]
        labels_at = defaultdict(list)  # offset in run -> [(name, global)]
        for a, name, glob in info["labels"]:
            labels_at[a - addr].append((name, glob))
        words_at = {a - addr: sym for a, sym in info["symbolic"]}
        points = sorted(set(labels_at) | set(words_at))
        rows = run["rows"] + [(None, len(run["bytes"]))]
        for (idx, off), (_, end) in zip(rows, rows[1:]):
            mine = [p for p in points if off <= p < end]
            if not mine:
                continue
            line = block.lines[idx].split("#")[0].strip()
            kind, rest = line.split(None, 1)
            kind = kind.lstrip(".")
            if kind not in ("byte", "half", "short", "word"):
                sys.exit(f"error: {block.path}:{block.line}: cannot split `{line[:40]}` (only numeric rows)")
            values = Block._bytes(kind, rest)
            if any(p in words_at and p + 4 > end for p in mine):
                sys.exit(f"error: {block.path}:{block.line}: a pointer word crosses a row")
            new, pos = [], off
            for p in mine + [end]:
                if p > pos:
                    new.append(emit_row(values[pos - off: p - off], kind, pos - off))
                    pos = p
                if p == end:
                    break
                for name, glob in labels_at.get(p, []):
                    if glob:
                        new.append(f".globl {name}")
                    new.append(f"{name}:")
                if p in words_at:
                    new.append(f".word {words_at[p]}")
                    pos = p + 4
            replace[idx] = new
    for idx in sorted(replace, reverse=True):
        out_lines[idx] = GROUP.join(replace[idx])  # one source line, as before: keeps __LINE__ and line markers
    return "\n".join(out_lines)


GROUP = "\x00"  # separates asm lines that share one source line


def as_c(block: Block, text: str, original: str) -> str:
    """The asm() call for text, on exactly the source lines the original call used (same line count, same
    indentation): every asm line goes back to the source line it came from."""
    lines = text.split("\n")
    last = len(lines) - 1
    buckets = [[] for _ in range(block.source_lines)]
    for i, line in enumerate(lines):
        if i == last and line == "":
            break  # the text ended with a newline: nothing follows it
        parts = line.split(GROUP)
        for j, part in enumerate(parts):
            end = "" if (i == last and j == len(parts) - 1) else "\\n"
            buckets[block.line_of[i]].append(f'"{part}{end}"')
    old_lines = original.split("\n")
    keyword = re.match(r"(__asm__|asm)\s*\(", original).group(1)
    out = []
    for k, bucket in enumerate(buckets):
        indent = re.match(r"[ \t]*", old_lines[k]).group(0) if k else ""
        out.append(indent + " ".join(bucket))
    return f"{keyword}(" + "\n".join(out) + ");"


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--unit", help="one unit (path under src/ without the extension)")
    parser.add_argument("--fix", action="store_true", help="rewrite the blocks (needs --unit)")
    parser.add_argument("-v", "--verbose", action="store_true")
    args = parser.parse_args()
    if args.fix and not args.unit:
        sys.exit("error: --fix needs --unit")

    if args.unit:
        paths = [p for p in (ROOT / "src").glob(f"{args.unit}.*") if p.suffix in (".c", ".cpp")]
    else:
        paths = [p for p in (ROOT / "src").rglob("*") if p.suffix in (".c", ".cpp")]
    rom = ROM.read_bytes()
    words, labels = load_listing()
    needed = needed_labels()

    total_sym = total_lab = unlocated = total_unnamed = 0
    for path in sorted(paths):
        blocks = find_blocks([path])
        if not blocks:
            continue
        unit_object = path.relative_to(ROOT / "src").with_suffix(".o").as_posix()
        analysed = [(block, analyse(block, rom, words, labels)) for block in blocks]
        select_labels([r for _, runs in analysed for r in runs], needed, unit_object)
        source = path.read_text(encoding="latin-1")
        changed = []
        for block, runs in analysed:
            sym = sum(len(r["symbolic"]) for r in runs)
            lab = sum(len(r["labels"]) for r in runs)
            unnamed = [u for r in runs for u in r["unnamed"]]
            lost = [r for r in runs if r["addr"] is None]
            total_sym, total_lab, unlocated = total_sym + sym, total_lab + lab, unlocated + len(lost)
            total_unnamed += len(unnamed)
            if sym or lab or unnamed or (lost and args.verbose):
                rel = path.relative_to(ROOT)
                print(f"{rel}:{block.line}: {sym} symbolic words as bytes, {lab} labels needed by name"
                      + (f", {len(unnamed)} pointers to unnamed C constants (stay bytes)" if unnamed else "")
                      + (f", {len(lost)} runs not located" if lost else ""))
                for a, s in unnamed:
                    print(f"    0x{a:08X} -> {s}: unnamed in this build (stays bytes until the asm'd function is C)")
                if args.verbose:
                    for r in runs:
                        if r["addr"] is None:
                            data = bytes(r["run"]["bytes"])
                            print(f"    not located ({r['hits']} hits): {r['run']['section']} {len(data)} bytes "
                                  f"{data[:16].hex(' ')}")
                        for a, s in r["symbolic"]:
                            print(f"    0x{a:08X} .word {s}")
                        for a, n, g in r["labels"]:
                            print(f"    0x{a:08X} {n}:" + ("  (global)" if g else ""))
            if args.fix and (sym or lab):
                changed.append((block, rewrite(block, runs)))
        if changed:
            for block, text in sorted(changed, key=lambda bt: -bt[0].start):
                new = as_c(block, text, source[block.start: block.end])
                if new.count("\n") != source.count("\n", block.start, block.end):
                    sys.exit(f"error: {path.relative_to(ROOT)}:{block.line}: the rewrite would change the line count")
                source = source[: block.start] + new + source[block.end:]
            path.write_text(source, encoding="latin-1")
            print(f"rewrote {len(changed)} blocks in {path.relative_to(ROOT)}")
    print(f"total: {total_sym} symbolic words written as bytes, {total_lab} labels needed by name, "
          f"{total_unnamed} pointers to unnamed C constants"
          + (f", {unlocated} runs not located (-v)" if unlocated else ""))


if __name__ == "__main__":
    main()
