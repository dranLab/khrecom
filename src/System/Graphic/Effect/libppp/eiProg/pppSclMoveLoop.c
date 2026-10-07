#include "pppSclMoveLoop.h"

void pppSclMoveLoopCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    float val = 0.0f;
    VSclMoveLoop *pv = (VSclMoveLoop *)&pobj->val[ctbl->useVal[1]];

    pv->m_dscl.x = pv->m_dscl.y = pv->m_dscl.z = val;
}

void pppSclMoveLoopCon2(pppPObject *pobj, pppCtrlTable *ctbl) {
    pppSclMoveLoopCon(pobj, ctbl);
}

void pppSclMoveLoop(pppPObject *pobj, PSclMoveLoop *p, pppCtrlTable *ctbl) {
    VScaleLoop *ps = (VScaleLoop *)&pobj->val[ctbl->useVal[0]];
    VSclMoveLoop *pv = (VSclMoveLoop *)&pobj->val[ctbl->useVal[1]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (p->cdt.time == pobj->time) {
        pv->m_dscl.x += p->m_dscl.x;
        pv->m_dscl.y += p->m_dscl.y;
        pv->m_dscl.z += p->m_dscl.z;
    }

    if (ppvEmptyLoop != 0) {
        return;
    }

    ps->m_scl.x += pv->m_dscl.x;
    ps->m_scl.y += pv->m_dscl.y;
    ps->m_scl.z += pv->m_dscl.z;
}
