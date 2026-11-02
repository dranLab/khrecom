
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/Player/SummonDumbo.h"
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
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Movement.h"

namespace Summon {

unsigned char g_DumboAttackID[4] = { 125, 131, 132, 0 };

CSummonDumbo::CSummonDumbo(CTask* inParent) : CSummon(inParent, 4) {
}

CSummonDumbo::~CSummonDumbo() {
}

INT CSummonDumbo::setSkill(INT inID) {
    if (m_iState != 1) return 0;
    switch (inID) {
    case 125:
        m_uiSkillID = 11;
        break;
    case 131:
        m_uiSkillID = 12;
        break;
    case 132:
        m_uiSkillID = 13;
        break;
    default:
        setExit(0);
        return 1;
    }
    m_uiLevel = m_uiSkillID - 11;
    changeState(2);
    return 1;
}

void CSummonDumbo::beginStateAppear() {
    m_iState = 0;
    setEntityFlag(0, 1);
    m_uiFlag |= 8;
    m_pMove->enableGravity(0);
    m_vStartPos.y += 0.75f;
    setPos(m_vStartPos);
    warp(m_vStartPos);
    clearSpeed();
    setRot(((CEntity*)m_pPlayer)->getRot());
    bodyCollOnBody(-1);
    updateCoord();
    coordFix();
    m_uiStartAnimID = 14;
    changeMotion(14, 0, 1, 0, 0, 0, 0);
}

void CSummonDumbo::stateAppear() {
    setRot(((CEntity*)m_pPlayer)->getRot());
    if (m_fScale < 1.0f) {
        m_fScale += getFrameRateF() * 0.05f;
        if (m_fScale > 1.0f) m_fScale = 1.0f;
        FVECTOR scale = getScale();
        scale.z = m_fScale;
        scale.x = m_fScale;
        scale.y = m_fScale * 0.5f + 0.5f;
        setScale(scale);
    } else if (isMotionEnd()) {
        m_uiFlag = (INT)m_uiFlag & ~8;
        changeMotion(1, 0, 1, -1, 0, 0, 0);
        changeState(1);
    }
}

void CSummonDumbo::beginStateAction1() {
    m_iState = 2;
    m_uiAttackID = g_DumboAttackID[m_uiLevel];
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_iAttackPower = getPower(m_uiSkillID, 0) / 10;
    if (m_uiLevel == 0) {
        m_AtkAttr = (ATK_ATTR)CardBattle::g_pBattleCardList[38].ATTR;
    } else {
        m_AtkAttr = (ATK_ATTR)CardBattle::g_pStockSkillList[m_uiLevel + 53].ATTR;
    }
    CardBattle::CCardBattle::calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, m_uiUpKind);
    setTimer(getPower(m_uiSkillID, 3));
    m_tqSubTimer = 3000;
    changeMotion(3, 0, 1, -1, 0, 0, 0);
    m_fMoveSpdXZ = 0.0f;
    homingPlayerTargetY(0, m_fMoveSpdXZ);
}

} // namespace Summon

static inline FLOAT _abs(FLOAT inValue) {
    FLOAT ret;
    __asm__ volatile("abs.s %0, %1" : "=f"(ret) : "f"(inValue));
    return ret;
}

static inline FLOAT _limitPI(FLOAT r) {
    if (r < -3.1415927f) r += 6.2831855f;
    else if (r > 3.1415927f) r -= 6.2831855f;
    return r;
}

namespace Summon {

void CSummonDumbo::stateAction1() {
    switch (m_uiAnimID) {
    case 3:
        if (isMotionEnd()) {
            changeMotion(4, 0, 1, -1, m_uiLevel, 0, m_uiLevel);
            m_pPlayer->getPlayerCamera()->setFlag(0x800, 1);
        }
        break;
    case 5:
        m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, 0.0f, 0.2f, getFrameRateF());
        m_pMove->calcVelocityXZ(m_fMoveSpdXZ);
        if (isMotionEnd()) {
            clearSpeed();
            setExit(0);
        }
        return;
    }
    FVECTOR rot = getRot();
    FLOAT x = getAnalogLX(0) * (1.0f / 4096.0f);
    FLOAT speed = m_pAtkData->MAX_SPEED * 0.01f;
    if (_abs(x) > 0.25f) {
        if (x < 0.0f) x = (x + 0.25f) * m_pAtkData->WORK0 * 0.1f * getFrameRateF() * 0.017453292f;
        else x = (x - 0.25f) * m_pAtkData->WORK0 * 0.1f * getFrameRateF() * 0.017453292f;
        rot.y = _limitPI(rot.y - x);
        speed *= 0.5f;
        m_pPlayer->getPlayerCamera()->setFlag(0x800, 1);
    }
    setRot(rot);
    m_fDir = rot.y;
    m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, speed, 0.1f, getFrameRateF());
    m_pMove->calcVelocityXZ(m_fMoveSpdXZ);
    m_tqSubTimer -= getFrameRate();
    if (m_tqSubTimer <= 0) {
        m_tqSubTimer = 3000;
        if (m_pAttack) m_pAttack->updateAttackID();
    }
    if (getTimer() <= 0) changeMotion(5, 0, 1, -1, 0, 0, 0);
}

void CSummonDumbo::beginStateExit() {
    m_iState = 5;
    changeMotion(2, 0, 1, -1, 0, 0, 0);
    bodyCollOffBody(-1);
    UINT flag = m_uiFlag;
    m_iSubState = 0;
    m_tqTrgFrame = (flag & 0x40) ? 0 : 2000;
    m_tqSubTimer = (flag & 0x40) ? 0 : 3000;
    setTimer(m_tqTrgFrame);
    m_uiFlag |= 8;
    m_pMove->enableGravity(0);
    clearSpeed();
}

} // namespace Summon

// SOURCE DATA: .data .rodata
