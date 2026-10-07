#include "pppAccele.h"

void pppAccele(pppPObject *pobj, PAccele *p, pppCtrlTable *ctbl) {
    VMove *mv = (VMove *)&pobj->val[ctbl->useVal[0]];
    VAccele *ac = (VAccele *)&pobj->val[ctbl->useVal[1]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (p->cdt.time == pobj->time) {
        ac->vec.x += p->vec.x;
        ac->vec.y += p->vec.y;
        ac->vec.z += p->vec.z;
    }

    mv->vec.x += ac->vec.x;
    mv->vec.y += ac->vec.y;
    mv->vec.z += ac->vec.z;
}

void pppAcceleCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    float val = 0.0f;
    VAccele *ac = (VAccele *)&pobj->val[ctbl->useVal[1]];

    ac->vec.x = ac->vec.y = ac->vec.z = val;
}
