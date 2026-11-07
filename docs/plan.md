# Plan: finish the matching decompilation

The one plan for the remaining work. Read it completely, then `CLAUDE.md`, then the memory index (`MEMORY.md`,
loaded at session start) before touching code. It replaces the earlier plans (big units, asm leftovers, PARTIAL
cleanup, library decompilation) and the handover notes; their history is in git.

Reference docs, kept current by tools (never edit their generated parts by hand):

| doc | what | regenerate |
|---|---|---|
| `docs/asm-leftovers.md` | every game function still `INCLUDE_ASM`, with its `Parked:` note; a hand-kept table of structural blockers at the top | `tools/leftovers.py game --update docs/asm-leftovers.md` |
| `docs/sdk-leftovers.md` | the library reference: open functions, how to work on lib units, levers, notes per unit, search history | `tools/leftovers.py lib --update docs/sdk-leftovers.md` |
| `docs/partials.md` | the header-cleanup report (PARTIAL markers, stand-ins, mechanisms, class readiness); its trend table is hand-kept, one row per cleanup merge | `tools/partials.py --update docs/partials.md` |
| `README.md` | the progress table | `tools/progress.py --update README.md` |

## Keeping the docs current

The generated parts come from the sources (`Parked:` notes, `PARTIAL`/`MATCH` comments), so they go stale with
every change that is merged without regenerating. Regenerating all four takes about 10 seconds:

    .venv/bin/python tools/leftovers.py game --update docs/asm-leftovers.md
    .venv/bin/python tools/leftovers.py lib --update docs/sdk-leftovers.md
    .venv/bin/python tools/partials.py --update docs/partials.md
    .venv/bin/python tools/progress.py --update README.md
    git status --short docs/ README.md

- **After every merge into `main`** (unit chunks, finished units, cleanup batches, library work, docs or tool
  changes): run the block on `main`, then commit what changed as `docs: regenerate leftovers and progress`.
- **At session start**, before any work: run the block. If anything changes, the docs were stale: commit it first,
  so the session starts from current docs.
- **Hand-kept parts**, updated in the same merge as the change that affects them:

  | part | update when |
  |---|---|
  | "Where things stand" and the work table below (this file) | a work item finishes or the order changes; at the end of every session (numbers and date). Drop a unit from the work table once it has no unparked function left: `tools/partials.py` reads the table to tell which owners are in progress. |
  | the blocker table at the top of `docs/asm-leftovers.md` | a blocker is found or resolved |
  | `docs/sdk-leftovers.md` "Working on a library unit", "Levers", "Notes per unit" | a library lever or finding appears, or a note there no longer holds (e.g. a listed function got matched) |
  | `docs/sdk-leftovers.md` "Search history" | after a permuter sweep over the library |
  | `docs/partials.md` trend table | one row per cleanup merge (`partials.py --summary` before and after) |
  | `CLAUDE.md` | a tool, rule or toolchain fact changes |

## Where things stand (2026-11-07)

- **Progress:** 12047 of 12311 functions matched: game 11264 of 11323, libraries 783 of 988 (plus 151 hand-written).
  `tools/check.sh` counts the 151 hand-written units as done too: its total is these matched functions plus 151
  (fixed 2026-11-07: it used to count a parked `#if 0 ... #else INCLUDE_ASM #endif` as done).
- **Game:** 59 functions still `INCLUDE_ASM`. Nearly all are parked (C draft under `#if 0` with a note).
  5 units take a data section from assembly.
- **Libraries:** 18 parked, 1 not attempted, 27 hand-written (stay assembly), no unit with data from assembly
  (the last two, __assert and __umoddi3, needed the split / link-alignment fixes of 2026-11-07);
  details in `docs/sdk-leftovers.md`.
- **Header cleanup:** PARTIAL 185 (155 in 44 headers, 30 in 17 .cpp), 11 foreign stand-ins in 6 files, 0
  duplicate classes, 0 untagged mechanisms; classes complete 873, superset 12, gated 3, missing 8.
  `tools/partials.py --queue` has the ready items.
- **Method that paid off (2026-11-06):** read `tools/helpers/asmlines.py` before permuting. Most long-parked drafts had a
  wrong source shape the line markers show directly (one-line brace init, a condition on one line, statement order,
  a direct assignment), not a scheduling accident. See memory void-callee-v0, fvector-brace-init-aligned,
  gcse-bucket-count-lever. `tools/helpers/lineorder.py` compresses the markers, `tools/helpers/normdiff.py` is a register-blind diff,
  `tools/helpers/lregpri.py` ranks pseudos by global-alloc priority, `tools/helpers/secsize.py` compares section sizes with the
  split, `tools/helpers/bracetry2.py`/`bracetmp.py` sweep drafts for brace-init forms.

