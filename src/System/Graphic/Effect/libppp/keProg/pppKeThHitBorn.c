#include "pppKeThHitBorn.h"

void pppKeThHitBorn(pppPObject *pobj, PKeThHitBorn *dat, pppCtrlTable *ctbl) {
    VKeThHitBorn *_t = (VKeThHitBorn *)&pobj->val[ctbl->useVal[IKeThHitBorn_This]];
    VKeTh *pTh;
    pppPObject *pObj;
    KeThBP *pBP;

    if (pobj->time == dat->cdt.time) {
        if (dat->CmdContinue) {
            _t->Stt |= FKeThHitBorn_SttContinue;
        } else {
            _t->Stt &= ~FKeThHitBorn_SttContinue;
        }
    }
    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    if (dat->BornPt.pdtn == 0xFFFFFFFF) {
        return;
    }

    pTh = (VKeTh *)&pobj->val[ctbl->useVal[IKeThHitBorn_KeTh]];
    pBP = (KeThBP *)pTh->Cp.UdtList.FirstItem;
    if (_t->Stt & FKeThHitBorn_SttContinue) {
        for (; pBP != NULL; pBP = (KeThBP *)pBP->UdtTag.NextItem) {
            if (pBP->Flg & KeThBP_SttHit) {
                pppPDataVal *pdv = &ppvMng->m_pdtval[dat->BornPt.pdtn];

                if (pdv == NULL) {
                    pObj = NULL;
                } else {
                    pObj = pppCreatePObject(ppvMng, pdv);
                    if (pObj != NULL) {
                        pObj->ppobj = pobj;
                    }
                }
                if (pObj != NULL) {
                    KeVu0CopyVecXYZ((float *)&pObj->val[dat->BornPt.valoff], pBP->RpTbl[pBP->LastIdx].RPos);
                }
            }
        }
    } else {
        for (; pBP != NULL; pBP = (KeThBP *)pBP->UdtTag.NextItem) {
            if ((pBP->Flg & KeThBP_SttHit) && !(pBP->Flg & KeThBP_SttHitted)) {
                pppPDataVal *pdv = &ppvMng->m_pdtval[dat->BornPt.pdtn];

                if (pdv == NULL) {
                    pObj = NULL;
                } else {
                    pObj = pppCreatePObject(ppvMng, pdv);
                    if (pObj != NULL) {
                        pObj->ppobj = pobj;
                    }
                }
                if (pObj != NULL) {
                    KeVu0CopyVecXYZ((float *)&pObj->val[dat->BornPt.valoff], pBP->RpTbl[pBP->LastIdx].RPos);
                }
            }
        }
    }
}

void pppKeThHitBornCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeThHitBorn *_t = (VKeThHitBorn *)&pobj->val[ctbl->useVal[IKeThHitBorn_This]];

    _t->Stt = FKeThHitBorn_SttClr_;
}
