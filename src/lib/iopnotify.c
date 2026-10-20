/* IOP reboot notification: handlers (with their own $gp) called when the IOP is rebooted. */
#include "sdk/eekernel.h"

void *memset(void *s, int c, unsigned int n);

typedef struct {
    void (*func)(void *data, void *arg);
    void *arg;
    int gp;
    int pad;
} RebootNotify;

static RebootNotify *g_pUserRebootBuf = 0;
static int g_iUserRebootSize = 0;
static RebootNotify g_SysRebootBuf[32];

static inline int GetGP(void)
{
    int gp;

    asm volatile ("move\t%0, $gp" : "=r" (gp));
    return gp;
}

/* Switch $gp, returning the previous one. */
static inline int SetGP(int gp)
{
    int old;

    asm volatile ("move\t%0, $gp\n\tmove\t$gp, %1" : "=&r" (old) : "r" (gp) : "memory");
    return old;
}

void sceSifInitRebootNotify(void)
{
    g_pUserRebootBuf = 0;
    g_iUserRebootSize = 0;
    memset(g_SysRebootBuf, 0, sizeof(g_SysRebootBuf));
}

void *sceSifSetRebootNotifyBuffer(RebootNotify *buf, int size)
{
    RebootNotify *old;
    int eie;

    eie = DIntr();
    old = g_pUserRebootBuf;
    g_pUserRebootBuf = buf;
    g_iUserRebootSize = size;
    memset(buf, 0, size * sizeof(RebootNotify));
    if (eie) {
        EIntr();
    }
    return old;
}

/* pos with the top bit set: a slot of the system table. */
int sceSifAddRebootNotifyHandler(unsigned int pos, void (*func)(void *data, void *arg), void *arg)
{
    RebootNotify *buf;
    unsigned int size;
    int eie;

    if ((int)pos < 0) {
        pos &= 0x7FFFFFFF;
        buf = g_SysRebootBuf;
        size = 32;
    } else {
        buf = g_pUserRebootBuf;
        size = g_iUserRebootSize;
    }
    if (pos >= size) {
        return 0x80000069;
    }
    eie = DIntr();
    buf[pos].func = func;
    buf[pos].arg = arg;
    buf[pos].gp = GetGP();
    if (eie) {
        EIntr();
    }
    return 0;
}

int sceSifRemoveRebootNotifyHandler(unsigned int pos)
{
    RebootNotify *buf;
    unsigned int size;

    if ((int)pos < 0) {
        pos &= 0x7FFFFFFF;
        buf = g_SysRebootBuf;
        size = 32;
    } else {
        buf = g_pUserRebootBuf;
        size = g_iUserRebootSize;
    }
    if (pos >= size) {
        return -0x69;
    }
    buf[pos].func = 0;
    return 0;
}

int sceSifExecNotifyHandler(int sys, void *data)
{
    RebootNotify *buf;
    int size, i, gp;

    if (sys) {
        buf = g_SysRebootBuf;
        size = 32;
    } else {
        buf = g_pUserRebootBuf;
        size = g_iUserRebootSize;
    }
    gp = GetGP();
    for (i = 0; i < size; i++) {
        if (buf[i].func) {
            SetGP(buf[i].gp);
            buf[i].func(data, buf[i].arg);
        }
    }
    return SetGP(gp);
}
// SOURCE DATA: .data .bss
