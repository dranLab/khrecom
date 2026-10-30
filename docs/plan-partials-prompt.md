You are continuing the matching decompilation in this repository (Kingdom Hearts Re:Chain of Memories, PS2),
now with the PARTIAL cleanup interleaved. Before touching any code, read completely, in this order:
1. docs/plan-partials.md (the cleanup plan you follow: Step 0/1, hooks A0-A5, queue, definition of done),
2. docs/plan-big-units.md (the unit plan it interleaves with; its validation levels V1-V6 apply to every change),
3. CLAUDE.md, the memory index (MEMORY.md), and the memory file of the unit you work on.

Rules that matter most:
- Work strictly sequentially in the main checkout: no sub-agents, workers, worktrees or parallel work
  (tools/permuter.py's parallel compiles are fine).
- tools/partials.py is the only approved change under tools/. No other changes to tools/, configure.py,
  config/ (except symbol_addrs naming lines) or the build flags.
- Every header change follows the A0 protocol: full `ninja` must pass the SHA-1 at every commit, and
  `tools/imagediff.py` classifies any difference. After a header edit, also run the hidden-conflict
  recheck (`partials.py --header`).
- `tools/check.sh` must exit 0 before every merge into main, and again on main afterwards.
- Cleanup work goes on `cleanup/<...>` branches. Unit hook edits go on the unit's `decomp/<Unit-with-dashes>`
  branch. Never mix function matches into cleanup branches.
- A kept matching mechanism gets a `MATCH: <reason>` comment. A PARTIAL comment is removed only when its class
  equals the ref.
- Stop and ask in the cases of plan-partials A5 and of plan-big-units "When to stop and ask".
- Never commit iso/, ref/, asm/ or build/. There is no git remote: never push, rebase main or force.
  Commits end with the Co-Authored-By line from the session instructions.

Start now:
1. If not done yet, do Step 0: write tools/partials.py and docs/partials.md on branch tools/partials. Check
   them against the plan's acceptance numbers, then merge.
2. If not done yet, do Step 1: wire plan-partials into docs/plan-big-units.md (and the tool line in CLAUDE.md),
   and commit both plans as one docs commit on main.
3. Otherwise continue where docs/partials.md and the progress logs (build/progress/*.md, including
   build/progress/cleanup-*.md) say you are. Order: the current gap's queue batch (`partials.py --queue`;
   the first is the CSound pilot), then the next unit of plan-big-units with its start hook (A1). Then that
   unit's functions, then its finish hook (A2 parts 1 and 2), then the next gap's batch.

Report briefly after each merge into main: what was merged (unit functions matched/parked, or the cleanup
item), the partials.py --summary deltas (stand-ins, PARTIAL, mechanisms), and the check.sh result.
To run it self-paced, use: /loop <this prompt>, and handle one function (or one cleanup group) per iteration.
