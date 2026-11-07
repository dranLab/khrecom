# Parked INCLUDE_ASM functions and asm data

Units whose remaining functions (or data) hit the time box of the leftovers plan. Each entry: what remains and
the closest result. Revisit after the units with few leftovers are done; lessons from later units may unlock these.

| Unit | Function / data | Remaining difference |
|---|---|---|
| System/Sound/driver/sddrv_ee | `.bss` from asm | `static STR_DATA_CHK str_data_ee[6]` (the ref's only static) is 0x60 of the 0x110 bytes; with it alone everything after moves by 0x80. The rest is unknown. |
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
| `Game/Player/PlayerDRiku` | 1 | 3473 | 1 |  |
| `Form/Menu/Camp` | 3 | 2631 | 3 |  |
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | 3 | 2143 | 3 |  |
| `Form/Menu/RoomCreation` | 1 | 1995 | 1 |  |
| `Game/Map` | 2 | 1952 | 2 |  |
| `System/Graphic/Effect/libppp/draw/shape` | 2 | 1771 | 2 |  |
| `System/Graphic/Model` | 3 | 1153 | 3 |  |
| `System/Graphic/DeFocus` | 2 | 1014 | 2 |  |
| `System/Graphic/Effect/libppp/keProg/KeTh` | 1 | 685 | 1 |  |
| `System/Graphic/FrecFont` | 1 | 656 | 1 |  |
| `Game/Player/Player` | 2 | 654 | 2 |  |
| `System/Graphic/TexUtil` | 4 | 651 | 4 |  |
| `System/Graphic/HermitePathCamera` | 1 | 632 | 1 |  |
| `Form/DebugWindow` | 1 | 549 | 1 |  |
| `System/Graphic/PackMan` | 2 | 540 | 2 |  |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3X` | 1 | 532 | 1 |  |
| `Game/Player/PlayerRiku` | 1 | 520 | 1 |  |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3` | 1 | 514 | 1 |  |
| `System/Graphic/PathData` | 1 | 489 | 1 |  |
| `System/Graphic/Glare` | 1 | 446 | 1 |  |
| `Game/Collision/CollUtil` | 1 | 424 | 1 |  |
| `Game/Collision/CollMap` | 2 | 365 | 2 |  |
| `Game/SceneInfo` | 2 | 314 | 2 |  |
| `System/Graphic/Effect/libppp/partMng` | 1 | 292 | 1 |  |
| `System/Sound/driver/sddrv_ee` | 1 | 262 | 1 | .bss |
| `Game/Collision/CollDisp` | 2 | 262 | 2 |  |
| `System/Graphic/DispList` | 1 | 245 | 1 |  |
| `Game/Enemy/BattleEnemy` | 1 | 245 | 1 |  |
| `System/Math3D` | 1 | 234 | 1 |  |
| `System/Graphic/ModelMan` | 1 | 204 | 1 |  |
| `Game/Atel/Atel` | 1 | 189 | 1 |  |
| `Game/Atel/AtelCrossFade` | 1 | 188 | 1 |  |
| `Game/Enemy/EnemyWhitemushroom` | 1 | 184 | 1 |  |
| `Game/Enemy/HumanBoss` | 1 | 181 | 1 |  |
| `System/Movie/MSVideo` | 1 | 180 | 1 |  |
| `Game/Atel/Avatar` | 1 | 139 | 1 |  |
| `System/TaskMan` | 1 | 135 | 1 |  |
| `System/Graphic/WorldEnv` | 1 | 128 | 1 |  |
| `System/Rcf` | 1 | 111 | 1 |  |
| `Game/Card` | 1 | 95 | 1 |  |
| `System/RcfCD` | 1 | 41 | 1 |  |

Units without a source file: `Game/Player/PlayerAnim`, `Game/Player/PlayerAttack`, `System/Graphic/Effect/libppp/draw/draw_w`, `System/Graphic/Effect/libppp/draw/draw_ws`, `System/Graphic/Effect/libppp/draw/global_w`, `System/Graphic/Effect/libppp/draw/md1`, `System/Graphic/Effect/libppp/draw/sin_t`, `System/Graphic/Effect/libppp/keProg/KeDMat`, `System/Graphic/Effect/libppp/pppfunctbl`, `cod/004FF700`, `cod/2C2B00`, `cod/3FF680`, `common`.

### Parked functions

INCLUDE_ASM'd functions with a C draft in a disabled block. Note: the source's `Parked:` note, or else
the comment next to the draft, marked (comment); shortened to 300 characters.

| unit | function | instr | note |
|---|---|---:|---|
| `System/Math3D` | `InvMatrix__6Math3DR7FMATRIXRC7FMATRIX` | 234 | (comment) Same code apart from register allocation: gcc hoists the 1e-8 constants and a "li 20" out of the outer loop (the original keeps them inside), which moves &c into s1. |
| `System/Graphic/DeFocus` | `_makePktDeFocus__12CDeFocusPrim` | 506 | 83 of 596 lines, same size - structure, branches and calls as the original (debug near/far override, auto near/far from the look distance, FRAME/TEST/ALPHA/PRIM A+D blocks with debug colours, sprites, defaults restore). Left: scheduling inside the A+D blocks (store pair order, the _FRAME0_START … |
| `System/Graphic/DeFocus` | `_makePktBlur__9CBlurPrim` | 508 | 97 of 586 lines, 4 bytes long - structure as the original (debug override, camera-distance Z or 0xFFFF28, A+D header, 6-kind switch of set_tsprite32 patterns, defaults restore). Left: `this` and iZ swap s2/s3 (most lines), the Bak_ registers (original spills the colour, keeps the rate in fp) and … |
| `System/Graphic/DispList` | `_drawOT__19CDispListShadowDrawR5OTtag` | 245 | 62 of 291 lines, 4 bytes long - logic matches (CShadowModel::getSize inline). Left: the original does not strength-reduce the draw loop over pBuf[j] (li 12 + mult every iteration), which frees an s-register: it keeps i in s6 and spills hHnd, ours spills i. Tried: hHnd as a function-level local or … |
| `System/Graphic/FrecFont` | `makePkt__9CFrecFontR6CFntM2` | 656 | 331 of 749 lines - the draft has the original's structure (all locals in stack slots, contiguous MMI colour sequences as volatile in-place asm, control-code switch, outline/shadow passes), but the spill slots differ: the original's G_RGBA locals colExt/colBak/colSave get their (16-aligned) slots … |
| `System/Graphic/Glare` | `makePkt__10CGlarePrim` | 446 |  |
| `System/Graphic/HermitePathCamera` | `updatePos__18CHermitePathCameraR7FVECTORT1fi` | 632 | 88 of 723 lines - same structure; left: the case-2 fStep lives in f12 in the original (ours f2), the hoisted addresses of the BG-check and learp blocks get swapped s-registers (&m_vWPos s0, &vAdd s0 / &OldPos s1 in the original), the mInv address is kept in a register here and recomputed there. … |
| `System/Graphic/Model` | `calcWorldBBox__6CModeli` | 153 | 38 lines off: the original keeps &pCrd->m_World as the strength-reduced loop pointer (s3 += 0xd0, counter counted down), &vMax/&vMin in s8/s7 for the init stores and the VU0 block, and loads the final bbox into f4/f5/f0..f3 in max-then-min order. Tried: pCrd[i] indexing (keeps the counter, 41 … |
| `System/Graphic/Model` | `_disp0_SetPacket__6CModelUiPA3_CiiUliPUl` | 588 | 137 of 659 lines off, same size; mostly registers: the original gives this s5 and iFlags&2 s4 (ours swapped), the bone/light min() selects use movn (an if-form gives that but swaps s0/s1 globally), the no-light branch materializes zero twice. Worked: case 2 with default, (USHORT) mode compares … |
| `System/Graphic/Model` | `makePkt__5CBBox` | 412 | 116 lines off, register allocation: structure matches (8-corner RotTransPers loop with min/max and W range, colour checks, four line-strip GIF packets), but the original keeps this in s4, &m_C0 in s1 and different caller-saved temporaries spilled around RotTransPers. Tried: x[0] > x[1] compare, a … |
| `System/Graphic/ModelMan` | `setupDrawEnv__9CModelMan` | 204 | 58 of 242 lines (65 with vFogParam.set inlined) - FVECTOR::setCall gives the original's set call, but the frame is 16 bytes larger (an extra saved register s3 for &wv_i) and the fog/matrix constant block is scheduled differently. |
| `System/Graphic/PackMan` | `_initFrameEffPacket__Fiiii` | 487 | 2 lines - case 2's two XYZ2 ORs: the original has the X register first (or v0,s2,v0 / or v0,t7,v0), ours puts the shifted Y first; x \| y and y \| x source order, a shared macro for the three cases, case 3's h/nh form with constant 12 and a 15 min permuter run (21k candidates) all stay at 2. … |
| `System/Graphic/PackMan` | `sceVif1PkCloseUpkCode__FP13pacVif1Packet` | 53 | ~25 lines - register allocation: the original keeps wl (numlen >> 8) in t0 and cl in numlen's register a1, ours swaps them (and the t2/t3 numbering follows); the vn != 3 branch layout, the code copy and the shared divide tail match. cl/wl declaration order, signed locals, ternaries and 5 + 15 min … |
| `System/Graphic/PathData` | `dispDebugLine__11CPathVectori` | 489 | 2 lines - in the first 8-step loop the original issues `mov.s $f21,$f20` (the hoisted 0.125f step) after `addiu s3,sp,608` (&Line.P), ours before it (scheduler tie); everything else matches. tried: t init/step forms (for header, t = t + 0.125f, block scope, init before SubVectorXYZ), ib/k forms of … |
| `System/Graphic/TexUtil` | `takeSnapShot__8CTexUtilPCc` | 234 | 72 of 280 lines, 4 bytes short - structure as the original (default display env -> width/height/PSM via (INT)(u_long >> n) & mask, TIM2 file and picture headers, per-line StoreImage + writeFile). Left: the reserved-bytes loop (pointer + own counter in the original), the GsTex0 TBW/PSM/TW/TH writes … |
| `System/Graphic/TexUtil` | `makeCustomClut__8CTexUtilPUlT1ii` | 163 | 20 of 168 lines, same size - up-counting j (the original's reversed count-down), *pSrc read per term (lbu for the low byte) and `iI \| (iI << 16 \| iI << 8)` fixed most of it. Left: the register order of case 1's three channel terms (the original computes the >> 8 term into a1 first) and two … |
| `System/Graphic/TexUtil` | `setupTexInfo__8CTexUtilR7TexInfoPCUcii` | 116 | 10 of 120 lines, same size - the mult between the allocTex call and its test (`r = allocTex(..); nImageSize = w h; if (r < 0)`, the original fills the bltz slot with it; 19 -> 10). Left: _nDistBufW, where the original adds 64 to the (w-1) of an earlier block and shifts (ours (w-1)>>6 + 1; every … |
| `System/Graphic/TexUtil` | `setupTexInfoImd__8CTexUtilR7TexInfoPCUci` | 138 | 93 of 153 lines, 16 bytes short - control flow as the original (tex_cnv PSM 19/20 cases, getTransSize else, allocTex, getTransInfo, TEX0). Left: the original keeps &outInfo._TexInfo/_ClutInfo in s1/s0 across the getTransInfo call (one more saved register, frame 0x90), which renumbers most … |
| `System/Graphic/WorldEnv` | `initialize__9CWorldEnv` | 128 |  |
| `System/Graphic/Effect/libppp/partMng` | `vramcache` | 292 | (comment) Register allocation: the original keeps abBlock (the VLA's address) on the stack and the assert's string addresses in s5/s6; here abBlock gets s7 and bUseBlockNum s3, so the file name's %hi lives in t0 (saved around the call, 16 more bytes of frame). The return blocks are also laid out … |
| `System/Graphic/Effect/libppp/draw/shape` | `pppVu1ShapeDraw` | 133 | 4 of 143 lines (2026-11-07): `or a0,a1,a0` (the call argument) issues before `addiu t6,a1,32` instead of after tag 2's next store, and tag 3's `sb t4,3(t6)` comes after its p[1] store instead of first. With tag 3 written id/next/qwc like the others its stores match, but t3/t4 swap: local-alloc … |
| `System/Graphic/Effect/libppp/draw/shape` | `op_shape_draw` | 1638 | 18 of 1680 lines (2026-11-07), 4 bytes short; and not honest C (V4): the six VU0 loops, the DMA send/wait block and the two OT-add inlines are the original's instruction stream copied from the disassembly (about 1190 asm lines, some 400 of them plain MIPS: loads, stores, branches, address … |
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | `drawShipoly` | 795 | 58 of 815 lines (2026-11-07): register allocation only, same layout: s1 in $f6 and the inner a1 in $f5 (the original the other way); i*16 for linev[.][i + 1] in a1 (original v1), so ret takes v1 (original a0) and r's mfc1 for InterVector, the 6000000 constant and the packet pointer sit one … |
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | `drawDistShipoly` | 648 | 24 of 674 lines (2026-11-07): register allocation in the inner j loop only: the original never takes v0 there (the (int)a for c0[3]/c1[3] in v1, the linev[0][i] reload in v1, the InterVector address pairs a0/v1, the RotTransPersST packet pointer a1/v1, the draw and poly_num reloads a0/a1), ours … |
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | `pppEiDrawDrawShipoly` | 700 | 40 of 743 lines (2026-11-07): Type!=0 copy loop: ours hoists li -1 before lui 0x7000 (the original after it; spl is not an original local, but without it the loop gets two givs and s3/s4 swap: 89 lines); Type 1 loop: centv giv and &dv swap a0/a1; Thin loop: i*16 in v0 (original a2), so the … |
| `System/Graphic/Effect/libppp/keProg/KeTh` | `KeThBP_Update` | 685 | (comment) Best attempt: same code apart from register allocation (the original has flg in s7, pTp in fp and &vf._06 in s6; this gets fp/s6/s7) and the order of a few spill slots (preStep - rootStep, &vf._07, &vf._08, flg & 0x120, &pRP->InVec), which shifts scheduling around them. |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3` | `pppKeShpTail3Draw` | 514 |  |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3X` | `pppKeShpTail3XDraw` | 532 |  |
| `System/Sound/driver/sddrv_ee` | `TsSdVsbPlay` | 262 | 28 of 324 lines (2026-11-07, was 217): every instruction matches except one register swap: the footer/end_flag pointer gets s0 and size/addr2/addr1-addr2 get s2 (original: the reverse). Global-alloc priority: the pointer 14 refs/59 insns = 0.712 beats addr2 4/12 = 0.667; it needs live > 63 or … |
| `System/Movie/MSVideo` | `setLoadImageTagsYX__12CMoviePlayerPUiPviiiii` | 180 | (comment) Not matching: mby - 1 is not hoisted into a register for the inner loop (the original keeps it in s8 and spills i * 16), so the j++ / dsay increments are scheduled differently. |
| `System/Rcf` | `finalize__4CRcf` | 111 | 0 or 6 of 153 lines (2026-11-07), random per compile (matched 4 of 6 rebuilds): s_nRcfMapCount/s_nRcfSceneCount's hoisted %hi pseudos tie in global-alloc priority (3 refs / 120 insns each), the tie goes to the lower pseudo, and PRE numbers them in expression-hash bucket order; the SYMBOL_REF hash … |
| `System/RcfCD` | `_startCdRead__Fii` | 41 | 4-8 lines, varying between identical builds (the draft above is the C) - the hoisted lui's of the s_n*_R statics get s5/s6 in an order that changes per compiler run (gcse PRE hash order, memory gcse-pre-hash-order); store orders tried (all worse). Kept asm because the result is not reproducible. |
| `System/TaskMan` | `update__8CTaskManiii` | 135 | 4 lines - the barrier settles nTQ/bPause (s5/s6). Left: the TaskMainParam stores: the original issues the nTQ store early and _SecondaryArray's after _nSysLag's; a scheduling tie across the clear() block. 2026-11-07: the ref lists no ctor for TaskMainParam, but brace init with these values builds … |
| `Game/Card` | `getAttribute__5CCardG4CARDPPCcN32` | 95 | 30 of 124 lines, registers only: buf's address (3 uses / 38 insns) outranks the CCtdMan::m_pInstance address (4 / 110) in global-alloc priority, the original has them the other way round (s6/s5) and keeps card.m_nKind in a0. Worked: `pLF = buf; *pOutStrAttack = pLF;` (parameter registers, 53 -> … |
| `Game/Map` | `init__4CMapii` | 1046 | 87 of 1193 lines, same size - the bbox loop's &b (ApplyMatrix output) is hoisted into s2 here, the original recomputes it per iteration, which shifts the saved registers of xmin/xmax/zmin/zmax and the division block; for/do/ while/goto loop shapes, by-value apply inline and a 10 min permuter run … |
| `Game/Map` | `disp__4CMap` | 906 | 709 of 1308 lines, 44 bytes short - structure as the original (debug clamp, shader modes, group selection by player position, pipelined kick/resultCheckBBox display via group table or grid, sky polygons). The 44 bytes: the original keeps two copies of the "flush the last model" block (group and … |
| `Game/SceneInfo` | `clear__10CSceneInfo` | 139 | 113 lines, 4 bytes long, frame 144 for 128 - the first run of stores: the original reloads m_pMCWork before each store (the *(T*)& scalar stores give that) but keeps two -1 registers and seven s-registers for the %hi parts; statement orders (incl. the lui order) and 7 min of permuter (10k … |
| `Game/SceneInfo` | `rollbackEventDoorState__10CSceneInfoi` | 175 | ~35 lines - the outer room loop: the original re-materializes the s_CurrentFloor address for the room (mtlo/madd) and for the count each iteration; ours shares one base. Door/state/norma logic matches; room addressing forms and 5 min of permuter (42 -> 38) did not get it. |
| `Game/Collision/CollDisp` | `setAttackLines__9CCollDispP14CCollisionList` | 167 | 62 of 178 lines - logic and jump table match; `this`, the collision and the end position take other registers. Tried (2026-11-02): pColl/type declaration order (62), inline getCollision()/getType() calls without locals as the ref's locals suggest (104). |
| `Game/Collision/CollDisp` | `setBodyLines__9CCollDispP14CCollisionList` | 95 | 38 of 105 lines (2026-11-07), same size: the s registers come out rotated by one through the loop (this s1 for s2, ...). The ref has no pColl/type locals but dropping either is worse (53-66); swapping their declarations: 38-39. |
| `Game/Collision/CollMap` | `collideRide__8CCollMapi` | 224 | (comment) collideRide and collideRideR2 don't match yet. collideRide: the original's `pRideAtari = NULL` store is not a "fixed scalar" to the scheduler (it is ordered before the coll constructor's stores), as if pRideAtari's address had been taken before; every way of doing that here also moves … |
| `Game/Collision/CollMap` | `collideRideR2__8CCollMapR10CCollisionPP5CRidei` | 141 |  |
| `Game/Collision/CollUtil` | `_detectMCD__9CCollUtilR10CCollision` | 424 | _detectMCD: 35 of 458 lines, same size (2026-11-07) - statements in marker order, chip loop as for (pPrimIdx = pPrimIdx0; iCnt; iCnt--) with its own iArea (this now in s2 as the original), vf4 load via an inline. Left: ours hoists iArea0*4 out of the retry loop and spills it (104(sp); the original … |
| `Game/Atel/Atel` | `setScreenFader__5CAtelifi` | 189 | 37 of 206 lines, 16 bytes short - logic, switch and jump table match. Left: the original computes the layer as `beqz; b then (li v0,1); else: b end (li 1); then: li 10` (a jump-evaluated boolean threaded away) and keeps nLayer in s1; ours branches once, which rotates the s-registers … |
| `Game/Atel/AtelCrossFade` | `drawCrossFadeImage__Q24ATEL14CAtelCrossFade` | 188 | (comment) Near-match: the second loop schedules the packet stores differently and hoists the GIF tag constant. |
| `Game/Atel/Avatar` | `RotateAvatarMain__Q24ATEL7CAvatar` | 139 | 8 lines - the first isOverAvatarRotTimeLimit() comes out as xori; the original builds bRes with li 1 + movz (its weak copy is xori too). Inline/caller forms and 8 min of permuter (9k candidates) did not change it. The VU0 add/clip block (FVECTOR::addRot in the ref) is written out here: through an … |
| `Game/Player/Player` | `exeHang__7CPlayer` | 550 | 23 lines differ, all FP-register/load-order of the two angle wraps: the original loads m_fDir first ($f0) then m_fAnalogViewDir ($f1) for `view - dir + PI` (ours the reverse), and in the drop-off arm it loads dir into $f12 in the branch delay slots and copies it to $f22 after getPos (ours loads … |
| `Game/Player/Player` | `initAttack__7CPlayeri` | 104 | same spot as initAttackField: around the g_fGravity load the original keeps the %hi in v0 (later s0) and stores m_JumpState after the load; ours uses v1 and stores it before, and the g_CardBreakTime %hi moves. The flag block (battle 0x10/0x80000/8, then trg 4) and the ternary m_iComboBoost match. … |
| `Game/Player/PlayerDRiku` | `exeStockSkill__Q22PL12CPlayerDRiku` | 3473 | 108 of 3890 lines (2026-11-07), same size: cross-jumped tails differ (ours merges the 0x683 setAnimSkillEx+setPlayerFlag(0x100000) tail into 0x6xx's copy, the original keeps 0x683's whole and merges the later copy into the setAnimRsrcEx tail at 2525); the sched2 order around global float loads in … |
| `Game/Player/PlayerRiku` | `setMagic__Q22PL11CPlayerRikuiii` | 520 | (comment) Best attempt (same structure): BULLET_PARAM field stores and the attack-power loads are scheduled differently. |
| `Game/Enemy/BattleEnemy` | `initEnemyParam__12CBattleEnemyiii` | 245 | 34 lines - the Riku-game scan loop: the original places the getRsrcData() ternary before the inline getExtraParam's found block and keeps the parameter in s0 (ours: s2, blocks swapped); the rest (seek loops, relocation, floor mapping, basic param) matches; tried: while/for/continue loop forms, … |
| `Game/Enemy/EnemyWhitemushroom` | `damage___19CEnemyWhitemushroom` | 184 | 3 of 211 lines (2026-11-07): case 4, the vptr load for getLockOnPos comes after all six setupFade stores, the original has it right after the two TIMEQ stores (sw 284/280) and before swc1 268, sw 288, sw 228, sw 272; tried: all store orders, flag forms, barriers, temporaries, call forms, pointer … |
| `Game/Enemy/HumanBoss` | `setupAct__10CHumanBoss` | 181 | 18 of 205 lines (2026-11-07): the normal arm's -1 (sMin/sMax) is its own hoisted register in the original (li s4,-1 after li s5,1), ours shares the compares' -1 (s2): loop.c (-dL) combines the arm's HImode -1 movable with the SImode compare constant ("matches 84"); the rest follows (14/12 store … |
| `Form/DebugWindow` | `disp__12CDebugWindow` | 549 | (comment) Close: the reload registers of the spilled WW/sign (t0 instead of a3) and the loop's line-font position stores (G_XYZ pointer formed before the x store) differ. |
| `Form/Menu/Camp` | `setTechWin__5CCampi` | 551 | 53 of 621 lines (2026-11-07; tools/drafts.py's "build failed" is the data block after the INCLUDE_ASM, which also defines this function's statics: drop it to build the draft): in the sleight-list loop the original reloads the spilled pTechCateTbl at the loop head and recomputes … |
| `Form/Menu/Camp` | `setMapCardWin__5CCampi` | 1227 | same size, 33 of 1409 lines differ (re-measured 2026-11-07; tools/drafts.py's "build failed" is the data block after the INCLUDE_ASM: drop it to build the draft): the stack slots of the spilled hoisted addresses (&m_pMCInfoNumCtrl, &m_pMCInfoNumFnt, &m_pMCFnt, &m_pMCCard, &m_nMapKind, i + 1) are … |
| `Form/Menu/Camp` | `setWorldmapCursor__5CCamp` | 853 | 35 of 939 lines differ (62 before _NORMA_PIECES; re-measured 2026-11-07). Left: pDoorInfo and nNI (not nNorma) have s4/s3 for s3/s4: global-alloc priority (-dl: pDoorInfo 28 refs over 187 insns = 0.60, nNI 22 over 121 = 0.73, so nNI is allocated first and takes s3); the original allocates … |
| `Form/Menu/RoomCreation` | `menuMain__13CRoomCreation` | 1995 | (comment) menuMain: 78 of 2211 lines, 8 bytes short (92 and 0x50 short before _NORMA_PIECES, 2026-11-07): the by-value NormaSrc arguments are written out now; left: case 0's registers (v1 + s7 where the original keeps a0, s4/s6/s8 from there). To enable this draft, also drop the data block after … |

Totals: 58 functions (27418 instructions) in 41 units, 58 of them parked; 1 units take a data section from assembly.

Generated by `tools/leftovers.py game`.
<!-- leftovers:game:end -->
