#include "pppSclAccele.h"

void pppSclAccele(pppPObject *pobj, PSclAccele *p, pppCtrlTable *ctbl) {
    VSclMove *mv = (VSclMove *)&pobj->val[ctbl->useVal[0]];
    VSclAccele *av = (VSclAccele *)&pobj->val[ctbl->useVal[1]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (p->cdt.time == pobj->time) {
        av->ddscl.x += p->ddscl.x;
        av->ddscl.y += p->ddscl.y;
        av->ddscl.z += p->ddscl.z;
    }

    mv->dscl.x += av->ddscl.x;
    mv->dscl.y += av->ddscl.y;
    mv->dscl.z += av->ddscl.z;
}

void pppSclAcceleCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    float val = 0.0f;
    VSclAccele *av = (VSclAccele *)&pobj->val[ctbl->useVal[1]];

    av->ddscl.x = av->ddscl.y = av->ddscl.z = val;
}
