#include "shape.h"
#include "draw.h"
#include "include_asm.h"
#include "global_w.h"
#include "System/Graphic/Effect/libppp/keProg/KeTh.h"

typedef u_long128 u_qword;
extern u_long128 ot1_mpg_table[];

void AssertApp(char *file, int line, char *exp, int arg);
void pppSendVu1Prog(int vu1prog);
void pppSendTagVif1Packet(sceDmaTag *pkt);
extern u_char ppvVu1SprDbId;

extern u_long128 pppVu1ShapeProg[];
extern u_long128 pppVu1FogShapeProg[];
extern u_long128 pppVU1CommonVector[];
extern u_long128 pppVU1FogCommonVector[];
extern u_long128 pppVU1ZTest0[];
extern u_long128 pppVU1ZTest1[];
extern u_long128 pppVU1ZTest2[];
extern u_long128 pppVU1FogZTest0[];
extern u_long128 pppVU1FogZTest1[];
extern u_long128 pppVU1FogZTest2[];
extern float pppVU1FogParameter[];
extern sceDmaTag pppVu1ShapeGifPacket;
extern sceDmaTag pppVu1FogShapeGifPacket;

static long128 s_InitVu1ShapePacket[558];
static long128 s_InitVu1FogShapePacket[558];

static char *makeVu1ShapeNoRotPacket(char *base, int vuid, int fog) {
    int i;

    *(long128 *)base = 0;
    if (vuid == 0) {
        ((int *)base)[3] = 0x6C600063;
    } else {
        ((int *)base)[3] = 0x6C600253;
    }
    base += 16;
    for (i = 12; i != 0; i--) {
        if (fog != 0) {
            *(u_long *)(base + 0x00) = 0xE0768000L << 31 | 0x8001;
        } else {
            *(u_long *)(base + 0x00) = 0xE0568000L << 31 | 0x8001;
        }
        *(u_long *)(base + 0x08) = 0x42412E6;
        *(long128 *)(base + 0x10) = 0;
        *(u_long *)(base + 0x20) = 0;
        *(u_long *)(base + 0x28) = 0x42;
        *(long128 *)(base + 0x30) = 0;
        *(long128 *)(base + 0x40) = 0;
        *(long128 *)(base + 0x50) = 0;
        *(long128 *)(base + 0x60) = 0;
        *(long128 *)(base + 0x70) = 0;
        *(int *)(base + 0x5C) = 0x8000;
        *(float *)(base + 0x68) = 1.0f;
        *(float *)(base + 0x38) = 1.0f;
        base += 0x80;
    }
    return base;
}

static char *makeVu1ShapeRotPacket(char *base, int vuid, int fog) {
    int i;

    *(long128 *)base = 0;
    if (vuid == 0) {
        ((int *)base)[3] = 0x6CB400C3;
    } else {
        ((int *)base)[3] = 0x6CB402B3;
    }
    base += 16;
    for (i = 12; i != 0; i--) {
        if (fog != 0) {
            *(u_long *)(base + 0x00) = 0xE03A400000008001L;
        } else {
            *(u_long *)(base + 0x00) = 0xE02A400000008001L;
        }
        *(u_long *)(base + 0x08) = 0x412412412412E6L;
        *(long128 *)(base + 0xE0) = 0;
        *(long128 *)(base + 0xD0) = 0;
        *(long128 *)(base + 0xC0) = 0;
        *(long128 *)(base + 0xB0) = 0;
        *(long128 *)(base + 0xA0) = 0;
        *(long128 *)(base + 0x90) = 0;
        *(long128 *)(base + 0x80) = 0;
        *(long128 *)(base + 0x70) = 0;
        *(long128 *)(base + 0x60) = 0;
        *(long128 *)(base + 0x50) = 0;
        *(long128 *)(base + 0x40) = 0;
        *(long128 *)(base + 0x30) = 0;
        *(u_long *)(base + 0x28) = 0x42;
        *(u_long *)(base + 0x20) = 0;
        *(long128 *)(base + 0x10) = 0;
        *(u_long *)(base + 0x20) = 0;
        *(u_long *)(base + 0x28) = 0x42;
        *(long128 *)(base + 0xE0) = 0;
        *(long128 *)(base + 0xD0) = 0;
        *(long128 *)(base + 0xC0) = 0;
        *(long128 *)(base + 0xB0) = 0;
        *(long128 *)(base + 0xA0) = 0;
        *(long128 *)(base + 0x90) = 0;
        *(long128 *)(base + 0x80) = 0;
        *(long128 *)(base + 0x70) = 0;
        *(long128 *)(base + 0x60) = 0;
        *(long128 *)(base + 0x50) = 0;
        *(long128 *)(base + 0x40) = 0;
        *(long128 *)(base + 0x30) = 0;
        *(int *)(base + 0x8C) = 0x8000;
        *(int *)(base + 0x5C) = 0x8000;
        *(float *)(base + 0xC8) = 1.0f;
        *(float *)(base + 0x98) = 1.0f;
        *(float *)(base + 0x68) = 1.0f;
        *(float *)(base + 0x38) = 1.0f;
        base += 0xF0;
    }
    return base;
}

void pppInitVu1ShapeProg(void) {
    char *base;
    u_long *p;

    p = (u_long *)&pppVU1CommonVector[4];
    p[0] = 0xE0568000L << 31 | 0x0001;
    p[1] = 0x42412E6;
    p[2] = 0xE0568000L << 31 | 0x8001;
    p[3] = 0x42412E6;
    p[4] = 0xE02A400000000001L;
    p[5] = 0x412412412412E6L;
    p[6] = 0xE02A400000008001L;
    p[7] = 0x412412412412E6L;

    p = (u_long *)pppVU1ZTest0;
    p[0] = 0x2000000000008001L;
    p[1] = 0xEE;
    p[2] = 0x7000D;
    p[3] = 0x47;
    p[4] = (long)*(volatile int *)&ppvZFrameBase | 0x31000000 | 0x100000000;
    p[5] = 0x4E;
    p = (u_long *)pppVU1ZTest1;
    p[0] = 0x2000000000008001L;
    p[1] = 0xEE;
    p[2] = 0x3000D;
    p[3] = 0x47;
    p[4] = (long)*(volatile int *)&ppvZFrameBase | 0x31000000 | 0x100000000;
    p[5] = 0x4E;
    p = (u_long *)pppVU1ZTest2;
    p[0] = 0x2000000000008001L;
    p[1] = 0xEE;
    p[2] = 0x7000D;
    p[3] = 0x47;
    p[4] = (long)*(volatile int *)&ppvZFrameBase | 0x31000000;
    p[5] = 0x4E;

    pppVu1ShapeGifPacket.qwc = 558;
    pppVu1ShapeGifPacket.next = (sceDmaTag *)s_InitVu1ShapePacket;
    pppVu1ShapeGifPacket.id = 0x30;
    pppVu1ShapeGifPacket.p[0] = 0;
    pppVu1ShapeGifPacket.p[1] = 0x51000002;
    ((u_long *)s_InitVu1ShapePacket)[0] = 0x1000000000008001L;
    ((u_long *)s_InitVu1ShapePacket)[1] = 0xE;
    ((u_long *)s_InitVu1ShapePacket)[2] = 5;
    ((u_long *)s_InitVu1ShapePacket)[3] = 8;
    base = makeVu1ShapeNoRotPacket((char *)s_InitVu1ShapePacket + 0x20, 0, 0);
    base = makeVu1ShapeNoRotPacket(base, 1, 0);
    base = makeVu1ShapeRotPacket(base, 0, 0);
    base = makeVu1ShapeRotPacket(base, 1, 0);

    p = (u_long *)&pppVU1FogCommonVector[4];
    p[0] = 0xE0768000L << 31 | 0x0001;
    p[1] = 0x42412E6;
    p[2] = 0xE0768000L << 31 | 0x8001;
    p[3] = 0x42412E6;
    p[4] = 0xE03A400000000001L;
    p[5] = 0x412412412412E6L;
    p[6] = 0xE03A400000008001L;
    p[7] = 0x412412412412E6L;

    p = (u_long *)pppVU1FogZTest0;
    p[0] = 0x2000000000008001L;
    p[1] = 0xEE;
    p[2] = 0x7000D;
    p[3] = 0x47;
    p[4] = (long)*(volatile int *)&ppvZFrameBase | 0x31000000 | 0x100000000;
    p[5] = 0x4E;
    p = (u_long *)pppVU1FogZTest1;
    p[0] = 0x2000000000008001L;
    p[1] = 0xEE;
    p[2] = 0x3000D;
    p[3] = 0x47;
    p[4] = (long)*(volatile int *)&ppvZFrameBase | 0x31000000 | 0x100000000;
    p[5] = 0x4E;
    p = (u_long *)pppVU1FogZTest2;
    p[0] = 0x2000000000008001L;
    p[1] = 0xEE;
    p[2] = 0x7000D;
    p[3] = 0x47;
    p[4] = (long)*(volatile int *)&ppvZFrameBase | 0x31000000;
    p[5] = 0x4E;

    p = (u_long *)pppVU1FogParameter;
    ((float *)p)[0] = OsFog.fFA;
    ((float *)p)[1] = OsFog.fFB;
    ((float *)p)[2] = OsFog.fFogMin;
    ((float *)p)[3] = OsFog.fFogMax;

    pppVu1FogShapeGifPacket.qwc = 558;
    pppVu1FogShapeGifPacket.next = (sceDmaTag *)s_InitVu1FogShapePacket;
    pppVu1FogShapeGifPacket.id = 0x30;
    pppVu1FogShapeGifPacket.p[0] = 0;
    pppVu1FogShapeGifPacket.p[1] = 0x51000002;
    ((u_long *)s_InitVu1FogShapePacket)[0] = 0x1000000000008001L;
    ((u_long *)s_InitVu1FogShapePacket)[1] = 0xE;
    ((u_long *)s_InitVu1FogShapePacket)[2] = 5;
    ((u_long *)s_InitVu1FogShapePacket)[3] = 8;
    base = makeVu1ShapeNoRotPacket((char *)s_InitVu1FogShapePacket + 0x20, 0, 1);
    base = makeVu1ShapeNoRotPacket(base, 1, 1);
    base = makeVu1ShapeRotPacket(base, 0, 1);
    base = makeVu1ShapeRotPacket(base, 1, 1);
    FlushCache(0);
}

