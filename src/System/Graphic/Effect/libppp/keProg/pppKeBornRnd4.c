#include "pppKeBornRnd4.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"
#include "System/Graphic/Effect/libppp/eiProg/pppMove.h"

void pppKeBornRnd4_Born(pppPObject *pObj, PKeBornRnd4 *pDat, pppSVECTOR *pVert) {
    sceVu0FVECTOR fv0;
    pppPObject *pNewObj;
    VPoint *pPt;
    VMove *pMv;
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
        pMv = (VMove *)&pNewObj->val[pDat->Mv.valoff];
        pDr = (VKeDrct *)&pNewObj->val[pDat->Dr.valoff];
        sceVu0ApplyMatrix(fv0, ppvPObj->cmat, fv0);
        KeVu0CopyVecXYZ((float *)pPt, fv0);
        sceVu0SubVector(fv0, fv0, ppvPObj->cmat[3]);
        sceVu0Normalize(fv0, fv0);
        KeVu0CopyVecXYZ((float *)pDr, fv0);
        sceVu0ScaleVector(fv0, fv0, pDat->Spd);
        KeVu0CopyVecXYZ((float *)pMv, fv0);
    }
}

void pppKeBornRnd4(pppPObject *pobj, PKeBornRnd4 *dat, pppCtrlTable *ctbl) {
    int r;
    sceVu0FVECTOR fv0;
    VKeBornRnd4 *_t;
    pppVsfSt *pVsf;
    pppSVECTOR *pTblVert;
    pppSVECTOR *pVert;
    pppPObject *pObj;
    VPoint *pPt;
    VMove *pMv;
    VKeDrct *pDr;
    int md;
    int i;
    int j;
    pppPDataVal *pdv;

    if (ppvEmptyLoop != 0) {
        return;
    }
    if (ppvUserStopPartF != 0) {
        return;
    }
    if (!(ppvMng->bFlagStop || ppvMng->bBoolSlow) && dat->Pstg != 0xFF && !((KeVu0RndI() >> 8) & dat->Pstg)) {
        if (dat->Pos.pdtn == 0xFFFFFFFF) {
            return;
        }
        if (dat->Mv.pdtn == 0xFFFFFFFF) {
            return;
        }
        if (dat->Dr.pdtn == 0xFFFFFFFF) {
            return;
        }
        KeBornRnd2(pobj, dat, pppKeBornRnd4_Born, (u_short *)&pobj->val[ctbl->useVal[IKeBornRnd4_This]],
                   dat->IdxVsf, dat->Mode, dat->Num);
    }

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    r = KeVu0RndI();
    if (dat->Pstg == 0xFF) {
        return;
    }
    if ((r >> 8) & dat->Pstg) {
        return;
    }
    if (dat->Pos.pdtn == 0xFFFFFFFF) {
        return;
    }
    if (dat->Mv.pdtn == 0xFFFFFFFF) {
        return;
    }
    if (dat->Dr.pdtn == 0xFFFFFFFF) {
        return;
    }
    _t = (VKeBornRnd4 *)&pobj->val[ctbl->useVal[IKeBornRnd4_This]];
    pVsf = &ppvEnv->lpVsf[dat->IdxVsf];
    pTblVert = ppvPObj->lpvert;
    if (pTblVert == NULL) {
        pTblVert = ppvEnv->lpModel[pVsf->modelno].vert;
    }
    md = dat->Mode;
    if (md) {
        j = r % pVsf->indexn;
    } else {
        j = _t->NextIdx;
    }
    for (i = dat->Num; i > 0; i--) {
        pdv = &ppvMng->m_pdtval[dat->Pos.pdtn];
        if (pdv == NULL) {
            pObj = NULL;
        } else {
            pObj = pppCreatePObject(ppvMng, pdv);
            if (pObj == NULL) {
                goto next;
            }
            pObj->ppobj = pobj;
        }
        if (pObj != NULL) {
            pVert = &pTblVert[pVsf->index[j]];
            pPt = (VPoint *)&pObj->val[dat->Pos.valoff];
            pMv = (VMove *)&pObj->val[dat->Mv.valoff];
            pDr = (VKeDrct *)&pObj->val[dat->Dr.valoff];
            fv0[0] = pVert->x;
            fv0[1] = pVert->y;
            fv0[2] = pVert->z;
            fv0[3] = 1.0f;
            sceVu0ApplyMatrix(fv0, ppvPObj->cmat, fv0);
            KeVu0CopyVecXYZ((float *)pPt, fv0);
            sceVu0SubVector(fv0, fv0, ppvPObj->cmat[3]);
            sceVu0Normalize(fv0, fv0);
            KeVu0CopyVecXYZ((float *)pDr, fv0);
            sceVu0ScaleVector(fv0, fv0, dat->Spd);
            KeVu0CopyVecXYZ((float *)pMv, fv0);
        }
    next:
        if (md) {
            j = KeVu0RndI() % pVsf->indexn;
        } else {
            j++;
            if (j >= pVsf->indexn) {
                j = 0;
            }
        }
    }
    _t->NextIdx = j;
}

void pppKeBornRnd4Con(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeBornRnd4 *_t = (VKeBornRnd4 *)&pobj->val[ctbl->useVal[IKeBornRnd4_This]];

    _t->NextIdx = 0;
}
