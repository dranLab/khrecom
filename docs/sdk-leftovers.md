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
- **Data:** a unit whose `.rodata` still comes from asm strips the compiled `.rodata`: a draft with string literals
  or double constants doesn't link there (extern labels work for strings, not for doubles). Bring the data from
  source first, also with INCLUDE_ASM functions left: C literals for C-owned labels, `asm(".rdata\n.align 3\n...")`
  blocks of ROM words (a jump table as symbolic `.word .Lxxx`) right after each asm owner's INCLUDE_ASM; delete a
  block with its INCLUDE_ASM (memory note source-data-owner-blocks; kprintf2 2026-11-07, filestub).

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

- **sifrpc** (4 parked; the pair-by-pair copies are stores of alias set 0 for some fields, `_PKT_STORE`, see `union-store-alias-zero`; sceSifCallRpc matched with it, sceSifCheckStatRpc and sceSifExecRequest did not need it: their notes did not describe them): the original copies packet fields load/store pair by pair; ours lets the scheduler hoist
  loads above stores of other pointer/int types (alias sets); `-fno-strict-aliasing` and scheduling flags don't
  reproduce it. Locals for the copied pointer/field and a `do { } while (0)` around one copy got `_request_end`
  (47 -> 11) and `_request_call` (45 -> 8) close; try the same on the others. Same family: sifcmd's
  `_sceSifCmdIntrHdlr` (registers only).
- **kprintf2** (`_sceSnprintf` parked, 2 lines; data from source 2026-11-07, `printfloat` and `ftoi` matched):
  `_sceSnprintf` is off only by a `nop` between `c.eq.s` and `bc1f` (gcc writes `#nop`). GNU as 2.9-ee-991111
  (`ee-gcc2.9-991111{,-01}/ee/bin/as`) gives exactly that and matches every other function except `ftoi` and
  `kputchar`, whose `jr ra` slots it never fills (2.10 does; gcc can't: a DI->SI truncation of length 8, a
  volatile store). 2.10/2.12 insert no c.cond nop with any `-mcpu`/`-mipsN`; 990721's as also adds one after `mtc1`.
