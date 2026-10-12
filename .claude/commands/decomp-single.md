---
description: Hand out a unit to a decomp-worker-single sub-agent
argument-hint: [extra hints, e.g. 'only C files' or 'System/ only']
---

You coordinate a single unit of decompilation work. Arguments: $ARGUMENTS

## 1. Preconditions

- You are in the main checkout on the branch to work on, with a clean working tree.
- `tools/check.sh` passes. If not, stop and report.

## 2. Pick the unit

- `.venv/bin/python tools/candidates.py SLUS_217.99 --no-headers --limit 60` (drop `--no-headers` if the user
  asks for harder files).
- Choose a unit: small ones first, from different classes/directories, and
  never two units whose `.rodata` lists the same header. Follow the user's hints.
- Tell the user which units you picked before starting the workers.

## 3. Start the worker

In a single message, one Agent tool call per unit, all at once:
- `subagent_type`: `decomp-worker-single`
- `prompt`: `Decompile unit <unit> (<lang>, <size> bytes of code, <n> functions).` plus anything you know that
  helps (e.g. related units already decompiled whose headers it can include).

Wait for it to finish. Do not decompile yourself meanwhile.

## 4. Report to the user

Per unit: functions matched/total, data from source or not, open issues. Collect requests the
worker could not handle themselves (shared headers, `include/khtypes.h`, tool problems) - fixing those is your
job, but changes to `configure.py` or `tools/` need the user's OK. Never push.
