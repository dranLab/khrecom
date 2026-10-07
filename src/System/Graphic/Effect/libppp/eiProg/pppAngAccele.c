#include "pppAngAccele.h"

void pppAngAccele(pppPObject *pobj, PAngAccele *p, pppCtrlTable *ctbl) {
    VAngMove *mv = (VAngMove *)&pobj->val[ctbl->useVal[0]];
    VAngAccele *av = (VAngAccele *)&pobj->val[ctbl->useVal[1]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (p->cdt.time == pobj->time) {
        av->ddang.x += p->ddang.x;
        av->ddang.y += p->ddang.y;
        av->ddang.z += p->ddang.z;
    }

    mv->dang.x += av->ddang.x;
    mv->dang.y += av->ddang.y;
    mv->dang.z += av->ddang.z;
}

void pppAngAcceleCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VAngAccele *av = (VAngAccele *)&pobj->val[ctbl->useVal[1]];

    av->ddang.x = av->ddang.y = av->ddang.z = 0;
}
