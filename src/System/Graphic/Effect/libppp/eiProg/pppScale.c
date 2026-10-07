#include "pppScale.h"

void pppScale(pppPObject *pobj, PScale *p, pppCtrlTable *ctbl) {
    VScale *v;

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    if (p->cdt.time != pobj->time) {
        return;
    }

    v = (VScale *)&pobj->val[ctbl->useVal[0]];
    v->scl.x += p->scl.x;
    v->scl.y += p->scl.y;
    v->scl.z += p->scl.z;
}

void pppScaleCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    float val = 0.0f;
    VScale *v = (VScale *)&pobj->val[ctbl->useVal[0]];

    v->scl.x = v->scl.y = v->scl.z = val;
}
