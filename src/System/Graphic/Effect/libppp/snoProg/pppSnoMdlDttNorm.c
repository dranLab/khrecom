#include "pppSnoMdlDttNorm.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"
#include "System/Graphic/Effect/libppp/draw/md.h"
#include "System/Graphic/Effect/libppp/draw/md0_p3f.h"
#include "System/Graphic/Effect/libppp/draw/draw_w.h"
#include "System/Graphic/Effect/libppp/keProg/pppKeDMat.h"

void pppSendGsPacket(u_long128 *pkt_top);

typedef struct { /* 0x1c */
    u_char r0;
    u_char g0;
    u_char b0;
    u_char a0;
    u_char r1;
    u_char g1;
    u_char b1;
    u_char a1;
    u_char r2;
    u_char g2;
    u_char b2;
    u_char a2;
    u_short v0;
    u_short v1;
    u_short v2;
    u_short flag;
    u_short n0;
    u_short n1;
    u_short n2;
    u_short res;
} OMD_G3L;

typedef union { /* 0x10 */
    u_long U64[2];
    u_long128 U128;
} KeData16;

typedef struct { /* 0x50 */
    u_long XYOFFSET;
    u_long TEX0;
    sceVu0FMATRIX *VSMat;
    sceVu0FMATRIX *LVMat;
    u_long128 *Pk;
    OMD_H *Dat;
    float Dtt;
    int Mode;
    u_long SCISSOR;
    sceVu0FVECTOR *PstCol;
    sceVu0FVECTOR *BasCol;
    void *Vtxs;
    void *norm;
    int NClipflag;
    int NoNormal;
    u_char zTest;
} KeDttMdlMkGifPk;

void pppSnoMdlDttNormCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VSnoMdlDttNorm *pThis = (VSnoMdlDttNorm *)&pobj->val[ctbl->useVal[0]];

    *(u_long *)&pThis->DttX = 0;
    *(int *)&pThis->AccX = 0;
}

void pppSnoMdlDttNormCon2(pppPObject *pobj, pppCtrlTable *ctbl) {
    VSnoMdlDttNorm *pThis = (VSnoMdlDttNorm *)&pobj->val[ctbl->useVal[0]];

    *(u_long *)&pThis->DttX = 0;
    *(int *)&pThis->AccX = 0;
}

