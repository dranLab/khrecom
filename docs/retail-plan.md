# Plan: align the decompilation with the US retail ELF (second build target)

Status: proposed 2026-11-10, not started. Phase 0 is read-only; nothing below has been implemented.

## Context

The project matches the **US translation beta** `iso/SLUS_217.99` (SHA-1 9bd4535c…), chosen for its debug info
(symbols, `.mdebug`, from which `ref/`, the object split and the line markers come). The user wants the work to also
produce the **US retail** executable, `iso/SLUS_217.99_retail` (SHA-1 865450e0…, 3.4 MB, dated 2008-10-21).
Decision (user, 2026-11-10): **keep both**. Retail becomes a second target built from the same `src/`, the beta stays
the reference and keeps its checksum and CI.

Measured today (read-only, masked comparison of every beta function against retail `.text`):

- Retail is **stripped** (no symbols, no `.mdebug`). Same SDK (libsce 3100) and toolchain fingerprints. `.text`
  0x232858 vs 0x292060 (-18 %). Its data is laid out differently (separate `.rodata`, small `.sdata`/`.sbss`).
- Retail has **none** of the beta's 2,325 `c:/KHCOM/...` assert/allocation strings: it is the release configuration
  of the same code (asserts, debug allocation messages and debug printing compiled out).
- **Libraries:** 936 of 940 functions present unchanged (address fields masked): library work carries over as is.
- **Game:** 6,805 of 9,295 functions present unchanged (73 %, 46 % of the bytes). Of the 2,494 that differ, 1,349
  call `AssertApp`/`com_malloc_d`/`printf` in the beta (1,357 do in total: almost all of them differ); 1,145 call none
  (inlined header asserts, layout effects, genuine retail changes: to be classified). 10 units match no function
  at all (CreateGimmick, CreateEnemy, System/Debug, CreateBullet, CollParam, DebugWork, CardKind, ...).
- Our source writes the debug forms explicitly: 595 `AssertApp(__FILE__, __LINE__, …)` sites plus unit-local
  `ASSERT` macros, 1,055 `new (__FILE__ "(N)") T` new-expressions, 55 direct `com_malloc_d` calls. 12 files have
  `INCLUDE_ASM("asm/SLUS_217.99/…")` paths.

The bet behind this plan: if our C is the original source, then compiling it with the debug macros switched off
reproduces retail code without further work for most functions. Phase 2 tests that bet on a pilot before any mass
change.

## Rules

- Branch `retail/target` (worktree), merged into `main` only by the user. The beta build stays byte-identical:
  `tools/check.sh` exits 0 for the beta on every commit.
- This plan may change `configure.py`, `tools/`, `include/` and add `config/SLUS_217.99_retail/` (user approval of
  this plan); it never edits `config/SLUS_217.99/SLUS_217.99.yaml` and never commits game data.
