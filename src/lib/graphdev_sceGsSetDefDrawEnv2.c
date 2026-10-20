/* libgraph (Sony EE SDK): default drawing environment (context 2) for a frame buffer of the given size. */

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
    unsigned long frame2;
    long frame2addr;
    unsigned long zbuf2;
    long zbuf2addr;
    unsigned long xyoffset2;
    long xyoffset2addr;
    unsigned long scissor2;
    long scissor2addr;
    sceGsPrmodecont prmodecont;
    long prmodecontaddr;
    sceGsColclamp colclamp;
    long colclampaddr;
    sceGsDthe dthe;
    long dtheaddr;
    unsigned long test2;
    long test2addr;
} sceGsDrawEnv2;

#define SCE_GS_XYOFFSET_2 0x19
#define SCE_GS_PRMODECONT 0x1a
#define SCE_GS_SCISSOR_2 0x41
#define SCE_GS_DTHE 0x45
#define SCE_GS_COLCLAMP 0x46
#define SCE_GS_TEST_2 0x48
#define SCE_GS_FRAME_2 0x4d
#define SCE_GS_ZBUF_2 0x4f

short sceGszbufaddr(short psm, short w, short h);

int sceGsSetDefDrawEnv2(sceGsDrawEnv2 *draw, short psm, short w, short h, short ztest, short zpsm)
{
    draw->frame2addr = SCE_GS_FRAME_2;
    draw->frame2 = (unsigned long)((w >> 6) & 0x3f) << 16 | (unsigned long)(psm & 0xf) << 24;
    draw->zbuf2addr = SCE_GS_ZBUF_2;
    if (ztest == 0) {
        draw->zbuf2 = (unsigned long)sceGszbufaddr(psm, w, h) | (unsigned long)(zpsm & 0xf) << 24 | (unsigned long)1 << 32;
    } else {
        draw->zbuf2 = (unsigned long)sceGszbufaddr(psm, w, h) | (unsigned long)(zpsm & 0xf) << 24;
    }
    draw->xyoffset2addr = SCE_GS_XYOFFSET_2;
    draw->xyoffset2 = (2048 - (unsigned long)(w >> 1)) << 4 | (2048 - (unsigned long)(h >> 1)) << 4 << 32;
    draw->scissor2addr = SCE_GS_SCISSOR_2;
    draw->scissor2 = (unsigned long)(w - 1) << 16 | (unsigned long)(h - 1) << 48;
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
    draw->test2addr = SCE_GS_TEST_2;
    if (ztest) {
        draw->test2 = (unsigned long)(ztest & 3) << 17 | 0x10000;
    } else {
        draw->test2 = 0x30000;
    }
    asm volatile("sync.l");
    return 8;
}
