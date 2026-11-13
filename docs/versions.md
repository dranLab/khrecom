# Two game versions from one source: the debug/retail switch

The repository builds two executables from the same `src/`:

| Version | ELF | Role |
|---|---|---|
| `SLUS_217.99` | US translation beta | the **reference**: its debug info gives `ref/`, the split and the line markers |
| `SLUS_217.99_retail` | US retail release (`iso/SLUS_217.99_retail`) | second target: the release configuration of the same code |

Retail is the beta's source compiled with one preprocessor switch, `-DKH_RETAIL`, which turns the debug code off.
Everything that differs between the two lives in the macros of `include/khversion.h`, which expand to exactly the
beta's original text in the beta build. The history of how this was worked out is in `docs/retail-plan.md` (the
plan, phases 0-4) and `docs/retail.md` (measurements, decisions, levers per unit).

## 1. Two builds

- `config/versions.yaml` lists the versions, reference first. `configure.py` writes `build/<VERSION>/build.ninja`
  for each (the root `build.ninja` includes both); `ninja` and `tools/check.sh` build and checksum both images.
  A version whose ELF is missing from `iso/` is skipped, loudly.
- `config/SLUS_217.99_retail/build.yaml`: the beta's compiler and flags **without `-g`** (the retail ELF is a
  release build) and **with `-DKH_RETAIL`**. Without `-g`, gcc 2.95 puts inline (weak) functions and vtables into
  `.gnu.linkonce.*` sections instead of the object's own; the asm post-processing (`tools/ee_asm_compat.py
  --release`) and the retail split handle that.
- The libraries (`lib/`) are identical in both ELFs and keep their `build.yaml` overrides.

## 2. The macros (`include/khversion.h`, included by `khtypes.h`)

| Macro | Beta | Retail | Used for |
|---|---|---|---|
| `KH_DEBUG(x)` | `x` | nothing | asserts, debug-only functions, members, tables and statements |
| `KH_RELEASE(x)` | nothing | `x` | retail-only code (rare) |
| `KH_VER(beta, retail)` | `beta` | `retail` | real retail changes: texts, arguments, callees |
| `KH_AT("(123)")` | `(__FILE__ "(123)")` | nothing | `new KH_AT("(123)") T`: the debug allocation becomes a plain `new T` |
| `KH_ATARG("(123)")` | `, __FILE__ "(123)"` | nothing | the message argument of the debug `operator new` |
| `KH_DW(DebugWork::g_dw.x)` | the read | `0` | debug-menu switches: off in retail (user decision 2026-11-11) |
| `KH_DW_ON(x)` / `KH_DWV(x, v)` | the read | `1` / `v` | switches that are on by default / have the default `v` |

In retail, `khversion.h` also redefines the debug calls directly:

- `AssertApp(...)` becomes `((void)0)`.
- `com_malloc_d` / `com_memalign_d` / `com_calloc_d` / `com_realloc_d` lose their message argument.
- `TRACE*(args)` becomes `((void)(args))`, so arguments with side effects stay.
- The `dbg*` button and user queries return `0`.

## 3. The rule that keeps the beta intact: line-neutral edits

The beta's assert and allocation strings contain `__LINE__`, so every edit keeps each statement on its original
line. Macros wrap code on the same line; multi-line blocks are wrapped line by line:

```cpp
KH_DEBUG(BTL_DECK g_deckSoraDebug[32] = {)
    KH_DEBUG({1, 7}, {1, 6}, {1, 5}, {1, 5}, {1, 4}, {1, 3}, {1, 4}, {1, 3},)
KH_DEBUG(};)
```

In the beta every macro expands to the original tokens, so the beta object stays byte-identical; the gate checks the
beta SHA-1 on every commit. Hard-coded `"c:/KHCOM/....cpp(123)"` allocation messages are written
`new KH_AT("(123)")`: a literal escapes the switch and leaves the debug string in retail's `.rodata`.

## 4. How each kind of difference is written, in order of preference

1. **Debug code retail doesn't have**: `KH_DEBUG`. A debug block whose condition folds to 0 in retail (`dbg*`,
   `KH_DW`) can still change the code around it (locals' stack slots, calls in conditions, scheduling): wrap the whole
   block. A hand-written `if (x) AssertApp(...)` leaves its test in retail: `KH_DEBUG` around the `if` as well.
2. **Real retail changes**: `KH_VER` (or `KH_RELEASE`), written from the retail assembly. Examples:
   `CRsrcDB::KH_VER(getRsrcGrpSizeOnMemory, getRsrcGrpSize)`, `reqPlayMovie(pName, KH_VER(1, i2))`,
   `KH_VER("Damage Zero", "Invincible")`. Look for differing constants, arguments and callees (`retail_units.py try
   --show <function>`) before treating a difference as a compiler puzzle.
3. **The same code compiled differently once the debug code is gone** (registers, scheduling, layout): first find
   one source that fits both builds. The beta's line markers (`tools/helpers/asmlines.py`) give the original
   statement order, and the ref's locals give the declarations and inline instances. This is usually the original
   source, and the beta keeps matching unchanged. Examples: CardRevolver setPlayCard's fall-through switch,
   PlayerManager setReqLoad's `goto` exit, VSME loadFromMem with the ref's locals.
4. **Only when nothing fits both builds**: a `KH_DEBUG(...)` / `KH_RELEASE(...)` pair on neighbouring lines (VSME's
   `look.Rate` after `LookAngle.set` in retail), or a filler the user accepted:
   - `INT x[1]` for a local passed by address (CChatMan::reqCommand);
   - one retail-only `KH_RELEASE(asm volatile("");)` for a measured insn-count or near-tie cause;
   - VSME execExt2Code's `FLOAT f[1]; asm volatile("" : : "m"(*f));`;
   - retail-only zero blocks `KH_RELEASE(__asm__(".rdata\n.align 3\n.word 0 ..."))` for data gaps no object
     explains (CollParam, CollMap, BattleCard).

   Each carries a `// MATCH:` comment saying why. New filler kinds need the user's approval.

