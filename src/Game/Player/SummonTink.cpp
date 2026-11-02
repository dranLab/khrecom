
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Player/SummonTink.h"
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

#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Player/PlayerManager.h"
#include "Game/Movement.h"
#include "System/Rnd.h"
#include "System/Graphic/Camera.h"
#include "include_asm.h"


extern "C" {
void sceVu0UnitMatrix(FMATRIX& m);
void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
}
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT t);

#include "Game/Player/PlayerGauge.h"

static inline void _unitMatrix(FMATRIX& m) {
    sceVu0UnitMatrix(m);
}
static inline void _rotMatrixY(FMATRIX& m, FLOAT r) {
    sceVu0RotMatrixY(m, m, r);
}

static inline FLOAT _limitPI(FLOAT r) {
    if (r < -3.1415927f) r += 6.2831855f;
    else if (r > 3.1415927f) r -= 6.2831855f;
    return r;
}

namespace Summon {

static LPCSTR g_szDpxSumTinkB = "_sum_tink_b";

unsigned char g_TinkAttackID[4] = { 126, 143, 144, 0 };

FVECTOR g_TinkOffset = { { -0.15f, 1.3f, 0.13f, 1.0f } };

BCAM_PARAM g_TinkCamParam = {
    { 55.0f, 0.05f, 0.25f, 1.5f, { { 0.0f, -0.1f, 0.8f, 0.0f } }, { { 0.0f, 0.3f, 0.0f, 0.0f } } },
    { 55.0f, 0.05f, 0.5f, 1.2f, { { 0.0f, 0.1f, 2.0f, 0.0f } }, { { 0.0f, -0.1f, 0.0f, 0.0f } } },
};

CSummonTink::CSummonTink(CTask* inParent) : CSummon(inParent, 5) {
    m_fCamHeight = 0.5f;
    m_hEffRecover = 0;
    m_tqEffRecover = 0;
}

CSummonTink::~CSummonTink() {
    PlayerCamera::CPlayerCamera::setBattleCamData(0, 0);
    fadeEffRecover(1);
}

INT CSummonTink::setSkill(INT inID) {
    if (m_iState != 1) return 0;
    switch (inID) {
    case 126:
        m_uiSkillID = 14;
        break;
    case 143:
        m_uiSkillID = 15;
        break;
    case 144:
        m_uiSkillID = 16;
        break;
    default:
        setExit(0);
        return 1;
    }
    m_uiLevel = m_uiSkillID - 14;
    changeState(2);
    return 1;
}

void CSummonTink::adjustPlayer() {
    FVECTOR pos;
    FMATRIX m;
    FVECTOR rot;

    m = m_pPlayer->Crd(0).getWorldMatrix();
    ApplyMatrixXYZ(pos, m, g_TinkOffset);
    warp(pos);
    rot = ((CEntity*)m_pPlayer)->getRot();
    PLM::getWorldRotY(rot.y, m);
    setRot(rot);
}

void CSummonTink::beginStateAppear() {
    m_iState = 0;
    setEntityFlag(0, 1);
    adjustPlayer();
    m_fScale = 1.0f;
    FVECTOR scale = { { m_fScale, m_fScale, m_fScale, 1.0f } };
    setScale(scale);
    m_uiStartAnimID = 2;
    changeMotion(2, 0, 1, 0, 0, 0, 0);
    m_uiFlag |= 0x18;
    m_pMove->enableGravity(0);
}

void CSummonTink::stateAppear() {
    adjustPlayer();
    if (isMotionEnd()) {
        changeMotion(1, 0, 1, -1, 0, 0, 0);
        changeState(1);
    }
}

void CSummonTink::beginStateAction1() {
    m_iState = 2;
    PlayerCamera::CPlayerCamera::setBattleCamData(getHandle(), &g_TinkCamParam);
    setLockon(getHandle());
    m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), m_hLockon);
    m_pPlayer->getPlayerCamera()->changeCamMode(4, 0);
    setPushon(2);
    m_uiAttackID = g_TinkAttackID[m_uiLevel];
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    changeMotion(4, 0, 1, -1, 0, 0, 0);
    INT pow = 100;
    getPower(m_uiSkillID, 3);
    CardBattle::CCardBattle::calcEnchant_AttackPoint(pow, m_AtkKind, m_AtkAttr, m_uiUpKind);
    FLOAT rate = pow / 100.0f;
    setTimer(getPower(m_uiSkillID, 3) * (200 - pow) / 100);
    m_tqSubTimer = 0;
    FLOAT hp_max = m_pPlayer->getMaxHP();
    m_fMaxRecover = hp_max * getPower(m_uiSkillID, 2) * 0.01f;
    m_fRecover = hp_max * getPower(m_uiSkillID, 0) * rate / getTimer();
    m_fAddRecover = hp_max * getPower(m_uiSkillID, 1) * 0.001f * rate;
    m_fOldRecover = 0.0f;
    m_fNowRecover = 0.0f;
    /* separate result variable: the original copies x in the branch delay slot */
    FLOAT x = RND::genrand_real1() * 3.1415927f;
    FLOAT r;
    if (x < -3.1415927f) {
        r = x + 6.2831855f;
    } else {
        r = x;
        if (r > 3.1415927f) r -= 6.2831855f;
    }
    m_fDir = r;
}

