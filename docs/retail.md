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

Fourteenth batch (2026-11-11): **537 units from source in retail**, 209 functions differ.

- CRootTask: the debug menu's settings load (steps 0-1, s_bDbgSettingLoaded, the dtor's check), the pad
  shortcut to step 5 and _launchTitle's debug branch; CMainTaskFactory keeps only the title and game tasks (the
  debug and viewer tasks, the editors' statics and the collision-editor placeholder class are beta-only).
- CDispListBase::m_iDebugFlags (the vtable pointer is at 0x8 in retail) and the packet-size check in _draw;
  CModelMan's draw/texture-send counters and their increments in Gprim and Model; the debug primitives
  C3DCircle, C3DPointLight(2), C3DLine, C3DLineCoord2, C3DLines (their out-of-line functions in Gprim.cpp:
  without them gcc emits neither vtables nor inlines).
- CameraMan's CMap stand-in pads to the retail m_pFog offset (`KH_VER(0x15e0, 0xee0)`).
- A beta-only global left in a unit's .data makes its recorded size too large and hides the next unit's data
  (RootTask's s_bDbgSettingLoaded over TaskMan's _pSLCManager: an undefined symbol at link time).

Fifteenth batch (2026-11-11): **548 units from source in retail**, 176 functions differ.

- The "error" texture fallbacks (2DSpr, 2DSeq, 2DSeqObj) and C2DMan::m_fg2DViewer (the 2D viewer) are beta-only.
- Debug switches with a nonzero default (DebugWork::initialize): `KH_DW_ON(x)` (1 in retail: m_iFukidasiDisp, the
  speech bubbles stay on) and `KH_DWV(x, v)` (v in retail: player.iCardNumLock is -1).
- KeVu0Sqrt's asm has no `nop` after the lqc2 in retail (`KH_VER("nop\n", "")`).
- ee_asm_compat --release, short loops: `.p2align 2` inside a loop is no alignment (no-op in code); a span with a
  real alignment is padded with a count taken from its instructions (the alignment counted as no padding) instead
  of being left to Ps2EeAs (which pads to 6, the original to 5); only a section switch in the span (a jump
  table's block) is no loop. This fixed 13 units, lib/nodebug/strstr among them.

24. (decision) Debug switches that are on by default read as their default in retail (KH_DW_ON, KH_DWV); the plain
    KH_DW stays 0 (the default of every other switch).

Sixteenth batch (2026-11-11): 548 units from source in retail, **160 functions differ**.

- CAvatar: the coordinate-axis lines lineX/Y/Z with disp(), the create*Struct functions in retail form (`if (!m)
  m = new ...; return m != NULL;`), FadeAvatarMain returns a result variable (set FALSE only while fading).
- CSceneInfo: the debug exception room s_roomExcept (retail returns NULL for floor 0xff), reqChangeRoom's
  explicit-scene branch and reqChangeEventRoom's except-room check.
- CGimmickManager: GimDebugTrace with its calls, the probability debug window (CGimmickProbDebug and its tables),
  the gimDropProb name sprintfs and memset, the throw-parameter setters, initRoomCreate's trace counter and its
  overflow checks.

Seventeenth batch (2026-11-11): **549 units from source in retail**, 151 functions differ.

- ATEL::CEventTrap's shape display (the dispShape virtuals: retail's vtables are shorter, and the C3DLines,
  C3DPolyG4 and CCoord members), VSME's DWTEvent debug windows, CActorDebug, CEventLineDisp, dispActorTag;
  ATEL::_TRACE has an empty body in retail.

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


### Session 2026-11-11 (afternoon): 549 -> 564 units

