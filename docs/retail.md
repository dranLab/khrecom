# Retail ELF: Phase 0 results (beta → retail map, classification)

Phase 0 of `docs/retail-plan.md`, done 2026-11-10 (read-only: no build input changed). Tools: `tools/version_map.py`
(the map and the classification), `tools/retail_units.py` (compile units as retail does and compare
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

`retail_try.py <unit> "-O2 -G0 -fno-exceptions -fno-rtti"` (now `tools/retail_units.py try`) on all 497 complete game units (no
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

## Phase 1 (2026-11-10): the retail build from assembly

`ninja` builds and checks both images; the retail one is built entirely from splat's assembly of the retail ELF
and matches (its `.rom` SHA-1 1c255c21…, the load image of the ELF 865450e0…). The split (`tools/split_version.py`):
655 units with code, 134 `.linkonce` code parts, 448 `.data`, 255 `.rodata`, 93 `.bss` units; the map gained a
fifth pairing pass (a unit's functions at one common offset: mpc's jump-table-only `_rix_*`, +32 functions).
How and what changed in the tools: `docs/retail-plan.md`, Phase 1.


## Phase 2 (2026-11-10): the pilot - go

With the version switches in headers only (include/khversion.h, below), the 497 complete units compiled with
`-DKH_RETAIL` match retail for ~94 % of their functions (tools/retail_units.py try): 820/925 `debug`,
1355/1439 `layout`, 112/194 `header`, 5679/5680 `same`. Pilot units Buf 14/14, TexCtrl 13/13, LevelUp 11/11 (and
its data: one retail text change, "MAX HP" -> "Max HP"), WorldWarp 8/9, Fsm 12/15, CoordMan 13/15, FifoFpl 5/8.
The first three now build from source in the retail image (the checksum holds).

What the release configuration needs in the sources (all line-neutral: `__LINE__` is in the beta's strings):

- `AssertApp`, `TRACE*`: macros that drop the call with its arguments (no strings); `com_*_d` -> `com_*`.
  Declarations name them `(AssertApp)(...)`. An assert's condition then remains as an empty `if`: usually harmless,
  but it can change gcc's block layout (FifoFpl isValidActiveBlockID): such an assert must vanish completely,
  `KH_DEBUG(if (!(e)) AssertApp(...);)` or a unit-local `ASSERT` defined through `KH_DEBUG`.
- `new (__FILE__ "(N)") T` -> `new KH_AT("(N)") T` (1,051 sites, mechanical), `operator new(n, __FILE__ "(N)")` ->
  `operator new(n KH_ATARG("(N)"))` (17): the literal must not exist in retail (gcc emits it even when the inline
  operator ignores it). Retail calls `operator new(size_t)` -> `com_malloc`; retail `new[]` likewise.
- Debug-only members: `KH_DEBUG(member;)` (CTask::m_pszDebugInfo; CEntity's damage display, 0x1b0-0x1c8 -> the
  class is 0x1b0, every derived class shifts 0x20), their uses `KH_DEBUG(statement)` per line.
- Text the retail version changed: `KH_VER(beta, retail)`.
- `DebugWork::g_dw` (the debug switches, 1,776 uses in 44 files): retail has no DebugWork; every function that reads
  it differs or is gone (98 absent, 68 of the 225 `shape`/`calls`). Open (task: Phase 3).

## Decisions (unattended run 2026-11-10/11)

Taken without the user (who asked for exactly that); revisit as needed.

1. Version macros live in include/khversion.h, included by khtypes.h (khtypes.h has no `__LINE__` use, so the extra
   line there is harmless). Retail gets `-DKH_RETAIL` from its build.yaml cflags.
2. Asserts stay as written in the sources; only where the leftover empty `if` changes the code is a site wrapped in
   `KH_DEBUG(...)`. A mass rewrite of the 595 sites was not done (it buys little: ~90 % already match).
3. `// SOURCE DATA:` is the reference version's; another version takes data from source only with its own marker,
   `// SOURCE DATA SLUS_217.99_retail: ...`, appended after it (end of file, line-neutral).
4. A unit compiled for retail always gives its .gnu.linkonce code and vtables from source (configure.py replaces its
   `<unit>.linkonce` assembly objects); linkonce copies the original kept in another object are removed
   (ee_asm_compat.py --linkonce).
5. Exact data starts of compiled retail units come from their relocations (retail_units.py switch ->
   config/SLUS_217.99_retail/unit_starts.txt, applied by split_version.py); a gap after a unit goes to the next one.
6. Global data definitions outside a compiled object's data from source are removed (ee_asm_compat.py
   drop_foreign_globals, objects only), like the existing weak-data rule: the assembly's copy defines them.

## Phase 3 (from 2026-11-11): units from source in the retail image

`tools/retail_units.py` (scan / switch / verify; see its docstring) brings complete units over. First batch: 292
eligible of 505 complete units, 282 build from source after `verify` (10 switched back: 2DSeqCtrl, PathCamera,
FriendDonald, Fnt, MonstroEventBattle, SummonGenie, GimmickHitReturn, GimmickPlantBullet, MonstroGauge, Pause - to
look at). State: config/SLUS_217.99_retail/sources.txt, unit_starts.txt; the sources carry
`// SOURCE DATA SLUS_217.99_retail: ...` where their data comes from source.

Not eligible (scan of 505): ~90 with code sizes that differ from the split's ranges (the split's unit starts are
heuristics; or retail has functions we don't), 58 with differing functions, ~20 linkonce sizes, 12 `.rodata` bytes
(text changes like LevelUp's), debug inline functions retail lacks (e.g. CComponentObj::dbgButtonTrg).

7. (decision) `verify` blames, per output section, the compiled object placed last before the first difference,
   data sections first; it switches those back and repeats. It may throw out an innocent neighbour; `scan` +
   `switch` restore a unit once fixed.

Second batch (2026-11-11): one-line asserts `if (...) AssertApp(...);` wrapped in `KH_DEBUG(...)` (136 in headers,
355 in sources; +83 functions), file-local statics and `_GLOBAL_$I$` names paired correctly in `scan`, one
anonymous-namespace suffix per file in the retail symbol file. 328 eligible, **318 build from source** after `verify`
(MonstroEventBattle, PathCamera, 2DSeqCtrl, FriendAriel, BaseGimmick, GimmickGuillotine, Pause, SummonGenie,
MonstroGauge, MoogleShop switched back: to look at).

8. (decision) One-line assert statements are wrapped `KH_DEBUG(...)` everywhere (supersedes decision 2's "only where
   needed": it is closer to the original's macro and costs nothing in the beta).
9. (decision) `tools/commons.py --allow-missing` keeps splat's extent of a COMMON piece beyond its declared size as
   padding (retail has unnamed COMMON data after its last named one).
10. (decision) `verify` blames from the linker map: the first input section, by address, not at its split start
    (also when the link's own checks fail), and the compiled object placed last before it.

Third batch (2026-11-11): `scan` covers the libraries too (each unit with its build.yaml toolchain; units still
with INCLUDE_ASM are left out). **446 units build from source in retail** (the libraries largely complete);
switched back by `verify`: Pause, SummonGenie, MonstroGauge, MoogleShop, lib/nodebug/Tim2CheckFileHeader.

Fourth batch (2026-11-11): **451 units from source in retail** (of 648 complete units; 456 eligible, `verify`
switched back _mbtowc_r, tlbfunc, libgcc2___default_terminate, SummonGenie, Tim2CheckFileHeader). Debug-only
functions retail lacks wrapped line by line (`CGameWork::getPoohSysFlag`, `CPlaySpeedCtrl::setDebugPlaySpeed`,
SoundMan's `_getBgmName_withID`).

11. (decision) The gap after a recorded unit: owned by a unit between the two in link order that has nothing of its
    own in that section (PackMan's 8 bytes of .rodata between MotData and PathCamera); else the next unit's leading
    data, unless the next object's own alignment produces it anyway (PathData after PathCamera).
12. (decision) `scan` rejects a unit when the retail image has nonzero data after our section's end inside the
    unit's range (retail has more data there: GameWork's .data).

### Open (for the next session)

- `DebugWork::g_dw`: done as decided by the user (2026-11-11, below); left: VSME's debug menu tables (data with
  `&g_dw...` pointers) and the debug-only overrides it implies (e.g. CBattleEnemy::update exists only for a switch:
  retail has no override, so the CFSM vtable slot differs).
- Retail changes proper (`calls`/`shape` and many `inline`/`header` DIFFs are real code changes, e.g.
  ATEL::CAvatar::InitAvatar's new `% 10000 == 1007` case): `KH_RELEASE`/`KH_VER` variants written from the retail
  assembly, unit by unit (`tools/retail_units.py try <unit> --show <function>`).
- Static initializers retail lacks (EnemyHeartsoldier, EnemySpadesoldier, GimmickDoor, SampleGimmick, MapUnit,
  main): the dynamically initialized statics are initialized differently there (e.g. `g_vDeadDemoPlayerPos`).
- Size mismatches with all functions matching: PlaySpeedCtrl (retail names getProgressTime and getSysProgressTime
  at one address), SoundMan (retail has a function the source lacks).
- Units still with INCLUDE_ASM (the beta's parked functions) are not scanned.

## DebugWork switches off (user decision 2026-11-11: "treat g_dw switches as off in retail")

- Reads of `DebugWork::g_dw.x` are `KH_DW(DebugWork::g_dw.x)`: the read in the beta, `0` in retail (gcc folds the
  dead branches without a trace: EnemyAirpirate::aiCardBattle matched). Whole-line writes are `KH_DEBUG(...)`
  (`KH_RELEASE(;)` after them where they are the body of an unbraced if/else). Vector members (vCamEye, vTemp, ...)
  stay plain reads: they only occur in code that folds away. 287 reads and 183 write lines
  (tools/helpers/retail_dw_wrap.py, debug-only files left out); VSME's debug tables and a few conditional writes are
  left for per-unit work.
- System/Debug's queries are off too: `dbgCheckUser`, `dbgButton*`, `dbgAnalog*` are `0` in retail (khversion.h;
  declarations `(name)`); CComponentObj's dbgButtonTrg/RTrg members and the CCollDisp::initialize calls are
  beta-only.
- Two ee_asm_compat.py fixes for code built without -g (`--release`, passed for the non-reference versions only):
  no short-loop padding over an alignment (a jump table's target, not a loop), and an explicit nop after a jump
  through a register before the jump table's section switch (Ps2EeAs crashed). And the sources' `__asm__(".rdata\n")`
  resyncs (JiminyMemo, Player: beta placeholders) are beta-only: without -g gcc's section bookkeeping differs, and the
  resync left functions in .rdata.
- Result: 10,353 functions match (was 9,320), **454 units from source in retail**.
- Seen on the way: the beta's PackMan beginScene failed one build and matched four rebuilds - the known
  anonymous-namespace suffix hash (memory "gcse PRE hash order"), not a change of this run.

Fifth batch (2026-11-11): **478 units from source in retail.** CBattleEnemy::main/update are beta-only (debug-switch
overrides; ~25 enemies' vtables). Split/layout fixes found on the way (decisions):

13. A unit with a hand-written `.s` source keeps the large code alignment cap (128) also while a version builds it
    from splat's assembly (retail klib after a compiled _mbtowc_r).
14. A library unit's data without pairs of its own takes the offset of the nearest paired library unit (by beta
    address), not the previous one (__FRAME_BEGIN__0 before default_terminate).
15. A gap after a recorded unit is alignment padding only if it is all zeros (PathData's floats after PathCamera).

Sixth batch (2026-11-11): **480 units from source in retail - every eligible unit**, `verify` switches nothing back.

16. Address-named labels in the sources (`extern char D_004FF688[]`) name beta addresses: for another version
    configure.py translates them through the version map (functions.tsv, data.tsv); unmapped ones are not
    provided (Tim2CheckFileHeader's "TIM2"/"CLT2" in .sdata).
17. A row that starts inside a recorded unit moves to its end (lib/nodebug/__CTOR_LIST__ inside tlbfunc's .data).

Seventh batch (2026-11-11): **503 units from source in retail.** `scan` counts references only from what the build
keeps (not from linkonce copies removed as foreign), lets pointers in data whose start is known vote for the
sections they point into (string tables in .data), and accepts a clear majority of votes:

18. (decision) A data start is accepted with >= 80 % of its votes (one outlier is typically a pointer just past an
    array); `verify` would catch a wrong one.

Eighth batch (2026-11-11): **517 units from source in retail** (of 648 complete units; 10 more are debug units retail
lacks). "100 Acre Woods" -> "100 Acre Wood" (CardKind.h's table, 8 units), the 71 unit-local `ASSERT` macros vanish
in retail (`KH_DEBUG` body), CCollUtil::_initDbg is empty in retail.

Ninth batch (2026-11-11, the differing functions): **524 units from source in retail**, 388 functions still differ
(from 480). Debug-only code found from the retail assembly:

- CCoords::Crd's `_coord_alart` / `_coord_nullalart` calls and the two functions (Coord.h, Coord.cpp): retail has
  neither (76 functions in all units that inline Crd).
- CBullet::m_pCollLines (the collision display's C3DLines) and every block that builds it: retail has no CCollDisp;
  whole `if (CCollDisp::m_fgDisp) {...}` blocks are wrapped line by line (their locals must go too: the frame).
- CEffectTask::m_pDebugLine, CEffectMan::dispDebugBBOXLine/_setDebugBBOXLine (absent in retail) and the
  `_checkData()` error blocks of _calc/draw (retail doesn't even call _checkData there).
- `tools/helpers/retail_wrap_lines.py FILE START END ...` wraps line ranges (each line's parentheses must balance;
  a multi-line statement is first joined onto its first line, the continuation left empty: line-neutral, and the
  beta's line note stays on the statement's first line).

19. (decision) `TRACE(...)` and its variants are `((void)(args))` in retail: the arguments are evaluated (calls stay,
    strings go). Evidence: CCoord::fix keeps both `CCoordMan::getExceptCoord(FALSE)` calls of its TRACE, and the
    change fixed 10 functions (Coord, Prize, Card, RsrcData, MSVideo, JiminyMemo, Game, HumanBoss). One TRACE whose
    arguments retail does not evaluate (CEffectMan::_updateCoord's `&effect->Crd(0)`) is wrapped `KH_DEBUG`.
20. (decision) `tools/split_version.py`: a library unit placed by offset before a data section's start is dropped
    when another unit starts the section (lib/nodebug/_GLOBAL__F_0build.cpp's frame data, absent from retail's
    .data, had displaced System/Coord's .data).

Tenth batch (2026-11-11): **526 units from source in retail**, 324 functions differ. More debug-only code from the
retail assembly:

- CBlurPrim::m_iDebugID and the debug-override blocks with their `Bak_*`/`BAK_*` save/restore (DeFocus: retail
  never saves or restores the parameters).
- CPlayerCamera's camera-info strings, CDebugWindow, C3DLines, the g_ParamTest/pTestCamData test copies, the
  `iCamRikuShiftY` ternaries and _initialize's else-store; its never-called inline updateParam.
- DWTPlayer.h's debug window functions and tables (`#ifndef KH_RETAIL` on two former blank lines: the header has
  no `__LINE__`), Player.cpp's CPlayerDebug section (`#ifndef`/`#endif` prefixed to the two comment lines around
  it), CPlayerManager's block strings/window/dbgDispTTY_RsrcBlock, setVibration's KKIKUCHI block, init's
  `case 22` (retail's switch ends at world 13).

21. (decision) A whole region of a file may be gated with `#ifndef KH_RETAIL` ... `#endif` when both directives
    take the place of existing blank or comment lines (line-neutral); otherwise lines are wrapped one by one.
22. (decision) A matched beta statement order may change when both versions match with the new one
    (PLM::setVibration's case 4: `motorL` first, retail sets motorS/tq after the call).
23. (decision) A data start of a unit that isn't from source may be recorded by hand in unit_starts.txt when the
    beta's bytes are found there in retail (FrecFont's color table at 0x332950, LightUnit's .data at 0x3329A0):
    a wrong start in the split shows up as `SHORT` data in the unit before it.

Eleventh batch (2026-11-11): **528 units from source in retail**, 275 functions differ.

- CJoyPad::m_Robot and all of CRobotPad (PadIO), CBBox without its CGPrim base in retail (`class CBBox
  KH_DEBUG(: public CGPrim)`: a plain 0x24-byte box in CMap's grid), CMap's m_gpPosDir and its `m_iMapBack`
  shader block (with the switch read as 0 the `== 0` branch would still run: retail has no block at all), CSky's
  debug writes, the name buffers that only feed a TRACE (CMap::_createEffIdProc, CSceneInfo::setSeRangeRatio:
  their TRACE goes too, as retail evaluates TRACE arguments).
- Debug-menu-only functions (CSceneInfo::getEventRoomSceneNum, setupCompleteFloorWorld, setEventDoorUnavailable,
  setTmpCurrentRoomID; CGame's debug-mode constructor, getDbgGameMode, dbgStartFromBattle, isDbgStartFromBattle)
  and their calls: `CGame::isDbgStartFromBattle()` and `CGame::getDbgGameMode()` reads are `KH_DW(...)` (0).
- DoorInfo::isValidNormaIndex returns TRUE in retail (`return KH_VER(FALSE, TRUE)` after the debug-only check).
- CGameMain: the world-map key's `getCurrentFloorIndex() < 16` test and the battle menu's TsSdDebugPrint() are
  beta-only.
- tools/helpers/retail_wrap_lines.py leaves preprocessor lines and block comments as they are.

Twelfth batch (2026-11-11): **531 units from source in retail**, 235 functions differ.

- The path editor: CDbgCube, CDbgArrow, CPathVector's and CPathData's debug lines and edit state (retail's
  CPathVector is 12 bytes: m_pVec, m_iVecMax, m_pPathData; _init only stores the first two, its getters return
  0.0f without path data: `KH_RELEASE(...)` variants on the same lines), the add/del/dispDebugLine functions,
  the path cameras' debug arrows; DebugPathData.h's body is `#ifndef KH_RETAIL` (on two blank lines).
- CDebugProcInfo's five functions are empty in retail.
- Hard-coded member offsets in matching code follow the retail layout (`KH_VER(0x450, 0x430)`: CEntity is 0x20
  smaller in retail): CBattleEnemy's state array, CHeartless's card kind, HumanBoss's player HP, the gimmick
  parameter in BattleEnemyManager. CEntity::setDamageNumber's callers are beta-only.
- TRACE stays `((void)(args))` (decision 19; CCard::tryAddBattleCardAppearance keeps its `card.getName()` calls),
  but retail also has debug-only regions around traces: statements that only feed a trace (CHermitePathCamera::out's
  camera dumps) and traces whose arguments retail doesn't evaluate (ProjectionToViewAngle in _init) are wrapped.
- Note for the wrap helpers: line numbers from compiler errors are `#line` numbers, not file lines.

Thirteenth batch (2026-11-11): **532 units from source in retail**, 224 functions differ.

- EFFECT_SE::check()'s calls (the function stays, empty), CEffect::print_prof/isValid, _checkData without its
  early `return FALSE` in retail (`if (KH_RELEASE(pData &&) ...)`); EFFECT_DATA::Setup matches without the beta's
  asm barrier (beta-only now).
- CPackMan: kickDMA/kickDMAFG's layer switches and size checks, terminateSemiPacket's body, the size accounting
  (getPkUseSize/getPkTotalSize), swapDBuf's VRAM viewer with its dispfb copy and the PCRTC register writes, the
  unit's unused EERAM statics (their static initializer is absent in retail). CFifoFpl::debugDump is empty.
- Correction: `main`'s static initializer is not missing in retail; it pairs under another anonymous suffix
  (`...main.cpp00aaaa`), which `try` reports as "extra".

### Where it stands (end of the unattended run)

- 517/638 units of retail from source; 10,392 functions match retail, 480 differ, 689 are beta-only ("extra").
- What blocks the rest (tools/retail_units.py scan, build/SLUS_217.99_retail/retail_units/scan.tsv):
  - real retail code changes (`calls`/`shape`, and many `debug`/`header`/`inline` DIFFs: scheduling and registers
    that the beta's matching tricks don't reproduce once the debug code is gone, or new code like
    CAvatar::InitAvatar's case) - per function, `tools/retail_units.py try <unit> --show <function>`;
  - VSME's debug tables (`&g_dw...`), the static initializers retail lacks (EnemyHeartsoldier, EnemySpadesoldier,
    GimmickDoor, SampleGimmick, MapUnit, main), retail data that the source lacks (`SHORT`: GameWork, Summon,
    FriendAriel, Glare, ...), CreateEnemy's unused "abnormal" (BattleEnemy.h's header static, absent in retail);
  - 45 bare `AssertApp(...)` statements (default branches) are left as written: in retail they are empty statements.

