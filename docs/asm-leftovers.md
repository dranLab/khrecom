# Parked INCLUDE_ASM functions and asm data

Units whose remaining functions (or data) hit the time box of the leftovers plan. Each entry: what remains and
the closest result. Revisit after the units with few leftovers are done; lessons from later units may unlock these.

| Unit | Function / data | Remaining difference |
|---|---|---|
| System/Sound/driver/sddrv_ee | `.bss` from asm | `static STR_DATA_CHK str_data_ee[6]` (the ref's only static) is 0x60 of the 0x110 bytes; with it alone everything after moves by 0x80. The rest is unknown. |
| System/Graphic/WorldEnv (also System/Rcf `finalize`, RcfCD `_startCdRead`, `_parseRCF_EntryFile`) | `initialize` | The C is the original's: it matches in about 1 of 6 builds. gcse PRE's hash of anonymous-namespace symbols is taken over the name's characters; ours contain `src_System_Graphic_WorldEnv.cpp` plus a pid-derived random suffix, the original `c__KHCOM_MainApp_System_Graphic_WorldEnv.cppUPaaaa`. Compiling with the original path as the main input name and the suffix `UPaaaa` (pid fixed) matches initialize (2026-11-08, worktree experiment). Needs a build change: deferred by the user 2026-11-08 (later session; scripts in build/gd/). |

## Inventory of the game units

Every game function still INCLUDE_ASM'd, generated from the sources by
`tools/leftovers.py game --update docs/asm-leftovers.md` (the table above is curated by hand).

<!-- leftovers:game:start -->
### Units with INCLUDE_ASM functions

Sorted by the instructions left. `parked`: functions with a C draft under `#if 0`.

| unit | functions left | instr left | parked | data from asm |
|---|---:|---:|---:|---|
| `Game/Player/PlayerDRiku` | 1 | 3473 | 1 |  |
| `Game/Map` | 2 | 1952 | 2 |  |
| `Form/Menu/Camp` | 2 | 1778 | 2 |  |
| `System/Graphic/Effect/libppp/draw/shape` | 2 | 1771 | 2 |  |
| `System/Graphic/Model` | 2 | 1000 | 2 |  |
| `System/Graphic/Effect/libppp/keProg/KeTh` | 1 | 685 | 1 |  |
| `System/Graphic/FrecFont` | 1 | 656 | 1 |  |
| `System/Graphic/HermitePathCamera` | 1 | 632 | 1 |  |
| `Game/Player/PlayerRiku` | 1 | 520 | 1 |  |
| `System/Graphic/Effect/libppp/partMng` | 1 | 292 | 1 |  |
| `System/Graphic/DispList` | 1 | 245 | 1 |  |
| `System/Math3D` | 1 | 234 | 1 |  |
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
| `System/Graphic/DispList` | `_drawOT__19CDispListShadowDrawR5OTtag` | 245 | 59 of 289 lines (2026-11-09): only loop.c on the draw loop over pBuf[j] (line 940) differs: the original keeps `li 12; mult` in the loop, ours strength-reduces it to a pointer, so the registers shift (ours spills i, the original keeps i in s6 and spills hHnd; s6/s7/s8 renamed throughout). … |
| `System/Graphic/FrecFont` | `makePkt__9CFrecFontR6CFntM2` | 656 | 343 of 749 lines (2026-11-09: 338 before the A+D qwords and _setRGBAQ went to Gprim.h's _makeQW) - the draft has the original's structure (all locals in stack slots, contiguous MMI colour sequences as volatile in-place asm, control-code switch, outline/shadow passes), but the spill slots differ: … |
| `System/Graphic/HermitePathCamera` | `updatePos__18CHermitePathCameraR7FVECTORT1fi` | 632 | 88 of 723 lines - same structure; left: the case-2 fStep lives in f12 in the original (ours f2), the hoisted addresses of the BG-check and learp blocks get swapped s-registers (&m_vWPos s0, &vAdd s0 / &OldPos s1 in the original), the mInv address is kept in a register here and recomputed there. … |
| `System/Graphic/Model` | `_disp0_SetPacket__6CModelUiPA3_CiiUliPUl` | 588 | 93 of 655 lines (2026-11-08): the block tree equals the original's .mdebug except before pack #1 (original: a CModel inline, x invisible, y visible, x loaded after y) and the colour-matrix getter (original has no block there but the getter's addiu shape). Still differs: this in s4 / iFlags&2 in … |
| `System/Graphic/Model` | `makePkt__5CBBox` | 412 | 108 of 458 lines (2026-11-08), register allocation: structure matches (8-corner RotTransPers loop with min/max and W range, colour checks, four line-strip GIF packets), but the original keeps this in s4, &m_C0 in s1 and different caller-saved temporaries spilled around RotTransPers; ours also … |
| `System/Graphic/WorldEnv` | `initialize__9CWorldEnv` | 128 | 0-8 of 177 lines (2026-11-08): this draft is the original source. Compiled with the original's own anonymous-namespace name (c:/KHCOM/MainApp/System/Graphic/WorldEnv.cpp + suffix UPaaaa = pid 2588, via a rewritten .ii line marker and a chosen pid) it matches every time; our build names the … |
| `System/Graphic/Effect/libppp/partMng` | `vramcache` | 292 | (comment) Register allocation: the original keeps abBlock (the VLA's address) on the stack and the assert's string addresses in s5/s6; here abBlock gets s7 and bUseBlockNum s3, so the file name's %hi lives in t0 (saved around the call, 16 more bytes of frame). The return blocks are also laid out … |
| `System/Graphic/Effect/libppp/draw/shape` | `pppVu1ShapeDraw` | 133 | 4 of 143 lines (2026-11-08): `or a0,a1,a0` (the call argument) issues before `addiu t6,a1,32` instead of after tag 2's next store, and tag 3's `sb t4,3(t6)` comes after its p[1] store instead of first. With tag 3 written id/next/qwc like the others its stores match, but t3/t4 swap: local-alloc … |
| `System/Graphic/Effect/libppp/draw/shape` | `op_shape_draw` | 1638 | 5 of 1680 lines (2026-11-08): only the join after the six loops differs, registers of three block-local values: the original has `move a0,t1` (the $9 read), `li v1,32`, `andi v0,a1,0x800`, ours `move v1,t1`, `li v0,32`, `andi a0,a1,0x800`. They are the block's only 3 local quantities, so … |
| `System/Graphic/Effect/libppp/keProg/KeTh` | `KeThBP_Update` | 685 | (comment) Best attempt: same code apart from register allocation (the original has flg in s7, pTp in fp and &vf._06 in s6; this gets fp/s6/s7) and the order of a few spill slots (preStep - rootStep, &vf._07, &vf._08, flg & 0x120, &pRP->InVec), which shifts scheduling around them. |
| `System/Rcf` | `finalize__4CRcf` | 111 | 0 or 6 of 153 lines (2026-11-07), random per compile (matched 4 of 6 rebuilds): s_nRcfMapCount/s_nRcfSceneCount's hoisted %hi pseudos tie in global-alloc priority (3 refs / 120 insns each), the tie goes to the lower pseudo, and PRE numbers them in expression-hash bucket order; the SYMBOL_REF hash … |
| `System/RcfCD` | `_startCdRead__Fii` | 41 | 4-8 lines, varying between identical builds (the draft above is the C) - the hoisted lui's of the s_n*_R statics get s5/s6 in an order that changes per compiler run (gcse PRE hash order, memory gcse-pre-hash-order); store orders tried (all worse). Kept asm because the result is not reproducible. |
| `Game/Map` | `init__4CMapii` | 1046 | 87 of 1193 lines, same size - the bbox loop's &b (ApplyMatrix output) is hoisted into s2 here, the original recomputes it per iteration, which shifts the saved registers of xmin/xmax/zmin/zmax and the division block; for/do/ while/goto loop shapes, by-value apply inline and a 10 min permuter run … |
| `Game/Map` | `disp__4CMap` | 906 | 709 of 1308 lines, 44 bytes short - structure as the original (debug clamp, shader modes, group selection by player position, pipelined kick/resultCheckBBox display via group table or grid, sky polygons). The 44 bytes: the original keeps two copies of the "flush the last model" block (group and … |
| `Game/Player/PlayerDRiku` | `exeStockSkill__Q22PL12CPlayerDRiku` | 3473 | 108 of 3890 lines (2026-11-07), same size: cross-jumped tails differ (ours merges the 0x683 setAnimSkillEx+setPlayerFlag(0x100000) tail into 0x6xx's copy, the original keeps 0x683's whole and merges the later copy into the setAnimRsrcEx tail at 2525); the sched2 order around global float loads in … |
| `Game/Player/PlayerRiku` | `setMagic__Q22PL11CPlayerRikuiii` | 520 | (comment) Best attempt (same structure): BULLET_PARAM field stores and the attack-power loads are scheduled differently. |
| `Game/Enemy/HumanBoss` | `setupAct__10CHumanBoss` | 181 | 18 of 205 lines (2026-11-08, third try: unchanged; mechanism found, no honest source for it): one cause left. Diagnostic: a `long m = -1;` as the loop body's first statement with the normal arm storing m gives 8 lines and exactly the original's preheader (li s2,-1 / li s5,1 / li s4,-1, one hoisted … |
| `Form/Menu/Camp` | `setTechWin__5CCampi` | 551 | 36 of 617 lines, same size (2026-11-08; drop the data block after the INCLUDE_ASM to build the draft): s-register roles in the sleight-list loop and one store order. The original increments nTechSelectTop in place before getName (s6, delay slot) and keeps i + 1 in s7 (i in s4, reused for … |
| `Form/Menu/Camp` | `setMapCardWin__5CCampi` | 1227 | 23 of 1407 lines (2026-11-08; drop the data block after the INCLUDE_ASM to build the draft): only the spill slots of seven PRE-hoisted values differ (orig 0x74 &m_pMCInfoNumFnt, 0x78 &m_nMapKind, 0x7c i + 1, 0x80 nKind - 161, 0x84 &m_pMCCard, 0x88 &m_pMCFnt, 0x8c &m_pMCInfoNumCtrl; ours 0x74 … |

Totals: 20 functions (13699 instructions) in 16 units, 20 of them parked; 1 units take a data section from assembly.

Generated by `tools/leftovers.py game`.
<!-- leftovers:game:end -->
