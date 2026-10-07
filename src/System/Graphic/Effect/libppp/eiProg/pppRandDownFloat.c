#include "pppRandDownFloat.h"

void pppRandDownFloat(pppPObject *pobj, PRandDownFloat *p, pppCtrlTable *ctbl) {
    VRandDownFloat *v;
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
        r0 = -r0;
        if (p->rct) {
            PPP_RAND_F(r1);
            r0 = (r0 - r1) * 0.5f;
        }
        v = (VRandDownFloat *)&pobj->val[ctbl->useVal[0]];
        v->r0 = r0;
    } else {
        if (p->cdt.time != pobj->time) {
            return;
        }
        v = (VRandDownFloat *)&pobj->val[ctbl->useVal[0]];
    }

    if (p->pvoff != -1) {
        av = (float *)&pobj->val[p->pvoff];
    } else {
        av = (float *)ppvDbgTemp;
    }
    *av += p->rw * v->r0;
}