- **Next method: compare drafts with the ref's locals (2026-11-06).** It matched 23 functions in one sitting (Gprim 12,
  Model 2, CollUtil, CollBody, ShadowModel, EffectMan, BattleEnemyManager, Rcf, BattleCard, ModelMan, AttackMotion 1 each; onMove 53 -> 1, detectMCInit 48 -> 18 lines) after three permuter queues had matched two. The ref lists every local and
  inline scope in order, so a missing or extra local means a wrong source shape: invented `flag`/`bTop` variables, a
  counted-down loop where the ref has `i`, asm helpers for what gcc emits itself (a `por` after `pcpyud` is a DI -> TI
  conversion, `*(UINT*)&f` gives the `mfc1`). `tools/helpers/reflocals.py > build/jm/reflocals.log`, then `tools/helpers/refjoin.py 60` (it and refextra, refcount,
  regswap, banddiff read `build/drafts.log`: `tools/drafts.py -j 12 > build/drafts.log` first),
  ranks near misses; `build/remeasure.log` was refreshed 2026-11-06 evening (tried without gain: createBootInfoMsg,
  setStockInfo, _invokePrimary_Main, dispDebugLine, setCharge, setDeckInfo, pppSetMatrix, HumanBoss initalize, effect Setup,
  checkBBox, setAttack (power forms), initEnemyParam (block order), sceVif1PkCloseUpkCode (the ref's locals: 25 -> 43-50), setScreenFader; 2DSprObj move matched 2026-11-07). `tools/helpers/refextra.py 40` lists drafts with invented locals, the better lead. Parked notes go stale: read the current diff first (stateDestroy and setupDrawMode were one statement swap).
  A permuter queue over the eight drafts changed on 2026-11-06 gained on two (the last fan 3 -> 2, detectMCInit 18 -> 14);
  onMove, acWaitReq, update, _atoi, _atolen and checkBBox held. `tools/helpers/regswap.py 60` separates pure register permutations (solve with the priority formula), `tools/helpers/refcount.py` lists
  locals declared fewer times than in the ref (one variable per loop: Model::link), `tools/helpers/banddiff.py lo hi` prints current diffs,
  `tools/helpers/permlines.py` sweeps the order of adjacent statements (no gain on initAttackField, setupFontTex). Check the helpers
  first: `asm volatile` against plain `asm`, and `=&r` where the original keeps inputs and output apart. See memory
  ref-locals-reveal-shape, ti-di-conversion-is-por.
- **Read the scheduler and reorg, then pin (2026-11-07).** 35 functions in one sitting, most long parked as "scheduling tie"
  (setCharge, initAttackField, setConfuse, CHumanBoss::initalize, CNakCamera::main, beginStateHitAttack, acWaitReq,
  createBootInfoMsg, setStockInfo, CBattleCard::destroy, the Movement constructor, testPlayingCard, rollDeck,
  getApplyMatrixBBox, checkBBox, pppSetMatrix, _invokePrimary_Main, the last fan, _sceRpcGetPacket, changeMode, replaceNorma,
  itemCardProc, initRecover, chkDamage, _hokanRot, the three CollAttack checks, setStatusWin1, setupFlick,
  exeMessageDlg, getRandomTargetPos, _sceCd_cbLoop, EFFECT_DATA::Setup, normalActProc). Dump `-dS -dR -fsched-verbose-9`, `tools/helpers/schedshow.py <.sched2> <fn signature> <rtl key>` prints the
  successors, priorities and issue order for the block. Ask what the original's order implies, in this order: a tail written out
  (call-arg-order-dependents), a cast store (fixed-scalar-varying-struct-alias), a clobber that marks a register dead for reorg, a load inside both arms for a cross-jump
  (crossjump-block-end-call), an empty asm in one arm to stop one,
  and only then issue-order groups with empty barriers and register locals (issue-order-pins). Guarded order searches found
  nothing more on the five reverted drafts: their honest scores stand at 30, 27, 9, 5, 31 (setStockInfo then matched by reading the cross-jump).
- **Re-measure before choosing (2026-11-07):** `tools/drafts.py` scores every parked draft, closest first (about 10 minutes
  with -j 12), whatever its layout, and gives the first error of a draft that does not build (`--failed`); notes go stale.
  Its numbers are funcdiff's, except for the functions whose gcse order varies per compiler run (CRcf::finalize,
  _parseRCF_EntryFile: gcse-pre-hash-order). (It replaced `remeasure.py`/`rmsort.py`; the helpers find drafts with its logic, `tools/helpers/draftlib.py`.) Notes go stale after header fixes (chkDamage's note listed four open
  spots, two were gone). 15 drafts report build-failed because a stopgap data block or an extern declaration clashes when they are
  enabled: `tools/helpers/en.py` takes a variant that drops it. Those are unmeasured, not far: the three CollAttack checks were each 2 lines.
- **Constructor with pool constants (2026-11-07):** CMovement went 53 -> 26 -> 0 once the two constants sat in `u_long128`
  locals (`vZero._q`, TImode pseudos): both lq first, registers and the vtable store right at once (16 lines). The rest was
  store order, so write chains and `init()` out as single stores and run the guarded search. See memory timode-local-pool-constants.
- **Guarded order search over 25 drafts (2026-11-07, 2-37 lines):** nothing gained. finalize's reported 17 -> 16 is noise: the same source gives 17, 17 and 11 on identical builds (gcse hash order, as
  _startCdRead), so its score means little. What is left there is
  register allocation or scheduling, not statement order: do not rerun it on these drafts unless their source changes.
