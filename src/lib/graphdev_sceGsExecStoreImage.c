#include "include_asm.h"

/* libgraph (Sony EE SDK): read an image from GS local memory over DMA channel 1 (VIF1, GS -> memory). */

typedef unsigned int u_long128 __attribute__((mode(TI)));

typedef struct {
    unsigned long RRW : 12;
    unsigned long pad12 : 20;
    unsigned long RRH : 12;
    unsigned long pad44 : 20;
} sceGsTrxreg;

typedef struct {
    unsigned long SBP : 14;
    unsigned long pad14 : 2;
    unsigned long SBW : 6;
    unsigned long pad22 : 2;
    unsigned long SPSM : 6;
    unsigned long pad30 : 2;
    unsigned long DBP : 14;
    unsigned long pad46 : 2;
    unsigned long DBW : 6;
    unsigned long pad54 : 2;
    unsigned long DPSM : 6;
    unsigned long pad62 : 2;
} sceGsBitbltbuf;

typedef struct {
    unsigned int vifcode[4];
    u_long128 giftag;
    sceGsBitbltbuf bitbltbuf;
    long bitbltbufaddr;
    unsigned long trxpos;
    long trxposaddr;
    sceGsTrxreg trxreg;
    long trxregaddr;
    unsigned long finish;
    long finishaddr;
    unsigned long trxdir;
    long trxdiraddr;
} sceGsStoreImage;

#define D1_CHCR ((volatile unsigned int *)0x10009000)
#define D1_MADR ((volatile unsigned int *)0x10009010)
#define D1_QWC ((volatile unsigned int *)0x10009020)
#define GIF_CTRL ((volatile unsigned int *)0x10003000)
#define VIF1_STAT ((volatile unsigned int *)0x10003C00)
#define VIF1_FBRST ((volatile unsigned int *)0x10003C10)
#define VIF1_FIFO ((volatile u_long128 *)0x10005000)
#define GS_CSR ((volatile unsigned long *)0x12001000)
#define GS_BUSDIR ((volatile unsigned long *)0x12001040)

/* Scratchpad addresses (0x7xxxxxxx) are given to the DMA controller with bit 31 (SPR) set. */
#define SET_MADR(p)                                                              \
    if (((unsigned int)(p) & 0x70000000) == 0x70000000) {                         \
        *D1_MADR = ((unsigned int)(p) & 0x0fffffff) | 0x80000000;                 \
    } else {                                                                      \
        *D1_MADR = (unsigned int)(p) & 0x0fffffff;                                \
    }

void scePrintf(const char *fmt, ...);
int GsGetIMR(void);
unsigned long GsPutIMR(unsigned long imr);

