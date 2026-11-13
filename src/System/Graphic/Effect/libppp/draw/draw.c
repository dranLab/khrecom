#include "draw.h"
OS_FOG OsFog; /* COMMON in the original (linker-placed) */
/* The original builds its DMA/GS packets with small inline-asm store macros: each store is an asm statement
   (a scheduling barrier), and some tag values are passed in a fixed register (not hoisted out of loops). */
#define SQ(v, p, off) __asm__ volatile("sq %0, " #off "(%1)" : : "r"(v), "r"(p))
#define SQ_ZERO(p, off) __asm__ volatile("sq $0, " #off "(%0)" : : "r"(p))
#define SW(v, p, off) __asm__ volatile("sw %0, " #off "(%1)" : : "r"(v), "r"(p))
#define SH(v, p, off) __asm__ volatile("sh %0, " #off "(%1)" : : "r"(v), "r"(p))
#define SW_REG(reg, v, p, off)                \
    {                                         \
        register int r_ asm(reg) = (v);       \
        SW(r_, p, off);                       \
    }

/* Wait for the GIF DMA channel (D2_CHCR.STR). The label uses gcc's $L form so that ee_asm_compat pads this short
   loop like the original assembler did. */
#define DMA_WAIT_GIF() \
    __asm__ volatile("\n$L9001:\n\tlw\t$2,0x1000a000\n\tandi\t$2,$2,0x100\n\tbnez\t$2,$L9001" : : : "$2")
/* Start the GIF DMA channel in source-chain mode from tag address tadr. */
#define DMA_KICK_GIF(tadr)                                                                        \
    __asm__ volatile("li $2, 0x10008000\n\tli $3, 4\n\tli $4, 0x105\n\tsw $0, 0x2020($2)\n\t" \
                     "sw %0, 0x2030($2)\n\tsw $4, 0x2000($2)"                                     \
                     : : "r"(tadr) : "$2", "$3", "$4")

u_char ot_pixel_format_tm2[] = { 0, 2, 1, 0, 20, 19 };

void op_init_draw() {
    ops_init_draw_work();
    ops_init_spr();
}

void ops_init_draw_work() {
    oa_dmagif = sceDmaGetChan(2);
}

void op_gs2_init() {
    OS_GS2_REGISTER *gs2;

    gs2 = &os_gs2_register;
    gs2->gif_code = 0x7000000000008001;
    gs2->giftag = 0xEEEEEEE;
    gs2->frame = *(volatile u_long *)&ppvGsFrame;
    gs2->frame_code = 0x4C;
    gs2->zbuf = (long)*(volatile int *)&ppvZFrameBase | 0x31000000 | 0x100000000;
    gs2->zbuf_code = 0x4E;
    gs2->test = 0x7000D;
    gs2->test_code = 0x47;
    gs2->tex1 = 0x21;
    gs2->tex1_code = 0x14;
    gs2->clamp = 0;
    gs2->clamp_code = 8;
    gs2->texa = 0x8000000000;
    gs2->texa_code = 0x3B;
    gs2->fogcol = 0x808080;
    gs2->fogcol_code = 0x3D;

    gs2 = &os_gs2_register2;
    gs2->gif_code = 0x7000000000008001;
    gs2->giftag = 0xEEEEEEE;
    gs2->frame = *(volatile u_long *)&ppvGsFrame;
    gs2->frame_code = 0x4C;
    gs2->zbuf = (long)*(volatile int *)&ppvZFrameBase | 0x31000000;
    gs2->zbuf_code = 0x4E;
    gs2->test = 0x7000D;
    gs2->test_code = 0x47;
    gs2->tex1 = 0x21;
    gs2->tex1_code = 0x14;
    gs2->clamp = 0;
    gs2->clamp_code = 8;
    gs2->texa = 0x8000000000;
    gs2->texa_code = 0x3B;
    gs2->fogcol = 0x808080;
    gs2->fogcol_code = 0x3D;
}

void op_otz_init() {
    char *cc;
    u_long128 q;
    int i;

    q = 0x20000000;
    cc = (char *)ow_ot_before;
    for (i = 0; i < 0x120; i++) {
        SQ(q, cc, 0);
        *(char **)(cc + 8) = cc;
        *(char **)(cc + 4) = cc + 16;
        cc += 16;
    }
    SQ(q, cc, 0);
    *(int *)cc = 0x70000000;
    op_ot_add_before(&os_gs2_register.dma_code, 9, 0, 0);
    op_ot_add_after(&os_gs2_register2.dma_code, 9, 15, 0);
}

