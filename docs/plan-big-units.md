# Plan: finish the big game units, one function at a time

For the model that takes over the game decompilation. Read this file completely, then `CLAUDE.md`, then the
memory index (`MEMORY.md`, loaded at session start) before touching code.

## Goal and scope

Bring the game units with the most remaining assembly to "complete": every function compiled from C/C++ and
byte-identical, and the unit's data from source (`// SOURCE DATA: ...` as the last line). The biggest units
come first. Work is strictly sequential: one unit at a time, one function at a time, in the main checkout, with
no sub-agents, workers, worktrees or parallel decompilation. (Parallel compiles inside `tools/permuter.py` are
fine; that is a tool, not a second worker.)

Out of scope: `lib/` units (see `docs/sdk-leftovers.md`), hand-written `.s` units (`crt0`, `System/expand`),
changes to `tools/`, `configure.py`, `config/` and the build flags. If a unit seems to need one of those, stop
and ask the user (see "When to stop and ask"). The one approved `tools/` change is `tools/partials.py`.

Interleaved with this plan: the PARTIAL cleanup of `docs/plan-partials.md` (start hook A1 and finish hook A2 per
unit, one queue batch A3 between units, the header-change protocol A0). Its report is `docs/partials.md`.

State at the start of this plan (main, 2026-10-21): 9955 functions matched in 662 files. 2312 game functions
(about 230,000 instructions) are still `INCLUDE_ASM` in 85 units; 120 of them have a parked C draft.

## Unit order

Biggest remaining assembly first. Regenerate the list at the start of every session, because it changes as you
work:

    .venv/bin/python tools/leftovers.py game | sed -n '/Units with INCLUDE_ASM/,/^Totals/p' | head -30

The table is sorted by instructions left. The order at the start, with the matching memory file to read first:

| # | unit | functions left / total | instr left | data from asm | read first (memory) |
|---|---|---:|---:|---|---|
| 1 | `Game/Player/Player` | 303 / 346 | 23609 | .bss .data .rodata | player-unit-lessons |
| 2 | `Game/Atel/VSME` | 90 / 137 | 18301 | .bss .data .rodata | vsme-unit-lessons |
| 3 | `Form/Menu/JiminyMemo` | 160 / 202 | 17447 | .bss .data .rodata | jiminymemo-unit-lessons |
| 4 | `Game/Player/PlayerSora` | 25 / 30 | 14383 | .data .rodata | playersora-unit-lessons |
| 5 | `Form/Menu/Camp` | 19 / 71 | 13530 | - | (13 of the 19 are parked drafts: read their comments) |
| 6 | `System/Graphic/Gprim` | 321 / 334 | 12993 | .data .rodata | gprim-unit-lessons |
| 7 | `Game/Enemy/BattleEnemy` | 249 / 390 | 9962 | .data .rodata | battleenemy-unit-lessons |
| 8 | `Form/Menu/SaveLoad` | 86 / 125 | 9524 | .bss .data .rodata | saveload-unit-lessons |
| 9 | `Game/CardBattle` | 85 / 119 | 7583 | .data .rodata | cardbattle-unit-lessons |
| 10 | `Game/Debug/DebugPathData` | 77 / 87 | 6931 | .data .rodata | debugpathdata-unit-lessons |
| 11 | `Game/CardRevolver` | 45 / 106 | 6930 | .data .rodata | cardrevolver-unit-lessons |
| 12 | `Game/Player/PlayerGauge` | 61 / 80 | 6867 | .bss .data .rodata | playergauge-unit-lessons |
| 13 | `System/Graphic/Model` | 170 / 206 | 5996 | .bss .data .rodata | model-unit-lessons |
| 14 | `Game/Player/PlayerDRiku` | 5 / 32 | 5620 | - | |
| 15 | `Game/BattleCard` | 73 / 104 | 5375 | .bss .data .rodata | battlecard-unit-lessons |
| 16 | `Game/Atel/Avatar` | 113 / 187 | 4856 | .rodata | avatar-unit-lessons |
| 17 | `Game/SceneInfo` | 57 / 169 | 4635 | .bss .data .rodata | sceneinfo-unit-lessons |
| 18 | `System/Graphic/PackMan` | 42 / 185 | 4290 | .bss .rodata | packman-unit-lessons |
| 19 | `Game/Entity` | 117 / 265 | 3865 | .rodata | entity-unit-lessons |
| 20 | `Game/Map` | 26 / 39 | 3779 | .data .rodata | map-unit-lessons |

After these come the smaller units; `docs/plan-asm-leftovers.md` covers them (smallest first).

