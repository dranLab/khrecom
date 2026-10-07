#include "pppSRandUpCV.h"

static char randchar(char rv, float r0) {
    return rv * r0;
}

static float randf(u_char rct) {
    float r0;
    float r1;

    PPP_RAND_F(r0);
    if (rct) {
        PPP_RAND_F(r1);
        r0 = (r0 + r1) * 0.5f;
    }
    return r0;
}

void pppSRandUpCV(pppPObject *pobj, PSRandUpCV *p, pppCtrlTable *ctbl) {
    VSRandUpCV *v;
    pppCVECTOR *av;

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    if (p->cdt.time != pobj->time) {
        return;
    }

    v = (VSRandUpCV *)&pobj->val[ctbl->useVal[0]];
    v->r0 = randf(p->rct);
    v->r1 = randf(p->rct);
    v->r2 = randf(p->rct);
    v->r3 = randf(p->rct);

    if (p->pvoff != -1) {
        av = (pppCVECTOR *)&pobj->val[p->pvoff];
    } else {
        av = (pppCVECTOR *)ppvDbgTemp;
    }
    av->r += randchar(p->rw.r, v->r0);
    av->g += randchar(p->rw.g, v->r1);
    av->b += randchar(p->rw.b, v->r2);
    av->a += randchar(p->rw.a, v->r3);
}
