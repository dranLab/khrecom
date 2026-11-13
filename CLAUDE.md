# khrecom - matching decompilation of Kingdom Hearts Re:Chain of Memories (PS2, SLUS_217.99)

Goal: C/C++ in `src/` that, built with the original toolchain, reproduces the game's executable byte for byte.
Everything runs in Linux (WSL), from the repo root, with the project's venv: `.venv/bin/python` (the system
`python3` lacks splat). **Never commit game data** (`iso/`, `ref/`, `asm/`, `build/`).

## Build and verify

- `tools/check.sh` - the gate: configure + full build + checksums (every version, see "Versions") + overview. Must exit
  0 before any commit/merge.
  CI (`.github/workflows/check.yml`) runs it from a fresh clone on every push to `main` (game ELF from the
  repository secret `GAME_ELF_URL`, optional `GAME_ELF_TOKEN`), so the build must not depend on local leftovers.
  Lists only files not complete or changed on the branch (all: `--full` or `build/check.overview.txt`).
- `.venv/bin/python configure.py` - re-split (splat) and write `build.ninja`/`objdiff.json` (~30 s). `check.sh` runs it.
- `ninja` - build; fails unless the rebuilt image matches the original (SHA-1).
- `.venv/bin/python tools/funcdiff.py <unit> [<function>]` - status per function, or original | ours side by side
  (only the differences with 3 lines of context; `-C N` for more, `--full` for every line).
- `.venv/bin/python tools/imagediff.py` - where the rebuilt image differs from the original (object, section,
  symbol); `check.sh` runs it when the checksum fails. funcdiff ignores data: wrong data from source shows here.
- `.venv/bin/python tools/asm.py <unit> [<function>...]` - the original's functions (size, weak/global) or their
  assembly, compact (a third of the raw `.s`); `--data` for the unit's data.
