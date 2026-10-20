/* DIntr / EIntr: disable / enable interrupts (Status.EIE), returning whether they were enabled. */
#include "include_asm.h"

/* The original reloads 0x10000 inside the di loop and pads it with two nops: no installed compiler/assembler
 * reproduces that from the obvious C (mfc0, &= 0x10000, do { di; sync.p; mfc0 } while). */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/diei", DIntr);

int EIntr(void)
{
    int eie;

    asm volatile ("mfc0\t%0, $12" : "=r" (eie));
    eie &= 0x10000;
    asm volatile ("ei");
    return eie != 0;
}
