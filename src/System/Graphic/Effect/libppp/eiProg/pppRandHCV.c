#include "pppRandHCV.h"

static short int randshort(short int rv, float r0) {
    return rv * r0 - rv;
}

void pppRandHCV(pppPObject *pobj, PRandHCV *p, pppCtrlTable *ctbl) {
    VRandHCV *v;
    pppHCVECTOR *av;
    float r0;
    float r1;

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (p->cdt.time != pobj->time) {
        return;
    }
    PPP_RAND_F(r0);
    if (p->rct) {
        PPP_RAND_F(r1);
        r0 += r1;
    } else {
        r0 += r0;
    }
    v = (VRandHCV *)&pobj->val[ctbl->useVal[0]];
    v->r0 = r0;

    if (p->pvoff != -1) {
        av = (pppHCVECTOR *)&pobj->val[p->pvoff];
    } else {
        av = (pppHCVECTOR *)ppvDbgTemp;
    }
    av->r += randshort(p->rw.r, v->r0);
    av->g += randshort(p->rw.g, v->r0);
    av->b += randshort(p->rw.b, v->r0);
    av->a += randshort(p->rw.a, v->r0);
}
