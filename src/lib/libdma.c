/* libdma (Sony EE SDK): DMA channel control. */
#include "sdk/eekernel.h"

typedef struct {
    unsigned int DIR : 1;
    unsigned int p0 : 1;
    unsigned int MOD : 2;
    unsigned int ASP : 2;
    unsigned int TTE : 1;
    unsigned int TIE : 1;
    unsigned int STR : 1;
    unsigned int p1 : 7;
    unsigned int TAG : 16;
} tD_CHCR;

typedef struct {
    unsigned int DMAE : 1;
    unsigned int RELE : 1;
    unsigned int MFD : 2;
    unsigned int STS : 2;
    unsigned int STD : 2;
    unsigned int RCYC : 2;
    unsigned int p0 : 22;
} tD_CTRL;

typedef struct {
    tD_CHCR chcr;
    unsigned int p0[3];
    void *madr;
    unsigned int p1[3];
    unsigned int qwc;
    unsigned int p2[3];
    void *tadr;
    unsigned int p3[3];
    void *as0;
    unsigned int p4[3];
    void *as1;
    unsigned int p5[3];
    unsigned int p6[4];
    unsigned int p7[4];
    void *sadr;
    unsigned int p8[3];
} sceDmaChan;

typedef struct {
    unsigned char sts;
    unsigned char std;
    unsigned char mfd;
    unsigned char rcycle;
    unsigned short express;
    unsigned short notify;
    unsigned short sqwc;
    unsigned short tqwc;
    void *rbadr;
    unsigned int rbmsk;
} sceDmaEnv;

#define D_CTRL ((volatile tD_CTRL *)0x1000E000)
#define D_STAT ((volatile unsigned int *)0x1000E010)
#define D_PCR ((volatile unsigned int *)0x1000E020)
#define D_SQWC ((volatile unsigned int *)0x1000E030)
#define D_RBSR ((volatile unsigned int *)0x1000E040)
#define D_RBOR ((volatile unsigned int *)0x1000E050)
#define D_STADR ((volatile unsigned int *)0x1000E060)
#define D_ENABLER ((volatile unsigned int *)0x1000F520)
#define D_ENABLEW ((volatile unsigned int *)0x1000F590)

void scePrintf(const char *fmt, ...);
unsigned int sceDmaPause(sceDmaChan *d);
int sceDmaPutEnv(sceDmaEnv *env);

static char __ps2_libinfo__[16] = "PsIIlibdma  3100";

sceDmaChan *dch[10] = {
    (sceDmaChan *)0x10008000, (sceDmaChan *)0x10009000, (sceDmaChan *)0x1000A000, (sceDmaChan *)0x1000B000,
    (sceDmaChan *)0x1000B400, (sceDmaChan *)0x1000C000, (sceDmaChan *)0x1000C400, (sceDmaChan *)0x1000C800,
    (sceDmaChan *)0x1000D000, (sceDmaChan *)0x1000D400,
};
static sceDmaEnv sceDmaCurrentEnv;

static void WaitDma(sceDmaChan *d)
{
    int i = 0x1000000;

    while (*(volatile unsigned int *)&d->chcr & 0x100) {
        if (--i < 0) {
            scePrintf("libdma: sync timeout\n");
            sceDmaPause(d);
        }
    }
}

static void WatchDma(sceDmaChan *d, void *addr, int count)
{
    if (count == 0) {
        count = 0x1000000;
    }
    while (*(volatile unsigned int *)&d->madr < (unsigned int)addr) {
        if (--count < 0) {
            scePrintf("libdma: sync timeout\n");
            sceDmaPause(d);
        }
    }
}

static const int isclr[10] = { 1, 1, 1, 1, 1, 0, 0, 0, 1, 1 };
static const unsigned char ststbl[10] = { 0, 0, 0, 3, 0, 1, 0, 0, 2 };
static const unsigned char stdtbl[10] = { 0, 1, 2, 0, 0, 0, 3 };
static const unsigned char mfdtbl[10] = { 0, 2, 3 };

static unsigned int CheckAddress(unsigned int addr)
{
    if ((addr >> 28) == 7) {
        addr &= 0x0fffffff;
        addr |= 0x80000000;
    }
    return addr;
}

static void memclr(void *p, int n)
{
    char *c = p;

    while (n--) {
        *c++ = 0;
    }
}

sceDmaChan *sceDmaGetChan(unsigned int id)
{
    return id < 10 ? dch[id] : 0;
}

int sceDmaReset(int mode)
{
    sceDmaEnv env;
    int i;
    int ret = *(volatile unsigned int *)D_CTRL & 1;

    for (i = 0; i < 10; i++) {
        if (isclr[i]) {
            volatile sceDmaChan *d = dch[i];
            d->sadr = 0;
            *(volatile unsigned int *)&d->chcr = 0;
            d->tadr = 0;
            d->madr = 0;
            d->as1 = 0;
            d->as0 = 0;
        }
    }
    *D_STAT = 0xff1f;
    *D_STAT = *D_STAT & 0xff1f0000; memclr(&env, sizeof(env));
    sceDmaPutEnv(&env);
    if (mode == 1) {
        *(volatile unsigned int *)D_CTRL |= 1;
    }
    return ret;
}

