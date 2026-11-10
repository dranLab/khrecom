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
| `Game/Map` | 2 | 1952 | 2 |  |
| `Form/Menu/Camp` | 2 | 1778 | 2 |  |
| `System/Graphic/DispList` | 1 | 245 | 1 |  |
| `System/Math3D` | 1 | 234 | 1 |  |
| `System/Graphic/Effect/libppp/draw/shape` | 1 | 133 | 1 |  |
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
| `System/Math3D` | `InvMatrix__6Math3DR7FMATRIXRC7FMATRIX` | 234 | 90 of 248 lines (2026-11-09): the outer loop of the elimination hoists 1e-8/-1e-8 (lui/ori/mtc1 into $f5/$f4) in loop pass 2 (-dL: 118 real insns; the original keeps them in the loop, so it had 123-125 there), which shifts every register after it (&c in s1 instead of a3, i in t0, k-part temps) and … |
| `System/Graphic/DispList` | `_drawOT__19CDispListShadowDrawR5OTtag` | 245 | 59 of 289 lines (2026-11-09): only loop.c on the draw loop over pBuf[j] (line 940) differs: the original keeps `li 12; mult` in the loop, ours strength-reduces it to a pointer, so the registers shift (ours spills i, the original keeps i in s6 and spills hHnd; s6/s7/s8 renamed throughout). … |
| `System/Graphic/WorldEnv` | `initialize__9CWorldEnv` | 128 | 0-8 of 177 lines (2026-11-08): this draft is the original source. Compiled with the original's own anonymous-namespace name (c:/KHCOM/MainApp/System/Graphic/WorldEnv.cpp + suffix UPaaaa = pid 2588, via a rewritten .ii line marker and a chosen pid) it matches every time; our build names the … |
| `System/Graphic/Effect/libppp/draw/shape` | `pppVu1ShapeDraw` | 133 | 4 of 143 lines (2026-11-10): `or a0,a1,a0` (the call argument) issues before `addiu t6,a1,32` instead of after tag 2's next store, and tag 3's `sb t4,3(t6)` comes after its p[1] store instead of first. With tag 3 written id/next/qwc like the others its stores match, but t3/t4 swap: local-alloc … |
| `System/Rcf` | `finalize__4CRcf` | 111 | 0 or 6 of 153 lines (2026-11-07), random per compile (matched 4 of 6 rebuilds): s_nRcfMapCount/s_nRcfSceneCount's hoisted %hi pseudos tie in global-alloc priority (3 refs / 120 insns each), the tie goes to the lower pseudo, and PRE numbers them in expression-hash bucket order; the SYMBOL_REF hash … |
| `System/RcfCD` | `_startCdRead__Fii` | 41 | 4-8 lines, varying between identical builds (the draft above is the C) - the hoisted lui's of the s_n*_R statics get s5/s6 in an order that changes per compiler run (gcse PRE hash order, memory gcse-pre-hash-order); store orders tried (all worse). Kept asm because the result is not reproducible. |
| `Game/Map` | `init__4CMapii` | 1046 | 82 of 1190 lines, 8 bytes short (2026-11-09) - only xmin/xmax/zmin/zmax's registers and the division block's schedule differ: the original keeps xmax's quotient apart (mflo v1; move s6,v1: cse kept the variable canonical, so xmax dies twice and goes to global-alloc, s6 after the fptodp temps … |
| `Game/Map` | `disp__4CMap` | 906 | 709 of 1308 lines, 44 bytes short - structure as the original (debug clamp, shader modes, group selection by player position, pipelined kick/resultCheckBBox display via group table or grid, sky polygons). The 44 bytes: the original keeps two copies of the "flush the last model" block (group and … |
| `Form/Menu/Camp` | `setTechWin__5CCampi` | 551 | 27 of 612 lines (2026-11-10; drop the data block after the INCLUDE_ASM to build the draft): only the s-register roles in the sleight-list loop differ (original s3 nTechKind, s4 i, s5 nTechSeqIdx, s6 nTechSelectTop, i + 1 in s7; this draft s3 nTechSelectTop, s4 nTechKind, s5 i, s6 nTechSeqIdx, … |
| `Form/Menu/Camp` | `setMapCardWin__5CCampi` | 1227 | 23 of 1407 lines (2026-11-10; drop the data block after the INCLUDE_ASM to build the draft): only the spill slots of seven PRE-hoisted values differ (orig 0x74 &m_pMCInfoNumFnt, 0x78 &m_nMapKind, 0x7c i + 1, 0x80 nKind - 161, 0x84 &m_pMCCard, 0x88 &m_pMCFnt, 0x8c &m_pMCInfoNumCtrl; ours 0x74 … |

Totals: 10 functions (4622 instructions) in 8 units, 10 of them parked; 1 units take a data section from assembly.

Generated by `tools/leftovers.py game`.
<!-- leftovers:game:end -->
