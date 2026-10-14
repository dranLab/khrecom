#include "pppKeMdlDtt.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"
#include "System/Graphic/Effect/libppp/draw/md.h"
#include "pppKeDMat.h"

typedef struct { /* 0x10 */
    char flag;
    char ptype;
    short pc;
    u_char tno;
    u_char timposx;
    u_char res[2];
    u_word tex0l;
    u_word tex0h;
} OMD_PRIM_H;

typedef struct { /* 0x14 */
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
} OMD_G3;

typedef union { /* 0x10 */
    u_long U64[2];
    u_long128 U128;
} KeData16;

typedef struct { /* 0x48 */
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
    sceVu0FVECTOR *StBuf;
    int StMode;
    int StFlg;
    void *Vtxs;
} KeDttMdlMkGifPk;

static inline void KeDttMdl_MkGifPk(KeDttMdlMkGifPk *Arg) {
    int i;
    short nItm;
    short *pVtx;
    OMD_G3 *pItm;
    KeData16 *pPk;
    char *pVtxs;
    OMD_PRIM_H *pPrm;
    int md;
    int stMode;
    int stFlg;
    struct {
        u_long hd[12];
        sceVu0IVECTOR pos[3];
        sceVu0FVECTOR col[3];
        int ofs[2];
        int z;
        int _Pad0[1];
        sceVu0FVECTOR *st;
    } *pWrk;

    stFlg = Arg->StFlg;
    stMode = Arg->StMode;
    pVtxs = Arg->Vtxs;
    pPk = (KeData16 *)Arg->Pk;
    pPrm = (OMD_PRIM_H *)((char *)Arg->Dat + Arg->Dat->pa);
    pWrk = op_spr_alloc(sizeof(*pWrk));
    if (pVtxs == 0) {
        pVtxs = (char *)Arg->Dat + Arg->Dat->va;
    }
    pWrk->hd[0] = 0xC02DC00000008001;
    pWrk->hd[1] = 0x412412412EEE;
    pWrk->hd[2] = Arg->TEX0;
    pWrk->hd[3] = 6;
    pWrk->hd[6] = 0x44;
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
    pWrk->st = Arg->StBuf;
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
        "vmove.x $vf13, $vf1\n"
        "vmove.y $vf13, $vf1\n"
        "vmulx.xy $vf13, $vf13, $vf29x\n"
        "lqc2 $vf5, 0x0(%0)\n"
        "lqc2 $vf6, 0x10(%0)\n"
        "lqc2 $vf8, 0x30(%0)\n"
        "lqc2 $vf10, 0x50(%0)\n"
        "lui $2, 0x3a80\n"
        "qmtc2.ni $2, $vf31\n"
        "vaddx.z $vf4, $vf0, $vf31x\n"
        "lqc2 $vf12, 0xC0(%0)\n"
        "vitof4.xyzw $vf12, $vf12\n"
        "lui $2, 0x4580\n"
        "qmtc2.ni $2, $vf31\n"
        "vaddx.xy $vf4, $vf0, $vf31x\n"
        "lqc2 $vf23, 0x0(%4)\n"
        "lqc2 $vf24, 0x0(%5)\n"
        :
        : "r"(pWrk), "r"(Arg->VSMat), "r"(Arg->LVMat), "r"(Arg), "r"(Arg->PstCol), "r"(Arg->BasCol)
        : "$2");
    /* OMD_G3 primitive groups (ptype 0) until the end marker (ptype -1) */
    do {
        /* pc read before the type test: it goes into the branch delay slot */
        pItm = (OMD_G3 *)(pPrm + 1);
        nItm = pPrm->pc;
        if (pPrm->ptype != 0) {
            break;
        }
        pPrm = (OMD_PRIM_H *)(pItm + nItm);
        for (; nItm > 0; nItm--) {
            pVtx = (short *)(pVtxs + pItm->v0 * 6);
            pWrk->pos[0][0] = pVtx[0];
            pWrk->pos[0][1] = pVtx[1];
            pWrk->pos[0][2] = pVtx[2];
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
            /* noreorder: the original fills the branch delay slots itself (vnop / nop) */
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
                "sqc2 $vf5, 0x10(%2)\n"
                "sqc2 $vf6, 0x20(%2)\n"
                "sqc2 $vf8, 0x30(%2)\n"
                "sqc2 $vf10, 0x40(%2)\n"
                "lqc2 $vf3, 0x90(%1)\n"
                "vmul.xyzw $vf3, $vf3, $vf23\n"
                "vftoi0.xyzw $vf3, $vf3\n"
                "sqc2 $vf3, 0x60(%2)\n"
                "vsub.xyz $vf28, $vf22, $vf19\n"
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
                "beqz %6, 1f\n"
                "nop\n"
                "vmulz.xy $vf31, $vf31, $vf31z\n"
                "1:\n"
                "vmul.xy $vf31, $vf31, $vf13\n"
                "vmulw.xyz $vf31, $vf31, $vf22w\n"
                "vadd.xy $vf31, $vf31, $vf11\n"
                "vsub.xy $vf31, $vf31, $vf12\n"
                "vmulz.xy $vf15, $vf31, $vf4z\n"
                "beqz %4, 3f\n"
                "nop\n"
                "beqz %5, 2f\n"
                "nop\n"
                "lqc2 $vf15, 0x0(%3)\n"
                "j 3f\n"
                "nop\n"
                "2:\n"
                "sqc2 $vf15, 0x0(%3)\n"
                "3:\n"
                "vaddw.z $vf15, $vf0, $vf22w\n"
                "vmulz.xy $vf15, $vf15, $vf15z\n"
                "sqc2 $vf15, 0x50(%2)\n"
                "lqc2 $vf30, 0xA0(%1)\n"
                "vmul.xyzw $vf30, $vf30, $vf23\n"
                "vftoi0.xyzw $vf30, $vf30\n"
                "sqc2 $vf30, 0x90(%2)\n"
                "vsub.xyz $vf28, $vf21, $vf19\n"
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
                "beqz %6, 4f\n"
                "nop\n"
                "vmulz.xy $vf31, $vf31, $vf31z\n"
                "4:\n"
                "vmul.xy $vf31, $vf31, $vf13\n"
                "vmulw.xyz $vf31, $vf31, $vf21w\n"
                "vadd.xy $vf31, $vf31, $vf25\n"
                "vsub.xy $vf31, $vf31, $vf12\n"
                "vmulz.xy $vf15, $vf31, $vf4z\n"
                "beqz %4, 6f\n"
                "nop\n"
                "beqz %5, 5f\n"
                "nop\n"
                "lqc2 $vf15, 0x10(%3)\n"
                "j 6f\n"
                "nop\n"
                "5:\n"
                "sqc2 $vf15, 0x10(%3)\n"
                "6:\n"
                "vaddw.z $vf15, $vf0, $vf21w\n"
                "vmulz.xy $vf15, $vf15, $vf15z\n"
                "sqc2 $vf15, 0x80(%2)\n"
                "lqc2 $vf30, 0xB0(%1)\n"
                "vmul.xyzw $vf30, $vf30, $vf23\n"
                "vftoi0.xyzw $vf30, $vf30\n"
                "sqc2 $vf30, 0xC0(%2)\n"
                "vsub.xyz $vf28, $vf20, $vf19\n"
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
                "beqz %6, 7f\n"
                "nop\n"
                "vmulz.xy $vf31, $vf31, $vf31z\n"
                "7:\n"
                "vmul.xy $vf31, $vf31, $vf13\n"
                "vmulw.xyz $vf31, $vf31, $vf20w\n"
                "vadd.xy $vf31, $vf31, $vf26\n"
                "vsub.xy $vf31, $vf31, $vf12\n"
                "vmulz.xy $vf15, $vf31, $vf4z\n"
                "beqz %4, 9f\n"
                "nop\n"
                "beqz %5, 8f\n"
                "nop\n"
                "lqc2 $vf15, 0x20(%3)\n"
                "j 9f\n"
                "nop\n"
                "8:\n"
                "sqc2 $vf15, 0x20(%3)\n"
                "9:\n"
                "vaddw.z $vf15, $vf0, $vf20w\n"
                "vmulz.xy $vf15, $vf15, $vf15z\n"
                "sqc2 $vf15, 0xB0(%2)\n"
                "vminix.y $vf31, $vf14, $vf14x\n"
                "vminiy.z $vf31, $vf14, $vf31y\n"
                "vftoi4.xyzw $vf31, $vf31\n"
                "sqc2 $vf31, 0xC0(%1)\n"
                "add %0, $0, $0\n"
                "10:\n"
                ".set reorder\n"
                : "=&r"(i)
                : "r"(pWrk), "r"(pPk), "r"(pWrk->st), "r"(stFlg), "r"(stMode), "r"(md)
                : "$2", "memory");
            /* i != 0: a vertex failed the clip test (0xC0 clip flags), the triangle is skipped */
            if (i == 0) {
                op_ot_add((u_long128 *)pPk, 14, pWrk->z / 0x3000, 0);
                pPk += 14;
            }
            pItm++;
            pWrk->st += 3;
        }
    } while (pPrm->ptype != -1);
    Arg->Pk = (u_long128 *)pPk;
    Arg->StBuf = pWrk->st;
    op_spr_free(sizeof(*pWrk));
}

