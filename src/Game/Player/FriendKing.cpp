
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Player/FriendKing.h"
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

#include "Game/Player/Player.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Movement.h"

void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

/* Through inline wrappers: called directly, cse keeps the matrix address in a saved register across the
 * calls; the original recomputes it for each call. */
static inline void _unitMatrix(FMATRIX& m) {
    sceVu0UnitMatrix(m.m);
}
static inline void _rotMatrixY(FMATRIX& m, FLOAT r) {
    sceVu0RotMatrixY(m.m, m.m, r);
}

namespace Friend {

static LPCSTR g_szBoneWep0 = "wep0";

unsigned char g_KingSkillID[4] = { 92, 116, 117, 0 };

BCAM_PARAM g_HolyBurstCamParam = {
    { 55.0f, 0.05f, 0.2f, 1.0f, { { 0.0f, 2.5f, 6.0f, 0.0f } }, { { 0.0f, 0.1f, 0.0f, 0.0f } } },
    { 55.0f, 0.05f, 0.2f, 5.0f, { { 0.0f, 2.5f, 6.0f, 0.0f } }, { { 0.0f, 0.0f, 0.0f, 0.0f } } },
};

CFriendKing::CFriendKing(CTask* inParent) : CFriend(inParent, 8, 0) {
    setDispOffFlag(4, 0);
    LPCSTR& name = g_szBoneWep0;
    m_iAtkBone = getBoneIndex(name, 0);
    m_vAtkOffset.set(-0.78f, 0.0f, 0.0f);
}

CFriendKing::~CFriendKing() {
}

void CFriendKing::beginStateAppear() {
    m_iState = 0;
    m_uiGroundAnimID = 0;
    setEntityFlag(0, 1);
    m_uiStartAnimID = 1;
    changeMotion(1, 0, 0, -1, 0, 0, 0);
    switch (m_pPlayer->getAttackID()) {
    case 118:
    case 119:
        adjustPlayer(0);
        m_uiFlag |= 0x2000008;
        break;
    }
    updateCoord();
    coordFix();
    setAppearEffect();
    bodyCollOnBody(-1);
}

void CFriendKing::stateAppear() {
    if (m_uiFlag & 0x2000000) {
        adjustPlayer(0);
    } else {
        m_pMove->homingY(m_pPlayer, 0.1f);
    }
    if (m_fScale < 1.0f) {
        m_fScale += getFrameRateF() * 0.05f;
        if (m_fScale > 1.0f) m_fScale = 1.0f;
        FVECTOR scale = getScale();
        scale.z = m_fScale;
        scale.x = m_fScale;
        scale.y = m_fScale * 0.5f + 0.5f;
        setScale(scale);
    } else {
        changeState(1);
    }
}

INT CFriendKing::setSkill(INT inID) {
    if (m_iState != 1) return 0;
    switch (inID) {
    case 92:
        m_uiSkillID = 25;
        m_uiLevel = 0;
        changeState(2);
        break;
    case 116:
        m_uiSkillID = 26;
        m_uiLevel = 1;
        changeState(2);
        break;
    case 117:
        m_uiSkillID = 27;
        m_uiLevel = 2;
        changeState(2);
        break;
    case 118:
    case 119:
        m_uiSkillID = 28;
        m_uiLevel = 0;
        changeState(3);
        break;
    default:
        setExit(0);
        break;
    }
    return 1;
}

void CFriendKing::beginStateAction1() {
    m_iState = 2;
    m_iSubState = 1;
    m_uiAttackID = g_KingSkillID[m_uiLevel];
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_iAttackPower = getPower(m_uiSkillID, 0) / 10;
    BC_PARAM*& bc = CardBattle::g_pBattleCardList;
    m_AtkAttr = bc[56].ATTR;
    changeMotion(2, 0, 1, -1, 0, 0, 0);
    setDispOffFlag(0, 4);
    m_pMove->enableGravity(1);
    m_uiFlag = (INT)m_uiFlag & ~8;
    m_fBodyRviseCoefficient = 0.25f;
}

void CFriendKing::stateAction1() {
    INT frame = getMotionCurrentTime() / 50;
    switch (m_iSubState) {
    case 1:
        if (frame >= 92) {
            setMagic(m_uiAttackID);
            m_iSubState = 2;
            setTimer(1000);
        }
        break;
    case 2:
        if (getTimer() <= 0) {
            INT pow = m_pPlayer->getMaxHP() * getPower(m_uiSkillID, 3) / 100;
            m_iAttackPower = pow;
            m_pPlayer->setCure(51, pow);
            m_iSubState = 7;
            changeMotion(3, 0, 1, -1, 0, 0, 0);
            if (m_pPlayer->getRevolver()) m_pPlayer->getRevolver()->setAction(56, 0);
        }
        break;
    case 7:
        if (isMotionEnd()) setExit(0);
        break;
    }
}

void CFriendKing::endStateAction1() {
    setDispOffFlag(4, 0);
}

void CFriendKing::beginStateAction2() {
    m_iState = 3;
    m_iSubState = 3;
    m_uiAttackID = 118;
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_uiKind = 0;
    BC_PARAM*& bc = CardBattle::g_pBattleCardList;
    m_AtkAttr = bc[56].ATTR;
    m_uiFlag |= 8;
    setDispOffFlag(4, 0);
    m_pMove->enableGravity(0);
    clearSpeed();
}

void CFriendKing::stateAction2() {
    if (exeKingHolyBurst()) setExit(0);
}

INT CFriendKing::exeKingHolyBurst() {
    switch (m_iSubState) {
    case 3:
        adjustPlayer(1);
        if (m_pPlayer->isConditionFlag(0x80000)) {
            m_iSubState = 4;
            changeMotion(11, 0, 0, 0, 0, 0, 0);
            m_uiFlag |= 8;
            userBodyCollOff(-1);
            userBodyCollOffBody(-1);
        }
        break;
    case 4:
        adjustPlayer(1);
        if (isMotionEnd()) {
            m_iSubState = 5;
            changeMotion(12, 0, 0, -1, 0, 0, 0);
            setTimer(m_pAtkData->TRG_FRAME * 50);
        }
        break;
    case 5:
        adjustPlayer(1);
        if (getTimer() <= 0) {
            setMagic(118);
            setTimer(m_pAtkData->TRG_FRAME * 50);
        }
        if (!m_pPlayer->isConditionFlag(0x80000)) {
            m_uiFlag = (INT)m_uiFlag & ~8;
            m_iSubState = 7;
            userBodyCollOn(-1);
            userBodyCollOnBody(-1);
            changeMotion(13, 0, 1, -1, 0, 0, 0);
        }
        break;
    case 7:
        if (isMotionEnd()) return 1;
        break;
    }
    return 0;
}

void CFriendKing::adjustPlayer(INT inWarp) {
    FVECTOR pos = { { -1.1f, 0.0f, 0.0f, 1.0f } };
    FVECTOR rot;
    FMATRIX m;

    rot = m_pPlayer->getRot();
    setRot(rot);
    _unitMatrix(m);
    _rotMatrixY(m, rot.y);
    ApplyMatrixXYZ(pos, m, pos);
    AddVectorXYZ(pos, pos, m_pPlayer->getPos());
    if (inWarp) {
        warp(pos);
    } else {
        setPos(pos);
    }
}

} // namespace Friend

// SOURCE DATA: .data .rodata