- Done: Atel (CAtel's world/floor/step flag setters are absent in retail; two raw CAvatar offsets above 0x1b0 are
  `KH_VER(beta, retail)`, CEntity being 0x20 smaller), EnemyLoudness (the `g_dw.enemy` act override block:
  `KH_DW` folds the test but its `for` leaves loop notes, a `.p2align` before the epilogue), WorldWarp (`PIN_V1` is
  beta-only: the plain pointer matches retail), SummonMushu (no `getBattleCamData` reload in retail), the debug
  movie player (Movie/MSVideo: `bForDebug`, flag 0x80000000, `openDbg`, `s_nStreamUnderflowCnt` and its getter
  are beta-only; retail's constructor is `__12CMoviePlayerP5CTask`, `_openMovie` takes only the file and never
  makes the dummy player). Statement orders taken from the beta's line markers where retail needed another order
  (AtelFog ctor, PadIO init, PlayerGauge _initialize: chained assignments on lines 126/127, MSVideo close); the
  beta matches with each.
- Tools: `retail_units.py try` pairs a function whose signature retail changed through retail's symbol, or (a
  file-local one) through the unit's one unproduced beta function of the same base name; with it the static
  initializer units (EnemyHeartsoldier, EnemySpadesoldier, GimmickDoor, SampleGimmick, MapUnit, main) switched.
  `split_version.py`: at one address a unit that starts there wins over one pushed there by the gap rule (retail's
  WorldWarp .data covers the beta's `__FRAME_BEGIN__0` bytes; that unit has no .data in retail).
- `try` masks the offset of every load/store whose base register some `lui` of the function loads, anywhere in
  it (`version_map.decode`), so a plain field offset can pass as OK (Atel createAvatar's 0x1e4/0x1c4); `verify`
  catches it. Read the image diff when `verify` switches a unit back that `try` calls eligible.
- Open, tried without a match:
  - CPoolList::init (MessageManager, LockOnManager constructors): retail computes `erase()`'s `&m_Tail`/`&m_Top`
    before the loop (base = the derived `this`, so cse folded them in the first block) and keeps them across it.
    Not the scheduler (single-block regions), not loop.c (the code is after LOOP_END in ours). Tried: unwrapped
    asserts (empty ifs), while/pointer/m_iSize loop forms, a guarding if, `do {} while (0)` at several places.
  - PlayerDRiku constructor: retail stores `m_pParam` with the vtables, the three zero stores after; ours floats the
    zero stores up. Every order of lines 163-167, with and without `_ref`.
  - EffectMan initialize: at the join after `new CEffect` retail issues the `lui` of `s_pExceptEffect` before the
    `s_pEffectList` reload (ours: reload first, reorg copies it into a `beql` slot). Release ASSERT forms at the
    site (`(void)(e)`, empty ifs, `do {} while (0)`) change nothing.
  - CardBattle testBreakingCard: retail keeps branches where ours makes `slt`/`sll 1` and a shared `return 2` tail;
    no release TRACE form (`do {} while (0)`, `if (0) {}`, ...) changes it: probably a source change in retail.
  - MainApp: `_mainLoop` (0x1D8 -> 0x50) and `_warmReset` are restructured in retail (real changes).
  - WorldCard WorldWndFour/Five: retail's frame is 0x10 larger with one more saved register.
- Triage helpers (scratch, build/): `rdiff.py <unit> <function>` (unified ours/retail diff, branch offsets masked),
  `difflist.sh`, `dstat.sh` (lines only in ours/retail per DIFF function; run one unit per process: `try` writes
  one object per unit), `rperm.py` (every order of n lines, retail score).

### Session 2026-11-11 (evening): 564 -> 576 units

- Done (beta-only in retail, line-neutral `KH_DEBUG`): TexInfo's `_nTexSize`/`_nClutSize` (retail TexInfo is 0x68),
  RsrcData's DLL load report and `dumpTag`/`getGroupTagList`, JiminyMemo's `ScaleStr[64]` (CJiminyMemo 0x530) and
  JMemoFlag's 21 debug event setters, RsrcDB's NULL-name fallback, GimmickManager's world-23 and early prize paths,
  Player's debug controls (battle toggle `isTrgBattleChange`, free move, KKIKUCHI keys, hi-jump, debug deck/attack/
  game mode, hang-hand display, `szAtkAnimID`, `m_pDebug` clear), the minigame validation and trace switch,
  LightUnit's map viewer hooks, Model's wireframe display, Math3D's `checkFloat`, CCollDisp::finalize, the attack
  collision list `m_pCollListA` (debug display only), the revolver's debug deck/dump/display blocks, HumanBoss's
  debug act blocks and its `.sdata` string placeholders.
- Retail forms (`KH_RELEASE`/`KH_VER`): JiminyMemo's assert-returns (`createSequence`: `if (!(n < 32)) return NULL`;
  `main`'s default ends the menu like -1), TiggerJump/WildDive object sizes, Player's `startSingleAction` offset
  (CEntity 0x20 smaller), `makeRevolver`'s deck taken at the declaration (movn form), LightUnit's `ll[5]`.
- Source shapes that fit both builds (the beta matches with either; retail needs this one): RsrcDB
  getFirstRsrc_Scene's `pRsrc = pContainer ? ... : NULL` without the NULL init, CollBody's two `detectColumnColumnEz`
  case groups ({0x22, 0x42} and {0x24, 0x44}: retail's jump table).
- Lessons: a debug block folded by `dbgCheckUser`/`dbgButton*`/`KH_DW` still changes code: its locals keep stack
  space (stateField/stateBattle frames), its calls in conditions stay (`isSoraGame()`, `isStandingCard()`), its
  loops leave notes/alignment, its tests get CSE'd into the code after it (MinigameManager sendEvent's trace
  switch), it shifts scheduling (CardRevolver main). Wrap the whole block. `scan` doesn't check `.bss` contents:
  a beta-only global there (`szAtkAnimID`) breaks the link (`verify` shows it).
- Open, tried without a match: HumanBoss enemyCardProc/stockActProc/normalActProc (retail keeps branches or
  cross-jumps where ours makes store-flags; no release TRACE form or empty asm explains all three), CardBattle
  testBreakingCard (same family), BattleEnemyManager main (the 15000/6000 arms: the inverted `if` matches retail and
  the beta, but the beta's markers put 15000 first), GimmickManager DecideGimmickCard_PossibleNoCheck (one saved
  register more in ours), FifoFpl getValidActiveBlock (early `return NULL` placed inline in retail), JiminyMemo
  setPlateText/contentMinigame/CMemo3DObj::main (register allocation), CardRevolver `_initialize` (ours cross-jumps
  the tutorial arm's convBtlDeckToCard/setDeck tail), TreasureBox CardMaxWarningMain, LightUnit _calc (two spill
  slots swapped), Card (retail's .data/.rodata carry BattleEnemy.h's `abnormal`/`cure01` statics: Card includes
  BattleEnemy.h in retail, not in the beta), CollBody/CollMap (retail has 16 zero bytes between their .rodata that
  neither object produces; `verify` keeps CollMap in assembly).
- Helpers (build/, scratch): `wrap_lines.py FILE A-B ...` (line-neutral KH_DEBUG wrapping, multi-line statements as
  one call), `khcheck.py FILES` (KH_DEBUG/KH_RELEASE calls whose parentheses don't close on their line),
  `calls.sh <unit> <function>` (call targets ours vs retail by name).

### Unattended run 2026-11-11 (night): 576 -> 601 units (user: "work through the rest of the plan unattended")

Decisions taken alone (revisit as needed):

13. (decision) `config/<VERSION>/unit_starts.manual.txt`: data starts measured by hand, each with its evidence;
    `split_version.py` applies them with `unit_starts.txt`, `retail_units.py scan` prefers them to its votes. Used for
    data-only units (pppfunctbl's .rodata), units still in assembly whose data begins before their first named
    symbol (CardBattle, HumanBoss, LockOnManager, VSME: BattleEnemy.h's file-local statics come first), and votes that
    split or pick the wrong copy (libpad2, thread, CreateGimmick, CreateEnemy).
14. (decision) `!unit` lines in sources.txt keep a unit in assembly; `switch` honours them. CollMap is excluded: 16
    zero bytes between CollBody's and CollMap's retail .rodata that neither object produces.
15. (decision) Where the beta's assert read a pointer right after storing it, retail issues the store first; the
    store (or the later global load) goes through `_ref()` with a `MATCH:` note (TaskMan initialize, DecknameEdit,
    EffectMan, PlayerDRiku). Both builds match with it.
16. (decision) The beta's build stamp before libgcc's frame.c data ("EEDO 2008/10/01 ...") is absent in retail:
    `#ifndef KH_RETAIL`, and lib/src/gcc/frame gets `-DKH_RETAIL` in the retail build.yaml.

Done besides: VramMan (`_allocSub`'s name parameter and the dump), TaskMan (debug play speed and stepping; the play
speed call takes the raw frame lag), PackMan (`_checkDmaTag` is an assert helper; ZBUF stores chained, as the line
markers and the ref show), MSStream (retail's ring buffer uses absolute counters), Memoca (debug save type, tests and
unformat; retail's card titles "Re:CoM System Data"), FifoFpl (release checks; `getValidActiveBlock` ends in one
ternary, which also matches the beta's single marker line), 2DMan (trace buffer), SaveLoadUI (update limit), partMng
(retail computes texture/CLUT addresses with one formula; debugDpdCheckShapeData), Card, AttackMotion, Heartless,
GameWork, CreateEnemy (its placeholder header string returned instead of traced: retail keeps the string),
MapEnemyManager and GimmickCarry (getters that are class inlines, weak in the beta as well), ChatMan (retail adds an
empty `main` override), PlaySpeedCtrl, dlmalloc (debug allocation API), SoundMan (`_getBgmName_withID` exists in
retail, unnamed), libpad2, thread, frame, sddrv_ee (bind wait 0x10000).

Tools: `split_version.py` unifies anonymous-namespace suffixes inside mangled type names; `ee_asm_compat.py`'s linkonce
drop stops at the next `.globl`; `configure.py` takes a non-reference version's label address from its symbol file
(`cardDataTbl_00474480` is the beta's address); `retail_units.py try` pairs retail-only weak copies at their symbol
and unresolved functions by their offset in the unit; `verify` stops at a DLL-table failure or a repeated culprit
(it had switched innocent units back ten times).

Lessons: read the whole `verify` log (it may switch a unit back and still end "OK"); a variant test must check that
it compiled (an edit that drops a brace scores 0 differences).

Open, with what was tried:
- The `""` of `CTask::getDebugInfo()` (retail): FrecFont, TriangleIK and SoundMan don't have it at Task.h's position
  in retail, ~50 other units do (an experiment returning a Task.cpp array broke those). Most likely these three don't
  include Task.h in the original; needs header evidence.
- Register/scheduling ties, no lever found yet: Avatar RotateAvatarMain (this vs vRot), Game setupWorld (this vs
  nState), SaveLoadUI's constructor (spill slots), GimmickManager DecideGimmickCard_PossibleNoCheck, JiminyMemo (3),
  MSStream threadStream (`bTrayOpened` as li 1 + movn), Model getTexParam (retail copies the hoisted VRAM loads),
  PathData getNearPath, LightUnit _calc (spill slots), CtdMan _sprintf, BattleEnemy makeCard, PlayerCamera (3),
  WorldCard (2), the CPoolList constructors (MessageManager, LockOnManager), HumanBoss (3), CardBattle (3),
  BattleEnemyManager (3), SceneInfo (5), CardRevolver (2).
- Retail restructured: MainApp (_mainLoop, _warmReset), VSME (execExt2Code/execExt3Code), HeapMan, PlayerManager,
  BattleCard, Entity, MapEnemy, ShadowModel, PlayerControl, pppDrawShapeX, pppPart, SaveLoadUtil.
- Units without retail code: DebugMenu, DebugWindow, DebugWindowTBL, CollDisp, KisEnemy, System/Debug, DebugWork,
  DebugPathData, vprintf, vsprintf.

### Unattended run 2026-11-11/12 (second night): 601 -> 631 units (user: "keep working on the what's-left units")

The main lever this time: **the beta's line markers give the original statement order** where our beta-matching
source happened to compile the same with another order (sched2 and cross-jumping hid it); retail's scheduling and
register allocation show the difference. `tools/helpers/asmlines.py` on the beta, then the retail try:
Game setupWorld (both arms assign before their call), GimmickManager DecideGimmickCard_PossibleNoCheck (Num, Kind, No,
StockKind; `breakFlag` declared after `ret` as the ref lists it), PlayerCamera initCamera (timer and near stores
before the second init), PathData getNearPath (515-518), BattleEnemyManager's constructor (setAppearRoulettePrize in
each arm), MainApp _loadIopModules (heap before pInfo), BattleCard's constructor (a mem-initializer list: all its
member stores carry the constructor's line; m_nNumber first). Each edit keeps the beta matching.

`build/rpermuter.py` (scratch): tools/permuter.py scored against the retail function (KH_VERSION, a retail compile
command for units not switched yet). It found setupWorld, DecideGimmickCard and getNearPath in seconds; the markers
then chose among its changes. It can't resolve constructors by mangled name, and fails while check.sh rebuilds.

Decisions taken alone:

17. (decision) Task.h's `getDebugInfo` "" only where retail has it: RsrcData.h includes Task.h's own includes
    (Debug.h, SLContainer.h) instead of Task.h, AttachMotion.h includes Task.h (EffectMotion and SEMotion have the ""
    before their own strings; of their headers only this one is theirs alone), TriangleIK drops the unused Camera.h.
18. (decision) `config/<VERSION>/function_pairs.manual.txt`: function pairs by hand, read by tools/version_map.py
    before the call graph. Retail's CMainApp::run no longer calls DebugWork::initialize, which had shifted the
    call-graph names of its other callees by one (`_warmReset` was named "initialize__9DebugWorkv"); MainApp's static
    functions are paired by link order.
19. (decision) A debug switch may behave as on in one place and off in another: BattleCard stateHand's
    `iDeckDbgDisp` tests match retail as KH_DW_ON (`if (!1) ...` keeps the block layout that removing the statement
    loses), setHandLocate's as KH_DW (off).
20. (decision) Placeholders for retail-only zero blocks in .rodata: BattleCard (16 bytes before the "" its tables
    use, so its jump tables stay 16-aligned at 0x407010), CollMap (the 16 unclaimed zero bytes after CollBody's end;
    this replaces decision 14's `!` exclusion), Game's beta placeholder `KH_DEBUG` (retail merges it into Task.h's "").
21. (decision) Hand pairs also for pppPart (pppRunPartDraw had been given callEndProg's address by the call graph).
22. (tool) `retail_units.py scan`: data that runs past the next unit's start onto its bytes is `LONG`, not OK.

Done: TriangleIK, SoundMan, FrecFont (Task.h; FrecFont's .data start by hand), Game, Entity (the debug camera check;
EntityFactory's unreferenced .rodata start by hand), ShadowModel (drawAfter's shadow-count knob and its loop),
GimmickManager, PlayerCamera (mode names; m_nBossCamID through `_ref`), PathData, PlayerControl (debug inputs),
HeapMan (the debug allocation API, dumps and reports), MainApp (trace screen, halt, AssertApp, debug pause stepping,
`_bSetupManagers`; retail's IOP module file is 0x8F sectors), SaveLoadUtil (a Riku-game rollback retail lacks),
BattleCard, pppPart (paired by hand), SceneInfo (AREA_SCENE's inlines `operator=`, `operator==`, `clear`, `isEmpty`
as the ref's locals and the beta's markers show; the floor-index early returns and openGoalDoor's message
beta-only), CollMap, WorldCard (a block around the per-card `start()` call, found by the retail permuter; the cause
is not known), MSStream (threadStream's error handling as the beta's markers show it: both tests on one line, one
count; `if (nErrCnt >= 2) bTrayOpened = TRUE; else ...` gives retail's `li 1; movn`), pppDrawShapeX (`wait = 0` in
each arm, as its eiProg sibling pppDrawShape that already matched). Partial: SceneInfo reqChangeRoom (retail refuses a given scene), Model getTexParam (VramDef.h's
`_getTmpTexArea`, as Gprim) and CSemiZModel::makePkt (one pass), TreasureBox (WndCmnd.h's constructor sets nLine),
VSME (`_TRACE` an empty inline, m_bTrace never set), pppDrawShapeX (no shape check).

Tried without a match (see the open list): TRACE as `do {} while (0)` or `((void)0)` (both worse across ~80 units),
CChatMan::instance() returning `*m_pInstance` (no effect where wanted, WorldSelect worse), loadFromMem's ref locals
(breaks the beta), PlayerManager setReqLoad without the `next` local (breaks the beta), CardRevolver's debug-deck
switch kept (no effect).

Levers that worked this run, in order of yield: the beta's line markers read closely (several statements or
conditions on one line, stores attributed to a header inline, the line order of initialisations), the ref's locals
(declarations, inline helpers), a sibling function that already matches in retail, the version map's pairing
(check `how` = call/search for a DIFF function with a very different size), and the retail permuter. An empty
`__asm__ volatile("")` where a TRACE was reproduces retail's branchy `if (x) return 2;` in CardBattle
testBreakingCard (kept, retail-only); it is not a general rule: as the TRACE macro it is worse across ~90 units, and
in HumanBoss stockActProc it also blocks the cross-jump retail has.

Continued the same night: 621 -> 631 units (LockOnManager, MessageManager, CardBattle, BattleEnemyManager,
HumanBoss, BattleEnemy, CtdMan, JiminyMemo, LightUnit, Avatar). What worked, all retail-only (`KH_RELEASE`/`KH_DEBUG`/`KH_VER`, line-neutral) unless noted:

- **Control flow where a TRACE and a `return` were.** Without the trace, retail's `if (x) { TRACE; return TRUE; }
  return FALSE;` is not if-converted and gets cross-jumped with a sibling test: the original falls through to the
  function's last `return` instead (HumanBoss stockActProc: the stock-card arm's `return FALSE` gone and the deck roll
  in an `else`; enemyCardProc: the normal-card arms and the deck roll share one `return TRUE` through an `else`).
- **A loop left with `break` instead of `return`** stays unrotated (BattleEnemyManager notifyDead): this one fits
  both builds and is plain source.
- **Asserts that are code in retail:** JiminyMemo CMemo3DObj::main's two allocation asserts are checks that leave the
  object (`if (!m_pModels) { out(); m_iStep = 0xFFFF; return; }`, also `m_pAnime`); BattleEnemy makeCard's `bem`
  stays referenced where its assert was (an empty `asm volatile` with `bem` as input: the allocno priority over
  `inPos`; a real check adds a branch retail doesn't have).
- **Declaration order / temporaries for spill slots and copies:** CardBattle exePlaying declares `iTrg` before
  `iDuelNG` (pseudo numbers decide the two spill slots; the ref, i.e. the beta, has the other order),
  calcEnchantParam takes `_PLAYER` through a temporary (retail's extra `move`). PoolList's `init` takes the two
  sentinel addresses before the fill loop (LockOnManager/MessageManager constructors).
- **One more use of a value (allocno priority floor_log2(refs)*refs/live_length, `-dl` "Register N used k times
  across n insns", `-dg` allocation order):** Avatar RotateAvatarMain's rotation vector had 3 refs over 11 insns
  (0.27) against this's 0.49; Coord.h `_addRot` stores through a second operand for `vOut` in retail (4 refs, 0.73),
  which every other `_addRot` user still matches with.
- **A register pin the beta needs is beta-only:** HumanBoss normalActProc's `register INT nNone asm("$22")`; without
  the pin retail allocates as the original.
- **Swapped arms:** BattleEnemyManager main's timer (`if (!m_bCreateCard) 6000 else 15000` in retail; the beta's
  markers put 6000 four lines after the test, so the beta keeps its order).
- **The ref's locals for declaration order** (fits both builds): CtdMan _sprintf declares `fgCtrlCode` before
  `pSrcText` as the ref lists them (s6/s7). JiminyMemo setPlateText's lookup as `if (inCTNo >= 0) { pText = ...; if
  (!pText) pText = ""; } else pText = "";` (both builds).
- **gcse's hash-table size** ([[gcse-bucket-count-lever]] in memory): PRE numbers its hoisted pseudos in bucket order,
  bucket = raw hash mod S, S = max_cuid/2|1, raw = 13258 + base regno + offset for `reg+const`. Retail has another
  insn count than the beta at gcse time, so the order (spill slots, s-registers) differs: count it with `-dG`,
  solve for S, then find the natural source difference. LightUnit _calc: the light count (`count = 0`, `if (pow > 0)
  count++`) only feeds the debug map viewer, so it is beta-only (S 125 -> 123). JiminyMemo contentMinigame: two of
  the five dead initializers (added for the beta's S) are beta-only, and the first list loop increments its
  pointers as statements (both builds). Probe with k empty asms at the function's end to find the needed count
  first (SaveLoad's constructor: 438 -> 292 only, more than S differs).
- **Real retail code changes:** CardRevolver _initialize's Sora starter deck takes `inShortCut` (retail shares its
  tail with Riku's; the beta passes -1). Look for constants and arguments that differ (`build/semdiff.py`: the retail
  try with registers masked) before treating a difference as a compiler puzzle.
- **Data starts by hand** (`unit_starts.manual.txt`): CardKind's `.data` (getName's `pBlankText`; the split had given
  its 8 bytes to CardBattle), MapEnemy's `.data` (`g_pIdlingMotion` right after HumanBoss's 0x200 bytes).

Tried here without a match: BattleEnemy makeCard with the asserts as checks (adds the branch), CChatMan::instance()
keeping `instance` referenced (much worse), MapEnemy searchGround_ with a precomputed loop bound or split
statements (no change), CardRevolver _initialize with the debug-deck switch as `switch (0)` (no change), VSME
close_dest_se's call forms (a local reference, `mode = 1`, blocks: no change).

Open after this run (17 complete units not from source in retail after the morning run below: 10 have no retail code (DebugMenu, DebugWindow,
DebugWindowTBL, CollDisp, KisEnemy, System/Debug, DebugWork, DebugPathData, vprintf, vsprintf); the rest, with
what was found):
- Register ties: CardRevolver setPlayCard (s0/s2 and s3/s4 renamed only: card 3*14/161 = 0.261 against pCB
  3*8/106 = 0.226; two more uses of pCB (probe: empty asms with pCB as input) fix s0/s2, s3/s4 (category vs inFlag)
  stays; the permuter's 36 -> 30 needs declaration moves the ref contradicts), SaveLoad's constructor (15 pointer-to-member temporaries' spill slots permuted: gcse's PRE order; our max_cuid is
  447 (S = 223), retail's 448-451 (S = 225): 1-4 empty asms anywhere early in the constructor match 124/124, not
  committed: no natural source found (the Fsm.h set() asserts as conditions change nothing; the ref names all five
  loop variables `i`, which changes no code)), Model sendTexture (s1/s2/s3 permuted: a global-alloc near-tie, `t` 4*23/107 = 0.8598 against
  nClutPos 3*8/28 and its 16-bit copy 2*6/14 = 0.8571; one more insn in t's range before nClutPos is computed flips
  it (an empty asm in the texture-animation block matches, not committed: no natural source found; the ref shapes
  the loop with TexInfo inlines and no `t` pointer).
- The CChatMan reqCommand pattern (GimmickTreasureBox CardMaxWarningMain, VSME close_dest_se, execExt2/3Code): where
  a local was stored just before the call (`iFg = 1; reqCommand(0x3EB, &iFg, 0)`), retail computes `this + 0x40` into
  its own register and ties the vtable delta to a0 (`addu a0, t0, a0`); ours ties the adjusted this. The calls
  without such a store match. VSME's execExt tails then cross-jump in retail. Local-alloc (`-dl`): the delta pseudo
  outranks the vtable pseudo because sched1 puts the `&mode` setup inside the vtable's life; `INT mode[1]` (an
  aggregate on the stack from the start, no ADDRESSOF) reproduces retail in close_dest_se, but the ref has plain
  `INT mode`/`INT iFg`. CChatMan::instance() forms make no difference. GimmickTreasureBox switches completely
  with `INT iFg[1]` (26/26), so the whole difference is "in memory from the start" versus gcc's ADDRESSOF for an
  address-taken scalar; `volatile INT` is worse, and function.c's ADDRESSOF condition (optimize, promoted mode) does
  not depend on -g. The beta (-g) matches with either form, so only the no-g compile tells them apart (likely
  sched1's tie order, which line notes change). Not committed: the ref (the beta's debug info) types both as INT.
- Layout: PlayerManager setReqLoad (mallocBattleRsrcBuffer matched 2026-11-12, see below). Jump1 copies the block
  loop's exit test before the loop in both; retail keeps the copy and its break path branches straight to `stack:`,
  while ours keeps the post-loop `if (i < 16)` on the break path (cse2 does not fold it: the break edges go to the
  loop's end label, which has many uses, and cse only follows single-use labels; in the beta the break's TRACE
  block is such a label) and jump2 then cross-jumps the copy into the loop. Tried without a match: `goto stack` in
  the loop with or without the post-loop test (the loop gets strength-reduced), other forms of the post-loop test,
  an empty statement or asm on the break path (much worse).
- Others: VSME loadFromMem (copy-loop registers: retail gives the counter a0 and the pointer a1 in every copy loop;
  the ref's `i`/`p` declaration order and dropping the `nMagic` local (the ref has none) change nothing or worse).

### Unattended run 2026-11-12 (morning, /loop): 631 -> 632 units

- **The assert's own `if` is beta-only** (PlayerManager mallocBattleRsrcBuffer): `if (inAssert) { if (!g_pRsrcFirst)
  AssertApp(...); }` written out leaves the inner test, a conditional jump to the return, in retail's failure block,
  and loop.c's find_and_verify_loops only moves a block ending in an unconditional exit out of the loop when no jump
  precedes it in the block (the original has the block at the barrier before the Riku arm). `KH_DEBUG(if
  (!g_pRsrcFirst) {)` / `KH_DEBUG(})` on the test and its brace. Any hand-written `if (...) AssertApp(...)` without
  KH_DEBUG around the `if` leaves such a test in retail.
- **An implicit destructor instead of an empty explicit one** (MapEnemy searchGround_, both builds): an inlined
  `virtual ~CPointCollision() {}` stores CPointCollision's own vptr before CCollision's (flow drops the dead store
  later, so no final code shows it, the out-of-line copy included), three insns at loop time. loop.c moves an
  invariant when threshold * savings * lifetime >= the loop's insn count, threshold = (1 + non-fixed regs) with a
  call in the loop, 3 less per moved insn (`-dL`: "Loop from .. : N real insns", "move-insn savings 1 not
  desirable"): the 45.0f increment was the fifth candidate of pass 2 (53 insns), retail hoists it. With the implicit
  destructor (CSphereCollision already has one) the loop counts 57/50 and 45.0f is hoisted.
- **Data starts:** MapEnemyAir's and MapEnemyGround's `.rodata` (only EnemyParam.h's `""` and `"@ENP"`, 0x10 each)
  at retail's 0x421190/0x4211A0: the split had given them to MapEnemy's `.rodata` ("bytes SHORT").

Tried here without a match: Model sendTexture with incTexSendCount as an empty inline in retail (no change).
