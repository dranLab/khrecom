#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Task.h"
#include "System/TaskMan.h"
#include "Game/Player/FriendPan.h"
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
#include "Game/Player/Player.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Movement.h"
#include "Game/EntityFactory.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"
#include "System/Math3D.h"
#include "System/Rnd.h"


namespace Friend {

unsigned char g_PanAttackID[4] = { 90, 111, 112, 0 };

CFriendPan::CFriendPan(CTask* inParent) : CFriend(inParent, 6, 0) {
    m_nLogCount = 0;
    m_nLogCountMax = 0;
    m_pTrgLog = 0;
}

CFriendPan::~CFriendPan() {
    if (m_pTrgLog) com_free(m_pTrgLog);
    m_pTrgLog = 0;
}

INT CFriendPan::setSkill(INT inID) {
    if (m_iState != 1) return 0;
    switch (inID) {
    case 90:
        m_uiSkillID = 19;
        break;
    case 111:
        m_uiSkillID = 20;
        break;
    case 112:
        m_uiSkillID = 21;
        break;
    default:
        setExit(0);
        return 1;
    }
    m_uiLevel = m_uiSkillID - 19;
    changeState(2);
    return 1;
}

void CFriendPan::beginStateAction1() {
    m_iState = 2;
    m_uiAttackID = g_PanAttackID[m_uiLevel];
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_iAttackPower = getPower(m_uiSkillID, 0) / 10;
    BC_PARAM*& bc = CardBattle::g_pBattleCardList;
    m_AtkAttr = bc[54].ATTR;
    m_iAttackNo = 0;
    changeMotion(7, 0, 1, -1, 0, 0, 0);
    homingPlayerTargetY(0, 0.1f);
    setLockon(m_hTarget);
    changeCamera(m_hTarget, 4, 0);
    setTimer(m_pAtkData->MOVE_START_FRAME * 100);
    m_fDist = m_pAtkData->RADIUS * 0.1f;
    m_fMoveSpdXZ = 0.0f;
    clearSpeed();
}

void CFriendPan::stateAction1() {
    homingPlayerTargetY(m_hTarget, 0.1f);
    FVECTOR rot = getRot();
    CEntity* pTarget = m_hTarget > 0 ? (CEntity*)CTaskMan::getTask(m_hTarget) : 0;
    m_pMove->homing(pTarget, 0.1f);
    m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, m_pAtkData->MAX_SPEED / 60.0f, 0.2f, getFrameRateF());
    m_pMove->calcVelocity(m_fMoveSpdXZ);
    setRot(rot);
    FLOAT dir = 0.0f;
    if (pTarget) {
        FLOAT r = calcDirection(pTarget->getPos()) - rot.y;
        if (r < -3.1415927f) {
            r += 6.2831855f;
        } else if (r > 3.1415927f) {
            r -= 6.2831855f;
        }
        dir = r;
    }
    INT anim;
    INT cur = m_uiAnimID;
    if (dir < -0.7853982f) {
        anim = 10;
    } else if (dir > 0.7853982f) {
        anim = 9;
    } else {
        anim = 7;
    }
    if (anim != cur) {
        if (anim != 7) {
            changeMotion(anim, 0, 1, -1, 0, 0, 0);
        } else {
            changeMotion(7, 1100, 1, -1, 0, 0, 0);
        }
    }
    if (m_pPlayer->isBattleFlag(4)) {
        setExit(0);
    } else if (getTimer() <= 0 || (pTarget && calcDistance(pTarget) < m_fDist)) {
        changeState(3);
    }
}

void CFriendPan::beginStateAction2() {
    m_iState = 3;
    changeMotion(4, 0, 1, -1, 0, 0, 0);
    m_fMoveSpdXZ = m_pAtkData->MAX_SPEED / 60.0f;
    m_pMove->calcVelocity(m_fMoveSpdXZ);
}

void CFriendPan::stateAction2() {
    homingPlayerTargetY(m_hTarget, 0.1f);
    if (m_hTarget != m_hLockon) {
        setLockon(m_hTarget);
        changeCamera(m_hTarget, 4, 0);
    }
    if (m_hTarget > 0) CTaskMan::getTask(m_hTarget);
    m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, 0.0f, 0.01f, getFrameRateF());
    m_pMove->calcVelocity(m_fMoveSpdXZ);
    if (isMotionEnd()) {
        m_fMoveSpdXZ = 0.0f;
        clearSpeed();
        changeState(4);
    }
}

