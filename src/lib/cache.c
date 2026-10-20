/* Data cache write-back (SyncDCache) and invalidation (InvalidDCache) by address range. */
#include "sdk/eekernel.h"

/* Write back and invalidate every data cache line (both ways) whose tag lies in [start, end]. */
void _sceSDC(unsigned int start, unsigned int end)
{
    int i;
    unsigned int tag;

    for (i = 0; i < 0x1000; i += 0x40) {
        asm volatile ("sync");
        asm volatile ("cache\t0x10, 0(%0)" : : "r" (i));
        asm volatile ("sync");
        asm volatile ("mfc0\t%0, $28" : "=r" (tag));
        tag = (tag & 0xFFFFF000) + i;
        if (tag >= start && tag <= end) {
            asm volatile ("sync");
            asm volatile ("cache\t0x14, 0(%0)" : : "r" (i));
            asm volatile ("sync");
        }
        asm volatile ("sync");
        asm volatile ("cache\t0x10, 1(%0)" : : "r" (i));
        asm volatile ("sync");
        asm volatile ("mfc0\t%0, $28" : "=r" (tag));
        tag = (tag & 0xFFFFF000) + i;
        if (tag >= start && tag <= end) {
            asm volatile ("sync");
            asm volatile ("cache\t0x14, 1(%0)" : : "r" (i));
            asm volatile ("sync");
        }
        asm volatile ("sync");
    }
}

void SyncDCache(void *start, void *end)
{
    int eie;

    asm volatile ("mfc0\t%0, $12" : "=r" (eie));
    eie &= 0x10000;
    if (eie) {
        DIntr();
    }
    _sceSDC((unsigned int)start & ~0x3F, (unsigned int)end & ~0x3F);
    if (eie) {
        EIntr();
    }
}

void iSyncDCache(void *start, void *end)
{
    _sceSDC((unsigned int)start & ~0x3F, (unsigned int)end & ~0x3F);
}

/* Invalidate every data cache line (both ways) whose tag lies in [start, end]. */
void _sceIDC(unsigned int start, unsigned int end)
{
    int i;
    unsigned int tag;

    for (i = 0; i < 0x1000; i += 0x40) {
        asm volatile ("sync");
        asm volatile ("cache\t0x10, 0(%0)" : : "r" (i));
        asm volatile ("sync");
        asm volatile ("mfc0\t%0, $28" : "=r" (tag));
        tag = (tag & 0xFFFFF000) + i;
        if (tag >= start && tag <= end) {
            asm volatile ("sync");
            asm volatile ("cache\t0x16, 0(%0)" : : "r" (i));
            asm volatile ("sync");
        }
        asm volatile ("sync");
        asm volatile ("cache\t0x10, 1(%0)" : : "r" (i));
        asm volatile ("sync");
        asm volatile ("mfc0\t%0, $28" : "=r" (tag));
        tag = (tag & 0xFFFFF000) + i;
        if (tag >= start && tag <= end) {
            asm volatile ("sync");
            asm volatile ("cache\t0x16, 1(%0)" : : "r" (i));
            asm volatile ("sync");
        }
        asm volatile ("sync");
    }
}

void InvalidDCache(void *start, void *end)
{
    int eie;

    asm volatile ("mfc0\t%0, $12" : "=r" (eie));
    eie &= 0x10000;
    if (eie) {
        DIntr();
    }
    _sceIDC((unsigned int)start & ~0x3F, (unsigned int)end & ~0x3F);
    if (eie) {
        EIntr();
    }
}

void iInvalidDCache(void *start, void *end)
{
    _sceIDC((unsigned int)start & ~0x3F, (unsigned int)end & ~0x3F);
}
