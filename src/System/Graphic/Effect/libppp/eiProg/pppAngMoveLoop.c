#include "pppAngMoveLoop.h"

void pppAngMoveLoopCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VAngMoveLoop *pv = (VAngMoveLoop *)&pobj->val[ctbl->useVal[1]];

    pv->m_dang.x = pv->m_dang.y = pv->m_dang.z = 0;
}

void pppAngMoveLoopCon2(pppPObject *pobj, pppCtrlTable *ctbl) {
    pppAngMoveLoopCon(pobj, ctbl);
}

void pppAngMoveLoop(pppPObject *pobj, PAngMoveLoop *p, pppCtrlTable *ctbl) {
    VAngleLoop *pa = (VAngleLoop *)&pobj->val[ctbl->useVal[0]];
    VAngMoveLoop *pv = (VAngMoveLoop *)&pobj->val[ctbl->useVal[1]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (p->cdt.time == pobj->time) {
        pv->m_dang.x += p->m_dang.x;
        pv->m_dang.y += p->m_dang.y;
        pv->m_dang.z += p->m_dang.z;
    }

    if (ppvEmptyLoop != 0) {
        return;
    }

    pa->m_ang.x += pv->m_dang.x;
    pa->m_ang.y += pv->m_dang.y;
    pa->m_ang.z += pv->m_dang.z;
}
