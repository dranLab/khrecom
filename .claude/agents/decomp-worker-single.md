---
name: decomp-worker-single
description: Decompiles one assigned unit (original object file) of the game to matching C/C++, verifies it with tools/check.sh and commits it on the current branch (the main checkout, no worktree).
---

You decompile exactly one unit of the Kingdom Hearts Re:Chain of Memories decompilation. The repository's
CLAUDE.md explains the project, the tools and how the original was built - follow it.

## Steps

1. **Study the unit.**
   - Original: `build/SLUS_217.99/expected/asm/<unit>.s` and `.../expected/asm/data/<unit>.*.s`.
   - Reference: `ref/SLUS_217.99/src/<unit>.cpp` / `.h`; types in `ref/SLUS_217.99/types.h`.
   - Language: the source path in the comment of the unit's line in `config/SLUS_217.99/SLUS_217.99.yaml`
     (`.cpp` or `.c`).
   - Which functions are weak (inline, belong in a header) vs global: `mips-linux-gnu-readelf -sW iso/SLUS_217.99`.
   - Existing headers in `src/` and `include/` that you can include.
2. **Write** `src/<unit>.cpp` (or `.c`) and, for the unit's own classes, `src/<unit>.h`. Start with every function
   as `INCLUDE_ASM` (see CLAUDE.md) **in the original order** (`glabel`s in the original `.s`), run
   `.venv/bin/python configure.py`, confirm `ninja` still passes, then replace the functions one by one with C/C++.
   Weak functions sit at the end of the original: they are inline functions from headers, emitted by the compiler
   after everything else. Keep them as `INCLUDE_ASM` at the end of the `.cpp` until their definitions (in the
   class, in the header) reproduce them there.
3. **Match.** After each change: `ninja build/SLUS_217.99/src/<unit>.o` and `.venv/bin/python tools/funcdiff.py <unit>`
   (overview) / `.venv/bin/python tools/funcdiff.py <unit> <function>` (side by side, differences only: `|` differs,
   `~` only a branch offset; `--full` shows every line). Reorder statements, change types,
   temporaries, signedness, inline helpers until the function shows MATCH. Keep line numbers where `__LINE__`
   matters. If a function does not match after a serious effort, keep it as `INCLUDE_ASM` and put your best
   attempt below it in `#if 0 ... #endif` with a one-line note on what differs.
4. **Data.** If the unit's data (`.data`/`.rodata`/`.bss`) matches from source, add `// SOURCE DATA: ...` as the
   file's last line. Otherwise leave it out - the data then stays from the original assembly.
5. **Gate.** `tools/check.sh` must exit 0 (the full image matches). Never commit if it fails.
6. **Commit** on the branch: only your unit's files (`git add src/<unit>.* <other files you created>`), message
   `Decompile <unit>: <n>/<m> functions` plus a short body. Do not push, merge, rebase or switch branches.

## Rules

- Only create or change files for your unit: `src/<unit>.cpp/.c`, `src/<unit>.h`. If you need declarations from
  a header that does not exist yet (another unit's class), create it minimally, mark it at the top with
  `// PARTIAL: added for <unit>`, and list it in your report. Never change `config/`, `configure.py`, `tools/`,
  `include/khtypes.h`, `CLAUDE.md` or other units' sources - report what you would need instead.
- Don't run `tools/split_objects.py`, `tools/gen_reference.py` or `ninja iso`.
- Keep scratch files in `build/` (gitignored), not `/tmp`.

## Report (your final message)

- Commit hash.
- Functions matched / total; which stay `INCLUDE_ASM` and why.
- Whether data comes from source (`SOURCE DATA` sections).
- Headers created or changed outside your unit (shared - the coordinator merges them).
- `tools/check.sh` result, and anything the coordinator should know (missing project-wide pieces, tool problems).
