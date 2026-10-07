#include "pppPObjPoint.h"

void pppPObjPoint(pppPObject *pobj, PPObjPoint *p, pppCtrlTable *ctbl) {
    VPObjPoint *v;

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    v = (VPObjPoint *)&pobj->val[ctbl->useVal[0]];
    if (p->cdt.time == pobj->time) {
        v->lpvp = (p->pobj.pdtn != 0xFFFFFFFF)
                      ? (VPoint *)&ppvMng->m_pdtval[p->pobj.pdtn].pobj->val[p->pobj.valoff]
                      : (VPoint *)ppvDbgTemp;
    }
    v->pos.x = v->lpvp->pos.x;
    v->pos.y = v->lpvp->pos.y;
    v->pos.z = v->lpvp->pos.z;
}
