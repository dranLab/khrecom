/* Pass this file to the linker with the `-T linker_script_extra.ld` flag */
ENTRY(_start);

/* Linker-defined in the original (end of .bss); main.cpp's static initializer computes the heap start from it.
 * linker_script_matching.ld checks that it lands where the original's did. */
_end = cod_BSS_END;
end = _end;

/* Sony's linker script, for src/crt0.s (and absolutes in the SN DLL export table): the main thread's stack (-1:
 * the kernel places it at the end of memory) and its size, the heap size (-1: up to the stack), and the start of
 * the data crt0 clears (.sbss, then .bss, up to _end). */
_stack = -1;
_stack_size = 0x00040000;
_heap_size = -1;
_fbss = cod_SBSS_START;

/* Zero words that splat wrote as references to address 0 (`.word func_0`): plain zeros in every layout. */
func_0 = 0;
