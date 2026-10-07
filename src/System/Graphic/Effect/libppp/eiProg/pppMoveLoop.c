#include "pppMoveLoop.h"

void pppMoveLoopCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    float val = 0.0f;
    VMoveLoop *pv = (VMoveLoop *)&pobj->val[ctbl->useVal[1]];

    pv->m_vec.x = pv->m_vec.y = pv->m_vec.z = val;
}

void pppMoveLoopCon2(pppPObject *pobj, pppCtrlTable *ctbl) {
    pppMoveLoopCon(pobj, ctbl);
}

void pppMoveLoop(pppPObject *pobj, PMoveLoop *p, pppCtrlTable *ctbl) {
    VPointLoop *pp = (VPointLoop *)&pobj->val[ctbl->useVal[0]];
    VMoveLoop *pv = (VMoveLoop *)&pobj->val[ctbl->useVal[1]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (p->cdt.time == pobj->time) {
        pv->m_vec.x += p->m_vec.x;
        pv->m_vec.y += p->m_vec.y;
        pv->m_vec.z += p->m_vec.z;
    }

    if (ppvEmptyLoop != 0) {
        return;
    }

    pp->m_pos.x += pv->m_vec.x;
    pp->m_pos.y += pv->m_vec.y;
    pp->m_pos.z += pv->m_vec.z;
}
