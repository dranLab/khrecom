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
- **Matched code:** the user allowed changes to already-matched code for this plan (2026-11-10), including the
  S2/S5 edits (literals for `extern D_` strings, DLL imports undefined, `&g_Gimmick...`). Conditions stay: the
  SHA-1 holds, edits are line-neutral where `__LINE__` or line markers can matter, and each one says why.
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

**Done 2026-11-10 (66 -> 11), with `tools/shiftblocks.py`.** Finding: the `asm()` data blocks that units with
INCLUDE_ASM'd functions emit for those functions held **hidden pointers**: Camp's two jump tables
(`jtbl_004E3800`, `jtbl_004E3C50`: code addresses `.L00315FC8`, ... as `.byte` rows) and its string-pointer
tables (`pTechCateNameTbl.4441`, ...: addresses of strings in the same blocks). No pin showed them, because
nothing referenced them by name. `shiftblocks.py` finds every such block, locates its bytes in the original
image, compares the range with the original data listing, and with `--fix` defines the labels the build needs
by name and writes the pointers as `.word <label>` again. It keeps the bytes (the SHA-1 checks) and the source
line count (line markers and `__LINE__` stay put). Applied to Camp (29 pointers, 33 labels), DebugPathData (17
labels), SceneInfo (1), DispList (5). SceneInfo, BattleEnemy and CardRevolver used `extern "C" char D_xxx[]` for
strings the unit already emits: replaced by the literal, which gcc pools with the existing copy (`"pDstRoom"`;
`"@ENP"` as MapEnemy does; EnemyParam.h's assert strings, `"src/Game/Enemy/EnemyParam.h"` being that header's
`__FILE__` as the compiler sees it; the TRACE format of line 370). Line-neutral edits, SHA-1 unchanged.

**Left (11 pins, 4 hidden pointers), all from parked INCLUDE_ASM functions:** the asm'd code of Camp
(`setTechWin`, `setMapCardWin`), WorldEnv `initialize`, shape `pppVu1ShapeDraw`, DispList and graphdev
`sceGsSetDefDispEnv` uses constants the compiled C emits under gcc's anonymous `$LC` labels (`D_004E26D8`,
`D_00463700`, ...), or Camp's copy of the CardKind.h table under splat's name `cardDataTbl_004E5840`; Camp's
`pStrNum.4452` table points at such constants (`"%02d/99"`, `"%02d"`, ...). None can be named without changing the
compiled C's emission (Ps2EeAs has no aliases; splat's `.s` is generated). They go away when those functions are
matched (docs/plan.md): a shifted build needs either that or a non-matching C version of them in the shifted
build only. Decide when everything else is done (S5/S6 first).

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

### S3 progress (2026-11-10)

