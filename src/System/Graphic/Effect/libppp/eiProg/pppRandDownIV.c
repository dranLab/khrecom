#include "pppRandDownIV.h"

static int randint(int rv, float r0) {
    return rv * r0;
}

void pppRandDownIV(pppPObject *pobj, PRandDownIV *p, pppCtrlTable *ctbl) {
    VRandDownIV *v;
    pppIVECTOR *av;
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
    v = (VRandDownIV *)&pobj->val[ctbl->useVal[0]];
    v->r0 = r0;

    if (p->pvoff != -1) {
        av = (pppIVECTOR *)&pobj->val[p->pvoff];
    } else {
        av = (pppIVECTOR *)ppvDbgTemp;
    }
    av->x += randint(p->rw.x, v->r0);
    av->y += randint(p->rw.y, v->r0);
    av->z += randint(p->rw.z, v->r0);
}
