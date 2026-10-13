#define TASK_H_NO_SLC_OVERFLOW_MSG
#include "Game/Gimmick/GimmickFlowerLump.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Anim.h"
#include "Game/Movement.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"

CGimmickFlowerLump::CGimmickFlowerLump(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_WaitCounter = 0.0f;
}

CGimmickFlowerLump::~CGimmickFlowerLump() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickFlowerLump::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    changeMotion(1, 0, FALSE, 0, 0, 0);
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, TRUE);
    m_pMove->setCollisionFlag(0xF, FALSE);
    m_pMove->setFlag(8, TRUE);
    return TRUE;
}

void CGimmickFlowerLump::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat != 0) {
        changeMotion(4, 0, FALSE, 0, 0, 0);
    } else {
        changeMotion(2, 0, FALSE, 0, 0, 0);
    }
}

void CGimmickFlowerLump::beginStateMain() {}

void CGimmickFlowerLump::stateMain() {
    if (m_Stat != 0) return;
    switch (m_Step) {
    case 0:
        changeMotion(2, 0, FALSE, 0, 0, 0);
        m_WaitCounter = 120.0f;
        m_Step = 10;
        break;
    case 10:
        m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) m_Step = 20;
        break;
    case 20:
        changeMotion(3, 0, FALSE, 0, 0, 0);
        m_Step = 30;
        break;
    case 30:
        if (isMotionEnd()) {
            changeMotion(4, 0, FALSE, 0, 0, 0);
            m_Step = 40;
            m_WaitCounter = 120.0f;
        }
        break;
    case 40:
        m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) m_Step = 50;
        break;
    case 50:
        changeMotion(1, 0, FALSE, 0, 0, 0);
        m_Step = 60;
        break;
    case 60:
        if (isMotionEnd()) m_Step = 0;
        break;
    }
}

void CGimmickFlowerLump::endStateMain() {}

void CGimmickFlowerLump::beginStateHitAttack() {
    if (m_Stat != 1) {
        if (m_pAnim->getAnimID() == 3 || m_pAnim->getAnimID() == 4) {
            changeMotion(4, 0, FALSE, -1, 0, 0);
            createPrize(0, 0);
            m_Stat = 1;
            saveStat(1);
        }
    }
    changeGimmickState(0);
}

void CGimmickFlowerLump::stateHitAttack() {}

void CGimmickFlowerLump::endStateHitAttack() {}
// SOURCE DATA: .rodata
