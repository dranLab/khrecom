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
| `Game/Player/PlayerSora` | 2 | 7322 | 2 |  |
| `Form/Menu/Camp` | 12 | 6098 | 12 |  |
| `Form/Menu/JiminyMemo` | 83 | 5244 | 19 | .bss |
| `Game/Player/PlayerDRiku` | 4 | 5226 | 4 |  |
| `Game/Atel/Avatar` | 113 | 4856 |  | .rodata |
| `Game/SceneInfo` | 57 | 4635 |  | .bss .data .rodata |
| `System/Graphic/PackMan` | 42 | 4290 |  | .bss .rodata |
| `Game/Entity` | 117 | 3865 |  | .rodata |
| `Game/Map` | 26 | 3779 |  | .data .rodata |
| `Game/Player/Player` | 11 | 3573 | 11 |  |
| `Game/Player/PlayerRiku` | 2 | 2940 | 2 |  |
| `Game/Collision/CollUtil` | 13 | 2562 |  | .rodata |
| `System/Graphic/Gprim` | 15 | 2159 | 15 |  |
| `System/Graphic/Effect/libppp/eiProg/pppEiDrawShipoly` | 3 | 2143 | 3 | .bss .data .rodata |
| `Game/CardRevolver` | 10 | 2061 | 10 |  |
| `Form/Menu/RoomCreation` | 1 | 1995 | 1 |  |
| `System/Graphic/Model` | 13 | 1812 | 13 |  |
| `Game/Card` | 12 | 1794 | 12 |  |
| `System/Graphic/Effect/libppp/draw/shape` | 2 | 1771 | 2 | .rodata |
| `System/Rcf` | 18 | 1770 | 5 |  |
| `Game/Enemy/HumanBoss` | 9 | 1467 | 8 |  |
| `Game/BattleCard` | 4 | 1461 | 4 |  |
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
| `Game/CardBattle` | 5 | 600 | 5 |  |
| `Game/Enemy/MapEnemy` | 3 | 562 | 3 |  |
| `Game/Enemy/BattleEnemy` | 5 | 558 | 5 |  |
| `System/RcfCD` | 4 | 549 |  |  |
| `Game/Atel/CameraMan` | 1 | 536 |  |  |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3X` | 1 | 532 | 1 |  |
| `System/Graphic/Effect/libppp/keProg/pppKeShpTail3` | 1 | 514 | 1 |  |
| `Form/Menu/SaveLoad` | 3 | 447 | 3 |  |
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
| `Game/Debug/DebugPathData` | 1 | 292 | 1 |  |
| `Game/Atel/AtelCrossFade` | 3 | 291 | 3 | .data |
| `Form/Menu/Roulette` | 1 | 270 | 1 |  |
| `System/Graphic/WorldEnv` | 3 | 268 | 3 |  |
| `Game/Collision/CollDisp` | 2 | 262 | 1 |  |
| `System/Graphic/DispList` | 1 | 245 |  |  |
| `System/Math3D` | 1 | 234 | 1 |  |
| `Game/Collision/CollBody` | 1 | 232 | 1 |  |
| `System/TaskMan` | 2 | 217 | 2 |  |
| `System/Graphic/Effect/effect` | 3 | 211 | 2 |  |
| `Game/Atel/VSME` | 3 | 197 | 3 |  |
| `Game/Atel/Atel` | 1 | 189 |  |  |
| `Game/Enemy/EnemyWhitemushroom` | 1 | 184 | 1 |  |
| `System/Movie/MSVideo` | 1 | 180 | 1 |  |
| `System/Graphic/Effect/libppp/pppPart` | 1 | 168 | 1 |  |
| `Game/Player/PlayerGauge` | 2 | 159 | 2 |  |
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
| `System/Graphic/Model` | `searchShaderDesc__FUs` | 23 | 12 lines off, registers only: the original keeps the masked mode in a1, the table pointer in a2, 0xffff in a3 and the TRACE string's %hi in a0; ours mode a0, pointer a1, 0xffff a2, string a3. Tried: for/while/break loop shapes, a UINT copy of mode; permuter 12 min (24539 candidates, stays 12). |
| `System/Graphic/Model` | `link__6CModelR7CCoords` | 207 | 25 lines off, registers only: the original gives the loop counter i s2 and nIdx s1 (ours i s1); the extra loop-head nop differs too. Worked: one getCoordNum() load in a local for the byte store and the compare, BONELABEL[i].m_ParentIndex, four Crd(i) inlines (four strength-reduced i*0xd0 … |
| `System/Graphic/Model` | `changeTextureSub__6CModeliPC7TexInfo` | 89 | 4 lines off, scheduling: the original keeps the m_pData load after both zero stores of nTmpTex/nTmpClut, ours hoists it between them. addTexture (same loop) matches. Tried: store orders, chained assignment, i init outside the for, condition order. |
| `System/Graphic/Model` | `getApplyMatrixBBox__FR7FVECTORT0RC7FMATRIXRC7FVECTORT3` | 88 | 3 lines off: the original stores vMN[1].z/.y through the &vMN[1] operand register (t1), ours through sp. Worked: vMN[i].set(MIN/MAX, ...) (default w = 1.0f), one asm statement with all operands, D_ labels for the +-1.7e38 constants. Tried: field stores, chained assignment, pointer local; permuter … |
| `System/Graphic/Model` | `calcWorldBBox__6CModeli` | 153 | 38 lines off: the original keeps &pCrd->m_World as the strength-reduced loop pointer (s3 += 0xd0, counter counted down), &vMax/&vMin in s8/s7 for the init stores and the VU0 block, and loads the final bbox into f4/f5/f0..f3 in max-then-min order. Tried: pCrd[i] indexing (keeps the counter, 41 … |
| `System/Graphic/Model` | `checkBSphere__FR6CCoordPC4BBOX` | 47 | 10 lines off, FP registers only: the original keeps the shared 0.5f in f5 (x sum f6, y[1] f7), ours 0.5f in f7. Worked: x[0] > x[1] compare, a copy tmp (declared first) of ctr as the call argument, sqrt after the copy. Tried: statement orders, w first/last, 0.5f local; permuter 12 min (37389 … |
| `System/Graphic/Model` | `checkBBox__FRC7FMATRIXPC4BBOX` | 36 | 12 lines off, registers only: the original keeps the scratchpad pointer in v1 and the clip result in a0 (or a0,v0,a0; move v0,a0), ours v0/v1. Worked: the 0xC0000000 return last (if (!(x[1] < x[0])) {...}), v assigned inside the if, asm "nop" opening the if (ret) block (the original's extra nop … |
| `System/Graphic/Model` | `resultCheckBBox__FR6CCoordPC4BBOX` | 18 | 5 lines off: the original leaves the bc1t delay slot empty (lui 0xc000 in the final jr slot), has an extra nop after the beqz slot and builds the result in v1 (move v0,v1); ours fills bc1t with the lui. Tried: compare forms (>, <=, !<), result locals (clip \|= ret, res), if/else return; permuter 15 … |
| `System/Graphic/Model` | `_disp0_GetData__6CModelRUii` | 15 | 2 lines off: the original loads *pData before the lui 0x6000 of the tag compare, ours lui first. Tried: d/mode locals, store forms, int (*)[4]/u_long/u_long128 pData, test forms (!=, ^, ternary, tag local); permuter 12 min (22337 candidates, stays 2). |
| `System/Graphic/Model` | `_disp0_SetPacket__6CModelUiPA3_CiiUliPUl` | 588 | 137 of 659 lines off, same size; mostly registers: the original gives this s5 and iFlags&2 s4 (ours swapped), the bone/light min() selects use movn (an if-form gives that but swaps s0/s1 globally), the no-light branch materializes zero twice. Worked: case 2 with default, (USHORT) mode compares … |
| `System/Graphic/Model` | `link__10CSemiModelP6CModel` | 72 | 13 lines off: after getRawMode the original computes isDataEmpty && isSemiDataEmpty into a1 and never uses it (dead code left after a cross-jump?); ours drops the semi half. Tried: unused BOOL local, empty if, ternary, statement, duplicated identical arms (24 lines). |
| `System/Graphic/Model` | `makePkt__5CBBox` | 412 | 116 lines off, register allocation: structure matches (8-corner RotTransPers loop with min/max and W range, colour checks, four line-strip GIF packets), but the original keeps this in s4, &m_C0 in s1 and different caller-saved temporaries spilled around RotTransPers. Tried: x[0] > x[1] compare, a … |
| `System/Graphic/Model` | `getChangeClut__6CModelii` | 64 | 5 lines off, operand order only: the original adds this + idx*8 (addu a1,a0,v0), ours idx*8 + this. Worked: the if/else-if chain with `nInType == t && p[0x54] != t`, the direct p->ext arm first, raw this-relative addressing (m_TexInfo[inIdx] makes gcc pre-add this+0x68). Tried: switch, nested ifs, … |
| `System/Graphic/ModelMan` | `reset__9CModelMan` | 43 | (comment) reset: near-match. The original reloads the 0.0f arguments (and stack addresses) after each call to FVECTOR::set (called out of line here); this build keeps them in callee-saved registers across the call. |
| `System/Graphic/ModelMan` | `setupDrawEnv__9CModelMan` | 204 | (comment) setupDrawEnv: near-match. The original reloads the 0.0f arguments (and stack addresses) after each call to FVECTOR::set (called out of line here); this build keeps them in callee-saved registers across the call. |
| `System/Graphic/ModelMan` | `setupDrawMode__9CModelMani` | 64 | (comment) setupDrawMode: near-match. The two halves of the first GIF tag get swapped registers ($v0/$v1). |
| `System/Graphic/ShadowModel` | `drawBefore__15CShadowModelMan` | 188 | (comment) Near match (two instructions: the original updates the i 64 + 63 induction register at the end of the loop body, not after i++), kept for reference: |
| `System/Graphic/WorldEnv` | `initialize__9CWorldEnv` | 128 |  |
| `System/Graphic/WorldEnv` | `mainProc__9CWorldEnv` | 111 | (comment) mainProc: near-match. The original stores _vecMikePos.z through a new %hi(_vecMikePos + 8) after the sqc2 of the unit vector (Coord.h's inline, lines 87-91); here the address register of the asm is reused. |
| `System/Graphic/WorldEnv` | `getBillboardMatrix__9CWorldEnvR7FMATRIXRC7FVECTORi` | 29 | (comment) getBillboardMatrix: near-match. The original compares nFlag with -1 in a register (slt) and copies the constant 1 it also compares with; a constant -1 is folded to slti here, a variable keeps slt but not the shared 1. |
| `System/Graphic/Gprim` | `makePkt__6CZFill` | 109 | 8 lines - two scheduling ties: the original loads 0x206 before the shared zero for the PRIM/PRMODE A+D pair, and strength-reduces i 64 + 63 with its increment at the loop end while i++ sits inside the SCISSOR expression (ours increments both there); this draft fixed the hoisted-constant order (XYZ … |
| `System/Graphic/Gprim` | `makePkt__8CRotSprt` | 173 | the corner setup before the two calcVert calls (load/store order of b, c and ab: the original keeps a separate copy of m_P0.y for ab.set's _y parameter) and the register numbering that follows; a reference to m_P1 (not in the ref's locals) reproduces the original's s0 = &m_P1 read of m_P1.y in the … |
| `System/Graphic/Gprim` | `_makePkt_TexLine__FPUlP7G_STRIPT1` | 75 | registers only - the original loads it->S into v1 before the mfc1 of fW0 (filling the beql slot) and keeps iW in a0, ours puts iW in v1 and loads it->S after; tried: STQ inline taking G_ST&/ULONGLONG/FLOAT, iW before/after the call, an inSTQ block local, permuter 15 min (15280 candidates, no gain) |
| `System/Graphic/Gprim` | `_makePkt_TexLine__FPUlP7G_STRIPT1P6CCoord` | 102 | registers only - as in the CCoord-less version the original keeps iW in a0 and the copy of the mfc1 in v1 (ld of it->S after it), ours swaps them; tried: STQ inline taking G_ST&+UINT, G_ST&+FLOAT, operand order in the pcpyld asm, iW assigned inside the call, permuter 15 min (12213 candidates, no … |
| `System/Graphic/Gprim` | `_makePkt_TexTriangle__FPUlP7G_STRIPT1i` | 209 | registers only - per vertex the original loads it->S into v1 (the first one in the beql slot) and copies the mfc1 result into a0 for iW, ours puts iW in v1 and loads it->S into v0; loop shape, per-loop fW and mfc1 order (volatile _floatBits) match; tried: S read into a local first/last, STQ inline … |
| `System/Graphic/Gprim` | `_makePkt_TexTriangle__FPUlP7G_STRIPT1P6CCoordi` | 263 | registers (as the CCoord-less version: it->S into v1 and the mfc1 copy in a0 for iW, ours swapped) and one loop-alignment nop before the second loop; the draft is the CCoord-less one plus the per-vertex bone checks of the matched _makePkt_Triangle (CCoord); tried: the CCoord-less version's levers … |
| `System/Graphic/Gprim` | `_makePkt_TexTriangleFan__FP6QWdataP7G_STRIPT1i` | 162 | the strip-skip loop entry only - the original jumps into the rolled loop at its Term test (b X with the lhu in the slot), ours copies the test to the loop entry (jump.c duplicate_loop_exit_test) or keeps a redundant it < end test; fg/end and iDir/0x8000 registers fixed by the plain while (it < … |
| `System/Graphic/Gprim` | `_makePkt_TexTriangleFan__FP6QWdataP7G_STRIPT1P6CCoordi` | 179 | the strip-skip loop entry only, as in the CCoord-less version (the original enters the rolled skip loop at its Term test, ours copies the test to the entry or keeps a redundant it < end test); bone check at the loop top, iDir = 0 last in this one; tried: the CCoord-less version's levers (see there) |
| `System/Graphic/Gprim` | `_makePkt_TriangleFan__FP6QWdataP7G_STRIPT1i` | 144 | the strip-skip loop entry only (as _makePkt_TexTriangleFan) - the original jumps into the rolled skip loop at its Term test, ours copies that test to the loop entry or keeps a redundant it < end test; this draft (11 lines) fixes the fg/end registers and the bLoop scheduling of the earlier one (32) … |
| `System/Graphic/Gprim` | `_makePkt_TriangleFan__FP6QWdataP7G_STRIPT1P6CCoordi` | 163 | the strip-skip loop entry only, as in _makePkt_TexTriangleFan; bone check at the loop top, iDir = 0 last; tried: the levers of the CCoord-less fans (see there) |
| `System/Graphic/Gprim` | `getPrimGetaZ__FiRC7FVECTORPC7FMATRIXf` | 100 | argument order of the RotTransPersW call (the original sets a1 before a0; sched1 issues a1 first only when its source REG_DEADs, ours keeps vW's address live) and with it the TImode result register (a1 vs a0); tried: matrix ref local, temp for the result, 0.0f > w, wrapper with swapped parameters, … |
| `System/Graphic/Gprim` | `calcZ__12C3DStripBase` | 122 | registers only - the original loads a1 (it->P) before a0 (mMat) for the RotTransPersW call and keeps the TImode result in a1 (ours a0); sched1 puts a1 first only when its source dies at the call (toy test), here it stays live for getPrimGetaZ; tried: copies/recomputed it, swapped-parameter … |
| `System/Graphic/Gprim` | `calcZ__15C3DTexStripBase` | 122 | registers only - the original loads a1 (it->P) before a0 (mMat) for the RotTransPersW call and keeps the TImode result in a1 (ours a0); sched1 puts a1 first only when its source dies at the call (toy test), here it stays live for getPrimGetaZ; tried: copies/recomputed it, swapped-parameter … |
| `System/Graphic/Gprim` | `calcZ__8C3DCPrim` | 118 | registers only - the original loads a1 (it->P) before a0 (mMat) for the RotTransPersW call and keeps the TImode result in a1 (ours a0); sched1 puts a1 first only when its source dies at the call (toy test), here it stays live for getPrimGetaZ; tried: copies/recomputed it, swapped-parameter … |
| `System/Graphic/Gprim` | `calcZ__8C3DTPrim` | 118 | registers only - the original loads a1 (it->P) before a0 (mMat) for the RotTransPersW call and keeps the TImode result in a1 (ours a0); sched1 puts a1 first only when its source dies at the call (toy test), here it stays live for getPrimGetaZ; tried: copies/recomputed it, swapped-parameter … |
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
| `Game/BattleCard` | `__Q210CardBattle11CBattleCardP5CTaskiiii` | 628 | 8 lines - in the inState == 2 block the original stores m_iState/m_pRevolver first (m_iState from the compare's reload); ours schedules them after the SCREEN_HEIGHT load and reloads inState. Store orders, changeState(inState), m_iState = 2, switch and hill.py did not change it. |
| `Game/BattleCard` | `destroy__Q210CardBattle11CBattleCardi` | 117 | 5 lines - the original keeps two copies of `addGimmickCount(-1); break;` (cases 0xe and 6); our jump2 cross-jumps case 0xe's tail into case 6. Per-case flag sets, gotos, labels, argument spellings and block scopes all merge too (synthetic tests build/ps/cj/d*.cpp, e*.cpp, f1.cpp). |
| `Game/BattleCard` | `stateDestroy__Q210CardBattle11CBattleCard` | 415 | 22 lines - the first _adjustRad of each case keeps the sum in the loaded register and copies it to ret (mov.s); ours computes the sum straight into ret (same as stateSnatch). Case 4 also swaps a1/a2 (timer vs dy). Inline/explicit wrap forms, store orders and 12 min of permuter (10k candidates) did … |
| `Game/BattleCard` | `stateSnatch__Q210CardBattle11CBattleCard` | 301 | 4 lines - the first _adjustRad (m_vRot.y) keeps the sum in the loaded m_vRot.y register (f4) and copies it to ret (mov.s f6,f4); ours computes the sum straight into ret (f6). In-place sums, explicit wrap code, operand order and 10 min of permuter (15k candidates) did not change it; the m_vRot.z … |
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
| `Game/CardBattle` | `testPlayingCard__Q210CardBattle11CCardBattleUi` | 89 | final return only - the original computes 4 into v0 and movn's 1 from v1 when iDuelOK (the != path sets v0 = 1 in the bne delay slot, one exit); ours keeps 1 in v1/v0 and movz's or branches; tried: ~35 return forms (?: nests, ret locals, if chains, && forms), iDuelOK as INT/UINT/BOOL/SHORT and its … |
| `Game/CardBattle` | `setPlayStock__Q210CardBattle11CCardBattleiiPPQ210CardBattle11CBattleCardiiii` | 144 | induction variable - the original's copy loop runs on &m_pCard[_PLAYER][i] (cardData at -0x38), ours on &m_cardData[_PLAYER][i] (m_pCard at +0x38); all else matches; tried: pointer/index forms of inCard (in-body and for-header increments), the m_pCard store per branch, an m_pCard row pointer, … |
| `Game/CardBattle` | `decEnchantTime__Q210CardBattle11CCardBattleiii` | 66 | address forms - the original computes &m_cardEnchant[_PLAYER] as (idx*4+0x17c)+base and loads the kind with offset 0 (kept in two registers), and &m_nEnchantTime[_PLAYER] as (base+0x184)+idx*4; ours folds the offsets the other way round; the reload of m_pInstance after the decrement matches via … |
| `Game/CardBattle` | `initCardBreak__Q210CardBattle11CCardBattleiUi` | 248 | registers/scheduling only (12 lines) - the m_pSeqCtrlBreak null test loads into v0 (ours a0, reused by the else call), and in the speed branch the original stores m_hSpeed before loading m_pSeqCtrlSpeed for enableDisp (jal delay slot); tried: _ref/== NULL test forms, m_hBreak store placement, … |
| `Game/CardBattle` | `setStart__Q210CardBattle10CDuelGaugeiUi` | 53 | one store slot - the original schedules the m_tqTimer store (0x5c) after the zero stores of m_uiCount and m_uiResult, ours before them (and the setLayer sb one slot earlier); the statement order is the $LM line order; tried: all 720 orders of the six stores (200 tested), chained/temp assignments … |
| `Game/CardRevolver` | `main__Q210CardBattle13CCardRevolver` | 321 | 27 lines off, only the TRACE argument loads in the three card dumps: the original loads uiFlag into v1 and uiOwner last into v0 (the card pointer's register), ours the other way round. Worked: state switch with explicit returns, m_pCards[i] indexed directly (pBC temp moved the pointer to t4), … |
| `Game/CardRevolver` | `rollDeck__Q210CardBattle13CCardRevolveriii` | 248 | 5 lines off, the direction `a`: the original computes it as li t3,1 / slt v0,v1,a1 / movz t3,v1,v0 with v1 = -1 set in both predecessors of the join, then negates num with bltzl; ours branches (bgez). Worked: one `i` for all three loops (s0), !m_iMode-first rc/pCards select, hand++ after setHand, … |
| `Game/CardRevolver` | `changeMode__Q210CardBattle13CCardRevolverii` | 101 | 12 lines off, all in `limit`: the original loads m_nTotal[m_iMode] (lhu + sll 16) above the m_iMode branch and sign-extends (sra 16) only in that arm (interblock-scheduled split extension); ours branches first and uses lh. Worked: `if (inMode == -1) ^= 1; else =`, `!old_mode ? m_pCards : … |
| `Game/CardRevolver` | `exeReload__Q210CardBattle13CCardRevolver` | 127 | 14 lines off: register allocation and latch order of the first loop - the original keeps i in t0 and the card offset in a2 (decremented first in the latch, in the continue paths' delay slots), ours the reverse; the second loop's i likewise. Worked (from 48 bytes over to same size): separate … |
| `Game/CardRevolver` | `initAction__Q210CardBattle13CCardRevolver` | 265 | ~165 lines off, mostly register allocation: ours keeps one more value live across calls (the getStock() == -1 value is CSE'd into an s-register for the later getHand() test), so two flags spill instead of one (bReloadOC) and every flag moves one register. Structure found: decEnchantTime inside … |
| `Game/CardRevolver` | `setStockInfo__Q210CardBattle13CCardRevolver` | 348 | 22 lines off, two places: the first block has CStockFolder::setStockInfo's open issue (the original issues the div before the pos.init() zero stores and the changeTexture zero arguments, ours after); the num > 0 arms load a0/a1 for setPos at the top of each arm with one shared jal in the original. … |
| `Game/CardRevolver` | `dropPlutoMarkCard__Q210CardBattle13CCardRevolverPC7FVECTOR` | 131 | 1 line off: the TRACE's card-kind argument - the original reloads it (lbu 0x150(s0)) as the last argument load; card->getCardKind() / getCardData()->m_nKind schedule it right after a3, param.PrizeKind (below) puts it last but from the stack. Worked: _clearPrizeFlags(&param), getCardData()->m_nKind … |
| `Game/CardRevolver` | `main__Q210CardBattle13CEnchantGauge` | 236 | 15 lines off, all one register: the original keeps m_nNext and num = m_nNext / 10 in a1 (then a3 = a1 + 0x2B), ours in a3 (same family as changeCount). Worked: the first switch without case 0 (gives the beqz / sltiu 3 / == 6 tree), explicit cases 0 and 6 in the jump-table switch, … |
| `Game/CardRevolver` | `changeCount__Q210CardBattle13CEnchantGaugei` | 67 | 14 lines off, register allocation of the two quotients: the original keeps inNum / 10 in a0 and m_nCount / 10 in v0 (then a3 = v0 + 0x6C); ours puts num into a3. Worked: `if (m_uiState == 4) return;` before the switch (the extra beq), case 4 explicit, num = m_nCount / 10 computed once. Tried: 8 … |
| `Game/CardRevolver` | `setStockInfo__Q210CardBattle12CStockFolder` | 217 | 17 lines off, scheduling of the first block: the original issues div, then the changeTexture argument zeros interleaved with pos.init(), the trap check after; ours keeps them apart. Worked: inverted layout ternary (>= 0x6F first), _ref(g_pStockSkillList), m_uiState before m_uiCategory in default. … |
| `Game/EffectMotion` | `updateAttachEffect___17CEffectMotionCtrli` | 339 | (comment) Close: only case 2's "m_iStep = 0" tails differ - gcc cross-jumps them all into the second body's copy, the original keeps the one in the effect == NULL branch (the other bodies jump there). jump2's cross-jumping (find_cross_jump) merges a "sw $0; j" tail that follows a label after one … |
| `Game/Movement` | `calcPos___9CMovement` | 369 | (comment) near-match (register copies of &end/&norm before the last two detect calls): kept as INCLUDE_ASM |
| `Game/SEMotion` | `update__9CSEMotion` | 57 | (comment) Matches except that `this` and `ctrl` swap registers (s1/s2). |
| `Game/Debug/DebugPathData` | `savePath__14CDebugPathDataPCc` | 292 | 5 lines off: the original hoists &sPathParam.fTime (the start of the parameter pointer) out of the paths loop into a stack slot, like &sVecParam and &vPath; ours computes it before the parameter loop. Worked: one j per inner loop, the 0xFFFFFFF0 mask, the _zeroVec helper (45 -> 5 lines). Tried for … |
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
| `Game/Atel/VSME` | `searchLockOnNpc__Q24ATEL4CVSM` | 47 | 8 bytes short; only the first return differs: the original keeps `if (!g_pPlayer) return -1;` as bnez over "b end; li -1", ours lets jump2 cross-jump it into the loop's final `li -1` (beqz v1 to it; -dJ dump: all four -1 returns share one label). $LM tags: 642 the test, 643 the return, 645-647 … |
| `Game/Atel/VSME` | `execActor__Q24ATEL4CVSM` | 118 | 3 lines, one store: the original issues the m_Freg store first after the Freg/prev-actor join, ours last (sched2 -fsched-verbose: our store gets priority 7 against 8 for the m_SP store and the m_Pctr load, so reorg puts it into the execCode jal slot instead of `move a0,s0`). Everything else … |
| `Game/Atel/VSME` | `acWaitReq__Q24ATEL4CVSMRQ24ATEL5Actorii` | 32 | same instructions and order except the wait_timer product: the original multiplies into LO ("mult $zero,s1,v1" + mflo before the sh), ours with the 3-operand mult into v1 (gcc's mulsi3_mult3 "l" vs "d" alternative, chosen by regclass: LO only when the product feeds a plain register copy, as for a … |
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
| `Game/Player/PlayerDRiku` | `setAttack__Q22PL12CPlayerDRiku` | 451 | 7 lines off, registers only: in the card branch the original keeps %hi(g_pBattleCardList) in t0 and the shared constant 1 (m_iAttackNo == 1, _upKind's shift) in a2, ours t1 and t0. Worked: CPlayer::setAttack's form, the player-flag 0x80000 arm first, `pre == 7 \|\| isAir()` for the last hit, case 7 … |
| `Game/Player/PlayerDRiku` | `initJump2nd__Q22PL12CPlayerDRiku` | 153 | with a target, the m_vMarkPos/m_hMark copies get swapped registers and rot.y is stored before m_fDir/m_fJumpDir instead of in the branch delay slot (11 lines differ). |
| `Game/Player/PlayerDRiku` | `exeStockSkill__Q22PL12CPlayerDRiku` | 3473 | structurally complete (35 cases in the original order, Riku's 0x6cc-0x6f9 group renumbered to 0x712-0x73f), same frame, 8 bytes long, 173 of 3903 lines off: case 1's attack-ID switch keeps a compare for the empty case 0x690 in the original; cross-jumped tails differ (0x67c's flag-and-return tail, … |
| `Game/Player/PlayerDRiku` | `setMagic__Q22PL12CPlayerDRikuiii` | 1149 | same structure and size within 8 bytes, 310 of 1363 lines off: the BULLET_PARAM field stores and the member loads feeding them are scheduled differently in every case (the original loads m_AtkKind, m_AtkAttr, m_iAttackPower in that order and stores them together), plus registers in the stock-skill … |
| `Game/Player/PlayerGauge` | `setDropMp__Q25Gauge11CPrizeGaugei` | 82 | 18 lines off: the %hi(m_pInstance) base and its loop copy swap s2/s3, and the inMp <= 0 return branch doesn't get the epilogue's lq s0 in its delay slot, so the loop top gets an alignment nop. Worked: *(T*)& stores for m_uiMogState and m_iDropMp (m_pInstance reloaded after them, as resetHP). … |
| `Game/Player/PlayerGauge` | `setCharge__Q25Gauge12CEscapeGaugef` | 77 | 3 lines off, the getLayPosition argument loads: the original sets a2 first and a1 (7) last (in the jal delay slot), ours a1 first. Worked: case 2 with its own copy of case 1's body (separate tree nodes 1/0/2/3, cross-jumped later), case 0 falling into it. Tried: _ref(g_szLadCockpit), FALSE vs 0, … |
| `Game/Player/PlayerRiku` | `exeStockSkill__Q22PL11CPlayerRiku` | 2420 | (comment) Best attempt (same structure): the CBattleEnemy flag tests get swapped registers, 0x6CD doesn't reload the flag, and 0.0f isn't rematerialized in f20 in 0x6CC. |
| `Game/Player/PlayerRiku` | `setMagic__Q22PL11CPlayerRikuiii` | 520 | (comment) Best attempt (same structure): BULLET_PARAM field stores and the attack-power loads are scheduled differently. |
| `Game/Player/PlayerSora` | `exeStockSkill__Q22PL11CPlayerSora` | 7049 | 14 of 7930 lines differ, in three case groups (711/741/1042; all other jump-table groups match): the original loads m_pMove (lw 0xa0) between the two stores into the {0, d0, 0, 0} temp and stores m_fDir before copying the temp; ours (sched1) loads m_pMove after both temp stores and sched2 keeps it … |
| `Game/Player/PlayerSora` | `exeGlide__Q22PL11CPlayerSora` | 273 | only the analog-steering block after Homing differs: the original loads m_pMove (lw v1,0xa0) before the y store into the {0, dir, 0, 0} temporary at sp+0x20 and copies it (lq/sq to sp+0x10) after the m_fDir store; ours stores y and loads the temp first (its stack-temp accesses conflict with this-> … |
| `Game/Player/SummonTink` | `initRecover__Q26Summon11CSummonTink` | 37 | (comment) Best attempt: same size; combine folds the Crd() result straight into a3 ("move a3,v0" right after the call), so the effect name's address lands in v0 instead of v1 and the lui/lw are scheduled later. |
| `Game/Enemy/BattleEnemy` | `recover__12CBattleEnemyiPCciRC7FVECTORPC6CCoord` | 121 | 3 lines - the original keeps the hp <= 0 `return 0` as a stub right after the bgtz (the count >= 8 and inParent paths jump to it) and a separate return-0 copy after ApplyMatrix; ours (this goto form) merges the stub into the ApplyMatrix copy; tried: early returns, nested if/else with final return … |
| `Game/Enemy/BattleEnemy` | `setupFlick__12CBattleEnemy` | 82 | the 30-degree constant (0.5235988f) is loaded once into $f20 and reused for sinf, cosf and calcOrbitSpeedFromDistance in ours; the original reloads it (lui/ori/mtc1) before each call and keeps the sinf result in $f20 instead (vel.y stored after cosf); tried: field stores vs vel.set(), … |
| `Game/Enemy/BattleEnemy` | `initEnemyParam__12CBattleEnemyiii` | 245 | 34 lines - the Riku-game scan loop: the original places the getRsrcData() ternary before the inline getExtraParam's found block and keeps the parameter in s0 (ours: s2, blocks swapped); the rest (seek loops, relocation, floor mapping, basic param) matches; tried: while/for/continue loop forms, … |
| `Game/Enemy/BattleEnemy` | `playSmallDamageSE__12CBattleEnemy` | 55 | the load of hitBody->m_pCollision (getDetectPos) is scheduled before the vol = -1 store in ours, after it in the original (8 lines: registers a2/v1, order): our vol store is a fixed scalar MEM that cannot alias the varying struct load; `INT vol[1]` matches but the ref says INT vol (same open … |
| `Game/Enemy/BattleEnemy` | `playBigDamageSE__12CBattleEnemy` | 55 | the load of hitBody->m_pCollision (getDetectPos) is scheduled before the vol = -1 store in ours, after it in the original (8 lines: registers a2/v1, order): our vol store is a fixed scalar MEM that cannot alias the varying struct load; `INT vol[1]` matches but the ref says INT vol (same open … |
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
| `Form/Menu/Camp` | `initCardKind__5CCampiii` | 331 | 26 of 342 lines differ: loop.c hoists the CCard::m_pCardInfo load of the 3-deck copy out of the outer loop (the original reloads it each iteration: the stack-array stores must alias it there). tried: destination pointer, direct m_pCardInfo->_deck[i] without _ref, pointer post-increment, _ref on … |
| `Form/Menu/Camp` | `mapCardWinMenu__5CCamp` | 723 | 2 of 819 lines differ (was 86): with deckEditMenu's forms (member arithmetic instead of nSelect, the number-grid moves with `n = 9; if (m < 5) n = 4;`, per-arm clamps in the down move, fgCursorMove in both arms of the right move, error stores Error/WinStep/SubStep) only the cancel block is left: … |
| `Form/Menu/Camp` | `statusWinMenu__5CCamp` | 392 | 2 of 457 lines differ: reorg fills setStatusWin(4)'s delay slot (cancel block, after the `cancel:` label) with `li a1,4` in ours, with `ret = 1` (s3) in the original. tried: ret before/after the call and before the label, ret/bChange declaration order (ref: ret, fgCursorMove, … |
| `Form/Menu/Camp` | `setDeckInfo__5CCampi` | 951 | 4 of 1048 lines differ in the last loop: the addu operand order of this + m_nKindCategorySelect 2 and v0/v1 swapped for the nColSeq != 0x73 compare. tried: without _ref (24 lines), _ref(*(p + i)), _ref(m_nKind)[i], != 0, !i, swapped operand/condition orders, !(a && b); permuter 7 min (4293) and 15 … |
| `Form/Menu/Camp` | `setTechWin__5CCampi` | 551 | 53 of 621 lines differ: in the sleight-list loop the original spills pTechCateTbl and nTechCateCnt and recomputes &pTechCateTbl[nTechSelectTop] from the spilled pointer every iteration; ours strength-reduces it into an s-register (the same giv difference as setMainCmd's font loop). tried: the … |
| `Form/Menu/Camp` | `setMapCardWin__5CCampi` | 1227 | same size, 33 of 1409 lines differ: the stack slots of the spilled hoisted addresses (&m_pMCInfoNumCtrl, &m_pMCInfoNumFnt, &m_pMCFnt, &m_pMCCard, &m_nMapKind, i + 1) are assigned in another order (orig 0x74 NumFnt, 0x78 MapKind, 0x7c i+1, 0x84 MCCard, 0x88 MCFnt, 0x8c NumCtrl; ours ascending by … |
| `Form/Menu/Camp` | `setWorldmapCursor__5CCamp` | 853 | 62 of 941 lines differ. The RCNORMA::NormaSrc argument of createNorma is built from four lhu of its stack copy in the original (extract_bit_field pieces), one ld in ours - the open issue of RoomCreation::menuMain and C2DCard::replaceNorma; the s3/s4 and spill-slot (0x64/0x68) swaps around it … |
| `Form/Menu/Camp` | `setMainCmd__5CCampii` | 257 | 53 of 282 lines differ (was 81): with the ref's function-level `INT i` plus a block `INT i` in the else branch `this` is in s0 as in the original; left: in the font-replacing loop the original recomputes &m_pMainCmdFnt[i] from i each iteration (only m_pMainCmdCtrl[i] strength-reduced), ours folds … |
| `Form/Menu/Camp` | `setStatusWin1__5CCampi` | 429 | 13 of 487 lines differ (was 23): vec.set() (the ref's inlined `FVECTOR *this`) and a precomputed `m_bPlayerKind + 0xF` (permuter) fixed the rest; left: the original keeps &vec in v0 (y/w/z stores and the later fShift = vec.z load through it), so pPlayerManager is reloaded from its spill slot for … |
| `Form/Menu/Camp` | `setCursor2__5CCampiii` | 184 | 2 of 198 lines differ: the two member addresses of the top-index select (&m_nMCSelSelectTop, &m_nKindSelectTop) issue in the other order in the prologue (equal-priority tie). tried: ternary both polarities, if/else and default+if forms, the ref's `INT& nKindSelectTop` reference local, idx local; … |
| `Form/Menu/Camp` | `searchRoom__5CCampii` | 158 | 15 of 169 lines differ: in the left/right loop the original's rotated prefix (loop test .. `if (!bDiscover) break`) stops before the checkNorma test, which stays at the loop top; ours rotates the whole body. tried: nested ifs, `continue` forms, `!bDiscover \|\| checkNorma`, bDiscover if/else, while … |
| `Form/Menu/Camp` | `getCardIndex__5CCampi` | 42 | 4 of 44 lines differ: the original computes nIndex + 1 from a1 and moves idx = nIndex into the beqz delay slot; ours copies nIndex through idx first. tried: idx assigned in an else / after the test, separate n = nIndex + 1, i <= nIndex, return nIndex directly, comma init in the for; permuter 5 + … |
| `Form/Menu/DecknameEdit` | `chgType__16CDnEditWordPlateii` | 67 | (comment) Scheduling only: same instructions, but the original fills the bSeOn branch's delay slot with the playSE "li a0,3" (ours: "li a1,-1") and allocates the tab label pointer to v1 (ours: a1). |
| `Form/Menu/JiminyMemo` | `ScrollBarChange__11CJiminyMemoPPQ22D210C2DSeqCtrlffiii` | 122 | 22 lines: ours hoists the 2.0f of case 2 out of the loop (an extra saved f24, frame +16, 0.01f moves from f23 to f24); the original loads it inside case 2 like ScrollBarInit does. Everything else matches. tried: y expression forms (temp, parenthesised, vec = vPos + .y), case order, INT i at the … |
| `Form/Menu/JiminyMemo` | `contentMenu__11CJiminyMemo` | 859 | 129 lines: the stack slots of iFlag[5]/bComp[5] (and the pMarkSeq/pNewSeq spills) differ, which shifts every s-register from the prologue on (s7 vs s8 saves); the code itself matches in structure. tried: declaration orders of posi/telopNo/iFlag/bComp/pMarkSeq/pNewSeq (posi after bComp fixes the … |
| `Form/Menu/JiminyMemo` | `contentCollection__11CJiminyMemo` | 1759 | 2 lines: in case 40's card-view branch the original stores the step (50, v1) before the next mode (8, v0); ours stores the mode first. The sched2 dump shows both stores tied (prio 5, both after the shift.x/y loads), broken by RTL order; writing setContentStep(50) first fixes the store order but … |
| `Form/Menu/JiminyMemo` | `contentCharaView_ModelView__11CJiminyMemo` | 287 | 6 lines: three FP registers of case 40 (DEG2RAD(2.0f) constant, 1/4096 constant, the rotation load: original f4/f1/f3, ours f1/f3/f4). Everything else matches. tried: line-marker layout (both table arms written out, flags 0x8000 then 0x4000, setter order, ry split over 3 statements), literal vs … |
| `Form/Menu/JiminyMemo` | `contentMinigame__11CJiminyMemo` | 688 | 34 lines, all stack offsets: the six hoisted string addresses (strBGlider..strBPanic, &scoreStr) are spilled in another slot order (GCSE/PRE creates their pseudos in hash-table order: ours BPanic first, the original BGlider first). Everything else matches (time math via the Com.h(59) seconds … |
| `Form/Menu/JiminyMemo` | `setPosition__15CMemo2DSequenceRC7FVECTOR` | 10 | final group (destructors = key functions, all weak class inlines, getTaskPointer<T>, static init, the .rodata after the vtables): gcc emits all of it at the end of the object, so it can only come from C at once. Drafted in full (build/jm/grp4.cpp; recipe in memory jiminymemo-unit-lessons): every … |
| `Form/Menu/JiminyMemo` | `setLocalPosition__15CMemo2DSequenceRC7FVECTOR` | 10 | final group (destructors = key functions, all weak class inlines, getTaskPointer<T>, static init, the .rodata after the vtables): gcc emits all of it at the end of the object, so it can only come from C at once. Drafted in full (build/jm/grp4.cpp; recipe in memory jiminymemo-unit-lessons): every … |
| `Form/Menu/JiminyMemo` | `setScale__11CMemo2DCardf` | 9 | final group (destructors = key functions, all weak class inlines, getTaskPointer<T>, static init, the .rodata after the vtables): gcc emits all of it at the end of the object, so it can only come from C at once. Drafted in full (build/jm/grp4.cpp; recipe in memory jiminymemo-unit-lessons): every … |
| `Form/Menu/JiminyMemo` | `setContentParam__11CJiminyMemoii` | 26 | final group (destructors = key functions, all weak class inlines, getTaskPointer<T>, static init, the .rodata after the vtables): gcc emits all of it at the end of the object, so it can only come from C at once. Drafted in full (build/jm/grp4.cpp; recipe in memory jiminymemo-unit-lessons): every … |
| `Form/Menu/JiminyMemo` | `getContentParam__11CJiminyMemoi` | 23 | final group (destructors = key functions, all weak class inlines, getTaskPointer<T>, static init, the .rodata after the vtables): gcc emits all of it at the end of the object, so it can only come from C at once. Drafted in full (build/jm/grp4.cpp; recipe in memory jiminymemo-unit-lessons): every … |
| `Form/Menu/JiminyMemo` | `setContentScroll__11CJiminyMemoiRC7FVECTOR` | 8 | final group (destructors = key functions, all weak class inlines, getTaskPointer<T>, static init, the .rodata after the vtables): gcc emits all of it at the end of the object, so it can only come from C at once. Drafted in full (build/jm/grp4.cpp; recipe in memory jiminymemo-unit-lessons): every … |
| `Form/Menu/JiminyMemo` | `getContentScroll__11CJiminyMemoiR7FVECTOR` | 12 | final group (destructors = key functions, all weak class inlines, getTaskPointer<T>, static init, the .rodata after the vtables): gcc emits all of it at the end of the object, so it can only come from C at once. Drafted in full (build/jm/grp4.cpp; recipe in memory jiminymemo-unit-lessons): every … |
| `Form/Menu/JiminyMemo` | `mainInit__11CJiminyMemo` | 320 | final group (destructors = key functions, all weak class inlines, getTaskPointer<T>, static init, the .rodata after the vtables): gcc emits all of it at the end of the object, so it can only come from C at once. Drafted in full (build/jm/grp4.cpp; recipe in memory jiminymemo-unit-lessons): every … |
| `Form/Menu/JiminyMemo` | `mainLoadWait__11CJiminyMemo` | 332 | final group (destructors = key functions, all weak class inlines, getTaskPointer<T>, static init, the .rodata after the vtables): gcc emits all of it at the end of the object, so it can only come from C at once. Drafted in full (build/jm/grp4.cpp; recipe in memory jiminymemo-unit-lessons): every … |
| `Form/Menu/JiminyMemo` | `mainIn__11CJiminyMemo` | 322 | final group (destructors = key functions, all weak class inlines, getTaskPointer<T>, static init, the .rodata after the vtables): gcc emits all of it at the end of the object, so it can only come from C at once. Drafted in full (build/jm/grp4.cpp; recipe in memory jiminymemo-unit-lessons): every … |
| `Form/Menu/JiminyMemo` | `mainInWait__11CJiminyMemo` | 18 | final group, see the note above getTaskPointer<T> |
| `Form/Menu/JiminyMemo` | `mainLoop__11CJiminyMemo` | 2 | final group, see the note above getTaskPointer<T> |
| `Form/Menu/JiminyMemo` | `mainOut__11CJiminyMemo` | 13 | final group, see the note above getTaskPointer<T> |
| `Form/Menu/JiminyMemo` | `mainOutWait__11CJiminyMemo` | 22 | final group, see the note above getTaskPointer<T> |
| `Form/Menu/RoomCreation` | `menuMain__13CRoomCreation` | 1995 | (comment) menuMain: the RCNORMA::NormaSrc by-value arguments (createNorma/replaceNorma) are built from four halfword loads in the original, ld in ours; the register allocation in case 0 follows from that (0x50 bytes shorter). |
| `Form/Menu/Roulette` | `setCardInfo__9CRoulettei` | 270 | (comment) Scheduling only: after the CButton constructor the copy to pButton (s1) is scheduled before the getLayPosition argument setup instead of after it (5 lines differ). |
| `Form/Menu/SaveLoad` | `exeMessageDlg__11CSaveLoadUI` | 126 | the case-1 OK tail (m_nDlgResult = 1; CSound::playSE(2, -1, 64)) is cross-jumped into the identical tail of case 2/3 in ours, the original keeps both copies; also the lockPadControl(TRUE) tail loads into v1 (original v0) and one nop in the select loop; tried: switch vs if/else chain, combined … |
| `Form/Menu/SaveLoad` | `checkSelector__11CSaveLoadUIRssiisii` | 127 | registers only (22 lines): the original keeps this in s1, nOutIdx in s2 and nValDownKey in s3, ours nValDownKey/this/nOutIdx in s1/s2/s3 (global-alloc priority); code and layout match once isButtonRpt/Trg are the if-return-call inlines; tried: ternary/if forms of the pad inlines, permuter 4 min … |
| `Form/Menu/SaveLoad` | `createBootInfoMsg__11CSaveLoadUIiPc` | 194 | 2 lines - the original fills the beqz (nSlot) delay slot with the else branch's first `move a1, s0` (pszForm), ours keeps it in the else block; everything else matches; tried: if/else polarity, szSlot at function scope, a nested slot-sprintf inline, case orders, locals for each CTD text, permuter … |

Totals: 716 functions (101990 instructions) in 85 units, 215 of them parked; 17 units take a data section from assembly.

Generated by `tools/leftovers.py game`.
<!-- leftovers:game:end -->
