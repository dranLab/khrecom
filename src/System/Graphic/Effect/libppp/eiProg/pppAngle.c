#include "pppAngle.h"

void pppAngle(pppPObject *pobj, PAngle *p, pppCtrlTable *ctbl) {
    VAngle *v;

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    if (p->cdt.time != pobj->time) {
        return;
    }

    v = (VAngle *)&pobj->val[ctbl->useVal[0]];
    v->ang.x += p->ang.x;
    v->ang.y += p->ang.y;
    v->ang.z += p->ang.z;
}

void pppAngleCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VAngle *v = (VAngle *)&pobj->val[ctbl->useVal[0]];

    v->ang.x = v->ang.y = v->ang.z = 0;
}
