#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "System/TaskMan.h"
#include "System/Task.h"
#include "Game/Player/FriendAriel.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in FriendAladdin). */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() {
    TRACE("");
}
inline void _emptyStringPlaceholder2() {
    TRACE("");
}

#include "Game/BodyCollision.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Movement.h"
#include "Game/LockOnManager.h"

static inline FLOAT Abs(FLOAT inVal) {
    FLOAT ret;
    asm volatile("abs.s %0, %1" : "=f"(ret) : "f"(inVal));
    return ret;
}

namespace Friend {

unsigned char g_ArielAttackID[4] = { 88, 109, 110, 0 };

CFriendAriel::CFriendAriel(CTask* inParent) : CFriend(inParent, 4, 0) {
}

CFriendAriel::~CFriendAriel() {
}

INT CFriendAriel::setSkill(INT inID) {
    if (m_iState != 1) return 0;
    switch (inID) {
    case 88:
        m_uiSkillID = 16;
        m_uiLevel = 0;
        changeState(2);
        break;
    case 109:
        m_uiSkillID = 17;
        m_uiLevel = 1;
        changeState(2);
        break;
    case 110:
        m_uiSkillID = 18;
        m_uiLevel = 2;
        changeState(2);
        break;
    default:
        setExit(0);
        break;
    }
    return 1;
}

void CFriendAriel::beginStateAction1() {
    m_iState = 2;
    m_iSubState = 1;
    m_uiAttackID = g_ArielAttackID[m_uiLevel];
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_iAttackPower = getPower(m_uiSkillID, 0) / 10;
    BC_PARAM*& bc = CardBattle::g_pBattleCardList;
    m_AtkAttr = bc[52].ATTR;
    m_iAttackNo = 0;
    changeMotion(4, 0, 1, -1, 0, 0, 0);
    homingPlayerTargetY(0, 0.1f);
    setLockon(m_hTarget);
    changeCamera(m_hTarget, 3, 0);
    setTimer(3000);
    m_pMove->enableGravity(0);
}

void CFriendAriel::stateAction1() {
    if (exeArielAttack()) setExit(0);
}

INT CFriendAriel::exeArielAttack() {
    HANDLE hTarget = m_hTarget;
    CEntity* pTarget = hTarget > 0 ? (CEntity*)CTaskMan::getTask(hTarget) : 0;
    FVECTOR aim;
    aim._q = 0;
    aim.w = 1.0f;
    if (!pTarget || !(pTarget->getFlag() & 0x10000)) {
        hTarget = LockOnManager::CLockOnManager::getLockOnObject(*this);
        pTarget = hTarget > 0 ? (CEntity*)CTaskMan::getTask(hTarget) : 0;
    }
    if (pTarget) pTarget->getLockOnPos(aim);
    FVECTOR vel;
    HANDLE hLockon = m_hLockon;
    CEntity* pLockon = 0;
    FVECTOR loc;
    loc.init();
    switch (m_iSubState) {
    case 3:
    case 4:
    case 5:
        pLockon = hLockon > 0 ? (CEntity*)CTaskMan::getTask(hLockon) : 0;
        if (!pLockon || !(pLockon->getFlag() & 0x10000)) {
            hLockon = LockOnManager::CLockOnManager::getRandomLockOnObject();
            pLockon = hLockon > 0 ? (CEntity*)CTaskMan::getTask(hLockon) : 0;
        }
        if (pLockon) pLockon->getLockOnPos(loc);
        break;
    }
    switch (m_iSubState) {
    case 1:
        m_hTarget = hTarget;
        m_vAimPos = aim;
        m_pMove->homingY(m_vAimPos, 0.1f);
        m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, 0.0f, 0.4f, getFrameRateF());
        m_pMove->calcVelocity(m_fMoveSpdXZ);
        if (isMotionEnd() || getTimer() <= 0) {
            m_iSubState = 2;
            changeMotion(5, 0, 1, -1, 0, 0, 0);
            m_iAttackNo++;
            m_uiFlag &= ~0x1fc00;
            setTimer(6000);
            m_fMoveSpdXZ = 0.0f;
            break;
        }
        return 0;
    case 2: {
        if (getTimer() > 1500) {
            m_hTarget = hTarget;
            m_vAimPos = aim;
            if (!(m_uiFlag & 0x800) && pTarget) m_pMove->homingY(m_vAimPos, 0.1f);
            m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, m_pAtkData->MAX_SPEED / 60.0f, 0.4f, getFrameRateF());
            m_pMove->calcVelocity(m_fMoveSpdXZ);
        } else {
            m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, 0.0f, 0.04f, getFrameRateF());
            m_pMove->calcVelocity(m_fMoveSpdXZ);
        }
        FVECTOR pos;
        getLockOnPos(pos);
        vel = getVel();
        FLOAT ret = Abs(m_vAimPos.y - pos.y);
        if (ret > 0.3f) {
            if (m_vAimPos.y < pos.y) {
                vel.y = -0.1f;
            } else if (pos.y < m_vAimPos.y) {
                vel.y = 0.1f;
            } else {
                vel.y = 0.0f;
            }
        }
        setVel(vel);
        checkChainTrg();
        if (getTimer() <= 4500) {
            m_uiFlag |= 0x800;
            INT mode = m_iAttackNo < m_pAtkData->WORK1 ? 1 : 3;
            if (entryChainTrg(mode)) {
                m_hLockon = LockOnManager::CLockOnManager::getRandomLockOnObject();
                setLockon(m_hLockon);
                changeCamera(m_hLockon, 4, 0);
            }
        }
        if (isSucceedChain()) goto succeed;
        if ((m_uiFlag & 0x400) && (m_pMove->getCollisionFlag() & 0xa)) {
            m_iSubState = 3;
            changeMotion(1, 0, 1, -1, 0, 0, 0);
            return 0;
        }
        if ((m_uiFlag & 0x40000) && (m_pMove->getCollisionFlag() & 0xa)) {
            setTimer(getTimer() - getFrameRate());
        }
        goto timeout;
    }
    case 3:
        m_hTarget = hTarget;
        m_vAimPos = aim;
        m_pMove->homingY(m_vAimPos, 0.1f);
        m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, 0.0f, 0.2f, getFrameRateF());
        m_pMove->calcVelocity(m_fMoveSpdXZ);
        checkChainTrg();
        if (isSucceedChain()) {
        succeed:
            m_iSubState = 4;
            break;
        }
    timeout:
        if (getTimer() <= 0) {
            setPushon(0);
            return 1;
        }
        return 0;
    case 4:
        m_hLockon = hLockon;
        changeCamera(hLockon, 4, 0);
        setLockon(m_hLockon);
        if (hLockon > 0) m_vAimPos = loc;
        if (getTimer() > 1500) {
            if (m_uiAnimID == 5) {
                m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, m_pAtkData->MAX_SPEED / 60.0f, 0.4f, getFrameRateF());
            } else {
                setTimer(1500);
            }
        } else {
            m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, 0.0f, 0.04f, getFrameRateF());
        }
        m_pMove->calcVelocity(m_fMoveSpdXZ);
        vel = getVel();
        vel.y *= 0.8f;
        setVel(vel);
        if (m_pMove->getCollisionFlag() & 0xa) setTimer(getTimer() - getFrameRate());
        if (getTimer() < 1500) {
            if (m_iAttackNo < m_pAtkData->WORK1) {
                m_iSubState = 1;
                changeMotion(4, 0, 1, -1, 0, 0, 0);
                setTimer(1500);
                m_hTarget = m_hLockon;
                setLockon(m_hTarget);
                return 0;
            }
            changeCamera(m_hTarget, 3, 0);
            m_iSubState = 5;
            changeMotion(6, 0, 1, -1, 0, 0, 0);
            m_fDist = m_pAtkData->RADIUS * 0.1f;
            m_fMoveSpdXZ = 0.0f;
            clearSpeed();
            m_hTarget = m_hLockon;
            setLockon(m_hTarget);
            setTimer(4500);
            break;
        }
        return 0;
    case 5:
        if (getTimer() > 1500) {
            m_hLockon = hLockon;
            setLockon(hLockon);
            m_hTarget = m_hLockon;
            if (hLockon > 0) m_vAimPos = loc;
            m_pMove->homingY(m_vAimPos, 0.1f);
        }
        m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, m_pAtkData->MAX_SPEED / 60.0f, 0.2f, getFrameRateF());
        m_pMove->calcVelocity(m_fMoveSpdXZ);
        if (!pLockon || calcDistance(pLockon) <= m_fDist || !(pLockon->getFlag() & 0x10000) || getTimer() <= 0) {
            m_iSubState = 6;
            m_iAttackPower = getPower(m_uiSkillID, 1) / 10;
            changeMotion(7, 0, 1, -1, 0, 0, 0);
            m_fMoveSpdXZ = 0.0f;
            clearSpeed();
            m_uiFlag &= ~0x1fc00;
            break;
        }
        if (m_pMove->getCollisionFlag() & 0xa) {
            setTimer(getTimer() - getFrameRate());
            break;
        }
        return 0;
    case 6:
        if (isMotionEnd()) {
            changeMotion(1, 0, 1, -1, 0, 0, 0);
            return 1;
        }
        break;
    }
    return 0;
}

} // namespace Friend

// SOURCE DATA: .data .rodata
