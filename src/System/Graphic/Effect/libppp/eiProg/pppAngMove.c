#include "pppAngMove.h"

void pppAngMove(pppPObject *pobj, PAngMove *p, pppCtrlTable *ctbl) {
    VAngle *pv = (VAngle *)&pobj->val[ctbl->useVal[0]];
    VAngMove *mv = (VAngMove *)&pobj->val[ctbl->useVal[1]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (p->cdt.time == pobj->time) {
        mv->dang.x += p->dang.x;
        mv->dang.y += p->dang.y;
        mv->dang.z += p->dang.z;
    }

    pv->ang.x += mv->dang.x;
    pv->ang.y += mv->dang.y;
    pv->ang.z += mv->dang.z;
}

void pppAngMoveCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VAngMove *mv = (VAngMove *)&pobj->val[ctbl->useVal[1]];

    mv->dang.x = mv->dang.y = mv->dang.z = 0;
}
