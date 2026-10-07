#include "pppEiWindFun.h"

void pppEiWindFunCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VEiWindFun *v = (VEiWindFun *)&pobj->val[ctbl->useVal[0]];

    v->len = 0.0f;
    v->r = 0.0f;
    v->wv = v->wdv = v->wddv = v->len;
}

void pppEiWindFun(pppPObject *pobj, PEiWindFun *p, pppCtrlTable *ctbl) {
    VEiWindFun *v = (VEiWindFun *)&pobj->val[ctbl->useVal[0]];
    float len;

    if (ppvUserStopPartF == 0 && !ppvMng->bFlagStop && !ppvMng->bBoolSlow) {
        v->wdv += v->wddv;
        v->wv += v->wdv;
    }

    if (p->cdt.time == pobj->time) {
        v->vec = p->vec;
        v->len = p->len;
        v->r = p->r;
        len = v->len * v->len + v->r * v->r;
        __asm__ __volatile__("qmtc2.ni %1, $vf2\n"
                             "\tvsqrt $Q, $vf2x\n"
                             "\tvwaitq\n"
                             "\tvaddq.x $vf2, $vf0, $Q\n"
                             "\tqmfc2.ni %0, $vf2"
                             : "=r"(len) : "r"(len));
        *(float *)&v->cosa = v->len / len;

        if (ppvUserStopPartF == 0 && !ppvMng->bFlagStop && !ppvMng->bBoolSlow) {
            v->wv += p->wv;
            v->wdv += p->wdv;
            v->wddv += p->wddv;
        }
    }
}