- **Order searches (2026-11-07):** `tools/helpers/hillqueue.py <min> <unit:name>...` climbs runs of simple statements; `tools/helpers/hillstmt.py`
  one chosen run. Real gains: Movement ctor 53 -> 26, CImageWnd::init 14 -> 5, calcWorldBBox 45 -> 28, finalize 17 -> 11,
  setupTexInfo 10 -> 6. **A lower line count proves nothing about behaviour; only a match does.** Five moves were wrong and
  were reverted (procFieldNormal, setStockInfo, drawCrossFadeImage, getAttribute, _parseRCF): read every move in context before
  committing it. The queue now refuses moves its dependency guard flags (`conflict`). See memory hillclimb-breaks-semantics.
- **Permuters are exhausted for now (2026-11-06):** three 10-minute queues covered every parked game draft 4-45 lines off
  (about 73 drafts, 12 hours). Only endFieldSub and getNearPath gained, and only their statement changes counted. Do not
  rerun them on a draft whose source did not change; read its line markers instead. tools/permuter.py can't enable some
  drafts and then reports an instant MATCH on the asm (Setup, AttackMotion initialize, CZFill::makePkt, the fans): check
  with `tools/helpers/showdraft.py`, not with its score.

Numbers change with every merge: `tools/check.sh` (totals), `tools/leftovers.py game|lib` (what is left) and
`tools/partials.py --summary` (cleanup) give the current state.

## Remaining work, in this order

1. **Parked game drafts**: retry them closest first, line markers first, then the levers index and both permuters.
   List them with their notes: `tools/leftovers.py game` ("Parked functions"). A private member a match needs: a `friend` on an existing line of its class (user decision 2026-11-07:
   CardRevolver main, HumanBoss getAttackCardNum/setupCardRevolver), never a changed access specifier.
   The hand-kept table at the top of
   `docs/asm-leftovers.md` lists the blockers that need more than a source change.
2. **Data from source**: CollUtil, CollUtilO and Ride (.rodata). The split fix of 2026-11-07 (a code address is no
   string start) put their boundaries right (CollUtil ends at 0x4893D8 with CCollision's vtable; CollUtilO and Ride
   now start 8-aligned): what is left is the header strings (CollUtil.h's inline asserts, twice, in CollUtilO; the
   extern D_ strings become the inlines' own). `pppEiDrawShipoly` (.bss .data .rodata) as before. `sddrv_ee`'s `.bss` is blocked (see the
   table at the top of `docs/asm-leftovers.md`).
3. **Library leftovers**: the parked and not attempted functions of `docs/sdk-leftovers.md`, with its "Working on
   a library unit" section.
4. **libppp's MIPS-in-asm (decided 2026-11-07, kept with MATCH evidence):** 23 matched functions keep GPR instructions
   in their asm. 7 are the original's hand-written asm (`.set noreorder` with hand-filled delay slots, trapping
   add/addi: init_spline x3, KeDttMdl_MkGifPk x2, KeDttSpr_MkGifPk, op_md_draw_path3_fog); the md0 family's C forms
   change the whole function (COPY_Q tested); pppColor's split form (C + one asm per VU0/MMI instruction, volatile
   max for the "m" operand) reaches 3 lines: the route for the 10 not attempted yet (their MATCH comments say so).
   kh1 has none of them in C. No matched function goes back to parked for this.
