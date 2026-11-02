#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "System/Task.h"
#include "System/TaskMan.h"
#include "Game/Player/SummonSinba.h"
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
#include "System/Graphic/Effect/effect.h"
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Movement.h"
#include "Game/Collision/CollMap.h"

extern "C" {
void sceVu0UnitMatrix(FMATRIX& m);
void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
}
void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

static inline void _unitMatrix(FMATRIX& m) {
    sceVu0UnitMatrix(m);
}
static inline void _rotMatrixY(FMATRIX& m, FLOAT r) {
    sceVu0RotMatrixY(m, m, r);
}

static inline FVECTOR _makeVector(FLOAT inX, FLOAT inY, FLOAT inZ) {
    FVECTOR v;
    v.set(inX, inY, inZ);
    return v;
}

namespace Summon {

unsigned char g_ShibaAttackID[4] = { 122, 129, 130, 0 };

CSummonSinba::CSummonSinba(CTask* inParent) : CSummon(inParent, 1) {
}

CSummonSinba::~CSummonSinba() {
}

void CSummonSinba::stateAppear() {
    m_uiFlag |= 8;
    homingPlayerTargetXZ(0, 0.1f);
    if (m_fScale < 1.0f) {
        turnPlayerTargetY(0, 0.1f);
        m_fScale += getFrameRateF() * 0.03f;
        if (m_fScale > 1.0f) m_fScale = 1.0f;
        FVECTOR scale = getScale();
        scale.z = m_fScale;
        scale.x = m_fScale;
        scale.y = m_fScale * 0.5f + 0.5f;
        setScale(scale);
        return;
    }
    FLOAT fHeight = 10.0f;
    FVECTOR pos = getPos();
    FVECTOR ofs;
    ofs.x = 0.0f;
    ofs.y = 0.0f;
    ofs.z = 1.0f;
    ofs.w = 1.0f;
    FVECTOR rot = getRot();
    FMATRIX m;
    _unitMatrix(m);
    _rotMatrixY(m, rot.y);
    ApplyMatrixXYZ(ofs, m, ofs);
    FVECTOR vStart = { { pos.x, pos.y, pos.z, 1.0f } };
    FVECTOR vEnd = { { pos.x + ofs.x, pos.y + ofs.y, pos.z + ofs.z, 1.0f } };
    CCollMap* coll = m_pMove->getMapCollision();
    coll->setStartPos(vStart);
    coll->setEndPos(vEnd);
    if (coll->collideRide(0) || coll->collideMap()) {
        m_uiFlag |= 0x20;
    } else {
        vStart = vEnd;
        vEnd.y -= fHeight;
        coll->setStartPos(vStart);
        coll->setEndPos(vEnd);
        if (coll->collideRide(0) || coll->collideMap()) {
            fHeight = pos.y - coll->getDetectPos()->y;
        }
    }
    if (fHeight > 0.1f) m_uiFlag |= 0x20;
    if (m_uiFlag & 0x20) {
        changeMotion(1, 0, 0, -1, 0, 0, 0);
        changeState(1);
    } else if (getTimer() <= 0) {
        changeMotion(1, 0, 0, -1, 0, 0, 0);
        changeState(1);
    }
}

INT CSummonSinba::setSkill(INT inID) {
    if (m_iState != 1) return 0;
    m_iAttackNo = 0;
    switch (inID) {
    case 122:
        m_uiSkillID = 1;
        m_uiLevel = 0;
        m_uiKind = 0;
        changeState(2);
        return 1;
    case 129:
        m_uiLevel = 1;
        m_uiSkillID = 2;
        m_uiKind = 1;
        changeState(2);
        return 1;
    case 130:
        m_uiSkillID = 3;
        m_uiLevel = 2;
        m_uiKind = 1;
        changeState(2);
        return 1;
    }
    setExit(0);
    return 1;
}

void CSummonSinba::beginStateAction1() {
    m_iState = 2;
    m_iSubState = 1;
    m_uiAttackID = g_ShibaAttackID[m_uiLevel];
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_iAttackPower = getPower(m_uiSkillID, 0) / 10;
    BC_PARAM*& bc = CardBattle::g_pBattleCardList;
    m_AtkAttr = (ATK_ATTR)bc[35].ATTR;
    CardBattle::CCardBattle::calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, m_uiUpKind);
    changeMotion(m_uiAnimID, 0, 0, -1, 0, 0, m_uiKind);
    m_pMove->enableGravity(1);
    homingPlayerTargetY(0, 0.01f);
    m_hLockon = m_hTarget;
    changeCamera(m_pPlayer->getHandle(), 4, 0);
}

void CSummonSinba::beginStateExit() {
    m_iState = 5;
    bodyCollOffBody(-1);
    UINT flag = m_uiFlag;
    m_tqTrgFrame = 0;
    setTimer(0);
    m_iSubState = 0;
    m_tqSubTimer = (flag & 0x40) ? 0 : 3000;
    m_uiFlag = flag | 8;
    m_pMove->enableGravity(0);
    clearSpeed();
}

void CSummonSinba::stateAction1() {
    if (exeSinbaAttack()) setExit(0);
}

INT CSummonSinba::exeSinbaAttack() {
    switch (m_iSubState) {
    case 1:
        homingPlayerTargetY(0, 0.01f);
        if (isMotionEnd() || getTimer() <= 0) {
            m_iSubState = 2;
            changeMotion(4, 0, 0, -1, 0, 0, m_uiKind);
            m_tqSubTimer = 1500;
        }
        break;
    case 2:
        m_tqSubTimer -= getFrameRate();
        if (m_tqSubTimer <= 0) {
            m_tqSubTimer = 1500;
            homingPlayerTargetY(0, 0.1f);
        }
        if (isMotionEnd()) {
            m_iSubState = 3;
            changeMotion(5, 0, 0, -1, 0, 0, m_uiKind);
            clearSpeed();
            setTimer(150);
        }
        break;
    case 3:
        if (isMotionEnd() || getTimer() <= 0) {
            m_iSubState = 4;
            changeMotion(6, 0, 0, -1, 0, 0, m_uiKind);
        }
        break;
    case 4:
        if (isMotionEnd()) {
            changeMotion(7, 0, 0, -1, 0, 0, m_uiKind);
            m_iSubState = 6;
            setTimer(2200);
        }
        break;
    case 5:
        if (isMotionEnd()) {
            m_iSubState = 6;
            changeMotion(1, 0, 0, -1, 0, 0, m_uiKind);
            setTimer(1500);
        }
        break;
    case 6:
        if (getTimer() <= 0) return 1;
        break;
    }
    return 0;
}

} // namespace Summon
// SOURCE DATA: .data .rodata
