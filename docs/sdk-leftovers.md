# SDK (lib/) leftovers

The `lib/` units that are not fully decompiled, as of branch `sdk-decomp` at 2026-10-21. All 167 lib units build
byte-identical. The entries below are what still comes from the original assembly, as a whole unit, a function
or a data section. Counts are instructions (`tools/asm.py <unit>`).

Status:
- **not attempted**: no C written yet.
- **parked**: C written, kept under `#if 0` next to the `INCLUDE_ASM`, with a comment on what still differs.
  It was parked after many variants stopped improving the diff.
- **hand-written**: no compiler output to match (MMI, trap handlers, `.s` sources). These stay assembly.

To re-check: `tools/check.sh` lists every unit with INCLUDE_ASM functions left. `tools/funcdiff.py <unit>`
shows a draft's diff after you switch its `#if 0` to `#if 1`.

## Units with no source file

These have no `src/` file, so their code or data comes entirely from the original assembly.

| unit | kind | status |
|---|---|---|
| `lib/nodebug/memcmp` | code (37 + 6 + 3 instr: memcmp, sn_reg_frame, sn_dereg_frame), bss | not attempted (no debug info) |
| `lib/table` | rodata (libat3dec Huffman/dequant tables) | not attempted |
| `lib/nodebug/_ctype_` | rodata | not attempted |
| `lib/nodebug/blanks.12` | rodata | not attempted |
| `lib/nodebug/lconv` | rodata | not attempted |
| `lib/nodebug/huge` | rodata | not attempted |
| `lib/nodebug/sn___infinity` | rodata | not attempted |
| `lib/src/newlib/libm/math/s_infconst` | rodata | not attempted |
| `lib/src/newlib/libm/common/s_lib_ver` | rodata | not attempted |
| `lib/nodebug/impure_data` | data | not attempted |
| `lib/nodebug/SN_LOADED_RELMODS` | data | not attempted |
| `lib/nodebug/__CTOR_LIST__` | data | not attempted |
| `lib/nodebug/__FRAME_BEGIN__0` | data | not attempted |
| `lib/nodebug/_GLOBAL__F_0build.cppF7aaaa` | data | not attempted |

## Functions still assembly in units with a source file

### Middleware (libmpeg, ATRAC3)

| unit | function | instr | status | what differs |
|---|---|---|---|---|
| `lib/mpc` | `_getAllRefs` | 450 | parked | 7 lines: in the field-picture setup the original schedules the constant 2 of the `x160 == 2` test before `sel = 0` (registers of the refs loads) |
| `lib/mpc` | `_getRef0` | 290 | parked | same structure and size; register allocation (original keeps y in s1, n in t9, spills `avg << 2`) |
| `lib/mpc` | `_sceMpegMbAddressIncrement` | 94 | parked | the loop keeps the IPU_CMD address in a saved register (original rebuilds it each pass) |
| `lib/mpc` | `_copyAddRefImage`, `_rix_000`..`_rix_111`, `_ri0_000`..`_ri0_111` (17 functions) | 31-73 each | hand-written | MMI reference-copy routines |
| `lib/pack` | `sceMpegDemuxPssRing` | 248 | parked | 6 lines: the zero store of allData, the delay slot of the first pack-header test, the reload register of allGp |
| `lib/libat3dec` | `imdct` | 390 | not attempted | keeps t9 live across the calls into at3core.s with sq/lq caller-saves; parameters go through the stack |
| `lib/libat3dec` | `unpack_mddata` | 316 | parked | gain loops: the original keeps `&gain[0][0][1]` in a register and reloads 255 (ours hoists it) |
| `lib/libat3dec` | `gainc_window` | 162 | parked | 23 lines: the original sets the return value -1 at the top of the loop (conditional returns branch straight to the epilogue) |
| `lib/libat3dec` | `channel_conv_joint` | 122 | parked | loop.c bases the inner loop on channel 0 (original: channel 1) and strength-reduces the band offset (original recomputes `(3 * band) * 136`) |
| `lib/libat3dec` | `at3dec_proc` | 97 | parked | 20 lines: the original enters the copy loop with the byte the 0xF8-skip loop loaded; registers differ |
| `lib/libat3dec` | `f2t_nodata` | 90 | parked | 5 lines: the band address `(x << 4) + x` with swapped registers |
| `lib/libat3dec` | data | - | data from asm | `.data` (coefpost, cbi) and `.rodata` (qmf tables, scaleDiv, assert strings) not from source; the C refers to the assert strings by their `D_` labels |
| `lib/hncAt3dec` | `hncAt3dec_Idle` | 72 | parked | same structure; registers (original: out s1, out2 s0, stereo/count s3, i s2) |
| `lib/output` | `_dispRefImageField` | 264 | not attempted | |
| `lib/output` | `_dispRefImage` | 176 | not attempted | |
| `lib/output` | `_cpr8` | 164 | not attempted | |
| `lib/csc` | `_sceMpegCscStoreRefImage` | 311 | parked | registers of the inlined CSC block (rgb/qwc in s1/s2) |
| `lib/csc` | `_doCSC2` | 112 | parked | two registers (0xffc0 / D3_CHCR) |

### Sony kernel and device libraries

