---
description: Hand out a batch of units to decomp-worker sub-agents in parallel, then merge their branches into main
argument-hint: "[number of workers, default 3] [extra hints, e.g. 'only C files' or 'System/ only']"
---

You coordinate a batch of parallel decompilation work. Arguments: $ARGUMENTS

## 1. Preconditions

- You are in the main checkout (first entry of `git worktree list`), on `main`, with a clean working tree.
- `tools/check.sh` passes. If not, stop and report.

## 2. Pick the units

- `.venv/bin/python tools/candidates.py SLUS_217.99 --no-headers --limit 60` (drop `--no-headers` if the user
  asks for harder files). Skip units that already have a branch (`git branch --list 'decomp/*'`).
- Choose as many units as requested (default 3): small ones first, from different classes/directories, and
  never two units whose `.rodata` lists the same header. Follow the user's hints.
- Tell the user which units you picked before starting the workers.

## 3. Start the workers

In a single message, one Agent tool call per unit, all at once:
- `subagent_type`: `decomp-worker`, `isolation`: `worktree`
- `prompt`: `Decompile unit <unit> (<lang>, <size> bytes of code, <n> functions).` plus anything you know that
  helps (e.g. related units already decompiled whose headers it can include).

Wait for all of them to finish. Do not decompile yourself meanwhile.

## 4. Merge, one branch at a time

For each worker that reported a commit, in the main checkout:
1. `git merge --no-ff --no-commit decomp/<slug>`
2. Conflicts are usually shared headers (two workers created/extended the same one): combine both sides'
   declarations, keep `// PARTIAL` markers.
3. `tools/check.sh`. If it exits 0: `git commit -m "Merge decomp/<slug>: <unit>"`. If not: `git merge --abort`
   and keep the branch for review.
4. After a successful merge: `git worktree remove <the worker's worktree>` and `git branch -d decomp/<slug>`.

## 5. Report to the user

Per unit: merged or not, functions matched/total, data from source or not, open issues. Collect requests the
workers could not handle themselves (shared headers, `include/khtypes.h`, tool problems) - fixing those is your
job, but changes to `configure.py` or `tools/` need the user's OK. Never push.
