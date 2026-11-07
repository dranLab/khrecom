---
description: Keep a fixed number of decomp-worker sub-agents running on the plan's work items (one unit each), following docs/plan.md; merge each one as it finishes and start the next
argument-hint: "[number of workers, default 3] [model=<name>] [extra hints, e.g. 'lib only', 'Game/Enemy only', a unit list]"
---

You coordinate a continuous pool of decomp-worker sub-agents. Arguments: $ARGUMENTS

**docs/plan.md is the plan you follow.** Read it completely first (its first lines say so), then CLAUDE.md and the
memory index. This command only adapts the plan to parallel work: the plan's "Working rules" allow parallel work
only through this pool, one unit per `decomp-worker` in its own worktree, merged one at a time with the gate. You
are the plan's single sequential worker for everything on `main`: merges, docs, memory, approved config lines. You
do not decompile yourself while the pool runs. Where this file and the plan differ, the plan wins: tell the user.

## 1. Session start (plan "Session start", once)

- The main checkout (first entry of `git worktree list`), on `main`, `git status` clean. A background job is forced
  into a worktree and can't merge `main`: run the pool in an interactive session.
- `tools/check.sh > build/check.log 2>&1; echo $?` prints 0; if not, stop and report.
- The plan's docs block ("Keeping the docs current"); if anything changed, commit it first
  (`docs: regenerate leftovers and progress`, files added by name).
- Workers already running in this session count towards the pool size, and their units are taken. Interrupted
  workers (a pause for the usage limit) leave a `decomp/<slug>` branch or a worktree with uncommitted work: resume
  those first (commit the work in the old worktree as `WIP <unit> (interrupted)`, only `src/`; brief the new worker
  to branch `decomp/<slug>-r` from main and `git merge decomp/<slug>`; afterwards remove both worktrees and delete
  `decomp/<slug>`, `decomp/<slug>-r` and both `worktree-agent-*` branches).

## 2. The work list (plan "Remaining work, in this order")

Measure: `.venv/bin/python tools/drafts.py -j 12 > build/pool.drafts 2> build/pool.sum` (about 10 minutes; every
parked draft with funcdiff's line count, closest first, or its first build error). Then hand out units in the plan's
order, each as one worker brief:

1. **Parked game drafts**, closest first: rank units by their closest draft, then by how many drafts are within
   about 10 lines; the leads the plan names first. A unit whose drafts only fail to build is still a candidate (the
   error goes in the brief: usually a stopgap block that clashes when the draft is enabled).
2. **Data from source** for the units the plan lists under item 2 (and the blocker table of
   `docs/asm-leftovers.md`): only where the plan doesn't record it as blocked.
3. **Library leftovers** (`docs/sdk-leftovers.md`).
4. **Header cleanup, interleaved:** after every two merged work items, one `tools/partials.py --queue` item as a
   worker in mode "cleanup" (plan A3), unless the plan's "Queue state" says the top items stay; never two cleanup
   workers on overlapping headers.
5. **New units** (mode "new") only for the C/C++ units `tools/candidates.py` still lists; assembly-only units stay
   assembly.

Skip units with a `decomp/*` branch or a running worker; prefer units from other directories than the running
workers (they may need the same header). Follow the user's hints. After a merge, refresh only that unit
(`tools/drafts.py --unit <unit>`).

## 3. Starting a worker

First the **usage gate** each time a slot is free: `python3 .claude/usage_gate.py --running <workers still running>
--per-worker <estimate>` (the status line `~/.claude/statusline.py` saves the plan's usage to
`~/.claude/usage-latest.json`). On `WAIT`, start nothing: say so in one line and leave the slot empty (section 5).
The estimate is the 5-hour-window % one worker uses: start with 8; append `<HH:MM> <5h %> <running> <event>` to
`build/usage.log` at every start and finish, and use the observed average once a few workers have finished.

Then one Agent tool call per unit (several in one message when filling several slots): `subagent_type`:
`decomp-worker`, `isolation`: `worktree`, and `model` if the user gave one. The worker reads docs/plan.md itself:
the brief adds only what the plan can't know.

    Mode: parked. Unit <unit> (<lang>). Its drafts, from tools/drafts.py (funcdiff lines, closest first):
      <the unit's rows, as tools/drafts.py printed them>
    Memory notes that fit: <two to five names from the plan's Levers index, by the symptoms in the notes>
    <what you know: a related unit that matched with the same lever, a note you know is stale>

- Mode "new": `Mode: new. Unit <unit> (<lang>, <size> bytes of code, <n> functions).` plus related units whose
  headers it can include.
- Mode "cleanup": `Mode: cleanup. Queue item <header or topic> (plan A3, tools/partials.py --queue).`
- Never ask a worker for something its definition or the plan forbids: no doc edits, no `tools/`/`config/`
  changes, no PARTIAL markers or other header changes against the ref. Report-only items come back to you.

Tell the user which unit you start, in one line.

## 4. When a worker finishes (plan "Commits and merges")

Its report and completion notification wake you. In the main checkout, one merge at a time:

1. **Leftover processes first:** list the worker's shells and runs (`/proc/<pid>/cwd` inside its worktree, e.g.
   `until ... pgrep` waits, permuter runs) and kill them by PID; never with a `pkill -f` pattern your own shell
   matches.
