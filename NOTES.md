## Notes

- **Status:** `ninja` rebuilds a byte-identical code image of SLUS_217.99 (see [Progress](#progress) for how
  much of it is compiled from C/C++). The ELF still has its symbol table (18k symbols) and `.mdebug` debug info;
  the game is C++ built with EE-GCC 2.9x (`gcc2_compiled.` markers, GCC 2 name mangling).
  Code and data are split into the original objects (game code under `System/`, `Game/`, `Form/`,
  Sony SDK / newlib / libgcc under `lib/`): 665 `.text`, 232 `.data`, 373 `.rodata` and 105 `.bss`
  units. Objects without debug info are `lib/nodebug/<first symbol>` (their data boundaries are
  approximate), uninitialized globals the linker placed last are `common`; `.sndata`, `.sdata` and
  `.sbss` are single files.
- `mips-linux-gnu-as` aligns every section to 16 bytes, the original toolchain didn't, and Sony's
  linker script starts each section at `ALIGN(128)`. `configure.py` resets each object's sections to
  the alignment its original address implies (see `object_fixups`).

- **Toolchain:** SN Systems ProDG, all Windows executables run under [wibo](https://github.com/decompals/wibo):
  compiler `ee-gcc2.95.3-136` with `-O2 -G0 -fno-exceptions -fno-rtti -g`, assembler **Ps2EeAs** (from the
  same package). Only this combination matches every function of `ColorFader`; 2.95.3-114, 2.95.2-273a
  (each with its own Ps2EeAs), the Linux 2.9 builds and Sony's 2.96 don't.
  - Ps2EeAs inserts the EE's hazard nops and pads divisions and short loops (hardware errata). Its
    divisions rule fires near labels, and `-g`'s line-number labels decide where: without `-g` the
    padding differs. `-g` changes no instruction otherwise.
  - The anonymous namespace names in the ELF (`...cppM1aaaa`, from a `c:\` path) have the low-entropy
    suffix only the Windows compilers produce; only `build.cpp` has exception tables; vtables have no
    RTTI slot.
- `tools/ee_asm_compat.py` sits between compiler and assembler: it gives anonymous namespaces their
  original names (`_GLOBAL_$N$c__KHCOM_...cppM1aaaa`) and source paths in strings their original
  `c:/KHCOM/MainApp/` prefix. Ps2EeAs can't read splat's macros, so `INCLUDE_ASM` files are inlined with
  the macros expanded and every instruction that needs no relocation written as a `.word`: otherwise
  Ps2EeAs would add its erratum padding a second time. (Without Ps2EeAs, it targets modern gas instead.)
  Ps2EeAs crashes under wibo instead of printing an error, so a crash in the `cc` step means an
  assembler error; `ee_asm_compat`'s output is in `build/.../<file>.o.s`.
- **`.rodata` layout of an object:** first the string literals of the inline functions of every included
  header, in include order and whether used or not (assert/allocation messages such as
  `"c:/KHCOM/MainApp/System/Graphic/Gprim.h(613)"`, `"m_V"`); then the constants of the file's own
  functions; vtables last. Nothing references the unused header strings, so `split_objects.py` assigns
  unreferenced data before a file's first referenced item to that file (they used to land with the
  previous file). To match a file's `.rodata`, its headers must reproduce those strings, `__LINE__`
  included.
- objdiff can report less than 100% for data that matches: the target objects contain splat's
  `.word func_0` relocations for zero words (e.g. empty vtable slots) and the padding before the next
  object, the compiled ones don't. The full build's checksum is what counts.