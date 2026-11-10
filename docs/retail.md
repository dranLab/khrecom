# Retail ELF: Phase 0 results (beta → retail map, classification)

Phase 0 of `docs/retail-plan.md`, done 2026-11-10 (read-only: no build input changed). Tools: `tools/version_map.py`
(the map and the classification), `tools/helpers/retail_try.py` (compile one unit with other flags and compare
each function with its retail pair). Data: `build/SLUS_217.99_retail/version_map/` (`functions.tsv`, `data.tsv`,
`units.tsv`, `layout.tsv`; regenerated in ~15 s).

## Result in one paragraph

Retail is the **same compiler and the same source, built without `-g` and with the debug configuration off**.
Without `-g`, gcc 2.95 puts weak (inline) functions into `.gnu.linkonce.t.*` and vtables into `.gnu.linkonce.d.*`
sections. That explains retail's layout: all weak functions in one block at the end of `.text`, in the beta's order;
vtables in `.data` (663 of them), not `.rodata`. Compiled from our **unchanged** sources with the beta's flags minus
`-g`, **6,062 of the 6,064 functions that are identical in both builds (or differ only in nops) match retail byte for
byte (masked addresses)**, in the 497 complete units. Everything else that differs is explained by the debug configuration:
asserts and debug allocations compiled out, debug members removed from classes (layouts), debug functions emptied or
gone. Only **~225 functions (2 %) are unexplained so far**. **Go:** continue with Phase 1.

## The map (`tools/version_map.py`)

12,120 beta functions (11,133 game, 987 library):

| How paired | Functions |
|---|---|
| name in both DLL tables | 10,726 |
| anonymous-namespace / `_GLOBAL_$I$` name without the per-compile suffix | 17 |
| call graph (`jal` targets at the same positions of paired functions) | 477 |
| masked search between paired neighbours | 169 |
| the only unpaired code between paired neighbours (size checked) | 24 |
| **paired** | **11,413** |
| absent: no code between the paired neighbours in retail | 234 |
| unresolved (almost all debug code: DebugMenu, DebugWindow(TBL), DebugPathData, CollDisp, System/Debug, ...) | 473 |

- Order: 98.7 % of consecutive pairs keep the beta order. Exceptions are the weak block (see below), 10 units with
  1-4 inversions (MainApp 4), and a few same-named statics in another unit.
- Data: 5,593 beta data addresses paired (1,661 by name in both tables, the rest from `lui`/`addiu`/load/store
  addresses at the same positions of aligned code; 254 with conflicting evidence, to be checked in Phase 1).
- Function boundaries in retail that no name gives: after `jr $ra`/`j` + delay slot, followed by padding or a
  frame prologue (14 false boundaries inside the 7,451 identical functions: good enough for unpaired code only).

### Layout of retail `.text`

- 0x100000-0x2EA070: the game's own (strong) functions, units in beta order.
- 0x2EA070-0x31F254: the libraries (same order).
- 0x31F260-0x332858: **the weak block**: 2,194 of the beta's 2,285 weak functions (the kept copies), in beta order
  (2,193 of 2,197 consecutive pairs), unit by unit: each object's `.gnu.linkonce.t.*` sections after all `.text`.
  Libraries' weak functions (`_init`, `_fini`, `fopen`, `atexit`) stay in the library area.

Consequence for Phase 1: every unit with weak functions has **two `.text` ranges** in the retail split (its strong
code, its part of the weak block). Its vtables are in `.data`, not `.rodata`.

## Classification of the paired functions

Compared masked: `j`/`jal` targets, `lui` immediates and immediates on `lui`-loaded registers cleared.

| Class | Game | Lib | Meaning |
|---|---|---|---|
| same | 6,502 | 949 | masked-identical |
| layout | 2,028 | 12 | only immediates (member/vtable offsets, frame) and registers differ, no debug calls |
| nops | 124 | | only the number of nops differs (`div.s`/`sqrt.s` hazard padding: no `-g` line labels) |
| debug | 1,131 | | the beta calls AssertApp/com_malloc_d/TRACE/printf; same calls otherwise |
| header | 227 | | as debug, with an inlined header assert among them |
| inline | 178 | | calls missing in retail are debug code (callees that assert, now inlined, or gone) |
| stub | 37 | | retail body is `jr $ra; nop` (debug function emptied) |
| shape | 98 | | no debug calls, same calls, other instruction differences |
| calls | 127 | | call sequences differ otherwise |

