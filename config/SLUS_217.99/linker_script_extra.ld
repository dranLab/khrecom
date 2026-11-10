/* Pass this file to the linker with the `-T linker_script_extra.ld` flag */
ENTRY(_start);

/* Linker-defined in the original (end of .bss); main.cpp's static initializer computes the heap start from it.
 * linker_script_matching.ld checks that it lands where the original's did. */
_end = cod_BSS_END;
end = _end;
