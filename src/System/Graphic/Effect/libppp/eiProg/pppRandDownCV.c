#include "pppRandDownCV.h"

static char randchar(char rv, float r0) {
    return rv * r0;
}

void pppRandDownCV(pppPObject *pobj, PRandDownCV *p, pppCtrlTable *ctbl) {
    VRandDownCV *v;
    pppCVECTOR *av;
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
    r0 = -r0;
    if (p->rct) {
        PPP_RAND_F(r1);
        r0 = (r0 - r1) * 0.5f;
    }
    v = (VRandDownCV *)&pobj->val[ctbl->useVal[0]];
    v->r0 = r0;

    if (p->pvoff != -1) {
        av = (pppCVECTOR *)&pobj->val[p->pvoff];
    } else {
        av = (pppCVECTOR *)ppvDbgTemp;
    }
    av->r += randchar(p->rw.r, v->r0);
    av->g += randchar(p->rw.g, v->r0);
    av->b += randchar(p->rw.b, v->r0);
    av->a += randchar(p->rw.a, v->r0);
}
