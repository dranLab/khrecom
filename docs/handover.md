# Handover (2026-10-30, updated after the second session)

State of the matching decompilation and the PARTIAL cleanup at the end of the last session, and how to continue.
Read with `docs/plan-partials.md` (cleanup plan, hooks A0-A5), `docs/plan-big-units.md` (unit workflow, validation
V1-V6) and `CLAUDE.md`.

## Where things stand

- `main` clean (last commit: `docs: regenerate partials`); `tools/check.sh` exits 0 on main.
- Totals (check.sh): 12135 functions decompiled in 662 files; 588 files complete, 74 not.
  Libraries: 916/985 functions in 153 files (134 complete).
- `partials.py --summary`: see `docs/partials.md` (PARTIAL about 930, foreign stand-ins 102 in 68 files; they were 323 two sessions ago); padding 26 arrays; mechanisms 959 sites (909 untagged); classes complete 575, superset 129,
  partial 93, gated 92, missing 9.
- Units of the "never attempted" table are done except the ones below (every remaining function there is either
  parked or noted `Not attempted:` with the reason). Local leftovers, not part of the work: `stash@{0}` (old
  Game-Entity WIP) and the untracked `docs/plan-partials-prompt.md` (never `git add docs` blindly: use file names).

## Done in the last session (all merged into main)