void CSummonTink::stateAction1() {
    FMATRIX m;
    FVECTOR vel;
    INT recover;

    m = m_pPlayer->Crd(0).getWorldMatrix();
    ApplyMatrixXYZ(m_vAimPos, m, g_TinkOffset);
    vel = ((CEntity*)m_pPlayer)->getVel();
    ScaleVector(vel, vel, getFrameRateF() * 10.0f);
    AddVectorXYZ(m_vAimPos, m_vAimPos, vel);
    FLOAT dist = calcDistanceXZ(m_vAimPos);
    PATK_DATA* atk = m_pAtkData;
    FLOAT range = atk->RADIUS;
    if (range * 0.11f < dist) {
        startTargetMoveXZ(m_vAimPos, atk->MAX_SPEED * 0.01f, atk->WORK0 * 0.01f, range * 0.1f);
        if (m_uiAnimID != 4) changeMotion(4, 0, 1, -1, 0, 0, 0);
    } else {
        endTargetMoveXZ();
        if (m_tqSubTimer <= 0) {
            m_fDir = _limitPI(m_fDir + rndArea(0.65f, 1.35f) * 3.1415927f);
            m_tqSubTimer = m_pAtkData->WORK1 * 100;
        }
        FVECTOR ofs = { { 0.0f, 0.0f, m_pAtkData->RADIUS * 0.1f, 1.0f } };
        FVECTOR pos = getPos();
        _unitMatrix(m);
        _rotMatrixY(m, m_fDir);
        ApplyMatrix(ofs, m, ofs);
        AddVectorXYZ(ofs, m_vAimPos, ofs);
        pos.x = Movement::Approach(pos.x, ofs.x, m_pAtkData->WORK0 * 0.005f, getFrameRateF());
        pos.z = Movement::Approach(pos.z, ofs.z, m_pAtkData->WORK0 * 0.005f, getFrameRateF());
        setPos(pos);
        if (calcDistanceXZ(m_vAimPos) <= m_pAtkData->RADIUS * 0.1f) {
            m_tqSubTimer -= getFrameRate();
            if (m_uiAnimID != 1) changeMotion(1, 0, 1, -1, 0, 0, 0);
        } else {
            if (m_uiAnimID != 4) changeMotion(4, 0, 1, -1, 0, 0, 0);
        }
        m_pMove->homingY(m_vAimPos, m_pAtkData->WORK0 * 0.02f);
    }
    m_fNowRecover += m_fRecover;
    if (isButtonTrg(0x10, 0)) {
        m_fNowRecover += m_fAddRecover;
        initRecover();
    }
    recover = (INT)(m_fNowRecover - m_fOldRecover);
    if (recover > 0) {
        if (m_pPlayer->getHP() < m_pPlayer->getMaxHP()) m_pPlayer->addHP(recover, 0, 0);
        m_fOldRecover += recover;
    }
    if (m_fOldRecover >= m_fMaxRecover) setTimer(0);
    m_pPlayer->setPrizeHealing(3000);
    if (getTimer() <= 0) {
        Gauge::CCommonGauge::setDispSeq(m_pPlayer->getHandle(), 0);
        endTargetMoveXZ();
        clearSpeed();
        setPushon(0);
        setLockon(0);
        setExit(0);
    }
    fadeEffRecover(1);
}

void CSummonTink::beginStateExit() {
    m_iState = 5;
    changeMotion(3, 0, 1, -1, 0, 0, 0);
    bodyCollOffBody(-1);
    m_uiFlag |= 0x18;
    m_pMove->enableGravity(0);
    UINT flag = m_uiFlag;
    setTimer(0);
    m_tqTrgFrame = 0;
    m_tqSubTimer = (flag & 0x40) ? 0 : 3000;
}

void CSummonTink::stateExit() {
    if (m_iSubState > 0) {
        if (getTimer() <= 0) out();
    } else if (getTimer() <= 0) {
        fadeEffRecover(1);
        if ((m_uiFlag & 0x40) || isMotionEnd()) {
            TIMEQ inTime = m_tqSubTimer;
            setTimer(inTime);
            m_iSubState = 1;
            killChild(inTime * 0.02f);
            setEntityFlag(1, 0);
        }
    }
}

void CSummonTink::fadeEffRecover(INT inFlag) {
    if (m_hEffRecover > 0) {
        if (inFlag) {
            m_tqEffRecover -= getFrameRate();
            if (m_tqEffRecover > 0) return;
        }
        m_tqEffRecover = 0;
        CEffect* effect = CEffectMan::getEffect(m_hEffRecover);
        if (effect) effect->loopEnd();
        m_hEffRecover = 0;
    }
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/SummonTink", initRecover__Q26Summon11CSummonTink);

/* Best attempt: same size; combine folds the Crd() result straight into a3 ("move a3,v0" right after the
 * call), so the effect name's address lands in v0 instead of v1 and the lui/lw are scheduled later. */
#if 0
void CSummonTink::initRecover() {
    m_tqEffRecover = 1000;
    if (m_hEffRecover > 0) return;
    FVECTOR pos;
    pos.init();
    register const CCoord* crd asm("$2") = &Crd(1);
    m_hEffRecover = attachEffect(g_szDpxSumTinkB, 0, crd, &pos, 0, 0.0f);
    CEffect* effect = CEffectMan::getEffect(m_hEffRecover);
    if (effect) effect->m_iFlag |= 4;
}
#endif

} // namespace Summon

// SOURCE DATA: .data .rodata
