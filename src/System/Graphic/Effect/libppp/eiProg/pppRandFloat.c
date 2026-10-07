#include "pppRandFloat.h"

void pppRandFloat(pppPObject *pobj, PRandFloat *p, pppCtrlTable *ctbl) {
    VRandFloat *v;
    float *av;
    float r0;
    float r1;

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (pobj->time == 0) {
        PPP_RAND_F(r0);
        if (p->rct) {
            PPP_RAND_F(r1);
            r0 += r1;
        } else {
            r0 += r0;
        }
        v = (VRandFloat *)&pobj->val[ctbl->useVal[0]];
        v->r0 = r0;
    } else {
        if (p->cdt.time != pobj->time) {
            return;
        }
        v = (VRandFloat *)&pobj->val[ctbl->useVal[0]];
    }

    if (p->pvoff != -1) {
        av = (float *)&pobj->val[p->pvoff];
    } else {
        av = (float *)ppvDbgTemp;
    }
    *av += p->rw * v->r0 - p->rw;
}
