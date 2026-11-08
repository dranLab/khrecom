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
  A segment that starts with another object's bytes or ends in section padding was a split/link problem, fixed
  2026-11-07: configure.py aligns each data section's start label to 128 (`_gp` follows cod_SDATA_START;
  libgcc2___umoddi3), and split_objects.py gives data-only objects without debug info their .rodata (impure.o's
  "C" before __assert).

## Levers that worked in lib/

- A constant in a variable set before the stores it should precede (`_getAllRefs`'s `2`, `at3dec_proc`'s `0xF8`).
- An early `if (x < 0) return x;` instead of an `if` around the success path (`scePad2CreateSocket`).
- A block of struct-field init stores: first try the struct's own field order, or the ps2sdk reimplementation's
  order where one exists, before any search (`sceSifInitRpc`: equal-priority ties go by rank_for_schedule's
  register weight, then the earlier luid, so the source order decides).
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
- The same few volatile reads repeated with different registers per copy, plus a store in a `jal` delay slot = a
  brace-block macro with block-local variables on one source line (`cbTimerHandler`'s `GET_TIME(now)`); `{ }`, not
  `do { } while (0)`, when an earlier statement has to schedule after the macro's volatile loads (2026-11-08).
- A conditional exit followed by an unconditional `b` back, the body peeled once before the loop = the `break` in
  the `else` arm: `if (t2 < t + 0x7333) t = t2; else break;` (`SetNextComp`).
- `addu ptr,idx` vs ours `idx,ptr` (gcc 2.96 puts a multiply first in an address): pointer + index in a store
  address = a byte offset with a shift, `*(int *)((char *)p + (k << 3))` (`unpack_mddata`); offset + base in an
  assigned pointer = an integer sum, offset first, `(int *)(k * 544 + (int)w)` (`f2t_nodata`, `channel_conv_joint`).
- `(x<<4)+x` vs ours `x+(x<<4)`: the original multiplies an unfolded value (`k = band * 3; k * 544`, not
  `band * 1632`).
- A caller's draft can depend on its static callee: `at3dec_proc` matched once `unpack_mddata` was C. Re-measure
  callers after matching a static callee.
- The last operand swap of a sum: store it back into one of its inputs (`y = n0 * (x >> 4) + n1 * (y >> 4)`).
- A value stored to a global and then used as an index: store the expression straight into the global and index by
  the global (gcse forwards it as a temporary, not a user variable); the address as one expression (filestub).
- An index left unreduced (`sll`/`addu` per iteration) plus a copy chain in a loop = a plain `+=` loop that gcse PRE
  gave a second entry (loop.c skips it); a draft that writes PRE's result out by hand gets strength-reduced instead
  (`hncAt3dec_Idle`). For 2.96 dumps: `-fsched-verbose=9` (with `=`); tools/helpers/schedshow.py doesn't parse them.
- A two-register swap inside one block that no priority change fixes, with exactly 3 local quantities in the block:
  local-alloc's hand-written sort for 2-3 quantities (`local-alloc.c` ~1380, gcc sources in the main checkout's
  `build/gccsrc/`) compares quantity numbers, so the first-born is allocated first; a 4th local quantity (a variable
  declared per arm, `int gp = SetGP(...)` crossing the call) restores priority order (`_sceSifCmdIntrHdlr`, 2026-11-08).
- `beqz` where the original has `blez` on a count from an 8-bit field: combine's known-bits turns `0 < n` into
  `n != 0` for a single-set value; reuse one variable for the size, the count and the index (`_sceSifCmdIntrHdlr`).
- The original's .mdebug locals (register, slot, scope blocks; `build/gat/mdsyms.py`) exist for the Sony units
  with debug info, not for libat3dec (names only).

## Notes per unit

