/* Pass this file to the linker with the `-T linker_script_extra.ld` flag */
ENTRY(_start);

/* Linker-defined in the original (end of .bss); main.cpp's static initializer computes the heap start from it.
 * linker_script_matching.ld checks that it lands where the original's did. */
_end = cod_BSS_END;
end = _end;

/* Sony's linker script: the main thread's stack size (crt0, and an absolute in the SN DLL export table). */
_stack_size = 0x00040000;

/* Zero words that splat wrote as references to address 0 (`.word func_0`): plain zeros in every layout. */
func_0 = 0;
