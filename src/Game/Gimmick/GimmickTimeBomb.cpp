#define TASK_H_NO_SLC_OVERFLOW_MSG
#include "Game/Gimmick/GimmickTimeBomb.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Movement.h"
#include "Game/AttackMotion.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"

CGimmickTimeBomb::CGimmickTimeBomb(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_WaitCounter = 0.0f;
    m_Kill = 0;
    m_Prize = 0;
}

CGimmickTimeBomb::~CGimmickTimeBomb() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickTimeBomb::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    changeMotion(1, 0, FALSE, -1, 0, 0);
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    bodyCollOffBody(-1);
    rideModelEnableAllCollision(FALSE);
    return TRUE;
}

void CGimmickTimeBomb::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    m_Prize = m_Stat;
    switch (getID() % 10000U) {
    case 0x2C4:
    case 0x2CD:
        bodyCollOnBody(-1);
        rideModelEnableAllCollision(TRUE);
        break;
    case 0x20B:
    case 0x20C:
    case 0x20D:
    case 0x271:
        bodyCollOnBody(-1);
        break;
    case 0x2C6:
        if (m_Prize) {
            m_Kill = 1;
        } else {
            bodyCollOnBody(-1);
            rideModelEnableAllCollision(TRUE);
        }
        break;
    }
}

void CGimmickTimeBomb::beginStateMain() {
    if (m_Kill) out();
}

void CGimmickTimeBomb::stateMain() {
    switch (getID() % 10000U) {
    case 0x20B:
    case 0x20C:
    case 0x20D:
    case 0x271:
    case 0x2C4:
    case 0x2CD:
        clockMain();
        break;
    case 0x2C6:
        bombMain();
        break;
    }
}

void CGimmickTimeBomb::endStateMain() {}

void CGimmickTimeBomb::beginStateHitAttack() {
    switch (getID() % 10000U) {
    case 0x20B:
    case 0x20C:
    case 0x20D:
    case 0x271:
    case 0x2C4:
    case 0x2CD:
        changeMotion(2, 0, FALSE, -1, 0, 0);
        m_Stat = 1;
        saveStat(1);
        m_Step = 10;
        break;
    case 0x2C6:
        if (m_pAnim->getAnimID() == 1) {
            changeMotion(2, 0, FALSE, -1, 0, 0);
            m_Stat = 1;
            saveStat(1);
            m_Step = 10;
        }
        break;
    }
    changeGimmickState(0);
}

void CGimmickTimeBomb::stateHitAttack() {}

void CGimmickTimeBomb::endStateHitAttack() {}

void CGimmickTimeBomb::clockMain() {
    switch (m_Step) {
    case 0:
        break;
    case 10: {
        GimParam* param = getGimParam();
        if (param != NULL) m_pAttack->setAttackParam(param->AtkPow, (ATK_ATTR)param->AtkAttr);
        m_pAttack->setCollKind(0);
        if (!m_Prize) {
            m_Prize = 1;
            createPrize(0, 0);
        }
        m_Step = 20;
        break;
    }
    case 20:
        if (isMotionEnd()) {
            changeMotion(1, 0, FALSE, -1, 0, 0);
            m_Step = 0;
        }
        break;
    }
}

void CGimmickTimeBomb::bombMain() {
    switch (m_Step) {
    case 0:
        break;
    case 10:
        m_WaitCounter = 180.0f;
        m_Step = 20;
        break;
    case 20:
        m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) {
            changeMotion(3, 0, FALSE, -1, 0, 0);
            m_pAttack->setCollKind(0);
            createPrize(0, 0);
            m_Kill = 1;
            m_Step = 30;
        }
        break;
    case 30:
        out();
        break;
    }
}
// SOURCE DATA: .rodata
