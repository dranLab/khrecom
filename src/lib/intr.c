/* Interrupt controller / DMA channel masks: the kernel calls wrapped with interrupts off and a sync. */

#include "sdk/eekernel.h"

/* the system calls (klib.s) */
int _DisableIntc(int ch);
int _EnableIntc(int ch);
int _DisableDmac(int ch);
int _EnableDmac(int ch);
int _iEnableIntc(int ch);
int _iDisableIntc(int ch);
int _iEnableDmac(int ch);
int _iDisableDmac(int ch);

/* Whether the CPU runs in an interrupt (exception) context: Status.IE clear. */
int QueryIntrContext(void)
{
    int status;

    asm volatile ("mfc0\t%0, $12" : "=r" (status));
    return (status ^ 1) & 1;
}

int DisableIntc(int ch)
{
    int eie, res;

    asm volatile ("mfc0\t%0, $12" : "=r" (eie));
    eie &= 0x10000;
    if (eie) {
        DIntr();
    }
    res = _DisableIntc(ch);
    asm volatile ("sync");
    if (eie) {
        EIntr();
    }
    return res;
}

int EnableIntc(int ch)
{
    int eie, res;

    asm volatile ("mfc0\t%0, $12" : "=r" (eie));
    eie &= 0x10000;
    if (eie) {
        DIntr();
    }
    res = _EnableIntc(ch);
    asm volatile ("sync");
    if (eie) {
        EIntr();
    }
    return res;
}

int DisableDmac(int ch)
{
    int eie, res;

    asm volatile ("mfc0\t%0, $12" : "=r" (eie));
    eie &= 0x10000;
    if (eie) {
        DIntr();
    }
    res = _DisableDmac(ch);
    asm volatile ("sync");
    if (eie) {
        EIntr();
    }
    return res;
}

int EnableDmac(int ch)
{
    int eie, res;

    asm volatile ("mfc0\t%0, $12" : "=r" (eie));
    eie &= 0x10000;
    if (eie) {
        DIntr();
    }
    res = _EnableDmac(ch);
    asm volatile ("sync");
    if (eie) {
        EIntr();
    }
    return res;
}

int iEnableIntc(int ch)
{
    int res;

    res = _iEnableIntc(ch);
    asm volatile ("sync");
    return res;
}

int iDisableIntc(int ch)
{
    int res;

    res = _iDisableIntc(ch);
    asm volatile ("sync");
    return res;
}

int iEnableDmac(int ch)
{
    int res;

    res = _iEnableDmac(ch);
    asm volatile ("sync");
    return res;
}

int iDisableDmac(int ch)
{
    int res;

    res = _iDisableDmac(ch);
    asm volatile ("sync");
    return res;
}
