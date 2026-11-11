
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Player/SummonMushu.h"
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
#include "Game/Player/Player.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Player/PlayerManager.h"
#include "Game/Movement.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"
#include "Game/Debug/DebugWork.h"
#include "System/Graphic/Camera.h"
#include "System/Debug.h"
#include "include_asm.h"

void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

namespace PL {
extern BULLET_EFFECT_INFO g_EffTbl_Skill[67][4]; /* Game/Player/PlayerAttack.h */
}

#include "System/Math3D.h"





static inline FLOAT _limitPI(FLOAT r) {
    if (r < -3.1415927f) r += 6.2831855f;
    else if (r > 3.1415927f) r -= 6.2831855f;
    return r;
}

namespace Summon {

static LPCSTR g_szBoneRoot = "root";

unsigned char g_MushuAttackID[4] = { 127, 136, 137, 0 };

FVECTOR g_MushuOffset = { { -0.2f, 0.42f, 0.1f, 1.0f } };

BCAM_PARAM g_MushuCamParam = {
    { 55.0f, 0.05f, 0.5f, 0.0f, { { 0.0f, 0.4f, 1.5f, 0.0f } }, { { 0.0f, 0.2f, 0.0f, 0.0f } } },
    { 55.0f, 0.05f, 0.5f, 0.0f, { { 0.0f, 0.4f, 1.5f, 0.0f } }, { { 0.0f, 0.2f, 0.0f, 0.0f } } },
};

CSummonMushu::CSummonMushu(CTask* inParent) : CSummon(inParent, 6) {
    /* through a reference: a direct load of the static is scheduled above the vtable stores */
    LPCSTR& name = g_szBoneRoot;
    m_iAtkBone = getBoneIndex(name, 0);
    m_vAtkOffset.set(0.0f, 0.12f, 0.36f);
    m_fCamHeight = 0.0f;
}

CSummonMushu::~CSummonMushu() {
    PlayerCamera::CPlayerCamera::setBattleCamData(0, 0);
}

void CSummonMushu::adjustPlayer() {
    FVECTOR pos;
    FMATRIX m;
    FVECTOR rot;

    m = m_pPlayer->Crd(1).getWorldMatrix();
    if (dbgCheckUser(KKIKUCHI) && KH_DW(DebugWork::g_dw.player.iTemp) == 7) {
        g_MushuOffset.x = KH_DW(DebugWork::g_dw.player.vTemp.x);
        g_MushuOffset.y = KH_DW(DebugWork::g_dw.player.vTemp.y);
        g_MushuOffset.z = KH_DW(DebugWork::g_dw.player.vTemp.z);
    }
    ApplyMatrixXYZ(pos, m, g_MushuOffset);
    warp(pos);
    if (m_iState == 0) {
        rot = ((CEntity*)m_pPlayer)->getRot();
        PLM::getWorldRotY(rot.y, m_pPlayer->Crd(0).getWorldMatrix());
        setRot(rot);
    }
}

void CSummonMushu::updateBefore() {
    CSummon::updateBefore();
    adjustPlayer();
}

void CSummonMushu::beginStateAppear() {
    m_iState = 0;
    setEntityFlag(0, 1);
    m_uiStartAnimID = 2;
    changeMotion(2, 0, 1, 0, 0, 0, 0);
    m_uiFlag |= 0x18;
    m_pMove->enableGravity(0);
}

INT CSummonMushu::setSkill(INT inID) {
    if (m_iState != 1) return 0;
    switch (inID) {
    case 127:
        m_uiSkillID = 17;
        break;
    case 136:
        m_uiSkillID = 18;
        break;
    case 137:
        m_uiSkillID = 19;
        break;
    default:
        setExit(0);
        return 1;
    }
    m_uiLevel = m_uiSkillID - 17;
    changeState(2);
    return 1;
}

void CSummonMushu::beginStateAction1() {
    m_iState = 2;
    m_uiAttackID = g_MushuAttackID[m_uiLevel];
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_iAttackPower = getPower(m_uiSkillID, 0) / 10;
    if (m_uiLevel == 0) {
        m_AtkAttr = (ATK_ATTR)CardBattle::g_pBattleCardList[40].ATTR;
    } else {
        m_AtkAttr = (ATK_ATTR)CardBattle::g_pStockSkillList[m_uiLevel + 58].ATTR;
    }
    CardBattle::CCardBattle::calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, m_uiUpKind);
    m_uiKind = getPower(m_uiSkillID, 3);
    PlayerCamera::CPlayerCamera::setBattleCamData(getHandle(), &g_MushuCamParam);
    setLockon(getHandle());
    m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), m_hLockon);
    m_pPlayer->getPlayerCamera()->changeCamMode(4, 0);
    setPushon(6);
    setTimer(30000);
}