void op_gs2_init_eff() {
    OS_GS2_REGISTER *gs2;
    u_long128 q;

    gs2 = &os_gs2_register2;
    q = gs2->dma_code;
    gs2->fogcol = (u_long)FogCol[0] | ((u_long)FogCol[1] << 8) | ((u_long)FogCol[2] << 16);
    gs2->dma_code = 0x70000008;
    gs2->fogcol_code = 0x3D;
    sceGsSyncPath(0, 0);
    FlushCache(0);
    sceDmaSend(oa_dmagif, gs2);
    sceGsSyncPath(0, 0);
    gs2->dma_code = q;
}

void op_ztest_init() {
    struct {
        u_long128 dmacode;
        u_long giftag[2];
        u_long test;
        u_long test_code;
        u_long zbuf;
        u_long zbuf_code;
    } packet;
    OS_GS2_REGISTER *gs2;

    gs2 = &os_gs2_register2;
    packet.dmacode = 0x70000003;
    packet.giftag[0] = 0x2000000000008001;
    packet.giftag[1] = 0xEE;
    packet.test = gs2->test;
    packet.test_code = gs2->test_code;
    packet.zbuf = gs2->zbuf;
    packet.zbuf_code = gs2->zbuf_code;
    sceGsSyncPath(0, 0);
    FlushCache(0);
    sceDmaSend(oa_dmagif, &packet);
    sceGsSyncPath(0, 0);
}

void op_random_seed() {
    register int i asm("$3");

    i = rand();
    __asm__ volatile("qmtc2.ni %0, $vf31\n\tvrxor $R, $vf31x" : : "r"(i));
}

int op_beki(int i) {
    int ret;
    int shift;

    ret = 0;
    for (shift = 1; shift < i; shift <<= 1) {
        ret++;
    }
    return ret;
}

void op_tm2_load(TM2HEADER *tm2) {
    sceGsLoadImage tex;
    sceGsLoadImage clut;
    short gs;
    short pformat;
    short x;
    short y;
    short absw;
    TM2INFO_R *infor;
    char *cc;

    infor = (TM2INFO_R *)(tm2 + 1);
    cc = 0;
    if (infor->image_size != 0) {
        pformat = ot_pixel_format_tm2[infor->image];
        gs = infor->tex0_l & 0x3FFF;
        if (tm2->otp == 0x6F) {
            absw = infor->absw * 64 / 256;
            x = infor->absx * 16;
            y = infor->absy * 16;
        } else {
            absw = infor->w / 64;
            x = 0;
            y = 0;
        }
        sceGsSetDefLoadImage(&tex, gs, absw, pformat, x, y, infor->w, infor->h);
        cc = (char *)(infor + 1);
        sceGsSyncPath(0, 0);
        FlushCache(0);
        sceGsExecLoadImage(&tex, (u_long128 *)cc);
    }
    if (infor->clut_size != 0) {
        pformat = ot_pixel_format_tm2[infor->clut];
        sceGsSetDefLoadImage(&clut, (infor->tex0_h >> 5) & 0x3FFF, 1, pformat, 0, 0, 16, infor->clut_size >> 6);
        sceGsSyncPath(0, 0);
        FlushCache(0);
        sceGsExecLoadImage(&clut, (u_long128 *)(cc + infor->image_size));
    }
}

void op_clut256_load(u_long128 *cadr, int counter, u_short gsadr, int wait) {
    char *spr;
    char *spr2;
    u_short gs;
    u_word w;
    char *cc;
    int c;

    if (counter <= 16) {
        spr = op_spr_alloc(0x800);
        sceGsSetDefLoadImage((sceGsLoadImage *)spr, gsadr, 1, 0, 0, 0, 16, 16);
        spr2 = spr + 0x100;
        SW_REG("$2", 0x30000005, spr2, 0);
        SW_REG("$2", (int)spr - (int)0x80000000, spr2, 4);
        gs = gsadr;
        w = (u_word)cadr;
        cc = spr + 0x110;
        for (c = 0; c < counter; c++) {
            SQ_ZERO(cc, 0);
            SW_REG("$2", 0x10000003, cc, 0);
            *(u_long *)(cc + 0x10) = 0x1000000000008001;
            *(u_long *)(cc + 0x18) = 0xE;
            *(u_long128 *)(cc + 0x20) = *(u_long128 *)(spr + 0x10);
            SH(gs, cc, 0x24);
            *(u_long128 *)(cc + 0x30) = *(u_long128 *)(spr + 0x50);
            SQ_ZERO(cc, 0x40);
            SW_REG("$2", 0x30000040, cc, 0x40);
            SW(w, cc, 0x44);
            gs += 4;
            w += 0x400;
            cc += 0x50;
        }
        SQ_ZERO(cc, 0);
        SW_REG("$2", 0x70000000, cc, 0);
        DMA_WAIT_GIF();
        DMA_KICK_GIF((u_int)(spr + 0x100) + 0x80000000);
        if (wait) {
            sceGsSyncPath(0, 0);
        }
        op_spr_free(0x800);
    }
}

