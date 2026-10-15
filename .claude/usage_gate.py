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
ap.add_argument("--max-age", type=float, default=300, help="minutes after which stale usage data is not used at all")
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
if used5 is None or age > args.max_age * 60:
    print(f"WAIT usage data missing or stale ({age / 60:.0f} min old)")
    sys.exit(1)

now = time.time()
fmt = lambda t: time.strftime("%H:%M", time.localtime(t)) if t else "?"


def starts_since(t):
    """Workers started after time t, from build/usage.log ("HH:MM <5h %> <running> start <unit>", today)."""
    log = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "build", "usage.log")
    n = 0
    try:
        for line in open(log):
            f = line.split()
            if len(f) < 4 or f[3] != "start":
                continue
            h, m = map(int, f[0].split(":"))
            lt = time.localtime(now)
            ts = time.mktime((lt.tm_year, lt.tm_mon, lt.tm_mday, h, m, 0, 0, 0, -1))
            if ts > now:
                ts -= 86400
            n += ts >= t
    except (OSError, ValueError):
        pass
    return n


stale = ""
if age > 600:
    # The status line only runs when the UI redraws, so the data goes stale while the session idles. Use the last
    # reading (0 if its 5h window has reset since) plus the full estimate for every worker that may have run since.
    since = time.time() - age
    if reset5 and now >= reset5:
        used5 = 0.0
    used5 += (args.running + starts_since(since)) * args.per_worker
    stale = f" [data {age / 60:.0f} min old, estimated]"
info = f"5h {used5:.0f}% (resets {fmt(reset5)}), 7d {used7 if used7 is not None else '?'}%{stale}"
projected = used5 + (args.running + 1) * args.per_worker
if used7 is not None and used7 >= args.limit_7d:
    print(f"WAIT weekly limit near: {info}")
    sys.exit(1)
if projected > args.limit_5h and not (reset5 and reset5 - now < 15 * 60 and used5 < args.limit_5h):
    print(f"WAIT projected {projected:.0f}% > {args.limit_5h:.0f}%: {info}")
    sys.exit(1)
print(f"START projected {projected:.0f}%: {info}")