void CSummonMushu::stateAction1() {
    PlayerCamera::CPlayerCamera::getBattleCamData(g_MushuCamParam);
    FVECTOR rot = getRot();
    FLOAT x = getAnalogLX(0) * (1.0f / 4096.0f);
    FLOAT y = -(getAnalogLY(0) * (1.0f / 4096.0f));
    if (_fabsf(x) > 0.25f) {
        rot.y = _limitPI(rot.y - x * 3.0f * getFrameRateF() * 0.017453292f);
    }
    if (_fabsf(y) > 0.25f) {
        y = y * 1.5f * getFrameRateF() * 0.017453292f;
        rot.x = _limitPI(rot.x - y);
        FLOAT limit = 0.7853982f;
        y = _fabsf(rot.x);
        if (y > limit) {
            if (rot.x < 0.0f) limit = -limit;
            rot.x = limit;
        }
    }
    setRot(rot);
    m_fDir = rot.y;
    INT frame = getMotionCurrentTime() / 50;
    switch (m_uiAnimID) {
    case 4:
        if (isMotionEnd()) changeMotion(5, 0, 1, -1, 0, 0, 0);
        break;
    case 5:
        if (!(m_uiFlag & 0x400)) {
            setFlare();
            m_uiFlag |= 0x400;
        }
        if (m_iAttackNo >= m_uiKind || m_pPlayer->isBattleFlag(4) || getTimer() <= 0) {
            if (isMotionEnd()) setExit(0);
        } else {
            if (m_pAtkData->TRG_FRAME < frame && isButtonTrg(0x40, 0)) {
                changeMotion(5, 0, 1, -1, 0, 0, 0);
                m_uiFlag = (INT)m_uiFlag & ~0x400;
                m_iAttackNo++;
            }
            if (isMotionEnd()) changeMotion(1, 0, 1, -1, 0, 0, 0);
        }
        break;
    case 1:
        if (isButtonTrg(0x40, 0)) {
            changeMotion(4, 0, 1, -1, 0, 0, 0);
            m_uiFlag = (INT)m_uiFlag & ~0x400;
            m_iAttackNo++;
        } else if (getTimer() <= 0) {
            setExit(0);
        }
        break;
    }
}

void CSummonMushu::setFlare() {
    BULLET_PARAM param;
    FVECTOR pos;

    getLockOnPos(pos);
    param.init(2, 3);
    param.CollKind = 0x80000000;
    param.AtkKind = m_pAtkData->DAMAGE;
    param.AtkAttr = m_AtkAttr;
    param.iAttackPoint = m_iAttackPower;
    getWeaponTopPos(param.vPos);
    calcDirection(param.vRot, pos);
    param.vRot.x += m_pAtkData->WORK0 * 0.017453292f;
    param.fMaxDist = 15.0f;
    param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
    param.fCollRadius = m_pAtkData->RADIUS * 0.01f;
    param.tqExplodeTime = 1000;
    param.vPos.w = param.fCollRadius + param.fCollRadius;
    param.pEffectTbl = PL::g_EffTbl_Skill[m_pAtkData->EFFECT];
    CEntityFactory::getInstance()->createBullet(param, this);
}

void CSummonMushu::beginStateExit() {
    m_iState = 5;
    changeMotion(3, 0, 1, -1, 0, 0, 0);
    bodyCollOffBody(-1);
    m_uiFlag |= 0x18;
    m_pMove->enableGravity(0);
    UINT flag = m_uiFlag;
    m_iSubState = 0;
    TIMEQ tq = (flag & 0x40) ? 0 : 1500;
    m_tqTrgFrame = tq;
    setTimer(tq);
    m_tqSubTimer = (flag & 0x40) ? 0 : 3000;
}

void CSummonMushu::getCameraTargetPos(FVECTOR& outPos) {
    getLockOnPos(outPos);
}

} // namespace Summon
// SOURCE DATA: .data .rodata