| unit | function | instr | status | what differs |
|---|---|---|---|---|
| `lib/sifrpc` | `sceSifCallRpc` | 128 | parked | the original copies packet fields in strict load/store order; ours lets the scheduler hoist loads (applies to all sifrpc drafts) |
| `lib/sifrpc` | `sceSifExecRequest` | 114 | parked | same |
| `lib/sifrpc` | `sceSifInitRpc` | 104 | not attempted | |
| `lib/sifrpc` | `_request_end` | 52 | parked | load/store order |
| `lib/sifrpc` | `_request_rdata` | 52 | parked | load/store order |
| `lib/sifrpc` | `_request_bind` | 52 | parked | load/store order |
| `lib/sifrpc` | `_sceRpcGetPacket` | 42 | parked | load/store order |
| `lib/sifrpc` | `_request_call` | 36 | parked | load/store order |
| `lib/sifrpc` | `sceSifCheckStatRpc` | 16 | parked | load/store order |
| `lib/sifcmd` | `_sceSifCmdIntrHdlr` | 83 | parked | same shape and size; pkt/size in a2/a0 (original a3/a1), scaled index kept apart from id |
| `lib/timer` | `cbTimerHandler` | 174 | parked | scheduling |
| `lib/timer` | `SetNextComp` | 100 | parked | loop shape |
| `lib/timer` | `TimerBusClock2Freq` | 22 | parked | the original keeps div.s out of the call's delay slot (only ee-gcc2.96 does that) |
| `lib/timer` | `TimerFreq2BusClock` | 14 | parked | same |
| `lib/filestub` | `_sceFs_Rcv_Intr` | 243 | parked | |
| `lib/eecdvd_sceCdCallback` | `sceCdLayerSearchFile` | 192 | parked | the 2nd/3rd SCE_CD_debug reloads land in v0 in the original, v1 here |
| `lib/eecdvd_sceCdCallback` | `sceCdInit` | 186 | parked | t0/t1 swapped for the `_scmd_dr_bind`/`_sceCd_ee_read_mode` %hi; order of li/lui before the `_sceCd_CdInit_ON` store |
| `lib/eecdvd_sceCdCallback` | `_sceCd_cbLoop` | 52 | parked | exit block: the original keeps cb_flag for the delay slot, ours schedules it earlier |
| `lib/eecdvd_sceCdCallback` | `_sceCdSC` | 44 | parked | the original is a compare chain with arms in reverse case order; a switch gives a jump table |
| `lib/libpad2` | `scePad2CreateSocket` | 66 | parked | the pad2_info address (lui) is scheduled before the 820 multiplier |
| `lib/libpad2` | `scePad2GetButtonInfo` | 56 | parked | button type in a1 (original a3), movz instead of movn for the digital case |
| `lib/graphdev_sceGsExecStoreImage` | `sceGsExecStoreImage` | 426 | parked | register allocation (PSMCT24 tail, trxreg.RRW kept extracted); also `.data`/`.rodata` not from source |
| `lib/graphdev_checkModelVersion` | `sceGsSetDefDispEnv` | 222 | parked | register order in the DISPLAY computation (original: DY and off[0] + 636 first) |
| `lib/kprintf2` | `_sceSnprintf` | 857 | parked | |
| `lib/kprintf2` | `printfloat` | 118 | parked | |
| `lib/kprintf2` | `ftoi` | 36 | parked | |
| `lib/kprintf2` | data | - | data from asm | `.rodata` not from source |

### libsn / newlib (`lib/nodebug`) and libgcc

| unit | function | instr | status | what differs |
|---|---|---|---|---|
| `lib/nodebug/strround` | `_vfprintf_r` | 1069 | not attempted | |
| `lib/nodebug/strround` | `fftoa` | 368 | parked | register allocation, one s-register off |
| `lib/nodebug/strround` | `add_separators` | 108 | parked | count and the constant 2 swap t0/t1 |
| `lib/nodebug/check_dll` | `relocate_dll` | 257 | parked | scheduling |
| `lib/nodebug/check_dll` | `snDllUnload` | 72 | parked | one register |
| `lib/nodebug/__errno` | `memcpy` | 43 | parked | register allocation (the original moves dst0 from a0 to t0) |
| `lib/nodebug/__errno` | `memmove` | 63 | parked | same |
| `lib/nodebug/__errno` | `memset`, `strcat`, `strchr`, `strcmp`, `strcpy`, `strlen`, `strncmp`, `strncpy` | 46-114 | hand-written | MMI string routines |
| `lib/nodebug/_setlocale_r` | `memchr` | 60 | hand-written | MMI |
| `lib/nodebug/snDebugInit` | `snTeq` | 27 | hand-written | trap handler |
| `lib/nodebug/__assert` | data | - | data from asm | `.rodata` not from source |
| `lib/src/gcc/libgcc2___umoddi3` | data | - | data from asm | `.rodata` ends the section; the padding up to `.sdata` is the original link's alignment |

## Hand-written `.s` sources (done, listed for completeness)

`src/lib/at3core.s`, `src/lib/klib.s`, `src/lib/tlbtrap.s`, `src/lib/nodebug/_init.s`.

## Totals

- 14 units without a source file (1 with code, 13 data-only).
- 22 units with a source file but functions or data still from assembly: 43 parked functions, 6 not
  attempted (`imdct`, `_vfprintf_r`, `sceSifInitRpc`, and the 3 `lib/output` display functions) and 27
  hand-written. 5 of the 22 units take a data section from assembly (2 of them have no functions left).

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