void op_oef2_texture_load(OEF2_TEXINFO *ti, int wait) {
    sceGsLoadImage tex;

    sceGsSetDefLoadImage(&tex, ti->tex0_l & 0x3FFF, ti->absw * 64 / 256, ti->psm, ti->absx * 16, ti->absy * 16, ti->w, ti->h);
    sceGsSyncPath(0, 0);
    FlushCache(0);
    sceGsExecLoadImage(&tex, (u_long128 *)ti->data);
    if (wait) {
        sceGsSyncPath(0, 0);
    }
}

void op_oef2_clut_load(OEF2_CLUTINFO *ci, int wait) {
    op_clut256_load((u_long128 *)ci->data, ci->counter, ci->gs, wait);
}

void op_gs_clear(int gsadr, int gssize) {
    char *spr;
    char *spr2;
    char *cc;
    u_short gs;
    u_long128 q;
    char *zero;
    int c;
    int c2;

    spr = op_spr_alloc(0xC00);
    gs = gsadr;
    sceGsSetDefLoadImage((sceGsLoadImage *)spr, gs, 1, 0, 0, 0, 16, 16);
    spr2 = spr + 0x80000800;
    cc = spr + 0x100;
    SW_REG("$4", 0x30000005, cc, 0);
    SW((int)spr - (int)0x80000000, cc, 4);
    q = ((u_long128)0x4444444444444444 << 64) | 0x4444444444444444;
    zero = (char *)((u_int)spr2 & 0x7FFFFFFF);
    zero += 0x3F0;
    for (c2 = 0x3F0; c2 >= 0; c2 -= 16) {
        *(u_long128 *)zero = q;
        zero -= 16;
    }
    for (c = 0; c < gssize / 32; c++) {
        sceGsSyncPath(0, 0);
        cc = spr + 0x110;
        for (c2 = 0; c2 < 8; c2++) {
            SQ_ZERO(cc, 0);
            SW_REG("$4", 0x10000004, cc, 0);
            *(u_long *)(cc + 0x10) = 0x2000000000000001;
            *(u_long *)(cc + 0x18) = 0xEEEE;
            *(u_long128 *)(cc + 0x20) = *(u_long128 *)(spr + 0x10);
            *(u_long128 *)(cc + 0x30) = *(u_long128 *)(spr + 0x40);
            SH(gs, cc, 0x24);
            *(u_long128 *)(cc + 0x40) = *(u_long128 *)(spr + 0x50);
            SQ_ZERO(cc, 0x50);
            SW_REG("$4", 0x30000040, cc, 0x50);
            SW(spr2, cc, 0x54);
            gs += 4;
            cc += 0x60;
        }
        SQ_ZERO(cc, 0);
        SW_REG("$4", 0x70000000, cc, 0);
        DMA_KICK_GIF((u_int)(spr + 0x100) + 0x80000000);
    }
    sceGsSyncPath(0, 0);
    op_spr_free(0xC00);
}

void op_ot_draw() {
    u_long128 *qq;

    qq = ow_ot_before;
    sceGsSyncPath(0, 0);
    FlushCache(0);
    sceGsSyncPath(0, 0);
    sceDmaSend(oa_dmagif, qq);
    sceGsSyncPath(0, 0);
}

