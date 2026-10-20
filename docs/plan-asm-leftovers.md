# Plan: finish the units that still use INCLUDE_ASM / asm data (smallest first)

## Context
On main (b1a2978), 509 units have source. 415 are complete; 94 are partial with 2295 functions still INCLUDE_ASM.
The goal is to bring each of these units to "all functions C + `// SOURCE DATA`", the unit with the fewest asm
functions first.

**Out of scope**
- SDK units (`lib/...`): they have a separate plan (docs/plan-library-decompilation.md).
- Hand-written `.s` units (`crt0`, `System/expand`): they stay assembly.

libppp and `sddrv_ee` are the game's own code (MainApp paths), so they are in scope.

The worklist comes from `build/check.overview.txt` and from checking for `// SOURCE DATA` in each source.
Tiers are by INCLUDE_ASM count:

| Tier | Asm fns/unit | Units | Examples |
|---|---|---|---|
| 0 | 0 (data only) | 1 | Game/Collision/Ride (misaligned .rodata, see memory misaligned-unit-rodata) |
| 1 | 1 | 41 | Roulette, Status, LinkedBuf, CollUtilA, Math3D, dlmalloc, libppp KeShpTail*/KeBornRnd, … |
| 2 | 2 | 13 | DebugWindow, CollDisp, CollMap, 2DSprObj, Wnd, sddrv_ee, partMng, PathData, … |
| 3 | 3 | 10 | DebugWindowTBL, AtelCrossFade, CollAttack, GimmickTreasureBox, Movement, effect, Memoca, … |
| 4 | 4–9 | 9 | EffectMotion, RcfCD, Game, PlayerDRiku, CollUtilO, DeFocus, MainApp, TexUtil, HumanBoss |
| 5 | 13–26 | 5 | CollUtil, Rcf, Camp, PlayerSora, Map |
| 6 | 42–321 | 16 | PackMan 42 … CardRevolver, SceneInfo, PlayerGauge, BattleCard, DebugPathData, CardBattle, SaveLoad, VSME, Avatar, Entity, JiminyMemo, Model, BattleEnemy, Player, Gprim 321 |

The exact ordered list is regenerated at the start of each session (step 1), so this table can go stale.

**Important:** most leftover functions in tiers 1–3 were already attempted. Their source often holds a comment
with the near-match and the remaining difference (e.g. `src/Game/Status.cpp:64`, `src/System/LinkedBuf.cpp:86`).
These are hard tails, not fresh work, so they get a time box.

## Workflow per unit (inline, no workers; see memory project-goal-matching)
1. **Pick the next unit.** Regenerate the ordered worklist from `build/check.overview.txt`: sort by the
   INCLUDE_ASM count, then by unit size (`tools/asm.py <unit>` sizes) to break ties. Skip units marked "parked".
2. **Branch:** `git checkout -b decomp/<Unit-with-dashes>`.
3. **For each INCLUDE_ASM function:**
   - Read the existing note or near-match C above it, and the matching memory lessons (MEMORY.md index:
     scheduling, cross-jump, gcse, loop-hoist, placeholder recipes, …).
   - Use `tools/funcdiff.py <unit> <fn>` and `tools/compare_compilers.py` to iterate. Try the levers from
     memory (statement order, `_ref()`, register asm, split statements, hill-climb/permfn scripts in `build/`).
   - **Time box: about 45 min of attempts per function.** If it still doesn't match, keep INCLUDE_ASM, update
     the comment with the best diff and what was tried, and move on.
4. **Data:** once no INCLUDE_ASM function needs asm data labels (or by using the placeholder recipes from memory:
   include-asm-with-source-data, rcf-placeholder-and-volatile), add `// SOURCE DATA: …` as the last line and
   check with `tools/imagediff.py`.
5. **Gate:** `tools/check.sh` must exit 0.
6. **Commit and merge:**
   - Commit on the branch.
   - `git checkout main && git merge --no-ff decomp/<…> -m "Merge decomp/<…>: <unit> (complete|partial, a/b)"`.
     This is the existing convention (see `git log --merges`).
7. **Memory:** if a new matching lever was found, save a memory file plus a MEMORY.md line (as existing
   "*-unit-lessons").
8. **Parking:** if every remaining function of a unit hit the time box, the unit is "parked". Record it in a
   short list at `docs/asm-leftovers.md` (unit, function, remaining diff). Revisit parked units after tier 3,
   since lessons learned on later units often unlock earlier ones.

## Order of execution
1. Tier 0, then tiers 1 → 3, smallest unit first within each tier. This is about 65 units and about 110
   functions, mostly hard tails.
2. Tiers 4–5 (14 units, ~150 functions). Here whole fresh functions remain, so the same loop applies with the
   fresh-function workflow from CLAUDE.md.
3. Tier 6 (16 large units, ~2000 functions). Work through them function by function, smallest function first
   inside each unit. Commit and merge in chunks of roughly 10–20 matched functions so main always passes
   check.sh.
4. **Side item, during tier 6:** the "NOT matching" `__static_initialization_and_destruction_0` (and
   `DWTPlayer::Debug*`) flags in Model, PackMan, JiminyMemo, VSME, BattleCard, Player and PlayerGauge. The
   image checksum passes, so first confirm whether these are objdiff relocation artifacts. Fix them when the
   unit gets its SOURCE DATA.

## Verification
- `tools/check.sh` exits 0 before every merge; the image SHA-1 is the single source of truth.
- Progress is tracked by the `total:` line and the per-unit counts in `build/check.overview.txt` (now 9023/11318
  functions).
- `tools/imagediff.py` must be clean whenever `// SOURCE DATA` is added.

## Housekeeping
- During planning, a scratch file `build/nosrcdata.tmp` was created (the list of units with asm data). It lives
  in ignored `build/` and can be deleted.
