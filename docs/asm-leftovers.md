# Parked INCLUDE_ASM functions and asm data

Units whose remaining functions (or data) hit the time box of the leftovers plan. Each entry: what remains and
the closest result. Revisit after the units with few leftovers are done; lessons from later units may unlock these.

| Unit | Function / data | Remaining difference |
|---|---|---|
| Game/Collision/Ride | `.rodata` from asm | Split puts the unit's `.rodata` at 4 mod 8 (starts with another unit's vtable tail); gcc 8-aligns strings, so source data can't line up. Needs a split change (out of scope). |
| System/dlmalloc | `destroy_mspace` | Original: `lw seg.next; beqz; lw; bnezl; lw; L: bnezl; lw` (peeled step + while loop with a duplicated exit test). jump2 cross-jumps the peeled `lw; beqz` into the loop; an asm statement between them stops that but reorg then leaves the second test's delay slot unfilled. See the note in the source. |
| System/Graphic/2d/2DCard | `replaceNorma` | Original loads the by-value `NormaSrc` as four `lhu` + `dsll`/`or` into a1; ours copies it to an aligned temporary and uses one `ld`. No struct shape or compiler version reproduces it. |
| Game/Gimmick/GimmickPlantParent | `beginStateHitAttack` | `sb m_Stat` is emitted before `li a1,1` for the `saveStat(1)` call; the original has the store last (delay slot). Both depend only on the call, so sched1's dying-source rule orders them; 58 spellings/orders tried. ~20 min over two rounds (stopped at the user's request, not the 45-min box). See the note in the source. |
| Game/Enemy/EnemyWhitemushroom | `damage_` | Case 4: the original loads the vptr for `getLockOnPos` right after setupFade's two TIMEQ stores. Ours loads it after all six stores, because the vtable-entry loads (unknown base) depend on every store. All 720 store orders (several flag forms, inline and written out), reference stores, asm fillers/barriers and local placement were tried. `-fstrict-aliasing` moves the load (an aliasing question), but that is not the build flag. ~45 min. Data comes from source (placeholder + jump table asm). |
| System/Graphic/Effect/libppp/keProg/pppKeShpTail3 | `pppKeShpTail3Draw` | 37 lines, all in one region: between the ScreenMatrix copy and the CMat asm the original addresses the stack-resident `pw` through `$a3` (`move a3,sp`, a copy of the soft frame pointer = &mat0). A dummy `mat0` pointer reproduces the copy, but CSE keeps the fixed frame register canonical, so `pw` never uses it. Near-match (with hacks) in the source. ~45 min (plus hill climbs and random mutation search). |
| System/Graphic/Effect/libppp/keProg/pppKeShpTail3X | `pppKeShpTail3XDraw` | Same as pppKeShpTail3Draw: 37 lines, only `pw` addressed through `$a3` (copy of the soft frame pointer) between the ScreenMatrix copy and the CMat asm. Other compiler versions don't match either. ~45 min (shared with Tail3, plus a random mutation search). |

## Inventory of the game units

Every game function still INCLUDE_ASM'd, generated from the sources by
`tools/leftovers.py game --update docs/asm-leftovers.md` (the table above is curated by hand).

<!-- leftovers:game:start -->
### Units with INCLUDE_ASM functions

Sorted by the instructions left. `parked`: functions with a C draft under `#if 0`.