/* With Page >= 4, VKeMdlDtt::AccX holds a pointer to the caller's sceVu0FVECTOR * (st buffer) instead of a
 * float. */
void pppKeMdlDttDraw(pppPObject *pobj, PKeMdlDtt *dat, pppCtrlTable *ctbl) {
    VKeMdlDtt *pThis;
    VColor *pCol;
    VKeDMat *pMat;
    KeDttMdlMkGifPk a;
    sceVu0FVECTOR pstCol;

    pMat = (VKeDMat *)&pobj->val[ctbl->useVal[IKeMdlDtt_KeDMat]];
    pThis = (VKeMdlDtt *)&pobj->val[ctbl->useVal[IKeMdlDtt_This]];
    if (!ppvUserStopPartF && !ppvMng->bFlagStop && !ppvMng->bBoolSlow) {
        pThis->SpdX += pThis->AccX;
        pThis->DttX += pThis->SpdX;
        if (pobj->time == dat->cdt.time) {
            pThis->DttX += dat->DttX;
            pThis->SpdX += dat->SpdX;
            if (dat->Page < 4) {
                pThis->AccX += dat->AccX;
            }
        }
    }
    if (dat->Mdl == 0xFFFF) {
        return;
    }
    pCol = (VColor *)&pobj->val[ctbl->useVal[IKeMdlDtt_Color]];
    if (pCol->release_col.r == 0) {
        return;
    }
    pstCol[0] = pCol->release_col.r;
    pstCol[1] = pCol->release_col.g;
    pstCol[2] = pCol->release_col.b;
    pstCol[3] = pCol->release_col.a;
    sceVu0ScaleVector(pstCol, pstCol, 1.0f / 255.0f);
    a.VSMat = &ppvScreenMatrix;
    a.PstCol = &pstCol;
    a.LVMat = &pMat->KeDMat;
    a.BasCol = &pstCol;
    a.Pk = ppvCurPrimp;
    a.Dat = ppvEnv->lpModel[dat->Mdl].omd;
    a.Mode = dat->Mode;
    a.Dtt = pThis->DttX * ppvParScl[0];
    a.Vtxs = ppvPObj->lpvert;
    if (dat->Page >= 4 && *(sceVu0FVECTOR ***)&pThis->AccX != 0) {
        sceVu0FVECTOR **pSt = *(sceVu0FVECTOR ***)&pThis->AccX;

        a.StFlg = 1;
        a.StBuf = *pSt;
        a.StMode = dat->Page & 8;
    } else {
        a.StFlg = 0;
    }
    /* the whole expressions in both branches: gcc cross-jumps their common tails */
    if (dat->Page == 0) {
        a.TEX0 = (u_long)(ppvDrawFrameBase << 5) | ((u_long)(ppvDrawWidth / 64) << 14) | ((u_long)10 << 26) |
                 ((u_long)10 << 30);
        a.XYOFFSET = (u_long)((2048 - ppvDrawWidth / 2) << 4) | ((u_long)((2048 - ppvDrawHeight / 2) << 4) << 32);
    } else {
        a.TEX0 = (u_long)(ppvDispFrameBase << 5) | ((u_long)(ppvDispWidth / 64) << 14) | ((u_long)10 << 26) |
                 ((u_long)10 << 30);
        a.XYOFFSET = (u_long)((2048 - ppvDispWidth / 2) << 4) | ((u_long)((2048 - ppvDispHeight / 2) << 4) << 32);
    }
    a.SCISSOR = ppvGsScissor;
    KeDttMdl_MkGifPk(&a);
    ppvCurPrimpLen += (int)a.Pk - (int)ppvCurPrimp;
    ppvCurPrimp = a.Pk;
    if (dat->Page >= 4 && *(sceVu0FVECTOR ***)&pThis->AccX != 0) {
        **(sceVu0FVECTOR ***)&pThis->AccX = a.StBuf;
    }
}

void pppKeMdlDttCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeMdlDtt *pThis = (VKeMdlDtt *)&pobj->val[ctbl->useVal[IKeMdlDtt_This]];

    *(u_long *)&pThis->DttX = 0;
    *(int *)&pThis->AccX = 0;
}

void pppKeMdlDttCon2(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeMdlDtt *pThis = (VKeMdlDtt *)&pobj->val[ctbl->useVal[IKeMdlDtt_This]];

    *(u_long *)&pThis->DttX = 0;
    *(int *)&pThis->AccX = 0;
}