- **filestub** (`_sceFs_Rcv_Intr`, jump table as file-scope asm): only the prologue differs, two swapped registers
  around the `ee_retbuf_sel` store (an empty `asm volatile` barrier moves it, but isn't source).
  `~/kh1/src/lib/libkernl/filestub.c` (an older libkernl, matched) was the base; ours adds named semaphores,
  sceFsSifCallRpc retries, double-buffered replies.
- **graphdev_sceGsExecStoreImage** (matched 2026-11-07, data from source): each case `if (bytes == 0) {...} else
  {...}` (gcc merges the identical then-parts); the else arms' v0/v1 and the PSMCT24 tail kept out of the
  cross-jump = the shifted product in one function-level variable shared by every case (referenced in several
  blocks, so local-alloc can't tie it to the product); raw trxreg for the TRXREG write = `sp->trxreg.RRW` read
  again (gcse removes the load, not the and); `int` GsGetIMR/GsPutIMR; `rest` before `qwc`; the byte copy through
  a `{u_long128; u_char[16]}` union (sp + i operand order).
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
- **timer** (`TimerBusClock2Freq`, `TimerFreq2BusClock`, matched 2026-11-07 with the Sony group on 991111-01): both match as they stand under ee-gcc2.9-991111-01 (its `div.s`
  erratum padding keeps it out of the call's delay slot), and no function of the unit differs there. Needs an override.
- **lib/init**: a `j $31` left unfilled where the original filled it: the last statement and `}` on one line.
- **nodebug/__assert** (.rodata, needs a split change): the segment at 0x3FE5A0 starts with impure.o's 8-byte
  `.rodata` ("C", `_current_locale` of `_REENT_INIT`, pointed at by impure_data); from source only those 8 bytes
  differ. Needs a `[0x3FE5A0, rodata, lib/nodebug/impure_data]` segment linked before __assert (split_objects.py).
- **libgcc2___umoddi3** (.rodata, needs a link change): the table matches from source, but the segment also holds
  0x28 zero bytes up to .sdata at 0x3FF680, the original link's 128-byte section alignment (.sndata, .rodata,
  .sbss start 128-aligned too). Needs `. = ALIGN(0x80)` before `.sdata` in the linker script (configure.py) or a
  padding segment.

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
| `lib/libpad2` | `scePad2GetButtonInfo` | 56 | parked | 3 lines (2026-11-07, through a pointer to bi, decomp-permuter): case 8's address in v0 and the byte in v1, swapped. |
| `lib/csc` | `_sceMpegCscStoreRefImage` | 310 | parked | _doCSC2 3 lines (6 before the do-while(0) around the DMA kick) - registers; _sceMpegCscStoreRefImage in s1/s2 for the inlined CSC block's rgb/qwc. |
| `lib/csc` | `_doCSC2` | 112 | parked | _doCSC2 3 lines (6 before the do-while(0) around the DMA kick) - registers; _sceMpegCscStoreRefImage in s1/s2 for the inlined CSC block's rgb/qwc. |
| `lib/pack` | `sceMpegDemuxPssRing` | 248 | parked | 2 lines off (2026-11-07, was 4: allFunc set between ret and allGp, decomp-permuter): allGp reloads into v1 where the original has v0. Reload hands out spill registers round-robin, so the choice hangs on the reloads before it. |
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
| `lib/libat3dec` | `imdct` | 389 | parked | first draft (2026-11-07), 334 of 457 lines, 80 bytes short; the butterfly stage has its structure (registers renamed: w, half, step); the window loop keeps fewer values on the stack than the original (5 spills at 0x18-0x2C) and the band copy's tail is not interleaved with its setup; … |
| `lib/libat3dec` | `unpack_mddata` | 316 | parked | structure right; the gain loops keep &gain[0][0][1] in a register and reload 255 (ours hoists it) |
| `lib/libat3dec` | `gainc_window` | 161 | parked | 10 lines off (23 before the early `lt` compare, decomp-permuter): the original sets the return value -1 at the top of the loop (the conditional returns branch straight to the epilogue) |
| `lib/libat3dec` | `channel_conv_joint` | 122 | parked | close: loop.c bases the inner loop on the channel 0 samples (the original on channel 1) and strength-reduces the band offset (the original recomputes (3 band) 136 from a counter) |
| `lib/libat3dec` | `at3dec_proc` | 97 | parked | 11 lines off (20 before the 0xF8 pad byte in a variable, decomp-permuter): the original enters the copy loop with the byte the skip loop loaded (reorg), registers differ |
| `lib/libat3dec` | `f2t_nodata` | 90 | parked | 5 lines off: the band address of the loop is (x << 4) + x with swapped registers |
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
| `lib/kprintf2` | `_sceSnprintf` | 837 | parked | 2 of 853 lines, same size (2026-11-07): only the nop between c.eq.s and bc1f in the %f case (gcc writes "#nop"; the original's assembler inserted it at the ".set noreorder" before bc1f). The 991111/991111-01 ee/bin/as (GNU as 2.9-ee-991111) gives exactly that nop and the whole function matches, … |
| `lib/sifcmd` | `_sceSifCmdIntrHdlr` | 82 | parked | the C below has the original's shape and size, but its registers differ (pkt/size in a2/a0 instead of a3/a1, the scaled index kept apart from id) |
| `lib/sifrpc` | `sceSifInitRpc` | 103 | parked | 62 lines differ (permuter score) |
| `lib/sifrpc` | `_request_rdata` | 52 | parked | 7 of 59 lines (2026-11-07, was 10): the strict load/store order is stores of alias set 0: through a union, every later read reloads, also the call's arguments (int casts on both sides do not: cse keeps those). pkt_addr's store is plain: receive loads ahead of it. Left: v0/v1 alternate the other … |
| `lib/sifrpc` | `_request_end` | 51 | parked | 2 of 53 lines, same size (2026-11-07, was 11): the 0x8000000A-first range check is a third case node (RDATA, only break), the client a local before the tid test (kept in s1 across the call), the bind copy's buff load behind the serve store (empty barrier). Left: buff in v0 for v1: the barrier … |
| `lib/sifrpc` | `_request_bind` | 51 | parked | 4 of 59 lines, same size (2026-11-07, was 17): !sd arm first, both loads ahead of a barrier, server's store of alias set 0 (sd stays in v0). Left: `ori` and `move a1` against the stores of the same block; all six store orders tried. |
| `lib/sifrpc` | `_request_call` | 35 | parked | 3 lines, 4 bytes long (2026-11-07, was 8): !qd->start arm first, qd->end's store of alias set 0 (the copies' loads stay behind it), do/while(0) around the fno copy (without it 26). Left: the rsize load goes ahead of the store at 40 and an extra nop before the jr; a union store on any single field … |
| `lib/filestub` | `_sceFs_Rcv_Intr` | 237 | parked | 14 lines differ (permuter score) |
| `lib/timer` | `cbTimerHandler` | 173 | parked | scheduling (32 lines; 45 before the handler's time argument in a local, decomp-permuter) |
| `lib/timer` | `SetNextComp` | 100 | parked | loop shape (15 lines differ) |

### Data sections from the original assembly

Not listed in the unit's `// SOURCE DATA:` line.

| unit | sections |
|---|---|
| `lib/libat3dec` | .data .rodata |
| `lib/nodebug/__assert` | .rodata |
| `lib/src/gcc/libgcc2___umoddi3` | .rodata |

Totals: 16 units without a source file (1 with code); 17 units with INCLUDE_ASM functions: 28 parked, 1 not attempted, 27 hand-written; 3 units take a data section from assembly.

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
