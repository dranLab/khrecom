#include "pppColAccele.h"

void pppColAccele(pppPObject *pobj, PColAccele *p, pppCtrlTable *ctbl) {
    VColMove *mv = (VColMove *)&pobj->val[ctbl->useVal[0]];
    VColAccele *av = (VColAccele *)&pobj->val[ctbl->useVal[1]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (p->cdt.time == pobj->time) {
        av->ddcol.r += p->ddcol.r;
        av->ddcol.g += p->ddcol.g;
        av->ddcol.b += p->ddcol.b;
        av->ddcol.a += p->ddcol.a;
    }

    mv->dcol.r += av->ddcol.r;
    mv->dcol.g += av->ddcol.g;
    mv->dcol.b += av->ddcol.b;
    mv->dcol.a += av->ddcol.a;
}

void pppColAcceleCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VColAccele *av = (VColAccele *)&pobj->val[ctbl->useVal[1]];

    av->ddcol.r = av->ddcol.g = av->ddcol.b = av->ddcol.a = 0;
}
