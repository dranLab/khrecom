# SDK (lib/) leftovers

The `lib/` units that are not fully decompiled. All 167 lib units build byte-identical. The list below is what
still comes from the original assembly, as a whole unit, a function or a data section. It is generated from the
sources by `tools/leftovers.py lib --update docs/sdk-leftovers.md`: the status and the "what differs" text come
from the `Parked:` / `Hand-written:` / `Not attempted:` note above each function (see the tool's help).

Status:
- **not attempted**: no C written yet.
- **parked**: C written, kept under `#if 0` next to the `INCLUDE_ASM`, with a note on what still differs.
  It was parked after many variants stopped improving the diff.
- **hand-written**: no compiler output to match (MMI, trap handlers, `.s` sources). These stay assembly.

To re-check: `tools/check.sh` lists every unit with INCLUDE_ASM functions left. `tools/funcdiff.py <unit>`
shows a draft's diff after you switch its `#if 0` to `#if 1`.

## Inventory

<!-- leftovers:lib:start -->
### Units with no source file

Their code or data comes entirely from the original assembly.

| unit | functions (instr) | data |
|---|---|---|
| `lib/nodebug/memcmp` | `memcmp` (37), `sn_reg_frame` (5), `sn_dereg_frame` (3), `func_00392060` (1) | .bss |
| `lib/nodebug/SN_LOADED_RELMODS` | - | .data |
| `lib/nodebug/_GLOBAL__F_0build.cppF7aaaa` | - | .data |
| `lib/nodebug/__CTOR_LIST__` | - | .data |
| `lib/nodebug/__FRAME_BEGIN__0` | - | .data |
| `lib/nodebug/__SN_Libsn_version_v26` | - | .bss |
| `lib/nodebug/_ctype_` | - | .rodata |
| `lib/nodebug/blanks.12` | - | .rodata |
| `lib/nodebug/errno` | - | .bss |
| `lib/nodebug/huge` | - | .rodata |
| `lib/nodebug/impure_data` | - | .data |
| `lib/nodebug/lconv` | - | .rodata |
| `lib/nodebug/sn___infinity` | - | .rodata |
| `lib/src/newlib/libm/common/s_lib_ver` | - | .rodata |
| `lib/src/newlib/libm/math/s_infconst` | - | .rodata |
| `lib/table` | - | .rodata |

### Functions still INCLUDE_ASM

Sorted by unit, then size. `instr`: instructions in the original.

| unit | function | instr | status | what differs |
|---|---|---:|---|---|
| `lib/nodebug/check_dll` | `relocate_dll` | 251 | parked | only the scheduling of the HI16 patch() call's arguments differs |
| `lib/nodebug/check_dll` | `snDllUnload` | 72 | parked | only the register of notify's address differs |
| `lib/nodebug/snDebugInit` | `snTeq` | 26 | hand-written | trap handler |
| `lib/graphdev_checkModelVersion` | `sceGsSetDefDispEnv` | 221 | parked | matches except register order in the DISPLAY computation (orig: DY and off[0] + 636 first). |
| `lib/graphdev_sceGsExecStoreImage` | `sceGsExecStoreImage` | 419 | parked | close, but register allocation differs (the PSMCT24 tail, trxreg.RRW kept extracted instead of raw). |
| `lib/libpad2` | `scePad2CreateSocket` | 66 | parked | the pad2_info address (lui) is scheduled before the 820 multiplier. |
| `lib/libpad2` | `scePad2GetButtonInfo` | 56 | parked | button type kept in a1 (orig a3), movz instead of movn for the digital case. |
| `lib/output` | `_dispRefImageField` | 264 | not attempted |  |
| `lib/output` | `_dispRefImage` | 176 | not attempted |  |
| `lib/output` | `_cpr8` | 164 | not attempted |  |
| `lib/csc` | `_sceMpegCscStoreRefImage` | 310 | parked | _doCSC2 differs only in two registers (0xffc0 / D3_CHCR); _sceMpegCscStoreRefImage in s1/s2 for the inlined CSC block's rgb/qwc. |
| `lib/csc` | `_doCSC2` | 112 | parked | _doCSC2 differs only in two registers (0xffc0 / D3_CHCR); _sceMpegCscStoreRefImage in s1/s2 for the inlined CSC block's rgb/qwc. |
| `lib/pack` | `sceMpegDemuxPssRing` | 248 | parked | 6 lines off: the zero store of allData, the delay slot of the first pack header test, the reload register of allGp |
| `lib/mpc` | `_getAllRefs` | 449 | parked | all but the field-picture setup matches: there the original schedules the constant 2 of the x160 test before sel = 0 (registers a1/a0/a2 of the refs loads differ) |
| `lib/mpc` | `_getRef0` | 290 | parked | same structure and size as the original; register allocation differs (the original keeps y in s1, n in t9 and spills avg << 2) |
| `lib/mpc` | `_rix_111` | 72 | hand-written | MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) |
| `lib/mpc` | `_ri0_111` | 69 | hand-written | MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) |
| `lib/mpc` | `_ri0_011` | 63 | hand-written | MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) |
| `lib/mpc` | `_rix_011` | 62 | hand-written | MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) |
| `lib/mpc` | `_copyAddRefImage` | 59 | hand-written | MMI reference-copy routine |
| `lib/mpc` | `_ri0_101` | 57 | hand-written | MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) |
| `lib/mpc` | `_rix_101` | 55 | hand-written | MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) |
| `lib/mpc` | `_rix_110` | 53 | hand-written | MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) |
| `lib/mpc` | `_ri0_110` | 52 | hand-written | MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) |
| `lib/mpc` | `_ri0_001` | 51 | hand-written | MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) |
| `lib/mpc` | `_ri0_010` | 46 | hand-written | MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) |
| `lib/mpc` | `_rix_001` | 45 | hand-written | MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) |
| `lib/mpc` | `_rix_010` | 43 | hand-written | MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) |
| `lib/mpc` | `_ri0_100` | 43 | hand-written | MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) |
| `lib/mpc` | `_rix_100` | 39 | hand-written | MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) |
| `lib/mpc` | `_ri0_000` | 37 | hand-written | MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) |
| `lib/mpc` | `_rix_000` | 29 | hand-written | MMI reference-copy routines (_rix_*: luminance, _ri0_*: chrominance) |
| `lib/eecdvd_sceCdCallback` | `sceCdLayerSearchFile` | 191 | parked | the 2nd/3rd SCE_CD_debug reloads land in v0 in the original, v1 here. |
| `lib/eecdvd_sceCdCallback` | `sceCdInit` | 186 | parked | t0/t1 swapped for the _scmd_dr_bind/_sceCd_ee_read_mode %hi, and the order of li/lui before the _sceCd_CdInit_ON store. |
| `lib/eecdvd_sceCdCallback` | `_sceCd_cbLoop` | 52 | parked | the exit block stores _sceCd_c_cb_sem, sceCdCbfunc_num, cb_thid and keeps cb_flag for the delay slot; ours schedules cb_flag earlier. |
| `lib/eecdvd_sceCdCallback` | `_sceCdSC` | 44 | parked | the original is a compare chain (no jump table) with the arms in reverse case order; a switch here gives a table. |
| `lib/hncAt3dec` | `hncAt3dec_Idle` | 72 | parked | same structure, but the registers differ (orig: out s1, out2 s0, stereo/count s3, i s2). |
| `lib/libat3dec` | `unpack_mddata` | 316 | parked | structure right; the gain loops keep &gain[0][0][1] in a register and reload 255 (ours hoists it) |
| `lib/libat3dec` | `gainc_window` | 161 | parked | 23 lines off: the original sets the return value -1 at the top of the loop (the conditional returns branch straight to the epilogue) |
| `lib/libat3dec` | `channel_conv_joint` | 122 | parked | close: loop.c bases the inner loop on the channel 0 samples (the original on channel 1) and strength-reduces the band offset (the original recomputes (3 band) 136 from a counter) |
| `lib/libat3dec` | `at3dec_proc` | 97 | parked | 20 lines off: the original enters the copy loop with the byte the skip loop loaded (reorg), registers differ |
| `lib/libat3dec` | `f2t_nodata` | 90 | parked | 5 lines off: the band address of the loop is (x << 4) + x with swapped registers |
| `lib/libat3dec` | `imdct` | 389 | not attempted | keeps t9 live across the calls into at3core.s with sq/lq caller-saves; parameters go through the stack |
| `lib/nodebug/__errno` | `memmove` | 63 | parked | memcpy/memmove: only register allocation differs (the original moves dst0 out of a0 into t0, a0 then holds the loop temporaries) |
| `lib/nodebug/__errno` | `memcpy` | 43 | parked | memcpy/memmove: only register allocation differs (the original moves dst0 out of a0 into t0, a0 then holds the loop temporaries) |
| `lib/nodebug/__errno` | `strncmp` | 114 | hand-written | MMI string routines (128-bit loads and compares) |
| `lib/nodebug/__errno` | `strncpy` | 114 | hand-written | MMI string routines (128-bit loads and compares) |
| `lib/nodebug/__errno` | `strchr` | 104 | hand-written | MMI string routines (128-bit loads and compares) |
| `lib/nodebug/__errno` | `strcmp` | 83 | hand-written | MMI string routines (128-bit loads and compares) |
| `lib/nodebug/__errno` | `strlen` | 81 | hand-written | MMI string routines (128-bit loads and compares) |
| `lib/nodebug/__errno` | `strcat` | 76 | hand-written | MMI string routines (128-bit loads and compares) |
| `lib/nodebug/__errno` | `strcpy` | 70 | hand-written | MMI string routines (128-bit loads and compares) |
| `lib/nodebug/__errno` | `memset` | 46 | hand-written | MMI string routines (128-bit loads and compares) |
| `lib/nodebug/strround` | `fftoa` | 367 | parked | fftoa: register allocation still differs (x, exp, n, ... one s-register off) |
| `lib/nodebug/strround` | `add_separators` | 108 | parked | add_separators: count and the constant 2 swap t0/t1 |
| `lib/nodebug/strround` | `_vfprintf_r` | 1041 | not attempted |  |
| `lib/nodebug/_setlocale_r` | `memchr` | 59 | hand-written | MMI (128-bit compares) |
| `lib/kprintf2` | `_sceSnprintf` | 837 | parked | 4 lines differ (permuter score) |
| `lib/kprintf2` | `printfloat` | 118 | parked | 16 lines differ (permuter score) |
| `lib/kprintf2` | `ftoi` | 36 | parked | 21 lines differ (permuter score) |
| `lib/sifcmd` | `_sceSifCmdIntrHdlr` | 82 | parked | the C below has the original's shape and size, but its registers differ (pkt/size in a2/a0 instead of a3/a1, the scaled index kept apart from id) |
| `lib/sifrpc` | `sceSifCallRpc` | 128 | parked | the original copies the packet fields in strict load/store order, ours lets the scheduler hoist loads |
| `lib/sifrpc` | `sceSifExecRequest` | 113 | parked | the original copies the packet fields in strict load/store order, ours lets the scheduler hoist loads |
| `lib/sifrpc` | `sceSifInitRpc` | 103 | parked | 62 lines differ (permuter score) |
| `lib/sifrpc` | `_request_rdata` | 52 | parked | the original copies the packet fields in strict load/store order, ours lets the scheduler hoist loads |
| `lib/sifrpc` | `_request_end` | 51 | parked | the original copies the packet fields in strict load/store order, ours lets the scheduler hoist loads |
| `lib/sifrpc` | `_request_bind` | 51 | parked | the original copies the packet fields in strict load/store order, ours lets the scheduler hoist loads |
| `lib/sifrpc` | `_sceRpcGetPacket` | 42 | parked | the original copies the packet fields in strict load/store order, ours lets the scheduler hoist loads |
| `lib/sifrpc` | `_request_call` | 35 | parked | the original copies the packet fields in strict load/store order, ours lets the scheduler hoist loads |
| `lib/sifrpc` | `sceSifCheckStatRpc` | 15 | parked | the original copies the packet fields in strict load/store order, ours lets the scheduler hoist loads |
| `lib/filestub` | `_sceFs_Rcv_Intr` | 237 | parked | 14 lines differ (permuter score) |
| `lib/timer` | `cbTimerHandler` | 173 | parked | scheduling (53 lines differ) |
| `lib/timer` | `SetNextComp` | 100 | parked | loop shape (15 lines differ) |
| `lib/timer` | `TimerBusClock2Freq` | 21 | parked | the original keeps div.s out of the call's delay slot (only ee-gcc2.96 does that) |
| `lib/timer` | `TimerFreq2BusClock` | 13 | parked | as TimerBusClock2Freq |

