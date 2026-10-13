#define TASK_H_NO_SLC_OVERFLOW_MSG
#include "Game/Gimmick/GimmickOnceReaction.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Anim.h"
#include "Game/Movement.h"
#include "Game/AttackMotion.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"

CGimmickOnceReaction::CGimmickOnceReaction(CTask* inParent, INT in_GimID) : CBaseGimmick(inParent) {
    m_ID = in_GimID;
    m_Step = 0;
    m_OnceFlag = 0;
    m_Drop = 0;
}

CGimmickOnceReaction::~CGimmickOnceReaction() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickOnceReaction::initalize(INT inGroupID, INT inRsrcID) {
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

void CGimmickOnceReaction::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat != 0) {
        changeMotion(3, 0, FALSE, -1, 0, 0);
        switch (m_ID) {
        case 0x19A:
        case 0x19B:
            changeMotion(3, 0, FALSE, 0, 0, 0);
            break;
        case 0x2C5:
        case 0x2C9:
            bodyCollOnBody(1);
            break;
        case 0x43F:
            setEntityFlag(0, 0x10000);
            bodyCollOffBody(0);
            changeMotion(3, 0, FALSE, 0, 0, 0);
            break;
        case 0x388:
        case 0x51C:
        case 0x51F:
        case 0x527:
        case 0x52A:
            setEntityFlag(0, 0x10000);
            bodyCollOffBody(-1);
            bodyCollOff(-1);
            changeMotion(3, 0, FALSE, 0, 0, 0);
            rideModelEnableChipCollision(0, FALSE);
            rideModelEnableChipCollision(1, TRUE);
            break;
        }
    } else {
        switch (m_ID) {
        case 0x2C5:
        case 0x2C9:
            bodyCollOffBody(1);
            rideModelEnableAllCollision(FALSE);
            break;
        }
    }
}

void CGimmickOnceReaction::beginStateMain() {}

void CGimmickOnceReaction::stateMain() {
    switch (m_ID) {
    case 0x19A:
    case 0x19B:
        flowerMain();
        break;
    case 0x2C5:
    case 0x2C9:
        shellMain();
        break;
    case 0x43F:
        tuboMain();
        break;
    case 0x388:
    case 0x51C:
    case 0x51F:
    case 0x527:
    case 0x52A:
        GimColChangeMain();
        break;
    default:
        defaultMain();
        break;
    }
}

void CGimmickOnceReaction::endStateMain() {}

void CGimmickOnceReaction::beginStateHitAttack() {
    if (m_Stat != 1) {
        changeMotion(2, 0, FALSE, 0, 0, 0);
        if (m_pAttack != NULL) m_pAttack->setCollKind(0);
        m_Stat = 1;
        saveStat(1);
        m_Step = 10;
    }
    changeGimmickState(0);
}

void CGimmickOnceReaction::stateHitAttack() {}

void CGimmickOnceReaction::endStateHitAttack() {}

void CGimmickOnceReaction::defaultMain() {
    switch (m_Step) {
    case 0:
        break;
    case 10:
        createPrize(0, 0);
        m_Step = 20;
        break;
    case 20:
        if (isMotionEnd()) {
            changeMotion(3, 0, FALSE, 0, 0, 0);
            m_Step = 50;
        }
        break;
    }
}

void CGimmickOnceReaction::flowerMain() {
    switch (m_Step) {
    case 0:
        break;
    case 10:
        if (m_pAnim->getCurrentTqCrd() >= 1000) {
            createPrize(0, 0);
            m_Step = 20;
        }
        break;
    case 20:
        if (isMotionEnd()) {
            changeMotion(3, 0, FALSE, 0, 0, 0);
            m_Step = 50;
        }
        break;
    }
}

void CGimmickOnceReaction::shellMain() {
    switch (m_Step) {
    case 0:
        break;
    case 10:
        if (m_pAnim->getCurrentTqCrd() >= 300) {
            m_Drop = createPrize(0, 0);
            m_Step = 15;
        }
        break;
    case 15:
        if (m_pAnim->getCurrentTqCrd() >= 1250) {
            if (m_Drop == 3) createPrize(0, 1);
            else m_Drop = createPrize(0, 0);
            m_Step = 20;
        }
        break;
    case 20:
        if (m_pAnim->getCurrentTqCrd() >= 2000) {
            if (m_Drop == 3) createPrize(0, 1);
            else m_Drop = createPrize(0, 0);
            m_Step = 25;
        }
        break;
    case 25:
        if (m_pAnim->getCurrentTqCrd() >= 2850) {
            if (m_Drop == 3) createPrize(0, 1);
            else m_Drop = createPrize(0, 0);
            m_Step = 30;
        }
        break;
    case 30:
        if (m_pAnim->getCurrentTqCrd() >= 3700) {
            if (m_Drop == 3) createPrize(0, 1);
            else m_Drop = createPrize(0, 0);
            m_Step = 35;
        }
        break;
    case 35:
        if (m_pAnim->getCurrentTqCrd() >= 4500) {
            if (m_Drop == 3) createPrize(0, 1);
            else m_Drop = createPrize(0, 0);
            m_Step = 40;
        }
        break;
    case 40:
        if (m_pAnim->getCurrentTqCrd() >= 5250) {
            if (m_Drop == 3) createPrize(0, 1);
            else m_Drop = createPrize(0, 0);
            m_Step = 45;
        }
        break;
    case 45:
        if (m_pAnim->getCurrentTqCrd() >= 7000) {
            if (m_Drop == 3) createPrize(0, 1);
            else m_Drop = createPrize(0, 0);
            m_Step = 50;
        }
        break;
    case 50:
        if (isMotionEnd()) {
            changeMotion(3, 0, FALSE, 0, 0, 0);
            bodyCollOnBody(1);
            rideModelEnableAllCollision(TRUE);
            m_Step = 55;
        }
        break;
    }
}

void CGimmickOnceReaction::tuboMain() {
    switch (m_Step) {
    case 0:
        break;
    case 10:
        createPrize(0, 0);
        bodyCollOffBody(0);
        setEntityFlag(0, 0x10000);
        m_Step = 20;
        break;
    case 20:
        if (isMotionEnd()) {
            changeMotion(3, 0, FALSE, 0, 0, 0);
            m_Step = 50;
        }
        break;
    }
}

void CGimmickOnceReaction::GimColChangeMain() {
    switch (m_Step) {
    case 0:
        break;
    case 10:
        createPrize(0, 0);
        bodyCollOffBody(-1);
        bodyCollOff(-1);
        setEntityFlag(0, 0x10000);
        m_Step = 20;
        break;
    case 20:
        if (isMotionEnd()) {
            rideModelEnableChipCollision(0, FALSE);
            rideModelEnableChipCollision(1, TRUE);
            changeMotion(3, 0, FALSE, 0, 0, 0);
            m_Step = 50;
        }
        break;
    }
}
// SOURCE DATA: .rodata
