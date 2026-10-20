/* libgraph (Sony EE SDK): DMA packet that transfers an image from GS local memory to main memory. */

typedef struct {
    unsigned long NLOOP : 15;
    unsigned long EOP : 1;
    unsigned long pad16 : 16;
    unsigned long id : 14;
    unsigned long PRE : 1;
    unsigned long PRIM : 11;
    unsigned long FLG : 2;
    unsigned long NREG : 4;
    unsigned long REGS0 : 4;
    unsigned long REGS1 : 4;
    unsigned long REGS2 : 4;
    unsigned long REGS3 : 4;
    unsigned long REGS4 : 4;
    unsigned long REGS5 : 4;
    unsigned long REGS6 : 4;
    unsigned long REGS7 : 4;
    unsigned long REGS8 : 4;
    unsigned long REGS9 : 4;
    unsigned long REGS10 : 4;
    unsigned long REGS11 : 4;
    unsigned long REGS12 : 4;
    unsigned long REGS13 : 4;
    unsigned long REGS14 : 4;
    unsigned long REGS15 : 4;
} sceGifTag __attribute__((aligned(16)));

typedef struct {
    unsigned int vifcode[4];
    sceGifTag giftag;
    unsigned long bitbltbuf;
    long bitbltbufaddr;
    unsigned long trxpos;
    long trxposaddr;
    unsigned long trxreg;
    long trxregaddr;
    unsigned long finish;
    long finishaddr;
    unsigned long trxdir;
    long trxdiraddr;
} sceGsStoreImage;

typedef unsigned int u_long128 __attribute__((mode(TI)));

#define SCE_GS_BITBLTBUF 0x50
#define SCE_GS_TRXPOS 0x51
#define SCE_GS_TRXREG 0x52
#define SCE_GS_TRXDIR 0x53
#define SCE_GS_FINISH 0x61

int sceGsSetDefStoreImage(sceGsStoreImage *sp, short sbp, short sbw, short spsm, short x, short y, short w, short h)
{
    sp->vifcode[0] = 0;
    sp->vifcode[1] = 0x06008000; /* MSKPATH3 */
    sp->vifcode[2] = 0x13000000; /* FLUSHA */
    sp->vifcode[3] = 0x50000006; /* DIRECT 6 */
    *(u_long128 *)&sp->giftag = 0;
    sp->giftag.NLOOP = 5;
    sp->giftag.EOP = 1;
    sp->giftag.NREG = 1;
    sp->giftag.REGS0 = 0xe; /* A+D */
    sp->bitbltbuf = (unsigned long)sbp | (unsigned long)sbw << 16 | (unsigned long)spsm << 24;
    sp->bitbltbufaddr = SCE_GS_BITBLTBUF;
    sp->trxpos = (unsigned long)x | (unsigned long)y << 16;
    sp->trxposaddr = SCE_GS_TRXPOS;
    sp->trxreg = (unsigned long)w | (unsigned long)h << 32;
    sp->trxregaddr = SCE_GS_TRXREG;
    sp->finish = 0;
    sp->finishaddr = SCE_GS_FINISH;
    sp->trxdir = 1;
    sp->trxdiraddr = SCE_GS_TRXDIR;
    asm volatile("sync.l");
    return 7;
}
