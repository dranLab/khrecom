/* libsn (SN Systems ProDG): debugger stubs and the EE boot parameter query */
#include "include_asm.h"

typedef unsigned int u_int;
typedef unsigned char u_char;

extern int __snIsDebuggerRunning;
extern int SetVCommonHandler(int, void *);
void snTeq(void);

static u_int bootparams[4];

int snDebugInit(void)
{
    return 1;
}

int snIsDebuggerRunning(void)
{
    return __snIsDebuggerRunning;
}

void snNotifyDebugger(void)
{
    __asm__ volatile("nop");
}

void snAllBreaksOut(void)
{
    __asm__ volatile("nop");
}

void snAllBreaksIn(void)
{
    __asm__ volatile("nop");
}

/* put back the kernel's trap vector that snGetEEBootParams replaced */
static void snRestoreTeq(void)
{
    u_int addr = 0x80000000;
    u_int handler = ((u_int)snTeq + 8) | 0x80000000;

    for (; addr != 0x80080000; addr += 4) {
        u_int *p = (u_int *)addr;
        if (*p == handler && p[-1] != 0 && p[-1] == p[-2] && p[-1] == p[-3] && p[-1] == p[-4]) {
            *p = p[-1];
            return;
        }
    }
}

static u_char *snQueryBootMode(int mode)
{
    u_char *p = (u_char *)(*(u_int *)0xbc0003f0 | 0xbc000000);

    if (*(u_int *)p != 0) {
        do {
            if (p[2] == mode)
                return p;
            p = p + p[3] * 4 + 4;
        } while (*(u_int *)p != 0);
    }
    return 0;
}

static void snGetBootParams(void)
{
    u_int *p;

    bootparams[1] = -1;
    bootparams[0] = -1;
    bootparams[3] = -1;
    bootparams[2] = -1;
    p = (u_int *)snQueryBootMode(2);
    if (p) {
        bootparams[0] = p[1];
        bootparams[1] = p[2];
    }
    p = (u_int *)snQueryBootMode(3);
    if (p) {
        bootparams[2] = p[1];
        bootparams[3] = p[2];
    }
}

/* Hand-written: trap handler */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/snDebugInit", snTeq);

u_int *snGetEEBootParams(void)
{
    if (SetVCommonHandler(13, (void *)(((u_int)snTeq + 8) | 0x80000000)) == 0)
        return 0;
    __asm__ volatile("teq $0, $0");
    if (bootparams[0] == -1 && bootparams[1] == -1)
        return 0;
    return bootparams;
}
// SOURCE DATA: .bss
// SOURCE DATA SLUS_217.99_retail: .bss
