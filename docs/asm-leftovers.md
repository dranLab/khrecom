# Parked INCLUDE_ASM functions and asm data

Units whose remaining functions (or data) hit the time box of the leftovers plan. Each entry: what remains and
the closest result. Revisit after the units with few leftovers are done; lessons from later units may unlock these.

| Unit | Function / data | Remaining difference |
|---|---|---|
| Game/Collision/CollUtilO | `.rodata` from asm | Same as Ride: the split puts its `.rodata` at 0x4893B4, 4 mod 8, starting with the previous unit's vtable tail. |
| System/Sound/driver/sddrv_ee | `.bss` from asm | `static STR_DATA_CHK str_data_ee[6]` (the ref's only static) is 0x60 of the 0x110 bytes; with it alone everything after moves by 0x80. The rest is unknown. |
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
| `Game/Player/PlayerDRiku` | 3 | 5073 | 3 |  |
| `Form/Menu/Camp` | 7 | 4599 | 7 |  |
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | 3 | 2143 | 3 | .bss .data .rodata |
| `Form/Menu/RoomCreation` | 1 | 1995 | 1 |  |
| `Game/Map` | 2 | 1952 | 2 |  |
| `System/Graphic/Effect/libppp/draw/shape` | 2 | 1771 | 2 |  |
| `Game/Player/Player` | 3 | 1615 | 3 |  |
| `System/Graphic/Model` | 5 | 1277 | 5 |  |
| `Game/Player/Friend` | 1 | 1274 | 1 |  |
| `System/Graphic/PathData` | 2 | 1075 | 2 |  |
| `Game/CardRevolver` | 5 | 1062 | 5 |  |
| `Game/Enemy/HumanBoss` | 6 | 1030 | 6 |  |
| `System/Graphic/DeFocus` | 2 | 1014 | 2 |  |
| `System/Graphic/FrecFont` | 2 | 908 | 2 |  |
| `Game/Collision/CollUtil` | 3 | 781 | 3 | .rodata |
| `System/Graphic/2d/2DSprObj` | 2 | 763 | 2 |  |
| `System/Graphic/Effect/libppp/keProg/KeTh` | 1 | 685 | 1 |  |
| `System/Graphic/TexUtil` | 4 | 651 | 4 |  |
| `System/Graphic/HermitePathCamera` | 1 | 632 | 1 |  |
| `System/Rcf` | 5 | 552 | 5 |  |
| `Form/DebugWindow` | 1 | 549 | 1 |  |
| `System/Graphic/PackMan` | 2 | 540 | 2 |  |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3X` | 1 | 532 | 1 |  |
| `Game/Atel/Avatar` | 3 | 531 | 3 |  |
| `Game/Player/PlayerRiku` | 1 | 520 | 1 |  |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3` | 1 | 514 | 1 |  |
| `System/Graphic/Glare` | 1 | 446 | 1 |  |
| `System/Memoca` | 3 | 442 | 3 |  |
| `System/Graphic/Gprim` | 3 | 426 | 3 |  |
| `Game/Player/NaviMap` | 1 | 425 | 1 |  |
| `System/Sound/driver/sddrv_ee` | 2 | 403 | 2 | .bss |
| `System/RcfCD` | 3 | 390 | 3 |  |
| `Game/Collision/CollMap` | 2 | 365 | 2 |  |
| `Game/Collision/CollAttack` | 3 | 334 | 3 |  |
| `Game/Player/PlayerCamera` | 1 | 332 | 1 |  |
| `Game/Enemy/BattleEnemy` | 2 | 327 | 2 |  |
| `Game/SceneInfo` | 2 | 314 | 2 |  |
| `System/Graphic/Effect/libppp/partMng` | 1 | 292 | 1 |  |
| `Game/Enemy/MapEnemy` | 1 | 290 | 1 |  |
| `Game/Collision/CollDisp` | 2 | 262 | 2 |  |
| `System/Graphic/DispList` | 1 | 245 | 1 |  |
| `System/Math3D` | 1 | 234 | 1 |  |
| `System/Wnd` | 1 | 225 | 1 |  |
| `System/TaskMan` | 2 | 217 | 2 |  |
| `System/Graphic/ModelMan` | 1 | 204 | 1 |  |
| `Game/Atel/Atel` | 1 | 189 | 1 |  |
| `Game/Atel/AtelCrossFade` | 1 | 188 | 1 |  |
| `Game/Enemy/EnemyWhitemushroom` | 1 | 184 | 1 |  |
| `System/Movie/MSVideo` | 1 | 180 | 1 |  |
| `System/Graphic/Effect/libppp/pppPart` | 1 | 168 | 1 |  |
| `System/Graphic/WorldEnv` | 1 | 128 | 1 |  |
| `Form/Menu/SaveLoad` | 1 | 126 | 1 |  |
| `Form/Menu/JiminyMemo` | 1 | 122 | 1 |  |
| `Game/Collision/CollUtilO` | 1 | 121 | 1 | .rodata |
| `System/Graphic/Effect/effect` | 1 | 105 | 1 |  |
| `Game/Card` | 1 | 95 | 1 |  |
| `Game/CardBattle` | 1 | 89 | 1 |  |
| `Game/Player/SummonTink` | 1 | 37 | 1 |  |
| `System/Graphic/Anim` | 1 | 33 | 1 |  |
| `System/Graphic/2d/2DCard` | 1 | 30 | 1 |  |
| `System/dlmalloc` | 1 | 14 | 1 |  |

Units with all functions in C but data from asm: `Game/Collision/Ride` (.rodata).

Units without a source file: `Game/Player/PlayerAnim`, `Game/Player/PlayerAttack`, `System/Graphic/Effect/libppp/draw/draw_w`, `System/Graphic/Effect/libppp/draw/draw_ws`, `System/Graphic/Effect/libppp/draw/global_w`, `System/Graphic/Effect/libppp/draw/md1`, `System/Graphic/Effect/libppp/draw/sin_t`, `System/Graphic/Effect/libppp/keProg/KeDMat`, `System/Graphic/Effect/libppp/pppfunctbl`, `cod/004FF700`, `cod/2C2B00`, `cod/3FF680`, `common`.

### Parked functions

INCLUDE_ASM'd functions with a C draft in a disabled block. Note: the source's `Parked:` note, or else
the comment next to the draft, marked (comment); shortened to 300 characters.

| unit | function | instr | note |
|---|---|---:|---|
| `System/Math3D` | `InvMatrix__6Math3DR7FMATRIXRC7FMATRIX` | 234 | (comment) Same code apart from register allocation: gcc hoists the 1e-8 constants and a "li 20" out of the outer loop (the original keeps them inside), which moves &c into s1. |
| `System/Wnd` | `init__9CImageWndRt7CPointT1ZiRt6CSizeT1ZiT1T2PCcfG6G_RGBAT7i` | 225 | 14 of 239 lines - the flip block (w2 = w; w = 0 under fgFlip bits, 4 USHORTs) gets other registers (original w a1, h a2, w2 a3, h2 t0; ours w v1, h a1, h2 a2) and the T0..T3 stores another order. Tried (2026-11-05: decl/assignment forms of w, h, w2, h2, INT locals): declaration orders (ref's w, h, … |
| `System/Memoca` | `_setupIconSys__FPCc` | 199 |  |
| `System/Memoca` | `_McCheckDataFileExist__FiPCc` | 99 |  |
| `System/Memoca` | `_McWriteIcons__FPCc` | 144 |  |
| `System/dlmalloc` | `destroy_mspace` | 14 | (comment) Not matching. The original is: lw seg.next; beqz; lw next (A); bnezl L, lw (entry test + body); L: bnezl L, lw. That is "if (sp) { sp = sp->next; while (sp) sp = sp->next; }" with the loop exit test duplicated, but jump2 cross-jumps the peeled "lw; beqz" into the loop (the insn before … |
| `System/Graphic/Anim` | `_hokanRot__Ffff` | 33 | (comment) Near-match: the angle difference is copied into another FP register before it is limited |
| `System/Graphic/DeFocus` | `_makePktDeFocus__12CDeFocusPrim` | 506 | 83 of 596 lines, same size - structure, branches and calls as the original (debug near/far override, auto near/far from the look distance, FRAME/TEST/ALPHA/PRIM A+D blocks with debug colours, sprites, defaults restore). Left: scheduling inside the A+D blocks (store pair order, the _FRAME0_START … |
| `System/Graphic/DeFocus` | `_makePktBlur__9CBlurPrim` | 508 | 97 of 586 lines, 4 bytes long - structure as the original (debug override, camera-distance Z or 0xFFFF28, A+D header, 6-kind switch of set_tsprite32 patterns, defaults restore). Left: `this` and iZ swap s2/s3 (most lines), the Bak_ registers (original spills the colour, keeps the rate in fp) and … |
| `System/Graphic/DispList` | `_drawOT__19CDispListShadowDrawR5OTtag` | 245 | 62 of 291 lines, 4 bytes long - logic matches (CShadowModel::getSize inline). Left: the original does not strength-reduce the draw loop over pBuf[j] (li 12 + mult every iteration), which frees an s-register: it keeps i in s6 and spills hHnd, ours spills i. Tried: hHnd as a function-level local or … |
| `System/Graphic/FrecFont` | `setupFontTex__9CFrecFontRC8RsrcInfoRC4CBuf` | 252 | (comment) Matches except for the order of two loop-invariant CSA values that loop.c hoists ((nCSA + 6) << 56 is computed before (nCSA + 2) << 56 in the original). |
| `System/Graphic/FrecFont` | `makePkt__9CFrecFontR6CFntM2` | 656 | 331 of 749 lines - the draft has the original's structure (all locals in stack slots, contiguous MMI colour sequences as volatile in-place asm, control-code switch, outline/shadow passes), but the spill slots differ: the original's G_RGBA locals colExt/colBak/colSave get their (16-aligned) slots … |
| `System/Graphic/Glare` | `makePkt__10CGlarePrim` | 446 |  |
| `System/Graphic/HermitePathCamera` | `updatePos__18CHermitePathCameraR7FVECTORT1fi` | 632 | 88 of 723 lines - same structure; left: the case-2 fStep lives in f12 in the original (ours f2), the hoisted addresses of the BG-check and learp blocks get swapped s-registers (&m_vWPos s0, &vAdd s0 / &OldPos s1 in the original), the mInv address is kept in a register here and recomputed there. … |
| `System/Graphic/Model` | `getApplyMatrixBBox__FR7FVECTORT0RC7FMATRIXRC7FVECTORT3` | 88 | 3 lines off: the original stores vMN[1].z/.y through the &vMN[1] operand register (t1), ours through sp. Worked: vMN[i].set(MIN/MAX, ...) (default w = 1.0f), one asm statement with all operands, D_ labels for the +-1.7e38 constants. Tried: field stores, chained assignment, pointer local; permuter … |
| `System/Graphic/Model` | `calcWorldBBox__6CModeli` | 153 | 38 lines off: the original keeps &pCrd->m_World as the strength-reduced loop pointer (s3 += 0xd0, counter counted down), &vMax/&vMin in s8/s7 for the init stores and the VU0 block, and loads the final bbox into f4/f5/f0..f3 in max-then-min order. Tried: pCrd[i] indexing (keeps the counter, 41 … |
| `System/Graphic/Model` | `checkBBox__FRC7FMATRIXPC4BBOX` | 36 | 12 lines, same size (the tail as resultCheckBBox) - registers only: the original has ret v0, the scratchpad pointer v1, clip a0, as if the three overlapped in priority order. Ours keeps the pointer local to the first block, so it takes v0 and clip v1. With "r"(v) in the join barrier v goes global … |
| `System/Graphic/Model` | `_disp0_SetPacket__6CModelUiPA3_CiiUliPUl` | 588 | 137 of 659 lines off, same size; mostly registers: the original gives this s5 and iFlags&2 s4 (ours swapped), the bone/light min() selects use movn (an if-form gives that but swaps s0/s1 globally), the no-light branch materializes zero twice. Worked: case 2 with default, (USHORT) mode compares … |
| `System/Graphic/Model` | `makePkt__5CBBox` | 412 | 116 lines off, register allocation: structure matches (8-corner RotTransPers loop with min/max and W range, colour checks, four line-strip GIF packets), but the original keeps this in s4, &m_C0 in s1 and different caller-saved temporaries spilled around RotTransPers. Tried: x[0] > x[1] compare, a … |
| `System/Graphic/ModelMan` | `setupDrawEnv__9CModelMan` | 204 | 58 of 242 lines (65 with vFogParam.set inlined) - FVECTOR::setCall gives the original's set call, but the frame is 16 bytes larger (an extra saved register s3 for &wv_i) and the fog/matrix constant block is scheduled differently. |
| `System/Graphic/PackMan` | `_initFrameEffPacket__Fiiii` | 487 | 2 lines - case 2's two XYZ2 ORs: the original has the X register first (or v0,s2,v0 / or v0,t7,v0), ours puts the shifted Y first; x \| y and y \| x source order, a shared macro for the three cases, case 3's h/nh form with constant 12 and a 15 min permuter run (21k candidates) all stay at 2. … |
| `System/Graphic/PackMan` | `sceVif1PkCloseUpkCode__FP13pacVif1Packet` | 53 | ~25 lines - register allocation: the original keeps wl (numlen >> 8) in t0 and cl in numlen's register a1, ours swaps them (and the t2/t3 numbering follows); the vn != 3 branch layout, the code copy and the shared divide tail match. cl/wl declaration order, signed locals, ternaries and 5 + 15 min … |
| `System/Graphic/PathData` | `getNearPath__11CPathVectorR7FVECTORPfT2PiRC7FVECTORPC7FVECTORif` | 586 | 4 lines (2026-11-06, was 9: fNearT = 0.0f right after iNearStep, statement changes from tools/permuter.py; its declaration shuffle was noise); left: the hoisted 0.0f compare constant is a copy of fNearT here, mtc1 zero in the original, and one mov.s $f27,$f3 sits in another arm. Earlier: 9 lines - … |
| `System/Graphic/PathData` | `dispDebugLine__11CPathVectori` | 489 | 2 lines - in the first 8-step loop the original issues `mov.s $f21,$f20` (the hoisted 0.125f step) after `addiu s3,sp,608` (&Line.P), ours before it (scheduler tie); everything else matches. tried: t init/step forms (for header, t = t + 0.125f, block scope, init before SubVectorXYZ), ib/k forms of … |
| `System/Graphic/TexUtil` | `takeSnapShot__8CTexUtilPCc` | 234 | 72 of 280 lines, 4 bytes short - structure as the original (default display env -> width/height/PSM via (INT)(u_long >> n) & mask, TIM2 file and picture headers, per-line StoreImage + writeFile). Left: the reserved-bytes loop (pointer + own counter in the original), the GsTex0 TBW/PSM/TW/TH writes … |
| `System/Graphic/TexUtil` | `makeCustomClut__8CTexUtilPUlT1ii` | 163 | 20 of 168 lines, same size - up-counting j (the original's reversed count-down), *pSrc read per term (lbu for the low byte) and `iI \| (iI << 16 \| iI << 8)` fixed most of it. Left: the register order of case 1's three channel terms (the original computes the >> 8 term into a1 first) and two … |
| `System/Graphic/TexUtil` | `setupTexInfo__8CTexUtilR7TexInfoPCUcii` | 116 | 10 of 120 lines, same size - the mult between the allocTex call and its test (`r = allocTex(..); nImageSize = w h; if (r < 0)`, the original fills the bltz slot with it; 19 -> 10). Left: _nDistBufW, where the original adds 64 to the (w-1) of an earlier block and shifts (ours (w-1)>>6 + 1; every … |
| `System/Graphic/TexUtil` | `setupTexInfoImd__8CTexUtilR7TexInfoPCUci` | 138 | 93 of 153 lines, 16 bytes short - control flow as the original (tex_cnv PSM 19/20 cases, getTransSize else, allocTex, getTransInfo, TEX0). Left: the original keeps &outInfo._TexInfo/_ClutInfo in s1/s0 across the getTransInfo call (one more saved register, frame 0x90), which renumbers most … |
| `System/Graphic/WorldEnv` | `initialize__9CWorldEnv` | 128 |  |
| `System/Graphic/Gprim` | `makePkt__6CZFill` | 109 | 8 lines - two scheduling ties: the original loads 0x206 before the shared zero for the PRIM/PRMODE A+D pair, and strength-reduces i 64 + 63 with its increment at the loop end while i++ sits inside the SCISSOR expression (ours increments both there); this draft fixed the hoisted-constant order (XYZ … |
| `System/Graphic/Gprim` | `makePkt__8CRotSprt` | 173 | the corner setup before the two calcVert calls (load/store order of b, c and ab: the original keeps a separate copy of m_P0.y for ab.set's _y parameter) and the register numbering that follows; a reference to m_P1 (not in the ref's locals) reproduces the original's s0 = &m_P1 read of m_P1.y in the … |
| `System/Graphic/Gprim` | `_makePkt_TriangleFan__FP6QWdataP7G_STRIPT1i` | 144 | 2 lines - the skip loop is solved (as the other fans); iW in a local, declared before iDir, gives the original's s7/s8 (without it iDir loses to the hoisted 0x8000, priority 833 vs 862). Left: its li sits before the entry test, the original sets it after, with the &vDir addiu; a guard + do/while … |
| `System/Graphic/2d/2DCard` | `replaceNorma__7C2DCardGQ27RCNORMA8NormaSrci` | 30 | (comment) Not matching: the original passes inNorma to createNorma as four lhu from the parameter's stack slot combined with dsll/or into a1 (extract_bit_field in 16-bit pieces); ours copies the struct to an 8-aligned temporary and loads it with one ld. No struct shape tried (bitfields, arrays, … |
| `System/Graphic/2d/2DSprObj` | `initForSeq__Q22D29C2DSprObjPQ22D213SEQ_ANIMATIONPQ22D210LAY_SCREEN` | 325 | 14 of 340 lines (2026-11-06, was 20: fVec2 is the brace init), all in the sway block. Markers: fVec.set(four sway values) (Coord.h:730) and m_vSway[i].set() (Coord.h:200); the stores come out sway0 x/y, fVec z/w, sway1, fVec x/y. Writing fVec.set + m_vSway[i].set(fVec...) fixes the source shape … |
| `System/Graphic/2d/2DSprObj` | `move__Q22D29C2DSprObj` | 438 | 25 of 465 lines - only the vertex loop differs: the MMI colour multiply's 0xFF splat is a TImode constant here (`por v0,zero,zero` before `li`, the original only does `li`), the pmulth/tmp registers come out v1/v0 instead of a0/v1, the vertex-loop pointers get other registers and lCol is loaded … |
| `System/Graphic/Effect/effect` | `Setup__11EFFECT_DATAPUxi` | 105 | 4 of 133 lines, same size - with i reused for the DPD loop and the ParNum TRACE before `p += 16` (draft below) the loop registers match; left: the two TRACE header reads go through data (s3) in the original, ours through p (s1), and `p += 16` sits before the ParNum TRACE there. The placeholder's … |
| `System/Graphic/Effect/libppp/partMng` | `vramcache` | 292 | (comment) Register allocation: the original keeps abBlock (the VLA's address) on the stack and the assert's string addresses in s5/s6; here abBlock gets s7 and bUseBlockNum s3, so the file name's %hi lives in t0 (saved around the call, 16 more bytes of frame). The return blocks are also laid out … |
| `System/Graphic/Effect/libppp/pppPart` | `pppSetMatrix` | 168 | 3 lines (24 before the pEtc local read after the first matrix scale, decomp-permuter) - v0/v1 register choice for the scale factors of SCALE_VECTOR_XYZ (the original rotates v0/v1). |
| `System/Graphic/Effect/libppp/draw/shape` | `pppVu1ShapeDraw` | 133 |  |
| `System/Graphic/Effect/libppp/draw/shape` | `op_shape_draw` | 1638 |  |
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | `drawShipoly` | 795 | (comment) register allocation differs; also short float loops (bc1t hazard nop lost by fix_short_loops) |
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | `drawDistShipoly` | 648 | (comment) register allocation of loop temporaries (v0/v1/a0) and a few float regs differs |
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | `pppEiDrawDrawShipoly` | 700 | (comment) 44 lines differ: Type!=0 copy loop hoists li -1 before lui 0x7000 (original after; spl is not an original local), Type 1 loop swaps a0/a1 (centv giv vs &dv) |
| `System/Graphic/Effect/libppp/keProg/KeTh` | `KeThBP_Update` | 685 | (comment) Best attempt: same code apart from register allocation (the original has flg in s7, pTp in fp and &vf._06 in s6; this gets fp/s6/s7) and the order of a few spill slots (preStep - rootStep, &vf._07, &vf._08, flg & 0x120, &pRP->InVec), which shifts scheduling around them. |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3` | `pppKeShpTail3Draw` | 514 |  |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3X` | `pppKeShpTail3XDraw` | 532 |  |
| `System/Sound/driver/sddrv_ee` | `TsSdSepPlayDBG` | 141 | 4 lines with the barrier above (2026-11-06): the lbu of byte 13 vs the hoisted lui/li of the scan loop; sep_size forms 4-11 lines. Earlier: Register allocation: sep_size's bytes and the sep_param scan loop get other temporaries (pointer/counter swapped). |
| `System/Sound/driver/sddrv_ee` | `TsSdVsbPlay` | 262 | (comment) Register allocation: addr is spilled instead of kept in s8 (one more stack slot), so most registers differ. |
| `System/Movie/MSVideo` | `setLoadImageTagsYX__12CMoviePlayerPUiPviiiii` | 180 | (comment) Not matching: mby - 1 is not hoisted into a register for the inner loop (the original keeps it in s8 and spills i * 16), so the j++ / dsay increments are scheduled differently. |
| `System/Rcf` | `finalize__4CRcf` | 111 | (comment) Near-match: register allocation of two statics and a loop-bound reload |
| `System/Rcf` | `_parseRCF_EntryFile__FUii` | 123 | 28 of 169 lines, same size - the 0x7F (sound list) branch's CBuf/RsrcInfo setup: the original loads s_nReadGroup/s_nLinkOwnerGroup/s_bSceneManagedMode into t2/t0/t1 before the CBuf vtable lui and keeps the constant 1 in a3; ours schedules the vtable first (t3/t1/t2, t0). Rest matches with … |
| `System/Rcf` | `_atoi__FPCc` | 71 | _atoi: 7 of 78 lines, same size - hex/decimal paths and the isupper branch (_tolower) as the original; left: the decimal loop: the original's top is not aligned and p++ stays last. Real loop forms give the align nop and move p++ (for (;; p++), while, tests in the condition); a goto loop drops it … |
| `System/Rcf` | `_atolen__FPCc` | 73 | _atolen: 14 of 81 lines, 4 bytes short - _atoi's shape (one return, goto exits, '$' test first with the 0x case inside `if (*p != '$')`); left: the same isupper branch-vs-movz in _hexValue as _atoi, and the original's block order (0x case jumps over the '$' block to `nRet = 2`, p = lpszInStr + 2 … |
| `System/Rcf` | `loadNextCategoryInfo__4CRcfiPciPi` | 174 | (comment) Near-match: register allocation |
| `System/RcfCD` | `_startCdRead__Fii` | 41 | 4-8 lines, varying between identical builds (the draft above is the C) - the hoisted lui's of the s_n*_R statics get s5/s6 in an order that changes per compiler run (gcse PRE hash order, memory gcse-pre-hash-order); store orders tried (all worse). Kept asm because the result is not reproducible. |
| `System/RcfCD` | `_analyzeCdMemberHeader__FRPUc` | 190 | 31 lines (draft above) - the member-header walk's pointer advance is if-converted (movn s2,s4,s4) here and branchy in the original, and the alloc/assert blocks are laid out differently; not iterated further. |
| `System/RcfCD` | `_addRsrcFileToDB__FUiiRPUc` | 159 | 4 lines (draft above) - `addiu s0,sp,32` (&info) and the _nLinkID load are scheduled before the group test in the original, after it here. tried: permuter 10 min, 33k candidates: no gain. 2026-11-07: ours has the addiu in the beq slot, the original fills that slot with move a0,s3 from the arm. One … |
| `System/TaskMan` | `update__8CTaskManiii` | 135 | 4 lines - the barrier settles nTQ/bPause (s5/s6). Left: the TaskMainParam stores: the original issues the nTQ store early and _SecondaryArray's after _nSysLag's; a scheduling tie across the clear() block. |
| `System/TaskMan` | `_invokePrimary_Main__8CTaskManR13TaskMainParamR5CTask` | 82 |  |
| `Game/Card` | `getAttribute__5CCardG4CARDPPCcN32` | 95 | 30 of 124 lines, registers only: buf's address (3 uses / 38 insns) outranks the CCtdMan::m_pInstance address (4 / 110) in global-alloc priority, the original has them the other way round (s6/s5) and keeps card.m_nKind in a0. Worked: `pLF = buf; *pOutStrAttack = pLF;` (parameter registers, 53 -> … |
| `Game/CardBattle` | `testPlayingCard__Q210CardBattle11CCardBattleUi` | 89 | 2 lines, the final return only - the original computes 4 into v0 and movn's 1 from v1 when iDuelOK (the != path sets v0 = 1 in the bne delay slot, one exit); this form branches (beqz iDuelOK) instead of the movn. `if (!=) return 1; return !iDuelOK ? 4 : 1;` gives the exact movn block, but jump2 … |
| `Game/CardRevolver` | `main__Q210CardBattle13CCardRevolver` | 321 | 27 lines off, only the TRACE argument loads in the three card dumps: the original loads uiFlag into v1 and uiOwner last into v0 (the card pointer's register), ours the other way round. Worked: state switch with explicit returns, m_pCards[i] indexed directly (pBC temp moved the pointer to t4), … |
| `Game/CardRevolver` | `rollDeck__Q210CardBattle13CCardRevolveriii` | 248 | 5 lines off, the direction `a`: the original computes it as li t3,1 / slt v0,v1,a1 / movz t3,v1,v0 with v1 = -1 set in both predecessors of the join, then negates num with bltzl; ours branches (bgez). Worked: one `i` for all three loops (s0), !m_iMode-first rc/pCards select, hand++ after setHand, … |
| `Game/CardRevolver` | `changeMode__Q210CardBattle13CCardRevolverii` | 101 | 12 lines off, all in `limit`: the original loads m_nTotal[m_iMode] (lhu + sll 16) above the m_iMode branch and sign-extends (sra 16) only in that arm (interblock-scheduled split extension); ours branches first and uses lh. Worked: `if (inMode == -1) ^= 1; else =`, `!old_mode ? m_pCards : … |
| `Game/CardRevolver` | `exeReload__Q210CardBattle13CCardRevolver` | 127 | 14 lines off: register allocation and latch order of the first loop - the original keeps i in t0 and the card offset in a2 (decremented first in the latch, in the continue paths' delay slots), ours the reverse; the second loop's i likewise. Worked (from 48 bytes over to same size): separate … |
| `Game/CardRevolver` | `initAction__Q210CardBattle13CCardRevolver` | 265 | ~165 lines off, mostly register allocation: ours keeps one more value live across calls (the getStock() == -1 value is CSE'd into an s-register for the later getHand() test), so two flags spill instead of one (bReloadOC) and every flag moves one register. Structure found: decEnchantTime inside … |
| `Game/Map` | `init__4CMapii` | 1046 | 87 of 1193 lines, same size - the bbox loop's &b (ApplyMatrix output) is hoisted into s2 here, the original recomputes it per iteration, which shifts the saved registers of xmin/xmax/zmin/zmax and the division block; for/do/ while/goto loop shapes, by-value apply inline and a 10 min permuter run … |
| `Game/Map` | `disp__4CMap` | 906 | 709 of 1308 lines, 44 bytes short - structure as the original (debug clamp, shader modes, group selection by player position, pipelined kick/resultCheckBBox display via group table or grid, sky polygons). The 44 bytes: the original keeps two copies of the "flush the last model" block (group and … |
| `Game/SceneInfo` | `clear__10CSceneInfo` | 139 | ~110 lines - the first run of stores: the original reloads m_pMCWork before each store (the *(T*)& scalar stores give that) but keeps two -1 registers and seven s-registers for the %hi parts; statement orders (incl. the lui order) and 7 min of permuter (10k candidates, 104 -> 98) did not get closer. |
| `Game/SceneInfo` | `rollbackEventDoorState__10CSceneInfoi` | 175 | ~35 lines - the outer room loop: the original re-materializes the s_CurrentFloor address for the room (mtlo/madd) and for the count each iteration; ours shares one base. Door/state/norma logic matches; room addressing forms and 5 min of permuter (42 -> 38) did not get it. |
| `Game/Collision/CollAttack` | `chkGuardSuccess__11CCollAttackP9CCollBody` | 112 | (comment) not matching: the original's last angle wrap branch has an empty delay slot (ours: bc1fl with the abs.s) |
| `Game/Collision/CollAttack` | `chkMutekiSuccess__11CCollAttackP9CCollBody` | 112 | (comment) not matching: the original's last angle wrap branch has an empty delay slot (ours: bc1fl with the abs.s) |
| `Game/Collision/CollAttack` | `chkBackAttack__11CCollAttackP9CCollBody` | 110 | (comment) not matching: the original's last angle wrap branch has an empty delay slot (ours: bc1fl with the abs.s) |
| `Game/Collision/CollDisp` | `setAttackLines__9CCollDispP14CCollisionList` | 167 | 62 of 178 lines - logic and jump table match; `this`, the collision and the end position take other registers. Tried (2026-11-02): pColl/type declaration order (62), inline getCollision()/getType() calls without locals as the ref's locals suggest (104). |
| `Game/Collision/CollDisp` | `setBodyLines__9CCollDispP14CCollisionList` | 95 | 38 of 105 lines, same size - structure matches (case 4/5 column first, `case 2: default:` sphere); left: the s-registers are rotated (original this s1, collision s0, type s2, fg s3, &m_inWE s4) and the sphere arm recomputes &m_inWE where ours reuses it. Tried: this vs a pThis copy, decl orders, … |
| `Game/Collision/CollMap` | `collideRide__8CCollMapi` | 224 | (comment) collideRide and collideRideR2 don't match yet. collideRide: the original's `pRideAtari = NULL` store is not a "fixed scalar" to the scheduler (it is ordered before the coll constructor's stores), as if pRideAtari's address had been taken before; every way of doing that here also moves … |
| `Game/Collision/CollMap` | `collideRideR2__8CCollMapR10CCollisionPP5CRidei` | 141 |  |
| `Game/Collision/CollUtil` | `detectMCInit__9CCollUtilP7MC_CHIPi` | 169 | 14 of 185 lines, same size - the helpers and the i/iPrimIdx loop follow the ref's locals. Left: reload's t8/t9 for the two qmfc2 (ours v0: iTmp read before written, "+r" as in EffectMan's _lengthVf4, but here it then outranks the pChip/&vCk copies, 28 lines) and with them d0 in f2 and the results … |
| `Game/Collision/CollUtil` | `_detectMCD__9CCollUtilR10CCollision` | 424 | _detectMCD: 146 of 467 lines, 8 bytes short - structure as the original (MCD path of _detectOCD: area chip table walk, two allocSpad asserts, same segment loop); ret declared after i for its spill slot, the vf4 asm before the vCkS reference. Left: this in s3 instead of s2 (most lines), the retry … |
| `Game/Collision/CollUtil` | `detectMap__9CCollUtilP7FVECTORT1` | 188 | 39 of 213 lines, 4 bytes long - two passes, renew test and info merge as the original; left: the pPrimInfo2/i init order and an extra delay-slot load at the second pass, the attr byte's `andi` copy (a0) and the return-1 block (the original sets v0 after the m_pDetectMatA/ChipA/NormalA stores). |
| `Game/Collision/CollUtilO` | `_detectColumnColumnEz__9CCollUtilR16CColumnCollisionT1` | 121 | 7 of 125 lines - the original loads columnColl1's radius after columnColl0's (sched1 hoists ours above the z subtraction, so c0.z/c1.R swap f3/f4). Tried: radius statement order, r1 split, direct/SubVector differences, permuter 9 min (23k candidates, no gain). |
| `Game/Atel/Atel` | `setScreenFader__5CAtelifi` | 189 | 37 of 206 lines, 16 bytes short - logic, switch and jump table match. Left: the original computes the layer as `beqz; b then (li v0,1); else: b end (li 1); then: li 10` (a jump-evaluated boolean threaded away) and keeps nLayer in s1; ours branches once, which rotates the s-registers … |
| `Game/Atel/AtelCrossFade` | `drawCrossFadeImage__Q24ATEL14CAtelCrossFade` | 188 | (comment) Near-match: the second loop schedules the packet stores differently and hoists the GIF tag constant. |
| `Game/Atel/Avatar` | `_$_Q24ATEL7CAvatar` | 95 | 6 lines - the implicit lineY/lineX destructor calls get s5/s4 instead of s4/s5: their address pseudos tie on priority (2 refs / 17 insns) and ours numbers lineX first (lreg dump); no body form changed it. The draft uses the full classes of Gprim.h (inline ~C3DLine/~CGPrim). |
| `Game/Atel/Avatar` | `onMove__Q24ATEL7CAvatariPvi` | 297 | 1 line (was 53) - the final test: the original has `slti 3`, then `bne s6, 0` twice; the switch's range node gives bltz for the first. The empty asm in case 0 settled the registers: pV 3 refs / 13 insns outranked p 14 / 187 by a hair. An INT local keeps slti but the second zero test folds (even … |
| `Game/Atel/Avatar` | `RotateAvatarMain__Q24ATEL7CAvatar` | 139 | 8 lines - the first isOverAvatarRotTimeLimit() comes out as xori; the original builds bRes with li 1 + movz (its weak copy is xori too). Inline/caller forms and 8 min of permuter (9k candidates) did not change it. The VU0 add/clip block (FVECTOR::addRot in the ref) is written out here: through an … |
| `Game/Player/Friend` | `setMagic__Q26Friend7CFriendi` | 1274 |  |
| `Game/Player/NaviMap` | `main__Q25Gauge8CNaviMap` | 425 | (comment) Not matching: in the debug-rotation block (g_dw.player.iTemp == 2) the original keeps PI in $f3 from before the branch and loads 180.0f/-PI in $f2/$f1 (100.0f and -PI share $f1); here 180.0f is hoisted too and the registers of 180.0f/-PI swap (9 lines). Everything else matches; the "deg" … |
| `Game/Player/Player` | `exeHang__7CPlayer` | 550 | 23 lines differ, all FP-register/load-order of the two angle wraps: the original loads m_fDir first ($f0) then m_fAnalogViewDir ($f1) for `view - dir + PI` (ours the reverse), and in the drop-off arm it loads dir into $f12 in the branch delay slots and copies it to $f22 after getPos (ours loads … |
| `Game/Player/Player` | `initAttack__7CPlayeri` | 104 | same spot as initAttackField: around the g_fGravity load the original keeps the %hi in v0 (later s0) and stores m_JumpState after the load; ours uses v1 and stores it before, and the g_CardBreakTime %hi moves. The flag block (battle 0x10/0x80000/8, then trg 4) and the ternary m_iComboBoost match. … |
| `Game/Player/Player` | `chkDamage__7CPlayer` | 961 | 8 lines differ (2026-11-06, was 20: both vel brace-initialized, m_fDir store between brace and copy; markers at 9456/9593: por, sq 0x30, swc1 y 0x34, lq 0x30, sq 0x20 all on one line, so a temporary copied straight into vec; (FVECTOR){..} does not compile at the first site, braces at both sites … |
| `Game/Player/PlayerCamera` | `procFieldNormal__Q212PlayerCamera13CPlayerCamera` | 332 |  |
| `Game/Player/PlayerDRiku` | `setAttack__Q22PL12CPlayerDRiku` | 451 | 7 lines off, registers only: in the card branch the original keeps %hi(g_pBattleCardList) in t0 and the shared constant 1 (m_iAttackNo == 1, _upKind's shift) in a2, ours t1 and t0. Worked: CPlayer::setAttack's form, the player-flag 0x80000 arm first, `pre == 7 \|\| isAir()` for the last hit, case 7 … |
| `Game/Player/PlayerDRiku` | `exeStockSkill__Q22PL12CPlayerDRiku` | 3473 | structurally complete (35 cases in the original order, Riku's 0x6cc-0x6f9 group renumbered to 0x712-0x73f), same frame, 8 bytes long, 173 of 3903 lines off: case 1's attack-ID switch keeps a compare for the empty case 0x690 in the original; cross-jumped tails differ (0x67c's flag-and-return tail, … |
| `Game/Player/PlayerDRiku` | `setMagic__Q22PL12CPlayerDRikuiii` | 1149 | same structure and size within 8 bytes, 310 of 1363 lines off: the BULLET_PARAM field stores and the member loads feeding them are scheduled differently in every case (the original loads m_AtkKind, m_AtkAttr, m_iAttackPower in that order and stores them together), plus registers in the stock-skill … |
| `Game/Player/PlayerRiku` | `setMagic__Q22PL11CPlayerRikuiii` | 520 | (comment) Best attempt (same structure): BULLET_PARAM field stores and the attack-power loads are scheduled differently. |
| `Game/Player/SummonTink` | `initRecover__Q26Summon11CSummonTink` | 37 | (comment) Best attempt: same size; combine folds the Crd() result straight into a3 ("move a3,v0" right after the call), so the effect name's address lands in v0 instead of v1 and the lui/lw are scheduled later. |
| `Game/Enemy/BattleEnemy` | `setupFlick__12CBattleEnemy` | 82 | the 30-degree constant (0.5235988f) is loaded once into $f20 and reused for sinf, cosf and calcOrbitSpeedFromDistance in ours; the original reloads it (lui/ori/mtc1) before each call and keeps the sinf result in $f20 instead (vel.y stored after cosf); tried: field stores vs vel.set(), … |
| `Game/Enemy/BattleEnemy` | `initEnemyParam__12CBattleEnemyiii` | 245 | 34 lines - the Riku-game scan loop: the original places the getRsrcData() ternary before the inline getExtraParam's found block and keeps the parameter in s0 (ours: s2, blocks swapped); the rest (seek loops, relocation, floor mapping, basic param) matches; tried: while/for/continue loop forms, … |
| `Game/Enemy/EnemyWhitemushroom` | `damage___19CEnemyWhitemushroom` | 184 | (comment) Near-match (3 lines): in case 4 the original loads the vtable pointer for getLockOnPos right after the two TIMEQ stores of setupFade; here the vtable entry loads depend on all six setupFade stores (unknown alias), so the vptr load ranks below them and comes last. Tried (~45 min): … |
| `Game/Enemy/HumanBoss` | `getAttackCardNum__C10CHumanBossUi` | 43 | (comment) near match, 20 lines (2026-11-05, was 27): the goto-built outer loop reloads iCardCount/pCards like the original (a for loop hoists them); left: the original hoists the m_pEnemyParam load once (t2), ours reloads it too. The ref lists card/j at function level and no getEnemyParam inline |
| `Game/Enemy/HumanBoss` | `setupCardRevolver__10CHumanBoss` | 334 | 21 of 371 lines - the param pointer is carried in a register copy through the build and enemy-card loops (the original's `move a3,v1`; ours reloads getEnemyParam() in the enemy-card loop body). Levers that got it here: the id % 10000 in a local, UINT search index, one INT i per loop, `cards = … |
| `Game/Enemy/HumanBoss` | `setupAct__10CHumanBoss` | 181 | 21 of 371 lines - the param pointer is carried in a register copy through the build and enemy-card loops (the original's `move a3,v1`; ours reloads getEnemyParam() in the enemy-card loop body). Levers that got it here: the id % 10000 in a local, UINT search index, one INT i per loop, `cards = … |
| `Game/Enemy/HumanBoss` | `itemCardProc__10CHumanBossPC4CARD` | 242 | (comment) near match: one rollDeck tail is cross-jumped (the original keeps both copies) |
| `Game/Enemy/HumanBoss` | `normalActProc__10CHumanBossPC4CARD` | 142 | (comment) near match, 7 lines: both setDoBattleAct/useCard tails kept (goto ret from the first, 2026-11-05; was 20); left: s5/s6 swap of the -1 constant and &tmp[count] (global-alloc tie), one move a0,s0 after the div |
| `Game/Enemy/HumanBoss` | `setStockCard__10CHumanBossPC4CARD` | 88 | (comment) near match: delay slots and one cross-jumped return differ |
| `Game/Enemy/MapEnemy` | `getRandomTargetPos__9CMapEnemyR7FVECTORfi` | 290 | (comment) near-match (one getOutPos() is if-converted; s3/s4 swapped): kept as INCLUDE_ASM |
| `Form/DebugWindow` | `disp__12CDebugWindow` | 549 | (comment) Close: the reload registers of the spilled WW/sign (t0 instead of a3) and the loop's line-font position stores (G_XYZ pointer formed before the x store) differ. |
| `Form/Menu/Camp` | `initCardKind__5CCampiii` | 331 | 26 of 342 lines differ: loop.c hoists the CCard::m_pCardInfo load of the 3-deck copy out of the outer loop (the original reloads it each iteration: the stack-array stores must alias it there). tried: destination pointer, direct m_pCardInfo->_deck[i] without _ref, pointer post-increment, _ref on … |
| `Form/Menu/Camp` | `setDeckInfo__5CCampi` | 951 | 3 of 1048 lines differ in the last loop: v0/v1 swapped for the nColSeq != 0x73 compare (nColSeq is spilled; reload loads it into v0 where the original has the 0x73 in v0). The addu order (this + sel 2) is fixed by `(BYTE*)this + (m_nKindCategorySelect << 1) + 6464` (4 -> 3). tried: without _ref … |
| `Form/Menu/Camp` | `setTechWin__5CCampi` | 551 | 53 of 621 lines differ: in the sleight-list loop the original spills pTechCateTbl and nTechCateCnt and recomputes &pTechCateTbl[nTechSelectTop] from the spilled pointer every iteration; ours strength-reduces it into an s-register (the same giv difference as setMainCmd's font loop). tried: the … |
| `Form/Menu/Camp` | `setMapCardWin__5CCampi` | 1227 | same size, 33 of 1409 lines differ: the stack slots of the spilled hoisted addresses (&m_pMCInfoNumCtrl, &m_pMCInfoNumFnt, &m_pMCFnt, &m_pMCCard, &m_nMapKind, i + 1) are assigned in another order (orig 0x74 NumFnt, 0x78 MapKind, 0x7c i+1, 0x84 MCCard, 0x88 MCFnt, 0x8c NumCtrl; ours ascending by … |
| `Form/Menu/Camp` | `setWorldmapCursor__5CCamp` | 853 | 62 of 941 lines differ. The RCNORMA::NormaSrc argument of createNorma is built from four lhu of its stack copy in the original (extract_bit_field pieces), one ld in ours - the open issue of RoomCreation::menuMain and C2DCard::replaceNorma; the s3/s4 and spill-slot (0x64/0x68) swaps around it … |
| `Form/Menu/Camp` | `setMainCmd__5CCampii` | 257 | 17 of 276 lines, 8 bytes short (40 before each loop got its own i, as in the ref; was 81, 53 before 2026-11-05: _ref(m_pMainCmdFnt)[i] in the font-replacing loop keeps its own address computation): with the ref's function-level `INT i` plus a block `INT i` in the else branch `this` is in s0 as in … |
| `Form/Menu/Camp` | `setStatusWin1__5CCampi` | 429 | 13 of 487 lines differ (was 23): vec.set() (the ref's inlined `FVECTOR *this`) and a precomputed `m_bPlayerKind + 0xF` (permuter) fixed the rest; left: the original keeps &vec in v0 (y/w/z stores and the later fShift = vec.z load through it), so pPlayerManager is reloaded from its spill slot for … |
| `Form/Menu/JiminyMemo` | `ScrollBarChange__11CJiminyMemoPPQ22D210C2DSeqCtrlffiii` | 122 | 22 lines: ours hoists the 2.0f of case 2 out of the loop (an extra saved f24, frame +16, 0.01f moves from f23 to f24); the original loads it inside case 2 like ScrollBarInit does. Everything else matches. tried: y expression forms (temp, parenthesised, vec = vPos + .y), case order, INT i at the … |
| `Form/Menu/RoomCreation` | `menuMain__13CRoomCreation` | 1995 | (comment) menuMain: the RCNORMA::NormaSrc by-value arguments (createNorma/replaceNorma) are built from four halfword loads in the original, ld in ours; the register allocation in case 0 follows from that (0x50 bytes shorter). |
| `Form/Menu/SaveLoad` | `exeMessageDlg__11CSaveLoadUI` | 126 | the case-1 OK tail (m_nDlgResult = 1; CSound::playSE(2, -1, 64)) is cross-jumped into the identical tail of case 2/3 in ours, the original keeps both copies; also the lockPadControl(TRUE) tail loads into v1 (original v0) and one nop in the select loop; tried: switch vs if/else chain, combined … |

Totals: 118 functions (42020 instructions) in 61 units, 118 of them parked; 5 units take a data section from assembly.

Generated by `tools/leftovers.py game`.
<!-- leftovers:game:end -->