static inline void KeDttMdl_MkGifPk(KeDttMdlMkGifPk *Arg, int acalc) {
    int pktid;
    float *pc2;
    int i;
    short nItm;
    short *pVtx;
    OMD_G3L *pItm;
    KeData16 *pPk;
    char *pVtxs;
    char *pNorms;
    OMD_PRIM_H *pPrm;
    int md;
    struct {
        u_long hd[12];
        sceVu0IVECTOR pos[3];
        sceVu0IVECTOR nor[3];
        sceVu0FVECTOR col[3];
        int ofs[2];
        int z;
        int _Pad0[1];
        u_long hd2[2];
        sceVu0FVECTOR col2;
    } *pWrk;

    pktid = 1;

    pVtxs = Arg->Vtxs;
    pNorms = Arg->norm;
    pPrm = (OMD_PRIM_H *)((char *)Arg->Dat + Arg->Dat->pa);
    /* two statements: Pk + 14 stays a register of its own (not folded into the sq offset) */
    pPk = (KeData16 *)Arg->Pk;
    pPk += 14;
    pPk[0].U128 = 0x70000002;
    pPk[1].U64[0] = 0x1000000000008001;
    pPk[1].U64[1] = 0xE;
    if (Arg->zTest) {
        pPk[2].U64[0] = 0x21001;
        pPk[2].U64[1] = 0x47;
    } else {
        pPk[2].U64[0] = 0x71001;
        pPk[2].U64[1] = 0x47;
    }
    pppSendGsPacket(&pPk->U128);
    pWrk = op_spr_alloc(sizeof(*pWrk));
    if (pVtxs == 0) {
        pVtxs = (char *)Arg->Dat + Arg->Dat->va;
    }
    if (pNorms == 0) {
        pNorms = (char *)Arg->Dat + Arg->Dat->na;
    }
    pWrk->hd[0] = 0xC02DC00000008001;
    pWrk->hd[1] = 0x412412412EEE;
    pWrk->hd[2] = Arg->TEX0;
    pWrk->hd[3] = 6;
    if (acalc == 0) {
        pWrk->hd[6] = 0x44;
    } else {
        pWrk->hd[6] = acalc;
    }
    pWrk->hd[7] = 0x42;
    pWrk->hd[10] = 2 | (2 << 2) | ((u_long)((sceGsScissor *)&Arg->SCISSOR)->SCAX0 << 4) |
                   (((u_long)((sceGsScissor *)&Arg->SCISSOR)->SCAX1 - 1) << 14) |
                   ((u_long)((sceGsScissor *)&Arg->SCISSOR)->SCAY0 << 24) |
                   (((u_long)((sceGsScissor *)&Arg->SCISSOR)->SCAY1 - 1) << 34);
    pWrk->hd[11] = 8;
    {
        u_long t0 = (u_long)((sceGsXyoffset *)&Arg->XYOFFSET)->OFX - 8;
        pWrk->ofs[0] = t0;
    }
    {
        u_long t1 = ((Arg->XYOFFSET >> 32) & 0xFFFF) - 8;
        pWrk->ofs[1] = t1;
    }
    md = Arg->Mode;
    /* the original passes pWrk->hd2 (here) and pWrk->col2 (inner asm) as operands it never uses */
    __asm__ __volatile__(
        "vsub.xyzw $vf9, $vf0, $vf0\n"
        "lqc2 $vf31, 0x0(%1)\n"
        "vmove.x $vf1, $vf31\n"
        "lqc2 $vf31, 0x10(%1)\n"
        "vmove.y $vf1, $vf31\n"
        "lqc2 $vf2, 0x20(%1)\n"
        "lqc2 $vf31, 0x30(%1)\n"
        "vmove.zw $vf1, $vf31\n"
        "lqc2 $vf16, 0x0(%2)\n"
        "lqc2 $vf17, 0x10(%2)\n"
        "lqc2 $vf18, 0x20(%2)\n"
        "lqc2 $vf19, 0x30(%2)\n"
        "lqc2 $vf29, 0x20(%3)\n"
        "lqc2 $vf5, 0x0(%0)\n"
        "lqc2 $vf6, 0x10(%0)\n"
        "lqc2 $vf8, 0x30(%0)\n"
        "lqc2 $vf10, 0x50(%0)\n"
        "lui $2, 0x3a80\n"
        "qmtc2.ni $2, $vf31\n"
        "vaddx.z $vf4, $vf0, $vf31x\n"
        "lqc2 $vf12, 0xF0(%0)\n"
        "vitof4.xyzw $vf12, $vf12\n"
        "lui $2, 0x4580\n"
        "qmtc2.ni $2, $vf31\n"
        "vaddx.xy $vf4, $vf0, $vf31x\n"
        "lqc2 $vf23, 0x0(%4)\n"
        "lqc2 $vf24, 0x0(%5)\n"
        :
        : "r"(pWrk), "r"(Arg->VSMat), "r"(Arg->LVMat), "r"(Arg), "r"(Arg->PstCol), "r"(Arg->BasCol), "r"(pWrk->hd2)
        : "$2");
    do {
        pItm = (OMD_G3L *)(pPrm + 1);
        nItm = pPrm->pc;
        if (pPrm->ptype != 1) {
            break;
        }
        pPrm = (OMD_PRIM_H *)(pItm + nItm);
        for (; nItm > 0; nItm--) {
            /* two packet buffers in turn (14 qwords each) */
            pPk = (KeData16 *)Arg->Pk;
            if (pktid == 0) {
                pPk += 14;
            }
            pktid ^= 1;
            pVtx = (short *)(pVtxs + pItm->v0 * 6);
            pWrk->pos[0][0] = pVtx[0];
            pWrk->pos[0][1] = pVtx[1];
            pWrk->pos[0][2] = pVtx[2];
            /* set here (not at the asm) so loop.c hoists it after the constant 6, into $fp */
            pc2 = pWrk->col2;
            pVtx = (short *)(pVtxs + pItm->v1 * 6);
            pWrk->pos[1][0] = pVtx[0];
            pWrk->pos[1][1] = pVtx[1];
            pWrk->pos[1][2] = pVtx[2];
            pVtx = (short *)(pVtxs + pItm->v2 * 6);
            pWrk->pos[2][0] = pVtx[0];
            pWrk->pos[2][1] = pVtx[1];
            pWrk->pos[2][2] = pVtx[2];
            pWrk->col[0][0] = pItm->r0;
            pWrk->col[0][1] = pItm->g0;
            pWrk->col[0][2] = pItm->b0;
            pWrk->col[0][3] = pItm->a0;
            pWrk->col[1][0] = pItm->r1;
            pWrk->col[1][1] = pItm->g1;
            pWrk->col[1][2] = pItm->b1;
            pWrk->col[1][3] = pItm->a1;
            pWrk->col[2][0] = pItm->r2;
            pWrk->col[2][1] = pItm->g2;
            pWrk->col[2][2] = pItm->b2;
            pWrk->col[2][3] = pItm->a2;
            pVtx = (short *)(pNorms + pItm->n0 * 6);
            pWrk->nor[0][0] = pVtx[0];
            pWrk->nor[0][1] = pVtx[1];
            pWrk->nor[0][2] = pVtx[2];
            pVtx = (short *)(pNorms + pItm->n1 * 6);
            pWrk->nor[1][0] = pVtx[0];
            pWrk->nor[1][1] = pVtx[1];
            pWrk->nor[1][2] = pVtx[2];
            pVtx = (short *)(pNorms + pItm->n2 * 6);
            pWrk->nor[2][0] = pVtx[0];
            pWrk->nor[2][1] = pVtx[1];
            pWrk->nor[2][2] = pVtx[2];
            __asm__ __volatile__(
                ".set noreorder\n"
                "lqc2 $vf31, 0x60(%1)\n"
                "vitof0.xyzw $vf31, $vf31\n"
                "vaddax.xyzw $ACC, $vf19, $vf0x\n"
                "vmaddax.xyzw $ACC, $vf16, $vf31x\n"
                "vmadday.xyzw $ACC, $vf17, $vf31y\n"
                "vmaddz.xyz $vf22, $vf18, $vf31z\n"
                "vaddax.zw $ACC, $vf1, $vf0x\n"
                "vmulax.x $ACC, $vf1, $vf22x\n"
                "vmulay.y $ACC, $vf1, $vf22y\n"
                "vmaddz.xyzw $vf11, $vf2, $vf22z\n"
                "vdiv $Q, $vf0w, $vf11w\n"
                "vwaitq\n"
                "vaddq.w $vf22, $vf1, $Q\n"
                "vmulq.xyz $vf11, $vf11, $Q\n"
                "vnop\n"
                "ctc2.ni $0, $vi16\n"
                "vsub.w $vf0, $vf11, $vf0\n"
                "vsub.xy $vf0, $vf11, $vf0\n"
                "vsub.xy $vf0, $vf4, $vf11\n"
                "vnop\n"
                "vnop\n"
                "vnop\n"
                "vnop\n"
                "vnop\n"
                "cfc2.ni $2, $vi16\n"
                "andi %0, $2, 0xC0\n"
                "bnez %0, 10f\n"
                "vnop\n"
                "lqc2 $vf31, 0x70(%1)\n"
                "vitof0.xyzw $vf31, $vf31\n"
                "vaddax.xyzw $ACC, $vf19, $vf0x\n"
                "vmaddax.xyzw $ACC, $vf16, $vf31x\n"
                "vmadday.xyzw $ACC, $vf17, $vf31y\n"
                "vmaddz.xyz $vf21, $vf18, $vf31z\n"
                "vaddax.zw $ACC, $vf1, $vf0x\n"
                "vmulax.x $ACC, $vf1, $vf21x\n"
                "vmulay.y $ACC, $vf1, $vf21y\n"
                "vmaddz.xyzw $vf25, $vf2, $vf21z\n"
                "vdiv $Q, $vf0w, $vf25w\n"
                "vwaitq\n"
                "vaddq.w $vf21, $vf1, $Q\n"
                "vmulq.xyz $vf25, $vf25, $Q\n"
                "vnop\n"
                "ctc2.ni $0, $vi16\n"
                "vsub.w $vf0, $vf25, $vf0\n"
                "vsub.xy $vf0, $vf25, $vf0\n"
                "vsub.xy $vf0, $vf4, $vf25\n"
                "vnop\n"
                "vnop\n"
                "vnop\n"
                "vnop\n"
                "vnop\n"
                "cfc2.ni $2, $vi16\n"
                "andi %0, $2, 0xC0\n"
                "bnez %0, 10f\n"
                "vnop\n"
                "lqc2 $vf31, 0x80(%1)\n"
                "vitof0.xyzw $vf31, $vf31\n"
                "vaddax.xyzw $ACC, $vf19, $vf0x\n"
                "vmaddax.xyzw $ACC, $vf16, $vf31x\n"
                "vmadday.xyzw $ACC, $vf17, $vf31y\n"
                "vmaddz.xyz $vf20, $vf18, $vf31z\n"
                "vaddax.zw $ACC, $vf1, $vf0x\n"
                "vmulax.x $ACC, $vf1, $vf20x\n"
                "vmulay.y $ACC, $vf1, $vf20y\n"
                "vmaddz.xyzw $vf26, $vf2, $vf20z\n"
                "vdiv $Q, $vf0w, $vf26w\n"
                "vwaitq\n"
                "vaddq.w $vf20, $vf1, $Q\n"
                "vmulq.xyz $vf26, $vf26, $Q\n"
                "vnop\n"
                "ctc2.ni $0, $vi16\n"
                "vsub.w $vf0, $vf26, $vf0\n"
                "vsub.xy $vf0, $vf26, $vf0\n"
                "vsub.xy $vf0, $vf4, $vf26\n"
                "vnop\n"
                "vnop\n"
                "vnop\n"
                "vnop\n"
                "vnop\n"
                "cfc2.ni $2, $vi16\n"
                "andi %0, $2, 0xC0\n"
                "bnez %0, 10f\n"
                "vnop\n"
                "beqz %4, 11f\n"
                "nop\n"
                "vsub.xyz $vf28, $vf25, $vf11\n"
                "vsub.xyz $vf3, $vf26, $vf25\n"
                "vopmula.xyz $ACC, $vf28, $vf3\n"
                "vopmsub.xyz $vf0, $vf3, $vf28\n"
                "vnop\n"
                "vnop\n"
                "vnop\n"
                "vnop\n"
                "vnop\n"
                "cfc2.ni $2, $vi17\n"
                "andi $2, $2, 0x20\n"
                "bnez $2, 10f\n"
                "addi %0, $0, 0x80\n"
                "11:\n"
                "sqc2 $vf5, 0x10(%2)\n"
                "sqc2 $vf6, 0x20(%2)\n"
                "sqc2 $vf8, 0x30(%2)\n"
                "sqc2 $vf10, 0x40(%2)\n"
                "lqc2 $vf30, 0xC0(%1)\n"
                "vmul.xyzw $vf30, $vf30, $vf23\n"
                "vftoi0.xyzw $vf30, $vf30\n"
                "sqc2 $vf30, 0x60(%2)\n"
                "beqz %5, 1f\n"
                "nop\n"
                "vsub.xyzw $vf28, $vf22, $vf19\n"
                "j 2f\n"
                "nop\n"
                "1:\n"
                "lqc2 $vf28, 0x90(%1)\n"
                "vitof12.xyzw $vf28, $vf28\n"
                "vaddax.xyzw $ACC, $vf0, $vf0x\n"
                "vmaddax.xyzw $ACC, $vf16, $vf28x\n"
                "vmadday.xyzw $ACC, $vf17, $vf28y\n"
                "vmaddz.xyz $vf28, $vf18, $vf28z\n"
                "2:\n"
                "vmul.xyz $vf31, $vf28, $vf28\n"
                "vaddy.x $vf31, $vf31, $vf31y\n"
                "vaddz.x $vf31, $vf31, $vf31z\n"
                "vsqrt $Q, $vf31x\n"
                "vaddz.x $vf14, $vf0, $vf11z\n"
                "vftoi4.xyz $vf9, $vf11\n"
                "sqc2 $vf9, 0x70(%2)\n"
                "vaddw.z $vf15, $vf0, $vf0w\n"
                "vwaitq\n"
                "vaddq.z $vf12, $vf0, $Q\n"
                "vdiv $Q, $vf0w, $vf12z\n"
                "vwaitq\n"
                "vmulq.xyz $vf31, $vf28, $Q\n"
                "beqz %5, 3f\n"
                "nop\n"
                "vmove.xyzw $vf28, $vf31\n"
                "3:\n"
                "beqz %3, 4f\n"
                "nop\n"
                "vmulz.xy $vf28, $vf28, $vf31z\n"
                "4:\n"
                "vmulx.xyz $vf28, $vf28, $vf29x\n"
                "vadd.xyz $vf28, $vf28, $vf22\n"
                "vmulaw.zw $ACC, $vf1, $vf0w\n"
                "vmula.xy $ACC, $vf1, $vf28\n"
                "vmaddz.xyzw $vf28, $vf2, $vf28z\n"
                "vdiv $Q, $vf0w, $vf28w\n"
                "vwaitq\n"
                "vmulq.xyz $vf28, $vf28, $Q\n"
                "vaddw.z $vf15, $vf0, $vf0w\n"
                "vsub.xy $vf28, $vf28, $vf12\n"
                "vmulz.xy $vf15, $vf28, $vf4z\n"
                "sqc2 $vf15, 0x50(%2)\n"
                "lqc2 $vf30, 0xD0(%1)\n"
                "vmul.xyzw $vf30, $vf30, $vf23\n"
                "vftoi0.xyzw $vf30, $vf30\n"
                "sqc2 $vf30, 0x90(%2)\n"
                "beqz %5, 5f\n"
                "nop\n"
                "vsub.xyzw $vf28, $vf21, $vf19\n"
                "j 6f\n"
                "nop\n"
                "5:\n"
                "lqc2 $vf28, 0xA0(%1)\n"
                "vitof12.xyzw $vf28, $vf28\n"
                "vaddax.xyzw $ACC, $vf0, $vf0x\n"
                "vmaddax.xyzw $ACC, $vf16, $vf28x\n"
                "vmadday.xyzw $ACC, $vf17, $vf28y\n"
                "vmaddz.xyz $vf28, $vf18, $vf28z\n"
                "6:\n"
                "vmul.xyz $vf31, $vf28, $vf28\n"
                "vaddy.x $vf31, $vf31, $vf31y\n"
                "vaddz.x $vf31, $vf31, $vf31z\n"
                "vsqrt $Q, $vf31x\n"
                "vaddz.y $vf14, $vf0, $vf25z\n"
                "vftoi4.xyz $vf9, $vf25\n"
                "sqc2 $vf9, 0xA0(%2)\n"
                "vaddw.z $vf15, $vf0, $vf0w\n"
                "vwaitq\n"
                "vaddq.z $vf12, $vf0, $Q\n"
                "vdiv $Q, $vf0w, $vf12z\n"
                "vwaitq\n"
                "vmulq.xyz $vf31, $vf28, $Q\n"
                "beqz %5, 7f\n"
                "nop\n"
                "vmove.xyzw $vf28, $vf31\n"
                "7:\n"
                "beqz %3, 8f\n"
                "nop\n"
                "vmulz.xy $vf28, $vf28, $vf31z\n"
                "8:\n"
                "vmulx.xyz $vf28, $vf28, $vf29x\n"
                "vadd.xyz $vf28, $vf28, $vf21\n"
                "vmulaw.zw $ACC, $vf1, $vf0w\n"
                "vmula.xy $ACC, $vf1, $vf28\n"
                "vmaddz.xyzw $vf28, $vf2, $vf28z\n"
                "vdiv $Q, $vf0w, $vf28w\n"
                "vwaitq\n"
                "vmulq.xyz $vf28, $vf28, $Q\n"
                "vaddw.z $vf15, $vf0, $vf0w\n"
                "vsub.xy $vf28, $vf28, $vf12\n"
                "vmulz.xy $vf15, $vf28, $vf4z\n"
                "sqc2 $vf15, 0x80(%2)\n"
                "lqc2 $vf30, 0xE0(%1)\n"
                "vmul.xyzw $vf30, $vf30, $vf23\n"
                "vftoi0.xyzw $vf30, $vf30\n"
                "sqc2 $vf30, 0xC0(%2)\n"
                "beqz %5, 12f\n"
                "nop\n"
                "vsub.xyzw $vf28, $vf20, $vf19\n"
                "j 13f\n"
                "nop\n"
                "12:\n"
                "lqc2 $vf28, 0xB0(%1)\n"
                "vitof12.xyzw $vf28, $vf28\n"
                "vaddax.xyzw $ACC, $vf0, $vf0x\n"
                "vmaddax.xyzw $ACC, $vf16, $vf28x\n"
                "vmadday.xyzw $ACC, $vf17, $vf28y\n"
                "vmaddz.xyz $vf28, $vf18, $vf28z\n"
                "13:\n"
                "vmul.xyz $vf31, $vf28, $vf28\n"
                "vaddy.x $vf31, $vf31, $vf31y\n"
                "vaddz.x $vf31, $vf31, $vf31z\n"
                "vsqrt $Q, $vf31x\n"
                "vaddz.z $vf14, $vf0, $vf26z\n"
                "vftoi4.xyz $vf9, $vf26\n"
                "sqc2 $vf9, 0xD0(%2)\n"
                "vaddw.z $vf15, $vf0, $vf0w\n"
                "vwaitq\n"
                "vaddq.z $vf12, $vf0, $Q\n"
                "vdiv $Q, $vf0w, $vf12z\n"
                "vwaitq\n"
                "vmulq.xyz $vf31, $vf28, $Q\n"
                "beqz %5, 14f\n"
                "nop\n"
                "vmove.xyzw $vf28, $vf31\n"
                "14:\n"
                "beqz %3, 15f\n"
                "nop\n"
                "vmulz.xy $vf28, $vf28, $vf31z\n"
                "15:\n"
                "vmulx.xyz $vf28, $vf28, $vf29x\n"
                "vadd.xyz $vf28, $vf28, $vf20\n"
                "vmulaw.zw $ACC, $vf1, $vf0w\n"
                "vmula.xy $ACC, $vf1, $vf28\n"
                "vmaddz.xyzw $vf28, $vf2, $vf28z\n"
                "vdiv $Q, $vf0w, $vf28w\n"
                "vwaitq\n"
                "vmulq.xyz $vf28, $vf28, $Q\n"
                "vaddw.z $vf15, $vf0, $vf0w\n"
                "vsub.xy $vf28, $vf28, $vf12\n"
                "vmulz.xy $vf15, $vf28, $vf4z\n"
                "sqc2 $vf15, 0xB0(%2)\n"
                "vminix.y $vf31, $vf14, $vf14x\n"
                "vminiy.z $vf31, $vf14, $vf31y\n"
                "vftoi4.xyzw $vf31, $vf31\n"
                "sqc2 $vf31, 0xF0(%1)\n"
                "add %0, $0, $0\n"
                "10:\n"
                ".set reorder\n"
                : "=&r"(i)
                : "r"(pWrk), "r"(pPk), "r"(md), "r"(Arg->NClipflag), "r"(Arg->NoNormal), "r"(pc2)
                : "$2", "memory");
            /* i != 0: clipped (0xC0) or back-facing (0x80), the triangle is skipped */
            if (i == 0) {
                pPk->U64[0] = 0x7000000D;
                pPk->U64[1] = 0;
                pppSendGsPacket(&pPk->U128);
            } else {
                while (sceGsSyncPath(1, 0) & 0x10) {
                }
            }
            pItm++;
        }
    } while (pPrm->ptype != -1);
    while (sceGsSyncPath(1, 0) & 0x10) {
    }
    op_spr_free(sizeof(*pWrk));
}