5. **Header cleanup**, interleaved: one `tools/partials.py --queue` item between two work items (see "Header
   cleanup"). The items waiting on a user decision are listed under "When to stop and ask".

Work on one unit at a time, one function at a time. A unit is done when every function is matched or parked and
its data comes from source (or is recorded as blocked). Never leave a function that was neither tried nor parked.

## Working rules

- Sequential in the main checkout by default. Parallel work only through `/decomp-pool` (user decision 2026-11-07):
  one unit per `decomp-worker` in its own worktree (that is this rule's form for a pool worker, not a deviation), mode "parked" (the unit's drafts from `tools/drafts.py`) or
  "new"; the coordinator merges one worker at a time with the gate. Parallel compiles inside `tools/permuter.py`,
  `tools/decomp_permuter.py` and `tools/drafts.py` are fine.
- No changes to `tools/`, `configure.py`, `config/` or the build flags, except `tools/partials.py` (approved)
  and `config/SLUS_217.99/symbol_addrs.txt` lines that name unnamed weak or vtable copies. Anything else: ask.
- Only real C/C++ (see V4). Keep `MATCH: <reason>` on every kept mechanism.
- Pushing `main` is the user's (origin is github.com/dranLab/khrecom; CI runs `tools/check.sh` on every push): never
  push unless asked, never rebase `main` or force anything. Never commit `iso/`, `ref/`, `asm/` or
  `build/`. Never use `git stash`. Keep `docs/plan-partials-prompt.md`-style local files out of commits: add
  files by name, never `git add docs`.
- Keep scratch files in `build/` (WSL wipes `/tmp`). Use `.venv/bin/python`.
- Don't run `ninja` or `tools/check.sh` while a background permuter queue is in its setup step: both call ninja
  (a collision fails the setup; queue scripts should retry).

## Session start (every session, also after a context compaction)

1. `git status` clean, on `main` or on the unit's branch `decomp/<Unit-with-dashes>` (slashes become dashes).
   Create the branch from `main` when you start a unit.
2. `tools/check.sh > build/check.log 2>&1; echo $?` must print 0 before you change anything. If it doesn't, find
   out why first (a stale tree usually just needs `configure.py`). Then the docs block of "Keeping the docs
   current": commit any change before starting.
3. Read the unit's memory file and the levers index below. Open `build/progress/<unit>.md` if it exists.
4. List the unit's remaining functions: `tools/leftovers.py game --unit <unit>` (state, size, note) and
   `tools/asm.py <unit>` (every function, size, weak/global).
5. First session of a unit: the start hook (header cleanup, A1) before the first function.

## Order of functions inside a unit

- The smallest remaining non-weak function first. Small functions (getters, state wrappers, forwards) fill in
  the class layout and the headers that the big functions need.
- Generated families first where memory has a generator (state wrappers from the vtable order; `acWait*`
  through `tools/helpers/vsgen.py`; Gprim's VU0 helpers through `tools/helpers/gpconv.py`).
- Weak functions are inline functions: do them only when their class definition is right, in the class body (or
  `inline` in the header) in the original emission order (key-function-emits-all-inlines,
  weak-emission-definition-order, inline-called-not-inlined).
- Parked drafts last, with fresh eyes once the rest of the unit is done.
- The source keeps the original function order: replace an `INCLUDE_ASM` line in place.

## The per-function loop

For one function `F` of unit `U`:

1. **Study.** `tools/asm.py U F`. In `ref/SLUS_217.99/src/U.cpp`, find the signature, parameter names and
   locals (the ref's locals and their scopes are evidence: block-local variables, inline temporaries). Look up
   only the types you need in `ref/SLUS_217.99/types.h` (`grep -n -A<n>`). Read the note or draft above `F`.
2. **Write** the C/C++ in place of the `INCLUDE_ASM` line. Keep the original line numbers wherever `__LINE__`
   reaches an assert or allocation message (`#line N` to resync). Without `// SOURCE DATA` the compiled
   `.rodata` is stripped: string literals and constant tables in new code refer to the original's labels as
   `extern const char D_xxxxxxxx[];`.
3. **Iterate** with `tools/funcdiff.py U F` (`|` differs, `~` only an offset or address). One change at a time,
   guided by the levers index.
4. **Near miss** (same structure, roughly 20 differing lines or fewer): run a permuter before parking. Both
   switch a parked `#if 0` draft on in their own copies.

       .venv/bin/python tools/permuter.py U F --minutes 15 -j 16          # C and C++: text mutations
       .venv/bin/python tools/decomp_permuter.py U F --minutes 15 -j 16   # .c units only: AST randomizations

   `tools/permuter.py` writes `build/permuter/F/best.diff`. `tools/decomp_permuter.py` ends with its best
   outputs rescored in differing lines, next to its own base (go by that count, not its score), in
   `build/decomp-permuter/F/output-*/diff.txt`. Its randomizations sometimes change the meaning and often add
   noise (duplicated `if` arms, casts): carry over only the sensible part by hand, then check it in the real
   source. A gain often depends on the noise and disappears without it. For code it can't parse:
   `--setup-only`, wrap the construct in `PERM_IGNORE(...)` in `base.c`, `--resume` (asm statements are
   wrapped automatically; statement expressions and nested functions can't be handled).
5. **Validate** with V1-V4. Only then does the function count as matched.
6. **Commit** (see "Commits and merges"), log it in `build/progress/U.md`, continue.

## Validation

Never skip a level because the one before looked fine.

**V1: the function.** `tools/funcdiff.py U F` prints `MATCH` and no `|` lines, and `tools/leftovers.py game
--unit U` no longer lists `F`: funcdiff also says MATCH for a function that is still `INCLUDE_ASM` or whose draft
sits in `#if 0`. funcdiff normalizes relocation targets, so it can't see two globals swapping registers when
their `%hi` is the same (same 64 KB page): only V3 can.

**V2: the rest of the unit.** `tools/funcdiff.py U` compared with the previous run (`> build/progress/U.overview`):
nothing that was MATCH may now differ, no new function may appear (extra weak copies, vtables). The known noise
(`__static_initialization_and_destruction_0`, `DWTPlayer::Debug*` "NOT matching" in units whose data is from
asm) is neither a regression nor a fix.

**V3: the whole image.** `ninja > build/ninja.log 2>&1; echo $?` prints 0: the SHA-1 of the complete
executable, the only real proof. On failure, `tools/imagediff.py` (object, section, symbol of the first
difference). After a shared-header edit every includer was recompiled: header strings and `__LINE__`s move other
units' `.rodata`, so keep the header's line layout (add members in place, `#line` if needed) and follow A0.

**V4: honest C.** Review `git diff` before each commit:
- Real C/C++ for what the original does. Not allowed: `.word`/`.byte` instructions, the original's instruction
  stream in `asm()`, an `INCLUDE_ASM` under another name, branch-target tricks, edits in `build/`, `asm/`,
  `config/` or `expected/`.
- Allowed inline asm: the VU0/MMI/COP2 instructions the original wrote as inline asm (see the ref and the
  vu0-asm-function-conversion / ppp-keshptail-patterns memories); empty `asm volatile("" ...)` barriers as a
  lever, with a comment. MIPS arithmetic stays in C (user's rule).
- Only the unit's `src/U.cpp`/`.h` and the headers its classes need. Never add a `// PARTIAL` marker, a stand-in or
  anything against the ref to another unit's header (the header cleanup below removes them); a missing
  declaration goes in exactly as the ref has it. A member private in the ref: a `friend class X;` on an existing
  line of its class with a MATCH reason (the ref can't show friends), never a changed access specifier; anything more
  is not the original's source: park it (user, 2026-11-07; hook edits A1/A2 may touch other files).

**V5: the gate.** `tools/check.sh` exits 0 before every merge into `main`, again on `main` after it, and at the
end of every session.

**V6: data.** When you add or extend `// SOURCE DATA:`: V3 passes and `tools/imagediff.py` reports nothing
(funcdiff ignores data).

If a level fails and you can't fix it, revert to the last commit. The branch passes V3 at every commit.

## Parking

No fixed time box. Park a function when many tries stop improving it: dozens of variants across several lever
families, plus a permuter run of at least 15 minutes on a near miss, with the best diff no longer shrinking. A
function that keeps improving is not parked. Form (`tools/leftovers.py` reads the `Parked:` prefix):

    /* Parked: <what still differs, concretely: registers/instructions, which block>; tried: <lever families,
       permuter runs and their best score> */
    #if 0
    <the best draft>
    #else
    INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/<unit>", <F>);
    #endif

Update the existing note of an old draft instead of adding a second one, including its line count when a draft
improves.

## Structural blockers

When one cause blocks many functions (example: compiled switches put jump tables into `.rodata`, which is
stripped until the unit has `// SOURCE DATA`): name it in `build/progress/<unit>.md` with the affected functions,
and consider resolving the cause itself (bring the data from source early with the placeholder recipes, V6).
The gate still runs; a half-done data step is not committed. If the fix needs `tools/`/`configure.py`/`config/`,
ask.

## Finishing a unit

Once every function is matched or parked:

1. **Retry the parked ones**, including old drafts.
2. **Data (`// SOURCE DATA`).** With parked functions left, use the placeholder recipes
   (include-asm-with-source-data, rcf-placeholder-and-volatile, include-asm-jumptable-data, wnd-unit-lessons);
   replace `extern D_...` strings by literals; reproduce the header strings' order (inline functions' strings in
   include order, then the file's constants, vtables last). Validate with V6. A unit whose `.rodata` can't start
   where the split puts it (misaligned-unit-rodata) keeps its data from assembly: note it in the table at the top
   of `docs/asm-leftovers.md`.
3. **Finish hook part 1** (A2): the unit's own classes to the full ref class.
4. **The "NOT matching" noise** should go away with the data; if not, find out whether it is an objdiff artifact.
5. **Gate, merge, docs** on `main`: V5, then the docs block of "Keeping the docs current", and update the work
   table and "Where things stand" of this file (the unit leaves the table).
6. **Memory:** the unit's `*-unit-lessons` file and its `MEMORY.md` line.
7. **Finish hook part 2** (A2) on a cleanup branch.

## Header cleanup

The PARTIAL pseudo-implementations in headers (`// PARTIAL: added for <unit>`), local stand-in copies of other
units' classes, and matching mechanisms are replaced by the real classes from the owning units: full ref classes
(all fields, methods and virtuals in ref order), mechanisms only where needed, each with a `MATCH:` reason.
The key fact: the strings of a header's inline functions land in the `.rodata` of every includer, in include
order, so the original `.rodata` (`build/SLUS_217.99/expected/asm/data/<U>.rodata.s`, `partials.py --strings`)
shows which headers each original unit included. Fix the header structure from that evidence and the ref,
rather than just including a header.

**A0. Header-change protocol** (every hook and batch):
1. Baseline: `tools/check.sh` exits 0; save `partials.py --header <h>` to `build/partials/<h>.before` and the
   funcdiff overviews of the units owning a class in the header.
2. Edit in place, in ref order, keeping the line layout (`#line N <X>_H_PATH` after insertions before a
   `__LINE__`/`__FILE__` site). All edits to one header in one build (5-15 minutes: long timeout).
3. V3. On failure classify with `tools/imagediff.py`: `.rodata` of a unit = strings or their order (A4); weak
   `.text` in a foreign unit = an inline now emitted there (A4.4); a normal function's `.text` = the caller's
   codegen changed (ref types, ref-return-types-matter); vtable data = declaration or virtual order
   (vtable-emission-order); link error = a mangled name changed (`elf_symbol_addrs.txt` is the truth). Bisect per
   class, don't guess.
4. V2 for the owner units; `--header` again: the includers that "add strings" must not grow (this also covers
   units whose `.rodata` is still asm, which V3 can't check).
5. V4 (only declarations, layouts, includes and mechanisms change), V5.

**A1. Unit start hook** (on `decomp/<U>`, before the first function; time-limited, it must never block the
unit): `partials.py --unit <U>`; replace each foreign stand-in whose owner is complete or superset and
string-compatible with an include of the owner header (at the position the original's string order says);
add missing members to the owner header in ref form; a stand-in that can't be replaced keeps
`/* PARTIAL: stand-in for <Owner> <Class>: <reason> */`. Commit per owner header.

**A2. Unit finish hook.** Part 1, on the unit branch after its data step: every class the ref assigns to U
becomes the full ref class (ref names and order, no padding, virtuals in `/* vtable[n] */` order, no implicit
members); inline bodies only with evidence (the key unit's weak tail, weak copies, visible inlining); try
removing each mechanism, keep it with `MATCH:` if V3 fails; delete PARTIAL comments of classes that now equal
the ref. Part 2, on `cleanup/<Unit-with-dashes>` after the merge: `partials.py --class` per class, replace the
stand-ins and duplicates elsewhere, drop unneeded gates and include-guard pre-defines; commit per header and per
group of about 10 .cpp files, V3 each.

**A3. Between-work batch:** on `main`, `partials.py --queue`, take the top ready item; branch
`cleanup/<Header-with-dashes>` or `cleanup/<topic>`; header changes first, then the .cpp swaps in groups;
log in `build/progress/cleanup-<name>.md`; merge with the `--summary` deltas; add a row to the trend table of
`docs/partials.md`, then the docs block. Items over about 30 files are split by top directory.

**Queue state (2026-11-07):** the R1 "gate" items are feature macros a unit defines before the include. Dropping each
one's `#define`s and rebuilding: ENTITY_H_SETFLAG_DIRECT, MODELDATA_H_TEXINFO_CTOR, GAME_COLLISION_COLLOBJ_H (CollUtil)
and VECTOR_H_SET_CALL all change the image, so they stay. The global operators are done (cleanup/heapman-operators, 2026-11-07): System/HeapMan.h has RET_ADDR (inline
asm: __builtin_return_address reloads ra), new, delete, delete[], new(size_t, char*), new[] and placement new, all
throw(); the sites are `new (__FILE__ "(N)") T` (Camp's setWorldmap keeps the emulated form, MATCH), the local copies
and the KH_PLACEMENT_NEW_DEFINED gate are gone. MapEnemy.h, SceneInfo.h, CollUtilA.h and Coord.h list gate users
without a feature macro.

**A4. The ladder**, when completing a header would break another unit's `.rodata` (stop at the first step that
works): forward declarations instead of includes; move the type to the header the original used (string
evidence + ref owner); move types only the owner uses into its .cpp; for a weak function now emitted in a
foreign unit, keep the method declaration-only outside the key unit (`#ifdef <KEY>_CPP`, `MATCH:`); last
resort, a per-unit opt-out macro with `MATCH:` and an entry in `docs/partials.md`'s open puzzles. Never invent a
header split the original didn't have, or make a class a template to hide its strings.

## Commits and merges

| work | branch | commit messages |
|---|---|---|
| functions of a unit, start hook, finish hook part 1 | `decomp/<Unit-with-dashes>` | `<unit>: match <F>`, `<unit>: park <F>`, `<unit>: <F> draft N -> M lines`, `<U>: use <header> instead of stand-ins`, `<U>: complete <Class> in <header>` |
| finish hook part 2, queue batches, docs-only and tool work | `cleanup/<...>` | `cleanup <header>: <what>` |
| regenerated docs | `main` | `docs: regenerate leftovers and progress` / `docs: regenerate partials` |

- Commit after every matched function (V1-V4), adding the unit's files by name. Commit messages end with the
  session's Co-Authored-By line.
- Merge into `main` in chunks (every 10-20 matched functions, at the end of a session, when a unit is finished):
  V5 on the branch, then `git checkout main && git merge --no-ff <branch> -m "Merge <branch>: <what> (stand-ins
  -N, PARTIAL -M, mechanisms -K)"` with the `partials.py --summary` deltas, then V5 on `main`, then the docs
  block of "Keeping the docs current" (every merge, not only a unit's last). Delete the branch once it is
  merged. A cleanup branch never contains function matches.
- Pushing is the user's: they push `main` to GitHub (`origin`), where CI runs `tools/check.sh` from a fresh clone
  (`.github/workflows/check.yml`). A local checkout keeps stale files in `asm/` and `build/` that a fresh clone
  lacks; when CI fails but the local gate passes, look for such a dependency first.

## Record keeping

- `build/progress/<unit>.md` (ignored, survives compaction): one line per attempt (function, date, result,
  levers). Read it at session start.
- The source holds the per-function truth (`Parked:` notes); the tools turn it into the docs. When a draft
  improves, update its note (line count, what still differs) in the same commit, so the regenerated docs
  show it.
- At the end of every session: V5 on `main`, the docs block, and "Where things stand" of this file.
- A new lever that works gets a memory file or a line in the unit's lessons file right away.

## Levers index (symptom -> memory file)

| symptom in the diff | read |
|---|---|
| argument loads rotated before a call near a block end | call-arg-order-dependents |
| member store issued before/after a global load's `lui` | fixed-scalar-varying-struct-alias |
| same insns, issue order or v0/v1 wrong, no source order fixes it | issue-order-pins |
| ours has an extra `move a0,..` before a call the original makes with a0 unset | the callee is `static`: read its body (getPlayerState) |
| ours keeps a constant in a saved register, the original reloads it per call | pin it to the argument register in each call (setupFlick) |
| ours if-converts one inline test, the original branches | empty asm in the arm (getRandomTargetPos) |
| `lhu`, `sll 16`, `sra 16` split around a branch | sign-extension-by-hand |
| struct argument built from four `lhu` + dsll/or | `_NORMA_PIECES` in `System/Graphic/2d/2DCard.h` |
| a register dies, then seems to hold the same value again | ref-locals-reveal-shape (local assigned twice, or read into a local before a branch) |
| `li 1` + `movn`/`movz` where ours branches or uses xori | cmov-operand-order |
| 16-byte constants loaded by lq before all stores | timode-local-pool-constants |
| ours repeats `...; jal x`, the original branches into the other copy (or the reverse) | crossjump-block-end-call |
| one-line `por/sq` + `swc1 w`, wrong store order around an FVECTOR | fvector-brace-init-aligned |
| delay slot of a call and the v0/v1 pair after it | void-callee-v0 |
| spill slots / regs of hoisted `this+off` addresses | gcse-bucket-count-lever, gcse-pre-hash-order |
| registers swapped, same instructions | scheduling-mismatch-techniques, global-alloc-priority-lreg, asm-barrier-death-note-levers, loop-const-merge-and-load-order, asymmetric-giv-overloads, gcse-pre-hash-order |
| preheader hoist order, constant hoisted only in ours | loop-two-pass-hoist-threshold, preheader-giv-init-order, loop-hoist-lifetime-store-order |
| the original repeats a test that cse folds in ours | duplicate-test-masked-form |
| a hoisted constant is a copy of a variable initialised to it | volatile-asm-cse-flush (`asm volatile("" : : "i"(0))` after the init) |
| array row base in its own register, no folded offset | row-base-pointer-register |
| v0/v1 swapped in several switch arms, a tail cross-jumped only in ours | shared-switch-intermediate-var |
| reload/spill register one off after inline asms | asm-inout-earlyclobber-reload |
| a giv step at the loop end in the original, mid-loop in ours | separate-biv-for-increment |
| peeled first test + rotated loop with `bnezl` | goto-layout-reorg-prediction (dlmalloc: goto body after the return) |
| a table or argument load stuck after a local struct store | rodata-table-extern-const (the symbol is in `.rodata`: declare it const) |
| stores in another order | sched-store-order-reg-weight, volatile-delay-slots-store-permutation, small-union-ctor-stores, fixed-scalar-varying-struct-alias, join-store-per-arm |
| the original reloads a global/member, ours keeps it (or the reverse) | global-reload-ref-helper, fixed-scalar-varying-struct-alias, const-global-reorg-redundant-load, mapenemy-unit-lessons |
| tails merged or not merged, branch layout | gcc-crossjump-alias-rules, crossjump-dup-arm-stores, crossjump-case-label-chain, state-machine-break-vs-return, guard-plus-while-and-static-members, first-return-stub-ternary, bool-tail-shared-false |
| loop shape, hoisted constants, extra nops | loop-rotation-break-layout, loop-hoist-constant-order, loop-hoist-lifetime-store-order, loop-align-empty-loop-note, short-loop-padding, div-line-label-nops, preheader-giv-init-order |
| stack offsets differ | stack-slot-order-decl-position, unused-locals-stack-frame, inline-return-temp-slot |
| conditional moves, compares | cmov-operand-order, angle-wrap-ternary, index-loop-signed-slt, bit-test-fold-jtbl-align |
| jump table vs compare chain | atel-unit-lessons (5+ case nodes), include-asm-jumptable-data, bit-test-fold-jtbl-align |
| weak functions, vtables, emission order | key-function-emits-all-inlines, vtable-emission-order, weak-emission-definition-order, inline-called-not-inlined, weak-order-and-scalar-mem-alias |
| strings, rodata order | include-asm-with-source-data, template-strings-not-merged, static-table-strings-position, placeholder-file-literal-and-vtable-decl-order, misaligned-unit-rodata |
| return type / getter form changes code | ref-return-types-matter, entity-unit-lessons |
| VU0 / GS packets / u128 | vu0-asm-function-conversion, ppp-keshptail-patterns, gs-packet-u128-patterns, sp-address-inline-wrappers, volatile-asm-cse-flush |
| Draft lacks a ref local, or "registers only" around asm helpers | `ref-locals-reveal-shape`, `ti-di-conversion-is-por` |
| Near-tie in global-alloc, or combine/reorg folds an insn | `empty-asm-live-length` (empty asm: +1 live length, blocks stealing; with an operand, a second use) |
| Temp in s3/t8, giv stepping far from its biv | `asm-temp-odd-register` |
| `bne; nop` where the original has `bnel` into the loop | a volatile load at the target: reorg never steals it (`_threadReadProc`) |
| several fields "look volatile", reloads after one store (Sony libs) | union-store-alias-zero |
| registers rotated by one in a block, hand shapes do not help | union-store-alias-zero (an inline's argument pseudos; decomp-permuter) |
| a draft says "near match" with no measured diff | remeasure-stale-drafts (its layout may hide it; it can match as it stands) |
| C units (lib/, libppp, sddrv) | `docs/sdk-leftovers.md` "Levers that worked in lib/", decomp-permuter-c-units |
| signal 11 in the cc step | cc1plus-silent-segfault, CLAUDE.md pitfalls (Ps2EeAs) |

Helpers in `tools/helpers/` (moved from `build/` 2026-11-07, run from the repository root; read one before using it;
copy it to `build/` to change it for an experiment): the method helpers named above, and `vtry.py` (enable a draft, apply
text variants, funcdiff each; `tryv.py`/`applyv.py`/`permfn.py` around it), `vifelse.py` / `ifeshow.sh` (the same for
`#if 0 ... #else` drafts), `dumpvar.py` (compile a variant with gcc dump flags such as `-dl -dg -dS`),
`permute_overload.py` (superseded by `tools/permuter.py --overload <params>`), `hill.py` (statement-order hill climb),
`rocmp.py` (rodata check), `vsgen.py`, `gpconv.py`, `coord_conv.py`, `swap_keep.py` (replace a stand-in with an
include, keeping the line count). `draftlib.py` is their shared draft finder (every layout, as `tools/drafts.py`).
One-off scripts of earlier sessions stay in the main checkout's `build/` (named in the memory notes; not maintained).

## Context economy

Keep output small: one function's assembly at a time; `funcdiff` without `--full`; logs to `build/*.log` with
`echo $?; tail -5`; never print a whole unit source, `grep -n` / `sed -n` the part you change.

## When to stop and ask the user

- A function or the unit's data needs a change to `tools/`, `configure.py`, `config/` (other than symbol_addrs
  names) or the compiler flags.
- V3 or V5 fails on `main` and the cause isn't in your change.
- Anything destructive (resetting branches, deleting drafts someone else wrote).
- Header cleanup: a partition fix that reaches more than about 20 includers or moves a type against the ref
  owner; string evidence that contradicts the ref; a new kind of mechanism; completion that needs changes to
  matched C beyond ref type corrections; unifying `Gprim.h` and `GprimFull.h`; a new opt-out macro more than
  about 5 units need; no net decrease of stand-ins plus PARTIAL markers after 3 consecutive units.
- **Done (cleanup/vector-types, merged 2026-11-07):** F_UL, G_XY, G_XYZ, G_RGBA, G_UV,
  G_ST at the end of System/Vector.h and the C++ QWdata after khtypes.h's u_int128 (the original's .mdebug type order,
  against the ref's placement; user decision). The former `D2MAN_H_PATH`/`RSRCDATA_H_FULL` entry is settled (checked 2026-11-07): RSRCDATA_H_FULL was dropped
  on 2026-11-02 (bfbdb0dd), 2DMan.h's classes are complete and ungated in all 166 includers (strings ok), and
  D2MAN_H_PATH is a path override like GPRIM_H_PATH: the 2d units name the header `c:/KHCOM/MainApp/...` (without it
  2DLay's assert strings change, tested). Nothing waits on a decision here.
- When the user runs a /loop unattended, decide these cases yourself, log the decision and continue
  (unattended-decide-stop-cases).
