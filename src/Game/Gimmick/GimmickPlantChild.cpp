
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Gimmick/GimmickPlantChild.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in GimmickBreakable); which
 * header it comes from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "System/Graphic/Model.h"
#include "Game/Movement.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"

CGimmickPlantChild::CGimmickPlantChild(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
}

CGimmickPlantChild::~CGimmickPlantChild() {}

BOOL CGimmickPlantChild::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    return TRUE;
}

void CGimmickPlantChild::AfterInit() {}

void CGimmickPlantChild::beginStateMain() {}

void CGimmickPlantChild::stateMain() {
    FLOAT al;
    INT i;

    switch (m_Step) {
    case 0:
        break;
    case 1:
        for (i = 0; i < getModelCount(); i++) {
            al = getModel()[i].getAlphaScale() - getFrameRateF() * 0.005;
            if (al <= 0.0f) {
                al = 0.0f;
                m_Step = 2;
            }
            getModel(i)->setAlphaScale(al);
        }
        break;
    case 2:
        out();
        break;
    }
}

void CGimmickPlantChild::endStateMain() {}

void CGimmickPlantChild::killPlant() {
    m_Step = 1;
    changeMotion(2, 0, FALSE, 0, 0, 0);
    setEntityFlag(0, 0x10000);
}
// SOURCE DATA: .rodata
