#include "pppRandInt.h"

void pppRandInt(pppPObject *pobj, PRandInt *p, pppCtrlTable *ctbl) {
    VRandInt *v;
    int *av;
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
        v = (VRandInt *)&pobj->val[ctbl->useVal[0]];
        v->r0 = r0;
    } else {
        if (p->cdt.time != pobj->time) {
            return;
        }
        v = (VRandInt *)&pobj->val[ctbl->useVal[0]];
    }

    if (p->pvoff != -1) {
        av = (int *)&pobj->val[p->pvoff];
    } else {
        av = (int *)ppvDbgTemp;
    }
    *av += (int)(p->rw * v->r0 - p->rw);
}
