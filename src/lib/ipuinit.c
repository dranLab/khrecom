/* libipu (Sony EE SDK): reset the IPU and load the default quantizer matrices and VQ colour table. */
#include "sdk/eekernel.h"


#define D_ENABLER ((volatile unsigned int *)0x1000F520)
#define D_ENABLEW ((volatile unsigned int *)0x1000F590)
#define D4_CHCR ((volatile unsigned int *)0x1000B400)
#define IPU_CMD ((volatile unsigned int *)0x10002000)
#define IPU_CTRL ((volatile int *)0x10002010)
#define IPU_IN_FIFO ((volatile u_long128 *)0x10007010)

#define IPU_WAIT() \
    while (*IPU_CTRL < 0) { \
    }

static char __ps2_libinfo__[16] = "PsIIlibipu  3100";

/* Intra and non-intra quantizer matrices (IQ): the MPEG-1 defaults, then a flat 16. */
static unsigned int iqval[20] __attribute__((aligned(16))) = {
    0x13101008, 0x16161310, 0x16161616, 0x1B1A181A,
    0x1A1A1B1B, 0x1B1B1A1A, 0x1D1D1D1B, 0x1D222222,
    0x1B1B1D1D, 0x20201D1D, 0x26252222, 0x22232325,
    0x28262623, 0x30302828, 0x38382E2E, 0x5345453A,
    0x10101010, 0x10101010, 0x10101010, 0x10101010,
};

/* VQ colour lookup table (CLUT, RGB 5:5:5). */
static unsigned int vqval[8] __attribute__((aligned(16))) = {
    0x04210000, 0x03E00842, 0x14A51084, 0x1CE718C6,
    0x2529001F, 0x7C00294A, 0x35AD318C, 0x39CE7FFF,
};

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

void sceIpuInit(void)
{
    setD4_CHCR(1);
    *IPU_CTRL = 0x40000000; /* RST */
    IPU_WAIT();
    *IPU_CMD = 0; /* BCLR */
    IPU_WAIT();
    *IPU_IN_FIFO = ((volatile u_long128 *)iqval)[0];
    *IPU_IN_FIFO = ((volatile u_long128 *)iqval)[1];
    *IPU_IN_FIFO = ((volatile u_long128 *)iqval)[2];
    *IPU_IN_FIFO = ((volatile u_long128 *)iqval)[3];
    *IPU_IN_FIFO = ((volatile u_long128 *)iqval)[4];
    *IPU_IN_FIFO = ((volatile u_long128 *)iqval)[4];
    *IPU_IN_FIFO = ((volatile u_long128 *)iqval)[4];
    *IPU_IN_FIFO = ((volatile u_long128 *)iqval)[4];
    *IPU_CMD = 0x50000000; /* SETIQ intra */
    IPU_WAIT();
    *IPU_CMD = 0x58000000; /* SETIQ non-intra */
    IPU_WAIT();
    *IPU_IN_FIFO = ((volatile u_long128 *)vqval)[0];
    *IPU_IN_FIFO = ((volatile u_long128 *)vqval)[1];
    *IPU_CMD = 0x60000000; /* SETVQ */
    IPU_WAIT();
    *IPU_CMD = 0x90000000; /* SETTH */
    IPU_WAIT();
    *IPU_CTRL = 0x40000000;
    IPU_WAIT();
    *IPU_CMD = 0;
    IPU_WAIT();
}
// SOURCE DATA: .data
// SOURCE DATA SLUS_217.99_retail: .data