Between two units: one cleanup batch from `tools/partials.py --queue` (plan-partials A3).

A unit is finished when it is complete, or when every function still left in it is parked (see "Parking").
Move to the next unit only then. Never leave a unit with a function that was neither tried nor parked.

## Session start (every session, also after a context compaction)

1. `git status` must be clean, on `main` or on the unit's branch `decomp/<Unit-with-dashes>` (slashes become
   dashes, e.g. `decomp/Game-Player-Player`). Create the branch from `main` when you start a unit.
2. `tools/check.sh > build/check.log 2>&1; echo $?` must print 0 before you change anything. If it doesn't,
   find out why first (a stale tree usually just needs `configure.py`).
3. Read the unit's memory file (table above) and the levers index below. Open `build/progress/<unit>.md`, your
   own working log (see "Record keeping"), if it exists.
4. List the unit's remaining functions with their sizes:

       .venv/bin/python tools/leftovers.py game --unit <unit>   # every function left: state, size, note
       .venv/bin/python tools/asm.py <unit>                       # every function, size, weak/global

5. First session of a unit: the start hook (plan-partials A1, `tools/partials.py --unit <unit>`) before the
   first function.

## Order of functions inside a unit

- Do the smallest remaining non-weak function first and work up. Small functions (getters, state wrappers,
  forwards) fill in the class layout and the headers, and the big functions need that afterwards.
- Generated families first, where memory has a generator or recipe (state wrappers from the vtable order in
  BattleEnemy and Player; `acWait*` in VSME through `build/vsgen.py`; the VU0 helpers in Gprim through
  `build/gpconv.py`). Check each generated function like any other.
- Weak functions (`weak` in `tools/asm.py`) are inline functions. Do them only when their class definition is
  right, by defining them in the class body (or `inline` in the header) in the original emission order. Never
  write them as ordinary functions in the `.cpp`. See key-function-emits-all-inlines,
  weak-emission-definition-order, inline-called-not-inlined.
- Parked drafts that already exist come last. They have been tried before, so retry them with fresh eyes once
  the rest of the unit (and its lessons) is done.
- The source keeps the original function order whatever order you work in. You only replace an `INCLUDE_ASM`
  line in place.

## The per-function loop

For one function `F` of unit `U`:

1. **Study.** `tools/asm.py U F` (the original, compact). In the reference `ref/SLUS_217.99/src/U.cpp`, find
   the signature, parameter names and locals. Look up only the types you need in `ref/SLUS_217.99/types.h`
   (`grep -n -A<n>`, never the whole file). Read the existing note or draft above `F` in the source, if any.
2. **Write** the C/C++ in place of `F`'s `INCLUDE_ASM` line. Keep the original line numbers wherever
   `__LINE__` reaches an assert or allocation message (use `#line N` to resync after your function). Without
   `// SOURCE DATA` the compiled object's `.rodata` is stripped, so string literals and assert texts in new C
   code must refer to the original's labels as `extern const char D_xxxxxxxx[];` (see map-unit-lessons,
   player-unit-lessons). The same goes for constant tables.
3. **Iterate** with `.venv/bin/python tools/funcdiff.py U F`. It builds the object itself and prints only the
   differences: `|` differs, `~` only an offset or address. Change one thing at a time. Use the levers index
   below for the symptom you see.
4. **Near miss** (same structure, only registers or scheduling differ, roughly 20 differing lines or fewer):
   run the permuter before parking:

       .venv/bin/python tools/permuter.py U F --minutes 15 -j 16

   It needs the draft compiled. Either run it while `F` is in C, or park the draft under `#if 0` / `#else
   INCLUDE_ASM` first, which the permuter switches on by itself. Read `build/permuter/F/best.diff`. Adopt a
   variant only if it keeps the program's meaning (the permuter's safe mutations do, but check moves of
   statements across calls or stores). Then validate it like any other change.

   For a `.c` unit, also run decomp-permuter (it parses C only; its AST randomizations found what
   `tools/permuter.py` missed in lib/kprintf2 ftoi, 21 -> 9 lines in 5 minutes):

       .venv/bin/python tools/decomp_permuter.py U F --minutes 15 -j 16

   It ends with the best outputs rescored in differing lines. Go by that count, not by its own score:
   on a far-off function its score can drop while the line count rises. Read the `diff.txt` of the best
   one. Its randomizations do change the meaning sometimes, and they often add noise (duplicated `if` arms,
   casts). Carry over only the sensible part by hand, then check it with funcdiff. For code it cannot parse,
   use `--setup-only`, wrap the construct in `PERM_IGNORE(...)` in `base.c`, and run with `--resume`. Asm
   statements are wrapped automatically. `PERM_GENERAL(a, b)` and other macros in `base.c` test chosen
   variants (see `tools/decomp-permuter/README.md`).
