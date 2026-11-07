# SDK (lib/) leftovers

The reference for the library units (`lib/...`: Sony SDK, libat3dec, libgcc, newlib, libsn): what is left, how to
work on it and what has been tried. All library units build byte-identical; 921 of 985 functions are matched
(135 of the 153 units with a source are complete), plus 16 units that have no source file (data only, except
`lib/nodebug/memcmp`). The inventory below is generated from the sources by `tools/leftovers.py lib --update
docs/sdk-leftovers.md`: the status and the "what differs" text come from the `Parked:` / `Hand-written:` /
`Not attempted:` note above each function. Keep those notes current in the source, then regenerate.

Status:
- **not attempted**: no C written yet.
- **parked**: C written, kept under `#if 0` next to the `INCLUDE_ASM`, with a note on what still differs.
  It was parked after many variants stopped improving the diff.
- **hand-written**: no compiler output to match (MMI, trap handlers, `.s` sources). These stay assembly.

To re-check: `tools/check.sh` lists every unit with INCLUDE_ASM functions left; `tools/funcdiff.py <unit> <fn>`
shows a draft's diff after you switch its `#if 0` to `#if 1` (the permuters switch drafts on in their own copies).

## Working on a library unit

- **Toolchain:** per family, set in `config/SLUS_217.99/build.yaml` `overrides:`; the table and the assembler
  rules are in CLAUDE.md ("Library units"). A function that only matches another compiler usually needs a source
  change, not another toolchain.
- **No `ref/`**, no `__LINE__`/assert strings: `.rodata` is constants, strings and jump tables. The .mdebug has
  names only (`stdump functions iso/SLUS_217.99` lists each file's functions, `static` for file-local ones).
  SDK declarations go in `include/sdk/*.h`, library-internal structs in the unit's source or `src/lib/*.h`.
- **Delay slots:** with GNU as (`.set reorder`) the assembler fills open delay slots, but only when no label
  precedes the branch, and `-g`'s `$LM` line labels count: statements on one line, or a `do { } while (0)` (loop
  note), change which slots get filled.
- **Split units** (`graphdev_*`, `libgcc2_*`) are one function per object. Weak-function and vtable rules don't
  apply (all C).
- **Searching:** `tools/permuter.py <unit> <fn>` (text mutations) and, for these C units,
  `tools/decomp_permuter.py <unit> <fn> --minutes N -j N` (AST randomizations; see `docs/plan.md`, the per-function loop step 4).
  Read each result: its randomizations can change the meaning or depend on noise.
- **Validation:** funcdiff, then the full build's SHA-1 (`ninja`), then `tools/check.sh` before a merge. funcdiff
  normalizes relocation targets, so a swap of two globals' `%hi` registers in the same 64 KB page shows as a
  MATCH that the SHA-1 rejects (`_sceCd_cbLoop`).
- **Data:** a unit whose `.rodata` still comes from asm (`kprintf2`, `libat3dec`, ...) strips the compiled
  `.rodata`: a draft with string literals or double constants doesn't link there (`printfloat`); the original's
  labels as externs work for strings but not for doubles (not constants any more).

## Levers that worked in lib/