void op_ot_add_before(u_long128 *packet, int qsize, int otz, int pri) {
    char *pp;
    char *cc;
    char *cc2;
    char *cc3;
    word w;

    pp = (char *)packet;
    w = otz;
    SQ_ZERO(packet, 0);
    SW((qsize - 1) | 0x20000000, pp, 0);
    if (w <= 0) {
        w = 0;
    }
    if (w > 15) {
        w = 15;
    }
    cc = (char *)ow_ot_before;
    cc += w * 16;
    cc2 = *(char **)(cc + 8);
    cc3 = *(char **)(cc2 + 4);
    *(char **)(cc2 + 4) = pp;
    *(char **)(pp + 4) = cc3;
    *(char *volatile *)(cc + 8) = pp;
}

void op_ot_add_after(u_long128 *packet, int qsize, int otz, int pri) {
    char *pp;
    char *cc;
    char *cc2;
    char *cc3;
    word w;

    pp = (char *)packet;
    w = otz;
    SQ_ZERO(packet, 0);
    SW((qsize - 1) | 0x20000000, pp, 0);
    if (w <= 0) {
        w = 0;
    }
    if (w > 15) {
        w = 15;
    }
    cc = (char *)ow_ot_after;
    cc += w * 16;
    cc2 = *(char **)(cc + 8);
    cc3 = *(char **)(cc2 + 4);
    *(char **)(cc2 + 4) = pp;
    *(char **)(pp + 4) = cc3;
    *(char *volatile *)(cc + 8) = pp;
}

void op_ot_add(u_long128 *packet, int qsize, int otz, int pri) {
    char *pp;
    char *cc;
    char *cc2;
    char *cc3;
    word w;

    pp = (char *)packet;
    w = otz;
    SQ_ZERO(packet, 0);
    SW((qsize - 1) | 0x20000000, pp, 0);
    if (w <= 0) {
        w = 0;
    }
    if (w > 255) {
        w = 255;
    }
    cc = (char *)ow_ot;
    cc += w * 16;
    cc2 = *(char **)(cc + 8);
    cc3 = *(char **)(cc2 + 4);
    *(char **)(cc2 + 4) = pp;
    *(char **)(pp + 4) = cc3;
    *(char *volatile *)(cc + 8) = pp;
}

u_long128 *op_flash_ot(u_long128 *packet, u_word rgba, u_char alpha, u_char alphafix) {
    char *cc;
    u_long128 col;

    cc = (char *)packet;
    *(u_long *)(cc + 0x10) = 0x41A3400000008001;
    *(u_long *)(cc + 0x18) = 0x441E;
    SQ_ZERO(cc, 0x20);
    cc[0x20] = alpha;
    cc[0x24] = alphafix;
    *(u_long *)(cc + 0x28) = 0x42;
    __asm__ volatile("pextlb %0, $0, %1\n\tpextlh %0, $0, %0" : "=r"(col) : "0"(rgba));
    SQ(col, cc, 0x30);
    *(u_long128 *)(cc + 0x40) = ((u_long128)0xFFFFFFFE << 64) | 0x780000007000;
    *(u_long128 *)(cc + 0x50) = ((u_long128)0xFFFFFFFE << 64) | 0x880000009000;
    op_ot_add_after(packet, 6, 0, 0);
    return (u_long128 *)(cc + 0x60);
}

void ops_init_fog_work() {
    OsFog.fFA = OsFog.fFB = 0.0f;
    OsFog.fFogMin = 255.0f;
    OsFog.fFogMax = 0.0f;
    OsFog.fVoidFA = OsFog.fVoidFB = 0.0f;
    OsFog.fVoidFogMax = OsFog.fVoidFogMin = 255.0f;
    OsFog.fFadeRate = 80.00001f;
    OsFog.fFadeAdjust = -32.0f;
    OsFog.fFadeMin = 128.0f;
    OsFog.fFadeMax = 0.0f;
    OsFog.fVoidFadeRate = 0.0f;
    OsFog.fVoidFadeMin = OsFog.fVoidFadeAdjust = 128.0f;
    OsFog.fVoidFadeMax = 0.0f;
    FogCol[0] = FogCol[1] = FogCol[2] = 0x80;
}

void op_fog_param_set(float near, float far) {
    OsFog.fFA = -255.0f / (far - near);
    OsFog.fFB = (far * 255.0f) / (far - near);
}

void op_fog_color_set(u_char r, u_char g, u_char b) {
    FogCol[0] = r;
    FogCol[1] = g;
    FogCol[2] = b;
}

void op_fog_clamp_set(u_char min, u_char max) {
    OsFog.fFogMin = max;
    OsFog.fFogMax = min;
}
// SOURCE DATA: .data
// SOURCE DATA SLUS_217.99_retail: .data
