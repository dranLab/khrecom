---
description: Keep a fixed number of decomp-worker sub-agents running continuously; merge each one as it finishes and start the next
argument-hint: "[number of workers, default 3] [extra hints, e.g. 'only C files' or 'System/ only']"
---

You coordinate a continuous pool of decompilation workers. Arguments: $ARGUMENTS

Unlike `/decomp-batch`, you don't wait for a whole batch: whenever a worker finishes, merge its branch and start a
replacement right away, so the pool stays at the requested size (default 3).

## 1. Preconditions (once, at the start)

- You are in the main checkout (first entry of `git worktree list`), on `main`, with a clean working tree.
- `tools/check.sh` passes. If not, stop and report.
- Note the workers already running in this session (if invoked while some are active): they count towards the
  pool size, and their units are taken.

## 2. Picking a unit

Each time a slot is free, first the **usage gate**: `python3 .claude/usage_gate.py --running <workers still
running> --per-worker <estimate>` (needs the status line `~/.claude/statusline.py`, which saves the plan's usage to
`~/.claude/usage-latest.json`; it only refreshes when the UI redraws, so while the session idles the gate estimates
from the last reading plus the workers started since, per the `start` lines in `build/usage.log`: always log them as
`<HH:MM> <5h %> <running> start <unit>`). On `WAIT`, start nothing: tell the user in one line and leave the slot empty (see
section 5). The estimate is the 5-hour-window % one worker uses: start with 8; append
`<time> <5h %> <running> <event>` to `build/usage.log` at every start and finish, and once a few workers have
finished, use the observed average.

Then:
- `.venv/bin/python tools/candidates.py SLUS_217.99 --no-headers --limit 60` (drop `--no-headers` if the user
  asks for harder files). Skip units that already have a branch (`git branch --list 'decomp/*'`) or are
  assigned to a running worker.
- Small ones first, from a different class/directory than the running workers, and never a unit whose
  `.rodata` lists the same header as a running worker's unit. Follow the user's hints.
- If an old worktree holds uncommitted work for a unit (an interrupted worker), give that unit's new worker the
  path to those files as its starting point; remove the old worktree only after the unit is merged.
- Tell the user which unit you start, in one line.

## 3. Starting a worker

One Agent tool call per unit (several in one message when filling several slots, e.g. at the start):
- `subagent_type`: `decomp-worker`, `isolation`: `worktree`
- `prompt`: `Decompile unit <unit> (<lang>, <size> bytes of code, <n> functions).` plus anything you know that
  helps (e.g. related units already decompiled whose headers it can include).

Do not decompile yourself meanwhile.

## 4. When a worker finishes

Its report and completion notification wake you. In the main checkout:
1. If it reported a commit: `git merge --no-ff --no-commit decomp/<slug>`. Conflicts are usually shared headers
   (two workers created/extended the same one): combine both sides' declarations, keep `// PARTIAL` markers.
2. `tools/check.sh`. If it exits 0: `git commit -m "Merge decomp/<slug>: <unit>"`, then
   `git worktree remove <the worker's worktree>`, `git branch -d decomp/<slug>` and delete the worktree's own
   `worktree-agent-*` branch. If not: `git merge --abort`, keep the branch for review and tell the user.
3. Handle small follow-ups the worker reported (shared headers, `include/khtypes.h`) if cheap; each such change
   passes `tools/check.sh` and gets its own commit. Changes to `configure.py` or `tools/` need the user's OK.
   - **`config/SLUS_217.99/symbol_addrs.txt` follow-ups are pre-approved by the user** (no need to ask): when a
     worker reports that its unit only matches with its data from source once an unnamed weak copy is named
     (e.g. `_$_6CState_<addr>` / `_vt$6CState_<addr>`), add those lines as a commented block for the unit, switch
     the unit to its full-source variant (the worker's macro to 1, the `#if` branches resolved, the
     `// SOURCE DATA:` line added), `touch` the source, and run the full `tools/check.sh`. It rebuilds and
     checksums the whole image, so it also proves no already matched unit changed. Commit only if it exits 0
     and `tools/funcdiff.py <unit>` shows every function MATCH. If another unit breaks, fix that unit the same
     way (name its copy, keep its source matching) until the gate passes again, or revert the follow-up
     (`git checkout -- config/SLUS_217.99/symbol_addrs.txt src/<unit>.*`) and report it.
4. Refill the slot (sections 2 and 3) right away, then report the merge in a few lines: merged or not,
   functions matched/total, data from source or not, open issues.

Only one merge at a time: if several workers finish together, merge them one after the other, then refill.

## 5. Keeping it going and stopping

- When running under `/loop`, re-arm the fallback wakeup (1200-1800 s) after each event; the workers'
  notifications are the real wake signal. On a fallback wakeup with all workers still running, do nothing else.
- While the usage gate says `WAIT`: don't refill. With workers still running, their notifications wake you as
  usual. With none left, schedule the wakeup for shortly after the 5-hour window resets (the gate prints the reset
  time; wakeups are capped at 3600 s, so chain them), then re-check the gate and refill.
- Stop refilling when the user asks, when no suitable candidate is left, or when two merges in a row fail
  `tools/check.sh` (something systemic, needs the user). Let running workers finish and merge them, then end
  the loop and summarize all units of the run.
- Never push.