- `.venv/bin/python tools/drafts.py [--part game|lib] [--unit <unit>] [--failed] [-j N]` - every parked draft built in a copy
  and measured (funcdiff's line count, closest first), wherever it sits relative to its INCLUDE_ASM; a draft that does not
  build gives its first error. The re-measure step: notes go stale when headers change.
- `.venv/bin/python tools/permuter.py <unit> <function> [--minutes N] [-j N]` - random source mutations of one nearly
  matching function (statement/declaration order, operand order, temporaries, ...), compiled in parallel and scored
  (C and C++: the function is its symbol, `Class::name`, or a source name unique in the unit; overloads: picked by the symbol's parameter count, then the parked draft; `--overload <params>` when still ambiguous)
  against the original; the best variant goes to `build/permuter/<function>/best.c` (+ `best.diff`). Parked drafts
  under `#if 0` are switched on in its copies; the real source is never touched. Check a result's semantics.
- `.venv/bin/python tools/decomp_permuter.py <unit> <function> [--minutes N] [-j N]` - for `.c` units only: runs
  decomp-permuter (`tools/decomp-permuter`, git submodule; pycparser, AST randomizations) on the preprocessed unit
  with the unit's own build; results in `build/decomp-permuter/<function>/output-*/diff.txt`, rescored at the end in
  funcdiff-style differing lines (its own scores weigh differences differently and can rise while lines drop).
  `--setup-only`, then edit `base.c` (PERM_* macros, PERM_IGNORE for unparsable code) and `--resume`; `--report`.
- `.venv/bin/python tools/compare_compilers.py SLUS_217.99 src/<unit>.cpp` - per function, with every installed compiler.
- `.venv/bin/python tools/candidates.py SLUS_217.99` - files not decompiled yet (size, functions, header strings).
- `.venv/bin/python tools/leftovers.py lib|game [--update docs/sdk-leftovers.md|docs/asm-leftovers.md]` - what is left
  per function (parked `#if 0` drafts with their `Parked:` notes, hand-written, not attempted), asm data sections;
  `game --unit <unit>` lists every function of one unit that is still INCLUDE_ASM.
- `.venv/bin/python tools/partials.py [--summary|--unit <unit>|--header <h>|--class <C>|--queue|--strings [<h>]]` -
  PARTIAL/stand-in/mechanism inventory and class readiness vs the ref (`docs/plan.md`, "Header cleanup");
  `--update docs/partials.md` regenerates the report.
- `tools/setup_worktree.sh` - prepare a fresh git worktree (links game files and tools, runs configure).
- `tools/helpers/` - the analysis helpers docs/plan.md's methods name (line markers `asmlines.py`, ref locals
  `reflocals.py`/`refjoin.py`/`refextra.py`, scheduler `schedshow.py`, order searches, variant testers); run from the
  repo root, read one before use, copy it to `build/` to change it. Several read `build/drafts.log` (`tools/drafts.py`).

A *unit* is one original object file, named after its source path, e.g. `System/Graphic/ColorFader`
(from `c:/KHCOM/MainApp/System/Graphic/ColorFader.cpp`). Libraries are `lib/...`: see "Library units" below.

## Where things are

- Original assembly of a unit: `build/SLUS_217.99/expected/asm/<unit>.s`, its data in
  `build/SLUS_217.99/expected/asm/data/<unit>.{data,rodata,bss}.s`.
- Reference from the debug info (not compilable, a guide): `ref/SLUS_217.99/src/<unit>.cpp` and `.h` (class
  layouts, signatures with parameter names, locals, initialized data), `ref/SLUS_217.99/types.h` (all types).
- Symbols: `config/SLUS_217.99/elf_symbol_addrs.txt` (original names; generated - don't edit).
- Splits: `config/SLUS_217.99/SLUS_217.99.yaml` is generated by `tools/split_objects.py`. **Don't edit it.**
- Base types (INT, BOOL, FLOAT, TIMEQ, ...): `include/khtypes.h`.

## Decompiling a unit

The plan for all remaining work (state, work order, per-function loop, validation, header cleanup):
`docs/plan.md`.

1. Create `src/<unit>.cpp` (or `.c`: see the source path in the yaml comment, or `candidates.py`). Its existence
   alone switches the unit's code to the compiled object (`configure.py` does it; never edit the yaml).
2. Headers for the unit's own classes go next to it (`src/System/Graphic/ColorFader.h`). Include other project
   headers as `"System/..."`. `__FILE__` then becomes the original `c:/KHCOM/MainApp/...` path.
3. Functions not done yet: `#include "include_asm.h"` and
   `INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/<unit>", <mangled name>);` - splat writes the function's original
   assembly there when you run `configure.py`. The file then still builds byte-identical. Functions must stay in
   the original order; weak (inline) functions come last (see below).
4. When the unit's data matches too, add as the **last line** of the source: `// SOURCE DATA: .data .rodata .bss`
   (only the sections the unit has). Until then its data comes from the original assembly (needed while
   INCLUDE_ASM'd functions reference data labels).
5. Iterate with `tools/funcdiff.py`, finish with `tools/check.sh`.

## How the original was built (matters for matching)

- SN Systems ProDG: `ee-gcc 2.95.3-136`, `-O2 -G0 -fno-exceptions -fno-rtti -g`, assembler Ps2EeAs; the build
  does this (`build.yaml`, `configure.py`). GCC 2 name mangling: `func__5CTaski`; signatures must reproduce the
  original mangled names exactly (see `elf_symbol_addrs.txt`).
- Anonymous-namespace statics (`_58_GLOBAL_$N$c__KHCOM_..._X.cppM1aaaa$s_fFoo`) are `namespace { ... }` members;
  the build gives them the original names. `name.123` symbols are function-local statics.
- **Weak functions at the end of an object are inline functions** defined in a class body (or `inline` in a
  header), emitted in the file that defines the class's first non-inline, non-pure virtual function. A class
  whose virtuals are all inline or pure has no vtable of its own. gcc still emits a weak vtable for it in every
  object that constructs it; the original linker kept one copy, so the build drops copies outside the object's
  data from source (`tools/ee_asm_compat.py`): declare such classes normally. Global functions are the `.cpp`'s own.
- `.rodata` of an object: first the string literals of all inline functions of every included header (in include
  order, used or not - assert/allocation messages like `"...Gprim.h(613)"`), then the constants of the file's
  functions, vtables last. To match it, headers must reproduce those strings, including `__LINE__`.
- `__LINE__` is in assert/allocation messages (and `AssertApp`'s line argument): keep the original line numbers
  (layout the file accordingly, or use `#line`). That is why `// SOURCE DATA:` goes at the end.
- Asserts: `AssertApp(__FILE__, __LINE__, "<expanded expression>", 0)`; debug allocations:
  `com_malloc_d(size, __FILE__ "(" line ")", -1)` (see `System/HeapMan.h`, `System/MainApp.h` in `ref/`).
- `long` is 64-bit; `int`, `LONG`, `INT` are 32-bit.

## Library units (`lib/...`; open work, levers, per-unit notes and history: `docs/sdk-leftovers.md`)

- Sony SDK, libgcc, newlib, libsn: built with other toolchains. `build.yaml`'s `overrides:` (fnmatch on the unit
  name, first match wins) sets `compiler`/`cflags`/`source_root`/`assembler` per unit. `assembler: <package>` uses
  that package's GNU `bin/ee-as` instead of the compiler's own (Ps2EeAs, or modern GNU as for packages without
  one). GNU as in `.set reorder` fills delay slots gcc left open and pads short loops itself.
  `compare_compilers.py` uses the unit's override (`--assembler none` for each compiler's own).
- No `ref/`, no `__LINE__`/assert strings. SDK declarations go in `include/sdk/*.h` (`#include "sdk/libmpeg.h"`),
  written from call sites and the asm; library-internal structs in the unit's own source or `src/lib/*.h`.
- Hand-written assembly: `src/lib/<unit>.s` replaces splat's `.s` for that unit (same object, GNU as); its data
  from source with a last line `# SOURCE DATA: .data` (klib.s).
- `candidates.py --lib` lists the open library units; `check.sh` reports library progress separately.
- The libs' .mdebug has only names, no types: `stdump functions iso/SLUS_217.99` (tools/ccc) lists each lib file's
  functions in order, `static` exactly for the file-local ones (everything else is global: other data may point
  at it). Assemblers: `ee-as` 2.10 (ee-gcc2.96 package; no mfc0 hazard nops) and 2.9
  (ee-gcc2.9-990721; adds them). Sony's code has inline asm blocks with their own `.set noreorder` loops (DIntr).
- What is left (parked drafts with their remaining difference), the levers that worked in lib/ and the notes per
  unit (sifrpc, kprintf2, filestub, graphdev, csc, libpad2, hncAt3dec, ...): `docs/sdk-leftovers.md`. Keep it
  current: `Parked:` notes in the source, then `tools/leftovers.py lib --update docs/sdk-leftovers.md`.
- GNU as fills an open delay slot only when no label precedes the branch: -g's `$LM` line labels count. A `j $31`
  left unfilled where the original filled it: put the last statement and `}` on one line (lib/init).

| Family | Toolchain (verified on) |
|---|---|
| Sony SDK (`lib/*`: libkernl, libmpeg, libdma, ...) | ee-gcc2.9-991111-01 + ee-as 2.10 (`assembler: ee-gcc2.96`), `-O2 -G0 -g`: one toolchain for all Sony libs. 991111-01 is the 991111 patch release, only `div.s` differs (erratum nops; lib/timer has the only two) (alarm: kCopy only matches the 991111 family; libmpeg error.c saves with `sd`, 2.95.3 with `sq`). A function that only matches another compiler usually needs a source change (alalc: an inverted `if`) |
| libat3dec (`lib/init`, `lib/hncAt3dec`) | ee-gcc2.96 + its ee-as 2.10, `-O2 -G0 -g` (atrac2_mode_* only match 2.96; hncAt3dec saves registers 8 bytes apart, the Sony 991111 builds 16) |
| libgcc (`lib/src/gcc/*`, from GCC 2.9's libgcc2.c) | ee-gcc2.9-991111 + the 2.9 ee-as of ee-gcc2.9-990721 (`assembler: ee-gcc2.9-990721`): __floatdisf leaves a `jal` delay slot open that GNU as 2.10 would fill. Source is GCC's own (`src/lib/src/gcc/libgcc2.h` has the types and `umul_ppmm`); the EH runtime (L_eh, `-fexceptions`) is libgcc2.c of 1999-03-24, frame.c that of 2.95.3 |
| newlib libm (`lib/src/newlib/*`, fdlibm) | ee-gcc2.9-991111-01 `-O2 -G0 -g -fno-builtin` + its own 2.9 as (`assembler: ee-gcc2.9-991111-01`): this compiler pads `div.s` with nops (EE erratum; ef_fmod, kf_tan), -fno-builtin keeps fabsf a call, its as adds hazard nops (mfc1, c.cond) only where gcc enters `.set noreorder` and moves no stores into `jr` slots. Source is newlib's own; `src/lib/src/newlib/libm/fdlibm.h` has the macros |
| `lib/nodebug/*` (newlib libc, libsn; no debug info) | the game's own toolchain: ee-gcc2.95.3-136 `-O2 -G0` + Ps2EeAs (no `assembler:`); only 990721 and 2.95.3-136 matched the first str*/stdlib functions, and only Ps2EeAs gives strstr's loop-head nops. Source is newlib libc's |

## Versions (how the switch works: `docs/versions.md`; history and per-unit levers: `docs/retail.md`)

- `config/versions.yaml`: the beta `SLUS_217.99` (the reference: ref/, objdiff, the tools' default) and the US retail
  `SLUS_217.99_retail` (`iso/SLUS_217.99_retail`, from `iso/khrecom_retail.iso`). `configure.py` writes
  `build/<VERSION>/build.ninja` for each; `ninja` checks every image (`check-<VERSION>` for one; the retail's other
  targets carry `-SLUS_217.99_retail`). Tools take `--version` or `KH_VERSION`. A version without its ELF in `iso/` is
  SKIPPED.
- Retail builds the units of `config/SLUS_217.99_retail/sources.txt` from `src/` with `-DKH_RETAIL` (the rest from
  splat's assembly of the retail ELF); its split comes from `tools/version_map.py` + `tools/split_version.py`, its
  unit list and exact data starts from `tools/retail_units.py` (scan/switch/verify) - generated, never hand-edited.
  A source's retail data comes from source only with `// SOURCE DATA SLUS_217.99_retail: ...` (after the beta's line).
- Version switches (`include/khversion.h`, via khtypes.h), each on the line it changes (the beta's `__LINE__`s):
  `KH_DEBUG(x)` beta-only code/members, `KH_RELEASE(x)` retail-only, `KH_VER(beta, retail)`, debug allocations
  `new KH_AT("(N)") T` / `operator new(n KH_ATARG("(N)"))`, one-line asserts `KH_DEBUG(if (!(e)) AssertApp(...);)`.
  Declarations of the switched functions are written `(AssertApp)(...)`. A beta change that breaks only the retail
  image: `tools/retail_units.py verify` switches that unit back (docs/plan.md "Working rules").
- Debug-menu switches: `KH_DW(DebugWork::g_dw.x)` reads 0 in retail (`KH_DW_ON`, `KH_DWV(x, v)` for other defaults).
  Never write a debug allocation as a `"c:/KHCOM/....cpp(N)"` literal: `new KH_AT("(N)")` (a literal stays in retail's
  `.rodata`).
- A retail difference, in this order: debug code -> `KH_DEBUG` (the whole block, also where its condition folds to
  0); a real retail change -> `KH_VER`/`KH_RELEASE`; otherwise one source that fits both builds (the beta's line
  markers, the ref's locals); only then a `KH_DEBUG`/`KH_RELEASE` line pair or a filler the user accepted
  (`docs/versions.md` 4; a new kind needs the user).
- `build/SLUS_217.99_retail/version_map/` is generated by `configure.py` when missing; rerun `tools/version_map.py`
  after changing `config/SLUS_217.99_retail/function_pairs.manual.txt`. `retail_units.py switch --eligible` rewrites
  the whole list from the last scan (after a partial scan, switch by name); `try` masks `jal` targets (a wrong callee
  shows only in `check.sh`).

## Shifted build (port step 1; plan and boot log: `docs/port-step1-shiftable.md`)

- `ninja shift` links the matching objects with padding (`config/<VERSION>/shift.yaml` presets) and without any label
  pinned to its original address; `tools/check.sh --shift` also checks them (`tools/shiftpins.py`: nothing undefined
  or absolute; `tools/shiftcheck.py`: word-by-word against the matching link). `ninja shift-iso-<preset>`: boot disc.
- Labels inside a compiled object's data from source get their original names as symbols
  (`tools/name_data_labels.py`, `<obj>.labels`), so INCLUDE_ASM'd code and splat data may keep using `D_` names.
- Don't write a pointer as bytes in an `asm()` data block: `tools/shiftblocks.py` finds them (`--fix` restores
  `.word <label>`, line-neutral). Don't define `X = 0x00...` image addresses in sources.
- DLL imports are undefined in the sources and 0 in `config/<VERSION>/dll_imports.ld`; `.sndata` (the SN DLL table)
  is generated by `tools/sndll.py`. A new import goes into `dll_imports.ld`.
- Data that splat took for references but isn't (VU microcode): `// can_reference:False` in `symbol_addrs.txt`.
- COMMON symbols (uninitialized globals) are placed by `tools/commons.py` from the objects' COMMON records, in the
  original order: declare them in their unit's C with the original type (and `aligned(...)` where the original had
  it); the build refuses if the result doesn't land every one on its original address. A data-only unit may have a
  source when it lists its data in `// SOURCE DATA:` (draw_w.c, global_w.c).

## Pitfalls

- INCLUDE_ASM'd code references splat's names (`D_004EF8A0`, `get_st_001828C8`, `jtbl_...`); `configure.py` PROVIDEs
  every such address-named label at its original address (`build/<VERSION>/original_labels.ld`), so compiled code
  needs no asm labels or asm names for them (Ps2EeAs crashes on aliases anyway).
- Don't compare data bytes against `build/<VERSION>/expected/obj/data/*.o`: where splat read string bytes as
  symbol + addend, the object holds only the addend. Compare the original image (`imagediff.py`) instead.
- A crash (signal 11) in the `cc` step is Ps2EeAs hitting an assembler error (it can't print it under wibo).
  Look at `build/SLUS_217.99/src/<unit>.o.s` (what it assembled) and `.o.gcc.s` (raw compiler output).
- objdiff's data percentages can be below 100% for matching data (`func_0` relocations on zero words, padding
  before the next object). The checksum in `tools/check.sh` is what counts.
- WSL wipes `/tmp` when idle: keep scratch files in `build/`.
- VU0 inline asm: splat's `.s` (rabbitizer) and objdump print `vadda`/`vmsuba` with the two source operands
  swapped (`fs` and `ft` reversed, unlike every other VU0 instruction). Ps2EeAs follows Sony's syntax, `fs` first, so swap
  them back when writing the source: the disassembly's `vadda.xyz $ACC, $vf0, $vf2` is `vadda.xyz $ACC, $vf2, $vf0`.
- Don't run `tools/split_objects.py` or change `configure.py`/`tools/` in a decompilation task.
