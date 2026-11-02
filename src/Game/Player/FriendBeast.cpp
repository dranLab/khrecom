#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Task.h"
#include "System/TaskMan.h"
#include "Game/Player/FriendBeast.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in FriendAladdin). */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}
inline void _emptyStringPlaceholder2() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}

#include "Game/BodyCollision.h"
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/AttackMotion.h"
#include "Game/Movement.h"

namespace Friend {

unsigned char g_BeastAttackID[4] = { 91, 113, 114, 0 };

static inline BOOL _isCatchable(CBattleEnemy* pTarget) {
    if (pTarget->isBtlEnemyFlag(0x80000)) return 0;
    if (pTarget->isBtlEnemyFlag(0x100000)) return 0;
    return 1;
}

CFriendBeast::CFriendBeast(CTask* inParent) : CFriend(inParent, 7, 0) {
    m_fCamHeight = 1.0f;
    m_fBodyRviseCoefficient = 0.25f;
}

CFriendBeast::~CFriendBeast() {
}

INT CFriendBeast::setSkill(INT inID) {
    if (m_iState != 1) return 0;
    switch (inID) {
    case 91:
        m_uiSkillID = 22;
        break;
    case 113:
        m_uiSkillID = 23;
        break;
    case 114:
        m_uiSkillID = 24;
        break;
    default:
        setExit(0);
        return 1;
    }
    m_uiLevel = m_uiSkillID - 22;
    changeState(2);
    return 1;
}

void CFriendBeast::beginStateAction1() {
    m_iAttackNo = 0;
    m_iState = 2;
    m_uiAttackID = g_BeastAttackID[m_uiLevel];
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_iAttackPower = getPower(m_uiSkillID, 0) / 10;
    BC_PARAM*& bc = CardBattle::g_pBattleCardList;
    m_AtkAttr = bc[55].ATTR;
    changeMotion(6, 0, 1, -1, 0, 0, 0);
    homingPlayerTargetY(0, 0.1f);
    setLockon(m_hTarget);
    changeCamera(m_hTarget, 4, 0);
    m_tqSubTimer = 0;
    m_iSubState = 0;
}

void CFriendBeast::stateAction1() {
    CBattleEnemy* pTarget = m_hTarget > 0 ? (CBattleEnemy*)CTaskMan::getTask(m_hTarget) : 0;
    INT frame = getMotionCurrentTime() / 100;
    if (m_iSubState == 0) {
        if (m_pAttack) m_pAttack->setEnemyPos(m_pPlayer->getPos());
        if (m_uiFlag & 0x800) {
            if (pTarget && _isCatchable(pTarget) && !pTarget->isBtlEnemyFlag(0x20000)) {
                setTimer(9000);
                m_iSubState++;
                m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), m_hTarget);
                m_pPlayer->getPlayerCamera()->changeCamMode(4, 0);
                return;
            }
            m_uiFlag = (INT)m_uiFlag & ~0x800;
        }
        if (m_tqSubTimer <= m_pAtkData->MOVE_START_FRAME * 100 || m_pAtkData->MOVE_END_FRAME * 100 < m_tqSubTimer) {
            homingPlayerTargetY(m_hTarget, 0.3f);
            if (m_hTarget != m_hLockon) {
                setLockon(m_hTarget);
                changeCamera(m_hTarget, 4, 0);
            }
        }
        if (isMotionEnd()) setExit(0);
        m_tqSubTimer += getFrameRate();
        return;
    }
    if (pTarget) {
        pTarget->getLockOnPos(m_vAimPos);
        if (checkChainTrg()) {
            m_pPlayer->setAttack_inFriend(115, m_uiLevel, 0);
            m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), m_hTarget);
            m_pPlayer->getPlayerCamera()->changeCamMode(3, 0);
        }
        if (pTarget->getFlag() & 0x10000) {
            if (pTarget->getVel().y <= 0.0f && entryChainTrg(1)) setTimer(3000);
        }
        if (m_uiAnimID == 6 && (frame <= m_pAtkData->MOVE_START_FRAME || m_pAtkData->MOVE_END_FRAME < frame)) {
            m_pMove->homingY(m_vAimPos, 0.3f);
        }
        if (isSucceedChain()) {
            if (m_uiFlag & 0x8000) {
                setPushon(0);
                setLockon(0);
                setExit(0);
                return;
            }
            if (m_uiFlag & 0x4000) {
                if (m_iAttackNo < m_uiLevel) {
                    changeState(3);
                } else {
                    changeState(4);
                }
                return;
            }
        }
    } else {
        m_pPlayer->setPlayerFlag(0, 0x20000);
        setPushon(0);
        if (!(m_uiFlag & 0x400) && m_uiAnimID == 1) setTimer(0);
    }
    if (isMotionEnd() && m_uiAnimID != 1) changeMotion(1, 0, 1, -1, 0, 0, 0);
    if (getTimer() <= 0) {
        setPushon(0);
        setLockon(0);
        setExit(0);
    }
}