| unit | functions left | instr left | parked | data from asm |
|---|---:|---:|---:|---|
| `Game/Atel/VSME` | 90 | 18301 |  | .bss .data .rodata |
| `Form/Menu/JiminyMemo` | 160 | 17447 |  | .bss .data .rodata |
| `Game/Player/PlayerSora` | 25 | 14383 |  | .data .rodata |
| `Form/Menu/Camp` | 19 | 13530 | 13 |  |
| `System/Graphic/Gprim` | 321 | 12993 |  | .data .rodata |
| `Game/Enemy/BattleEnemy` | 249 | 9962 |  | .data .rodata |
| `Form/Menu/SaveLoad` | 86 | 9524 |  | .bss .data .rodata |
| `Game/CardBattle` | 85 | 7583 |  | .data .rodata |
| `Game/Debug/DebugPathData` | 77 | 6931 |  | .data .rodata |
| `Game/CardRevolver` | 45 | 6930 |  | .data .rodata |
| `Game/Player/PlayerGauge` | 61 | 6867 |  | .bss .data .rodata |
| `System/Graphic/Model` | 170 | 5996 |  | .bss .data .rodata |
| `Game/Player/PlayerDRiku` | 5 | 5620 | 1 |  |
| `Game/BattleCard` | 73 | 5375 |  | .bss .data .rodata |
| `Game/Atel/Avatar` | 113 | 4856 |  | .rodata |
| `Game/SceneInfo` | 57 | 4635 |  | .bss .data .rodata |
| `System/Graphic/PackMan` | 42 | 4290 |  | .bss .rodata |
| `Game/Entity` | 117 | 3865 |  | .rodata |
| `Game/Map` | 26 | 3779 |  | .data .rodata |
| `Game/Player/Player` | 11 | 3573 | 11 |  |
| `Game/Player/PlayerRiku` | 2 | 2940 | 2 |  |
| `Game/Collision/CollUtil` | 13 | 2562 |  | .rodata |
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | 3 | 2143 | 3 | .bss .data .rodata |
| `Form/Menu/RoomCreation` | 1 | 1995 | 1 |  |
| `Game/Card` | 12 | 1794 | 12 |  |
| `System/Graphic/Effect/libppp/draw/shape` | 2 | 1771 | 2 | .rodata |
| `System/Rcf` | 18 | 1770 | 5 |  |
| `Game/Enemy/HumanBoss` | 9 | 1467 | 8 |  |
| `System/Graphic/DeFocus` | 6 | 1387 |  |  |
| `Game/Collision/CollUtilO` | 6 | 1363 | 3 | .rodata |
| `Game/Player/Friend` | 1 | 1274 | 1 |  |
| `System/Graphic/FrecFont` | 3 | 1208 | 1 |  |
| `System/Graphic/TexUtil` | 8 | 1133 |  |  |
| `System/Graphic/PathData` | 2 | 1075 |  |  |
| `Game/Movement` | 3 | 1020 | 1 |  |
| `System/MainApp` | 6 | 940 | 2 |  |
| `Game/Gimmick/GimmickTreasureBox` | 4 | 908 | 4 | .rodata |
| `System/Graphic/ShadowModel` | 2 | 859 | 1 |  |
| `System/Graphic/2d/2DSprObj` | 2 | 763 |  |  |
| `System/Graphic/Anim` | 4 | 742 | 4 |  |
| `System/Graphic/Effect/libppp/keProg/KeTh` | 1 | 685 | 1 |  |
| `Form/DebugWindow` | 2 | 667 | 2 |  |
| `System/Graphic/HermitePathCamera` | 1 | 632 |  |  |
| `Game/Game` | 5 | 624 |  |  |
| `Game/EffectMotion` | 4 | 609 | 1 |  |
| `System/Graphic/Effect/EffectMan` | 3 | 604 | 3 |  |
| `Game/Enemy/MapEnemy` | 3 | 562 | 3 |  |
| `System/RcfCD` | 4 | 549 |  |  |
| `Game/Atel/CameraMan` | 1 | 536 |  |  |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3X` | 1 | 532 | 1 |  |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3` | 1 | 514 | 1 |  |
| `System/Graphic/Glare` | 1 | 446 | 1 | .data |
| `System/Memoca` | 3 | 442 | 3 |  |
| `Game/Player/NaviMap` | 1 | 425 | 1 |  |
| `System/Wnd` | 2 | 409 |  |  |
| `System/Sound/driver/sddrv_ee` | 2 | 403 | 2 | .bss |
| `System/Graphic/Effect/libppp/partMng` | 2 | 400 | 2 |  |
| `Game/Player/PlayerCamera` | 2 | 371 | 2 |  |
| `Game/Collision/CollMap` | 2 | 365 | 2 |  |
| `Game/Collision/CollAttack` | 3 | 334 | 3 |  |
| `System/Graphic/ModelMan` | 3 | 311 | 3 |  |
| `Game/Atel/AtelCrossFade` | 3 | 291 | 3 | .data |
| `Form/Menu/Roulette` | 1 | 270 | 1 |  |
| `System/Graphic/WorldEnv` | 3 | 268 | 3 |  |
| `Game/Collision/CollDisp` | 2 | 262 | 1 |  |
| `System/Graphic/DispList` | 1 | 245 |  |  |
| `System/Math3D` | 1 | 234 | 1 |  |
| `Game/Collision/CollBody` | 1 | 232 | 1 |  |
| `System/TaskMan` | 2 | 217 | 2 |  |
| `System/Graphic/Effect/effect` | 3 | 211 | 2 |  |
| `Game/Atel/Atel` | 1 | 189 |  |  |
| `Game/Enemy/EnemyWhitemushroom` | 1 | 184 | 1 |  |
| `System/Movie/MSVideo` | 1 | 180 | 1 |  |
| `System/Graphic/Effect/libppp/pppPart` | 1 | 168 | 1 |  |
| `Game/Enemy/BattleEnemyManager` | 1 | 149 | 1 |  |
| `Game/AttackMotion` | 1 | 136 | 1 |  |
| `Game/Gimmick/GimmickPlantParent` | 1 | 108 | 1 |  |
| `Form/DebugWindowTBL` | 3 | 105 |  |  |
| `System/Movie/AudioDec` | 1 | 97 |  |  |
| `Game/GameSubField` | 1 | 71 |  |  |
| `Form/Menu/DecknameEdit` | 1 | 67 | 1 |  |
| `Game/SEMotion` | 1 | 57 | 1 |  |
| `Game/Player/SummonTink` | 1 | 37 | 1 |  |
| `System/Graphic/2d/2DCard` | 1 | 30 | 1 |  |
| `System/dlmalloc` | 1 | 14 | 1 |  |

