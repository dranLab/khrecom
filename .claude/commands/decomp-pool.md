---
description: Keep a fixed number of decomp-worker sub-agents running on parked drafts (one unit each); merge each one as it finishes and start the next
argument-hint: "[number of workers, default 3] [model=<name>] [extra hints, e.g. 'lib only', 'Game/Enemy only', a unit list]"
---

You coordinate a continuous pool of decompilation workers. Arguments: $ARGUMENTS

The work left is mostly parked drafts in existing units, so the pool hands out units with drafts, closest first,
one unit per worker (mode "parked"). Units without source (mode "new") are only the C/C++ ones `tools/candidates.py`
still lists; assembly-only units stay assembly. Whenever a worker finishes, merge it and start the next right
away, so the pool stays at the requested size (default 3).

## 1. Preconditions (once, at the start)

- You are in the main checkout (first entry of `git worktree list`), on `main`, with a clean working tree. A
  background job is forced into a worktree and can't merge `main`: run the pool in an interactive session.
- `tools/check.sh` passes. If not, stop and report.
- Workers already running in this session count towards the pool size, and their units are taken.

## 2. The work list

At the start: `.venv/bin/python tools/drafts.py -j 12 > build/pool.drafts 2> build/pool.sum` (about 10 minutes;
every parked draft with funcdiff's line count, closest first, or its first build error). Group the rows by unit.
Rank units by their closest draft, then by how many drafts are within about 10 lines. A unit whose drafts only fail
to build is still a candidate (give the worker the error); a function with "no draft" in an otherwise drafted unit
goes in the same worker's brief. Skip units that have a `decomp/*` branch already or a running worker.

Prefer units from different directories than the running workers (they may need the same header). Follow the
user's hints. After a merge, refresh only that unit: `tools/drafts.py --unit <unit>`.

Interrupted workers (a pause for the usage limit) leave a `decomp/<slug>` branch with WIP commits or a worktree with
uncommitted work. Resume those first:
1. Uncommitted work: commit it in the old worktree as `WIP <unit> (interrupted)` (only `src/`).
2. The old branch stays checked out in its worktree: tell the new worker to create `decomp/<slug>-r` from main,
   `git merge decomp/<slug>`, run `tools/drafts.py --unit <unit>` and continue.
3. After merging `-r`, remove both worktrees and delete `decomp/<slug>`, `decomp/<slug>-r` and both
   `worktree-agent-*` branches.

## 3. Starting a worker

First the **usage gate** each time a slot is free: `python3 .claude/usage_gate.py --running <workers still
running> --per-worker <estimate>` (the status line `~/.claude/statusline.py` saves the plan's usage to
`~/.claude/usage-latest.json`). On `WAIT`, start nothing: say so in one line and leave the slot empty (section 5).
The estimate is the 5-hour-window % one worker uses: start with 8; append `<HH:MM> <5h %> <running> <event>` to
`build/usage.log` at every start and finish, and use the observed average once a few workers have finished.

Then one Agent tool call per unit (several in one message when filling several slots):
- `subagent_type`: `decomp-worker`, `isolation`: `worktree`, and `model` if the user gave one.
- `prompt`, for mode "parked":

      Mode: parked. Unit <unit> (<lang>). Its drafts, from tools/drafts.py (funcdiff lines, closest first):
        <the unit's rows, as tools/drafts.py printed them>
      Memory notes that fit (read them first, ~/.claude/projects/-home-alex-khrecom/memory/<name>.md): <names>
      <anything else you know: a related unit that matched with the same lever, a stale note you noticed>

  Pick the memory notes from docs/plan.md's "Levers index" and MEMORY.md by the symptoms in the drafts' notes
  (two to five, not the whole index: the worker pays for each in context).
- For mode "new": `Mode: new. Decompile unit <unit> (<lang>, <size> bytes of code, <n> functions).` plus related
  units whose headers it can include.

Tell the user which unit you start, in one line. Do not decompile yourself meanwhile.

## 4. When a worker finishes

Its report and completion notification wake you. In the main checkout, one merge at a time:
1. If it reported commits: `git merge --no-ff --no-commit decomp/<slug>`. Conflicts are usually shared headers:
   combine both sides' declarations, keep `// PARTIAL` markers.
2. `tools/check.sh`. If it exits 0: `git commit -m "Merge decomp/<slug>: <unit>, <matched> matched, <parked> parked"`,
   then `git worktree remove <the worker's worktree>`, `git branch -d decomp/<slug>` and delete the worktree's own
   `worktree-agent-*` branch. If not: `git merge --abort`, keep the branch for review and tell the user.
3. Refresh the unit's rows (`tools/drafts.py --unit <unit>`) and check the worker's claims against them.
4. Follow-ups it reported (shared headers, `include/khtypes.h`): handle them if cheap, each with `tools/check.sh`
   and its own commit. Changes to `configure.py` or `tools/` need the user's OK.
   **`config/SLUS_217.99/symbol_addrs.txt` lines that name unnamed weak or vtable copies are pre-approved**: add them
   as a commented block for the unit, switch the unit to its full-source variant, `touch` the source and run the
   full `tools/check.sh`; commit only if it exits 0 and `tools/funcdiff.py <unit>` shows every function MATCH,
   otherwise revert (`git checkout -- config/SLUS_217.99/symbol_addrs.txt src/<unit>.*`) and report.
5. A lever the worker found that is new: add it to the memory notes (one file per lever, linked from MEMORY.md).
6. Refill the slot (sections 2 and 3) right away, then report the merge in a few lines: merged or not, functions
   matched and parked (before -> after), open issues.

Every few merges, and at the end: `tools/leftovers.py game --update docs/asm-leftovers.md`,
`tools/leftovers.py lib --update docs/sdk-leftovers.md`, the counts in docs/plan.md ("Where things stand"), one
commit, `tools/check.sh`.

## 5. Keeping it going and stopping

- Under `/loop`, re-arm the fallback wakeup (1200-1800 s) after each event; the workers' notifications are the real
  wake signal. On a fallback wakeup with all workers still running, do nothing else.
- While the usage gate says `WAIT`: don't refill. With none left running, schedule the wakeup for shortly after the
  5-hour window resets (the gate prints the reset time; wakeups are capped at 3600 s, so chain them).
- Stop refilling when the user asks, when the work list has no unit left with a draft within about 30 lines that
  no earlier worker of this run already had, or when two merges in a row fail `tools/check.sh` (something
  systemic: tell the user). Let running workers finish and merge them, then summarize the run: units, functions
  matched, drafts improved (before -> after), what is left.
- Never push: that is the user's.
