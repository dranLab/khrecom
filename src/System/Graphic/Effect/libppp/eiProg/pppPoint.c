#include "pppPoint.h"

void pppPoint(pppPObject *pobj, PPoint *p, pppCtrlTable *ctbl) {
    VPoint *v;

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    if (p->cdt.time != pobj->time) {
        return;
    }

    v = (VPoint *)&pobj->val[ctbl->useVal[0]];
    v->pos.x += p->pos.x;
    v->pos.y += p->pos.y;
    v->pos.z += p->pos.z;
}

void pppPointCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VPoint *v = (VPoint *)&pobj->val[ctbl->useVal[0]];

    v->pos.x = v->pos.y = v->pos.z = 0.0f;
}
