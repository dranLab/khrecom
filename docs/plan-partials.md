# Plan: replace the PARTIAL pseudo-implementations with the real ones

For the model that does the header cleanup. It runs interleaved with `docs/plan-big-units.md`: read that plan
first, then this one, then `CLAUDE.md` and the memory index. Where this plan says nothing, plan-big-units
applies (validation levels V1-V6, commits, merges, record keeping, stop-and-ask).

## Why

Headers in `src/` grew incrementally: whenever a unit needed something from another unit's class, it was added
there as `// PARTIAL: added for <unit>`. State on 2026-10-28:

- 1092 PARTIAL markers: 1024 in 112 headers, 68 in 30 .cpp files.
  - About 840 are additions in place: 518 methods, 173 types, 70 data members, 68 changes in place, 54 forward
    declarations, 28 `#include` lines.
  - 30 are padding arrays, or accessors that cast into them.
  - 23 are path overrides and 26 are `#ifdef` unit or feature blocks.
  - 46 of them say the name or the parameters were guessed.
- About 470 foreign **local stand-in** types live in 133 .cpp files (131 distinct names). The most common:
  `CRsrcDB` (65 files), `CPackMan` (31), `NAME_CASE` (30), `D2::C2DMan` (27), `CRcf` (26), `CSound` (14),
  `CBattleEnemyManager` (11) and `CRsrcData` (11). About 140 more are overlay or unattributed types
  (`CEnemyOverlay*`, `CGimmick*`).
- 26 class names are defined in more than one header: `CCardRevolver`, `CCollBody`, `CFSM`, and 14 Gprim
  classes in both `Gprim.h` and `GprimFull.h`.
- About 58 padding arrays in headers stand in for real members.
- Per-includer gates give different units different views of one class: `MOVEMENT_H_FULL_CLASS`,
  `ANIM_H_FULL_CLASS`, `RSRCDATA_H_FULL`, `COLLOBJ_H_COLLISION_CLASSES`, `MODELDATA_H_TEXINFO`,
  `ENTITY_H_SETFLAG_DIRECT`, and `TASK_H_NO_SLC_OVERFLOW_MSG`, which 154 .cpp files define.

Now that the owners are mostly matched, these should become the real classes from the owning units. The user
decided:
- **Cadence:** per-unit hooks plus a queue between units.
- **Depth:** full ref classes. That means all fields, methods and virtuals in ref order; matching mechanisms
  stay only where they are needed, each with a reason.
- **Tooling:** a committed report tool `tools/partials.py` with a generated `docs/partials.md`. This is the
  one approved `tools/` change.

**The key fact behind the plan.** Many stand-ins exist because the *original* unit's `.rodata` lacks a
header's strings. The strings of a header's inline functions land in the `.rodata` of every includer, in
include order. The original `.rodata` (`build/SLUS_217.99/expected/asm/data/<U>.rodata.s`) therefore shows
which headers each original unit included, and it proves some of our headers are split wrongly:
- `"System/RsrcData.h(156)"` is in only 21 originals, yet our `RsrcDB.h` includes `RsrcData.h`. The ref puts
  `NAME_CASE` in `RsrcDB.h`.
- The task-container overflow message is in 99 originals; 154 of our sources opt out of it.

So the fix is often to correct the header structure, guided by the ref and the string evidence, rather than to
include the header.

## Step 0: `tools/partials.py` and `docs/partials.md` (branch `tools/partials`)

A read-only report tool modelled on `tools/leftovers.py`. It reuses `ROOT`, `units`, `source`, `active_lines`
and `INCLUDE_ASM` from `tools/progress.py`. `--update` replaces the text between
`<!-- partials:start -->` and `<!-- partials:end -->`.