#line 261
void CFriendPan::beginStateAction3() {
    INT i;
    m_iState = 4;
    changeMotion(5, 0, 1, -1, 0, 0, 0);
    setTimer(m_pAtkData->MOVE_END_FRAME * 1000);
    m_nLogCount = 0;
    m_nLogCountMax = m_pAtkData->WORK0;
    m_pTrgLog = new KH_AT("(268)") TIMEQ[m_nLogCountMax];
    KH_DEBUG(if (!m_pTrgLog) AssertApp(__FILE__, __LINE__, "m_pTrgLog", 0);)
    for (i = 0; i < m_nLogCountMax; i++) {
        m_pTrgLog[i] = getTimer() + m_pAtkData->TRG_FRAME * 100;
    }
    m_iAttackNo = 0;
    m_fMoveSpdXZ = 0.033333335f;
}

void CFriendPan::stateAction3() {
    if (m_uiAnimID == 8 && isMotionEnd()) {
        setExit(0);
        return;
    }
    homingPlayerTargetY(m_hTarget, 0.1f);
    if (m_hTarget != m_hLockon) {
        setLockon(m_hTarget);
        changeCamera(m_hTarget, 4, 0);
    }
    CEntity* pTarget = m_hTarget > 0 ? (CEntity*)CTaskMan::getTask(m_hTarget) : 0;
    FVECTOR rot = getRot();
    if (pTarget && calcDistance(pTarget) > 1.0f) m_pMove->homing(pTarget, 0.1f);
    m_pMove->calcVelocity(m_fMoveSpdXZ);
    setRot(rot);
    UINT flag = m_uiFlag;
    if (!(flag & 0x400)) {
        if (m_uiAnimID == 5) {
            if (isMotionEnd()) {
                changeMotion(6, 0, 1, -1, 0, 0, 0);
                setAnimSpeed(m_pAtkData->WORK1 * 0.1f);
                setPushon(2);
                m_uiFlag &= ~0x1fc00;
                setTimer(m_pAtkData->MOVE_END_FRAME * 1000);
            }
            return;
        }
        if (isButtonTrg(0x10, 0)) {
            TIMEQ old = m_pTrgLog[m_nLogCount];
            m_pTrgLog[m_nLogCount] = getTimer();
            if (m_pAtkData->TRG_FRAME * 100 >= old - m_pTrgLog[m_nLogCount]) {
                m_uiFlag |= 0x400;
                setPushon(0);
                changeMotion(5, 0, 1, -1, 1, 0, 0);
                setAnimSpeed(m_pAtkData->WORK2 * 0.1f);
                m_fMoveSpdXZ = 0.06666667f;
            }
            m_nLogCount = (m_nLogCount + 1) % m_nLogCountMax;
        }
    } else {
        if (m_uiAnimID == 5) {
            if (isMotionEnd()) {
                changeMotion(6, 0, 1, -1, 1, 0, 0);
                setAnimSpeed(m_pAtkData->WORK2 * 0.1f);
            }
            return;
        }
        if (flag & 2) {
            if (++m_iAttackNo < 4) {
                PRIZE_PARAM param;
                param.uiFlag = 0;
                param.uiPrizeFlag = 0;
                getWeaponTopPos(param.vPos);
                param.PrizeKind = 212;
                param.fSpeed = 0.25f;
                FLOAT rx = (-70.0f - RND::genrand_real1() * 10.0f) * 0.017453292f;
                FLOAT ry = RND::genrand_real1() * 360.0f * 0.017453292f;
                param.vRot.set(rx, ry, 0.0f);
                Math3D::LimitPIVector(param.vRot);
                CEntityFactory::getInstance()->createPrize(param, 0);
            }
        }
        if (m_uiAnimID == 6 && isMotionEnd()) m_iAttackNo = 0;
    }
    if (m_pPlayer->isBattleFlag(4)) setTimer(0);
    if (getTimer() <= 0 && m_uiAnimID != 8) {
        setPushon(0);
        changeMotion(8, 0, 1, -1, 0, 0, 0);
        setAnimSpeed(1.0f);
        m_fMoveSpdXZ = 0.0f;
        clearSpeed();
    }
}

} // namespace Friend

// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data
