# Plan: decompile the library (SDK / runtime / middleware) units

## Context
Work so far covers only the game's own sources (`c:/KHCOM/MainApp/...`). The 154 `lib/...` units are skipped:
- `tools/candidates.py:55` filters them out.
- `CLAUDE.md` says to skip them.

Together they are about 218 KB of `.text`, roughly 8% of the code. The goal is to decompile all of them: the Sony SDK libraries, the GCC runtime, newlib (libm and libc), SN's libsn, and the hand-written assembly files. This starts only **after the 142 open game units are done**. The one exception is the infrastructure (Phase 0), which can land earlier.

The current build can't do this:
- One compiler and one set of flags for everything (`config/SLUS_217.99/build.yaml`: ee-gcc 2.95.3-136, `-O2 -G0 -fno-exceptions -fno-rtti -g`).
- Assembly always goes through SN's Ps2EeAs.

The libraries were built by other toolchains. Sony's `.a` files used Cygnus/Sony ee-gcc 2.9x with GNU as, and the newlib/libsn objects have no debug info, so they were built without `-g`. There's no `ref/` reference for any of it.

## Inventory (from the yaml)
| Group | Units | ~bytes | Notes |
|---|---|---|---|
| Sony SDK, with debug info | ~55 | 110 K | filestub, mpc, pack, libmc, libat3dec+hncAt3dec, eecdvd, graphdev (10 split pieces), timer, kprintf2, ext, mpeg, csc, ipuinit, libipu, libdbc, sifrpc, sifcmd, eeloadfile, libpad2, libdma, thread, intr, iop*, tlb*, alarm, cache, deci2, libosd, libvib, ... |
| GCC runtime | ~19 | 40 K | `lib/src/gcc/libgcc2_*`, frame.c, fp-bit, dp-bit |
| newlib libm | ~30 | 25 K | `lib/src/newlib/libm/{math,common}/*` |
| `lib/nodebug/*` | 50 | 45 K | newlib libc (printf family, malloc, `_*_r` stubs, str*), libsn (snDebugInit, snDllCacheFlush, check_dll), libgraph leftovers, data-only pieces (`__CTOR_LIST__`, `_ctype_`, `impure_data`) |
| hand-written `.s` | 4+ | 6 K | klib.s, tlbtrap.s, at3core.s, `_init` |

## Phase 0: infrastructure (coordinator, not workers; `CLAUDE.md` bans tool changes in decomp tasks)
1. **Per-unit toolchain in `build.yaml`.** Add an `overrides:` list of `{match: <unit glob>, compiler:, cflags:, source_root:}` entries.
   - In `configure.py` `write_ninja()`, give each `cc` build its own `cc`/`cflags`/assemble variables. Today these are rule-level, set at `configure.py:303` and `:314-334`.
   - `compiler_commands()` (`configure.py:112`) already returns "no SN assembler" for non-SN packages, which falls back to GNU `as`. Reuse that.
   - Make `ee_asm_compat.py`'s SN-specific rewriting conditional on `--assembler snas`. It already receives that flag; check which passes depend on it.
2. **Hand-written assembly as source.** Allow `src/lib/<unit>.s`. The `as` path already exists for `.s` at `configure.py:370`. Check that `source_config()` switches a unit to its source when that source is `.s`.
3. **Tools treat `lib/` as an option, not an exclusion.**
   - `candidates.py`: add `--lib`, which lists only lib units. Keep game-only as the default until the game pool is empty.
   - `compare_compilers.py`: accept `--cflags` per run (already there) and use the unit's override.
   - `check.sh` overview: report library progress separately.
4. **Headers.** Add `include/sdk/` with the SDK declarations the libraries need (`eekernel.h`, `sif*.h`, `libgraph.h`, `libdma.h`, `libmc.h`, `libmpeg.h`, ...). Write them from the call sites already in `src/` and from the asm; no external SDK copies. newlib and libgcc get their own small internal headers (`fdlibm.h`, `reent.h` subset, `fp-bit` macros).
5. **Workers.** Add `.claude/agents/decomp-worker-lib.md`, a variant of decomp-worker:
   - no `ref/`, no `__LINE__`/assert rules;
   - step 1 is "identify the toolchain with compare_compilers";
   - matching notes for GNU as (no Ps2EeAs erratum nops) and for objects without `-g`.

   Extend `decomp-pool`/`decomp-batch` to take `--lib`.

## Phase 1: identify the toolchain per library family (coordinator, before handing out units)
For each family, pick 1–2 small, plain-C functions and run `tools/compare_compilers.py` across all 11 packages in `tools/compilers/`, trying `-O2`/`-O3`, `-G0`/`-G8`, with and without `-g`. Families: alarm/diei/defhandler (libkernl), libvib, libdma, graphdev, mpeg, libat3dec, sf_fabs (newlib), `__muldi3` (libgcc), atoi or strrchr (newlib libc).
- **Expected:** Sony libs on ee-gcc 2.96 or 2.9-991111, libgcc/newlib on the toolchain version that shipped them (2.95.x/2.96).
- Record the results as `overrides:` entries plus a short table in `CLAUDE.md`.
- Some functions may not match any package. Those are probably hand-written or built with a compiler we don't have, so they stay INCLUDE_ASM'd or move to `.s`.

## Phase 2: hand-out order (one unit per worker, smallest and most self-contained first)
1. **Hand-written `.s`** (klib, tlbtrap, at3core, `_init`): move them to `src/lib/*.s`, cleaned up (labels named, macros). No compiler involved.
2. **Small Sony kernel/system units**: alarm, alalc, diei, defhandler, intr, cache, exit, error, init, initsys, glue, deci2, iop*, tlbfunc, timeralarm, thread, tty, sifcmd, sifrpc, eeloadfile, filestub. These confirm the Sony toolchain cheaply.
3. **Device libraries**: libdma, graphdev pieces, libpad2, libvib, libosd, libmc, eecdvd, libipu, ipuinit, csc, mpeg, libdbc, timer, kprintf2, output, ext.
4. **Middleware**: libat3dec, hncAt3dec, mpc, pack. Identify the vendor from strings first.
5. **GCC runtime**: libgcc2 split pieces, fp-bit, dp-bit, frame. These are compiled from GCC's own sources (one unit per `L_xxx` macro), so write the C from the algorithm.
6. **newlib libm**, fdlibm-style functions.
7. **nodebug units**: newlib libc and libsn. Data-only pieces get `// SOURCE DATA:` sources. Functions with no C equivalent stay asm.

## Matching notes for workers
- No assert strings or `__LINE__`. `.rodata` is just constants and jump tables.
- With GNU as, branch-delay-slot scheduling comes from gcc (`.set reorder`), so ordering problems are compiler problems.
- In split units like `graphdev_*` and `libgcc2_*`, each piece is its own object, i.e. one function per file.
- Weak-function and vtable rules don't apply (all C).

## Verification
- Phase 0: with no lib sources, `tools/check.sh` still exits 0 (checksum unchanged), and `build.ninja` is identical except for the new per-build variables.
- Then convert one tiny unit (e.g. `lib/defhandler`) end to end with an override. Check `funcdiff.py lib/defhandler` reaches 100% and `check.sh` passes.
- Per unit: `tools/funcdiff.py`, `tools/imagediff.py`, then the `tools/check.sh` gate before each merge, as for game units.
