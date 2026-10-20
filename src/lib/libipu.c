/* libipu (Sony EE SDK): stop/resume the IPU's DMA channels (3: from IPU, 4: to IPU) around other IPU use. */
#include "sdk/eekernel.h"
#include "sdk/libipu.h"

#define D_ENABLER ((volatile unsigned int *)0x1000F520)
#define D_ENABLEW ((volatile unsigned int *)0x1000F590)
#define D3_CHCR ((volatile unsigned int *)0x1000B000)
#define D3_MADR ((volatile unsigned int *)0x1000B010)
#define D3_QWC ((volatile unsigned int *)0x1000B020)
#define D4_CHCR ((volatile unsigned int *)0x1000B400)
#define D4_MADR ((volatile unsigned int *)0x1000B410)
#define D4_QWC ((volatile unsigned int *)0x1000B420)
#define D4_TADR ((volatile unsigned int *)0x1000B430)
#define IPU_CMD ((volatile unsigned int *)0x10002000)
#define IPU_CTRL ((volatile int *)0x10002010)
#define IPU_BP ((volatile unsigned int *)0x10002020)

#define IPU_WAIT() \
    while (*IPU_CTRL < 0) { \
    }

static void setD3_CHCR(unsigned int chcr)
{
    int oldei = DIntr();

    *D_ENABLEW = *D_ENABLER | 0x10000;
    *D3_CHCR = chcr;
    *D_ENABLEW = *D_ENABLER & ~0x10000;
    if (oldei) {
        EIntr();
    }
}

static void setD4_CHCR(unsigned int chcr)
{
    int oldei = DIntr();

    *D_ENABLEW = *D_ENABLER | 0x10000;
    *D4_CHCR = chcr;
    *D_ENABLEW = *D_ENABLER & ~0x10000;
    if (oldei) {
        EIntr();
    }
}

void sceIpuStopDMA(sceIpuDmaEnv *env)
{
    setD4_CHCR(1);
    env->d4madr = *D4_MADR;
    env->d4tadr = *D4_TADR;
    env->d4qwc = *D4_QWC;
    env->d4chcr = *D4_CHCR;
    while (*IPU_CTRL & 0xf0) {
    }
    setD3_CHCR(0);
    env->d3madr = *D3_MADR;
    env->d3qwc = *D3_QWC;
    env->d3chcr = *D3_CHCR;
    env->ipubp = *IPU_BP;
    env->ipuctrl = *IPU_CTRL;
}

void sceIpuRestartDMA(sceIpuDmaEnv *env)
{
    unsigned int bp = env->ipubp & 0x7f;
    unsigned int fifo = ((env->ipubp >> 16) & 3) + ((env->ipubp >> 8) & 0xf);
    unsigned int madr = env->d4madr - fifo * 16;
    unsigned int qwc = env->d4qwc + fifo;

    if (env->d3madr && env->d3qwc) {
        *D3_MADR = env->d3madr;
        *D3_QWC = env->d3qwc;
        setD3_CHCR(env->d3chcr | 0x100);
    }
    IPU_WAIT();
    *IPU_CMD = bp; /* BCLR */
    IPU_WAIT();
    if (madr && qwc) {
        *D4_MADR = madr;
        *D4_TADR = env->d4tadr;
        *D4_QWC = qwc;
        setD4_CHCR(env->d4chcr | 0x100);
    }
}

int sceIpuSync(int mode, unsigned short timeout)
{
    int ret = 0;

    switch (mode) {
    case 0:
        IPU_WAIT();
        ret = 0;
        break;
    case 1:
        ret = (unsigned int)*IPU_CTRL >> 31;
        break;
    }
    return ret;
}
