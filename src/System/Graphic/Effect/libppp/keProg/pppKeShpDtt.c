#include "pppKeShpDtt.h"
#include "KeDttSpr.h"
#include "pppKeDMat.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"

void pppKeShpDtt(pppPObject *pobj, PKeShpDtt *dat, pppCtrlTable *ctbl) {}

void pppKeShpDttDraw(pppPObject *pobj, PKeShpDtt *dat, pppCtrlTable *ctbl) {
    KeaDttSpr_MkGifPk a;
    VKeShpDtt *pThis;
    VColor *pCol;
    VKeDMat *pKeDMat;
    VPoint *pPt;
    int w;
    int h;
    int fb;

    /* pKeDMat before pThis: the useVal loads then come out in index order */
    pKeDMat = (VKeDMat *)&pobj->val[ctbl->useVal[IKeShpDtt_KeDMat]];
    pThis = (VKeShpDtt *)&pobj->val[ctbl->useVal[IKeShpDtt_This]];
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
    a.VSMat = &ppvScreenMatrix;
    a.Sz[0] = dat->SzX * ppvParScl[0];
    a.Sz[1] = dat->SzY * ppvParScl[1];
    a.Pk = (u_long *)(ppvCurPrimp + 1);
    /* TEX0 and XYOFFSET computed in each arm (the tail is cross-jumped); TEX0 first */
    if (dat->Page == 0) {
        a.TEX0 = (u_long)(ppvDrawFrameBase << 5) | ((u_long)(ppvDrawWidth / 64) << 14) | ((u_long)10 << 26) | ((u_long)10 << 30);
        a.XYOFFSET = (u_long)((2048 - ppvDrawWidth / 2) << 4) | ((u_long)((2048 - ppvDrawHeight / 2) << 4) << 32);
    } else {
        a.TEX0 = (u_long)(ppvDispFrameBase << 5) | ((u_long)(ppvDispWidth / 64) << 14) | ((u_long)10 << 26) | ((u_long)10 << 30);
        a.XYOFFSET = (u_long)((2048 - ppvDispWidth / 2) << 4) | ((u_long)((2048 - ppvDispHeight / 2) << 4) << 32);
    }
    a.SCISSOR = ppvGsScissor;
    {
        float z = 0.0f; /* the zeros come from a float register; store order found by permutation */
        *(float *)&a._Pad0[0] = z;
        a.Dtt[2] = z;
        a.Dtt[0] = z;
        a.Dtt[1] = -pThis->DttX;
    }
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


void pppKeShpDttCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeShpDtt *pThis = (VKeShpDtt *)&pobj->val[ctbl->useVal[IKeShpDtt_This]];

    *(u_long *)&pThis->DttX = 0;
    *(int *)&pThis->AccX = 0;
}
