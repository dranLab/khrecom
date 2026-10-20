/* libgraph (Sony EE SDK): default drawing environment (context 1) for a frame buffer of the given size. */

typedef struct {
    unsigned long AC : 1;
    unsigned long pad1 : 63;
} sceGsPrmodecont;

typedef struct {
    unsigned long CLAMP : 1;
    unsigned long pad1 : 63;
} sceGsColclamp;

typedef struct {
    unsigned long DTHE : 1;
    unsigned long pad1 : 63;
} sceGsDthe;

typedef struct {
    unsigned long frame1;
    long frame1addr;
    unsigned long zbuf1;
    long zbuf1addr;
    unsigned long xyoffset1;
    long xyoffset1addr;
    unsigned long scissor1;
    long scissor1addr;
    sceGsPrmodecont prmodecont;
    long prmodecontaddr;
    sceGsColclamp colclamp;
    long colclampaddr;
    sceGsDthe dthe;
    long dtheaddr;
    unsigned long test1;
    long test1addr;
} sceGsDrawEnv1;

#define SCE_GS_XYOFFSET_1 0x18
#define SCE_GS_PRMODECONT 0x1a
#define SCE_GS_SCISSOR_1 0x40
#define SCE_GS_DTHE 0x45
#define SCE_GS_COLCLAMP 0x46
#define SCE_GS_TEST_1 0x47
#define SCE_GS_FRAME_1 0x4c
#define SCE_GS_ZBUF_1 0x4e

short sceGszbufaddr(short psm, short w, short h);

int sceGsSetDefDrawEnv(sceGsDrawEnv1 *draw, short psm, short w, short h, short ztest, short zpsm)
{
    draw->frame1addr = SCE_GS_FRAME_1;
    draw->frame1 = (unsigned long)(((w + 63) >> 6) & 0x3f) << 16 | (unsigned long)(psm & 0xf) << 24;
    draw->zbuf1addr = SCE_GS_ZBUF_1;
    if (ztest == 0) {
        draw->zbuf1 = (unsigned long)sceGszbufaddr(psm, w, h) | (unsigned long)(zpsm & 0xf) << 24 | (unsigned long)1 << 32;
    } else {
        draw->zbuf1 = (unsigned long)sceGszbufaddr(psm, w, h) | (unsigned long)(zpsm & 0xf) << 24;
    }
    draw->xyoffset1addr = SCE_GS_XYOFFSET_1;
    draw->xyoffset1 = (2048 - (unsigned long)(w >> 1)) << 4 | (2048 - (unsigned long)(h >> 1)) << 4 << 32;
    draw->scissor1addr = SCE_GS_SCISSOR_1;
    draw->scissor1 = (unsigned long)(w - 1) << 16 | (unsigned long)(h - 1) << 48;
    draw->prmodecontaddr = SCE_GS_PRMODECONT;
    draw->prmodecont.AC = 1;
    draw->colclampaddr = SCE_GS_COLCLAMP;
    draw->colclamp.CLAMP = 1;
    draw->dtheaddr = SCE_GS_DTHE;
    if (psm & 2) {
        draw->dthe.DTHE = 1;
    } else {
        draw->dthe.DTHE = 0;
    }
    draw->test1addr = SCE_GS_TEST_1;
    if (ztest) {
        draw->test1 = (unsigned long)(ztest & 3) << 17 | 0x10000;
    } else {
        draw->test1 = 0x30000;
    }
    asm volatile("sync.l");
    return 8;
}
