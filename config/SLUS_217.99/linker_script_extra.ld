/* Pass this file to the linker with the `-T linker_script_extra.ld` flag */
ENTRY(_start);

/* Linker-defined in the original (end of .bss); main.cpp's static initializer computes the heap start from it */
_end = 0x006A0EF8;
end = _end;
