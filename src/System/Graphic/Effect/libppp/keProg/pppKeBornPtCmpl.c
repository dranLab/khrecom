#include "pppKeBornPtCmpl.h"

void KeBornPtCmlp(pppPObject *pObj, PKeBornPtCmpl *pDat, pppCtrlTable *pTbl) {
    sceVu0FVECTOR fv0;
    sceVu0FVECTOR fv1;
    sceVu0FVECTOR fv2;
    VKeBornPtCmpl *_t;
    pppPObject *pNewObj;
    VPoint *pNewPt;
    VPoint *pPt;
    float d;
    float c;

    {
        pppPDataVal *pdv = &ppvMng->m_pdtval[pDat->Pos.pdtn];

        if (pdv == NULL) {
            pNewObj = NULL;
        } else {
            pNewObj = pppCreatePObject(ppvMng, pdv);
            if (pNewObj != NULL) {
                pNewObj->ppobj = pObj;
            }
        }
    }
    if (pNewObj != NULL) {
        pNewPt = (VPoint *)&pNewObj->val[pDat->Pos.valoff];
        KeVu0CopyVecXYZ((float *)pNewPt, (float *)&pObj->val[pTbl->useVal[IKeBornPtCmpl_POINT]]);
        _t = (VKeBornPtCmpl *)&pObj->val[pTbl->useVal[IKeBornPtCmpl_This]];
        if (_t->PrePt == NULL) {
            _t->Pstg = 1.0f / (pDat->Num + 1);
        } else {
            sceVu0CopyVector(fv0, (float *)_t->PrePt);
            sceVu0SubVector(fv1, (float *)pNewPt, fv0);
            d = _t->Pstg;
            for (c = pDat->Num; c > 0.0f; c -= 1.0f) {
                pppPDataVal *pdv = &ppvMng->m_pdtval[pDat->Pos.pdtn];

                if (pdv == NULL) {
                    pNewObj = NULL;
                } else {
                    pNewObj = pppCreatePObject(ppvMng, pdv);
                    if (pNewObj != NULL) {
                        pNewObj->ppobj = pObj;
                    }
                }
                if (pNewObj != NULL) {
                    pPt = (VPoint *)&pNewObj->val[pDat->Pos.valoff];
                    sceVu0ScaleVector(fv2, fv1, d * c);
                    KeVu0AddVecXYZ((float *)pPt, fv0, fv2);
                }
            }
        }
    }
    _t->PrePt = pNewPt;
}

void pppKeBornPtCmpl(pppPObject *pobj, PKeBornPtCmpl *dat, pppCtrlTable *ctbl) {
    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    if (dat->Pos.pdtn == 0xFFFFFFFF) {
        return;
    }
    if (dat->Col.pdtn == 0xFFFFFFFF) {
        return;
    }
    KeBornPtCmlp(pobj, dat, ctbl);
}

void pppKeBornPtCmplCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeBornPtCmpl *_t = (VKeBornPtCmpl *)&pobj->val[ctbl->useVal[IKeBornPtCmpl_This]];

    _t->PrePt = NULL;
}