## 5. Which units retail builds from source

- The retail ELF is stripped. `tools/version_map.py` pairs it with the beta, using SN's DLL symbol table in the
  retail ELF, the call graph and masked code comparison. Its output goes to `build/SLUS_217.99_retail/version_map/`;
  `configure.py` generates it when it is missing, and it must be rerun by hand after a change to
  `config/SLUS_217.99_retail/function_pairs.manual.txt`.
- `tools/split_version.py` derives the retail split (`config/SLUS_217.99_retail/SLUS_217.99_retail.yaml`) from that
  map and the recorded data starts.
- `config/SLUS_217.99_retail/sources.txt` lists the units compiled from source in retail. Every other unit links
  from retail's own assembly, as the beta did before its units were decompiled. A unit still with `INCLUDE_ASM` in the
  beta stays assembly in retail.
- Data from source is marked per version, as the source's last lines:
  `// SOURCE DATA SLUS_217.99_retail: .data .rodata`. Data starts are in `unit_starts.txt` (written by the tool) and
  `unit_starts.manual.txt` (measured by hand, each with its evidence).

### The workflow: `tools/retail_units.py`

| Command | What it does |
|---|---|
| `try <unit> [--show <mangled>] [--data]` | compile the unit as retail does and compare every function with its retail pair (registers and relocated fields masked); `--data`: each data section's start, voted by the matching functions' relocations, and its bytes |
| `scan [units...]` | `try --data` per unit and whether it can switch (all functions, code sizes, data starts and bytes) |
| `switch <unit>...` | add the units to `sources.txt`, record their data starts, write their retail data marker, regenerate the split |
| `verify` | build the retail image; while it differs, switch back the units it blames |

Pitfalls:

- `switch --eligible` writes a fresh list from the last scan: after scanning only a few units, switch them by name.
- `try` masks `jal` targets: a wrong callee only shows in `tools/check.sh` (`imagediff.py`).
- `tools/funcdiff.py --version SLUS_217.99_retail` reports size differences for units that match in the image
  (seen 2026-11-13 on CollParam): compare retail functions with `retail_units.py try`.
- A data gap after a newly switched unit that no object fills makes the next unit's data land early: see 4.4.

## 6. Checks

- `tools/check.sh` builds and checksums both images (exit 0 = both match); `--shift` adds the shifted builds.
- CI checks the retail image only when the repository secret `RETAIL_ELF_URL` is set; without it the retail version
  is skipped.
- Every commit keeps the beta image byte-identical. A beta change that breaks only retail:
  `tools/retail_units.py verify` switches the affected unit back; say so in the commit.