void pppStartVu1ShapeProg(int fog) {
    if (fog == 0) {
        pppSendVu1Prog((int)pppVu1ShapeProg);
    } else {
        pppSendVu1Prog((int)pppVu1FogShapeProg);
    }
}


/* Parked: 11 of 142 lines (2026-11-07): t3/t4 swapped between the -16 mask (local) and &sh->spvu1 (global; then 0x70/0x13000000
   follow), and `or a0,a1,a0` (the call argument) issued before `addiu t6,a1,32` instead of after the tag 2 next store;
   the tag stores now match. tried: tag field orders (id/next/qwc per tag), sp = dspa0 + (spaPktSize >> 4) (34 -> 11),
   spPktSize in qwords (folds the sra) */
#if 0
#line 346
void pppVu1ShapeDraw(OSHAPE_VU1 *sh, OSHAPE_IN_VU1 *vu1shi) {
    int spPktSize;
    int spaPktSize;
    int i;
    long128 *sp; sceDmaTag *pkt; OSPRITE_VU1A *spa; OSPRITE_VU1A *dspa0; OSPRITE_VU1A *dspa; int sh_counter; float y;
    sh_counter = sh->counter;

    if (sh_counter > 12) sh_counter = 12;










    if (!(sh_counter <= 12)) AssertApp(__FILE__, __LINE__, "sh_counter <= 12", 0);


    if (ppvMonochrome) {
        y = vu1shi->r * 0.3f + vu1shi->g * 0.59f + vu1shi->b * 0.11f;
        vu1shi->b = vu1shi->g = vu1shi->r = y;
    }

    vu1shi->shape_flag = sh->flag;
    vu1shi->counter = sh_counter;
    *(u_long *)&vu1shi->tex0_l = *(u_long *)&sh->tex0_l;

    dspa = (OSPRITE_VU1A *)((char *)sh + sh->spvu1a);
    spa = dspa0 = (OSPRITE_VU1A *)(vu1shi + 1);

    i = sh_counter; while (i--) {

        *(u_long *)spa++ = *(u_long *)dspa++;
    }


    spaPktSize = ((long)(sh_counter * 8) + 15) & ~15;
    sp = (long128 *)dspa0 + (spaPktSize >> 4);




    pkt = (sceDmaTag *)sp;

    pkt->id = 0x30; pkt->next = (sceDmaTag *)((u_int)vu1shi | 0x80000000); pkt->qwc = 7;

    pkt->p[0] = 0x10000000;
    pkt->p[1] = 0x6C078000;
    pkt = (sceDmaTag *)(sp + 1);


    spPktSize = ((long)(sh->spvu1a - 0x20) + 15) & ~15;

    pkt->id = 0x30; pkt->next = (sceDmaTag *)sh->spvu1; pkt->qwc = spPktSize >> 4;
    pkt->p[0] = 0;
    pkt->p[1] = 0x6D008007 | ((spPktSize >> 4) << 17);
    pkt = (sceDmaTag *)(sp + 2);


    pkt->id = 0x30; pkt->next = (sceDmaTag *)((u_int)dspa0 | 0x80000000); pkt->qwc = spaPktSize >> 4;
    pkt->p[0] = 0;
    pkt->p[1] = 0x6E00C02B | ((spaPktSize >> 4) << 18);
    pkt = (sceDmaTag *)(sp + 3);


    pkt->id = 0x70; pkt->next = 0; pkt->qwc = 0;
    pkt->p[0] = 0x13000000;
    pkt->p[1] = 0x17000000;


    pppSendTagVif1Packet((sceDmaTag *)((u_int)sp | 0x80000000));
}
#else
/* pppVu1ShapeDraw's strings, for its INCLUDE_ASM; remove with it. */
asm(".section .rodata\n.align 3\n.asciz \"c:/KHCOM/MainApp/System/Graphic/Effect/libppp/draw/shape.c\"\n"
    ".align 3\n.asciz \"sh_counter <= 12\"\n.align 3\n.previous");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/libppp/draw/shape", pppVu1ShapeDraw);
#endif

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/libppp/draw/shape", op_shape_draw);

/* Close: 18 of 1680 lines off. The six VU0 loops are the original asm (taken from the disassembly), the
   rest matches except register allocation: the join copies the loop count (t1) to a0 before the
   zero-extension and the "move" placement differs, and the 0x3000 division in the first site uses
   swapped dividend/divisor registers (v1/v0 in the original). */
#if 0
#define SCE_GIF_SET_TAG(nloop, eop, pre, prim, flg, nreg)                                                \
    ((u_long)(nloop) | ((u_long)(eop) << 15) | ((u_long)(pre) << 46) | ((u_long)(prim) << 47) |       \
     ((u_long)(flg) << 58) | ((u_long)(nreg) << 60))

/* inline from o_draw.h */
static inline word shape_spr_get(void) {
    word spr2;
    word spr;

    sceGsSyncPath(0, 0);
    spr = spr_a[spr_index++];
    spr2 = spr + 0x2000;
    spr_a[spr_index] = spr2;
    return spr;
}

static inline void shape_ot_add(u_qword *packet, int qsize, int otz) {
    if (otz < 0) {
        otz = 0;
    } else if (otz > 255) {
        otz = 255;
    }
    {
    register u_qword *p asm("$25") = packet;
    register int q asm("$6") = qsize;
    register int z asm("$5") = otz;
    register u_long128 *ot asm("$4") = ow_ot;

    __asm__ volatile(
".set\tnoreorder\n"
"\tpsubw\t$2, $2, $2\n"
"\tlui\t$2, 0x2000\n"
"\taddi\t$3, $6, -0x1\n"
"\tor\t$2, $2, $3\n"
"\tsq\t$2, 0x0($25)\n"
"\tdsll\t$2, $5, 4\n"
"\tadd\t$2, $2, $4\n"
"\tlw\t$3, 0x8($2)\n"
"\tsw\t$25, 0x8($2)\n"
"\taddiu\t$2, $2, 0x10\n"
"\tsw\t$25, 0x4($3)\n"
"\tsw\t$2, 0x4($25)\n"
"\t.set\treorder\n"
        : : "r"(p), "r"(q), "r"(z), "r"(ot) : "$2", "$3", "memory");
    }
}

static inline void shape_ot_add2(u_qword *packet, int qsize, int otz) {
    if (otz < 0) {
        otz = 0;
    } else if (otz > 255) {
        otz = 255;
    }
    {
    register u_qword *p asm("$25") = packet;
    register int q asm("$6") = qsize;
    register int z asm("$5") = otz;
    register u_long128 *ot asm("$4") = ow_ot;

    __asm__ volatile(
".set\tnoreorder\n"
"\tpsubw\t$2, $2, $2\n"
"\tlui\t$2, 0x2000\n"
"\taddi\t$3, $6, -0x1\n"
"\tor\t$2, $2, $3\n"
"\tsq\t$2, 0x0($25)\n"
"\tdsll\t$2, $5, 4\n"
"\tadd\t$2, $2, $4\n"
"\tlw\t$3, 0x8($2)\n"
"\tsw\t$25, 0x8($2)\n"
"\taddiu\t$2, $2, 0x10\n"
"\tsw\t$25, 0x4($3)\n"
"\tsw\t$2, 0x4($25)\n"
"\t.set\treorder\n"
        : : "r"(p), "r"(q), "r"(z), "r"(ot) : "$2", "$3", "memory");
    }
}

#define SHAPE_DMA_SEND(packet, qsize, L)                                 \
    {                                                                     \
        register u_qword *p asm("$25") = (packet);                        \
        register int q asm("$6") = (qsize);                               \
                                                                          \
        __asm__ volatile(                                                 \
".set\tnoreorder\n"                                                       \
"\taddiu\t$3, $6, -0x1\n" \
"\tlui\t$2, 0x7000\n" \
"\tor\t$2, $3, $2\n" \
"\tsd\t$zero, 0x8($25)\n" \
"\tsd\t$2, 0x0($25)\n" \
"\tsync\n" \
"\tlui\t$2, 0x1000\n" \
"\tori\t$2, $2, 0xa000\n" \
"$Ldma_0017FED8" L ":\n" \
"\tlw\t$3, 0x0($2)\n" \
"\tnop\n" \
"\tnop\n" \
"\tnop\n" \
"\tnop\n" \
"\tandi\t$3, $3, 0x100\n" \
"\tbne\t$zero, $3, $Ldma_0017FED8" L "\n" \
"\tlui\t$5, 0x8000\n" \
"\tor\t$25, $5, $25\n" \
"\taddiu\t$5, $zero, -0x1\n" \
"\tlw\t$6, 0x30($2)\n" \
"\tbnel\t$6, $5, $Ldma_0017FF0C" L "\n" \
"\tsw\t$25, 0x30($2)\n" \
"$Ldma_0017FF0C" L ":\n" \
"\taddiu\t$4, $zero, -0xD\n" \
"\tsw\t$zero, 0x20($2)\n" \
"\tand\t$4, $4, $3\n" \
"\tori\t$4, $4, 0x105\n" \
"\tsw\t$4, 0x0($2)\n" \
"$Ldma_0017FF20" L ":\n" \
"\tlw\t$3, 0x0($2)\n" \
"\tnop\n" \
"\tnop\n" \
"\tnop\n" \
"\tnop\n" \
"\tandi\t$3, $3, 0x100\n" \
"\tbne\t$zero, $3, $Ldma_0017FF20" L "\n" \
"\tnop\n" \
"\t.set\treorder\n"                                                       \
            : "+r"(p) : "r"(q) : "$2", "$3", "$4", "$5", "$6", "memory");        \
    }

#define SHAPE_LOOP_REGS "$2", "$3", "$4", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15"