- **sifrpc** done 24/24 (the four `_request_*` 2026-11-07, `sceSifInitRpc` 2026-11-08): the original copies packet
  fields load/store pair by pair; ours lets the scheduler hoist loads above stores of other types (alias sets;
  `-fno-strict-aliasing` and scheduling flags don't reproduce it). Fields of alias set 0 through a union fix it,
  on both sides: `_PKT_STORE` for a store later reads must not pass, `_PKT_ANY` for a read that must stay behind
  earlier stores (memory note `union-store-alias-zero`); search the plain/read/store combinations per field.
  A `do { } while (0)` far above a return label can add an alignment nop there (its loop note). `sceSifInitRpc`:
  early return for the RPCINIT test (tail call to sceSifSetReg); the init stores in the struct's field order with
  pid last (ps2sdk's SifInitRpc order). The old "sched2 tie" note was the statement order: a hill climb over all
  orders from 3 starts never reached it. (sifcmd's `_sceSifCmdIntrHdlr`, once listed as the same family, matched 2026-11-08 by source shape.)
- **kprintf2** (`_sceSnprintf` parked, 2 lines; data from source 2026-11-07, `printfloat` and `ftoi` matched):
  `_sceSnprintf` is off only by a `nop` between `c.eq.s` and `bc1f` (gcc writes `#nop`). GNU as 2.9-ee-991111
  (`ee-gcc2.9-991111{,-01}/ee/bin/as`) gives exactly that and matches every other function except `ftoi` and
  `kputchar`, whose `jr ra` slots it never fills (2.10 does; gcc can't: a DI->SI truncation of length 8, a
  volatile store). 2.10/2.12 insert no c.cond nop with any `-mcpu`/`-mipsN`; 990721's as also adds one after `mtc1`.
- **filestub** done 44/44 (2026-11-08): `_sceFs_Rcv_Intr` matched once the selector went straight into the global
  (`ee_retbuf_sel = _fs_rcv_bufdbl ? ((s32*)pkt)[3] : 0;`) and the address was one expression indexed by it
  (`rp = UNCACHED(&_rcv_data_cmd[ee_retbuf_sel]);`); the invented local `sel` was the cause, not a scheduling tie.
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
- **csc** (matched 2026-11-07, data from source): both "register swaps" were source shape: `*D_STAT = 8;
  EnableDmac(3);` on one line (as moves the volatile store into the jal slot), the `do { } while (0)` around exactly
  the `DIntr() ... if (oldei) EIntr();` critical section, and qwc (`mbnum * 0x40`) in its own local next to rgb.
- **libpad2** (matched 2026-11-07): `if (valid) { switch ... }` with every case returning; the v0/v1 and a0/a1
  swaps in case 8 came from case 1 written as a ternary into a shared `ret` (it gave the bit an a0 preference):
  `if (!x) return type; return 0;` there, and case 8's shift in a plain block-local `v`.
- **hncAt3dec** (done 2026-11-08): `hncAt3dec_Idle` was not a register problem but a hand-written PRE result: the
  mono loop is a plain `out += 0x100; i += 0x100;` loop with `out2 = out + 0x100` computed before the `if`; gcse PRE
  moves the add to the back edge (the original's copy chain and `move` in the `beqz` slot), and the block it adds
  gives the loop a second entry, so loop.c ignores it ("multiple entry points") and `i` stays unreduced. Init matched
  once each switch case sets all three values itself.
- **eecdvd_sceCdCallback**: `_sceCd_cbLoop` matches by funcdiff with the exit block on one line and
  `sceCdCbfunc_num = cb_flag = 0`, but two hoisted luis swap registers (SHA-1 fails, gcse order of the symbols).
- **mpc** (`_getRef0`): decomp-permuter can't parse it (a statement expression `({ ... })` in a condition).
- **timer** (`TimerBusClock2Freq`, `TimerFreq2BusClock`, matched 2026-11-07 with the Sony group on 991111-01): both match as they stand under ee-gcc2.9-991111-01 (its `div.s`
  erratum padding keeps it out of the call's delay slot); the `lib/*` override covers it. Unit done 36/36 (2026-11-08):
  `cbTimerHandler` (GET_TIME brace-block macro, `volatile` overflow field) and `SetNextComp` (break in the else arm).
- **libat3dec** (2026-11-08: 11/12, unpack_mddata, f2t_nodata, channel_conv_joint, at3dec_proc matched): only
  `imdct` is left (334 of 457 lines; register-blind still 148 of 390 instructions off; lead: the original stores
  sh1 = gain + shift - 1 into gain's stack slot, so the source probably reassigns `gain`). `build/fastcmp.py` /
  `build/mnemdiff.py`: ~1 s per variant without ninja.
- **lib/init**: a `j $31` left unfilled where the original filled it: the last statement and `}` on one line.
- **nodebug/__assert**, **libgcc2___umoddi3** (.rodata from source 2026-11-07): needed a split and a link change,
  see "Data:" above (impure.o's .rodata segment; the 128-byte section start alignment).

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
| `lib/nodebug/impure_data` | - | .data .rodata |
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
| `lib/libat3dec` | `imdct` | 389 | parked | first draft (2026-11-07), 334 of 457 lines, 80 bytes short; the butterfly stage has its structure (registers renamed: w, half, step); the window loop keeps fewer values on the stack than the original (5 spills at 0x18-0x2C) and the band copy's tail is not interleaved with its setup; … |
| `lib/nodebug/__errno` | `memmove` | 63 | parked | memcpy 11 of 43 lines, memmove 31 of 68 (2026-11-08): registers only, one cause in both: the original keeps the first parameter in t0, gives the quadword loop lq v1/sltiu a0/lq v0 (ours v0/v1/v0) and copies the byte loop's -1 from the entry test's register (li v0,-1; beq; move a0,v0; ours li … |
| `lib/nodebug/__errno` | `memcpy` | 43 | parked | memcpy 11 of 43 lines, memmove 31 of 68 (2026-11-08): registers only, one cause in both: the original keeps the first parameter in t0, gives the quadword loop lq v1/sltiu a0/lq v0 (ours v0/v1/v0) and copies the byte loop's -1 from the entry test's register (li v0,-1; beq; move a0,v0; ours li … |
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

### Data sections from the original assembly

Not listed in the unit's `// SOURCE DATA:` line.

| unit | sections |
|---|---|

Totals: 16 units without a source file (1 with code); 8 units with INCLUDE_ASM functions: 7 parked, 1 not attempted, 27 hand-written; 0 units take a data section from assembly.

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
  | rejected (changed the meaning or depended on noise) | `hncAt3dec_Idle` (matched 2026-11-08 by source shape), `_sceFs_Rcv_Intr`, `unpack_mddata`, `sceSifCallRpc`, `TimerFreq2BusClock` |
  | not parseable | `_getRef0` (statement expression) |
  | no gain | the others (`sceGsSetDefDispEnv`, `sceGsExecStoreImage`, `memcpy`/`memmove`, `fftoa`, `_sceSnprintf`, the rest of sifrpc, `SetNextComp`, ...) |
- **2026-11-08, `tools/decomp_permuter.py`** (after the typedef-attribute fix: earlier runs on units using `long128`
  compiled it as `int`, so their results don't count), 15 minutes each: `memcpy` 85k iterations, no gain; `memmove`
  outputs claimed 22 lines but changed the meaning (rejected). About 300 hand variants (all 120 declaration orders,
  loop forms, pointer types) gave memmove 38 -> 31 (`int len = length;` declared first). Left in both: the quadword
  loop's three local quantities get v1/a0/v0 in the original, which local-alloc can't give from this C, and the byte
  loop copies its -1 from the entry test's register, which no 2.95.3 pass creates from a constant: possibly edited
  compiler output, like the MMI neighbours. `memset` (marked hand-written) looks compiled (size_t `while (n--)`'s
  lui/ori -1, short-loop padding nops; only pcpyh/pcpyld need asm): worth a try. `long long` is 128-bit on this
  compiler ("unsupported wide integer operation").

  Logs: `build/dpq.log`; outputs per function in `build/decomp-permuter/<function>/`.
