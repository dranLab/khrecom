#include "pppRandUpFV.h"

static float randf(float rv, float r0) {
    return rv * r0;
}

void pppRandUpFV(pppPObject *pobj, PRandUpFV *p, pppCtrlTable *ctbl) {
    VRandUpFV *v;
    pppFVECTOR *av;
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
            r0 = (r0 + r1) * 0.5f;
        }
        v = (VRandUpFV *)&pobj->val[ctbl->useVal[0]];
        v->r0 = r0;
    } else {
        if (p->cdt.time != pobj->time) {
            return;
        }
        v = (VRandUpFV *)&pobj->val[ctbl->useVal[0]];
    }

    if (p->pvoff != -1) {
        av = (pppFVECTOR *)&pobj->val[p->pvoff];
    } else {
        av = (pppFVECTOR *)ppvDbgTemp;
    }
    av->x += randf(p->rw.x, v->r0);
    av->y += randf(p->rw.y, v->r0);
    av->z += randf(p->rw.z, v->r0);
}