```
partials.py [--summary]                 # full report, or only the counts
partials.py --update docs/partials.md   # regenerate the generated part of the docs
partials.py --unit <U>                  # start-hook view of one unit
partials.py --header <src/...h>         # finish-hook view of one header
partials.py --class <Name>              # every definition and stand-in of one class
partials.py --queue [-n 20]             # between-units view
partials.py --strings [<header>]        # header string sets vs the originals' fingerprints
```

**Inputs:**
- **Sources:** the `src/` headers and .cpp files. Comments and string literals are tokenized, `#if 0` is
  handled through `active_lines`, and gates are tracked by their macro.
- **Ref classes:** the class blocks in `ref/SLUS_217.99/src/**/*.h` (fields, methods, `/* vtable[n] */`
  slots, statics).
  - The owner is the ref header's unit. `lost+found.h` means no ref owner; the `src/` home decides then.
  - Names are matched by their last component, so `D2::C2DMan` matches `C2DMan`.
  - Skipped: implicit members (`operator=()`, repeated empty constructors, `$vf`) and template instances.
- **Unit status:** INCLUDE_ASM count (parked counted apart), the SOURCE DATA line, and the order from
  leftovers.
- **Original `.rodata` strings per unit:** the addresses come from the expected `.rodata.s`, the bytes from
  `iso/SLUS_217.99`. This also catches Shift-JIS strings that splat wrote as `.word`.
- **Includers:** from `ninja -t deps` (`deps = gcc`), falling back to a static include closure.

**Detects:**
- **PARTIAL comments** by category: method, data member, type, fwd decl, `#include`, changed in place, path
  override, gate, guessed, header-level. It records the unit after "for <unit>".
- **Padding:** `_m_0x..[`, `_pad..[`, and accessors that cast into padding.
- **Stand-ins in .cpp:** own (legitimate), foreign, or unattributed (the overlays faked with `.weak X = 0`).
- **Duplicate class definitions across headers,** including guard-skip tricks such as `#ifndef SYSTEM_FSM_H`
  around a copy.
- **Gates and mechanisms:**
  - `*_FULL_CLASS`, `*_H_FULL` and the other feature macros;
  - opt-outs, and .cpp files that pre-define another header's include guard;
  - `*_H_PATH` defines grouped by value, and `#line` counts;
  - `#pragma interface/implementation`;
  - string placeholders (an uncalled `_name` inline holding only TRACE, AssertApp or com_malloc_d);
  - `MATCH:` comments.
- **Class completeness compared with the ref:** fields by name and order, methods by name and parameter
  count, and virtuals against the vtable slots. Each class gets a readiness: `complete`, `superset`, `gated`,
  `missing` or `unattributed`.
- **String compatibility per includer** (a heuristic, and labelled so):
  - The literal set of the header's non-template inline code and its include closure. `__FILE__` resolves
    through `*_H_PATH` and `#line`; `#ifdef <Y>_CPP` literals count only for Y.
  - Per unit the result is `ok`; or `n/a` for a unit whose `.rodata` is still asm, with a hidden-conflict
    warning when literals are missing; or `adds <strings>`.
  - `--strings <H>` compares the original units that contain H's literals with our includers of H.
- **Weak hint:** whether the original includers have the weak functions of templates with virtuals that a
  header instantiates. This is for the Rcf/SJoint case.

**Queue.**
- An item is one owner header with all its copies, duplicates and gate users, or a named partition fix.
- Leave out `missing` and `unattributed` items, and items whose owner unit is still in progress in the unit
  order. Those show as "deferred: <U> hook".
- Risk tiers:
  - R0: no strings, no new inline bodies or virtuals.
  - R1: inline bodies or virtuals are added or ungated.
  - R2: the header adds strings to some includer, or the weak hint fires.
  - R3: a partition change (an include removed, a type moved between headers).
- Benefit = 2 × stand-in files + 3 × duplicate headers + gate users + 0.5 × the header classes' PARTIAL
  markers.
- Order: risk ascending, then benefit descending.

