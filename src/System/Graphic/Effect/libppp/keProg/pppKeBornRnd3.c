#include "pppKeBornRnd3.h"
#include "System/Graphic/Effect/libppp/eiProg/pppMove.h"

void pppKeBornRnd3_Born(pppPObject *pObj, PKeBornRnd3 *pDat, pppSVECTOR *pVert) {
    sceVu0FVECTOR fv0;
    pppPObject *pNewObj;
    VPoint *pPt;
    VKeDrct *pDr;
    pppPDataVal *pdv;

    pdv = &ppvMng->m_pdtval[pDat->Pos.pdtn];
    if (pdv == NULL) {
        pNewObj = NULL;
    } else {
        pNewObj = pppCreatePObject(ppvMng, pdv);
        if (pNewObj == NULL) {
            return;
        }
        pNewObj->ppobj = pObj;
    }
    if (pNewObj != NULL) {
        fv0[0] = pVert->x;
        fv0[1] = pVert->y;
        fv0[2] = pVert->z;
        fv0[3] = 1.0f;
        pPt = (VPoint *)&pNewObj->val[pDat->Pos.valoff];
        pDr = (VKeDrct *)&pNewObj->val[pDat->Dr.valoff];
        sceVu0ApplyMatrix(fv0, ppvPObj->cmat, fv0);
        KeVu0CopyVecXYZ((float *)pPt, fv0);
        sceVu0SubVector(fv0, fv0, ppvPObj->cmat[3]);
        sceVu0Normalize(fv0, fv0);
        KeVu0CopyVecXYZ((float *)pDr, fv0);
    }
}

void pppKeBornRnd3(pppPObject *pobj, PKeBornRnd3 *dat, pppCtrlTable *ctbl) {
    if (ppvEmptyLoop != 0) {
        return;
    }
    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    if (dat->Pstg == 0xFF) {
        return;
    }
    if ((KeVu0RndI() >> 8) & dat->Pstg) {
        return;
    }
    if (dat->Pos.pdtn == 0xFFFFFFFF) {
        return;
    }
    if (dat->Dr.pdtn == 0xFFFFFFFF) {
        return;
    }
    KeBornRnd2(pobj, dat, pppKeBornRnd3_Born, (u_short *)&pobj->val[ctbl->useVal[0]], dat->IdxVsf, dat->Mode,
               dat->Num);
}

void pppKeBornRnd3Con(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeBornRnd3 *_t = (VKeBornRnd3 *)&pobj->val[ctbl->useVal[0]];

    _t->NextIdx = 0;
}
