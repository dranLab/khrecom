#include "pppKeShpDtt.h"
#include "KeDttSpr.h"
#include "pppKeDMat.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"
#include "include_asm.h"

void pppKeShpDtt(pppPObject *pobj, PKeShpDtt *dat, pppCtrlTable *ctbl) {}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/libppp/keProg/pppKeShpDtt", pppKeShpDttDraw);

/* Close but not matching (~70 lines differ in the packet-setup part): the original stores the Dtt zeros
 * from a float register, loads SCISSOR after the Dtt setup and orders the TEX0/XYOFFSET arithmetic
 * differently. */
#if 0
void pppKeShpDttDraw(pppPObject *pobj, PKeShpDtt *dat, pppCtrlTable *ctbl) {
    KeaDttSpr_MkGifPk a;
    VKeShpDtt *pThis;
    VColor *pCol;
    VKeDMat *pKeDMat;
    VPoint *pPt;
    int w;
    int h;
    int fb;

    pThis = (VKeShpDtt *)&pobj->val[ctbl->useVal[IKeShpDtt_This]];
    pKeDMat = (VKeDMat *)&pobj->val[ctbl->useVal[IKeShpDtt_KeDMat]];
    if (!ppvUserStopPartF && !ppvMng->bFlagStop && !ppvMng->bBoolSlow) {
        pThis->SpdX += pThis->AccX;
        pThis->DttX += pThis->SpdX;
        if (pobj->time == dat->cdt.time) {
            pThis->DttX += dat->DttX;
            pThis->SpdX += dat->SpdX;
            pThis->AccX += dat->AccX;
        }
    }
    pCol = (VColor *)&pobj->val[ctbl->useVal[IKeShpDtt_Color]];
    a.Col[0] = pCol->release_col.r;
    a.Col[1] = pCol->release_col.g;
    a.Col[2] = pCol->release_col.b;
    a.Col[3] = pCol->release_col.a;
    if (dat->FEdg & 2) {
        pPt = (VPoint *)&pobj->val[ctbl->useVal[IKeShpDtt_Point]];
        a.Pos[0] = pPt->pos.x;
        a.Pos[1] = pPt->pos.y;
        a.Pos[2] = 512.0f;
        a.Pos[3] = 1.0f;
        a.LSMat = &ppvScreenMatrix;
    } else {
        *(u_long128 *)a.Pos = *(u_long128 *)pKeDMat->KeDMat[3];
        a.LSMat = &ppvScreenMatrix;
    }
    a.Sz[0] = dat->SzX * ppvParScl[0];
    a.Sz[1] = dat->SzY * ppvParScl[1];
    a.VSMat = &ppvScreenMatrix;
    a.Pk = (u_long *)(ppvCurPrimp + 1);
    if (dat->Page == 0) {
        h = ppvDrawHeight;
        w = ppvDrawWidth;
        fb = ppvDrawFrameBase;
    } else {
        h = ppvDispHeight;
        w = ppvDispWidth;
        fb = ppvDispFrameBase;
    }
    a.XYOFFSET = (u_long)((2048 - w / 2) << 4) | ((u_long)((2048 - h / 2) << 4) << 32);
    a.TEX0 = (u_long)(fb << 5) | ((u_long)(w / 64) << 14) | ((u_long)10 << 26) | ((u_long)10 << 30);
    a.SCISSOR = ppvGsScissor;
    a.Dtt[0] = 0.0f;
    a.Dtt[1] = -pThis->DttX;
    a.Dtt[2] = 0.0f;
    *(float *)&a._Pad0[0] = 0.0f;
    sceVu0ApplyMatrix(a.Dtt, pKeDMat->KeDMat, a.Dtt);
    a._Pad0[0] = dat->FEdg;
    KeDttSpr_MkGifPk_Pre(&a);
    if (KeDttSpr_MkGifPk(&a)) {
        if (dat->FEdg & 2) {
            op_ot_add_before(ppvCurPrimp, ((u_int)a.Pk - (u_int)ppvCurPrimp) >> 4, 4, 0);
        } else {
            op_ot_add(ppvCurPrimp, ((u_int)a.Pk - (u_int)ppvCurPrimp) >> 4, a.Z / 0x3000, 0);
        }
        ppvCurPrimpLen += (int)a.Pk - (int)ppvCurPrimp;
        ppvCurPrimp = (u_long128 *)a.Pk;
    }
}

#endif

void pppKeShpDttCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeShpDtt *pThis = (VKeShpDtt *)&pobj->val[ctbl->useVal[IKeShpDtt_This]];

    *(u_long *)&pThis->DttX = 0;
    *(int *)&pThis->AccX = 0;
}