**`docs/partials.md` sections:**
1. Summary counts.
2. A trend table, kept by hand above the markers: one row per merge.
3. The queue (top 20).
4. Per class with foreign copies.
5. Per header.
6. Mechanisms, each with its users and `MATCH:` reason, or `untagged`.
7. String-evidence mismatches.
8. Open puzzles, kept by hand.

**A new comment tag:** `MATCH: <reason>` marks every mechanism that stays, as distinct from `PARTIAL:`. The
tool counts both. When a header is touched anyway, its "PARTIAL: path override added for X" comments become
`MATCH:` on the same line, keeping the line count.

**Acceptance:** the first run reproduces the inventory above:
- 1092 / 1024 / 68 PARTIAL;
- about 470 stand-ins in 133 files;
- 26 duplicate names;
- 154 opt-out defines.

Spot checks: `--strings src/System/RsrcData.h` lists 21 originals; `--class CRsrcDB` lists 65 files;
`--unit Form/Menu/TechHelp` lists its `C2DMan`, `CRsrcDB` and `NAME_CASE` stand-ins.

Commit `tools: add partials.py (PARTIAL/stand-in inventory)`, then `docs: add partials report`. Add one line
for the tool to CLAUDE.md "Build and verify". Merge with `--no-ff` after `tools/check.sh` exits 0.

## Step 1: wire this plan into `docs/plan-big-units.md` (docs commit on main)

- **Goal and scope:** a pointer to this file. `tools/partials.py` is the one approved tools change.
- **Unit order:** "between two units: one cleanup batch from `partials.py --queue` (A3)".
- **Session start:** a new step, "first session of a unit: start hook (A1)".
- **V3:** after a header change, follow A0. **V4:** hook edits outside the unit's files are allowed; kept
  mechanisms carry `MATCH:`.
- **Finishing a unit:**
  - After the data step: "own headers to the full ref class (A2 part 1)".
  - The docs step adds `partials.py --update docs/partials.md`.
  - A new last step: "cleanup branch for the other files (A2 part 2)".
- **Commits and merges:** the `cleanup/...` conventions below. **When to stop and ask:** add A5, which
  replaces "beyond adding members in place".

## Procedures

### A0. The header-change protocol (every hook and every batch)

1. **Baseline.** `tools/check.sh` exits 0. Save `partials.py --header <h>` to `build/partials/<h>.before`,
   and save the funcdiff overviews of the units that own a class in the header.

2. **Edit in place, in ref order, keeping the line layout.** After any insertion that comes before a
   `__LINE__` or `__FILE__` site, restore the numbering with `#line N <X>_H_PATH`. Put all edits to one
   header into one build: a header rebuild takes 5-15 minutes, so run `ninja` in the background or with a
   long timeout.

3. **V3:** `ninja > build/ninja.log 2>&1; echo $?`. On failure, `tools/imagediff.py` shows what differs.
   Classify it:

   | difference | cause | next step |
   |---|---|---|
   | `.rodata` of a unit | strings or their order | A4 |
   | weak `.text` in a foreign unit | an inline is now emitted there | A4.4 |
   | a normal function's `.text` | the caller's codegen changed | check the ref types (`ref-return-types-matter`) |
   | vtable data | class declaration order or virtual order | `vtable-emission-order` |
   | link error | a mangled name changed | `elf_symbol_addrs.txt` is the truth |

   With several edits in one build, bisect per class. Don't guess.

4. **V2:** the funcdiff overviews of the owner units equal the baseline.

5. **Hidden-conflict recheck:** run `--header` again. The number of includers that "add strings" must not
   grow, and that includes units whose `.rodata` is still asm, which V3 can't check.

6. **V4:** review the diff. Only declarations, layouts, includes and mechanisms change; matched function C
   changes only where the ref types require it.

7. **V5:** `tools/check.sh` before the merge, and again on main.

### A1. Unit start hook

On `decomp/<U>`, before the first function:

1. `partials.py --unit <U>`.

