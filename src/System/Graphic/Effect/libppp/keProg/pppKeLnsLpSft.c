#include "pppKeLnsLpSft.h"

extern BOOL ppvUserStopPartF;

void pppKeLnsLpSft(pppPObject *pobj, PKeLnsLpSft *dat, pppCtrlTable *ctbl) {
    VKeLnsLpSft *pThis;
    KeLnsLp *pTgt;

    if (ppvUserStopPartF == 0 && ppvMng->bFlagStop == 0 && ppvMng->bBoolSlow == 0) {
        pThis = (VKeLnsLpSft *)&pobj->val[ctbl->useVal[0]];
        pTgt = (KeLnsLp *)&pobj->val[ctbl->useVal[1]];
        if (pobj->time == dat->cdt.time) {
            pThis->LWid += dat->LWid;
            pThis->Pow += dat->Pow;
            if (pobj->time == 0 && pobj->loop_in == 0) {
                return;
            }
        }
        pTgt->LVec[3] += pThis->LWid;
        pTgt->Pow += pThis->Pow;
    }
}

void pppKeLnsLpSftCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeLnsLpSft *pThis;

    pThis = (VKeLnsLpSft *)&pobj->val[ctbl->useVal[0]];
    pThis->LWid = 0;
    pThis->Pow = 0;
}