2. **Baseline:** `tools/partials.py --summary > build/pool.partials.before`.
3. `git merge --no-ff --no-commit decomp/<slug>`. Conflicts are usually shared headers: combine both sides'
   declarations; in a generated doc, take main's side and regenerate it.
4. **Review the diff against the plan before the gate (V4, header cleanup):** `git diff --cached | grep
   '^+.*PARTIAL'` prints nothing; no header changes against the ref (access specifiers, extra members, stand-ins);
   only real C/C++, every kept mechanism with `MATCH:`; no changes to `tools/`/`configure.py`/`config/`/docs the
   user didn't approve. A branch that fails this is not merged: `git merge --abort`, tell the user, and brief a new
   worker to park the function instead.
5. **V5:** `tools/check.sh`. On exit 0: `tools/partials.py --summary` again, then commit with the plan's message,
   `Merge decomp/<slug>: <unit>, <matched> matched, <parked> parked (stand-ins -N, PARTIAL -M, mechanisms -K)`.
   Otherwise `git merge --abort`, keep the branch for review and tell the user.
6. **The docs block** (every merge, plan "Keeping the docs current"): the four regenerations, commit what changed
   as `docs: regenerate leftovers and progress`, files added by name (never `git add docs`). The hand-kept parts in
   the same step: the work table of docs/plan.md (drop a unit once it has no unparked function left), the blocker
   table of `docs/asm-leftovers.md`, `docs/sdk-leftovers.md`'s "Notes per unit"/"Levers" for a library finding the
   worker reported.
7. **Clean up:** `git worktree remove --force <worktree>` (after step 1 and a `git -C <worktree> status` check),
   `git branch -d decomp/<slug>` and the worktree's own `worktree-agent-*` branch.
8. **Check the worker's claims** against `tools/drafts.py --unit <unit>`.
9. **Records and memory:** a new lever gets its memory file (one per lever, linked from MEMORY.md) and a row in the
   plan's Levers index; a unit that is now finished gets its `*-unit-lessons` file and MEMORY.md line (plan
   "Finishing a unit" 6). A wrong lever (one that broke the plan) is corrected, not kept.
10. **Follow-ups** the worker reported: those the plan allows you (`config/SLUS_217.99/symbol_addrs.txt` lines that
    name unnamed weak or vtable copies: add them as a commented block for the unit, switch the unit to its
    full-source variant, `touch` the source, full `tools/check.sh`, commit only if it exits 0 and
    `tools/funcdiff.py <unit>` shows every function MATCH, otherwise revert and report). Everything in the plan's
    "When to stop and ask the user" (a `tools/`/`configure.py`/`config/` change, a header-cleanup decision, ...):
    ask the user; under an unattended `/loop`, decide it yourself as the plan says and log the decision.
11. **Refill** the slot (sections 2 and 3) right away, then report the merge in a few lines: merged or not,
    functions matched and parked (before -> after), the partials deltas, open issues.

## 5. Keeping it going and stopping

- Under `/loop`, re-arm the fallback wakeup (1200-1800 s) after each event; the workers' notifications are the real
  wake signal. On a fallback wakeup with all workers still running, do nothing else.
- While the usage gate says `WAIT`: don't refill. With none left running, schedule the wakeup for shortly after the
  5-hour window resets (the gate prints the reset time; wakeups are capped at 3600 s, so chain them).
- Stop refilling when the user asks, when the work list has no item left that no earlier worker of this run already
  had (parked drafts: none within about 30 lines), or when two merges in a row fail `tools/check.sh` (something
  systemic: tell the user). Let running workers finish and merge them.
- **At the end** (plan "Record keeping"): V5 on `main`, the docs block, "Where things stand" in docs/plan.md
  (numbers and date), one commit; then summarize the run for the user: units, functions matched, drafts improved
  (before -> after), partials deltas, what is left.
- Never push: that is the user's.
