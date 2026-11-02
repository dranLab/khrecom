#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Task.h"
#include "System/TaskMan.h"
#include "Game/Player/SummonBambi.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in FriendAladdin). */
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
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Movement.h"
#include "Game/LockOnManager.h"
#include "Game/EntityFactory.h"
#include "System/Math3D.h"
#include "System/Rnd.h"
#include "include_asm.h"

extern "C" {
float sinf(float);
float cosf(float);
}

namespace Summon {

static LPCSTR g_szDpxSumBanbiB = "sum_banbi_b";

unsigned char g_BambiAttackID[4] = { 124, 133, 134, 135 };

CSummonBambi::CSummonBambi(CTask* inParent) : CSummon(inParent, 3) {
}

CSummonBambi::~CSummonBambi() {
}

void CSummonBambi::beginStateAppear() {
    m_iState = 0;
    m_uiAnimID = m_uiStartAnimID = 2;
    setEntityFlag(0, 1);
    changeMotion(2, 0, 1, 0, 0, 0, 0);
    updateCoord();
    coordFix();
    setAppearEffect();
    bodyCollOnBody(-1);
}

INT CSummonBambi::setSkill(INT inID) {
    if (m_iState != 1) return 0;
    switch (inID) {
    case 124:
        m_uiSkillID = 7;
        break;
    case 133:
        m_uiSkillID = 8;
        break;
    case 134:
        m_uiSkillID = 9;
        break;
    case 135:
        m_uiSkillID = 10;
        m_uiLevel = 3;
        changeState(3);
        return 1;
    default:
        setExit(0);
        return 1;
    }
    m_uiLevel = m_uiSkillID - 7;
    changeState(2);
    return 1;
}

void CSummonBambi::beginStateAction1() {
    m_iState = 2;
    m_uiAttackID = g_BambiAttackID[m_uiLevel];
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_iAttackPower = getPower(m_uiSkillID, 0) / 10;
    if (m_uiLevel == 0) {
        m_AtkAttr = (ATK_ATTR)CardBattle::g_pBattleCardList[37].ATTR;
    } else {
        m_AtkAttr = (ATK_ATTR)CardBattle::g_pStockSkillList[m_uiLevel + 55].ATTR;
    }
    CardBattle::CCardBattle::calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, m_uiUpKind);
    switch (m_pAtkData->DAMAGE) {
    case 10:
        m_uiKind = 2;
        break;
    case 7:
        m_uiKind = 1;
        break;
    default:
        m_uiKind = 0;
        break;
    }
    changeMotion(4, 0, 1, -1, 0, 0, 0);
    m_uiFlag &= 0xFFFF03FF;
    m_iAttackNo++;
    homingPlayerTargetY(0, 0.1f);
    m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), getHandle());
    m_pPlayer->getPlayerCamera()->changeCamMode(4, 0);
    clearSpeed();
    m_fMoveSpdXZ = 0.0f;
    m_pMove->enableGravity(0);
    m_uiFlag = ((INT)m_uiFlag & ~4) | 0x80;
    setTimer(9000);
}

void CSummonBambi::stateAction1() {
    FVECTOR vel;
    FLOAT fMoveSpdY;
    INT frame = getMotionCurrentTime() / 50;
    vel = m_pMove->getVel();
    switch (m_uiAnimID) {
    case 4:
        if (frame <= m_pAtkData->MOVE_START_FRAME) {
            homingPlayerTargetY(0, 0.3f);
            break;
        }
        if (!(m_uiFlag & 4)) {
            FLOAT& spd = m_fMoveSpdXZ;
            spd = m_pAtkData->MAX_SPEED / 60.0f;
            vel.y = PL::g_fAntiGravity * m_pAtkData->WORK0;
            m_pMove->setVel(vel);
            m_pMove->enableGravity(1);
            m_uiFlag |= 4;
        }
        if (isMotionEnd()) changeMotion(5, 0, 1, -1, 0, 0, m_uiKind);
        break;
    case 5:
        if (isMotionEnd()) changeMotion(6, 0, 1, -1, 0, 0, m_uiKind);
    case 6:
        if (m_uiFlag & 0x800) {
            dropPrize();
            m_fMoveSpdXZ = 0.0f;
            vel.y = 0.0f;
            clearSpeed();
            changeMotion(7, 0, 1, -1, 0, 0, 0);
        } else if (isGround()) {
            dropPrize();
            m_fMoveSpdXZ = 0.0f;
            clearSpeed();
            vel.y = 0.0f;
            changeMotion(7, 0, 1, -1, 0, 0, 0);
            FVECTOR rot = { { 0.0f, 3.1415927f, 0.0f, 1.0f } };
            setEffect(g_szDpxSumBanbiB, 4, &getPos(), &rot, 0.0f);
        }
        break;
    case 7:
        if (isMotionEnd()) {
            if (m_iAttackNo < m_pAtkData->TRG_FRAME) {
                changeState(2);
                return;
            }
            if (isGround()) {
                FVECTOR rot = { { 0.0f, 3.1415927f, 0.0f, 1.0f } };
                setEffect(g_szDpxSumBanbiB, 4, &getPos(), &rot, 0.0f);
                setExit(0);
                return;
            }
            m_uiKind = 10;
            changeMotion(6, 0, 1, -1, 0, 0, m_uiKind);
            m_uiFlag = (INT)m_uiFlag & ~0x800;
        }
        break;
    }
    fMoveSpdY = vel.y;
    m_pMove->calcVelocityXZ(m_fMoveSpdXZ);
    vel = m_pMove->getVel();
    vel.y = fMoveSpdY;
    m_pMove->setVel(vel);
    if (getTimer() <= 0) setExit(0);
}