### Data sections from the original assembly

Not listed in the unit's `// SOURCE DATA:` line.

| unit | sections |
|---|---|
| `lib/graphdev_sceGsExecStoreImage` | .data .rodata |
| `lib/libat3dec` | .data .rodata |
| `lib/nodebug/__assert` | .rodata |
| `lib/kprintf2` | .rodata |
| `lib/src/gcc/libgcc2___umoddi3` | .rodata |

Totals: 16 units without a source file (1 with code); 20 units with INCLUDE_ASM functions: 43 parked, 5 not attempted, 27 hand-written; 5 units take a data section from assembly.

Generated by `tools/leftovers.py lib`.
<!-- leftovers:lib:end -->

## Hand-written `.s` sources (done, listed for completeness)

`src/lib/at3core.s`, `src/lib/klib.s`, `src/lib/tlbtrap.s`, `src/lib/nodebug/_init.s`.

## Permuter sweep (2026-10-21)

`tools/permuter.py`, 5 minutes per parked function (18 parallel compiles). Score: differing instruction lines,
0 = match. No function reached a match; the improved variants only reorder independent statements, swap operands
or add temporaries, and were not adopted into the drafts (rerun the permuter to get them back).

| unit | function | start | best | candidates |
|---|---|---|---|---|
| `lib/libpad2` | `scePad2CreateSocket` | 2 | 2 | 36071 |
| `lib/nodebug/check_dll` | `snDllUnload` | 3 | 3 | 28149 |
| `lib/kprintf2` | `_sceSnprintf` | 4 | 4 | 24377 |
| `lib/libat3dec` | `f2t_nodata` | 5 | 5 | 40707 |
| `lib/sifrpc` | `_sceRpcGetPacket` | 5 | 5 | 43216 |
| `lib/nodebug/check_dll` | `relocate_dll` | 5 | 5 | 26540 |
| `lib/csc` | `_doCSC2` | 7 | 7 | 40714 |
| `lib/mpc` | `_getAllRefs` | 9 | 9 | 14356 |
| `lib/csc` | `_sceMpegCscStoreRefImage` | 9 | 9 | 41037 |
| `lib/timer` | `TimerBusClock2Freq` | 10 | 10 | 0 |
| `lib/sifrpc` | `_request_rdata` | 11 | 11 | 57190 |
| `lib/nodebug/strround` | `add_separators` | 11 | 11 | 34164 |
| `lib/sifrpc` | `sceSifCallRpc` | 12 | 12 | 37453 |
| `lib/libpad2` | `scePad2GetButtonInfo` | 12 | 12 | 27728 |
| `lib/pack` | `sceMpegDemuxPssRing` | 14 | 14 | 13932 |
| `lib/filestub` | `_sceFs_Rcv_Intr` | 14 | 14 | 12407 |
| `lib/timer` | `SetNextComp` | 15 | 15 | 32159 |
| `lib/nodebug/__errno` | `memcpy` | 15 | 15 | 48759 |
| `lib/timer` | `TimerFreq2BusClock` | 16 | 16 | 0 |
| `lib/kprintf2` | `printfloat` | 16 | 16 | 41578 |
| `lib/libat3dec` | `at3dec_proc` | 20 | 20 | 40065 |
| `lib/eecdvd_sceCdCallback` | `_sceCd_cbLoop` | 20 | 20 | 29755 |
| `lib/sifrpc` | `_request_bind` | 21 | 21 | 46065 |
| `lib/kprintf2` | `ftoi` | 21 | 21 | 29054 |
| `lib/sifrpc` | `sceSifCheckStatRpc` | 25 | 25 | 11 |
| `lib/mpc` | `_sceMpegMbAddressIncrement` | 28 | 28 | 19223 |
| `lib/sifrpc` | `sceSifExecRequest` | 28 | 28 | 41720 |
| `lib/libat3dec` | `gainc_window` | 31 | 29 | 35959 |
| `lib/hncAt3dec` | `hncAt3dec_Idle` | 36 | 36 | 53651 |
| `lib/sifcmd` | `_sceSifCmdIntrHdlr` | 36 | 36 | 44663 |
| `lib/eecdvd_sceCdCallback` | `sceCdLayerSearchFile` | 37 | 37 | 22756 |
| `lib/eecdvd_sceCdCallback` | `sceCdInit` | 42 | 38 | 24252 |
| `lib/sifrpc` | `_request_call` | 45 | 45 | 48696 |
| `lib/timer` | `cbTimerHandler` | 53 | 51 | 29222 |
| `lib/sifrpc` | `_request_end` | 62 | 62 | 58153 |
| `lib/nodebug/__errno` | `memmove` | 62 | 62 | 47810 |
| `lib/graphdev_sceGsExecStoreImage` | `sceGsExecStoreImage` | 88 | 66 | 32270 |
| `lib/eecdvd_sceCdCallback` | `_sceCdSC` | 73 | 73 | 51205 |
| `lib/libat3dec` | `channel_conv_joint` | 95 | 79 | 36362 |
| `lib/graphdev_checkModelVersion` | `sceGsSetDefDispEnv` | 113 | 113 | 47180 |
| `lib/libat3dec` | `unpack_mddata` | 162 | 162 | 33164 |
| `lib/nodebug/strround` | `fftoa` | 199 | 162 | 25625 |
| `lib/mpc` | `_getRef0` | 182 | 181 | 16990 |

Not searchable this way: `TimerBusClock2Freq`, `TimerFreq2BusClock`, `sceSifCheckStatRpc` (too small for the
mutations; the timer pair differs in where div.s is placed). The drafts that are stuck at a low score differ in
register choices that no statement-level mutation reaches.
