# Plan: port step 1 - a shiftable PS2 build

First step towards a port: the decompiled game must keep working when its code and data move to other
addresses. Still built for the PS2 with the original toolchain (ee-gcc 2.95.3-136, Ps2EeAs, the library
toolchains of `build.yaml`), still booted in PCSX2. Only the link changes: no label may be pinned to its
original address, and padding is inserted so that everything moves.

A build that passes this proves that no code or data depends on where things are. Every later port step (a
modern compiler, a host build against a stub SDK) relies on that and would otherwise hit address bugs that are
very hard to trace.

## Rules for this plan

- **Branch:** all work happens on `port/shiftable` (worktree `.claude/worktrees/port-shiftable`). Never merge it
  into `main` automatically, not even when it is done: the user reviews and merges. Merging `main` *into*
  `port/shiftable` to stay current is allowed. `decomp-pool` / `decomp-worker` do not take items from this plan.
- **Tools:** this plan may change `configure.py` and `tools/` (this overrides CLAUDE.md's "Don't change
  `configure.py`/`tools/` in a decompilation task" for this plan only). It may also add names to
  `config/SLUS_217.99/symbol_addrs.txt`. It still never edits the generated `SLUS_217.99.yaml`, never runs
  `tools/split_objects.py`, and never commits game data.
- **The matching build stays matching:** `tools/check.sh` exits 0 on every commit. Plain `ninja` keeps building
  and checking the original image with the original SHA-1. Shifted builds are separate, opt-in targets.
- **No faked success.** None of these are allowed, in either build:
  - changing the expected SHA-1, the `sha1` rule or what it hashes; skipping or weakening the checksum of the
    matching build;
  - in the shifted link: `PROVIDE`, `--defsym`, absolute assignments or linker-script lines that put a named
    image object (code or data) at a fixed address. Allowed absolutes, each with a comment in the source: hardware
    addresses (0x1000xxxx, 0x1200xxxx, scratchpad 0x70000000), the load address 0x100000, and the
    weak-absolute-0 DLL imports that already exist (they are genuinely unresolved in the original too);
  - patching the linked ELF or image after the link to make it boot or pass a check. Exception: build_iso.py's
    existing `p_paddr = p_vaddr` fix-up of the boot copy. Generated tables (the SN DLL table, see S5) must be
    link *inputs*, built from the link's own relocations;
  - new `.incbin`s or byte blobs copied from the original image in order to fill places the shifted link got
    wrong. Data that is splat assembly today may stay assembly, if every pointer in it is symbolic;
  - dropping objects from the shifted link, `--unresolved-symbols=ignore-*`, `--noinhibit-exec`, or anything
    else that turns a link error into a warning;
  - blanket allow-lists in the checking tool (address ranges, whole files). Every allowed suspect is listed
    on its own, with the reason;
  - boot results that were not observed. If a run was not done, write "not run". If something stays broken,
    it stays in the status table as broken.
- **Commits:** small, one item at a time, with the evidence in the message (the check that proves it).
  Attribution as usual.

## Findings (2026-11-10, main at b6ae2342)

### How the image is linked now

- `configure.py` links splat's script `build/SLUS_217.99/SLUS_217.99.ld`. Only `.cod 0x100000` is fixed there,
  everything else is laid out in sequence. `align_section_starts()` adds `ALIGN(., 128)` before each data
  section start label, and `_gp = cod_SDATA_START + 0x7FF0`. All of that moves along with the layout.
- Extra scripts on the `ld` line:
  1. `config/SLUS_217.99/linker_script_extra.ld`: `ENTRY(_start)` and **`_end = 0x006A0EF8;`**, a hard-coded end
     of `.bss`. `main.cpp`'s static initializer computes the heap start from it, so after any shift the heap
     would start at the wrong place.
  2. `undefined_funcs_auto.txt` (14) and `undefined_syms_auto.txt` (23), written by splat. They mix constants
     that splat read as addresses (`D_FFFF`, `D_80000000`, `D_BC000000`, ...; harmless as constants) with real
     image addresses (`func_0010012C`, `D_0035F3D0`, `D_6A0EF8`, ...). 3 of them are used by the link.
  3. `build/SLUS_217.99/original_labels.ld` (`configure.py` `write_original_labels()`): **16,621 `PROVIDE`s**,
     every address-named label in the asm and the sources, at its original address.
- `--no-check-sections` is set on the `ld` rule.
- The checksum covers `objcopy -O binary` of the ELF (the loaded image), not the ELF file.
- `ee_asm_compat.py`'s address-based decisions (dropping foreign weak data, renaming function-local statics)
  read the symbol file and the split ranges, not the link layout, so a shifted link doesn't affect them.

### Which pins the link really needs

`tools/shiftpins.py` (S1) lists them; `ninja shift-<preset>` fails on exactly the same symbols. After merging main
on 2026-11-10 (Map finished): **244** undefined symbols (241 from `original_labels.ld`, 2 from
`undefined_syms_auto.txt`, 1 from `undefined_funcs_auto.txt`). The other ~16,380 `PROVIDE`s are unused. Users
(`shiftpins.py --by-object`):

| user | pins |
|---|---:|
| `cod/2C2B00` (`.sndata`, the SN DLL table, see below) | 173 |
| `Form/Menu/Camp` | 31 |
| `Game/Debug/DebugPathData` | 17 |
| `System/Graphic/DispList` | 6 |
| `Game/Enemy/BattleEnemy`, `System/Graphic/WorldEnv` | 3 each |
| `lib/nodebug/impure_data` (.data) | 3 |
| `Game/SceneInfo`, `libppp/draw/shape`, `crt0`, `lib/nodebug/lconv` (.rodata) | 2 each |
| `Game/CardRevolver`, `lib/graphdev_checkModelVersion`, `cod/3FF680` (.sdata), and the data-only objects `PlayerAnim` (.data), `PlayerAttack` (.data, .rodata), `libppp/draw/md1`, `libppp/pppfunctbl`, `lib/table`, `lib/nodebug/{__CTOR_LIST__, SN_LOADED_RELMODS, _GLOBAL__F_0build.cppF7aaaa, __FRAME_BEGIN__0, sn___infinity}`, `libm/{s_infconst, s_lib_ver}` | 1 each |

Some of these are `func_0` (zero words splat wrote as a relocation against `func_0`); they need a plain `0`, not
a name.

### Alignment the original layout hides

Every link warns: `alignment 8 of symbol _15CShadowModelMan$m_vParam in .../data/common.bss.o is smaller than 16
in .../ShadowModel.o` (also `m_vCamPos`, `m_mWorldTexture`, `m_mWorldScreen`, `m_mProjectionTexture`,
`m_mProjectionScreen`, `_9CSceneEnv$m_vOutColor`). `common.bss` is splat assembly, and it declares these FVECTOR /
FMATRIX statics with 8-byte alignment. In the original layout they land 16-aligned anyway. In a shifted `.bss`
they may not, and VU0 `lqc2`/`sqc2` on them would raise an address error. See S3.

### The SN DLL table (`.sndata`, `cod/2C2B00`, 0x3C2B00, 622,656 bytes)

- `sn_dll_header_root`, magic `SNR2`: the main program's symbol table for SN Systems' runtime DLL loader
  (`src/lib/nodebug/check_dll.c`). The game loads DLLs: the disc holds about 14 `SNR2` modules, starting on
  sector boundaries in sectors the ISO directory does not list. Users in the source: `MainTaskFactory` (debug and
  viewer tasks), `CreateEnemy`/`CreateGimmick` (enemy and gimmick classes "not in the main executable"),
  `MinigameManager`, `TitleReportDllStarter`, `VSME`, `Model`, `LightUnit`, `CollUtil`, `2DSprObj`, `Map`.
- **Exports:** 13,167 symbols. 13,161 values are symbolic in splat's asm, 6 are absolutes outside the image.
  `lookup_symbol()` resolves a DLL's imports by name + hash, so exports survive a shift **if** every value is a
  symbol. 173 of them name `D_` labels that only `original_labels.ld` defines (things a compiled unit names its
  own way now: statics, vtables, string labels).
- **Imports (the blocker):** the main program imports 109 symbols from DLLs. In the source they are weak
  absolutes at 0 (`.weak X\nX = 0`, 11 files), so every use assembles to `jal 0` or a `lui`/`addiu` pair of 0.
  The table's 125 relocations say where to patch, and **120 of them hold raw `.text` addresses**. Examples:
  `0x0012C398` type 4 (`R_MIPS_26`) `setActiveLight__10CMapVieweriPvi`; `0x00161F78`/`0x00161F7C` types 5/6
  (`HI16`/`LO16`) `_9C2DViewer$m_seqControl`; `0x001C687C` type 4 `__14CKishiwadaTestP5CTask`. After a shift
  the loader would patch whatever instruction now sits at those addresses: silent corruption as soon as a DLL
  is loaded.

### Pointers hidden as numbers

- INCLUDE_ASM code: no `lui` with a numeric immediate inside the image. Branches and jumps there use local
  labels.
- Asm data: 34 words equal the address of a known symbol, and 38 more do after masking DMA-tag bits
  (`& 0x7FFFFFF0`). Almost all are string bytes (`" %d\0"` = `0x00642520`). Not yet explained:
  `lib/nodebug/__CTOR_LIST__` (29,812 bytes: more than a constructor list), `__FRAME_BEGIN__0`,
  `_GLOBAL__F_0build.cppF7aaaa`, `libgcc2___default_terminate` (EH data holds absolute code addresses).
  Heuristic scans cannot prove anything: S6 builds a real check.
- `.DVP.overlay*` sections (17, VU microcode) are not in the loaded segment. The microcode the game uploads
  comes from `.data` (`Model.cpp`'s `vu1mpg_start`/`vu1mpg2_start`, symbolic). DMA chains with addresses inside
  tags are what S6's masked check is for.

### Not blockers

- The 10 game functions still INCLUDE_ASM and the hand-written `.s` units are relocatable as long as their
  labels are symbolic. They stay in docs/plan.md's matching work; a later port step needs them as C.
- Hardware addresses in the source (`0x10000000` 115 times, `0x12000000`, `0x70000000`) are not image
  addresses.
- The disc: build_iso.py writes the new ELF into the original file's sectors (the original is 50 MB, 45 MB of
  it `.mdebug`; ours is far smaller), and every other sector, the hidden game data and DLLs included, stays
  where it is.

## Work items, in this order

### S1 - shifted link target and pin report (tools)

- `configure.py` writes, besides the matching build, a second link: `ninja shift` (not a default target). It
  reuses **the same objects** as the matching build and differs only in the link:
  - splat's linker script with padding objects inserted, configured in `config/SLUS_217.99/shift.yaml` (new):
    per output section (`.text`, `.data`, `.rodata`, `.sdata`, `.sbss`, `.bss`) an amount and a position
    (section start, or after a named unit). Padding objects are assembled `.space` (with `nop`s in `.text`);
  - no `original_labels.ld`, no `undefined_*_auto.txt`;
  - `--emit-relocs` (for S6) and the same other flags. Try to drop `--no-check-sections` here; if the link
    needs it, write down why;
  - outputs under `build/SLUS_217.99/shift/` (`SLUS_217.99.elf`, `.map`, `.rom`), no sha1 step.
- `tools/shiftpins.py` (from the session's scratch script): every undefined symbol of the link objects that only
  the pin files define, per object and kind. That list is the to-do list for S2-S4; its total must reach 0.
- Done when: `ninja shift` runs, and fails only on the undefined symbols that `shiftpins.py` lists (ld's
  error list and the tool agree). `ninja` and `tools/check.sh` are unchanged.

### S2 - pins in compiled code (66 references, 9 objects; 87 in 10 before Map was finished)

Camp, DebugPathData, DispList, BattleEnemy, WorldEnv, SceneInfo, shape, CardRevolver,
graphdev_checkModelVersion. Mostly strings and tables that an INCLUDE_ASM'd function or a `.rdata` owner block
names as `extern D_xxx`. For each one, *define* the label where its data is emitted: a plain label in the
existing `__asm__(".rdata ...")` block (plain labels are fine for Ps2EeAs, only aliases crash it), or the C
object's real name in place of the `D_` name. The matching build's SHA-1 proves the address is still the same;
the shifted link proves it is symbolic. These edits touch matched units, so CLAUDE.md's usual rules apply
(funcdiff, check.sh).

### S3 - pins in asm data (non-DLL), splat auto symbols, `_end`

- The 1-3-reference users (lib/nodebug, crt0, libm, the data-only units `PlayerAnim`, `PlayerAttack`, `md1`,
  `pppfunctbl`, `lib/table`): each references a label nobody defines by name (a string + addend, a constant
  splat took for an address). Fix it by naming the target in `symbol_addrs.txt` (after a fresh configure, both
  sides use the name), by writing the data-only unit from source (its own matching task: `docs/plan.md` rules),
  or, for a constant taken for an address, by making it a constant again. Check each one by hand.
- `undefined_*_auto.txt`: the 3 used entries the same way. The constants among them are fine as constants,
  but in the shifted link nothing may come from these files.
- `_end`: **done in S1.** `linker_script_extra.ld` has `_end = cod_BSS_END;`, and
  `linker_script_matching.ld` (matching link only) has `ASSERT(_end == 0x006A0EF8, ...)`.
- `crt0`: splat's crt0 uses splat names for what Sony's crt0.s calls `_fbss`/`_end`/`_stack_size`/`_args`:
  the `.bss` clear runs from `_15CShadowModelMan$m_fADensity` (the first `.bss` variable) to `D_6A0EF8`
  (`_end`); `D_40000` (a constant, the stack size) and `D_004FF800` come from the auto file. Write
  `src/crt0.s` by hand (the build supports a hand-written `.s` in place of splat's, like `src/lib/klib.s`), with
  the real symbols: `_fbss` = start of `.sbss`/`.bss` from the linker script, `_end`, the constants as
  constants. The matching SHA-1 proves the bytes.
- `common.bss` alignment: give the 16-byte statics their 16-byte alignment. That means `common.bss` from source
  (the owning units' statics defined in C, `ld_bss_contains_common`), or, until then, whatever keeps the
  original bytes and states the alignment. The warnings must be gone, in every preset too.

### S4 - SN DLL exports

The 173 `D_` references in the export table. For each: the address's real symbol in the matching ELF (`nm`;
compiled units name statics, vtables, strings their own way) goes into the table in place of the `D_` name. The
matching SHA-1 must stay. If S5's generator replaces the table, this happens inside it; otherwise as a list in
`symbol_addrs.txt`.

### S5 - SN DLL imports (the 125 relocations)

Goal: the import relocations come out of the link itself, like SN's linker did.

1. Find out what Ps2EeAs emits for a `jal` to a weak absolute-0 symbol: an `R_MIPS_26` against the symbol, or a
   resolved `jal 0` without one (`mips-linux-gnu-readelf -r` on `MainTaskFactory.o`). Same for the `HI16`/`LO16`
   imports of data (`_9C2DViewer$m_seqControl`).
2. Read the order of the original's 125 relocs (by address? by symbol?) and the format of the root header
   fields around them (`relocs`, `nrelocs`, `packed_relocs`).
3. With relocations available: `tools/sndll.py` generates the `.sndata` object from the original's symbol list
   (names, types, hash, order: data, like any splat data) plus the link's relocations against the import
   symbols. Two passes: link once with a table of the right size, read the relocs (`--emit-relocs`), generate the
   table, link again. The second link must leave every address unchanged (the tool checks it). The table becomes
   a link input that replaces splat's `cod/2C2B00` object; configure.py changes accordingly.
   - In the matching build the generated table must reproduce the original bytes: the SHA-1 is the proof.
4. Without relocations (Ps2EeAs resolves them): emit each import site under a label the generator can find,
   e.g. a wrapper the assembler cannot resolve (a real undefined weak via `ee_asm_compat.py`, or a per-site
   label), as long as the matching build's bytes stay the same. Write down the mechanism in CLAUDE.md.
5. Interim, only if 3 and 4 both stall: `.word <function> + <offset>` entries (the function containing the
   site). That survives pure shifts but not code changes; S6 must then also check that each site holds the
   expected instruction (`jal 0`, `lui $x, 0`, `addiu $x, $x, 0`). Mark it in the status table as interim.

### S6 - shift check (tools)

`tools/shiftcheck.py`, run after `ninja shift`: compares the matching ELF and the shifted ELF (both with
relocations: also give the matching link `--emit-relocs`; it doesn't change the image, and the SHA-1 says
whether that holds). The two ELFs are paired by symbol: every symbol of the matching image has a counterpart.

- Every relocated word (`R_MIPS_32`, `26`, `HI16`/`LO16`, `GPREL16`, the DVP types) must equal the matching
  value moved by the target's displacement.
- Every non-relocated word must be identical in both images (paired by containing symbol + offset).
- **Suspects:** a non-relocated word whose value is an image address that moved: exact, or masked as a DMA tag
  (`& 0x7FFFFFF0`), a `jal` target, or a `lui`/`addiu`/`lw` pair. Reported with object, symbol and offset.
  Each suspect gets fixed (made symbolic) or goes into `config/SLUS_217.99/shift_allow.txt`, one line per word
  with its reason ("string bytes `\" %d\"`"). No ranges, no whole files.
- Also reported: symbols of the matching image missing from the shifted one, size differences, and section
  alignments that changed.
- Done when: 0 unexplained suspects for each shift configuration of S7.

### S7 - shift configurations

`shift.yaml` presets, each linked and checked with S6:

| preset | what moves | catches |
|---|---|---|
| `text128` | `.text` +0x80 after crt0 | code addresses, jump tables, function pointers in data |
| `data128` | every data section +0x80 | data pointers, DMA chains, `_gp`-relative data, `_end` |
| `all8` | `.text` +0x8, each data section +0x10 | alignment assumptions (qword DMA data, 64-byte packets) |
| `mid` | padding after `Game/Map` (`.text`) and after `Game/Player/Player` (`.data`) | distances between parts of the image (pointer arithmetic across units) |
| `big` | `.text` +0x10000, data +0x10000 | 16-bit offsets, `%hi` carries, memory-size limits (heap, `_end`) |

`big` also grows the image: check the heap and stack layout against the 32 MB of RAM (`main.cpp`, HeapMan).

### S8 - boot test

- `ninja iso` for the shifted ELF: build_iso.py takes `--elf build/SLUS_217.99/shift/SLUS_217.99.elf`. Its
  image comparison with the original must then say clearly that it was skipped ("shifted build: image not
  compared"); never skip it silently. Add a ninja target `shift-iso`.
- First, with the **original** game in PCSX2's debugger: find which DLLs get loaded and when (breakpoint on the
  loader, `snDllLoaded` in `src/lib/nodebug/check_dll.c`; log module name and RCF ID). Record that list here: it decides which scenes
  the test has to reach.
- Protocol per preset (PCSX2 version and settings recorded once; same input sequence for the original and the
  shifted ISO): boot, title, new game, the first room and battle, a field room with gimmicks and enemies, a
  scene that loads one of the DLLs, a movie, the camp menu, memory-card save and load (save from the original,
  load in the shifted build and the reverse), at least 30 minutes of play. Screenshots at fixed points; compare
  against the original.
- Every run goes into the status table with date, preset, what was reached and what went wrong.

### S9 - keeping it

- `tools/check.sh` gets an optional `--shift` that also runs `ninja shift` + `shiftcheck.py` for the presets
  (no boot). Not the default as long as this branch is unmerged; once the user merges, decide whether CI runs
  it.
- CLAUDE.md, on this branch: the shifted build, the new tools, and the rule that new `extern D_` references
  must have a definition by name.

## Done criteria

1. `tools/check.sh` exit 0; plain `ninja` unchanged (same SHA-1, same checks).
2. `ninja shift` links every S7 preset without `original_labels.ld` and without `undefined_*_auto.txt`;
   `shiftpins.py` reports 0.
3. `shiftcheck.py`: 0 unexplained suspects for every preset; `shift_allow.txt` reviewed by the user.
4. The SN DLL import table comes from the link (S5 step 3 or 4), or the interim form is marked as such.
5. Boot tests per S8 for `text128`, `data128` and `mid`, with a DLL load, recorded below.
6. The user has reviewed the branch. Merging stays the user's decision.

## Status

| item | state | evidence / notes |
|---|---|---|
| S1 shift target + pin report | done 2026-11-10 | `ninja shift` links all 5 presets of `config/SLUS_217.99/shift.yaml` from the matching objects; each fails only on undefined symbols, and ld's 244 equal `shiftpins.py`'s 244. `assert_no_pins()` rejects `PROVIDE` and image-address assignments (tested on samples). `--no-check-sections` still on: revisit once a preset links. |
| S2 compiled-code pins (66 after the Map merge) | open | |
| S3 asm-data pins, auto symbols, `_end`, crt0, `common.bss` alignment | `_end` done (S1) | |
| S4 DLL exports (173) | open | |
| S5 DLL imports (125 relocs, 120 raw) | open | |
| S6 shiftcheck | open | |
| S7 presets | open | |
| S8 boot tests | open | |
| S9 check.sh/CLAUDE.md | open | |

Boot-test log:

| date | preset | build (commit) | reached | problems |
|---|---|---|---|---|
