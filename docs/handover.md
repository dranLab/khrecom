# Handover (2026-10-30)

State of the matching decompilation and the PARTIAL cleanup at the end of the last session, and how to continue.
Read with `docs/plan-partials.md` (cleanup plan, hooks A0-A5), `docs/plan-big-units.md` (unit workflow, validation
V1-V6) and `CLAUDE.md`.

## Where things stand

- `main` at `dd4e3af`, clean; `tools/check.sh` exits 0 on main.
- Totals (check.sh): 12131 functions decompiled in 662 files; 588 files complete, 74 not.
  Libraries: 916/985 functions in 153 files (134 complete).
- `partials.py --summary`: PARTIAL 1023, MATCH 136; foreign stand-ins 323 (120 files); padding 30 arrays;
  mechanisms 958 sites (911 untagged); classes complete 573, superset 129, partial 94, gated 93, missing 9.
- All units of `plan-big-units.md` are done. Units are now taken in the order "most asm left among units that still
  have never-attempted functions".
- No git remote: never push. Local leftovers, not part of the work: `stash@{0}` (old Game-Entity WIP) and the
  untracked `docs/plan-partials-prompt.md`; leave both alone.

## Done in the last session (all merged into main)

| Item | Result |
|---|---|
| Game/Collision/CollUtilO | 15/16 from C (`_detectColumnColumnEz` parked); CCollUtil all ref methods in CollUtilA.h; COLL_CUBE/MCD_INFO/OCD_WORK moved there; MainApp's CCollUtil copy removed |
| System/MainApp | +5 matched (`_VBlankStartHandler`, `_initFntTbl`, `_halt`, `_mainProc`, `_mainLoop`); `setSysPause` parked; 5 stand-ins replaced; MainApp.h SOFTRESET_MODE/CMainApp in ref form |
| Game/Enemy/HumanBoss | +2 matched (`enemyCardProc`, `stockActProc`); `setupCardRevolver` drafted and parked; 7 parked in total; ENEMY_PARAM `iEnemyCard`/`byEnemyCards` fields |
| Cleanup batches | WorldEnv.h (PackMan), PadIO/Movie/PlaySpeedCtrl (MainApp), Player.h (6 files) |

Per-unit and per-cleanup logs: `build/progress/<Unit-dashes>.md`, `build/progress/cleanup-*.md`.

## Next steps, in plan order

1. **Between-units cleanup batch (A3):** top of `partials.py --queue` is `Game/Enemy/BattleEnemyManager.h`
   (11 stand-in files, R1). Then `Game/CardBattle.h`, `Game/Atel/Atel.h`, `Game/Gimmick/GimmickManager.h`, ...
   One queue item per gap between units.
2. **Next unit** (start hook A1, functions, finish hook A2 parts 1+2). Units with never-attempted functions:

   | Unit | not attempted | INCLUDE_ASM left |
   |---|---|---|
   | Game/Movement | 2 | 3 |
   | System/Graphic/FrecFont | 2 | 3 |
   | System/Graphic/PathData | 2 | 2 |
   | System/Graphic/2d/2DSprObj | 2 | 2 |
   | System/Graphic/ShadowModel | 1 | 2 |
   | System/Graphic/HermitePathCamera | 1 | 1 |

   After these, every remaining function is parked: revisit parked drafts with the levers below
   (`tools/leftovers.py game|lib` lists them with their `Parked:` notes).
3. Candidate follow-up: CollUtil's parked `distanceLine` (215/506) has the same layout as CollUtilO's
   `distanceLineO`, matched with the loop-rotation levers below.

## Parked functions touched last session (diff lines / total)

- HumanBoss: `initalize` 4/71 (setEntityFlag vs getParam schedule), `setStockCard` 6/97 (reorg/cross-jump of the
  return-FALSE paths), `itemCardProc` 15/269 (first rollDeck tail cross-jumped; tail shapes tried),
  `normalActProc` 20/159 (two setDoBattleAct/useCard tails merged), `setupCardRevolver` 21/371 (param pointer
  register copy across loops), `setupAct` 22/207, `getAttackCardNum` 27/47.
- MainApp: `setSysPause` 5/12 (registers only).
- CollUtilO: `_detectColumnColumnEz` 7/125 (radius load order).

## Levers that worked last session (details in the memory index)

- **Loop rotation:** a leading `b` to a test placed at the bottom = gcc 2.95 `expand_end_loop` rotation. Write
  `for (;;) { ...; if (bad) break; ...; return 1; }` or `while (!(ret = f()) && ...) { ...; break; }`.
  An empty `do {} while (0);` is a scheduling barrier (call arguments set up after preceding stores).
- **Return stubs:** `if (ret) { ...; return 1; } return 0;` keeps the first return-0 stub; `goto end` + `end: return r;`
  after a loop avoids a const-propagated `li v0,1` without rotating the loop (break would).
- **Constants in registers:** a local `TIMEQ nMax = 200;` keeps a clamp constant hoisted (slt/movz instead of slti/movn).
- **sqrt:** a local early-clobber `"=&f"` sqrt helper when every original `sqrt.s` has distinct in/out registers.
- **Register/frame:** one shared function-scope temp for two branches gives the same hard register; declaration
  order sets stack slots; assign a pointer before an ASSERT call to keep it callee-saved; one `INT i` per loop.
- **Reloads:** `volatile` globals (`_nVWaitCount`) or volatile reads of a pointer/field where the original reloads.
- **Cross-jump shapes:** two identical tails where the original merges only part: write the last copy as
  `if ((x & 1) == 0) { ...; return TRUE; } return FALSE;` and the earlier one the other way round.

## Working rules (unchanged)

- Main checkout only, strictly sequential, no sub-agents/worktrees; permuter's parallel compiles are fine.
- Only `tools/partials.py` may change under `tools/`; no changes to `configure.py`, `config/` (except symbol_addrs
  naming lines) or build flags.
- Header edits: full `ninja` must pass the SHA-1 at every commit; `imagediff.py` for differences;
  `partials.py --header` recheck afterwards.
- `tools/check.sh` exit 0 before every merge into main and again on main; then `docs: regenerate partials`
  (`partials.py --update docs/partials.md`, `progress.py --update README.md`).
- Branches: `cleanup/<...>` for cleanup, `decomp/<Unit-dashes>` for unit work (start/finish hooks included); never mix
  function matches into cleanup branches. If a branch name exists already (old merged work), add a suffix.
- Kept mechanisms get `MATCH: <reason>`; PARTIAL comments go only when the class equals the ref.
- Park form: `/* Parked: ... */` + `#if 0` draft (+ `#else` string placeholder/INCLUDE_ASM when the function has strings).
- Never commit `iso/`, `ref/`, `asm/`, `build/`. Commit trailer: `Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>`.

## Handy scripts in build/ (not committed)

- `build/progress/model_bigbang/multi.py <src> <mangled> <unit> build/v.json`: test source variants
  (JSON list of `[name, [[old, new], ...]]`).
- `build/progress/Game/Player/permute_method.py <unit> <Class::method> <mangled> --minutes N -j 6`: permuter for a
  member function (keep `timeout` below 590 s).
- `build/act_if0.py` / `build/act_after.py <src> <mangled>`: variant JSON that enables a parked `#if 0` draft
  (before resp. after its INCLUDE_ASM).
- `build/swap_one.py <src> <Class> <header>`: replace a stand-in class with an include.
- `build/sbs.py`: side-by-side view of funcdiff output; `build/skel.py <mangled>`: call/branch skeleton compare.
