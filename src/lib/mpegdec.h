/* libmpeg's internal decoder state (the library has no type information: fields are named by offset) */
#ifndef MPEGDEC_H
#define MPEGDEC_H

typedef struct _sceMpegDecoder _sceMpegDecoder;

typedef struct {
    int width;
    int height;
    int frameCount;
    long pts;
    long dts;
    unsigned long flags;
    long pts2nd;
    long dts2nd;
    unsigned long flags2nd;
    _sceMpegDecoder *sys;
} sceMpeg;

typedef struct {
    int type;
    long pts;
    long dts;
} sceMpegCbData;

typedef int (*sceMpegCallback)(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData);

typedef struct {
    sceMpegCallback func;
    void *data;
    void *gp;
} _sceMpegCbEntry;

typedef struct {
    unsigned int buf; /* 0x00 */
    int width;
    int height;
    int mbw;
    int mbh;
    int _pad14;
    long x18;
    long x20;
    int x28; /* holds a decoded picture */
    int x2C;
    int x30;
    int x34;
    int x38;
    int x3C;
    int x40;
    int x44;
    int x48;
    int x4C;
    int x50;
    int x54;
    int x58;
    int x5C;
    int x60;
    int x64;
} _sceMpegPicture;

typedef struct {
    int _w[4];
} _sceMpegAlalc;

/* arguments of one hand-written reference copy (_rix_* / _ri0_*) */
typedef struct {
    void *dst;
    int shift;
    int count;
    int count2;
    int stride;
    void *src0;
    void *src1;
} _sceMpegRiArg;

/* one macroblock's motion compensation work (double-buffered) */
typedef struct {
    unsigned int sadr; /* 0x000 */
    unsigned int buf; /* 0x004 */
    unsigned int addr[8]; /* 0x008: DMA sources */
    void (*rix[4])(void *); /* 0x028 */
    void (*ri0[4])(void *); /* 0x038 */
    _sceMpegRiArg rixArg[4]; /* 0x048 */
    _sceMpegRiArg ri0Arg[4]; /* 0x0B8 */
    unsigned int dst; /* 0x128 */
    int count; /* 0x12C */
    int intra; /* 0x130 */
    int x134;
    int mc; /* 0x138 */
    int skip; /* 0x13C */
} _sceMpegSpr;

struct _sceMpegDecoder {
    int isEnd; /* 0x00 */
    int x4;
    int x8;
    _sceMpegCbEntry cb[7]; /* 0x0C */
    unsigned char *x60;
    int x64;
    char _pad68[0x8C - 0x68];
    int x8C;
    long x90;
    int x98;
    int x9C;
    long xA0;
    int xA8;
    int xAC;
    int xB0;
    int xB4;
    int xB8;
    int xBC;
    int xC0;
    int xC4;
    int xC8;
    int xCC;
    int xD0;
    int xD4;
    int xD8;
    int xDC;
    int xE0;
    int xE4;
    int xE8;
    unsigned int xEC;
    int xF0;
    int xF4;
    int xF8;
    int xFC;
    long x100;
    int x108;
    int x10C;
    int x110;
    int x114;
    _sceMpegAlalc alalc; /* 0x118 */
    int x128;
    int x12C;
    int x130;
    int x134; /* horizontal size */
    int x138; /* vertical size */
    int x13C; /* mb width */
    int x140; /* mb height */
    int x144;
    int x148;
    int x14C; /* progressive sequence */
    int x150; /* chroma format */
    int x154;
    int x158;
    int x15C;
    int x160; /* picture coding type */
    int x164;
    int x168;
    int x16C;
    int x170;
    int x174;
    int x178;
    int x17C;
    int x180;
    int x184; /* picture structure */
    int x188;
    int x18C;
    int x190;
    int x194;
    int x198;
    int hoff[3]; /* 0x19C */
    int voff[3]; /* 0x1A8 */
    int x1B4;
    int x1B8;
    int x1BC;
    int x1C0;
    int x1C4;
    _sceMpegPicture *frame[4]; /* 0x1C8 */
    _sceMpegPicture *top[4]; /* 0x1D8 */
    _sceMpegPicture *bottom[4]; /* 0x1E8 */
    _sceMpegPicture pict[9]; /* 0x1F8 */
    _sceMpegSpr spr[2]; /* 0x5A0 */
    int x820;
    int _pad824;
    int x828;
    int x82C;
    unsigned int x830;
    int x834;
    long x838;
    long x840;
    int x848; /* bit buffer */
    int x84C; /* bits left in it */
    int x850;
    int x854;
    int x858;
    int x85C;
    int x860;
    int x864;
    sceMpeg *mp; /* 0x868 */
    char _pad86C[0x878 - 0x86C];
    int abort; /* 0x878 */
    int buffType; /* 0x87C */
    unsigned char bstag[0x300]; /* 0x880 */
    unsigned char idct[0x300]; /* 0xB80 */
    unsigned char xE80[0x300];
    unsigned long dmatag[0x104]; /* 0x1180 */
};

#define REG(a) (*(volatile unsigned int *)(a))
#define D_ENABLER 0x1000F520
#define D_ENABLEW 0x1000F590
#define IPU_CTRL 0x10002010
#define D3_CHCR 0x1000B000
#define D3_QWC 0x1000B020
#define D4_CHCR 0x1000B400
#define D4_QWC 0x1000B420
#define D4_TADR 0x1000B430
#define D9_CHCR 0x1000D400
#define D9_QWC 0x1000D420

#define SET_GP(gp)                                                            \
    ({                                                                        \
        void *__old;                                                          \
        __asm__ volatile("move %0,$gp\n\tmove $gp,%1" : "=&r"(__old) : "r"(gp) : "memory"); \
        __old;                                                                \
    })

#endif