/* Parked: close, but register allocation differs (the PSMCT24 tail, trxreg.RRW kept extracted instead of raw). */
#if 0
int sceGsExecStoreImage(sceGsStoreImage *sp, u_long128 *dstaddr)
{
    static unsigned int init_mp3[4] __attribute__((aligned(16))) = { 0x06000000, 0, 0, 0 };
    u_long128 tmp;
    unsigned long imr;
    unsigned int count = 0;
    int qwc = 0, rest = 0, bytes = 0, dummy = 0;
    int w, h, rh, n, i, psm;
    sceGsTrxreg trxreg = sp->trxreg;

    w = trxreg.RRW;
    h = trxreg.RRH;
    psm = sp->bitbltbuf.SPSM;
    rh = 0;
    switch (psm) {
    case 0x00: /* PSMCT32 */
    case 0x30: /* PSMZ32 */
        n = w * h * 4;
        bytes = n & 0xf;
        qwc = (n >> 4) & 0xfffffff8;
        rest = (n >> 4) & 7;
        if (bytes == 0) {
            rh = h;
            dummy = 0;
        } else {
            rh = (h + 3) & 0xfffffffc;
            dummy = (w * rh >> 2) - qwc - rest - 1;
        }
        break;
    case 0x01: /* PSMCT24 */
    case 0x31: /* PSMZ24 */
        n = w * h * 3;
        bytes = n & 0xf;
        qwc = (n >> 4) & 0xfffffff8;
        rest = (n >> 4) & 7;
        if (bytes == 0) {
            rh = h;
            dummy = 0;
        } else {
            rh = (h + 15) & 0xfffffff0;
            dummy = (w * rh * 3 >> 4) - qwc - rest - 1;
        }
        break;
    case 0x02: /* PSMCT16 */
    case 0x0a: /* PSMCT16S */
    case 0x32: /* PSMZ16 */
    case 0x3a: /* PSMZ16S */
        n = w * h * 2;
        bytes = n & 0xf;
        qwc = (n >> 4) & 0xfffffff8;
        rest = (n >> 4) & 7;
        if (bytes == 0) {
            rh = h;
            dummy = 0;
        } else {
            rh = (h + 7) & 0xfffffff8;
            dummy = (w * rh >> 3) - qwc - rest - 1;
        }
        break;
    case 0x13: /* PSMT8 */
    case 0x1b: /* PSMT8H */
        n = w * h;
        bytes = n & 0xf;
        qwc = (n >> 4) & 0xfffffff8;
        rest = (n >> 4) & 7;
        if (bytes == 0) {
            rh = h;
            dummy = 0;
        } else {
            rh = (h + 7) & 0xfffffff8;
            dummy = (w * rh >> 4) - qwc - rest - 1;
        }
        break;
    case 0x14: /* PSMT4 */
    case 0x24: /* PSMT4HL */
    case 0x2c: /* PSMT4HH */
        n = w * h;
        bytes = (n >> 1) & 0xf;
        qwc = (n >> 5) & 0xfffffff8;
        rest = (n >> 5) & 7;
        if (bytes == 0) {
            rh = h;
            dummy = 0;
        } else {
            rh = (h + 7) & 0xfffffff8;
            dummy = (w * rh >> 5) - qwc - rest - 1;
        }
        break;
    }
    if (bytes) {
        /* Read whole rows: the GS only sends complete quadwords. */
        *(volatile unsigned long *)((unsigned int)&sp->trxreg | 0x20000000) =
            (unsigned long)(int)trxreg.RRW | (unsigned long)rh << 32;
    }

    while (*D1_CHCR & 0x100) {
        if (count++ > 0x1000000) {
            scePrintf("sceGsExecStoreImage: DMA Ch.1 does not terminate\r\n");
            return -1;
        }
    }
    imr = GsPutIMR(GsGetIMR() | 0x200);
    *GS_CSR = 2;
    *D1_QWC = 7;
    SET_MADR(sp);
    *D1_CHCR = 0x101;
    while (*D1_CHCR & 0x100) {
        if (count++ > 0x1000000) {
            scePrintf("sceGsExecStoreImage: DMA Ch.1 does not terminate\r\n");
            return -1;
        }
    }
    while (!(*GS_CSR & 2)) {
        if (count++ > 0x1000000) {
            scePrintf("sceGsExecStoreImage: GS does not terminate\r\n");
            *VIF1_FIFO = *(u_long128 *)init_mp3;
            return -1;
        }
    }
    *VIF1_STAT = 0x800000;
    *GS_BUSDIR = 1;
    if (qwc) {
        *D1_QWC = qwc;
        SET_MADR(dstaddr);
        *D1_CHCR = 0x100;
        while (*D1_CHCR & 0x100) {
            if (count++ > 0x1000000) {
                scePrintf("sceGsExecStoreImage: DMA Ch.1 (GS->MEM) does not terminate\r\n");
                *GS_CSR = 0x100;
                *GS_BUSDIR = 0;
                *GIF_CTRL = 1;
                *VIF1_FBRST = 1;
                return -1;
            }
        }
    }
    for (i = 0; i < rest; i++) {
        while (!(*VIF1_STAT & 0x1f000000)) {
            if (count++ > 0x1000000) {
                scePrintf("sceGsExecStoreImage: Enough data does not reach VIF1\n");
                *GS_CSR = 0x100;
                *GS_BUSDIR = 0;
                *GIF_CTRL = 1;
                *VIF1_FBRST = 1;
                return -1;
            }
        }
        dstaddr[qwc + i] = *VIF1_FIFO;
    }
    if (bytes) {
        while (!(*VIF1_STAT & 0x1f000000)) {
            if (count++ > 0x1000000) {
                scePrintf("sceGsExecStoreImage: Enough data does not reach VIF1\n");
                *GS_CSR = 0x100;
                *GS_BUSDIR = 0;
                *GIF_CTRL = 1;
                *VIF1_FBRST = 1;
                return -1;
            }
        }
        tmp = *VIF1_FIFO;
        for (i = 0; i < bytes; i++) {
            ((unsigned char *)&dstaddr[qwc + rest])[i] = *((unsigned char *)&tmp + i);
        }
        for (i = 0; i < dummy; i++) {
            while (!(*VIF1_STAT & 0x1f000000)) {
                if (count++ > 0x1000000) {
                    scePrintf("sceGsExecStoreImage: Enough data does not reach VIF1\n");
                    *GS_CSR = 0x100;
                    *GS_BUSDIR = 0;
                    *GIF_CTRL = 1;
                    *VIF1_FBRST = 1;
                    return -1;
                }
            }
            tmp = *VIF1_FIFO;
        }
    }
    *VIF1_STAT = 0;
    *GS_BUSDIR = 0; GsPutIMR(imr);
    *GS_CSR = 2;
    *VIF1_FIFO = *(u_long128 *)init_mp3;
    return 0;
}
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/graphdev_sceGsExecStoreImage", sceGsExecStoreImage);
/* sceGsExecStoreImage's static, strings and jump table (the function is still assembly). */
asm(".data\n.align 4\ninit_mp3.12:\n.word 0x06000000\n.word 0\n.word 0\n.word 0\n"
    ".rdata\n.align 3\nD_004FB3E0:\n.asciz \"sceGsExecStoreImage: DMA Ch.1 does not terminate\\r\\n\"\n"
    ".align 3\nD_004FB418:\n.asciz \"sceGsExecStoreImage: GS does not terminate\\r\\n\"\n"
    ".align 3\nD_004FB448:\n.asciz \"sceGsExecStoreImage: DMA Ch.1 (GS->MEM) does not terminate\\r\\n\"\n"
    ".align 3\nD_004FB488:\n.asciz \"sceGsExecStoreImage: Enough data does not reach VIF1\\n\"\n"
    ".align 3\njtbl_004FB4C0:\n"
    ".word .L00362A0C\n.word .L00362A44\n.word .L00362A94\n.word .L00362B4C\n.word .L00362B4C\n"
    ".word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n"
    ".word .L00362A94\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n"
    ".word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362ACC\n"
    ".word .L00362B00\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n"
    ".word .L00362B4C\n.word .L00362B4C\n.word .L00362ACC\n.word .L00362B4C\n.word .L00362B4C\n"
    ".word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n"
    ".word .L00362B4C\n.word .L00362B00\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n"
    ".word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362B00\n"
    ".word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362A0C\n.word .L00362A44\n"
    ".word .L00362A94\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n"
    ".word .L00362B4C\n.word .L00362B4C\n.word .L00362B4C\n.word .L00362A94\n"
    ".text\n");
// SOURCE DATA: .data .rodata