5. **Validate** with the ladder below. Only a function that passed V1-V4 counts as matched.
6. **Commit** (see "Commits and merges"), update `build/progress/U.md`, and continue with the next function.

## Validation workflow (the important part)

Every claim of "matched" must pass these checks, in this order. They are cheap: a full `ninja` takes seconds
when little changed. Never skip a level because the level before it looked fine.

**V1: the function itself.** `tools/funcdiff.py U F` prints `MATCH F: original N bytes, ...` and no `|` lines.
- **Trap:** funcdiff also prints MATCH for a function that is still `INCLUDE_ASM` (the original's assembly
  matches itself), and for one whose draft sits in an `#if 0`. So V1 also requires the function to be
  compiled: `tools/leftovers.py game --unit U` must no longer list `F` (it lists every `INCLUDE_ASM` in
  active code, parked or not). A function counts only if C produced it.

**V2: the rest of the unit.** `tools/funcdiff.py U` (the overview). Compare it with the previous run (save
it: `> build/progress/U.overview`). Nothing that was MATCH before may now be `differs` or `missing`, and no new
function may appear. A new C function can make gcc emit extra weak inline copies or vtables, or change which
inline functions are expanded. The known noise (`__static_initialization_and_destruction_0` and the
`DWTPlayer::Debug*` functions flagged "NOT matching" in Player, PlayerGauge, VSME, JiminyMemo, Model, BattleCard
and PackMan) was there before. Leave it alone until the unit's data comes from source, and never count it as a
regression or a fix.

**V3: the whole image.** `ninja > build/ninja.log 2>&1; echo $?` must print 0. This is the SHA-1 check of the
complete executable, and the only real proof. It catches what funcdiff can't see: data, alignment and padding
after the function, weak copies, and other units changed through a shared header. If it fails, run
`.venv/bin/python tools/imagediff.py` (object, section, symbol of the first difference) and fix it before
anything else.
- If you edited a shared header (e.g. `Game/Player/Player.h`, `Game/Entity.h`), every unit that includes it
  was recompiled, and V3 is what checks them. Header string literals and `__LINE__`s move other units'
  `.rodata`, which is why the header's line layout must not change (add members in place, keep line counts,
  `#line` if needed).
- After a header change, follow plan-partials A0 (baseline, imagediff classification, `partials.py --header`
  hidden-conflict recheck).

**V4: honest C.** Review `git diff` before each commit:
- The function is real C/C++ for what the original does. Not allowed: `.word`/`.byte` emitting instructions,
  copying the original's instruction stream into `asm()`, an `INCLUDE_ASM` under another name, branch-target
  tricks, or editing `build/`, `asm/`, `config/` or `expected/` files.
- Allowed inline asm (all already used in the project): the VU0/MMI/COP2 instructions the original source
  wrote as inline asm (the reference and the `vu0-asm-function-conversion` and `ppp-keshptail-patterns`
  memories show which), and empty `asm volatile("" ...)` barriers as a matching lever
  (asm-barrier-death-note-levers). Mark such a barrier with a short comment.
- No changes outside the unit's `src/U.cpp`/`.h` and the headers its classes need. A header of another unit
  that you create or extend is marked `// PARTIAL: added for <unit>` at the top if it is new. Exception: the
  plan-partials hook edits (A1, A2) may touch other files.
- A matching mechanism that stays (gate, opt-out, include-guard pre-define, placeholder, path override,
  pragma) carries a `MATCH: <reason>` comment.
- `config/SLUS_217.99/symbol_addrs.txt` may get lines that name unnamed weak or vtable copies
  (symbol-addrs-followups); nothing else in `config/`.

**V5: the gate**, before every merge into `main` and at the end of every session: `tools/check.sh` exits 0
(configure + full build + checksum + overview). Then run it once more on `main` after the merge.

**V6: data**, when you add or extend `// SOURCE DATA:` (see "Finishing a unit"): V3 must pass, and
`tools/imagediff.py` must report no difference. funcdiff ignores data completely, so it proves nothing here.

If any level fails and you can't fix it, revert to the last commit (`git checkout -- <files>`) rather than
committing something half-right. The branch must pass V3 at every commit.

## Parking