The 12 library `layout` functions (`_malloc_r`, `_free_r`, `sceMcWrite`, `check_dll`'s `add_relmod`, ...) are
most likely address operands the mask misses (the libraries are the same objects). To check in Phase 1.

**Category (c) of the plan = `shape` + `calls` = 225 functions** (top units: Player 19, BattleEnemy 11, Entity 10,
SceneInfo 7, PlayerCamera 7, DispList 6, EffectMan 6, HumanBoss 6, Bullet 6). Many of them will turn out to be layout
or debug effects too (a changed `sizeof` changes multiplications, a removed member a store); the rest are real
retail changes, decompiled as `#ifdef` variants in Phase 3.

## Compile test: our source, beta flags without `-g`

`tools/helpers/retail_try.py <unit> "-O2 -G0 -fno-exceptions -fno-rtti"` on all 497 complete game units (no
INCLUDE_ASM; no source change, debug forms still in):

| Class | Match retail | Differ |
|---|---|---|
| same | 5,953 | 1 (`TsInitSif`: empty-loop padding) |
| nops | 109 | 1 (`KeVu0Sqrt`: VU0 asm nop) |
| shape | 1 | 78 |
| debug, header, inline, layout, stub, calls | 0 | 3,201 (expected: need the macros and layouts) |

So `-g` off reproduces retail wherever the source is the same. The two misses are line-label effects to look at
with the macros in place (the beta matched them through `-g`'s `$LM` labels).

## What the release configuration changed (for Phase 2)

- **Asserts** (`AssertApp`) compiled out: 1,358 functions (debug + header), plus 178 whose callees inline once their
  assert is gone.
- **Debug allocations**: `com_malloc_d(size, "file(line)", ra)` → `com_malloc(size)`, `com_memalign_d` →
  `com_memalign`: plain `operator new(size)` in retail (HeapMan.h already has it: `com_malloc`).
- **Class layouts** (`layout.tsv`, 259 classes): debug members removed.
  - `CTask::m_pszDebugInfo` (0x30) gone: the vptr moves 0x34 → 0x30 in every task class.
  - `CEntity` loses 0x20: candidates `m_tqDamageDispTimer`, `m_szNumberString[16]`, `m_pDamageString`
    (0x1B0-0x1C8), so `CAttachCard::m_pCardModel` 0x1D4 → 0x1B4 and every derived class shifts 0x20.
  - `PlayerCamera::CPlayerCamera` loses 0x140 around 0x180-0x2C0; `CPlayer`/`PL::CPlayer*` and others show their
    own shifts on top of CEntity's.
  - In the sources: `#ifndef KH_RETAIL` around those members (types from `ref/`, so the beta layout stays exact).
- **Debug functions** emptied (37 stubs) or gone (707 absent/unresolved, among them `_coord_alart`, `dbg*`, TRACE*,
  `CDbgCube`/`CDbgArrow`, the debug menus and windows, `CRobotPad`, `mspace_malloc_d`).
- **Units with no function in common** (25): debug units (System/Debug, DebugPathData, DebugWork, KisEnemy,
  CollDisp, DebugMenu, DebugWindow(TBL)), small data units (CardKind, CollParam), factory units (CreateEnemy,
  CreateGimmick, CreateBullet, BulletManager: their task tables/sizes changed with the layouts), CEntity-derived
  units where every function shifts (FriendAladdin, FriendBeast, FriendPan, SummonBambi, SummonDumbo), the libppp
  float variants (md0_f, md0_onef, md0_p3f, pppEiWfacc: to look at) and lib vprintf/vsprintf (not linked in retail).

## Next (Phase 1/2 adjustments to the plan)

- Retail build flags: the beta's minus `-g` (`build.yaml` per version). `tools/ee_asm_compat.py` must handle the
  `.gnu.linkonce.*` sections (weak functions and vtables) the compiler then emits.
- Retail split: strong and weak `.text` range per unit, vtables in `.data`; from `functions.tsv`/`data.tsv`.
- Phase 2 pilot needs the layout `#ifdef`s of `CTask`/`CEntity` as well as the assert/allocation macros: pick
  pilot units without CEntity classes first (System/*, Form/*), then one CEntity unit.
