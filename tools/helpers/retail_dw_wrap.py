"""KH_DW(...) around reads of DebugWork::g_dw, KH_DEBUG(...) around whole-line write statements (line-neutral)."""
import re, sys
from pathlib import Path
DEBUG = ("Form/DebugWindow", "Form/DebugMenu", "Form/DebugWindowTBL", "Game/Debug/", "Game/Collision/CollDisp",
         "System/DebugProcInfo", "Game/Player/DWTPlayer.h")
USE = re.compile(r"(?<![\w:])((?:DebugWork::)?g_dw(?:\s*\.\s*\w+|\s*\[[^\]\[]*\])+)")
WRITE_AFTER = re.compile(r"\s*(\+\+|--|[-+*/|&^%]?=(?!=)|<<=|>>=)")
reads = writes = left = 0
report = []
for p in sorted(Path("src").rglob("*")):
    if p.suffix not in (".cpp", ".h", ".c"): continue
    rel = p.relative_to("src").as_posix()
    if any(rel.startswith(d) for d in DEBUG): continue
    lines = p.read_text(encoding="latin-1").split("\n"); changed = False
    for i, l in enumerate(lines):
        if "g_dw" not in l or "KH_DW(" in l or "KH_DEBUG(" in l or l.lstrip().startswith(("//", "#", "*", "/*")):
            continue
        uses = list(USE.finditer(l))
        if not uses: continue
        is_write = any(WRITE_AFTER.match(l[m.end():]) or (l[:m.start()].rstrip().endswith(("++", "--", "&")) and not l[:m.start()].rstrip().endswith("&&")) for m in uses)
        code = l.strip()
        if is_write:
            # a whole statement line: wrap it
            if code.endswith(";") and (code.startswith(("DebugWork::g_dw", "g_dw", "++", "--")) ) and code.count(";") == 1:
                ind = l[: len(l) - len(l.lstrip())]
                lines[i] = f"{ind}KH_DEBUG({code})"; writes += 1; changed = True
            else:
                left += 1; report.append(f"{rel}:{i + 1}: {code[:140]}")
            continue
        out, pos = [], 0
        for m in uses:
            out.append(l[pos:m.start()]); out.append(f"KH_DW({m.group(1)})"); pos = m.end(); reads += 1
        out.append(l[pos:]); lines[i] = "".join(out); changed = True
    if changed:
        p.write_text("\n".join(lines), encoding="latin-1")
print(f"reads {reads}, write lines {writes}, left {left}")
Path("build/retail_try/dw_left.txt").write_text("\n".join(report) + "\n")