void pppSnoMdlDttNormDraw(pppPObject *pobj, PSnoMdlDttNorm *p, pppCtrlTable *ctbl) {
    VSnoMdlDttNorm *pThis;
    VColor *pCol;
    sceVu0FVECTOR pstCol;
    VKeDMat *pMat;
    KeDttMdlMkGifPk a;
    float zback;

    pMat = (VKeDMat *)&pobj->val[ctbl->useVal[2]];
    pThis = (VSnoMdlDttNorm *)&pobj->val[ctbl->useVal[0]];
    if (!ppvUserStopPartF) {
        pThis->SpdX += pThis->AccX;
        pThis->DttX += pThis->SpdX;
        if (pobj->time == p->cdt.time) {
            pThis->DttX += p->DttX;
            pThis->SpdX += p->SpdX;
            pThis->AccX += p->AccX;
        }
    }
    if (p->Mdl == 0xFFFF) {
        return;
    }
    pCol = (VColor *)&pobj->val[ctbl->useVal[1]];
    if (pCol->release_col.a == 0) {
        return;
    }
    pstCol[0] = pCol->release_col.r;
    pstCol[1] = pCol->release_col.g;
    pstCol[2] = pCol->release_col.b;
    pstCol[3] = pCol->release_col.a;
    sceVu0ScaleVector(pstCol, pstCol, 1.0f / 128.0f);
    zback = ppvScreenMatrix[3][2];
    ppvScreenMatrix[3][2] = zback - p->zoffset * 10000000.0 / pMat->KeDMat[3][2];
    a.VSMat = &ppvScreenMatrix;
    a.LVMat = &pMat->KeDMat;
    a.PstCol = &pstCol;
    a.BasCol = &pstCol;
    a.Pk = op_spr_alloc(0x1C0);
    a.Dat = ppvEnv->lpModel[p->Mdl].omd;
    if (p->Normal) {
        a.Dtt = pThis->DttX * ppvParScl[0];
    } else {
        a.Dtt = pThis->DttX;
    }
    a.Mode = p->Mode;
    a.Vtxs = ppvPObj->lpvert;
    a.norm = ppvEnv->lpModel[p->Mdl].norm;
    if (a.Dat->flag & 2) {
        a.NClipflag = 0;
    } else {
        a.NClipflag = 1;
    }
    a.NoNormal = p->Normal;
    if (p->Page == 0) {
        a.TEX0 = (u_long)(ppvDrawFrameBase << 5) | ((u_long)(ppvDrawWidth / 64) << 14) | ((u_long)10 << 26) |
                 ((u_long)10 << 30);
        a.XYOFFSET = (u_long)((2048 - ppvDrawWidth / 2) << 4) | ((u_long)((2048 - ppvDrawHeight / 2) << 4) << 32);
    } else {
        a.TEX0 = (u_long)(ppvDispFrameBase << 5) | ((u_long)(ppvDispWidth / 64) << 14) | ((u_long)10 << 26) |
                 ((u_long)10 << 30);
        a.XYOFFSET = (u_long)((2048 - ppvDispWidth / 2) << 4) | ((u_long)((2048 - ppvDispHeight / 2) << 4) << 32);
    }
    a.zTest = p->ztest;
    a.SCISSOR = ppvGsScissor;
    KeDttMdl_MkGifPk(&a, p->m_acalc);
    ppvScreenMatrix[3][2] = zback;
    op_spr_free(0x1C0);
}