| Item | Result |
|---|---|
| Cleanup BattleEnemyManager.h | 11 stand-ins replaced (`build/swap_keep.py` keeps the line layout with blank lines) |
| Cleanup CardBattle.h | full `CCardRevolver` moved from CardRevolver.h into CardBattle.h (ref owner); partial copy and its gate removed (PARTIAL -30, duplicates -1) |
| Cleanup Atel.h | 7 stand-ins replaced; `friend class CEntityFactory` (MATCH) for the protected static `createAvatar`; DebugWindowTBL's ATP/Actor/CVSM stand-ins now in `namespace ATEL` |
| Cleanup JMemoFlag.h | 6 stand-ins replaced |
| Game/Movement | `detectWall_` matched (permuter found the statement order; 4 plain C moves); constructor parked (named statics vs the original's constant-pool loads, 54/143), `calcPos_` stays parked (5 lines) |
| System/Graphic/FrecFont | `getDispSize` matched; `makePkt` noted `Not attempted:` (656 instr, stack-heavy MMI colour code) |
| System/Graphic/2d/2DSprObj | `initForSeq` parked at 20/342 lines (prologue schedule + sway block); `move` noted `Not attempted:` (flow written in the note) |
| Game/Collision/CollUtil | `distanceLine` matched with the distanceLineO levers (`for (;;)` + `break`, `_sqrtfE`, one `l2` temp, `g = f1*f1; g -= 1.0f;`) |

Second session (cleanup batches, all merged; scripts `build/batch_swap.sh <Class> <header> <branch>` and
`build/finish_batch.sh <branch> <commit msg> <merge msg>` do swap+build and commit+gate+merge+docs):

| Item | Result |
|---|---|
| Buf.h, VSME.h, SaveLoad.h, Game.h, Camp.h, MotData.h | small stand-in swaps; VSME.h: CVSM `loadFromMem/execInit/exec` public as in the ref, flag accessors inline only under `GAME_ATEL_VSME_CPP` (MATCH: Atel.o calls them out of line) |
| Pause, BossDeadDemo, MessageManager, Glare, TexCtrl, GameOver, LevelUp, RoomCreation, EffectMotion, Wnd, MovieScript, Roulette, LockOnManager, MinigameManager, SaveLoadUtil | 1-3 files each; includes inside `namespace X {` must move out (the stand-in sat inside it) |
| Rcf.h | 23 of 24 CRcf stand-ins replaced (DebugMenu keeps its own RcfSceneInfo/RcfMapInfo; cc1plus fails silently) |
| RsrcDB.h (R3) | `NAME_CASE` + `struct RsrcInfo;` live in RsrcDB.h, which no longer includes RsrcData.h/Task.h; RsrcData.h includes it and holds PackedRsrcHeader/CRsrcIMZ; 66 CRsrcDB and 30 NAME_CASE stand-ins replaced (stand-ins -96) |

Third round (item 2 of the suggestions, merged): `Game/Collision/CollInfo.h` (COLL_INFO_UNION; `CCollObj::getDetectInfo`
returns it, TriangleIK keeps INT via `COLLOBJ_H_INFO_AS_INT`), the partial `CMovement` and `MOVEMENT_H_FULL_CLASS` are gone
(191 call sites moved to the real `setFlag/setCollisionFlag(set, mask)`, `setVel/setScl/setPos`), `Game/CardKindEnum.h` (full
`CARD_KIND`) with `GimmickManager.h` ungated and 7 `DropPrize` stand-ins replaced, `System/Graphic/TexInfo.h` (one TexInfo/VTransInfo
for Gprim.h, modeldata.h and 9 units; TexCtrl reads the flag bits itself, MATCH).

Kept stand-ins with a `PARTIAL: stand-in for ...` reason: MSVideo CPause (`_ref(getResult)` scheduling), TexCtrl CRsrcDB
(CRsrcList typedef clash), MovieScript CTextWnd (assembler crash), DebugMenu CRcf.

Per-unit and per-cleanup logs: `build/progress/<Unit-dashes>.md`, `build/progress/cleanup-*.md`.

## Next steps, in plan order

1. **Cleanup (A3), what is left needs a decision or a unit hook:**
   - `C2DMan` (28 files, gated by `D2MAN_H_PATH`/2DMan.h) and `CRsrcData`/`RsrcInfo`/`CRsrcANN` (RSRCDATA_H_FULL):
     plan items 7 and 8; the string evidence for 2DMan.h is ambiguous: ask the user.
   - Task SLC overflow message (plan item 3, 158 opt-out defines): 99 originals have the message; all 84 of our
     `TaskMan.h` includers are among them, but 15 more originals have it without our TaskMan.h include
     (BtlReportCtrl, GameOver, LevelUp, TitleReportDllStarter, WorldCard, BulletManager, BaseGimmick, ...), so the
     partition is not decided by the evidence: ask the user before moving `CTaskSLCManager`.
   - `ShadowModel.h` (`CShadowModelMan`, 3 files use its private statics): complete the class in the ShadowModel unit's
     finish hook; `CPlayerDRiku`/`CPrize`/`C2DSeq` (virtual order differs).
2. **Units that still have functions never attempted** (all large, each 300-670 instructions):

   | Unit | not attempted | note |
   |---|---|---|
   | Game/Movement | constructor (listed as "not attempted": leftovers.py misses constructors in drafts) | parked 54/143 |
   | System/Graphic/FrecFont | makePkt (656) | see its `Not attempted:` note |
   | System/Graphic/PathData | getNearPath (586), dispDebugLine (489) | |
   | System/Graphic/2d/2DSprObj | move (438) | flow in its note |
   | System/Graphic/ShadowModel | getScreenAreaClipToWorldPos (671) | drawBefore parked |
   | System/Graphic/HermitePathCamera | updatePos (632) | |

   After these, every remaining function is parked: revisit parked drafts with the levers below
   (`tools/leftovers.py game|lib` lists them with their `Parked:` notes). Near misses worth another look:
   2DSprObj `initForSeq` 20/342, Movement `calcPos_` 5 lines (`move t0,v0` / `move s2,v0` around the last two
   detect calls), CollUtil's other parked functions (`_detectOCD` 2 lines, `detectMap`, `detectMCInit`).

## Parked functions touched in earlier sessions (diff lines / total)

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

- **Bitfield test as a value:** `BOOL b = m_status.x_stop; if (b) ...` gives the original's `dsll/dsra32/andi 1`
  extraction instead of a plain `andi` (2DSprObj initForSeq).
- **Duplicate arms the original keeps:** write the arms inside one case with preloaded locals
  (`FIX4 ww = prim.m_Size; FIX4 sp = space.x; if (!b) x += A; else x += A;`); an inline helper lets gcc cross-jump them.
- **Reuse a variable for the intermediate:** `nStyle &= 7;` before the loop (register reuse of the style word),
  `FIX4 xNew = x + d; x = xNew;` (temp + move), `g = f1*f1; g -= 1.0f;` (product and difference share a register),
  `USHORT nPPW` for a byte width that keeps `if (n < base) n++` as a branch.
- **Signed vs unsigned compares:** `UINT c` with `switch ((INT)c)` gives sltiu for range tests and slti inside the switch.
- **permuter on a member function:** `build/progress/Game/Player/permute_method.py <unit> <name> <mangled>` needs a name
  that the source regex finds: use the bare method name (`CMovement`, not `CMovement::CMovement`).

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
- `build/swap_one.py <src> <Class> <header>`: replace a stand-in class with an include (shortens the file);
  `build/swap_keep.py` (same arguments) keeps the line count with blank lines, which is what `__LINE__` sites need.
- `build/sbs.py`: side-by-side view of funcdiff output; `build/skel.py <mangled>`: call/branch skeleton compare.
