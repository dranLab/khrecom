# Plan: align the decompilation with the US retail ELF (second build target)

Status: proposed 2026-11-10, not started. Phase 0 is read-only; nothing below has been implemented.
Updated 2026-11-10 (after port step 1 was merged into `main`, and merged into this branch): the retail ELF's SN
DLL table, what port step 1 brings, and Phase 0/1 revised accordingly.

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

### The retail ELF has a symbol table after all: the SN DLL table (measured 2026-11-10)

Retail is stripped of ELF symbols and `.mdebug`, but like the beta it carries **SN Systems' DLL table** in `.sndata`
(`sn_dll_header_root`, magic `SNR2`, at 0x367880; format in `src/lib/nodebug/check_dll.c`, parser in
`tools/sndll.py`): the main program's exported symbols, **names with retail addresses**, for the runtime DLL loader.

- **12,511 symbols** (beta 13,167): 9,552 global, 2,864 weak, 88 imports, 6 absolutes, 1 local; 101 import
  relocations (beta 125). 12,150 names are in both tables.
- **10,726 of the beta's 12,120 functions (88 %) are named in it** with their retail address. Sorted by beta address,
  the retail addresses keep the order for 98.7 % of consecutive pairs: units stay in link order, so per-unit retail
  ranges follow directly. 1,424 shared names are data (globals, class statics, vtables `_vt$...`): data anchors.
- The 361 names only in retail are not new code: 342 are anonymous-namespace statics whose 6-character suffix
  differs per compile (`_47_GLOBAL_$N$c__KHCOM_MainApp_Game_Game.cppziaaaa$s_pFadeMask` in the beta,
  `...Game.cpptAaaaa$s_pFadeMask` in retail), 19 are static constructors/destructors (`_GLOBAL_$I$...`/`$D$`) with
  the same kind of suffix. The suffixes are per version: the retail symbol file must carry retail's (ee_asm_compat
  gives compiled objects the names from the version's symbol file).
- The 1,017 names only in the beta: the suffix variants above, plus 675 really missing in retail: debug code
  (`AssertApp`, `CDbgCube`, `CCollDisp`, `CRobotPad`, `CNakCamera`, the debug allocator `mspace_malloc_d`, test
  tasks) and functions whose signature changed (e.g. `__12CMoviePlayerP5CTaski`).
- The 1,394 beta functions without a retail name are file-local statics (`dbgButtonTrg__F4USERUii`, ...), crt0's
  `_root`, and debug-only code: these, and only these, need the masked instruction matching.
- **Imports:** retail's 88 are a subset of the beta's 109; the 21 missing are the debug/viewer tasks of
  `MainTaskFactory` (`__14CKishiwadaTestP5CTask`, the editors, viewers) and `C2DViewer::m_seqControl`.
- **Absolutes:** `_stack_size` 0x40000, `_stack`/`_heap_size` -1 (as the beta), `_gp` 0x43F2F0, `_end`/`end` 0x5D36E8.
- **COMMON block:** 38 of the beta's 44 COMMON symbols are in the retail table, in a *different* order (the SN
  linker's own order, as in the beta): the retail order comes from these addresses.
- **Layout:** the same section order as the beta (`.text` 0x100000, `.data` 0x332880, `.sndata` 0x367880 with the
  same size 0x98000, `.rodata` 0x3FF880, `.sdata` 0x437300, `.sbss` 0x437380, `.bss` 0x437480 to 0x5D36E8) and the
  same 17 `.DVP.overlay` sections.
- **crt0** (0x280 bytes at 0x100000) differs from the beta only in address operands (8 `lui` + 8 `addiu`
  immediates, 4 `jal`, 2 `j` targets): `src/crt0.s` (symbolic since port step 1) should build retail's as is.

### What port step 1 (merged into `main` 2026-11-10) brings

Port step 1 (`docs/port-step1-shiftable.md`) made the beta build independent of its addresses; a retail build is
the same code at another layout, so most of that work carries over:

- **No pinned addresses left** (`tools/shiftpins.py`: 0): the objects link anywhere. A retail link needs a retail
  object order and the retail data layout, not address fixes.
- **`.sndata` is generated** (`tools/sndll.py`): from the image's table bytes and the link's objects, every pointer
  symbolic, import sites checked against relocations. For retail it reads retail's table; the import set comes from
  `config/<VERSION>/dll_imports.ld` (retail: the 88 above). `sndll.py --names` lists source names that differ from
  the table's.
- **The COMMON block is placed from C** (`tools/commons.py`): sizes/alignments from the objects, the order from the
  original's addresses (`build/<VERSION>/common/pieces.txt`, from splat's `common.bss.s` today). For retail: the
  order from the retail table/split; the 6 COMMON symbols missing in retail's table must be checked (debug-only?).