- A constant in a variable set before the stores it should precede (`_getAllRefs`'s `2`, `at3dec_proc`'s `0xF8`).
- An early `if (x < 0) return x;` instead of an `if` around the success path (`scePad2CreateSocket`).
- A statement duplicated into both arms of the preceding `if` (`add_separators`' `count++`).
- `do { ... } while (0)` around one statement or block as a scheduling barrier / loop note (`relocate_dll`'s
  `patch()`, `_request_call`'s `fno` copy, `_doCSC2`'s DMA kick).
- A pointer or field read into a local (`_request_end`'s client and `buff`, `cbTimerHandler`'s time argument,
  libppp's `pppSetMatrix` after a `"memory"` asm).
- A callee declared `int` instead of `void` (v0 not free after the call: `snDllUnload`'s `snAllBreaksIn`).
- `volatile` on single struct fields (reloads), a parameter reused as loop index or result (shares its
  s-register) (libkernl).
- printf family: one variable for several roles (the original's `n`/`p`/`uval` share s-registers), the tail
  `start = p + 1; len = bufend - p; goto output;` per case, `if (val >= 0) {...} else '-'` arm order.
- Sony's code has inline asm blocks with their own `.set noreorder` loops (DIntr).

## Notes per unit

- **sifrpc** (9 parked): the original copies packet fields load/store pair by pair; ours lets the scheduler hoist
  loads above stores of other pointer/int types (alias sets); `-fno-strict-aliasing` and scheduling flags don't
  reproduce it. Locals for the copied pointer/field and a `do { } while (0)` around one copy got `_request_end`
  (47 -> 11) and `_request_call` (45 -> 8) close; try the same on the others. Same family: sifcmd's
  `_sceSifCmdIntrHdlr` (registers only).
- **kprintf2** (`_sceSnprintf`, `printfloat`, `ftoi` parked; `.rodata` stays asm): `_sceSnprintf` is off only by a
  `nop` between `c.eq.s` and `bc1f` (gcc writes `#nop`); no assembler here gives that nop together with the
  unit's other code (2.9 gas inserts it but also one after `mtc1` and fills no `j $31`; 2.10/2.12 insert none).
  `printfloat` matches with its literals compiled (ftoi's result in a temporary first), so it waits for the
  unit's `.rodata` from source.
- **filestub** (`_sceFs_Rcv_Intr`, jump table as file-scope asm): only the prologue differs, two swapped registers
  around the `ee_retbuf_sel` store (an empty `asm volatile` barrier moves it, but isn't source).
  `~/kh1/src/lib/libkernl/filestub.c` (an older libkernl, matched) was the base; ours adds named semaphores,
  sceFsSifCallRpc retries, double-buffered replies.
- **graphdev_sceGsExecStoreImage**: the per-PSM switch matches once each case is `if (bytes == 0) {...} else
  {...}` (gcc merges the identical then-parts); left: registers in the PSMCT24 tail and the original keeping the
  raw trxreg (t1) for the final TRXREG write.
- **graphdev_checkModelVersion** (`sceGsSetDefDispEnv`, ~100 lines): structure matches (if/else if chain, not
  switch; DX = dx*magh + ((long)off[0] + 636) through a `long` temp); left: the original computes DY and
  off[0]+636 before magh and ORs the DISPLAY fields in another order. String kept by an unused static inline.
- **csc** (`_doCSC2`, `_sceMpegCscStoreRefImage`; the handlers match): sceMpegCbData must be the 32-byte union
  (stack layout); the IPU command store + cmd-type check is an inline taking cmd as parameter; left: register
  swaps (rgb/qwc s1/s2 in the inlined small-picture CSC block).
- **libpad2** (`scePad2GetButtonInfo`, 5 lines): needs `if (valid) { switch ... }`; keeps the button type in a1
  instead of a3. Lever here: the shared `return state` with the fail block inside the else branch (goto into it).
- **hncAt3dec** (`hncAt3dec_Idle`): structure matches (per-channel `final_dout` loops counting down); only the
  register assignment differs. Init matched once each switch case sets all three values itself.
- **eecdvd_sceCdCallback**: `_sceCd_cbLoop` matches by funcdiff with the exit block on one line and
  `sceCdCbfunc_num = cb_flag = 0`, but two hoisted luis swap registers (SHA-1 fails, gcse order of the symbols).
- **mpc** (`_getRef0`): decomp-permuter can't parse it (a statement expression `({ ... })` in a condition).
- **timer** (`TimerBusClock2Freq`, `TimerFreq2BusClock`): the original keeps `div.s` out of the call's delay
  slot, which only ee-gcc2.96 does; the unit is built with the Sony toolchain (991111).
- **lib/init**: a `j $31` left unfilled where the original filled it: the last statement and `}` on one line.

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
| `lib/nodebug/snDebugInit` | `snTeq` | 26 | hand-written | trap handler |
| `lib/graphdev_checkModelVersion` | `sceGsSetDefDispEnv` | 221 | parked | matches except register order in the DISPLAY computation (orig: DY and off[0] + 636 first). |
| `lib/graphdev_sceGsExecStoreImage` | `sceGsExecStoreImage` | 419 | parked | close, but register allocation differs (the PSMCT24 tail, trxreg.RRW kept extracted instead of raw). |
| `lib/libpad2` | `scePad2GetButtonInfo` | 56 | parked | 5 lines - button type kept in a1 (orig a3), movz instead of movn for the digital case. |
| `lib/output` | `_dispRefImageField` | 264 | parked | 13 of 272 lines, same size, _dispRefImage's body twice with showCount known to be 1 (the second __muldi3 folds). Left: in the second body the value is in a1 for v1, step in v1 for a0, sb in a0 for a1 (a local for the value only adds a truncation; six shapes of that body, 13-16); `lw t1` (hasPts, a … |
| `lib/csc` | `_sceMpegCscStoreRefImage` | 310 | parked | _doCSC2 3 lines (6 before the do-while(0) around the DMA kick) - registers; _sceMpegCscStoreRefImage in s1/s2 for the inlined CSC block's rgb/qwc. |
| `lib/csc` | `_doCSC2` | 112 | parked | _doCSC2 3 lines (6 before the do-while(0) around the DMA kick) - registers; _sceMpegCscStoreRefImage in s1/s2 for the inlined CSC block's rgb/qwc. |
| `lib/pack` | `sceMpegDemuxPssRing` | 248 | parked | 4 lines off (2026-11-07, was 6): sw v0,32(sp) and the zero store of ret are swapped in _bsInit's store group, and allGp reloads into v1 where the original has v0. ret after _bsInit moves more of the group (10), before it with a barrier moves the frame setup (18). |
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
| `lib/hncAt3dec` | `hncAt3dec_Idle` | 72 | parked | same structure, but the registers differ (orig: out s1, out2 s0, stereo/count s3, i s2). |
| `lib/libat3dec` | `unpack_mddata` | 316 | parked | structure right; the gain loops keep &gain[0][0][1] in a register and reload 255 (ours hoists it) |
| `lib/libat3dec` | `gainc_window` | 161 | parked | 11 lines off (23 before the early `lt` compare, decomp-permuter): the original sets the return value -1 at the top of the loop (the conditional returns branch straight to the epilogue) |
| `lib/libat3dec` | `channel_conv_joint` | 122 | parked | close: loop.c bases the inner loop on the channel 0 samples (the original on channel 1) and strength-reduces the band offset (the original recomputes (3 band) 136 from a counter) |
| `lib/libat3dec` | `at3dec_proc` | 97 | parked | 11 lines off (20 before the 0xF8 pad byte in a variable, decomp-permuter): the original enters the copy loop with the byte the skip loop loaded (reorg), registers differ |
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
| `lib/nodebug/strround` | `_vfprintf_r` | 1041 | not attempted |  |
| `lib/nodebug/_setlocale_r` | `memchr` | 59 | hand-written | MMI (128-bit compares) |
| `lib/kprintf2` | `_sceSnprintf` | 837 | parked | 4 lines differ (permuter score) |
| `lib/kprintf2` | `printfloat` | 118 | parked | 16 lines differ - blocked by the unit's asm .rodata: with its literals compiled (decomp-permuter's build) the ftoi result in a temporary before the "0.%d" call matches; the original's D_004FEC90.. labels as externs do not (69 lines: extern doubles are not constants) |
| `lib/kprintf2` | `ftoi` | 36 | parked | 9 lines differ |
| `lib/sifcmd` | `_sceSifCmdIntrHdlr` | 82 | parked | the C below has the original's shape and size, but its registers differ (pkt/size in a2/a0 instead of a3/a1, the scaled index kept apart from id) |
| `lib/sifrpc` | `sceSifCallRpc` | 128 | parked | the original copies the packet fields in strict load/store order, ours lets the scheduler hoist loads |
| `lib/sifrpc` | `sceSifExecRequest` | 113 | parked | the original copies the packet fields in strict load/store order, ours lets the scheduler hoist loads |
| `lib/sifrpc` | `sceSifInitRpc` | 103 | parked | 62 lines differ (permuter score) |
| `lib/sifrpc` | `_request_rdata` | 52 | parked | the original copies the packet fields in strict load/store order, ours lets the scheduler hoist loads |
| `lib/sifrpc` | `_request_end` | 51 | parked | 11 lines (47 before the client/buff locals, decomp-permuter) - the original tests 0x8000000A first with a range check (more cases in its switch?), ours 0x80000009 |
| `lib/sifrpc` | `_request_bind` | 51 | parked | the original copies the packet fields in strict load/store order, ours lets the scheduler hoist loads |
| `lib/sifrpc` | `_request_call` | 35 | parked | 8 lines, 4 bytes short (45 before the do-while(0) around the fno copy, decomp-permuter); the rest of the field copies still let the scheduler hoist loads |
| `lib/sifrpc` | `sceSifCheckStatRpc` | 15 | parked | the original copies the packet fields in strict load/store order, ours lets the scheduler hoist loads |
| `lib/filestub` | `_sceFs_Rcv_Intr` | 237 | parked | 14 lines differ (permuter score) |
| `lib/timer` | `cbTimerHandler` | 173 | parked | scheduling (32 lines; 45 before the handler's time argument in a local, decomp-permuter) |
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

Totals: 16 units without a source file (1 with code); 19 units with INCLUDE_ASM functions: 36 parked, 2 not attempted, 27 hand-written; 5 units take a data section from assembly.

Generated by `tools/leftovers.py lib`.
<!-- leftovers:lib:end -->

## Hand-written `.s` sources (done, listed for completeness)

`src/lib/at3core.s`, `src/lib/klib.s`, `src/lib/tlbtrap.s`, `src/lib/nodebug/_init.s`.

## Search history

- **2026-10-21, `tools/permuter.py`**, 5 minutes per parked function: no match; the improvements only reordered
  independent statements, swapped operands or added temporaries and were not adopted.
- **2026-11-05, `tools/decomp_permuter.py`**, 8 minutes per parked C function (lines: funcdiff in the real source):

  | result | functions |
  |---|---|
  | matched | `relocate_dll`, `snDllUnload` (check_dll), `scePad2CreateSocket` (libpad2), `_getAllRefs` (mpc), `add_separators` (strround) |
  | improved | `_request_end` 47 -> 11, `_request_call` 45 -> 8 (sifrpc); `cbTimerHandler` 45 -> 32 (timer); `gainc_window` 23 -> 11, `at3dec_proc` 20 -> 11 (libat3dec); `ftoi` 17 -> 9 (kprintf2); `scePad2GetButtonInfo` 12 -> 5 (libpad2); `_doCSC2` 6 -> 3 (csc) |
  | false or blocked | `_sceCd_cbLoop` (funcdiff MATCH, SHA-1 fails), `printfloat` (matches only with the unit's `.rodata` from source) |
  | rejected (changed the meaning or depended on noise) | `hncAt3dec_Idle`, `_sceFs_Rcv_Intr`, `unpack_mddata`, `sceSifCallRpc`, `TimerFreq2BusClock` |
  | not parseable | `_getRef0` (statement expression) |
  | no gain | the others (`sceGsSetDefDispEnv`, `sceGsExecStoreImage`, `memcpy`/`memmove`, `fftoa`, `_sceSnprintf`, the rest of sifrpc, `SetNextComp`, ...) |

  Logs: `build/dpq.log`; outputs per function in `build/decomp-permuter/<function>/`.
