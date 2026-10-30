#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/Gimmick/GimmickHitRepetition.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in GimmickWindowLid/Bell); which
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

#include "Game/Movement.h"
#include "Game/AttackMotion.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"

CGimmickHitRepetition::CGimmickHitRepetition(CTask* inParent, INT in_GimID) : CBaseGimmick(inParent), m_ID(in_GimID) {
    m_Step = 0;
    m_RepeatFlag = 0;
    m_ReloadWaitCounter = 0.0f;
    m_ReloadFlag = 0;
}

CGimmickHitRepetition::~CGimmickHitRepetition() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickHitRepetition::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    return TRUE;
}

void CGimmickHitRepetition::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat & 0x10) {
        changeMotion(0, 0, FALSE, -1, 0, 0);
        m_RepeatFlag = 1;
        if (m_ID == 0x136 || m_ID == 0x38A) bodyCollOffBody(1);
    } else {
        changeMotion(3, 0, FALSE, -1, 0, 0);
    }
    if (m_pAttack != NULL) {
        GimParam* param = getGimParam();
        if (param != NULL) m_pAttack->setAttackParam(param->AtkPow, (ATK_ATTR)param->AtkAttr);
    }
}

void CGimmickHitRepetition::beginStateMain() {}

void CGimmickHitRepetition::stateMain() {
    if (getGimmickFlag() & 1) {
        m_ReloadFlag = 1;
        m_ReloadWaitCounter = 60.0f;
    }
    if (m_ReloadWaitCounter != 0.0f) m_ReloadWaitCounter -= (FLOAT)getFrameRate() * 0.0200000014f;
    if (m_ReloadWaitCounter <= 0.0f && m_ReloadFlag) {
        m_ReloadFlag = 0;
        m_ReloadWaitCounter = 0.0f;
        m_pAttack->updateAttackID();
    }
    if ((m_ID == 0x136 || m_ID == 0x38A) && isRideOnPlayer()) {
        if (m_RepeatFlag && g_pPlayer->getPlayerState() != HANG) putPrize();
    }
}

void CGimmickHitRepetition::endStateMain() {}

void CGimmickHitRepetition::beginStateHitAttack() {
    if (m_ID != 0x136 && m_ID != 0x38A) putPrize();
    m_RepeatFlag ^= 1;
    m_Stat ^= 0x10;
    if (m_RepeatFlag) {
        changeMotion(2, 0, FALSE, -1, 0, 0);
        if (m_ID == 0x136 || m_ID == 0x38A) bodyCollOffBody(1);
    } else {
        changeMotion(1, 0, FALSE, -1, 0, 0);
        if (m_pAttack != NULL) {
            GimParam* param = getGimParam();
            if (param != NULL) m_pAttack->setAttackParam(param->AtkPow, (ATK_ATTR)param->AtkAttr);
        }
        if (m_ID == 0x136 || m_ID == 0x38A) bodyCollOnBody(1);
    }
    changeGimmickState(0);
}

void CGimmickHitRepetition::stateHitAttack() {}

void CGimmickHitRepetition::endStateHitAttack() {}

void CGimmickHitRepetition::putPrize() {
    if (!(m_Stat & 1)) {
        createPrize(0, 0);
        m_Stat |= 1;
        saveStat(m_Stat);
    }
}
// SOURCE DATA: .rodata