There is no fixed time box (user's rule, memory time-box-by-the-clock). Park a function when a significant
number of tries stop improving it: dozens of variants across several lever families from the index below,
plus a permuter run of at least 15 minutes on a near miss, with the best diff no longer shrinking. A function
that keeps improving is not parked. Keep going.

To park:

    /* Parked: <what still differs, concretely: which registers/instructions, in which block>; tried: <the lever
       families and the permuter run, with their best score> */
    #if 0
    <the best draft>
    #else
    INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/<unit>", <F>);
    #endif

The `Parked:` prefix is what `tools/leftovers.py` reads. Keep this exact form. Then update the existing note
of an old draft instead of adding a second one.

## Structural blockers

Sometimes one cause blocks many functions of a unit at once, rather than one function being hard. Example
(Game/Player/Player, 2026-10-21): 30 of the remaining functions use a switch jump table; a compiled switch puts
its table in `.rodata`, which the build strips until the unit has `// SOURCE DATA`, so objcopy rejects the
object (`symbol .rodata required but not present`). Parking each of them one by one only defers the real work.

When you hit such a blocker:

1. Name it in `build/progress/<unit>.md` with the list of affected functions (e.g. the jump-table scan
   `awk '/^glabel /{fn=$2} /jtbl_/{print fn}' build/SLUS_217.99/expected/asm/<unit>.s | sort -u`).
2. Consider resolving the cause itself, even if the plan would normally do it later (for the jump-table case:
   bring the unit's data from source early, with the placeholder recipes under "Finishing a unit" and the
   `// SOURCE DATA` validation V6). Weigh the size of that work against the number of functions it unblocks,
   and do it on the unit branch like any other change.
3. Whatever you decide, the gate still runs: `tools/check.sh` must exit 0 before the merge into `main`, and
   V3 (`ninja`, the SHA-1) must pass at every commit. A data step that is only partly done is not committed.
4. If the resolution needs `tools/`, `configure.py` or `config/` changes, stop and ask (see "When to stop and
   ask the user").

## Finishing a unit

Once every function is matched or parked:

1. **Retry the parked ones.** That includes drafts from earlier models, which the generated inventory lists.
   The unit's lessons are freshest now.
2. **Data (`// SOURCE DATA`).** Data can come from source only when no `INCLUDE_ASM` function references the
   unit's data labels in a way the C can't reproduce. With parked functions left, use the placeholder recipes:
   include-asm-with-source-data, rcf-placeholder-and-volatile, include-asm-jumptable-data, wnd-unit-lessons.
   - Replace the `extern D_...` strings in the C with real literals at this point.
   - Reproduce the header strings' order (rodata: inline functions' strings in include order, then the
     file's constants, vtables last; see CLAUDE.md).
   - Validate with V6.
   - A unit whose `.rodata` can't start where the split puts it (misaligned-unit-rodata) keeps its data from
     assembly. Note that in `docs/asm-leftovers.md` (the hand-written table at the top).
   - Then the unit's own headers to the full ref class (plan-partials A2 part 1).
3. **The "NOT matching" noise** of the unit should go away once its data comes from source. If it doesn't,
   find out whether it is an objdiff artifact (the checksum passes) or real, and record what you find.
4. **Gate, merge, docs:** V5, merge (below), then regenerate the docs on `main`:

       .venv/bin/python tools/leftovers.py game --update docs/asm-leftovers.md
       .venv/bin/python tools/progress.py --update README.md
       .venv/bin/python tools/partials.py --update docs/partials.md

   Commit them as `docs: regenerate leftovers and progress`.
5. **Memory:** write or update the unit's `*-unit-lessons` memory with the levers that worked and what stayed
   unsolved, and its line in `MEMORY.md`.
6. **Cleanup branch for the other files** (plan-partials A2 part 2): `cleanup/<Unit-with-dashes>` from main,
   the stand-ins and duplicates of the unit's classes elsewhere.

## Commits and merges

- On the unit branch, commit after every matched function (V1-V4 passed): `git add` the unit's files only,
  with the message `<unit>: match <F>` and the closing line `Co-Authored-By: ...` from the session's
  instructions. Parking commits: `<unit>: park <F>`.
- Merge into `main` in chunks: every 10-20 matched functions, at the end of a session, and when the unit is
  finished. First V5 on the branch, then:

      git checkout main && git merge --no-ff decomp/<Unit-with-dashes> \
          -m "Merge decomp/<Unit-with-dashes>: <unit> (partial|complete, <matched>/<total>)"
      tools/check.sh > build/check.log 2>&1; echo $?      # V5 again, on main

  Continue on the same branch afterwards (`git checkout decomp/...`, it already contains main).
- There is no git remote. Never push, rebase `main`, or force anything.
- Never commit game data (`iso/`, `ref/`, `asm/`, `build/`).
- PARTIAL cleanup (plan-partials "Branches and commits"): batches on `cleanup/<Header-with-dashes>` or
  `cleanup/<topic>`, finish hook part 2 on `cleanup/<Unit-with-dashes>`; hook edits A1/A2 part 1 on the unit's
  `decomp/...` branch. A cleanup branch never contains function matches. Merge message:
  `Merge cleanup/<x>: <what> (stand-ins -N, PARTIAL -M, mechanisms -K)`, the `partials.py --summary` deltas;
  then `docs: regenerate partials` on main.

## Record keeping

- `build/progress/<unit>.md` (ignored by git, survives compaction): one line per function attempt: function,
  date, result (matched / parked / best score) and the levers that worked or failed. Read it at session start.
  It is your memory within a unit. The durable lessons go to the memory files at the end of the unit.
- The source holds the per-function truth (`Parked:` notes). `tools/leftovers.py` and `tools/progress.py`
  turn it into the docs, so don't edit their generated parts by hand.
- A new lever that works (one not already in memory) gets its own memory file or a line in the unit's
  lessons file right away, not only at the end of the unit.

## Levers index (symptom → memory file)

| symptom in the diff | read |
|---|---|
| registers swapped, same instructions | scheduling-mismatch-techniques, asm-barrier-death-note-levers, loop-const-merge-and-load-order, asymmetric-giv-overloads, gcse-pre-hash-order |
| stores in another order | sched-store-order-reg-weight, volatile-delay-slots-store-permutation, small-union-ctor-stores |
| the original reloads a global/member, ours keeps it (or the reverse) | global-reload-ref-helper, fixed-scalar-varying-struct-alias, const-global-reorg-redundant-load, mapenemy-unit-lessons |
| tails merged or not merged, branch layout | gcc-crossjump-alias-rules, crossjump-dup-arm-stores, state-machine-break-vs-return, guard-plus-while-and-static-members |
| loop shape, hoisted constants, extra nops | loop-hoist-constant-order, loop-align-empty-loop-note, short-loop-padding, div-line-label-nops |
| stack offsets differ | stack-slot-order-decl-position, unused-locals-stack-frame |
| jump table vs compare chain | atel-unit-lessons (5+ case nodes), include-asm-jumptable-data, bit-test-fold-jtbl-align |
| weak functions, vtables, emission order | key-function-emits-all-inlines, vtable-emission-order, weak-emission-definition-order, inline-called-not-inlined, weak-order-and-scalar-mem-alias |
| strings, rodata order | include-asm-with-source-data, template-strings-not-merged, static-table-strings-position, placeholder-file-literal-and-vtable-decl-order, misaligned-unit-rodata |
| return type / getter form changes code | ref-return-types-matter, entity-unit-lessons |
| VU0 / GS packets / u128 | vu0-asm-function-conversion, ppp-keshptail-patterns, gs-packet-u128-patterns, sp-address-inline-wrappers |
| signal 11 in the cc step | cc1plus-silent-segfault, CLAUDE.md pitfalls (Ps2EeAs) |

Useful scripts in `build/` from earlier sessions (read one before using it; they are scratch, not
maintained): `hill.py` (statement-order hill climb), `permfn.py` (permutations of one function's
statements), `rocmp.py` (rodata check), `vsgen.py`, `gpconv.py`, `coord_conv.py`.

## Context economy

A unit like Player takes many sessions. Keep output small (the rules in `.claude/agents/decomp-worker-single.md`,
"Context economy", apply):
- one function's assembly at a time;
- `funcdiff` without `--full`;
- logs to `build/*.log` with `echo $?; tail -5`;
- never cat the whole unit source; `grep -n` / `sed -n` the part you change.

## When to stop and ask the user

- A function or the unit's data needs a change to `tools/`, `configure.py`, `config/` (other than
  symbol_addrs names) or the compiler flags.
- The cases of plan-partials A5 (they replace the former "a header change beyond adding members in place").
- V3 or V5 fails on `main` and the cause isn't in your change.
- A whole unit ends with all remaining functions parked: report it and the next unit you would take.
- Anything destructive (resetting branches, deleting drafts that someone else wrote).

## How the other steps fold in

- **Permuter on near misses** (former step 3): it is step 4 of the per-function loop.
- **Hardest functions** (former step 4): the parked drafts are retried as part of "Finishing a unit", so no
  separate pass is needed for game units. The `lib/` parked list (`docs/sdk-leftovers.md`) stays a separate,
  later task.
- **Data** (former step 5): it is step 2 of "Finishing a unit", required for "complete". The data-only `lib/`
  units without a source file stay a separate task.
