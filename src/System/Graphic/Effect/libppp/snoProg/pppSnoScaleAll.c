#include "pppSnoScaleAll.h"

void pppSnoScaleAll(pppPObject *pobj, PSnoScaleAll *p, pppCtrlTable *ctbl) {
    VSnoScaleAll *v;

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    v = (VSnoScaleAll *)&pobj->val[ctbl->useVal[0]];
    if (p->cdt.time == pobj->time) {
        v->ddscl.x += p->ddscl.x;
        v->ddscl.y += p->ddscl.y;
        v->ddscl.z += p->ddscl.z;
        v->dscl.x += p->dscl.x;
        v->dscl.y += p->dscl.y;
        v->dscl.z += p->dscl.z;
        v->scl0.x += p->scl.x;
        v->scl0.y += p->scl.y;
        v->scl0.z += p->scl.z;
        v->all += p->all;
        v->dall += p->dall;
        v->ddall += p->ddall;
    }

    v->dscl.x += v->ddscl.x;
    v->dscl.y += v->ddscl.y;
    v->dscl.z += v->ddscl.z;
    v->scl0.x += v->dscl.x;
    v->scl0.y += v->dscl.y;
    v->scl0.z += v->dscl.z;
    v->dall += v->ddall;
    v->all += v->dall;
    v->scl.x = v->scl0.x * v->all;
    v->scl.y = v->scl0.y * v->all;
    v->scl.z = v->scl0.z * v->all;
}

void pppSnoScaleAllCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    pppMemClear(&pobj->val[ctbl->useVal[0]], sizeof(VSnoScaleAll));
}