void CSummonBambi::dropPrize() {
    INT i;
    PRIZE_PARAM param;
    if (m_pAtkData->EFFECT == 0) return;
    param.uiFlag = 0;
    param.uiPrizeFlag = 0;
    getLockOnPos(param.vPos);
    param.fSpeed = 0.25f;
    param.PrizeKind = m_pAtkData->EFFECT + 0xD1;
    for (i = 0; i < m_pAtkData->WORK1; i++) {
        FLOAT rx = (-70.0f - RND::genrand_real1() * 10.0f) * 0.017453292f;
        FLOAT ry = RND::genrand_real1() * 360.0f * 0.017453292f;
        param.vRot.set(rx, ry, 0.0f);
        Math3D::LimitPIVector(param.vRot);
        CEntityFactory::getInstance()->createPrize(param, 0);
    }
}

void CSummonBambi::beginStateAction2() {
    m_iSubState = 0;
    m_iState = 3;
    m_uiAttackID = g_BambiAttackID[m_uiLevel];
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_iAttackPower = getPower(m_uiSkillID, 0) / 10;
    SS_PARAM*& ss = CardBattle::g_pStockSkillList;
    m_AtkAttr = (ATK_ATTR)ss[58].ATTR;
    m_uiKind = 2;
    CardBattle::CCardBattle::calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, m_uiUpKind);
    changeMotion(4, 0, 1, -1, 0, 0, 0);
    m_uiFlag &= 0xFFFF03FF;
    m_iAttackNo++;
    m_hTarget = LockOnManager::CLockOnManager::getRandomLockOnObject();
    homingPlayerTargetY(m_hTarget, 0.1f);
    m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), getHandle());
    m_pPlayer->getPlayerCamera()->changeCamMode(4, 0);
    clearSpeed();
    m_fMoveSpdXZ = 0.0f;
    m_pMove->enableGravity(0);
    m_uiFlag = ((INT)m_uiFlag & ~4) | 0x80;
    setTimer(9000);
}

void CSummonBambi::stateAction2() {
    FVECTOR vel;
    FLOAT fMoveSpdY;
    INT frame = getMotionCurrentTime() / 50;
    vel = m_pMove->getVel();
    switch (m_uiAnimID) {
    case 4:
        if (frame <= m_pAtkData->MOVE_START_FRAME) {
            homingPlayerTargetY(m_hTarget, 0.3f);
            break;
        }
        if (!(m_uiFlag & 4)) {
            CEntity* pTarget = m_hTarget > 0 ? (CEntity*)CTaskMan::getTask(m_hTarget) : 0;
            if (pTarget && m_iAttackNo >= 2) {
                pTarget->getLockOnPos(m_vAimPos);
                m_fDist = calcDistance(m_vAimPos);
                FLOAT theta = m_pAtkData->WORK1 * 0.017453292f;
                FLOAT s = sinf(theta);
                FLOAT c = cosf(theta);
                vel.y = s;
                vel.z = c;
                vel.w = 1.0f;
                vel.x = 0.0f;
                m_pMove->calcVelocity(m_pMove->calcOrbitSpeedFromDistance(theta, m_fDist), vel);
                vel = m_pMove->getVel();
            } else {
                FLOAT& spd = m_fMoveSpdXZ;
                spd = m_pAtkData->MAX_SPEED / 60.0f;
                vel.y = PL::g_fAntiGravity * m_pAtkData->WORK0;
                m_pMove->setVel(vel);
            }
            m_pMove->enableGravity(1);
            m_uiFlag |= 4;
        }
        if (isMotionEnd()) changeMotion(5, 0, 1, -1, 0, 0, m_uiKind);
        break;
    case 5:
        if (isMotionEnd()) changeMotion(6, 0, 1, -1, 0, 0, m_uiKind);
    case 6:
        if (m_uiFlag & 0x800) {
            clearSpeed();
            changeMotion(7, 0, 1, -1, 0, 0, 0);
        } else if (isGround()) {
            m_fMoveSpdXZ = 0.0f;
            vel.y = 0.0f;
            clearSpeed();
            changeMotion(7, 0, 1, -1, 0, 0, 0);
            FVECTOR rot = { { 0.0f, 3.1415927f, 0.0f, 1.0f } };
            setEffect(g_szDpxSumBanbiB, 4, &getPos(), &rot, 0.0f);
        }
        break;
    case 7:
        if (isMotionEnd()) {
            if (m_iAttackNo < m_pAtkData->TRG_FRAME) {
                changeState(2);
                return;
            }
            if (isGround()) {
                FVECTOR rot = { { 0.0f, 3.1415927f, 0.0f, 1.0f } };
                setEffect(g_szDpxSumBanbiB, 4, &getPos(), &rot, 0.0f);
                setExit(0);
                return;
            }
            m_uiKind = 10;
            changeMotion(6, 0, 1, -1, 0, 0, m_uiKind);
            m_uiFlag = (INT)m_uiFlag & ~0x800;
        }
        break;
    }
    fMoveSpdY = vel.y;
    m_pMove->calcVelocityXZ(m_fMoveSpdXZ);
    vel = m_pMove->getVel();
    vel.y = fMoveSpdY;
    m_pMove->setVel(vel);
    if (getTimer() <= 0) setExit(0);
}

void CSummonBambi::beginStateExit() {
    m_iState = 5;
    changeMotion(3, 0, 1, -1, 0, 0, 0);
    bodyCollOffBody(-1);
    UINT flag = m_uiFlag;
    TIMEQ tq = (flag & 0x40) ? 0 : 1500;
    m_tqTrgFrame = tq;
    setTimer(tq);
    m_tqSubTimer = (flag & 0x40) ? 0 : 3000;
}

} // namespace Summon

// SOURCE DATA: .data .rodata