2. Replace each foreign stand-in whose owner is `complete` or `superset` and string-compatible with an
   `#include` of the owner header.
   - If the unit has `.rodata` from source, put the include where the original's header-string order says
     (the tool prints it).
   - Respect vtable-emission order.
   - Fix call sites only where the ref types differ from the stand-in's.

3. If the owner header lacks a member the stand-in had, add it there in ref form. Mark it
   `PARTIAL: added for <U>` while the class is still incomplete.

4. A stand-in that can't be replaced keeps a reason the tool can read:
   `/* PARTIAL: stand-in for <Owner> <Class>: <owner not written | gated by M | strings: ...> */`.

5. Optional and encouraged: the unit's own class fields from the ref instead of padding.

6. Commit per owner header: `<U>: use <header> instead of stand-ins (<classes>)`.

7. **Time-limited:** a failure that stays unclear after a few A4 rounds is reverted, logged in
   `build/progress/<U>.md`, and the unit's functions start. The hook must never block the unit.

### A2. Unit finish hook

**Part 1, on the unit branch, after the data step of "Finishing a unit"** (so the unit's weak tail and
`.rodata` come from C, and the build checks the header). For every class the ref assigns to U:

1. **Bring it to the full ref class.**
   - Fields under their ref names, in ref order, with no padding arrays. Delete accessors that cast into
     padding.
   - All methods in ref order. Virtuals in the `/* vtable[n] */` order. Statics, enums and typedefs too.
   - Skip the implicit members.

2. **Inline bodies need evidence:** the key unit's weak tail, weak copies seen in some original object, or
   code you can see inlined. Without evidence, a method is only declared. Adding a body to a declared method
   changes every caller that then inlines it.

3. **Re-check each mechanism** (`#ifdef <U>_CPP`, `*_FULL_CLASS`, placeholders): try removing it and run
   V3. If V3 fails, keep it with `MATCH:` and name the units that need it.

4. **PARTIAL comments:** delete those of classes that now equal the ref. Delete the header's line-1 marker
   when the whole header matches the ref.

5. Commits: `<U>: complete <Class> in <header>` and `<U>: drop <MACRO>`.

**Part 2, on branch `cleanup/<Unit-dashes>`, from main after the unit's merge:**

1. Run `partials.py --class` for each of U's classes.
2. Replace the stand-ins as in A1. Delete duplicates in other headers, together with their guard-skip
   tricks, and include the owner header instead.
3. Drop include-guard pre-defines and per-includer gates that are no longer needed.
4. Commit per header edit, and the .cpp swaps per directory group of at most about 10 files, with V3 each.
5. Gate, merge, and regenerate `docs/partials.md` on main.

### A3. Between-units batch

1. On main, with a clean tree and `check.sh` passing, run `partials.py --queue` and take the top ready
   item.
2. Branch `cleanup/<Header-dashes>` (e.g. `cleanup/System-RsrcDB`), or `cleanup/<topic>` for a partition fix
   (e.g. `cleanup/task-slc-message`).
3. Header changes first, one commit each after V3. Then the .cpp swaps in groups, with V3 per group.
4. Keep a log in `build/progress/cleanup-<name>.md`: per file, the result and the cause of any failure.
5. V5, then `git merge --no-ff -m "Merge cleanup/<x>: <what> (stand-ins -N, PARTIAL -M, mechanisms -K)"`.
   The numbers are the `--summary` differences before and after.
6. V5 on main, then commit `docs: regenerate partials`.

An item over about 30 files is split by top directory (`Form/`, `Game/`, `System/`); each part is still one
queue slot. Do one batch per gap between units; an unfinished remainder stays at the top of the queue.

### A4. The ladder, when completing a header would break another unit's `.rodata`

Climb in this order and stop at the first step that works:

1. **Forward declarations instead of includes.** Example: `RsrcDB.h` drops `RsrcData.h` and `Task.h` and
   forward-declares the classes it needs. `NAME_CASE` moves into `RsrcDB.h`, as in the ref (enums can't be
   forward-declared in gcc 2.95), and `RsrcData.h` includes `RsrcDB.h`.
