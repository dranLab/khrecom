/* libgraph (Sony EE SDK): GS packet that clears a rectangle (a sprite with the Z test set to always). */

typedef struct {
    unsigned long testa;
    long testaaddr;
    unsigned long prim;
    long primaddr;
    unsigned long rgbaq;
    long rgbaqaddr;
    unsigned long xyz2a;
    long xyz2aaddr;
    unsigned long xyz2b;
    long xyz2baddr;
    unsigned long testb;
    long testbaddr;
} sceGsClear;

#define SCE_GS_TEST_1 0x47
#define SCE_GS_PRIM 0x00
#define SCE_GS_RGBAQ 0x01
#define SCE_GS_XYZ2 0x05

int sceGsSetDefClear(sceGsClear *cp, short ztest, short x, short y, short w, short h, unsigned char r,
                     unsigned char g, unsigned char b, unsigned char a, unsigned int z)
{
    cp->testaaddr = SCE_GS_TEST_1;
    cp->testa = 0x30000; /* ZTE on, ZTST always */
    cp->prim = 6; /* sprite */
    cp->primaddr = SCE_GS_PRIM;
    cp->rgbaqaddr = SCE_GS_RGBAQ;
    cp->rgbaq = (unsigned long)r | (unsigned long)g << 8 | (unsigned long)b << 16 | (unsigned long)a << 24
                | (unsigned long)0x3f800000 << 32;
    cp->xyz2a = (unsigned long)(x << 4) | (unsigned long)(y << 4) << 16 | (unsigned long)z << 32;
    cp->xyz2aaddr = SCE_GS_XYZ2;
    cp->xyz2baddr = SCE_GS_XYZ2;
    cp->xyz2b = (unsigned long)((x + w) << 4) | (unsigned long)((y + h) << 4) << 16 | (unsigned long)z << 32;
    cp->testbaddr = SCE_GS_TEST_1;
    if (ztest) {
        cp->testb = (unsigned long)(ztest & 3) << 17 | 0x10000;
    } else {
        cp->testb = 0x30000;
    }
    asm volatile("sync.l");
    return 6;
}
