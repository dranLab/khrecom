
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Player/FriendAladdin.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in GimmickBell); which
 * headers they come from is not known. */
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
#include "System/Math3D.h"
#include "System/Rnd.h"

namespace Friend {

static LPCSTR g_szBoneWep0 = "wep0";

unsigned char g_AladdinAttackID[4] = { 87, 105, 106, 0 };

CFriendAladdin::CFriendAladdin(CTask* inParent) : CFriend(inParent, 3, 0) {
    /* through a reference: a direct load of the static is scheduled above the vtable stores */
    LPCSTR& name = g_szBoneWep0;
    m_iAtkBone = getBoneIndex(name, 0);
    m_vAtkOffset.set(0.75f, 0.0f, 0.0f);
    m_fCamHeight = 0.8f;
}

CFriendAladdin::~CFriendAladdin() {
}

INT CFriendAladdin::setSkill(INT inID) {
    if (m_iState != 1) return 0;
    switch (inID) {
    case 87:
        m_uiSkillID = 10;
        m_uiLevel = 0;
        changeState(2);
        break;
    case 105:
        m_uiSkillID = 11;
        m_uiLevel = 1;
        changeState(2);
        break;
    case 106:
        m_uiSkillID = 12;
        m_uiLevel = 2;
        changeState(2);
        break;
    default:
        setExit(0);
        break;
    }
    return 1;
}

void CFriendAladdin::beginStateAction1() {
    m_iSubState = 1;
    m_iState = 2;
    m_uiAttackID = g_AladdinAttackID[m_uiLevel];
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_iAttackPower = getPower(m_uiSkillID, 0) / 10;
    BC_PARAM*& bc = CardBattle::g_pBattleCardList;
    m_AtkAttr = bc[51].ATTR;
    m_iSubState = 2;
    if (m_pAtkData->DAMAGE == 7) {
        changeMotion(5, 0, 1, -1, 0, 0, 1);
    } else {
        changeMotion(5, 0, 1, -1, 0, 0, 0);
    }
    changeCamera(m_pPlayer->getHandle(), 4, 0);
    m_pMove->enableGravity(1);
}

void CFriendAladdin::stateAction1() {
    INT i;
    PRIZE_PARAM param;

    homingPlayerTargetY(0, 0.1f);
    if ((m_uiFlag & 2) && m_pAtkData->WORK0) {
        param.uiFlag = 0;
        param.uiPrizeFlag = 0;
        getWeaponTopPos(param.vPos);
        param.fSpeed = 0.25f;
        param.PrizeKind = m_pAtkData->WORK0 + 211;
        for (i = 0; i < m_pAtkData->WORK1; i++) {
            FLOAT rx = (-70.0f - RND::genrand_real1() * 10.0f) * 0.017453292f;
            FLOAT ry = RND::genrand_real1() * 360.0f * 0.017453292f;
            param.vRot.set(rx, ry, 0.0f);
            Math3D::LimitPIVector(param.vRot);
            CEntityFactory::getInstance()->createPrize(param, 0);
        }
    }
    if (isMotionEnd()) setExit(0);
}

} // namespace Friend

// SOURCE DATA: .data .rodata
