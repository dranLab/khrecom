#include "pppColMove.h"

void pppColMove(pppPObject *pobj, PColMove *p, pppCtrlTable *ctbl) {
    VColor *pv = (VColor *)&pobj->val[ctbl->useVal[0]];
    VColMove *mv = (VColMove *)&pobj->val[ctbl->useVal[1]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (p->cdt.time == pobj->time) {
        mv->dcol.r += p->dcol.r;
        mv->dcol.g += p->dcol.g;
        mv->dcol.b += p->dcol.b;
        mv->dcol.a += p->dcol.a;
    }

    pv->col.r += mv->dcol.r;
    pv->col.g += mv->dcol.g;
    pv->col.b += mv->dcol.b;
    pv->col.a += mv->dcol.a;
}

void pppColMoveCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VColMove *mv = (VColMove *)&pobj->val[ctbl->useVal[1]];

    mv->dcol.r = mv->dcol.g = mv->dcol.b = mv->dcol.a = 0;
}