void CFriendBeast::beginStateAction2() {
    m_iState = 3;
    m_iAttackPower = getPower(m_uiSkillID, 1) / 10;
    changeMotion(7, 0, 1, -1, 0, 0, 0);
    m_iAttackNo++;
    m_uiFlag &= ~0x1fc00;
    setLockon(m_hTarget);
    changeCamera(m_hTarget, 4, 0);
    m_tqSubTimer = 0;
    m_uiFlag = (INT)m_uiFlag & ~8;
    m_iSubState = 0;
    setTimer(9000);
}

void CFriendBeast::stateAction2() {
    CBattleEnemy* pTarget = m_hTarget > 0 ? (CBattleEnemy*)CTaskMan::getTask(m_hTarget) : 0;
    if (pTarget) {
        pTarget->getLockOnPos(m_vAimPos);
        if (m_iSubState == 0) {
            if (m_uiFlag & 0x800) {
                if (_isCatchable(pTarget) && !pTarget->isBtlEnemyFlag(0x20000)) {
                    setTimer(9000);
                    m_iSubState++;
                    m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), m_hTarget);
                    m_pPlayer->getPlayerCamera()->changeCamMode(4, 0);
                    return;
                }
                m_uiFlag = (INT)m_uiFlag & ~0x800;
            }
            if (!(m_pAtkData->MOVE_START_FRAME * 100 < m_tqSubTimer)) m_pMove->homingY(m_vAimPos, 0.1f);
            if (m_pAttack) m_pAttack->setEnemyPos(m_pPlayer->getPos());
            if (isMotionEnd()) setExit(0);
            m_tqSubTimer += getFrameRate();
            return;
        }
        INT frame = getMotionCurrentTime() / 100;
        if (checkChainTrg()) {
            m_pPlayer->setAttack_inFriend(115, m_uiLevel, 0);
            m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), m_hTarget);
            m_pPlayer->getPlayerCamera()->changeCamMode(3, 0);
        }
        if (pTarget->getFlag() & 0x10000) {
            if (pTarget->getVel().y <= 0.0f && entryChainTrg(1)) setTimer(3000);
        }
        if (isSucceedChain()) {
            if (m_uiFlag & 0x8000) {
                setPushon(0);
                setLockon(0);
                setExit(0);
                return;
            }
            if (m_uiFlag & 0x4000) {
                if (m_iAttackNo < m_uiLevel) {
                    changeState(3);
                } else {
                    changeState(4);
                }
                return;
            }
        }
    } else {
        m_pPlayer->setPlayerFlag(0, 0x20000);
        setPushon(0);
        if (!(m_uiFlag & 0x400) && m_uiAnimID == 1) setTimer(0);
    }
    if (isMotionEnd() && m_uiAnimID != 1) changeMotion(1, 0, 1, -1, 0, 0, 0);
    if (getTimer() <= 0) {
        setPushon(0);
        setLockon(0);
        setExit(0);
    }
}

void CFriendBeast::beginStateAction3() {
    m_iState = 4;
    m_iAttackPower = getPower(m_uiSkillID, 2) / 10;
    changeMotion(8, 0, 1, -1, 0, 0, 0);
    m_iAttackNo++;
    m_uiFlag &= ~0x1fc00;
    setLockon(m_hTarget);
    changeCamera(m_hTarget, 4, 0);
    m_tqSubTimer = 0;
    m_uiFlag = (INT)m_uiFlag & ~8;
}

void CFriendBeast::stateAction3() {
    CBattleEnemy* pTarget = m_hTarget > 0 ? (CBattleEnemy*)CTaskMan::getTask(m_hTarget) : 0;
    if (pTarget) {
        pTarget->getLockOnPos(m_vAimPos);
        if (!(m_pAtkData->MOVE_START_FRAME * 100 < m_tqSubTimer)) m_pMove->homingY(m_vAimPos, 0.1f);
        m_tqSubTimer += getFrameRate();
    }
    if (isMotionEnd()) {
        setPushon(0);
        setLockon(0);
        setExit(0);
    }
}

} // namespace Friend

// SOURCE DATA: .data .rodata
