#include "pppMove.h"

void pppMove(pppPObject *pobj, PMove *p, pppCtrlTable *ctbl) {
    VPoint *pv = (VPoint *)&pobj->val[ctbl->useVal[0]];
    VMove *mv = (VMove *)&pobj->val[ctbl->useVal[1]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (p->cdt.time == pobj->time) {
        mv->vec.x += p->vec.x;
        mv->vec.y += p->vec.y;
        mv->vec.z += p->vec.z;
    }

    pv->pos.x += mv->vec.x;
    pv->pos.y += mv->vec.y;
    pv->pos.z += mv->vec.z;
}

void pppMoveCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    float val = 0.0f;
    VMove *mv = (VMove *)&pobj->val[ctbl->useVal[1]];

    mv->vec.x = mv->vec.y = mv->vec.z = val;
}
