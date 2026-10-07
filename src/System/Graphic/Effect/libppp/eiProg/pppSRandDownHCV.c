#include "pppSRandDownHCV.h"

static char randshort(short int rv, float r0) {
    return rv * r0;
}

static float randf(u_char rct) {
    float r0;
    float r1;

    PPP_RAND_F(r0);
    r0 = -r0;
    if (rct) {
        PPP_RAND_F(r1);
        r0 = (r0 - r1) * 0.5f;
    }
    return r0;
}

void pppSRandDownHCV(pppPObject *pobj, PSRandDownHCV *p, pppCtrlTable *ctbl) {
    VSRandDownHCV *v;
    pppHCVECTOR *av;

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    if (p->cdt.time != pobj->time) {
        return;
    }

    v = (VSRandDownHCV *)&pobj->val[ctbl->useVal[0]];
    v->r0 = randf(p->rct);
    v->r1 = randf(p->rct);
    v->r2 = randf(p->rct);
    v->r3 = randf(p->rct);

    if (p->pvoff != -1) {
        av = (pppHCVECTOR *)&pobj->val[p->pvoff];
    } else {
        av = (pppHCVECTOR *)ppvDbgTemp;
    }
    av->r += randshort(p->rw.r, v->r0);
    av->g += randshort(p->rw.g, v->r1);
    av->b += randshort(p->rw.b, v->r2);
    av->a += randshort(p->rw.a, v->r3);
}
