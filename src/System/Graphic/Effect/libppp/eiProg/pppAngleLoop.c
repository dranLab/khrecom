#include "pppAngleLoop.h"
#include "include_asm.h"

static void pppAngleLoopInit(PAngleLoop *p, VAngleLoop *pv) {
    pv->m_pp = p;
    pv->m_ang.z = pv->m_ang.x = pv->m_ang.y = 0;
}

void pppAngleLoopCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VAngleLoop *pv = (VAngleLoop *)&pobj->val[ctbl->useVal[0]];

    pv->m_pp = NULL;
}

void pppAngleLoopCon2(pppPObject *pobj, pppCtrlTable *ctbl) {
    VAngleLoop *pv = (VAngleLoop *)&pobj->val[ctbl->useVal[0]];
    PAngleLoop *p = (PAngleLoop *)pv->m_pp;

    pv->m_ang.x -= p->m_ang.x;
    pv->m_ang.y -= p->m_ang.y;
    pv->m_ang.z -= p->m_ang.z;
}

void fix_angle(pppIVECTOR *angle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/libppp/eiProg/pppAngleLoop", fix_angle);
#if 0
/* Code matches, but Ps2EeAs pads each short loop with 3 nops (6 instructions before the delay
   slot); the original has 2 (6 including the delay slot). */
static void fix_angle(pppIVECTOR *angle) {
    while (angle->x > 0x10000) angle->x -= 0x10000;
    while (angle->y > 0x10000) angle->y -= 0x10000;
    while (angle->z > 0x10000) angle->z -= 0x10000;
}
#endif

void pppAngleLoop(pppPObject *pobj, PAngleLoop *p, pppCtrlTable *ctbl) {
    VAngleLoop *pv = (VAngleLoop *)&pobj->val[ctbl->useVal[0]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (pv->m_pp == NULL) {
        pppAngleLoopInit(p, pv);
    }

    if (p->cdt.time == pobj->time) {
        pv->m_ang.x += p->m_ang.x;
        pv->m_ang.y += p->m_ang.y;
        pv->m_ang.z += p->m_ang.z;
        fix_angle(&pv->m_ang);
    }
}
