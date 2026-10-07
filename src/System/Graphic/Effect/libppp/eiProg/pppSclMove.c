#include "pppSclMove.h"

void pppSclMove(pppPObject *pobj, PSclMove *p, pppCtrlTable *ctbl) {
    VScale *pv = (VScale *)&pobj->val[ctbl->useVal[0]];
    VSclMove *mv = (VSclMove *)&pobj->val[ctbl->useVal[1]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (p->cdt.time == pobj->time) {
        mv->dscl.x += p->dscl.x;
        mv->dscl.y += p->dscl.y;
        mv->dscl.z += p->dscl.z;
    }

    pv->scl.x += mv->dscl.x;
    pv->scl.y += mv->dscl.y;
    pv->scl.z += mv->dscl.z;
}

void pppSclMoveCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    float val = 0.0f;
    VSclMove *mv = (VSclMove *)&pobj->val[ctbl->useVal[1]];

    mv->dscl.x = mv->dscl.y = mv->dscl.z = val;
}
