# Parked INCLUDE_ASM functions and asm data

Units whose remaining functions (or data) hit the time box of the leftovers plan. Each entry: what remains and
the closest result. Revisit after the units with few leftovers are done; lessons from later units may unlock these.

| Unit | Function / data | Remaining difference |
|---|---|---|
| System/Sound/driver/sddrv_ee | `.bss` from asm | `static STR_DATA_CHK str_data_ee[6]` (the ref's only static) is 0x60 of the 0x110 bytes; with it alone everything after moves by 0x80. The rest is unknown. |
| System/Graphic/WorldEnv (also System/Rcf `finalize`, RcfCD `_startCdRead`, `_parseRCF_EntryFile`) | `initialize` | The C is the original's: it matches in about 1 of 6 builds. gcse PRE's hash of anonymous-namespace symbols is taken over the name's characters; ours contain `src_System_Graphic_WorldEnv.cpp` plus a pid-derived random suffix, the original `c__KHCOM_MainApp_System_Graphic_WorldEnv.cppUPaaaa`. Compiling with the original path as the main input name and the suffix `UPaaaa` (pid fixed) matches initialize (2026-11-08, worktree experiment). Needs a build change: deferred by the user 2026-11-08 (later session; scripts in build/gd/). |
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
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | 3 | 2143 | 3 |  |
| `Form/Menu/RoomCreation` | 1 | 1995 | 1 |  |
| `Game/Map` | 2 | 1952 | 2 |  |
| `Form/Menu/Camp` | 2 | 1778 | 2 |  |
| `System/Graphic/Effect/libppp/draw/shape` | 2 | 1771 | 2 |  |
| `System/Graphic/DeFocus` | 2 | 1014 | 2 |  |
| `System/Graphic/Model` | 2 | 1000 | 2 |  |
| `System/Graphic/Effect/libppp/keProg/KeTh` | 1 | 685 | 1 |  |
| `System/Graphic/FrecFont` | 1 | 656 | 1 |  |
| `System/Graphic/HermitePathCamera` | 1 | 632 | 1 |  |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3X` | 1 | 532 | 1 |  |
| `Game/Player/PlayerRiku` | 1 | 520 | 1 |  |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3` | 1 | 514 | 1 |  |
| `System/Graphic/Glare` | 1 | 446 | 1 |  |
| `Game/Collision/CollMap` | 2 | 365 | 2 |  |
| `Game/SceneInfo` | 2 | 314 | 2 |  |
| `System/Graphic/Effect/libppp/partMng` | 1 | 292 | 1 |  |
| `System/Graphic/DispList` | 1 | 245 | 1 |  |
| `System/Math3D` | 1 | 234 | 1 |  |
| `System/Graphic/ModelMan` | 1 | 204 | 1 |  |
| `Game/Enemy/HumanBoss` | 1 | 181 | 1 |  |
| `System/Graphic/WorldEnv` | 1 | 128 | 1 |  |
| `System/Rcf` | 1 | 111 | 1 |  |
| `System/RcfCD` | 1 | 41 | 1 |  |

Units with all functions in C but data from asm: `System/Sound/driver/sddrv_ee` (.bss).

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
| `System/Graphic/Model` | `_disp0_SetPacket__6CModelUiPA3_CiiUliPUl` | 588 | 93 of 655 lines (2026-11-08): the block tree equals the original's .mdebug except before pack #1 (original: a CModel inline, x invisible, y visible, x loaded after y) and the colour-matrix getter (original has no block there but the getter's addiu shape). Still differs: this in s4 / iFlags&2 in … |
| `System/Graphic/Model` | `makePkt__5CBBox` | 412 | 108 of 458 lines (2026-11-08), register allocation: structure matches (8-corner RotTransPers loop with min/max and W range, colour checks, four line-strip GIF packets), but the original keeps this in s4, &m_C0 in s1 and different caller-saved temporaries spilled around RotTransPers; ours also … |
| `System/Graphic/ModelMan` | `setupDrawEnv__9CModelMan` | 204 | 58 of 242 lines (65 with vFogParam.set inlined) - FVECTOR::setCall gives the original's set call, but the frame is 16 bytes larger (an extra saved register s3 for &wv_i) and the fog/matrix constant block is scheduled differently. |
| `System/Graphic/WorldEnv` | `initialize__9CWorldEnv` | 128 | 0-8 of 177 lines (2026-11-08): this draft is the original source. Compiled with the original's own anonymous-namespace name (c:/KHCOM/MainApp/System/Graphic/WorldEnv.cpp + suffix UPaaaa = pid 2588, via a rewritten .ii line marker and a chosen pid) it matches every time; our build names the … |
| `System/Graphic/Effect/libppp/partMng` | `vramcache` | 292 | (comment) Register allocation: the original keeps abBlock (the VLA's address) on the stack and the assert's string addresses in s5/s6; here abBlock gets s7 and bUseBlockNum s3, so the file name's %hi lives in t0 (saved around the call, 16 more bytes of frame). The return blocks are also laid out … |
| `System/Graphic/Effect/libppp/draw/shape` | `pppVu1ShapeDraw` | 133 | 4 of 143 lines (2026-11-08): `or a0,a1,a0` (the call argument) issues before `addiu t6,a1,32` instead of after tag 2's next store, and tag 3's `sb t4,3(t6)` comes after its p[1] store instead of first. With tag 3 written id/next/qwc like the others its stores match, but t3/t4 swap: local-alloc … |
| `System/Graphic/Effect/libppp/draw/shape` | `op_shape_draw` | 1638 | 18 of 1680 lines (2026-11-08), 4 bytes short; and not honest C (V4): the six VU0 loops, the DMA send/wait block and the two OT-add inlines are the original's instruction stream copied from the disassembly (about 1190 asm lines, some 400 of them plain MIPS: loads, stores, branches, address … |
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | `drawShipoly` | 795 | 58 of 815 lines (2026-11-08): register allocation only, same instructions; block tree, locals and their registers as the original's .mdebug except s1 $f6/a1 $f5 (original $f5/$f6: global-alloc priority 4*24/186 for s1 below 3*12/69 for a1; writing a1 = alpha1 before a0 = alpha0 gives the … |
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | `drawDistShipoly` | 648 | 24 of 674 lines (2026-11-08): register allocation in the inner j loop only, block tree and named locals' registers as the original's .mdebug except the RotTransPersST stv copies (a1/v1 in the original, ours a0/v0): the original never takes v0 there (the (int)a for c0[3]/c1[3] in v1, the … |
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | `pppEiDrawDrawShipoly` | 700 | 40 of 743 lines (2026-11-08): Type!=0 copy loop: ours hoists li -1 before lui 0x7000 (the original after it: in ours loop pass 1 moves the -1 before the giv init is emitted; spl, a pointer local the .mdebug cannot show at function level, keeps the original's single giv: without it two givs and … |
| `System/Graphic/Effect/libppp/keProg/KeTh` | `KeThBP_Update` | 685 | (comment) Best attempt: same code apart from register allocation (the original has flg in s7, pTp in fp and &vf._06 in s6; this gets fp/s6/s7) and the order of a few spill slots (preStep - rootStep, &vf._07, &vf._08, flg & 0x120, &pRP->InVec), which shifts scheduling around them. |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3` | `pppKeShpTail3Draw` | 514 |  |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3X` | `pppKeShpTail3XDraw` | 532 |  |
| `System/Rcf` | `finalize__4CRcf` | 111 | 0 or 6 of 153 lines (2026-11-07), random per compile (matched 4 of 6 rebuilds): s_nRcfMapCount/s_nRcfSceneCount's hoisted %hi pseudos tie in global-alloc priority (3 refs / 120 insns each), the tie goes to the lower pseudo, and PRE numbers them in expression-hash bucket order; the SYMBOL_REF hash … |
| `System/RcfCD` | `_startCdRead__Fii` | 41 | 4-8 lines, varying between identical builds (the draft above is the C) - the hoisted lui's of the s_n*_R statics get s5/s6 in an order that changes per compiler run (gcse PRE hash order, memory gcse-pre-hash-order); store orders tried (all worse). Kept asm because the result is not reproducible. |
| `Game/Map` | `init__4CMapii` | 1046 | 87 of 1193 lines, same size - the bbox loop's &b (ApplyMatrix output) is hoisted into s2 here, the original recomputes it per iteration, which shifts the saved registers of xmin/xmax/zmin/zmax and the division block; for/do/ while/goto loop shapes, by-value apply inline and a 10 min permuter run … |
| `Game/Map` | `disp__4CMap` | 906 | 709 of 1308 lines, 44 bytes short - structure as the original (debug clamp, shader modes, group selection by player position, pipelined kick/resultCheckBBox display via group table or grid, sky polygons). The 44 bytes: the original keeps two copies of the "flush the last model" block (group and … |
| `Game/SceneInfo` | `clear__10CSceneInfo` | 139 | 113 lines, 4 bytes long, frame 144 for 128 - the first run of stores: the original reloads m_pMCWork before each store (the *(T*)& scalar stores give that) but keeps two -1 registers and seven s-registers for the %hi parts; statement orders (incl. the lui order) and 7 min of permuter (10k … |
| `Game/SceneInfo` | `rollbackEventDoorState__10CSceneInfoi` | 175 | ~35 lines - the outer room loop: the original re-materializes the s_CurrentFloor address for the room (mtlo/madd) and for the count each iteration; ours shares one base. Door/state/norma logic matches; room addressing forms and 5 min of permuter (42 -> 38) did not get it. |
| `Game/Collision/CollMap` | `collideRide__8CCollMapi` | 224 | (comment) collideRide and collideRideR2 don't match yet. collideRide: the original's `pRideAtari = NULL` store is not a "fixed scalar" to the scheduler (it is ordered before the coll constructor's stores), as if pRideAtari's address had been taken before; every way of doing that here also moves … |
| `Game/Collision/CollMap` | `collideRideR2__8CCollMapR10CCollisionPP5CRidei` | 141 |  |
| `Game/Player/PlayerDRiku` | `exeStockSkill__Q22PL12CPlayerDRiku` | 3473 | 108 of 3890 lines (2026-11-07), same size: cross-jumped tails differ (ours merges the 0x683 setAnimSkillEx+setPlayerFlag(0x100000) tail into 0x6xx's copy, the original keeps 0x683's whole and merges the later copy into the setAnimRsrcEx tail at 2525); the sched2 order around global float loads in … |
| `Game/Player/PlayerRiku` | `setMagic__Q22PL11CPlayerRikuiii` | 520 | (comment) Best attempt (same structure): BULLET_PARAM field stores and the attack-power loads are scheduled differently. |
| `Game/Enemy/HumanBoss` | `setupAct__10CHumanBoss` | 181 | 18 of 205 lines (2026-11-08, re-measured after the BattleEnemy.h cleanup: unchanged): one cause left. The normal arm's -1 (sMin/sMax, 656/657) is its own pass-1 movable in the original (li s2,-1 / li s5,1 / li s4,-1: compares + stock arm s2, normal arm s4); ours combines it into the compares' … |
| `Form/Menu/Camp` | `setTechWin__5CCampi` | 551 | 36 of 617 lines, same size (2026-11-08; drop the data block after the INCLUDE_ASM to build the draft): s-register roles in the sleight-list loop and one store order. The original increments nTechSelectTop in place before getName (s6, delay slot) and keeps i + 1 in s7 (i in s4, reused for … |
| `Form/Menu/Camp` | `setMapCardWin__5CCampi` | 1227 | 23 of 1407 lines (2026-11-08; drop the data block after the INCLUDE_ASM to build the draft): only the spill slots of seven PRE-hoisted values differ (orig 0x74 &m_pMCInfoNumFnt, 0x78 &m_nMapKind, 0x7c i + 1, 0x80 nKind - 161, 0x84 &m_pMCCard, 0x88 &m_pMCFnt, 0x8c &m_pMCInfoNumCtrl; ours 0x74 … |
| `Form/Menu/RoomCreation` | `menuMain__13CRoomCreation` | 1995 | (comment) menuMain: 78 of 2211 lines, 8 bytes short (92 and 0x50 short before _NORMA_PIECES, 2026-11-07): the by-value NormaSrc arguments are written out now; left: case 0's registers (v1 + s7 where the original keeps a0, s4/s6/s8 from there). To enable this draft, also drop the data block after … |

Totals: 34 functions (21226 instructions) in 25 units, 34 of them parked; 1 units take a data section from assembly.

Generated by `tools/leftovers.py game`.
<!-- leftovers:game:end -->
