#include "pppKeBornRnd2.h"
#include "System/Graphic/Effect/libppp/eiProg/pppMove.h"

void pppKeBornRnd2_Born(pppPObject *pObj, PKeBornRnd2 *pDat, pppSVECTOR *pVert) {
    sceVu0FVECTOR fv0;
    pppPObject *pNewObj;
    VPoint *pPt;
    VMove *pMv;
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
        pMv = (VMove *)&pNewObj->val[pDat->Mv.valoff];
        sceVu0ApplyMatrix(fv0, ppvPObj->cmat, fv0);
        KeVu0CopyVecXYZ((float *)pPt, fv0);
        sceVu0SubVector(fv0, fv0, ppvPObj->cmat[3]);
        sceVu0Normalize(fv0, fv0);
        sceVu0ScaleVector(fv0, fv0, pDat->Spd);
        KeVu0CopyVecXYZ((float *)pMv, fv0);
    }
}

void pppKeBornRnd2(pppPObject *pobj, PKeBornRnd2 *dat, pppCtrlTable *ctbl) {
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
    if (dat->Mv.pdtn == 0xFFFFFFFF) {
        return;
    }
    KeBornRnd2(pobj, dat, pppKeBornRnd2_Born, (u_short *)&pobj->val[ctbl->useVal[0]], dat->IdxVsf, dat->Mode,
               dat->Num);
}

void pppKeBornRnd2Con(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeBornRnd2 *_t = (VKeBornRnd2 *)&pobj->val[ctbl->useVal[0]];

    _t->NextIdx = 0;
}
