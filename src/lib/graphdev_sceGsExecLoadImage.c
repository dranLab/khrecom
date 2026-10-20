/* libgraph (Sony EE SDK): send an image to GS local memory over DMA channel 2 (GIF). */

typedef unsigned int u_long128 __attribute__((mode(TI)));

typedef struct {
    unsigned long NLOOP : 15;
    unsigned long EOP : 1;
    unsigned long pad16 : 16;
    unsigned long id : 14;
    unsigned long PRE : 1;
    unsigned long PRIM : 11;
    unsigned long FLG : 2;
    unsigned long NREG : 4;
    unsigned long REGS;
} sceGifTag __attribute__((aligned(16)));

typedef struct {
    sceGifTag giftag0;
    unsigned long bitbltbuf;
    long bitbltbufaddr;
    unsigned long trxpos;
    long trxposaddr;
    unsigned long trxreg;
    long trxregaddr;
    unsigned long trxdir;
    long trxdiraddr;
    sceGifTag giftag1;
} sceGsLoadImage;

#define D2_CHCR ((volatile unsigned int *)0x1000A000)
#define D2_MADR ((volatile unsigned int *)0x1000A010)
#define D2_QWC ((volatile unsigned int *)0x1000A020)

/* Scratchpad addresses (0x7xxxxxxx) are given to the DMA controller with bit 31 (SPR) set. */
#define SET_MADR(p)                                                              \
    if (((unsigned int)(p) & 0x70000000) == 0x70000000) {                         \
        *D2_MADR = ((unsigned int)(p) & 0x0fffffff) | 0x80000000;                 \
    } else {                                                                      \
        *D2_MADR = (unsigned int)(p) & 0x0fffffff;                                \
    }

void scePrintf(const char *fmt, ...);

int sceGsExecLoadImage(sceGsLoadImage *lp, u_long128 *srcaddr)
{
    unsigned int count = 0;

    while (*D2_CHCR & 0x100) {
        if (count++ > 0x1000000) {
            scePrintf("sceGsExecLoadImage: DMA Ch.2 does not terminate\r\n");
            return -1;
        }
    }
    *D2_QWC = 6;
    SET_MADR(lp);
    *D2_CHCR = 0x101;
    while (*D2_CHCR & 0x100) {
        if (count++ > 0x1000000) {
            scePrintf("sceGsExecLoadImage: DMA Ch.2 does not terminate\r\n");
            return -1;
        }
    }
    *D2_QWC = lp->giftag1.NLOOP;
    SET_MADR(srcaddr);
    *D2_CHCR = 0x101;
    return 0;
}
// SOURCE DATA: .rodata