- No faked success: no SHA-1 changes, no pins of named objects to retail addresses, no post-link patching (as in the
  port plan's rules).

## Phase 0: classify the differences (read-only, ~1 session)

1. Script `tools/version_map.py` (new): matches beta functions to retail by masked instruction sequences (the
   method above), then extends matches by link order (units keep their order) and by call graph (a matched caller's
   `jal` targets pin its callees). Output: beta address → retail address for functions, a per-unit retail range,
   and the unmatched functions per unit.
2. Data mapping from code: for every matched function pair, pair the `lui/addiu`/`lw`/`sw` targets of beta and retail →
   beta data label → retail address (tables, strings, vtables, globals).
3. Classify the 2,494 differing game functions with retail asm: (a) only debug calls removed, (b) inlined header asserts
   removed, (c) other changes. Category (c) decides how much real retail decompilation remains. Report as
   `docs/retail.md` (new): counts per unit, the list of (c), and the retail `operator new`/allocation form.

**Go/no-go after Phase 0:** if (c) is small (a few hundred functions), continue. If it is large, retail is a
substantially different build and the user decides again.

## Phase 1: a retail build in assembly (infrastructure)

1. `config/SLUS_217.99_retail/` via `tools/init_version.py iso/SLUS_217.99_retail` (name = the file name the user
   chose; it avoids the beta's `SLUS_217.99`). `build.yaml` copied from the beta's, overrides included (libraries
   are identical).
2. Split without `.mdebug`: a generator (extend `tools/split_objects.py` with a `--from-map` mode, or a small new
   tool) writes the retail yaml from Phase 0's unit ranges and data mapping, with the beta's unit names and source
   paths. Symbols: `config/SLUS_217.99_retail/elf_symbol_addrs.txt` generated from the map (names of matched beta
   symbols at their retail addresses; unmatched ones keep splat names).
3. Multi-version build: `configure.py <version>` already takes a version; make it write a per-version ninja file
   (`build/<VERSION>/build.ninja`, root `build.ninja` including both) so one `ninja` builds and checks both images.
   `tools/check.sh` checks both (`--version` to limit). Tools that default to `SLUS_217.99` (funcdiff, asm, drafts,
   imagediff, leftovers, candidates, compare_compilers) get `--version`.
4. `INCLUDE_ASM` becomes version-neutral: `include/include_asm.h` prefixes `"asm/" KH_VERSION_DIR "/nonmatchings/"`;
   the 12 files' paths lose `asm/SLUS_217.99/` (mechanical, beta image unchanged).
5. Milestone: the retail image builds 100 % from splat assembly and passes its SHA-1 (the "Starting a new game
   version" step 3 of the README, for retail).

## Phase 2: pilot (the bet)

1. Version macros in one place: `include/khversion.h` (new), included by `System/MainApp.h` and `System/HeapMan.h`;
   `configure.py` passes `-DKH_RETAIL` for the retail version (from its `build.yaml`).
2. The original's debug forms as macros that compile to the beta's exact code and to nothing (asserts) / the plain
   form (allocations) in retail: an `ASSERT(e)` like the unit-local ones already in RoomCreation.cpp,
   GameSubBattle.cpp, AttackMotion.cpp, AttachCard.cpp; a `NEW` form for `new (__FILE__ "(N)") T` (retail form taken
   from Phase 0's operator new finding); the same for `com_malloc_d` and debug printf.
3. Convert 3-5 small complete units that mix assert, allocation and plain code (pick from Phase 0's category (a)
   list), switch them to compiled source in the retail split, and compare per function with `tools/funcdiff.py
   --version SLUS_217.99_retail`.
4. **Go/no-go:** most of their functions match retail unchanged → Phase 3. If not, the differences show what the
   release build changed (flags, macro forms), and the plan is revised with the user.

## Phase 3: bring all units over

1. Mechanical conversion of all assert/allocation/printf sites to the macros, unit group by unit group, gate on the
   beta after each group (beta image must stay identical; line numbers kept, as the cleanups did).
2. Units switch to compiled source in the retail yaml as they match; data: `// SOURCE DATA:` needs a per-version
   form where retail data differs (configure change; e.g. `// SOURCE DATA retail: .rodata`).
3. Category (c) functions: `#ifdef KH_RETAIL` variants decompiled from retail asm with the existing loop
   (funcdiff/permuter, `--version`), parked like today when they don't match.
4. `tools/progress.py`/`docs`: a retail column; `docs/retail.md` keeps the per-unit state.

## Phase 4: workflow and CI

- `.github/workflows/check.yml`: a second secret (`RETAIL_ELF_URL`, SHA-1 865450e0…), both images checked.
- `docs/plan.md`: a "Retail" section (rules, work order); decomp-pool can take retail items once Phase 3 runs.
- CLAUDE.md: the two versions, `--version`, the macros.

## Critical files

`configure.py`, `tools/check.sh`, `tools/split_objects.py` (or the new generator), `tools/init_version.py`,
`tools/funcdiff.py` and the other version-defaulting tools, `include/include_asm.h`, new `include/khversion.h`,
`src/System/MainApp.h`, `src/System/HeapMan.h`, `config/SLUS_217.99_retail/*` (new), `.github/workflows/check.yml`,
new `tools/version_map.py`, new `docs/retail.md`. Reuse: the masked-function matcher prototyped in this session's
measurements; `tools/compare_compilers.py`'s `functions()`/`difference()` for per-function comparison;
`tools/elf_info.py`; splat's normal flow for the assembly-only build.

## Verification

- Every commit: `tools/check.sh` (beta) exits 0, image SHA-1 9bd4535c… unchanged.
- Phase 1 done: `ninja` builds both images; retail SHA-1 865450e0… matches from assembly.
- Phase 2/3: `tools/funcdiff.py --version SLUS_217.99_retail <unit>` per unit; `tools/check.sh` checks both images.
- CI green with both secrets.