- **Data labels named in compiled objects** (`tools/name_data_labels.py`, `configure.py` `data_label_lines()`):
  every original address-named label that something mentions (asm, splat data, sources) and that lies in a
  compiled object's data from source becomes a global symbol of that object, at its offset in the section
  (`<obj>.labels`). The object's section is that range of the original, so the offset is exact; the matching
  link no longer needs `original_labels.ld` for them, and the SHA-1 proves each address. In a shifted link they
  move with their section. This resolved the S2 leftovers (constants the INCLUDE_ASM'd functions use), the S3
  data-to-data pins (`lconv`, `impure_data`, `cod/3FF680`) and the export table's 173 references (S4).
  Limitation: valid while the object's data equals the original's (every matching unit; checked by the SHA-1
  on every build). A non-matching port must give such data real names in C.
- `shiftblocks.py` now also points at labels of the unit's own data: Camp's last 4 raw pointers
  (`pStrNum.4452`) are `.word` labels; no hidden pointers left in source blocks.
- `func_0 = 0;` (zero words splat wrote as references), `_stack_size = 0x40000;` in `linker_script_extra.ld`;
  `symbol_addrs.txt` names `_end` (0x6A0EF8) and `_stack_size` (0x40000), so crt0 and `.sndata` use the names.
- `shiftpins.py` also reports absolute symbols at image addresses defined inside objects.
- **Result:** the shifted link has 1 undefined symbol (`D_00696E6F`) and 3 source pins (`D_004E5374` in Camp,
  `D_00504D20`/`D_00504D4D` in dlmalloc). All four are false references: string bytes in `.sndata` that splat
  read as pointers. They go with the generated table (S4/S5).
- Still open in S3: `src/crt0.s` with `_fbss` (crt0 clears `.bss` from the first variable,
  `_15CShadowModelMan$m_fADensity`: correct in every preset, because padding at the start of `.bss` needs no
  clearing, but not what Sony's crt0 says); `common.bss` alignment (below).
- **`common.bss` alignment, confirmed:** its section has alignment 8 and the FVECTOR/FMATRIX statics sit at
  offsets 0x88, 0x108, 0x4228 (8 mod 16): they are 16-aligned only because the section starts at 8 mod 16 in the
  original. A shift that moves the section start by 8 mod 16 misaligns them (VU0 `lqc2` faults; EE `lq` silently
  reads the rounded-down address). The presets only pad by multiples of 16; S6 must check every symbol's
  alignment anyway. The fix is `common.bss` from source (a matching task: the original linker's COMMON order).

### S4 - SN DLL exports

**Finding (2026-11-10):** not only the 173 `D_` references: most **name pointers** of the export table are raw
too (`.word 0x003CB8D5`: they point into the middle of the name strings, where splat has no label), and string
bytes inside the table were taken for references (`D_00696E6F` = `"oni"`, `D_004E5374`, `D_00504D20`; worse,
string bytes that equal a real symbol's address become relocated words). A shifted build would corrupt the names
the DLL loader looks up. The table must be generated: original bytes, with the pointer fields written
symbolically: name pointers as `sn_dll_header_root + offset`, values as `.word <the entry's own name>` (each
entry carries its symbol's name; `_gp`, `_end`, `end` by name; true absolutes like `_stack = -1` as numbers),
header pointers likewise. The matching SHA-1 checks it. The value fields then no longer need the S3 data labels.

The 173 `D_` references in the export table. For each: the address's real symbol in the matching ELF (`nm`;
compiled units name statics, vtables, strings their own way) goes into the table in place of the `D_` name. The
matching SHA-1 must stay. If S5's generator replaces the table, this happens inside it; otherwise as a list in
`symbol_addrs.txt`.

### S5 - SN DLL imports (the 125 relocations)

Goal: the import relocations come out of the link itself, like SN's linker did.

1. Find out what Ps2EeAs emits for a `jal` to a weak absolute-0 symbol: an `R_MIPS_26` against the symbol, or a
   resolved `jal 0` without one (`mips-linux-gnu-readelf -r` on `MainTaskFactory.o`). Same for the `HI16`/`LO16`
   imports of data (`_9C2DViewer$m_seqControl`).
   **Answer (2026-11-10): no relocation.** `MainTaskFactory.o` has no reloc against `__14CKishiwadaTestP5CTask`
   (nm: `00000000 W`): the assembler resolves the weak absolute completely. So step 3 needs step 4's change
   first: the imports undefined in the sources (no `.weak X\nX = 0` asm in the 11 files) and `X = 0;` for the
   109 imports in a linker script that both links use (an absolute 0, not an image address). The assembler then
   keeps `R_MIPS_26`/`HI16`/`LO16` at each site, the bytes stay `jal 0`, and the generator reads the sites from
   the objects' relocations: object + offset -> `.word <containing global function> + delta`, in the original
   table's order (each original offset checked against an import relocation of the object at that address).
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

### S4/S5 result (2026-11-10)

- **Imports undefined in the sources:** the 104 `.weak X\nX = 0` definitions (10 files) are comments now, and
  MapUnitTable's five NULLs at the end of `mapUnitTable_10` are `&g_Gimmick1060`, `&g_Gimmick1077`,
  `&g_Gimmick1090`, `&g_Gimmick1091`, `&g_Gimmick1092` (the table's R_MIPS_32 imports at 0x39BEF8..0x39BF08).
  `config/SLUS_217.99/dll_imports.ld` sets the 109 imports to 0 for both links. Every use keeps its relocation.
  Line-neutral.
- **`tools/sndll.py`** generates `.sndata` (configure finds the object by `dlabel sn_dll_header_root` and builds
  it from `build/SLUS_217.99/sndll/sndata.s`, after all other objects; the manifest
  `build/SLUS_217.99/sndll/objects.txt` gives each object section's original start). 26,344 symbolic words: header
  pointers and name pointers as `sn_dll_header_root + off`, values as the entry's own symbol (or the object's
  symbol at that address, or the nearest global + offset), `_gp`/`_end`/`end`/`_stack_size` by name, VU
  micro-memory values (`VU0_cbbox = 0x20`) and absolutes as numbers. Each of the 125 import sites is checked
  against a relocation of the same type against a `dll_imports.ld` symbol in the object at that address, and
  written as `<global before it> + offset`. Matching SHA-1 unchanged.
- **VU microcode false references:** `pppVu1ShapeProg` also `can_reference:False` (D_004039DC).
- The source pins for `.sndata`'s false references (Camp `D_004E5374`, dlmalloc `D_00504D20`/`D_00504D4D`)
  are gone.
- **`shiftpins.py`: 0 undefined, 0 absolute; `ninja shift` links all five presets.** text128: `main`,
  `sn_dll_header_root`, `_gp`, `_end` all +0x80, `_start` (crt0, before the padding) unchanged.
- Follow-up (decomp accuracy, not needed here): the table has the original names of the imports; several source
  names are guesses (`__16CEnemyOverlay105P5CTask` where the table says `__10CBossAnsemP5CTask`, ...). The site
  -> name pairs come from sndll.py's reloc check.
- The fallback `<global before it> + offset` for import sites holds while those functions' code is the original's
  (true for every matching unit). The relocations themselves are in the objects, so a later version can emit the
  sites from them directly.

### S6 - shift check (tools)

`tools/shiftcheck.py`, run after `ninja shift`: compares the matching ELF and the shifted ELF (both with
relocations: also give the matching link `--emit-relocs`; it doesn't change the image, and the SHA-1 says
whether that holds). The two ELFs are paired by symbol: every symbol of the matching image has a counterpart.

- Every relocated word (`R_MIPS_32`, `26`, `HI16`/`LO16`, `GPREL16`, the DVP types) must equal the matching
  value moved by the target's displacement.
- Every non-relocated word must be identical in both images (paired by containing symbol + offset).
- Lesson from S2: hidden pointers often target addresses that have **no symbol** (`.L` code labels inside a
  function, a string in the middle of a block). Compare against every address of the matching image's moved
  sections, not only against symbol addresses.
- **Suspects:** a non-relocated word whose value is an image address that moved: exact, or masked as a DMA tag
  (`& 0x7FFFFFF0`), a `jal` target, or a `lui`/`addiu`/`lw` pair. Reported with object, symbol and offset.
  Each suspect gets fixed (made symbolic) or goes into `config/SLUS_217.99/shift_allow.txt`, one line per word
  with its reason ("string bytes `\" %d\"`"). No ranges, no whole files.
- Also reported: symbols of the matching image missing from the shifted one, size differences, and section
  alignments that changed.
- Done when: 0 unexplained suspects for each shift configuration of S7.

**Done 2026-11-10 (`tools/shiftcheck.py`).** Pairs every input section of the matching and the shifted link
through both map files and checks 1,047,683 words per preset (.text 673,748, .data 205,422, .rodata 168,507). It
follows the plan with these refinements, each for a reason found on the way:
- in `.text` only unrelocated `j`/`jal` count as suspects: instruction words often look like addresses
  (`0x00431024` is `and $v0,$v0,$v1`), and gcc puts no data in code;
- one-past-the-end addresses (`_end`) move with the section that ends there; linker-script symbols (`_gp =
  cod_SDATA_START + 0x7FF0`) are compared by their value in each ELF;
- the generated `.sndata` is not a suspect source (every pointer field symbolic by construction);
- unrelocated words of **compiled** objects in the image range are counted, not failed: a pointer from C always
  carries a relocation, so they are constants unless a source writes a numeric address (~800 per preset, mostly
  pairs of 16-bit values such as CardBattle's tables). S8 is the judge for those.
- alignment is checked for data symbols only (code needs 4).

Result for all five presets: **0 errors, 0 lost alignment, 0 unexplained suspects**; 92 words (union of the
presets) cleared one by one in `config/SLUS_217.99/shift_allow.txt`: 56 VU microcode words (inside the
microprograms, see S3), 16 PATK_DATA byte fields (`g_AttackDataList`, types.h), 17 string tails
(`pppKeThRes128x8`, `.../GPrim.h(684)`, ...), 2 DWARF CIE bytes (`__FRAME_BEGIN__`), 1 `lib/table` 16-bit pair.
Informational: 257 `R_MIPS_32` words of `impure_data` point into the middle of data, all into `impure_data`
itself (newlib's `_reent` pointing at its own `__sf[]` streams).

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
| S2 compiled-code pins (66 after the Map merge) | done except 11 (INCLUDE_ASM-dependent) | 244 -> 189 undefined in the shifted link; 29 hidden pointers restored as `.word`, 4 left (Camp `pStrNum.4452` -> unnamed C constants); `shiftblocks.py` total: 0 restorable left. check.sh exit 0. |
| S3 asm-data pins, auto symbols, `_end`, crt0, `common.bss` alignment | mostly done 2026-11-10 | shifted link: 189 -> 1 undefined (+3 source pins), all `.sndata` false references (S4/S5). Data labels named in compiled objects (`name_data_labels.py`), `func_0`, `_end`, `_stack_size`. Open: `src/crt0.s` (`_fbss`), `common.bss` alignment (from source). check.sh exit 0. |
| S4 DLL exports (173) | done 2026-11-10 | `.sndata` generated by `tools/sndll.py` (26,344 symbolic words); SHA-1 unchanged |
| S5 DLL imports (125 relocs, 120 raw) | done 2026-11-10 | imports undefined in sources + `dll_imports.ld`; all 125 sites checked against object relocations. `shiftpins.py` 0/0; all 5 presets link. check.sh exit 0 |
| S6 shiftcheck | done 2026-11-10 | all 5 presets: 0 errors, 0 suspects (92 cleared individually), 0 lost alignment |
| S7 presets | done 2026-11-10 | text128, data128, all8, mid, big link and pass shiftcheck. `--no-check-sections` still on (untested without). |
| S8 boot tests | prepared, **not run** (no PCSX2 in this environment) | `ninja shift-iso-<preset>` builds the disc (`build_iso.py --shifted`: image comparison skipped and said so); text128's disc built 2026-11-10 (`build/SLUS_217.99/shift/text128/SLUS_217.99.iso`, ELF 0x60CF38 bytes at sector 631, entry 0x100008). Runs and the DLL list from the original are the user's. |
| S9 check.sh/CLAUDE.md | done 2026-11-10 | `tools/check.sh --shift` (all presets: shiftpins + shiftcheck), exit 0; CLAUDE.md section "Shifted build" on this branch. CI unchanged (branch unmerged). |

Boot-test log:

| date | preset | build (commit) | reached | problems |
|---|---|---|---|---|