void *op_shape_draw(OSHAPE_VU1 *sh, OSHAPE_IN *shi, int nPacketSize) {
    char *spr;
    register char *base asm("$25");
    u_short spritec;
    word w;
    int flag;

    if (shi->flag & 0x80) {
        if (ppvCurPrimpLen + nPacketSize >= ppvPrimpLen) {
            return ppvCurPrimp;
        }
    }
    if (ppvMonochrome) {
        float y = shi->r * 0.3f + shi->g * 0.59f + shi->b * 0.11f;
        shi->b = shi->g = shi->r = y;
    }
    if (shi->flag & 0x80) {
        spr = (char *)(((u_int)shi->packet + 15) & 0xFFFFFFF0);
    } else {
        spr = (char *)shape_spr_get();
    }
    base = spr;
    {
        register OSPRITE_VU1 *sp asm("$6");
        char *spa;

        sp = sh->spvu1;
        spa = sh->spvu1a + (char *)sh;
        {
            register u_long128 *tbl asm("$3") = ot1_mpg_table;
            __asm__ volatile("lqc2 $vf12, 0x30(%0)\n"
                             "\tlqc2 $vf13, 0x40(%0)\n"
                             "\tlqc2 $vf14, 0x50(%0)"
                             : : "r"(tbl));
        }
        if (shi->flag & 0x10) {
            spr += 0x60;
        } else {
            spr += 0x50;
        }
        if (!(sh->flag & 8)) {
            if (!(shi->flag & 0xC00)) {
                if (shi->pzmat == 0) {
                    __asm__ volatile(
".set\tnoreorder\n"
"\tlhu\t$15, 0x2($17)\n"
"\tdaddu\t$9, $zero, $zero\n"
"\tlw\t$2, 0x4($16)\n"
"\tlui\t$3, 0x3c00\n"
"\tqmtc2.ni\t$3, $vf1\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tqmtc2.ni\t$2, $vf15\n"
"\tvitof0.xyzw\t$vf15, $vf15\n"
"\tvmulx.xyzw\t$vf15, $vf15, $vf1x\n"
"\tlw\t$2, 0x10($16)\n"
"\tlqc2\t$vf28, 0x0($2)\n"
"\tlqc2\t$vf29, 0x10($2)\n"
"\tlqc2\t$vf30, 0x20($2)\n"
"\tlqc2\t$vf31, 0x30($2)\n"
"\tpaddw\t$14, $zero, $zero\n"
"\tlui\t$14, 0x3f80\n"
"\tprot3w\t$14, $14\n"
"\tpaddw\t$13, $zero, $zero\n"
"\tppacw\t$12, $14, $zero\n"
"\tld\t$11, 0x10($17)\n"
"\tlhu\t$2, 0xA($16)\n"
"\tlhu\t$3, 0x8($16)\n"
"\tdsll32\t$2, $2, 5\n"
"\tor\t$2, $2, $3\n"
"\tdadd\t$11, $11, $2\n"
"\tori\t$10, $zero, 0x8000\n"
"$LshA_0017EAA8:\n"
"\tlh\t$2, 0x0(%1)\n"
"\tlh\t$3, 0x2(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf20\n"
"\tvitof4.xy\t$vf20, $vf20\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf20x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf20y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf20z\n"
"\tvmaddw.xyzw\t$vf20, $vf31, $vf20w\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvdiv\t$Q, $vf0w, $vf20w\n"
"\tvnop\n"
"\tcfc2.ni\t$2, $vi17\n"
"\tandi\t$2, $2, 0x11\n"
"\tbnez\t$2, $LshA_0017EC40\n"
"\tlh\t$2, 0x4(%1)\n"
"\tlh\t$3, 0x6(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf23\n"
"\tvitof4.xy\t$vf23, $vf23\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf20, $vf20, $Q\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf23x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf23y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf23z\n"
"\tvmaddw.xyzw\t$vf23, $vf31, $vf23w\n"
"\tvdiv\t$Q, $vf0w, $vf23w\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf23, $vf23, $Q\n"
"\tvsub.xyz\t$vf1, $vf20, $vf12\n"
"\tvsub.xyz\t$vf4, $vf23, $vf12\n"
"\tvnop\n"
"\tvnop\n"
"\tvmax.xyz\t$vf5, $vf1, $vf4\n"
"\tvmini.xyz\t$vf6, $vf1, $vf4\n"
"\tvclipw.xyz\t$vf5, $vf13w\n"
"\tvclipw.xyz\t$vf6, $vf13w\n"
"\tvclipw.xyz\t$vf5, $vf14w\n"
"\tvclipw.xyz\t$vf6, $vf14w\n"
"\tlui\t$2, 0x28\n"
"\tori\t$2, $2, 0x514a\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tcfc2.ni\t$3, $vi18\n"
"\tvclipw.xyz\t$vf5, $vf12w\n"
"\tvclipw.xyz\t$vf6, $vf12w\n"
"\tand\t$3, $3, $2\n"
"\tbnez\t$3, $LshA_0017EC40\n"
"\taddiu\t$2, $zero, 0x810\n"
"\tvftoi4.xyzw\t$vf20, $vf20\n"
"\tvftoi4.xyzw\t$vf23, $vf23\n"
"\tvnop\n"
"\tvnop\n"
"\tcfc2.ni\t$3, $vi18\n"
"\tand\t$3, $3, $2\n"
"\tbnez\t$3, $LshA_0017EC40\n"
"\tlbu\t$13, 0x7(%2)\n"
"\tpaddw\t$2, $zero, $zero\n"
"\tdsll32\t$2, $13, 5\n"
"\tpaddw\t$2, $2, $11\n"
"\tsq\t$2, 0x0(%0)\n"
"\tlw\t$2, 0x4(%2)\n"
"\taddiu\t$3, $zero, 0x42\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tsd\t$2, 0x10(%0)\n"
"\tsd\t$3, 0x18(%0)\n"
"\tlw\t$2, 0x0(%2)\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tqmtc2.ni\t$2, $vf1\n"
"\tvitof0.xyzw\t$vf1, $vf1\n"
"\tvmul.xyzw\t$vf1, $vf1, $vf15\n"
"\tvftoi0.xyzw\t$vf1, $vf1\n"
"\tsqc2\t$vf1, 0x30(%0)\n"
"\tlh\t$2, 0x8(%1)\n"
"\tlh\t$3, 0xA(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$14, $2, $zero\n"
"\tqmtc2.ni\t$14, $vf1\n"
"\tvitof12.xy\t$vf1, $vf1\n"
"\tlh\t$2, 0xC(%1)\n"
"\tlh\t$3, 0xE(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$14, $2, $zero\n"
"\tqmtc2.ni\t$14, $vf4\n"
"\tvitof12.xy\t$vf4, $vf4\n"
"\tvadd.xy\t$vf4, $vf1, $vf4\n"
"\tsqc2\t$vf20, 0x40(%0)\n"
"\tsqc2\t$vf23, 0x60(%0)\n"
"\tsw\t$zero, 0x4C(%0)\n"
"\tsw\t$zero, 0x6C(%0)\n"
"\tsqc2\t$vf1, 0x20(%0)\n"
"\tsqc2\t$vf4, 0x50(%0)\n"
"\taddiu\t$9, $9, 0x1\n"
"\taddiu\t%0, %0, 0x70\n"
"$LshA_0017EC40:\n"
"\taddi\t$15, $15, -0x1\n"
"\taddi\t%1, %1, 0x10\n"
"\taddi\t%2, %2, 0x8\n"
"\tbgtz\t$15, $LshA_0017EAA8\n"
"\tnop\n"
"\t.set\treorder\n"
                        : "+r"(spr), "+r"(sp), "+r"(spa)
                        :
                        : SHAPE_LOOP_REGS);
                } else {
                    __asm__ volatile(
".set\tnoreorder\n"
"\tlhu\t$15, 0x2($17)\n"
"\tdaddu\t$9, $zero, $zero\n"
"\tlw\t$2, 0x4($16)\n"
"\tlui\t$3, 0x3c00\n"
"\tqmtc2.ni\t$3, $vf1\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tqmtc2.ni\t$2, $vf15\n"
"\tvitof0.xyzw\t$vf15, $vf15\n"
"\tvmulx.xyzw\t$vf15, $vf15, $vf1x\n"
"\tlw\t$2, 0x10($16)\n"
"\tlqc2\t$vf28, 0x0($2)\n"
"\tlqc2\t$vf29, 0x10($2)\n"
"\tlqc2\t$vf30, 0x20($2)\n"
"\tlqc2\t$vf31, 0x30($2)\n"
"\tlw\t$3, 0x1C($16)\n"
"\tbnez\t$3, $LshB_0017ECA8\n"
"\tnop\n"
"\tdaddu\t$3, $2, $zero\n"
"$LshB_0017ECA8:\n"
"\tlqc2\t$vf8, 0x0($3)\n"
"\tlqc2\t$vf9, 0x10($3)\n"
"\tlqc2\t$vf10, 0x20($3)\n"
"\tlqc2\t$vf11, 0x30($3)\n"
"\tpaddw\t$14, $zero, $zero\n"
"\tlui\t$14, 0x3f80\n"
"\tprot3w\t$14, $14\n"
"\tpaddw\t$13, $zero, $zero\n"
"\tppacw\t$12, $14, $zero\n"
"\tld\t$11, 0x10($17)\n"
"\tlhu\t$2, 0xA($16)\n"
"\tlhu\t$3, 0x8($16)\n"
"\tdsll32\t$2, $2, 5\n"
"\tor\t$2, $2, $3\n"
"\tdadd\t$11, $11, $2\n"
"\tori\t$10, $zero, 0x8000\n"
"$LshB_0017ECE8:\n"
"\tlh\t$2, 0x0(%1)\n"
"\tlh\t$3, 0x2(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf20\n"
"\tvitof4.xy\t$vf20, $vf20\n"
"\tqmtc2.ni\t$12, $vf24\n"
"\tvitof4.xy\t$vf24, $vf24\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf20x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf20y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf20z\n"
"\tvmaddw.xyzw\t$vf20, $vf31, $vf20w\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvdiv\t$Q, $vf0w, $vf20w\n"
"\tvnop\n"
"\tcfc2.ni\t$2, $vi17\n"
"\tandi\t$2, $2, 0x11\n"
"\tbnez\t$2, $LshB_0017EEAC\n"
"\tvmulax.xyzw\t$ACC, $vf8, $vf24x\n"
"\tvmadday.xyzw\t$ACC, $vf9, $vf24y\n"
"\tvmaddaz.xyzw\t$ACC, $vf10, $vf24z\n"
"\tvmaddw.xyzw\t$vf24, $vf11, $vf24w\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf20, $vf20, $Q\n"
"\tvdiv\t$Q, $vf0w, $vf24w\n"
"\tlh\t$2, 0x4(%1)\n"
"\tlh\t$3, 0x6(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf23\n"
"\tvitof4.xy\t$vf23, $vf23\n"
"\tvwaitq\n"
"\tvmulq.z\t$vf24, $vf24, $Q\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf23x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf23y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf23z\n"
"\tvmaddw.xyzw\t$vf23, $vf31, $vf23w\n"
"\tvdiv\t$Q, $vf0w, $vf23w\n"
"\tvmove.z\t$vf20, $vf24\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf23, $vf23, $Q\n"
"\tvmove.z\t$vf23, $vf24\n"
"\tvsub.xyz\t$vf1, $vf20, $vf12\n"
"\tvsub.xyz\t$vf4, $vf23, $vf12\n"
"\tvnop\n"
"\tvnop\n"
"\tvmax.xyz\t$vf5, $vf1, $vf4\n"
"\tvmini.xyz\t$vf6, $vf1, $vf4\n"
"\tvclipw.xyz\t$vf5, $vf13w\n"
"\tvclipw.xyz\t$vf6, $vf13w\n"
"\tvclipw.xyz\t$vf5, $vf14w\n"
"\tvclipw.xyz\t$vf6, $vf14w\n"
"\tlui\t$2, 0x28\n"
"\tori\t$2, $2, 0x514a\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tcfc2.ni\t$3, $vi18\n"
"\tvclipw.xyz\t$vf5, $vf12w\n"
"\tvclipw.xyz\t$vf6, $vf12w\n"
"\tand\t$3, $3, $2\n"
"\tbnez\t$3, $LshB_0017EEAC\n"
"\taddiu\t$2, $zero, 0x810\n"
"\tvftoi4.xyzw\t$vf20, $vf20\n"
"\tvftoi4.xyzw\t$vf23, $vf23\n"
"\tvnop\n"
"\tvnop\n"
"\tcfc2.ni\t$3, $vi18\n"
"\tand\t$3, $3, $2\n"
"\tbnez\t$3, $LshB_0017EEAC\n"
"\tlbu\t$13, 0x7(%2)\n"
"\tpaddw\t$2, $zero, $zero\n"
"\tdsll32\t$2, $13, 5\n"
"\tpaddw\t$2, $2, $11\n"
"\tsq\t$2, 0x0(%0)\n"
"\tlw\t$2, 0x4(%2)\n"
"\taddiu\t$3, $zero, 0x42\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tsd\t$2, 0x10(%0)\n"
"\tsd\t$3, 0x18(%0)\n"
"\tlw\t$2, 0x0(%2)\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tqmtc2.ni\t$2, $vf1\n"
"\tvitof0.xyzw\t$vf1, $vf1\n"
"\tvmul.xyzw\t$vf1, $vf1, $vf15\n"
"\tvftoi0.xyzw\t$vf1, $vf1\n"
"\tsqc2\t$vf1, 0x30(%0)\n"
"\tlh\t$2, 0x8(%1)\n"
"\tlh\t$3, 0xA(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$14, $2, $zero\n"
"\tqmtc2.ni\t$14, $vf1\n"
"\tvitof12.xy\t$vf1, $vf1\n"
"\tlh\t$2, 0xC(%1)\n"
"\tlh\t$3, 0xE(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$14, $2, $zero\n"
"\tqmtc2.ni\t$14, $vf4\n"
"\tvitof12.xy\t$vf4, $vf4\n"
"\tvadd.xy\t$vf4, $vf1, $vf4\n"
"\tsqc2\t$vf20, 0x40(%0)\n"
"\tsqc2\t$vf23, 0x60(%0)\n"
"\tsw\t$zero, 0x4C(%0)\n"
"\tsw\t$zero, 0x6C(%0)\n"
"\tsqc2\t$vf1, 0x20(%0)\n"
"\tsqc2\t$vf4, 0x50(%0)\n"
"\taddiu\t$9, $9, 0x1\n"
"\taddiu\t%0, %0, 0x70\n"
"$LshB_0017EEAC:\n"
"\taddi\t$15, $15, -0x1\n"
"\taddi\t%1, %1, 0x10\n"
"\taddi\t%2, %2, 0x8\n"
"\tbgtz\t$15, $LshB_0017ECE8\n"
"\tnop\n"
"\t.set\treorder\n"
                        : "+r"(spr), "+r"(sp), "+r"(spa)
                        :
                        : SHAPE_LOOP_REGS);
                }
            } else {
                __asm__ volatile(
".set\tnoreorder\n"
"\tlui\t$4, %%hi(OsFog)\n"
"\taddiu\t$4, $4, %%lo(OsFog)\n"
"\tlhu\t$15, 0x2($17)\n"
"\tdaddu\t$9, $zero, $zero\n"
"\tlw\t$2, 0x4($16)\n"
"\tlui\t$3, 0x3c00\n"
"\tqmtc2.ni\t$3, $vf1\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tqmtc2.ni\t$2, $vf15\n"
"\tvitof0.xyzw\t$vf15, $vf15\n"
"\tvmulx.xyzw\t$vf15, $vf15, $vf1x\n"
"\tlw\t$2, 0x10($16)\n"
"\tlqc2\t$vf28, 0x0($2)\n"
"\tlqc2\t$vf29, 0x10($2)\n"
"\tlqc2\t$vf30, 0x20($2)\n"
"\tlqc2\t$vf31, 0x30($2)\n"
"\tlw\t$3, 0x1C($16)\n"
"\tbnez\t$3, $LshC_0017EF1C\n"
"\tnop\n"
"\tdaddu\t$3, $2, $zero\n"
"$LshC_0017EF1C:\n"
"\tlqc2\t$vf8, 0x0($3)\n"
"\tlqc2\t$vf9, 0x10($3)\n"
"\tlqc2\t$vf10, 0x20($3)\n"
"\tlqc2\t$vf11, 0x30($3)\n"
"\tpaddw\t$14, $zero, $zero\n"
"\tlui\t$14, 0x3f80\n"
"\tprot3w\t$14, $14\n"
"\tpaddw\t$13, $zero, $zero\n"
"\tppacw\t$12, $14, $zero\n"
"\tld\t$11, 0x10($17)\n"
"\tlhu\t$2, 0xA($16)\n"
"\tlhu\t$3, 0x8($16)\n"
"\tdsll32\t$2, $2, 5\n"
"\tor\t$2, $2, $3\n"
"\tdadd\t$11, $11, $2\n"
"\tori\t$10, $zero, 0x8000\n"
"\tlhu\t$2, 0x2($16)\n"
"\tandi\t$3, $2, 0x400\n"
"\tlqc2\t$vf19, 0x20($4)\n"
"\tbnez\t$3, $LshC_0017EF74\n"
"\tnop\n"
"\tlqc2\t$vf19, 0x30($4)\n"
"$LshC_0017EF74:\n"
"\tandi\t$3, $2, 0x800\n"
"\tlqc2\t$vf7, 0x0($4)\n"
"\tbnez\t$3, $LshC_0017EF88\n"
"\tnop\n"
"\tlqc2\t$vf7, 0x10($4)\n"
"$LshC_0017EF88:\n"
"\tlh\t$2, 0x0(%1)\n"
"\tlh\t$3, 0x2(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf20\n"
"\tvitof4.xy\t$vf20, $vf20\n"
"\tqmtc2.ni\t$12, $vf24\n"
"\tvitof4.xy\t$vf24, $vf24\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf20x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf20y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf20z\n"
"\tvmaddw.xyzw\t$vf20, $vf31, $vf20w\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvdiv\t$Q, $vf0w, $vf20w\n"
"\tvnop\n"
"\tcfc2.ni\t$2, $vi17\n"
"\tandi\t$2, $2, 0x11\n"
"\tbnez\t$2, $LshC_0017F184\n"
"\tvmulax.xyzw\t$ACC, $vf8, $vf24x\n"
"\tvmadday.xyzw\t$ACC, $vf9, $vf24y\n"
"\tvmaddaz.xyzw\t$ACC, $vf10, $vf24z\n"
"\tvmaddw.xyzw\t$vf24, $vf11, $vf24w\n"
"\tvmove.xyzw\t$vf18, $vf20\n"
"\tvmulay.w\t$ACC, $vf0, $vf7y\n"
"\tvmulx.w\t$vf4, $vf0, $vf7x\n"
"\tvmaddw.w\t$vf4, $vf4, $vf24w\n"
"\tvminiz.w\t$vf4, $vf4, $vf7z\n"
"\tvmaxw.w\t$vf4, $vf4, $vf7w\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf20, $vf20, $Q\n"
"\tvdiv\t$Q, $vf0w, $vf24w\n"
"\tlh\t$2, 0x4(%1)\n"
"\tlh\t$3, 0x6(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf23\n"
"\tvitof4.xy\t$vf23, $vf23\n"
"\tvwaitq\n"
"\tvmulq.z\t$vf24, $vf24, $Q\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf23x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf23y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf23z\n"
"\tvmaddw.xyzw\t$vf23, $vf31, $vf23w\n"
"\tvdiv\t$Q, $vf0w, $vf23w\n"
"\tvmove.z\t$vf20, $vf24\n"
"\tvmove.w\t$vf20, $vf4\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf23, $vf23, $Q\n"
"\tvmove.z\t$vf23, $vf24\n"
"\tvmove.w\t$vf23, $vf4\n"
"\tvsub.xyz\t$vf1, $vf20, $vf12\n"
"\tvsub.xyz\t$vf4, $vf23, $vf12\n"
"\tvnop\n"
"\tvnop\n"
"\tvmax.xyz\t$vf5, $vf1, $vf4\n"
"\tvmini.xyz\t$vf6, $vf1, $vf4\n"
"\tvclipw.xyz\t$vf5, $vf13w\n"
"\tvclipw.xyz\t$vf6, $vf13w\n"
"\tvclipw.xyz\t$vf5, $vf14w\n"
"\tvclipw.xyz\t$vf6, $vf14w\n"
"\tlui\t$2, 0x28\n"
"\tori\t$2, $2, 0x514a\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tcfc2.ni\t$3, $vi18\n"
"\tvclipw.xyz\t$vf5, $vf12w\n"
"\tvclipw.xyz\t$vf6, $vf12w\n"
"\tand\t$3, $3, $2\n"
"\tbnez\t$3, $LshC_0017F184\n"
"\taddiu\t$2, $zero, 0x810\n"
"\tvftoi4.xyzw\t$vf20, $vf20\n"
"\tvftoi4.xyzw\t$vf23, $vf23\n"
"\tvnop\n"
"\tvnop\n"
"\tcfc2.ni\t$3, $vi18\n"
"\tand\t$3, $3, $2\n"
"\tbnez\t$3, $LshC_0017F184\n"
"\tlbu\t$13, 0x7(%2)\n"
"\tpaddw\t$2, $zero, $zero\n"
"\tdsll32\t$2, $13, 5\n"
"\tpaddw\t$2, $2, $11\n"
"\tsq\t$2, 0x0(%0)\n"
"\tlw\t$2, 0x4(%2)\n"
"\taddiu\t$3, $zero, 0x42\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tsd\t$2, 0x10(%0)\n"
"\tsd\t$3, 0x18(%0)\n"
"\tlw\t$2, 0x0(%2)\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tqmtc2.ni\t$2, $vf1\n"
"\tvitof0.xyzw\t$vf1, $vf1\n"
"\tvmul.xyzw\t$vf1, $vf1, $vf15\n"
"\tlui\t$3, 0x3c00\n"
"\tqmtc2.ni\t$3, $vf2\n"
"\tvmulx.w\t$vf4, $vf18, $vf19x\n"
"\tvaddy.w\t$vf4, $vf4, $vf19y\n"
"\tvminiz.w\t$vf4, $vf4, $vf19z\n"
"\tvmaxw.w\t$vf4, $vf4, $vf19w\n"
"\tvmulw.xyzw\t$vf1, $vf1, $vf4w\n"
"\tvmulx.xyzw\t$vf1, $vf1, $vf2x\n"
"\tvftoi0.xyzw\t$vf1, $vf1\n"
"\tsqc2\t$vf1, 0x30(%0)\n"
"\tlh\t$2, 0x8(%1)\n"
"\tlh\t$3, 0xA(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$14, $2, $zero\n"
"\tqmtc2.ni\t$14, $vf1\n"
"\tvitof12.xy\t$vf1, $vf1\n"
"\tlh\t$2, 0xC(%1)\n"
"\tlh\t$3, 0xE(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$14, $2, $zero\n"
"\tqmtc2.ni\t$14, $vf4\n"
"\tvitof12.xy\t$vf4, $vf4\n"
"\tvadd.xy\t$vf4, $vf1, $vf4\n"
"\tsqc2\t$vf20, 0x40(%0)\n"
"\tsqc2\t$vf23, 0x60(%0)\n"
"\tsqc2\t$vf1, 0x20(%0)\n"
"\tsqc2\t$vf4, 0x50(%0)\n"
"\taddiu\t$9, $9, 0x1\n"
"\taddiu\t%0, %0, 0x70\n"
"$LshC_0017F184:\n"
"\taddi\t$15, $15, -0x1\n"
"\taddi\t%1, %1, 0x10\n"
"\taddi\t%2, %2, 0x8\n"
"\tbgtz\t$15, $LshC_0017EF88\n"
"\tnop\n"
"\t.set\treorder\n"
                    : "+r"(spr), "+r"(sp), "+r"(spa)
                    :
                    : SHAPE_LOOP_REGS);
            }
        } else {
            if (!(shi->flag & 0xC00)) {
                if (shi->pzmat == 0) {
                    __asm__ volatile(
".set\tnoreorder\n"
"\tlhu\t$15, 0x2($17)\n"
"\tdaddu\t$9, $zero, $zero\n"
"\tlw\t$2, 0x4($16)\n"
"\tlui\t$3, 0x3c00\n"
"\tqmtc2.ni\t$3, $vf1\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tqmtc2.ni\t$2, $vf15\n"
"\tvitof0.xyzw\t$vf15, $vf15\n"
"\tvmulx.xyzw\t$vf15, $vf15, $vf1x\n"
"\tlw\t$2, 0x10($16)\n"
"\tlqc2\t$vf28, 0x0($2)\n"
"\tlqc2\t$vf29, 0x10($2)\n"
"\tlqc2\t$vf30, 0x20($2)\n"
"\tlqc2\t$vf31, 0x30($2)\n"
"\tpaddw\t$14, $zero, $zero\n"
"\tlui\t$14, 0x3f80\n"
"\tprot3w\t$14, $14\n"
"\tpaddw\t$13, $zero, $zero\n"
"\tppacw\t$12, $14, $zero\n"
"\tld\t$11, 0x10($17)\n"
"\tlhu\t$2, 0xA($16)\n"
"\tlhu\t$3, 0x8($16)\n"
"\tdsll32\t$2, $2, 5\n"
"\tor\t$2, $2, $3\n"
"\tdadd\t$11, $11, $2\n"
"\tori\t$10, $zero, 0x8000\n"
"$LshD_0017F220:\n"
"\tlh\t$2, 0x0(%1)\n"
"\tlh\t$3, 0x2(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf20\n"
"\tvitof4.xy\t$vf20, $vf20\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf20x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf20y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf20z\n"
"\tvmaddw.xyzw\t$vf20, $vf31, $vf20w\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvdiv\t$Q, $vf0w, $vf20w\n"
"\tvnop\n"
"\tcfc2.ni\t$2, $vi17\n"
"\tandi\t$2, $2, 0x11\n"
"\tbnez\t$2, $LshD_0017F4BC\n"
"\tlh\t$2, 0x4(%1)\n"
"\tlh\t$3, 0x6(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf21\n"
"\tvitof4.xy\t$vf21, $vf21\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf20, $vf20, $Q\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf21x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf21y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf21z\n"
"\tvmaddw.xyzw\t$vf21, $vf31, $vf21w\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvdiv\t$Q, $vf0w, $vf21w\n"
"\tvnop\n"
"\tcfc2.ni\t$2, $vi17\n"
"\tandi\t$2, $2, 0x11\n"
"\tbnez\t$2, $LshD_0017F4BC\n"
"\tlh\t$2, 0x8(%1)\n"
"\tlh\t$3, 0xA(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf22\n"
"\tvitof4.xy\t$vf22, $vf22\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf21, $vf21, $Q\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf22x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf22y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf22z\n"
"\tvmaddw.xyzw\t$vf22, $vf31, $vf22w\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvdiv\t$Q, $vf0w, $vf22w\n"
"\tvnop\n"
"\tcfc2.ni\t$2, $vi17\n"
"\tandi\t$2, $2, 0x11\n"
"\tbnez\t$2, $LshD_0017F4BC\n"
"\tlh\t$2, 0xC(%1)\n"
"\tlh\t$3, 0xE(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf23\n"
"\tvitof4.xy\t$vf23, $vf23\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf22, $vf22, $Q\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf23x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf23y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf23z\n"
"\tvmaddw.xyzw\t$vf23, $vf31, $vf23w\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvdiv\t$Q, $vf0w, $vf23w\n"
"\tvnop\n"
"\tcfc2.ni\t$2, $vi17\n"
"\tandi\t$2, $2, 0x11\n"
"\tbnez\t$2, $LshD_0017F4BC\n"
"\tnop\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf23, $vf23, $Q\n"
"\tvsub.xyz\t$vf1, $vf20, $vf12\n"
"\tvsub.xyz\t$vf2, $vf21, $vf12\n"
"\tvsub.xyz\t$vf3, $vf22, $vf12\n"
"\tvsub.xyz\t$vf4, $vf23, $vf12\n"
"\tvmax.xyz\t$vf5, $vf1, $vf2\n"
"\tvmini.xyz\t$vf6, $vf1, $vf2\n"
"\tvmax.xyz\t$vf5, $vf5, $vf3\n"
"\tvmini.xyz\t$vf6, $vf6, $vf3\n"
"\tvmax.xyz\t$vf5, $vf5, $vf4\n"
"\tvmini.xyz\t$vf6, $vf6, $vf4\n"
"\tvclipw.xyz\t$vf5, $vf13w\n"
"\tvclipw.xyz\t$vf6, $vf13w\n"
"\tvclipw.xyz\t$vf5, $vf14w\n"
"\tvclipw.xyz\t$vf6, $vf14w\n"
"\tlui\t$2, 0x28\n"
"\tori\t$2, $2, 0x514a\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tcfc2.ni\t$3, $vi18\n"
"\tvclipw.xyz\t$vf5, $vf12w\n"
"\tvclipw.xyz\t$vf6, $vf12w\n"
"\tand\t$3, $3, $2\n"
"\tbnez\t$3, $LshD_0017F4BC\n"
"\taddiu\t$2, $zero, 0x810\n"
"\tvftoi4.xyzw\t$vf20, $vf20\n"
"\tvftoi4.xyzw\t$vf21, $vf21\n"
"\tvftoi4.xyzw\t$vf22, $vf22\n"
"\tvftoi4.xyzw\t$vf23, $vf23\n"
"\tcfc2.ni\t$3, $vi18\n"
"\tand\t$3, $3, $2\n"
"\tbnez\t$3, $LshD_0017F4BC\n"
"\tlbu\t$13, 0x7(%2)\n"
"\tpaddw\t$2, $zero, $zero\n"
"\tdsll32\t$2, $13, 5\n"
"\tpaddw\t$2, $2, $11\n"
"\tsq\t$2, 0x0(%0)\n"
"\tlw\t$2, 0x4(%2)\n"
"\taddiu\t$3, $zero, 0x42\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tsd\t$2, 0x10(%0)\n"
"\tsd\t$3, 0x18(%0)\n"
"\tlw\t$2, 0x0(%2)\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tqmtc2.ni\t$2, $vf1\n"
"\tvitof0.xyzw\t$vf1, $vf1\n"
"\tvmul.xyzw\t$vf1, $vf1, $vf15\n"
"\tvftoi0.xyzw\t$vf1, $vf1\n"
"\tsqc2\t$vf1, 0x30(%0)\n"
"\tsqc2\t$vf1, 0x60(%0)\n"
"\tsqc2\t$vf1, 0x90(%0)\n"
"\tsqc2\t$vf1, 0xC0(%0)\n"
"\tlh\t$2, 0x10(%1)\n"
"\tlh\t$3, 0x12(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$14, $2, $zero\n"
"\tqmtc2.ni\t$14, $vf1\n"
"\tvitof12.xy\t$vf1, $vf1\n"
"\tlh\t$2, 0x14(%1)\n"
"\tlh\t$3, 0x16(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$14, $2, $zero\n"
"\tqmtc2.ni\t$14, $vf4\n"
"\tvitof12.xy\t$vf4, $vf4\n"
"\tvmove.xyzw\t$vf2, $vf1\n"
"\tvmove.xyzw\t$vf3, $vf1\n"
"\tvadd.x\t$vf2, $vf2, $vf4\n"
"\tvadd.y\t$vf3, $vf3, $vf4\n"
"\tvadd.xy\t$vf4, $vf1, $vf4\n"
"\tsqc2\t$vf20, 0x40(%0)\n"
"\tsqc2\t$vf21, 0x70(%0)\n"
"\tsqc2\t$vf22, 0xA0(%0)\n"
"\tsqc2\t$vf23, 0xD0(%0)\n"
"\tsw\t$10, 0x4C(%0)\n"
"\tsw\t$10, 0x7C(%0)\n"
"\tsw\t$zero, 0xAC(%0)\n"
"\tsw\t$zero, 0xDC(%0)\n"
"\tsqc2\t$vf1, 0x20(%0)\n"
"\tsqc2\t$vf2, 0x50(%0)\n"
"\tsqc2\t$vf3, 0x80(%0)\n"
"\tsqc2\t$vf4, 0xB0(%0)\n"
"\taddiu\t$9, $9, 0x1\n"
"\taddiu\t%0, %0, 0xE0\n"
"$LshD_0017F4BC:\n"
"\taddi\t$15, $15, -0x1\n"
"\taddi\t%1, %1, 0x18\n"
"\taddi\t%2, %2, 0x8\n"
"\tbgtz\t$15, $LshD_0017F220\n"
"\tnop\n"
"\t.set\treorder\n"
                        : "+r"(spr), "+r"(sp), "+r"(spa)
                        :
                        : SHAPE_LOOP_REGS);
                } else {
                    __asm__ volatile(
".set\tnoreorder\n"
"\tlhu\t$15, 0x2($17)\n"
"\tdaddu\t$9, $zero, $zero\n"
"\tlw\t$2, 0x4($16)\n"
"\tlui\t$3, 0x3c00\n"
"\tqmtc2.ni\t$3, $vf1\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tqmtc2.ni\t$2, $vf15\n"
"\tvitof0.xyzw\t$vf15, $vf15\n"
"\tvmulx.xyzw\t$vf15, $vf15, $vf1x\n"
"\tlw\t$2, 0x10($16)\n"
"\tlqc2\t$vf28, 0x0($2)\n"
"\tlqc2\t$vf29, 0x10($2)\n"
"\tlqc2\t$vf30, 0x20($2)\n"
"\tlqc2\t$vf31, 0x30($2)\n"
"\tlw\t$3, 0x1C($16)\n"
"\tbnez\t$3, $LshE_0017F524\n"
"\tnop\n"
"\tdaddu\t$3, $2, $zero\n"
"$LshE_0017F524:\n"
"\tlqc2\t$vf8, 0x0($3)\n"
"\tlqc2\t$vf9, 0x10($3)\n"
"\tlqc2\t$vf10, 0x20($3)\n"
"\tlqc2\t$vf11, 0x30($3)\n"
"\tpaddw\t$14, $zero, $zero\n"
"\tlui\t$14, 0x3f80\n"
"\tprot3w\t$14, $14\n"
"\tpaddw\t$13, $zero, $zero\n"
"\tppacw\t$12, $14, $zero\n"
"\tld\t$11, 0x10($17)\n"
"\tlhu\t$2, 0xA($16)\n"
"\tlhu\t$3, 0x8($16)\n"
"\tdsll32\t$2, $2, 5\n"
"\tor\t$2, $2, $3\n"
"\tdadd\t$11, $11, $2\n"
"\tori\t$10, $zero, 0x8000\n"
"$LshE_0017F564:\n"
"\tlh\t$2, 0x0(%1)\n"
"\tlh\t$3, 0x2(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf20\n"
"\tvitof4.xy\t$vf20, $vf20\n"
"\tqmtc2.ni\t$12, $vf24\n"
"\tvitof4.xy\t$vf24, $vf24\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf20x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf20y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf20z\n"
"\tvmaddw.xyzw\t$vf20, $vf31, $vf20w\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvdiv\t$Q, $vf0w, $vf20w\n"
"\tvnop\n"
"\tcfc2.ni\t$2, $vi17\n"
"\tandi\t$2, $2, 0x11\n"
"\tbnez\t$2, $LshE_0017F88C\n"
"\tvmulax.xyzw\t$ACC, $vf8, $vf24x\n"
"\tvmadday.xyzw\t$ACC, $vf9, $vf24y\n"
"\tvmaddaz.xyzw\t$ACC, $vf10, $vf24z\n"
"\tvmaddw.xyzw\t$vf24, $vf11, $vf24w\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf20, $vf20, $Q\n"
"\tvdiv\t$Q, $vf0w, $vf24w\n"
"\tlh\t$2, 0x4(%1)\n"
"\tlh\t$3, 0x6(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf21\n"
"\tvitof4.xy\t$vf21, $vf21\n"
"\tqmtc2.ni\t$12, $vf25\n"
"\tvitof4.xy\t$vf25, $vf25\n"
"\tvwaitq\n"
"\tvmulq.z\t$vf20, $vf24, $Q\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf21x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf21y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf21z\n"
"\tvmaddw.xyzw\t$vf21, $vf31, $vf21w\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvdiv\t$Q, $vf0w, $vf21w\n"
"\tvnop\n"
"\tcfc2.ni\t$2, $vi17\n"
"\tandi\t$2, $2, 0x11\n"
"\tbnez\t$2, $LshE_0017F88C\n"
"\tvmulax.xyzw\t$ACC, $vf8, $vf25x\n"
"\tvmadday.xyzw\t$ACC, $vf9, $vf25y\n"
"\tvmaddaz.xyzw\t$ACC, $vf10, $vf25z\n"
"\tvmaddw.xyzw\t$vf25, $vf11, $vf25w\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf21, $vf21, $Q\n"
"\tvdiv\t$Q, $vf0w, $vf25w\n"
"\tlh\t$2, 0x8(%1)\n"
"\tlh\t$3, 0xA(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf22\n"
"\tvitof4.xy\t$vf22, $vf22\n"
"\tqmtc2.ni\t$12, $vf26\n"
"\tvitof4.xy\t$vf26, $vf26\n"
"\tvwaitq\n"
"\tvmulq.z\t$vf21, $vf25, $Q\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf22x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf22y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf22z\n"
"\tvmaddw.xyzw\t$vf22, $vf31, $vf22w\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvdiv\t$Q, $vf0w, $vf22w\n"
"\tvnop\n"
"\tcfc2.ni\t$2, $vi17\n"
"\tandi\t$2, $2, 0x11\n"
"\tbnez\t$2, $LshE_0017F88C\n"
"\tvmulax.xyzw\t$ACC, $vf8, $vf26x\n"
"\tvmadday.xyzw\t$ACC, $vf9, $vf26y\n"
"\tvmaddaz.xyzw\t$ACC, $vf10, $vf26z\n"
"\tvmaddw.xyzw\t$vf26, $vf11, $vf26w\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf22, $vf22, $Q\n"
"\tvdiv\t$Q, $vf0w, $vf26w\n"
"\tlh\t$2, 0xC(%1)\n"
"\tlh\t$3, 0xE(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf23\n"
"\tvitof4.xy\t$vf23, $vf23\n"
"\tqmtc2.ni\t$12, $vf27\n"
"\tvitof4.xy\t$vf27, $vf27\n"
"\tvwaitq\n"
"\tvmulq.z\t$vf22, $vf26, $Q\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf23x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf23y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf23z\n"
"\tvmaddw.xyzw\t$vf23, $vf31, $vf23w\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvdiv\t$Q, $vf0w, $vf23w\n"
"\tvnop\n"
"\tcfc2.ni\t$2, $vi17\n"
"\tandi\t$2, $2, 0x11\n"
"\tbnez\t$2, $LshE_0017F88C\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf27x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf27y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf27z\n"
"\tvmaddw.xyzw\t$vf27, $vf31, $vf27w\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf23, $vf23, $Q\n"
"\tvdiv\t$Q, $vf0w, $vf27w\n"
"\tvwaitq\n"
"\tvmulq.z\t$vf23, $vf27, $Q\n"
"\tvsub.xyz\t$vf1, $vf20, $vf12\n"
"\tvsub.xyz\t$vf2, $vf21, $vf12\n"
"\tvsub.xyz\t$vf3, $vf22, $vf12\n"
"\tvsub.xyz\t$vf4, $vf23, $vf12\n"
"\tvmax.xyz\t$vf5, $vf1, $vf2\n"
"\tvmini.xyz\t$vf6, $vf1, $vf2\n"
"\tvmax.xyz\t$vf5, $vf5, $vf3\n"
"\tvmini.xyz\t$vf6, $vf6, $vf3\n"
"\tvmax.xyz\t$vf5, $vf5, $vf4\n"
"\tvmini.xyz\t$vf6, $vf6, $vf4\n"
"\tvclipw.xyz\t$vf5, $vf13w\n"
"\tvclipw.xyz\t$vf6, $vf13w\n"
"\tvclipw.xyz\t$vf5, $vf14w\n"
"\tvclipw.xyz\t$vf6, $vf14w\n"
"\tlui\t$2, 0x28\n"
"\tori\t$2, $2, 0x514a\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tcfc2.ni\t$3, $vi18\n"
"\tvclipw.xyz\t$vf5, $vf12w\n"
"\tvclipw.xyz\t$vf6, $vf12w\n"
"\tand\t$3, $3, $2\n"
"\tbnez\t$3, $LshE_0017F88C\n"
"\taddiu\t$2, $zero, 0x810\n"
"\tvftoi4.xyzw\t$vf20, $vf20\n"
"\tvftoi4.xyzw\t$vf21, $vf21\n"
"\tvftoi4.xyzw\t$vf22, $vf22\n"
"\tvftoi4.xyzw\t$vf23, $vf23\n"
"\tcfc2.ni\t$3, $vi18\n"
"\tand\t$3, $3, $2\n"
"\tbnez\t$3, $LshE_0017F88C\n"
"\tlbu\t$13, 0x7(%2)\n"
"\tpaddw\t$2, $zero, $zero\n"
"\tdsll32\t$2, $13, 5\n"
"\tpaddw\t$2, $2, $11\n"
"\tsq\t$2, 0x0(%0)\n"
"\tlw\t$2, 0x4(%2)\n"
"\taddiu\t$3, $zero, 0x42\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tsd\t$2, 0x10(%0)\n"
"\tsd\t$3, 0x18(%0)\n"
"\tlw\t$2, 0x0(%2)\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tqmtc2.ni\t$2, $vf1\n"
"\tvitof0.xyzw\t$vf1, $vf1\n"
"\tvmul.xyzw\t$vf1, $vf1, $vf15\n"
"\tvftoi0.xyzw\t$vf1, $vf1\n"
"\tsqc2\t$vf1, 0x30(%0)\n"
"\tsqc2\t$vf1, 0x60(%0)\n"
"\tsqc2\t$vf1, 0x90(%0)\n"
"\tsqc2\t$vf1, 0xC0(%0)\n"
"\tlh\t$2, 0x10(%1)\n"
"\tlh\t$3, 0x12(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$14, $2, $zero\n"
"\tqmtc2.ni\t$14, $vf1\n"
"\tvitof12.xy\t$vf1, $vf1\n"
"\tlh\t$2, 0x14(%1)\n"
"\tlh\t$3, 0x16(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$14, $2, $zero\n"
"\tqmtc2.ni\t$14, $vf4\n"
"\tvitof12.xy\t$vf4, $vf4\n"
"\tvmove.xyzw\t$vf2, $vf1\n"
"\tvmove.xyzw\t$vf3, $vf1\n"
"\tvadd.x\t$vf2, $vf2, $vf4\n"
"\tvadd.y\t$vf3, $vf3, $vf4\n"
"\tvadd.xy\t$vf4, $vf1, $vf4\n"
"\tsqc2\t$vf20, 0x40(%0)\n"
"\tsqc2\t$vf21, 0x70(%0)\n"
"\tsqc2\t$vf22, 0xA0(%0)\n"
"\tsqc2\t$vf23, 0xD0(%0)\n"
"\tsw\t$10, 0x4C(%0)\n"
"\tsw\t$10, 0x7C(%0)\n"
"\tsw\t$zero, 0xAC(%0)\n"
"\tsw\t$zero, 0xDC(%0)\n"
"\tsqc2\t$vf1, 0x20(%0)\n"
"\tsqc2\t$vf2, 0x50(%0)\n"
"\tsqc2\t$vf3, 0x80(%0)\n"
"\tsqc2\t$vf4, 0xB0(%0)\n"
"\taddiu\t$9, $9, 0x1\n"
"\taddiu\t%0, %0, 0xE0\n"
"$LshE_0017F88C:\n"
"\taddi\t$15, $15, -0x1\n"
"\taddi\t%1, %1, 0x18\n"
"\taddi\t%2, %2, 0x8\n"
"\tbgtz\t$15, $LshE_0017F564\n"
"\tnop\n"
"\t.set\treorder\n"
                        : "+r"(spr), "+r"(sp), "+r"(spa)
                        :
                        : SHAPE_LOOP_REGS);
                }
            } else {
                __asm__ volatile(
".set\tnoreorder\n"
"\tlui\t$4, %%hi(OsFog)\n"
"\taddiu\t$4, $4, %%lo(OsFog)\n"
"\tlhu\t$15, 0x2($17)\n"
"\tdaddu\t$9, $zero, $zero\n"
"\tlw\t$2, 0x4($16)\n"
"\tlui\t$3, 0x3c00\n"
"\tqmtc2.ni\t$3, $vf1\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tqmtc2.ni\t$2, $vf15\n"
"\tvitof0.xyzw\t$vf15, $vf15\n"
"\tvmulx.xyzw\t$vf15, $vf15, $vf1x\n"
"\tlw\t$2, 0x10($16)\n"
"\tlqc2\t$vf28, 0x0($2)\n"
"\tlqc2\t$vf29, 0x10($2)\n"
"\tlqc2\t$vf30, 0x20($2)\n"
"\tlqc2\t$vf31, 0x30($2)\n"
"\tlw\t$3, 0x1C($16)\n"
"\tbnez\t$3, $LshF_0017F8FC\n"
"\tnop\n"
"\tdaddu\t$3, $2, $zero\n"
"$LshF_0017F8FC:\n"
"\tlqc2\t$vf8, 0x0($3)\n"
"\tlqc2\t$vf9, 0x10($3)\n"
"\tlqc2\t$vf10, 0x20($3)\n"
"\tlqc2\t$vf11, 0x30($3)\n"
"\tpaddw\t$14, $zero, $zero\n"
"\tlui\t$14, 0x3f80\n"
"\tprot3w\t$14, $14\n"
"\tpaddw\t$13, $zero, $zero\n"
"\tppacw\t$12, $14, $zero\n"
"\tld\t$11, 0x10($17)\n"
"\tlhu\t$2, 0xA($16)\n"
"\tlhu\t$3, 0x8($16)\n"
"\tdsll32\t$2, $2, 5\n"
"\tor\t$2, $2, $3\n"
"\tdadd\t$11, $11, $2\n"
"\tori\t$10, $zero, 0x8000\n"
"\tlhu\t$2, 0x2($16)\n"
"\tandi\t$3, $2, 0x400\n"
"\tlqc2\t$vf19, 0x20($4)\n"
"\tbnez\t$3, $LshF_0017F954\n"
"\tnop\n"
"\tlqc2\t$vf19, 0x30($4)\n"
"$LshF_0017F954:\n"
"\tandi\t$3, $2, 0x800\n"
"\tlqc2\t$vf7, 0x0($4)\n"
"\tbnez\t$3, $LshF_0017F968\n"
"\tnop\n"
"\tlqc2\t$vf7, 0x10($4)\n"
"$LshF_0017F968:\n"
"\tlh\t$2, 0x0(%1)\n"
"\tlh\t$3, 0x2(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf20\n"
"\tvitof4.xy\t$vf20, $vf20\n"
"\tqmtc2.ni\t$12, $vf24\n"
"\tvitof4.xy\t$vf24, $vf24\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf20x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf20y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf20z\n"
"\tvmaddw.xyzw\t$vf20, $vf31, $vf20w\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvdiv\t$Q, $vf0w, $vf20w\n"
"\tvnop\n"
"\tcfc2.ni\t$2, $vi17\n"
"\tandi\t$2, $2, 0x11\n"
"\tbnez\t$2, $LshF_0017FCD8\n"
"\tvmulax.xyzw\t$ACC, $vf8, $vf24x\n"
"\tvmadday.xyzw\t$ACC, $vf9, $vf24y\n"
"\tvmaddaz.xyzw\t$ACC, $vf10, $vf24z\n"
"\tvmaddw.xyzw\t$vf24, $vf11, $vf24w\n"
"\tvmove.xyzw\t$vf18, $vf20\n"
"\tvmulay.w\t$ACC, $vf0, $vf7y\n"
"\tvmulx.w\t$vf4, $vf0, $vf7x\n"
"\tvmaddw.w\t$vf4, $vf4, $vf24w\n"
"\tvminiz.w\t$vf4, $vf4, $vf7z\n"
"\tvmaxw.w\t$vf4, $vf4, $vf7w\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf20, $vf20, $Q\n"
"\tvdiv\t$Q, $vf0w, $vf24w\n"
"\tlh\t$2, 0x4(%1)\n"
"\tlh\t$3, 0x6(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf21\n"
"\tvitof4.xy\t$vf21, $vf21\n"
"\tqmtc2.ni\t$12, $vf25\n"
"\tvitof4.xy\t$vf25, $vf25\n"
"\tvwaitq\n"
"\tvmulq.z\t$vf20, $vf24, $Q\n"
"\tvmove.w\t$vf20, $vf4\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf21x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf21y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf21z\n"
"\tvmaddw.xyzw\t$vf21, $vf31, $vf21w\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvdiv\t$Q, $vf0w, $vf21w\n"
"\tvnop\n"
"\tcfc2.ni\t$2, $vi17\n"
"\tandi\t$2, $2, 0x11\n"
"\tbnez\t$2, $LshF_0017FCD8\n"
"\tvmulax.xyzw\t$ACC, $vf8, $vf25x\n"
"\tvmadday.xyzw\t$ACC, $vf9, $vf25y\n"
"\tvmaddaz.xyzw\t$ACC, $vf10, $vf25z\n"
"\tvmaddw.xyzw\t$vf25, $vf11, $vf25w\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf21, $vf21, $Q\n"
"\tvdiv\t$Q, $vf0w, $vf25w\n"
"\tlh\t$2, 0x8(%1)\n"
"\tlh\t$3, 0xA(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf22\n"
"\tvitof4.xy\t$vf22, $vf22\n"
"\tqmtc2.ni\t$12, $vf26\n"
"\tvitof4.xy\t$vf26, $vf26\n"
"\tvwaitq\n"
"\tvmulq.z\t$vf21, $vf25, $Q\n"
"\tvmove.w\t$vf21, $vf4\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf22x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf22y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf22z\n"
"\tvmaddw.xyzw\t$vf22, $vf31, $vf22w\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvdiv\t$Q, $vf0w, $vf22w\n"
"\tvnop\n"
"\tcfc2.ni\t$2, $vi17\n"
"\tandi\t$2, $2, 0x11\n"
"\tbnez\t$2, $LshF_0017FCD8\n"
"\tvmulax.xyzw\t$ACC, $vf8, $vf26x\n"
"\tvmadday.xyzw\t$ACC, $vf9, $vf26y\n"
"\tvmaddaz.xyzw\t$ACC, $vf10, $vf26z\n"
"\tvmaddw.xyzw\t$vf26, $vf11, $vf26w\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf22, $vf22, $Q\n"
"\tvdiv\t$Q, $vf0w, $vf26w\n"
"\tlh\t$2, 0xC(%1)\n"
"\tlh\t$3, 0xE(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$12, $2, $zero\n"
"\tqmtc2.ni\t$12, $vf23\n"
"\tvitof4.xy\t$vf23, $vf23\n"
"\tqmtc2.ni\t$12, $vf27\n"
"\tvitof4.xy\t$vf27, $vf27\n"
"\tvwaitq\n"
"\tvmulq.z\t$vf22, $vf26, $Q\n"
"\tvmove.w\t$vf22, $vf4\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf23x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf23y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf23z\n"
"\tvmaddw.xyzw\t$vf23, $vf31, $vf23w\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvdiv\t$Q, $vf0w, $vf23w\n"
"\tvnop\n"
"\tcfc2.ni\t$2, $vi17\n"
"\tandi\t$2, $2, 0x11\n"
"\tbnez\t$2, $LshF_0017FCD8\n"
"\tvmulax.xyzw\t$ACC, $vf28, $vf27x\n"
"\tvmadday.xyzw\t$ACC, $vf29, $vf27y\n"
"\tvmaddaz.xyzw\t$ACC, $vf30, $vf27z\n"
"\tvmaddw.xyzw\t$vf27, $vf31, $vf27w\n"
"\tvwaitq\n"
"\tvmulq.xyz\t$vf23, $vf23, $Q\n"
"\tvdiv\t$Q, $vf0w, $vf27w\n"
"\tvwaitq\n"
"\tvmulq.z\t$vf23, $vf27, $Q\n"
"\tvmove.w\t$vf23, $vf4\n"
"\tvsub.xyz\t$vf1, $vf20, $vf12\n"
"\tvsub.xyz\t$vf2, $vf21, $vf12\n"
"\tvsub.xyz\t$vf3, $vf22, $vf12\n"
"\tvsub.xyz\t$vf4, $vf23, $vf12\n"
"\tvmax.xyz\t$vf5, $vf1, $vf2\n"
"\tvmini.xyz\t$vf6, $vf1, $vf2\n"
"\tvmax.xyz\t$vf5, $vf5, $vf3\n"
"\tvmini.xyz\t$vf6, $vf6, $vf3\n"
"\tvmax.xyz\t$vf5, $vf5, $vf4\n"
"\tvmini.xyz\t$vf6, $vf6, $vf4\n"
"\tvclipw.xyz\t$vf5, $vf13w\n"
"\tvclipw.xyz\t$vf6, $vf13w\n"
"\tvclipw.xyz\t$vf5, $vf14w\n"
"\tvclipw.xyz\t$vf6, $vf14w\n"
"\tlui\t$2, 0x28\n"
"\tori\t$2, $2, 0x514a\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tvnop\n"
"\tcfc2.ni\t$3, $vi18\n"
"\tvclipw.xyz\t$vf5, $vf12w\n"
"\tvclipw.xyz\t$vf6, $vf12w\n"
"\tand\t$3, $3, $2\n"
"\tbnez\t$3, $LshF_0017FCD8\n"
"\taddiu\t$2, $zero, 0x810\n"
"\tvftoi4.xyzw\t$vf20, $vf20\n"
"\tvftoi4.xyzw\t$vf21, $vf21\n"
"\tvftoi4.xyzw\t$vf22, $vf22\n"
"\tvftoi4.xyzw\t$vf23, $vf23\n"
"\tcfc2.ni\t$3, $vi18\n"
"\tand\t$3, $3, $2\n"
"\tbnez\t$3, $LshF_0017FCD8\n"
"\tlbu\t$13, 0x7(%2)\n"
"\tpaddw\t$2, $zero, $zero\n"
"\tdsll32\t$2, $13, 5\n"
"\tpaddw\t$2, $2, $11\n"
"\tsq\t$2, 0x0(%0)\n"
"\tlw\t$2, 0x4(%2)\n"
"\taddiu\t$3, $zero, 0x42\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tsd\t$2, 0x10(%0)\n"
"\tsd\t$3, 0x18(%0)\n"
"\tlw\t$2, 0x0(%2)\n"
"\tpextlb\t$2, $zero, $2\n"
"\tpextlh\t$2, $zero, $2\n"
"\tqmtc2.ni\t$2, $vf1\n"
"\tvitof0.xyzw\t$vf1, $vf1\n"
"\tvmul.xyzw\t$vf1, $vf1, $vf15\n"
"\tlui\t$3, 0x3c00\n"
"\tqmtc2.ni\t$3, $vf2\n"
"\tvmulx.w\t$vf4, $vf18, $vf19x\n"
"\tvaddy.w\t$vf4, $vf4, $vf19y\n"
"\tvminiz.w\t$vf4, $vf4, $vf19z\n"
"\tvmaxw.w\t$vf4, $vf4, $vf19w\n"
"\tvmulw.xyzw\t$vf1, $vf1, $vf4w\n"
"\tvmulx.xyzw\t$vf1, $vf1, $vf2x\n"
"\tvftoi0.xyzw\t$vf1, $vf1\n"
"\tsqc2\t$vf1, 0x30(%0)\n"
"\tsqc2\t$vf1, 0x60(%0)\n"
"\tsqc2\t$vf1, 0x90(%0)\n"
"\tsqc2\t$vf1, 0xC0(%0)\n"
"\tlh\t$2, 0x10(%1)\n"
"\tlh\t$3, 0x12(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$14, $2, $zero\n"
"\tqmtc2.ni\t$14, $vf1\n"
"\tvitof12.xy\t$vf1, $vf1\n"
"\tlh\t$2, 0x14(%1)\n"
"\tlh\t$3, 0x16(%1)\n"
"\tpextlw\t$2, $3, $2\n"
"\tdaddu\t$14, $2, $zero\n"
"\tqmtc2.ni\t$14, $vf4\n"
"\tvitof12.xy\t$vf4, $vf4\n"
"\tvmove.xyzw\t$vf2, $vf1\n"
"\tvmove.xyzw\t$vf3, $vf1\n"
"\tvadd.x\t$vf2, $vf2, $vf4\n"
"\tvadd.y\t$vf3, $vf3, $vf4\n"
"\tvadd.xy\t$vf4, $vf1, $vf4\n"
"\tsqc2\t$vf20, 0x40(%0)\n"
"\tsqc2\t$vf21, 0x70(%0)\n"
"\tsqc2\t$vf22, 0xA0(%0)\n"
"\tsqc2\t$vf23, 0xD0(%0)\n"
"\tlw\t$2, 0xDC(%0)\n"
"\tor\t$2, $10, $2\n"
"\tsw\t$2, 0x4C(%0)\n"
"\tsw\t$2, 0x7C(%0)\n"
"\tsqc2\t$vf1, 0x20(%0)\n"
"\tsqc2\t$vf2, 0x50(%0)\n"
"\tsqc2\t$vf3, 0x80(%0)\n"
"\tsqc2\t$vf4, 0xB0(%0)\n"
"\taddiu\t$9, $9, 0x1\n"
"\taddiu\t%0, %0, 0xE0\n"
"$LshF_0017FCD8:\n"
"\taddi\t$15, $15, -0x1\n"
"\taddi\t%1, %1, 0x18\n"
"\taddi\t%2, %2, 0x8\n"
"\tbgtz\t$15, $LshF_0017F968\n"
"\tnop\n"
"\t.set\treorder\n"
                    : "+r"(spr), "+r"(sp), "+r"(spa)
                    :
                    : SHAPE_LOOP_REGS);
            }
        }
    }
    w = 0;
    if (shi->flag & 0x800) {
        w = 0x20;
    }
    __asm__("move %0, $9" : "=r"(spritec));
    if (spritec != 0) {
        if (shi->flag & 0x10) {
            if (shi->flag & 0x80) {
                *(u_long *)(base + 0x10) = 0x3000000000000001L;
                *(u_long *)(base + 0x18) = 0xFE8;
                *(u_long *)(base + 0x20) = 5;
                *(u_long *)(base + 0x30) = 0x3000D;
                *(u_long *)(base + 0x38) = 0x47;
                *(u_long128 *)(base + 0x40) = 0;
            } else {
                *(u_long *)(base + 0x10) = 0x3000000000000001L;
                *(u_long *)(base + 0x18) = 0xEE8;
                *(u_long *)(base + 0x20) = 5;
                *(u_long *)(base + 0x30) = 0x3000D;
                *(u_long *)(base + 0x38) = 0x47;
                *(u_long128 *)(base + 0x40) = *(u_long128 *)&os_gs2_register.zbuf;
            }
            if (sh->flag & 8) {
                *(u_long *)(base + 0x58) = 0x412412412412E6L;
                *(u_long *)(base + 0x50) = SCE_GIF_SET_TAG(spritec, 0, 1, w | 0x54, 0, 14);
            } else {
                *(u_long *)(base + 0x58) = 0x42412E6;
                *(u_long *)(base + 0x50) = SCE_GIF_SET_TAG(spritec, 0, 1, w | 0x56, 0, 7);
            }
            *(u_long *)(spr + 0x00) = 0x1000000000008001L;
            *(u_long *)(spr + 0x08) = 0xE;
            *(u_long128 *)(spr + 0x10) = *(u_long128 *)&os_gs2_register.test;
            spr += 0x20;
            {
                int qsize = (spr - base) / 16;
                if (shi->flag & 0x80) {
                    if (shi->flag & 0x20) {
                        shape_ot_add((u_qword *)base, qsize, shi->ot);
                    } else {
                        shape_ot_add2((u_qword *)base, qsize, *(int *)(spr - 8) / 0x3000);
                    }
                } else {
                    SHAPE_DMA_SEND((u_qword *)base, qsize, "a")
                }
            }
        } else {
            if (shi->flag & 0x80) {
                *(u_long *)(base + 0x10) = 0x2000000000000001L;
                *(u_long *)(base + 0x18) = 0xF8;
                *(u_long *)(base + 0x20) = 5;
                *(u_long128 *)(base + 0x30) = 0;
            } else {
                *(u_long *)(base + 0x10) = 0x2000000000000001L;
                *(u_long *)(base + 0x18) = 0xE8;
                *(u_long *)(base + 0x20) = 5;
                if (shi->flag & 0x100) {
                    *(u_long128 *)(base + 0x30) = *(u_long128 *)&os_gs2_register2.zbuf;
                } else {
                    *(u_long128 *)(base + 0x30) = *(u_long128 *)&os_gs2_register.zbuf;
                }
            }
            if (sh->flag & 8) {
                *(u_long *)(base + 0x48) = 0x412412412412E6L;
                *(u_long *)(base + 0x40) = SCE_GIF_SET_TAG(spritec, 1, 1, w | 0x54, 0, 14);
            } else {
                *(u_long *)(base + 0x48) = 0x42412E6;
                *(u_long *)(base + 0x40) = SCE_GIF_SET_TAG(spritec, 1, 1, w | 0x56, 0, 7);
            }
            {
                int qsize = (spr - base) / 16;
                if (shi->flag & 0x80) {
                    if (shi->flag & 0x20) {
                        shape_ot_add((u_qword *)base, qsize, shi->ot);
                    } else {
                        shape_ot_add2((u_qword *)base, qsize, *(int *)(spr - 8) / 0x3000);
                    }
                } else {
                    SHAPE_DMA_SEND((u_qword *)base, qsize, "b")
                }
            }
        }
    }
    if (shi->flag & 0x80) {
        ppvCurPrimpLen += spr - (char *)ppvCurPrimp;
        return spr;
    } else {
        sceGsSyncPath(0, 0);
        spr_index--;
        return (void *)*(word *)&shi->packet;
    }
}
#endif

void *pppGetVu1DrawSpad0(void) {
    return (void *)0x70001A00;
}

void *pppGetVu1DrawSpad1(void) {
    return (void *)0x70002D00;
}

void *pppGetVu1DrawSpad(void) {
    ppvVu1SprDbId ^= 1;
    return ppvVu1SprDbId ? (void *)0x70002D00 : (void *)0x70001A00;
}
// SOURCE DATA: .rodata .bss
