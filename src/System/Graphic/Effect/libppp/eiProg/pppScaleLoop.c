#include "pppScaleLoop.h"

static void pppScaleLoopInit(PScaleLoop *p, VScaleLoop *pv) {
    pv->m_pp = p;
    pv->m_scl.x = pv->m_scl.y = pv->m_scl.z = 0.0f;
}

void pppScaleLoopCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VScaleLoop *pv = (VScaleLoop *)&pobj->val[ctbl->useVal[0]];

    pv->m_pp = NULL;
}

void pppScaleLoopCon2(pppPObject *pobj, pppCtrlTable *ctbl) {
    VScaleLoop *pv = (VScaleLoop *)&pobj->val[ctbl->useVal[0]];
    PScaleLoop *p = (PScaleLoop *)pv->m_pp;

    pv->m_scl.x -= p->m_scl.x;
    pv->m_scl.y -= p->m_scl.y;
    pv->m_scl.z -= p->m_scl.z;
}

void pppScaleLoop(pppPObject *pobj, PScaleLoop *p, pppCtrlTable *ctbl) {
    VScaleLoop *pv = (VScaleLoop *)&pobj->val[ctbl->useVal[0]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (pv->m_pp == NULL) {
        pppScaleLoopInit(p, pv);
    }

    if (p->cdt.time == pobj->time) {
        pv->m_scl.x += p->m_scl.x;
        pv->m_scl.y += p->m_scl.y;
        pv->m_scl.z += p->m_scl.z;
    }
}