- **Data labels named in compiled objects** (`tools/name_data_labels.py`, `<obj>.labels` from the version's split):
  works per version as long as the retail split's ranges are right.
- **Per-version config files** besides the yaml/symbol files: `dll_imports.ld`, `linker_script_matching.ld`
  (`_end` == 0x5D36E8 for retail), `shift.yaml`/`shift_allow.txt` (if the retail build is shift-checked too),
  `symbol_addrs.txt` entries `// can_reference:False` (VU microcode `My_VU0_start`, `vu1mpg_start`, `vu1mpg2_start`,
  `pppVu1FogShapeProg`, `pppVu1ShapeProg`, `chswTable`: retail has the same DVP data; splat will make the same false
  references there).
- **Lessons for a splat split without `.mdebug`:** splat takes string bytes, VU microcode and 16-bit tables for
  pointers (`.sndata`'s strings, `lib/table`, the DVP area); a data object's labels may be gaps (`D_...` one past an
  array) rather than symbols; check every retail data object's references the same way (`tools/shiftcheck.py`'s
  suspects/review lists work on any version's matching vs shifted link).
- **Source changes already done** that retail needs anyway: DLL imports undefined and under their original names
  (`CTitle`/`CBtlReport` swap, the eleven bosses, gimmicks), the 44 COMMON globals declared in their units,
  `draw_w.c`/`global_w.c` for data-only units, `mpc.c`'s mask address symbolic.

The bet behind this plan: if our C is the original source, then compiling it with the debug macros switched off
reproduces retail code without further work for most functions. Phase 2 tests that bet on a pilot before any mass
change.

## Rules

- Branch `retail/target` (worktree), merged into `main` only by the user. The beta build stays byte-identical:
  `tools/check.sh` exits 0 for the beta on every commit, and since port step 1 `tools/check.sh --shift` too (no new
  pinned addresses, no pointers as bytes: CLAUDE.md "Shifted build").
- This plan may change `configure.py`, `tools/`, `include/` and add `config/SLUS_217.99_retail/` (user approval of
  this plan); it never edits `config/SLUS_217.99/SLUS_217.99.yaml` and never commits game data.
- No faked success: no SHA-1 changes, no pins of named objects to retail addresses, no post-link patching (as in the
  port plan's rules).

## Phase 0: classify the differences (read-only, ~1 session)

1. Script `tools/version_map.py` (new): **first the DLL table** (`tools/sndll.py`'s parser on both images): every
   name in both tables pairs a beta address with a retail address (10,726 functions, 1,424 data symbols); names that
   differ only in the anonymous-namespace / `_GLOBAL_$I$` suffix pair by the name without it. **Then** masked
   instruction sequences (the method above) for the rest (~1,400 beta functions: statics, debug-only code), anchored
   between named neighbours (98.7 % of the order is kept), and the call graph (a matched caller's `jal` targets pin
   its callees). Output: beta address → retail address for functions, a per-unit retail range, the unmatched
   functions per unit, and the retail names of the anonymous-namespace statics.
2. Data mapping: the table's data names first; for the rest, from code: for every matched function pair, pair the
   `lui/addiu`/`lw`/`sw` targets of beta and retail → beta data label → retail address (tables, strings, vtables,
   globals).
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
   symbols at their retail addresses, the retail suffixes of anonymous-namespace names; unmatched ones keep splat
   names). The DLL table gives most of them directly.
   Per-version files from port step 1: `dll_imports.ld` (the 88 retail imports), `linker_script_matching.ld`
   (`_end` == 0x5D36E8), the `can_reference:False` entries of `symbol_addrs.txt` (VU microcode, `chswTable`), the
   COMMON order. `configure.py`'s port-step-1 parts (`split_common`, the `.sndata` generator, `name_data_labels`,
   the shifted links) read per-version files already, but were only run for the beta: check each with the retail
   config.
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
`tools/elf_info.py`; splat's normal flow for the assembly-only build; from port step 1: `tools/sndll.py` (the
DLL table parser and generator), `tools/commons.py`, `tools/name_data_labels.py`, `tools/shiftpins.py` /
`tools/shiftcheck.py` (to check a retail split's references), `config/SLUS_217.99/dll_imports.ld` (the retail one is
its 88-name subset).

## Verification

- Every commit: `tools/check.sh` (beta) exits 0, image SHA-1 9bd4535c… unchanged.
- Phase 1 done: `ninja` builds both images; retail SHA-1 865450e0… matches from assembly.
- Phase 2/3: `tools/funcdiff.py --version SLUS_217.99_retail <unit>` per unit; `tools/check.sh` checks both images.
- CI green with both secrets.