int sceDmaPutEnv(sceDmaEnv *env)
{
    unsigned int ctrl = *(volatile unsigned int *)D_CTRL;
    unsigned int pcr = *D_PCR;
    unsigned int sqwc = *D_SQWC;
    unsigned int rbor = *D_RBOR;
    unsigned int rbsr = *D_RBSR;

    if (env->sts >= 10) {
        return -1;
    }
    if (env->std >= 10) {
        return -2;
    }
    if (env->mfd >= 10) {
        return -3;
    }
    if (env->rcycle >= 7) {
        return -4;
    }
    ctrl = (ctrl & 0xffffffcf) | ststbl[env->sts] << 4;
    ctrl = (ctrl & 0xffffff3f) | stdtbl[env->std] << 6;
    ctrl = (ctrl & 0xfffffff3) | mfdtbl[env->mfd] << 2;
    if (env->rcycle) {
        ctrl |= 2;
        ctrl = (ctrl & 0xfffffcff) | (env->rcycle - 1) << 8;
    } else {
        ctrl &= 0xfffffffd;
    }
    pcr = env->express << 16 | env->notify;
    sqwc = env->tqwc << 16 | env->sqwc;
    rbor = (unsigned int)env->rbadr;
    rbsr = env->rbmsk;
    *(volatile unsigned int *)D_CTRL = ctrl;
    *D_PCR = pcr;
    *D_SQWC = sqwc;
    *D_RBOR = rbor;
    *D_RBSR = rbsr;
    sceDmaCurrentEnv = *env;
    return 0;
}

sceDmaEnv *sceDmaGetEnv(sceDmaEnv *env)
{
    *env = sceDmaCurrentEnv;
    return env;
}

void *sceDmaPutStallAddr(void *addr)
{
    void *old = (void *)*D_STADR;

    *D_STADR = (unsigned int)addr; return old; }

void sceDmaSend(sceDmaChan *d, void *tag)
{
    void *p = (void *)CheckAddress((unsigned int)tag);

    WaitDma(d);
    if (d->tadr != (void *)-1) {
        d->tadr = p;
    }
    d->qwc = 0;
    d->chcr.MOD = 1;
    d->chcr.DIR = 1;
    d->chcr.STR = 1;
}

void sceDmaSendN(sceDmaChan *d, void *addr, int size)
{
    void *p = (void *)CheckAddress((unsigned int)addr);

    WaitDma(d);
    if (d->madr != (void *)-1) {
        d->madr = p;
    }
    d->qwc = size;
    d->chcr.MOD = 0;
    d->chcr.DIR = 1;
    d->chcr.STR = 1;
}

void sceDmaSendI(sceDmaChan *d, void *addr, int size)
{
    void *p = (void *)CheckAddress((unsigned int)addr);

    WaitDma(d);
    if (d->madr != (void *)-1) {
        d->madr = p;
    }
    d->qwc = size;
    d->chcr.MOD = 2;
    d->chcr.DIR = 1;
    d->chcr.STR = 1;
}

void sceDmaRecv(sceDmaChan *d)
{
    WaitDma(d);
    d->qwc = 0;
    d->chcr.MOD = 1;
    d->chcr.DIR = 0;
    d->chcr.STR = 1;
}

void sceDmaRecvN(sceDmaChan *d, void *addr, int size)
{
    void *p = (void *)CheckAddress((unsigned int)addr);

    WaitDma(d);
    if (d->madr != (void *)-1) {
        d->madr = p;
    }
    d->qwc = size;
    d->chcr.MOD = 0;
    d->chcr.DIR = 0;
    d->chcr.STR = 1;
}

void sceDmaRecvI(sceDmaChan *d, void *addr, int size)
{
    void *p = (void *)CheckAddress((unsigned int)addr);

    WaitDma(d);
    if (d->madr != (void *)-1) {
        d->madr = p;
    }
    d->qwc = size;
    d->chcr.MOD = 2;
    d->chcr.DIR = 0;
    d->chcr.STR = 1;
}

int sceDmaSync(sceDmaChan *d, int mode, int timeout)
{
    if (mode == 1) {
        return d->chcr.STR;
    }
    WaitDma(d);
    return 0;
}

int sceDmaWatch(sceDmaChan *d, void *addr, int mode, int count)
{
    if (mode == 1) {
        return (unsigned int)d->madr < (unsigned int)addr;
    }
    WatchDma(d, addr, count);
    return 0;
}

unsigned int sceDmaPause(sceDmaChan *d)
{
    int oldei = DIntr();
    unsigned int enabler = *D_ENABLER;
    unsigned int ctrl;
    unsigned int chcr;

    if (!(enabler & 0x10000)) {
        *D_ENABLEW = enabler | 0x10000;
    }
    ctrl = *(volatile unsigned int *)D_CTRL;
    chcr = *(volatile unsigned int *)&d->chcr;
    *(volatile unsigned int *)&d->chcr = chcr & 0xfffffeff;
    *D_ENABLEW = enabler;
    if (oldei) {
        EIntr();
    }
    return chcr;
}

int sceDmaRestart(sceDmaChan *d, unsigned int chcr)
{
    int ret = d->chcr.STR;

    *(unsigned int *)&d->chcr = chcr;
    return ret;
}
// SOURCE DATA: .data .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss .data .rodata
