#include "pppSRandFV.h"

static float randfloat(float rv, float r0) {
    return rv * r0 - rv;
}

static float randf(u_char rct) {
    float r0;
    float r1;

    PPP_RAND_F(r0);
    if (rct) {
        PPP_RAND_F(r1);
        r0 += r1;
    } else {
        r0 += r0;
    }
    return r0;
}

void pppSRandFV(pppPObject *pobj, PSRandFV *p, pppCtrlTable *ctbl) {
    VSRandFV *v;
    pppFVECTOR *av;

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (pobj->time == 0) {
        v = (VSRandFV *)&pobj->val[ctbl->useVal[0]];
        v->r0 = randf(p->rct);
        v->r1 = randf(p->rct);
        v->r2 = randf(p->rct);
    } else {
        if (p->cdt.time != pobj->time) {
            return;
        }
        v = (VSRandFV *)&pobj->val[ctbl->useVal[0]];
    }

    if (p->pvoff != -1) {
        av = (pppFVECTOR *)&pobj->val[p->pvoff];
    } else {
        av = (pppFVECTOR *)ppvDbgTemp;
    }
    av->x += randfloat(p->rw.x, v->r0);
    av->y += randfloat(p->rw.y, v->r1);
    av->z += randfloat(p->rw.z, v->r2);
}