Units with all functions in C but data from asm: `Game/Atel/AtelBlur` (.data), `Game/Collision/Ride` (.rodata), `Game/EntityFactory` (.data).

Units without a source file: `Game/Player/PlayerAnim`, `Game/Player/PlayerAttack`, `System/Graphic/Effect/libppp/draw/draw_w`, `System/Graphic/Effect/libppp/draw/draw_ws`, `System/Graphic/Effect/libppp/draw/global_w`, `System/Graphic/Effect/libppp/draw/md1`, `System/Graphic/Effect/libppp/draw/sin_t`, `System/Graphic/Effect/libppp/keProg/KeDMat`, `System/Graphic/Effect/libppp/pppfunctbl`, `cod/004FF700`, `cod/2C2B00`, `cod/3FF680`, `common`.

### Parked functions

INCLUDE_ASM'd functions with a C draft in a disabled block. Note: the source's `Parked:` note, or else
the comment next to the draft, marked (comment); shortened to 300 characters.

| unit | function | instr | note |
|---|---|---:|---|
| `System/MainApp` | `_mainLoop__Fv` | 118 |  |
| `System/MainApp` | `setSysPause__8CMainAppi` | 9 |  |
| `System/Math3D` | `InvMatrix__6Math3DR7FMATRIXRC7FMATRIX` | 234 | (comment) Same code apart from register allocation: gcc hoists the 1e-8 constants and a "li 20" out of the outer loop (the original keeps them inside), which moves &c into s1. |
| `System/Memoca` | `_setupIconSys__FPCc` | 199 |  |
| `System/Memoca` | `_McCheckDataFileExist__FiPCc` | 99 |  |
| `System/Memoca` | `_McWriteIcons__FPCc` | 144 |  |
| `System/dlmalloc` | `destroy_mspace` | 14 | (comment) Not matching. The original is: lw seg.next; beqz; lw next (A); bnezl L, lw (entry test + body); L: bnezl L, lw. That is "if (sp) { sp = sp->next; while (sp) sp = sp->next; }" with the loop exit test duplicated, but jump2 cross-jumps the peeled "lw; beqz" into the loop (the insn before … |
| `System/Graphic/Anim` | `setMotion__5CAnimiii` | 152 | (comment) Near-match: register choice for m_pMotData (a0 vs t0) and the operand order of one addu |
| `System/Graphic/Anim` | `_setUpdate__5CAnimiii` | 365 | (comment) Near-match: the fixed-quaternion test (original sets 1 by default, then tests the second word) |
| `System/Graphic/Anim` | `_exeRootMove__5CAnim` | 192 | (comment) Near-match: the angle limit keeps the argument in another FP register (mov.s copy) |
| `System/Graphic/Anim` | `_hokanRot__Ffff` | 33 | (comment) Near-match: the angle difference is copied into another FP register before it is limited |
| `System/Graphic/FrecFont` | `setupFontTex__9CFrecFontRC8RsrcInfoRC4CBuf` | 252 | (comment) Matches except for the order of two loop-invariant CSA values that loop.c hoists ((nCSA + 6) << 56 is computed before (nCSA + 2) << 56 in the original). |
| `System/Graphic/Glare` | `makePkt__10CGlarePrim` | 446 | (comment) not matching: register allocation differs (the original keeps the -1 of its divisions by 32 in a saved register from the start); with it the function-local statics ofsX/ofsY, so .data comes from the original assembly |
| `System/Graphic/ModelMan` | `reset__9CModelMan` | 43 | (comment) reset: near-match. The original reloads the 0.0f arguments (and stack addresses) after each call to FVECTOR::set (called out of line here); this build keeps them in callee-saved registers across the call. |
| `System/Graphic/ModelMan` | `setupDrawEnv__9CModelMan` | 204 | (comment) setupDrawEnv: near-match. The original reloads the 0.0f arguments (and stack addresses) after each call to FVECTOR::set (called out of line here); this build keeps them in callee-saved registers across the call. |
| `System/Graphic/ModelMan` | `setupDrawMode__9CModelMani` | 64 | (comment) setupDrawMode: near-match. The two halves of the first GIF tag get swapped registers ($v0/$v1). |
| `System/Graphic/ShadowModel` | `drawBefore__15CShadowModelMan` | 188 | (comment) Near match (two instructions: the original updates the i 64 + 63 induction register at the end of the loop body, not after i++), kept for reference: |
| `System/Graphic/WorldEnv` | `initialize__9CWorldEnv` | 128 |  |
| `System/Graphic/WorldEnv` | `mainProc__9CWorldEnv` | 111 | (comment) mainProc: near-match. The original stores _vecMikePos.z through a new %hi(_vecMikePos + 8) after the sqc2 of the unit vector (Coord.h's inline, lines 87-91); here the address register of the asm is reused. |
| `System/Graphic/WorldEnv` | `getBillboardMatrix__9CWorldEnvR7FMATRIXRC7FVECTORi` | 29 | (comment) getBillboardMatrix: near-match. The original compares nFlag with -1 in a register (slt) and copies the constant 1 it also compares with; a constant -1 is folded to slti here, a variable keeps slt but not the shared 1. |
| `System/Graphic/2d/2DCard` | `replaceNorma__7C2DCardGQ27RCNORMA8NormaSrci` | 30 | (comment) Not matching: the original passes inNorma to createNorma as four lhu from the parameter's stack slot combined with dsll/or into a1 (extract_bit_field in 16-bit pieces); ours copies the struct to an 8-aligned temporary and loads it with one ld. No struct shape tried (bitfields, arrays, … |
| `System/Graphic/Effect/effect` | `_updateSE__7CEffect` | 83 | (comment) Near match: "li v0,-1" and "lw s0,36(s1)" swap in the first block. |
| `System/Graphic/Effect/effect` | `strcpyEffName__FPcPCci` | 23 | (comment) Near match: the original keeps the destination in a second register (t1/a3). |
| `System/Graphic/Effect/EffectMan` | `_updateCoord__10CEffectManP7CEffect` | 243 | (comment) Near-match: register choice of the length temporary and a hoisted address high part |
| `System/Graphic/Effect/EffectMan` | `_calc__10CEffectManfRC7FMATRIXT2P7CCamerai` | 303 | (comment) Near-match: pPos/pLook get each other's registers |
| `System/Graphic/Effect/EffectMan` | `update__10CEffectMan` | 58 |  |
| `System/Graphic/Effect/libppp/partMng` | `vramcache` | 292 | (comment) Register allocation: the original keeps abBlock (the VLA's address) on the stack and the assert's string addresses in s5/s6; here abBlock gets s7 and bUseBlockNum s3, so the file name's %hi lives in t0 (saved around the call, 16 more bytes of frame). The return blocks are also laid out … |
| `System/Graphic/Effect/libppp/partMng` | `pppInitPartRender` | 108 | (comment) Scheduling only: gcc puts the g_Env.lpVsf store first; the original keeps the stores to ppvEnv and g_Env.lpModel/lpShape/lpVsf in source order (all orders of the five statements tried). |
| `System/Graphic/Effect/libppp/pppPart` | `pppSetMatrix` | 168 | (comment) Differs only in v0/v1 register choice for the scale factors of SCALE_VECTOR_XYZ (reload rotation): the original rotates v0/v1, ours always uses v0. |
| `System/Graphic/Effect/libppp/draw/shape` | `pppVu1ShapeDraw` | 133 |  |
| `System/Graphic/Effect/libppp/draw/shape` | `op_shape_draw` | 1638 |  |
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | `drawShipoly` | 795 | (comment) register allocation differs; also short float loops (bc1t hazard nop lost by fix_short_loops) |
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | `drawDistShipoly` | 648 | (comment) register allocation of loop temporaries (v0/v1/a0) and a few float regs differs |
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | `pppEiDrawDrawShipoly` | 700 | (comment) 44 lines differ: Type!=0 copy loop hoists li -1 before lui 0x7000 (original after; spl is not an original local), Type 1 loop swaps a0/a1 (centv giv vs &dv) |
| `System/Graphic/Effect/libppp/keProg/KeTh` | `KeThBP_Update` | 685 | (comment) Best attempt: same code apart from register allocation (the original has flg in s7, pTp in fp and &vf._06 in s6; this gets fp/s6/s7) and the order of a few spill slots (preStep - rootStep, &vf._07, &vf._08, flg & 0x120, &pRP->InVec), which shifts scheduling around them. |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3` | `pppKeShpTail3Draw` | 514 |  |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3X` | `pppKeShpTail3XDraw` | 532 |  |
| `System/Sound/driver/sddrv_ee` | `TsSdSepPlayDBG` | 141 | (comment) Register allocation: sep_size's bytes and the sep_param scan loop get other temporaries (pointer/counter swapped). |
| `System/Sound/driver/sddrv_ee` | `TsSdVsbPlay` | 262 | (comment) Register allocation: addr is spilled instead of kept in s8 (one more stack slot), so most registers differ. |
| `System/Movie/MSVideo` | `setLoadImageTagsYX__12CMoviePlayerPUiPviiiii` | 180 | (comment) Not matching: mby - 1 is not hoisted into a register for the inner loop (the original keeps it in s8 and spills i * 16), so the j++ / dsay increments are scheduled differently. |
| `System/Rcf` | `finalize__4CRcf` | 111 | (comment) Near-match: register allocation of two statics and a loop-bound reload |
| `System/Rcf` | `reqLoadRCFFront__4CRcfiiiPUci` | 92 | (comment) Near-match: register choice inside linkFront |
| `System/Rcf` | `_threadReadProc__4CRcfPv` | 97 | (comment) Near-match: the original re-reads the entry type after every global store |
| `System/Rcf` | `_launchReadThread__4CRcfUii` | 75 | (comment) Near-match: store scheduling of s_nReadRsrcID |
| `System/Rcf` | `loadNextCategoryInfo__4CRcfiPciPi` | 174 | (comment) Near-match: register allocation |
| `System/TaskMan` | `update__8CTaskManiii` | 135 |  |
| `System/TaskMan` | `_invokePrimary_Main__8CTaskManR13TaskMainParamR5CTask` | 82 | (comment) Matches except for one srl that the original has as dsrl (isSecondaryObj) and a delay slot. |
| `Game/AttackMotion` | `initialize__13CAttackMotionP14ATK_COLL_PARAMi` | 136 |  |
| `Game/Card` | `initCardWorld__5CCard` | 181 | (comment) near-match: kept as INCLUDE_ASM |
| `Game/Card` | `initCard__5CCardUs8WORLD_IDs` | 171 | (comment) near-match: kept as INCLUDE_ASM |
| `Game/Card` | `deleteCard__5CCardUss` | 200 | (comment) near-match: kept as INCLUDE_ASM |
| `Game/Card` | `searchCard__5CCardUss` | 125 | (comment) near-match: kept as INCLUDE_ASM |
| `Game/Card` | `searchCardKind__5CCardUss` | 36 | (comment) near-match: kept as INCLUDE_ASM |
| `Game/Card` | `insertDeck__5CCardG4CARDUssi` | 221 | (comment) near-match: kept as INCLUDE_ASM |
| `Game/Card` | `getAttribute__5CCardG4CARDPPCcN32` | 95 | (comment) near-match: kept as INCLUDE_ASM |
| `Game/Card` | `addCard__5CCardUss` | 224 | (comment) near-match: kept as INCLUDE_ASM |
| `Game/Card` | `canAddCard__5CCardUss` | 167 | (comment) near-match: kept as INCLUDE_ASM |
| `Game/Card` | `isAddCardValid__5CCardUss` | 124 | (comment) near-match: kept as INCLUDE_ASM |
| `Game/Card` | `correctCardRank__5CCardG4CARD` | 212 | (comment) near-match: kept as INCLUDE_ASM |
| `Game/Card` | `isBattleTotalMaxUpCard__5CCardUs` | 38 | (comment) near-match: kept as INCLUDE_ASM |
| `Game/EffectMotion` | `updateAttachEffect___17CEffectMotionCtrli` | 339 | (comment) Close: only case 2's "m_iStep = 0" tails differ - gcc cross-jumps them all into the second body's copy, the original keeps the one in the effect == NULL branch (the other bodies jump there). jump2's cross-jumping (find_cross_jump) merges a "sw $0; j" tail that follows a label after one … |
| `Game/Movement` | `calcPos___9CMovement` | 369 | (comment) near-match (register copies of &end/&norm before the last two detect calls): kept as INCLUDE_ASM |
| `Game/SEMotion` | `update__9CSEMotion` | 57 | (comment) Matches except that `this` and `ctrl` swap registers (s1/s2). |
| `Game/Collision/CollAttack` | `chkGuardSuccess__11CCollAttackP9CCollBody` | 112 | (comment) not matching: the original's last angle wrap branch has an empty delay slot (ours: bc1fl with the abs.s) |
| `Game/Collision/CollAttack` | `chkMutekiSuccess__11CCollAttackP9CCollBody` | 112 | (comment) not matching: the original's last angle wrap branch has an empty delay slot (ours: bc1fl with the abs.s) |
| `Game/Collision/CollAttack` | `chkBackAttack__11CCollAttackP9CCollBody` | 110 | (comment) not matching: the original's last angle wrap branch has an empty delay slot (ours: bc1fl with the abs.s) |
| `Game/Collision/CollBody` | `collideBody__9CCollBodyi` | 232 | (comment) not matching: the original copies the results at the end relative to m_collTmp.m_outWE (ours: relative to m_collTmp, or with the registers of ret1 and m_collTmp swapped) |
| `Game/Collision/CollDisp` | `setAttackLines__9CCollDispP14CCollisionList` | 167 |  |
| `Game/Collision/CollMap` | `collideRide__8CCollMapi` | 224 | (comment) collideRide and collideRideR2 don't match yet. collideRide: the original's `pRideAtari = NULL` store is not a "fixed scalar" to the scheduler (it is ordered before the coll constructor's stores), as if pRideAtari's address had been taken before; every way of doing that here also moves … |
| `Game/Collision/CollMap` | `collideRideR2__8CCollMapR10CCollisionPP5CRidei` | 141 |  |
| `Game/Collision/CollUtilO` | `_detectColumnPoint__9CCollUtilR16CColumnCollisionR15CPointCollision` | 90 | (comment) Near match (the original sets up the distanceLineO arguments after the p0/p1 copies), kept for reference: |
| `Game/Collision/CollUtilO` | `_detectColumnSphere__9CCollUtilR16CColumnCollisionR16CSphereCollisioni` | 105 | (comment) Near match (as _detectColumnPoint, and its distanceSphereO delay slot), kept for reference: |
| `Game/Collision/CollUtilO` | `_detectColumnColumnEz__9CCollUtilR16CColumnCollisionT1` | 121 | (comment) Near match (the load order of the first coordinates and radii), kept for reference: |
| `Game/Atel/AtelCrossFade` | `onCreateTexMemory__Q24ATEL14CAtelCrossFadeiPvi` | 34 | (comment) Near-match: the three TRACE/return paths are tail-merged here (one jal TRACE), separate in the original. |
| `Game/Atel/AtelCrossFade` | `AnimColor__Q24ATEL14CAtelCrossFade` | 69 | (comment) Near-match: the original keeps the temporary vector at sp+0x20 in s0 (a 0x70 frame); here it is at sp. |
| `Game/Atel/AtelCrossFade` | `drawCrossFadeImage__Q24ATEL14CAtelCrossFade` | 188 | (comment) Near-match: the second loop schedules the packet stores differently and hoists the GIF tag constant. |
| `Game/Player/Friend` | `setMagic__Q26Friend7CFriendi` | 1274 |  |
| `Game/Player/NaviMap` | `main__Q25Gauge8CNaviMap` | 425 | (comment) Not matching: in the debug-rotation block (g_dw.player.iTemp == 2) the original keeps PI in $f3 from before the branch and loads 180.0f/-PI in $f2/$f1 (100.0f and -PI share $f1); here 180.0f is hoisted too and the registers of 180.0f/-PI swap (9 lines). Everything else matches; the "deg" … |
| `Game/Player/Player` | `exeWalk__7CPlayer` | 83 | 2 lines differ: the delay slot of jal calcVelocityXZ (the original issues the m_fWalkSpeed store first and puts mov.s $f12,$f1 in the slot; this draft the reverse). The angle-wrap register (r in $f1, not $f13) is fixed by keeping aim_dir live after the call with an empty asm (see the draft); … |
| `Game/Player/Player` | `chkWall__7CPlayeri` | 705 | 11 lines differ (frame, structure, every branch and call match): (1) the first ofs constants - the original loads 0.07f into $f1 and 0.5f into $f0, ours the reverse (pseudo-number order of the two li.s); (2) the join block of the final fix-up: the original loads pos.x ($f0) before pos.z ($f1) in … |
| `Game/Player/Player` | `exeWall__7CPlayer` | 517 | 4 lines differ (size, frame, every branch, call and register allocation match): at the start the original issues `lw m_iSubState` (getSubState) before the `sw zero` of vec.y, ours stores vec.y first (right after the lq of the getRot copy). Found 2026-10-25 from the $LM tags (25 -> 4): the first … |
| `Game/Player/Player` | `isStartLadderJump__7CPlayer` | 120 | everything but the ofs block matches (wrap: r wrapped in place, then dir = r); left: after getPos the original issues lq pos (v1) before lui LADDER_LOCAL_OFS (v0), ours schedules the lui first (sched1 priority: hi->lwc1->swc1 chain) and allocates pos to a1. tried: all orders of pos/ofs statements … |
| `Game/Player/Player` | `exeHang__7CPlayer` | 550 | 23 lines differ, all FP-register/load-order of the two angle wraps: the original loads m_fDir first ($f0) then m_fAnalogViewDir ($f1) for `view - dir + PI` (ours the reverse), and in the drop-off arm it loads dir into $f12 in the branch delay slots and copies it to $f22 after getPos (ours loads … |
| `Game/Player/Player` | `initAttackField__7CPlayeri` | 81 | registers match; only the stores after the g_fGravity load are scheduled differently (the original stores m_JumpState right after the load and m_fJumpGravity before the g_CardBreakTime load; ours stores them later). tried: all 24 orders of the JumpState/CardBreakTime/JumpGravity/JumpSpdY block, … |
| `Game/Player/Player` | `initAttack__7CPlayeri` | 104 | same spot as initAttackField: around the g_fGravity load the original keeps the %hi in v0 (later s0) and stores m_JumpState after the load; ours uses v1 and stores it before, and the g_CardBreakTime %hi moves. The flag block (battle 0x10/0x80000/8, then trg 4) and the ternary m_iComboBoost match. … |
| `Game/Player/Player` | `chkDamage__7CPlayer` | 961 | 20 lines differ, same size; structure, all branches, calls, the 40-entry jump table, frame and strings match. Left, four scheduling/register spots: (1) old_condition (isConditionFlag(0x881803F)): the original loads the flag word into v1 and the mask into v0, ours the reverse; (2) case 6 … |
| `Game/Player/Player` | `setConfuse__7CPlayerii` | 68 | in the set arm the original loads ConditionFlag (v1), li 21000 (v0), PadFlag (a2); this order gets the registers right but schedules the PadFlag load first; source order cond/tq/pad gives the load order but swaps v0/a2 (tq/pad). tried: all 6 statement orders x tq forms (ternary, param default, … |
| `Game/Player/Player` | `chkChargeMove__7CPlayeri` | 91 | only the block before chargeReloadCard is scheduled differently: the original zeroes pos with v1 after the flag 'and' and loads m_pRevolver after the flag store; ours zeroes pos in a0 first and loads m_pRevolver before the store. tried: all orders of setPlayerFlag/pos._q/pos.w (the 5-line ones … |
| `Game/Player/Player` | `exeFooting__7CPlayer` | 293 | all but 4 instructions match (frame, branches, walk block registers). Left: in the analog walk arm the original issues `lw m_pMove` between the `sq t._q` and the `swc1 t.y` and loads `lq t` only after the m_fDir store; ours puts the lq right after the t.y store and the m_pMove load after it. … |
| `Game/Player/PlayerCamera` | `getPlayerCameraDir__Q212PlayerCamera13CPlayerCamera` | 39 | (comment) Only the scheduling of the first block differs: the original zeroes dir (por/sq) before loading m_pInstance (the zero gets $v1, then m_pInstance), ours loads m_pInstance first. |
| `Game/Player/PlayerCamera` | `procFieldNormal__Q212PlayerCamera13CPlayerCamera` | 332 |  |
| `Game/Player/PlayerDRiku` | `initJump2nd__Q22PL12CPlayerDRiku` | 153 | (comment) Best attempt: with a target, the m_vMarkPos/m_hMark copies get swapped registers and rot.y is stored before m_fDir/m_fJumpDir instead of in the branch delay slot (11 lines differ). |
| `Game/Player/PlayerRiku` | `exeStockSkill__Q22PL11CPlayerRiku` | 2420 | (comment) Best attempt (same structure): the CBattleEnemy flag tests get swapped registers, 0x6CD doesn't reload the flag, and 0.0f isn't rematerialized in f20 in 0x6CC. |
| `Game/Player/PlayerRiku` | `setMagic__Q22PL11CPlayerRikuiii` | 520 | (comment) Best attempt (same structure): BULLET_PARAM field stores and the attack-power loads are scheduled differently. |
| `Game/Player/SummonTink` | `initRecover__Q26Summon11CSummonTink` | 37 | (comment) Best attempt: same size; combine folds the Crd() result straight into a3 ("move a3,v0" right after the call), so the effect name's address lands in v0 instead of v1 and the lui/lw are scheduled later. |
| `Game/Enemy/BattleEnemyManager` | `createBattleEnemies___19CBattleEnemyManager` | 149 | (comment) Near match: only `this` and `group` swap registers (s2/s3). |
| `Game/Enemy/EnemyWhitemushroom` | `damage___19CEnemyWhitemushroom` | 184 | (comment) Near-match (3 lines): in case 4 the original loads the vtable pointer for getLockOnPos right after the two TIMEQ stores of setupFade; here the vtable entry loads depend on all six setupFade stores (unknown alias), so the vptr load ranks below them and comes last. Tried (~45 min): … |
| `Game/Enemy/HumanBoss` | `initalize__10CHumanBossiiUi` | 58 |  |
| `Game/Enemy/HumanBoss` | `getAttackCardNum__C10CHumanBossUi` | 43 |  |
| `Game/Enemy/HumanBoss` | `setupAct__10CHumanBoss` | 181 |  |
| `Game/Enemy/HumanBoss` | `enemyCardProc__10CHumanBossPC4CARD` | 125 |  |
| `Game/Enemy/HumanBoss` | `itemCardProc__10CHumanBossPC4CARD` | 242 |  |
| `Game/Enemy/HumanBoss` | `stockActProc__10CHumanBossPC4CARD` | 254 |  |
| `Game/Enemy/HumanBoss` | `normalActProc__10CHumanBossPC4CARD` | 142 |  |
| `Game/Enemy/HumanBoss` | `setStockCard__10CHumanBossPC4CARD` | 88 |  |
| `Game/Enemy/MapEnemy` | `getRandomTargetPos__9CMapEnemyR7FVECTORfi` | 290 | (comment) near-match (one getOutPos() is if-converted; s3/s4 swapped): kept as INCLUDE_ASM |
| `Game/Enemy/MapEnemy` | `hitTest___9CMapEnemy` | 148 | (comment) near-match (the vol store lands in a delay slot): kept as INCLUDE_ASM |
| `Game/Enemy/MapEnemy` | `searchGround___9CMapEnemyfP7FVECTOR` | 124 | (comment) near-match (inRadius and the end angle in swapped FP registers): kept as INCLUDE_ASM |
| `Game/Gimmick/GimmickPlantParent` | `beginStateHitAttack__19CGimmickPlantParent` | 108 | (comment) Only the scheduling before saveStat differs: ours puts "sb m_Stat" before "li a1,1" (delay slot gets li a1). Both issue in the same cycle (sched2 log); the emission order is sched1's, where the store with its dying source goes first. Tried (2026-10-20): saveStat(m_Stat = 1), an INT/BYTE … |
| `Game/Gimmick/GimmickTreasureBox` | `CardMaxWarningMain__19CGimmickTreasureBox` | 257 | (comment) Only the scheduling differs: the original stores CWCP2.ctdID before loading CChatMan::m_pInstance, ours after it (every statement order tried). It blocks the unit's .rodata from source (SOURCE DATA). |
| `Game/Gimmick/GimmickTreasureBox` | `NormalMain__19CGimmickTreasureBox` | 335 | (comment) Matches, but uses string literals and a jump table: needs the unit's .rodata from source, which CardMaxWarningMain blocks. |
| `Game/Gimmick/GimmickTreasureBox` | `EnemyMain__19CGimmickTreasureBox` | 125 | (comment) Matches, but uses a string literal and a jump table: needs the unit's .rodata from source, which CardMaxWarningMain blocks. |
| `Game/Gimmick/GimmickTreasureBox` | `EnemyPop__19CGimmickTreasureBox` | 191 | (comment) Matches, but uses string literals and a jump table: needs the unit's .rodata from source, which CardMaxWarningMain blocks. |
| `Form/DebugWindow` | `disp__12CDebugWindow` | 549 | (comment) Close: the reload registers of the spilled WW/sign (t0 instead of a3) and the loop's line-font position stores (G_XYZ pointer formed before the x store) differ. |
| `Form/DebugWindow` | `main__10CNakCamera` | 118 | (comment) Only the scheduling differs: the m_fProjectionDist store after ViewAngleToProjection is placed after the CDeFocus instance load (the original stores it first, with %hi in v0). |
| `Form/Menu/Camp` | `initCardKind__5CCampiii` | 331 |  |
| `Form/Menu/Camp` | `deckCommonButton__5CCamp` | 270 |  |
| `Form/Menu/Camp` | `deckCommonButton2__5CCamp` | 73 |  |
| `Form/Menu/Camp` | `mapCardWinMenu__5CCamp` | 723 |  |
| `Form/Menu/Camp` | `statusWinMenu__5CCamp` | 392 |  |
| `Form/Menu/Camp` | `setDeckInfo__5CCampi` | 951 |  |
| `Form/Menu/Camp` | `setTechWin__5CCampi` | 551 |  |
| `Form/Menu/Camp` | `setDeckSetWin__5CCampi` | 457 |  |
| `Form/Menu/Camp` | `setMainCmd__5CCampii` | 257 |  |
| `Form/Menu/Camp` | `setStatusWin1__5CCampi` | 429 |  |
| `Form/Menu/Camp` | `setCursor2__5CCampiii` | 184 |  |
| `Form/Menu/Camp` | `searchRoom__5CCampii` | 158 |  |
| `Form/Menu/Camp` | `getCardIndex__5CCampi` | 42 |  |
| `Form/Menu/DecknameEdit` | `chgType__16CDnEditWordPlateii` | 67 | (comment) Scheduling only: same instructions, but the original fills the bSeOn branch's delay slot with the playSE "li a0,3" (ours: "li a1,-1") and allocates the tab label pointer to v1 (ours: a1). |
| `Form/Menu/RoomCreation` | `menuMain__13CRoomCreation` | 1995 | (comment) menuMain: the RCNORMA::NormaSrc by-value arguments (createNorma/replaceNorma) are built from four halfword loads in the original, ld in ours; the register allocation in case 0 follows from that (0x50 bytes shorter). |
| `Form/Menu/Roulette` | `setCardInfo__9CRoulettei` | 270 | (comment) Scheduling only: after the CButton constructor the copy to pButton (s1) is scheduled before the getLayPosition argument setup instead of after it (5 lines differ). |

Totals: 2020 functions (209796 instructions) in 85 units, 131 of them parked; 28 units take a data section from assembly.

Generated by `tools/leftovers.py game`.
<!-- leftovers:game:end -->
