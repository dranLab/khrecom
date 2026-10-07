#include "pppPointLoop.h"

static void pppPointLoopInit(PPointLoop *p, VPointLoop *pv) {
    pv->m_pp = p;
    pv->m_pos.x = pv->m_pos.y = pv->m_pos.z = 0.0f;
}

void pppPointLoopCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VPointLoop *pv = (VPointLoop *)&pobj->val[ctbl->useVal[0]];

    pv->m_pp = NULL;
}

void pppPointLoopCon2(pppPObject *pobj, pppCtrlTable *ctbl) {
    VPointLoop *pv = (VPointLoop *)&pobj->val[ctbl->useVal[0]];
    PPointLoop *p = (PPointLoop *)pv->m_pp;

    pv->m_pos.x -= p->m_pos.x;
    pv->m_pos.y -= p->m_pos.y;
    pv->m_pos.z -= p->m_pos.z;
}

void pppPointLoop(pppPObject *pobj, PPointLoop *p, pppCtrlTable *ctbl) {
    VPointLoop *pv = (VPointLoop *)&pobj->val[ctbl->useVal[0]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (pv->m_pp == NULL) {
        pppPointLoopInit(p, pv);
    }

    if (p->cdt.time == pobj->time) {
        pv->m_pos.x += p->m_pos.x;
        pv->m_pos.y += p->m_pos.y;
        pv->m_pos.z += p->m_pos.z;
    }
}
