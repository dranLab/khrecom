
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Gimmick/GimmickSteamTrap.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in GimmickWindowLid/Bell); which
 * headers they come from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() {
    TRACE("");
}
inline void _emptyStringPlaceholder2() {
    TRACE("");
}

#include "System/Graphic/modeldata.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Movement.h"
#include "Game/AttackMotion.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"

CGimmickSteamTrap::CGimmickSteamTrap(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_RepeatFlag = 0;
    m_Wait = 0.0f;
    m_MotWait = 0.0f;
    m_AtkWait = 0.0f;
}

CGimmickSteamTrap::~CGimmickSteamTrap() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickSteamTrap::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    return TRUE;
}

void CGimmickSteamTrap::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    else m_Stat = 1;
    changeMotion(1, 0, FALSE, 0, 0, 0);
    if (m_pAttack != NULL) {
        GimParam* param = getGimParam();
        if (param != NULL) m_pAttack->setAttackParam(param->AtkPow, (ATK_ATTR)param->AtkAttr);
    }
}

void CGimmickSteamTrap::beginStateMain() {}

void CGimmickSteamTrap::stateMain() {
    if (m_MotWait != 0.0f) {
        m_MotWait -= (FLOAT)getFrameRate() * 0.0200000014f;
        if (isMotionEnd() && m_pAnim->getAnimID() != 1 && m_MotWait <= 0.0f) {
            m_MotWait = 0.0f;
            m_Step = 0;
            changeMotion(1, 0, FALSE, 0, 0, 0);
        }
    }
    if (getGimmickFlag() & 1) {
        m_Wait = 60.0f;
        setGimmickFlag(1, FALSE);
    }
    if (m_Wait != 0.0f) {
        m_Wait -= (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_Wait <= 0.0f) {
            m_Wait = 0.0f;
            m_pAttack->updateAttackID();
        }
    }
    if (m_pAnim->getAnimID() != 2) {
        switch (m_Step) {
        case 0:
            m_Step = 10;
            m_SteamWait = 240.0f;
            break;
        case 10:
            m_SteamWait -= (FLOAT)getFrameRate() * 0.0200000014f;
            if (m_SteamWait <= 0.0f) {
                changeMotion(0, 0, FALSE, 0, 0, 0);
                setGimmickFlag(0x10, TRUE);
                m_Step = 20;
                m_SteamWait = 120.0f;
            }
            break;
        case 20:
            m_SteamWait -= (FLOAT)getFrameRate() * 0.0200000014f;
            if (m_SteamWait <= 0.0f) {
                changeMotion(1, 0, FALSE, 0, 0, 0);
                m_Step = 0;
                m_AtkWait = 60.0f;
            }
            break;
        }
    }
    if (m_AtkWait != 0.0f) {
        m_AtkWait -= (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_AtkWait <= 0.0f) {
            m_AtkWait = 0.0f;
            setGimmickFlag(0x10, FALSE);
        }
    }
}

void CGimmickSteamTrap::endStateMain() {}

void CGimmickSteamTrap::beginStateHitAttack() {
    if (m_Stat == 0) {
        createPrize(0, 0);
        m_Stat = 1;
        saveStat(1);
    }
    changeMotion(2, 0, FALSE, -1, 0, 0);
    m_MotWait = 300.0f;
    changeGimmickState(0);
}

void CGimmickSteamTrap::stateHitAttack() {}

void CGimmickSteamTrap::endStateHitAttack() {}
// SOURCE DATA: .rodata
