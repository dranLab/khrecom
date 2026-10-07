#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/Gimmick/GimmickBreakable.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
#include "Game/Movement.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"

/* Placeholder: the last header string in .rodata is an empty string, after CollUtil.h's (the same in
 * Game/Enemy/EnemyWyvern); which header it comes from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

CGimmickBreakable::CGimmickBreakable(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_Stat = 0;
}

CGimmickBreakable::~CGimmickBreakable() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickBreakable::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, TRUE);
    m_pMove->setCollisionFlag(0xF, FALSE);
    m_pMove->setFlag(8, TRUE);
    setGimmickFlag(8, TRUE);
    bodyCollOffBody(-1);
    rideModelEnableAllCollision(FALSE);
    return TRUE;
}

void CGimmickBreakable::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat == 0) {
        bodyCollOnBody(-1);
        rideModelEnableAllCollision(TRUE);
    }
    rideModelChangeListR2();
}

void CGimmickBreakable::beginStateMain() {
    if (m_Stat == 1) {
        saveStat(1);
        out();
    }
}

void CGimmickBreakable::stateMain() {}

void CGimmickBreakable::endStateMain() {}

void CGimmickBreakable::beginStateHitAttack() {
    if (m_Stat == 0) createPrize(0, 0);
    rideModelEnableAllCollision(FALSE);
    changeMotion(2, 0, FALSE, -1, 0, 0);
    m_Stat = 1;
    setEntityFlag(1, TRUE);
    changeGimmickState(0);
}

void CGimmickBreakable::stateHitAttack() {}

void CGimmickBreakable::endStateHitAttack() {}
// SOURCE DATA: .rodata
