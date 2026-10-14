#!/usr/bin/env python3
"""Usage gate for /decomp-pool: may another worker start without running into the plan's usage limit?

Reads ~/.claude/usage-latest.json (written by the status line script ~/.claude/statusline.py).
Prints one line, "START ..." or "WAIT ...", and exits 0 / 1.

    python3 .claude/usage_gate.py --running 2 --per-worker 8
"""
import argparse, json, os, sys, time

ap = argparse.ArgumentParser()
ap.add_argument("--running", type=int, default=0, help="workers still running")
ap.add_argument("--per-worker", type=float, default=8.0, help="estimated 5h-window %% one worker uses")
ap.add_argument("--limit-5h", type=float, default=90.0, help="projected 5h %% that must not be exceeded")
ap.add_argument("--limit-7d", type=float, default=90.0)
args = ap.parse_args()

path = os.path.expanduser("~/.claude/usage-latest.json")
try:
    age = time.time() - os.path.getmtime(path)
    rl = json.load(open(path)).get("rate_limits") or {}
except (OSError, ValueError):
    print("WAIT no usage data (status line not running?)")
    sys.exit(1)

def window(key):
    w = rl.get(key) or {}
    return w.get("used_percentage"), w.get("resets_at")

used5, reset5 = window("five_hour")
used7, reset7 = window("seven_day")
if used5 is None or age > 600:
    print(f"WAIT usage data missing or stale ({age / 60:.0f} min old)")
    sys.exit(1)

now = time.time()
fmt = lambda t: time.strftime("%H:%M", time.localtime(t)) if t else "?"
info = f"5h {used5:.0f}% (resets {fmt(reset5)}), 7d {used7 if used7 is not None else '?'}%"
projected = used5 + (args.running + 1) * args.per_worker
if used7 is not None and used7 >= args.limit_7d:
    print(f"WAIT weekly limit near: {info}")
    sys.exit(1)
if projected > args.limit_5h and not (reset5 and reset5 - now < 15 * 60 and used5 < args.limit_5h):
    print(f"WAIT projected {projected:.0f}% > {args.limit_5h:.0f}%: {info}")
    sys.exit(1)
print(f"START projected {projected:.0f}%: {info}")