2. **Move the type to the header the original used.** Evidence: `partials.py --strings`, plus the ref owner.
3. **Move types that only the owner uses into the owner's .cpp**, keeping its include order and vtable
   order.
4. **A weak function now emitted in a foreign unit:** check the original object (`tools/asm.py <unit>`). If
   the original doesn't have it, keep the method declaration-only outside the key unit with
   `#ifdef <KEY>_CPP` and a `MATCH:` reason (`inline-called-not-inlined`).
5. **Last resort: a per-unit opt-out macro,** with `MATCH:` ("original unit lacks <H>'s strings; real
   header split unknown") and an "open puzzles" entry.

Not allowed: inventing a header split the original didn't have, or making a class a template to hide its
strings.

### A5. Stop and ask the user (in addition to plan-big-units' list)

- A partition fix outside the approved queue below that reaches more than about 20 includers, or that moves a
  type against the ref owner.
- String evidence that contradicts the ref.
- A new kind of mechanism.
- Completion that needs changes to matched function C beyond ref type corrections.
- Unifying `Gprim.h` and `GprimFull.h` (the `#pragma interface` mechanism).
- A new opt-out macro that more than about 5 units need.
- No net decrease of stand-ins plus PARTIAL markers after 3 consecutive units.

### Branches and commits

| work | branch | messages |
|---|---|---|
| start hook | `decomp/<Unit-dashes>` | `<U>: use <header> instead of stand-ins (<classes>)` |
| finish hook, part 1 | `decomp/<Unit-dashes>` | `<U>: complete <Class> in <header>`, `<U>: drop <MACRO>` |
| finish hook, part 2 | `cleanup/<Unit-dashes>` | `cleanup <header>: use it in <dir> (<n> files)`, `cleanup <header>: remove the duplicate <Class> from <other.h>` |
| between-units batch | `cleanup/<Header-dashes>` or `cleanup/<topic>` | `cleanup <header>: <what>` |
| tool | `tools/partials` | `tools: add partials.py (PARTIAL/stand-in inventory)` |

- Cleanup and decomp commits are never mixed, except the hook edits on U's own branch. A cleanup branch never
  contains function matches.
- V3 at every commit, V5 before every merge and again on main.
- No rebase, no remote, never commit `build/`, `ref/`, `asm/` or `iso/`.
- Commit messages end with the Co-Authored-By line from the session's instructions.

## Initial queue (between-units batches, in order)

1. **Pilot: `CSound` → `System/Sound/SoundMan.h`** (14 files, R0, .cpp-only edits, fast rebuilds). It
   validates the tool and A1.
2. **The `System/RsrcDB.h` partition** (R3), then the stand-in swaps.
   - Move `NAME_CASE` and forward declarations into `RsrcDB.h`, and drop its `RsrcData.h`/`Task.h` includes.
   - Move the types only RsrcDB.cpp uses into it, keeping the include order (`RsrcDB.h` before
     `LinkedBuf.h`, rsrcdb lessons).
   - Then swap the `CRsrcDB` (65) and `NAME_CASE` (30) stand-ins, in 3 directory groups.
   - Watch for stand-ins whose signatures differ from the real ones.
3. **Relocate the Task SLC overflow message** (R3). `CTaskSLCManager` moves to `TaskMan.h`, as in the ref;
   the `Task.h` placeholder and the 154 `TASK_H_NO_SLC_OVERFLOW_MSG` defines go.
   - Only if the evidence (99 originals with the message) agrees with our `TaskMan.h` includers. Otherwise
     ask.
   - It is a prerequisite for 5, 7 and 8.
4. **A new `System/Graphic/PackMan.h` from the ref** (string-free) and the `CPackMan` swaps (31). The ref
   decides the signatures (e.g. `pkOpenGifTag`).
5. `CBattleEnemyManager` (11), `CAtel` (7) and `CSceneEnv` (6). Check the strings of `SceneEnv.h`'s
   `EffectMan.h` include.
6. **Retro finish hooks of already finished units,** one per gap: BattleEnemy, Movement, Anim (plus its 2
   include-guard pre-defines), Game, CardBattle (after CardRevolver), Player, and Gprim (without the
   GprimFull unification).
7. **Ungate `2DMan.h`** (`D2::C2DMan` ×27). Evidence step first: the 2D container message is in 166
   originals, but the 2DMan.h path is plain text in only about 119, so the message may belong to another
   header. Ask if the evidence is ambiguous.
8. **Ungate `RsrcData.h`** (`RSRCDATA_H_FULL`, `CRsrcData` ×11), after 2 and 3.
9. **`CRcf` (26) → `Rcf.h`**, with the weak-hint check for `CSJointListT<RcfSceneInfo>`.
10. **The duplicate `CCollBody`** (`BodyCollision.h` vs `Collision/CollBody.h`).

**Deferred to unit hooks:**
- CardRevolver: its `CStockFolder`, `CEnchantGauge` and `CCardRevolver` copies in `CardBattle.cpp` and
  `CardBattle.h`.
- Model: `TexInfo`, `VTransInfo` and `MODELDATA_H_TEXINFO`.
- SceneInfo: 76 PARTIAL markers.
- Entity: 106 PARTIAL markers, the `CFSM` duplicate and `ENTITY_H_SETFLAG_DIRECT`.

## Sequencing

1. Step 0 (the tool).
2. Step 1 (the docs).
3. Batch 1, the CSound pilot, in the current gap: after CardBattle, before PlayerSora.
4. PlayerSora's start hook.
5. Then the normal unit loop of plan-big-units, with the start and finish hooks and one queue batch per gap.

## Definition of done

1. Every class whose owner unit is finished is `complete`: one definition, in its owner header, equal to the
   ref, no padding, no PARTIAL inside.
2. There are no foreign stand-ins for complete or superset owners. The rest are listed in `docs/partials.md`
   with a reason (owner pending, or an unattributed overlay).
3. There are no duplicate class definitions, except mechanisms the user approved (the Gprim pragma), tagged
   `MATCH:`.
4. Every remaining gate, opt-out, include-guard pre-define, placeholder, path override and pragma has a
   `MATCH:` reason: `untagged` = 0.
5. The PARTIAL count falls with every merge in the trend table, and is 0 in headers whose owners are all
   finished.
6. The string-evidence section has no unexplained mismatch.
7. `tools/check.sh` exits 0 on main.

## Verification

- **The tool:** the acceptance numbers above, and `--strings` output that matches a manual grep of the
  expected `.rodata.s` files.
- **Every cleanup commit:** a full `ninja` that passes the SHA-1 (V3), and `tools/imagediff.py` reporting no
  differing bytes.
- **Every merge:**
  - `tools/check.sh` exits 0 on the branch and again on main.
  - The funcdiff overviews of the affected owner units show no regressions.
  - The `partials.py --summary` counts are lower than before; quote them in the merge message and in the short
    report to the user.

## Critical files

- **New:** `tools/partials.py` (pattern `tools/leftovers.py`, helpers from `tools/progress.py`) and
  `docs/partials.md`.
- **Amended:** `docs/plan-big-units.md`, and `CLAUDE.md` (one line for the tool).
- **First batches:**
  - `src/System/Sound/SoundMan.h`;
  - `src/System/RsrcDB.h` and `src/System/RsrcData.h`;
  - `src/System/Task.h` (around lines 179-205) and `src/System/TaskMan.h`;
  - `src/System/Graphic/2d/2DMan.h`;
  - `src/System/Graphic/PackMan.cpp`, with a new `PackMan.h`;
  - the stand-in .cpp files the tool lists.
