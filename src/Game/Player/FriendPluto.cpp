
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Player/FriendPluto.h"
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

#include "System/TaskMan.h"



#include "Game/Player/Player.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Movement.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"
#include "System/Graphic/Camera.h"
#include "System/Math3D.h"
#include "System/Rnd.h"

void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

namespace PL {
extern BULLET_EFFECT_INFO g_EffTbl_Skill[67][4]; /* Game/Player/PlayerAttack.h */
}

namespace Friend {

unsigned char g_PlutoSkillID[4] = { 93, 120, 121, 0 };

unsigned char g_PlutoItemTable[2][4] = {
    { 20, 60, 20, 0 },
    { 5, 5, 90, 0 },
};

CFriendPluto::CFriendPluto(CTask* inParent) : CFriend(inParent, 9, 0) {
    m_nTable = 0;
    m_nDiscovery = 0;
    m_iAtkBone = 0;
    m_vAtkOffset.set(0.0f, 0.1f, 0.32f);
}

CFriendPluto::~CFriendPluto() {
}

INT CFriendPluto::setSkill(INT inID) {
    if (m_iState != 1) return 0;
    switch (inID) {
    case 93:
        m_uiSkillID = 29;
        m_uiLevel = 0;
        changeState(2);
        break;
    case 120:
        m_uiSkillID = 30;
        m_uiLevel = 1;
        changeState(2);
        break;
    case 121:
        m_uiSkillID = 31;
        m_uiLevel = 2;
        changeState(2);
        break;
    default:
        setExit(0);
        break;
    }
    return 1;
}

void CFriendPluto::beginStateAction1() {
    m_iState = 2;
    m_iSubState = 1;
    m_uiAttackID = g_PlutoSkillID[m_uiLevel];
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    calcDiscovery();
    FVECTOR& aim = m_vAimPos;
    aim._q = 0;
    aim.w = 1.0f;
    m_fRotDir = Math3D::LimitPI(RND::genrand_real1() * 360.0f * 0.017453292f);
    m_fDist = m_pAtkData->RADIUS * 0.1f;
    calcCirclePos(m_vAimPos, m_fDist, m_fRotDir);
    AddVectorXYZ(m_vAimPos, m_vAimPos, getPos());
    setTimer(m_pAtkData->MOVE_END_FRAME * 100);
    changeMotion(4, 0, 1, -1, 0, 0, 0);
    startTargetMoveXZ(m_vAimPos, m_pAtkData->MAX_SPEED * 0.001f, m_pAtkData->WORK1 * 0.01f,
                      m_pAtkData->RADIUS * 0.01f);
    m_pMove->enableGravity(1);
}

void CFriendPluto::stateAction1() {
    TIMEQ frameTQ = getMotionCurrentTime();
    INT frame = frameTQ / 50; /* unused: only the divide-by-zero trap is left in the original */
    switch (m_iSubState) {
    case 1:
        if (!isTargetMoveXZ() || getTimer() <= 0) {
            endTargetMoveXZ();
            m_iSubState = 2;
            changeMotion(6, 0, 1, -1, 0, 0, 0);
            m_uiFlag |= 0x100000;
            m_fBodyRviseCoefficient = 0.2f;
        }
        break;
    case 2:
        if (isMotionEnd()) {
            dropDiscovery();
            if (m_nDiscovery == 0) {
                changeMotion(1, 0, 1, -1, 0, 0, 0);
            } else {
                changeMotion(5, 0, 1, -1, 0, 0, 0);
            }
            m_iSubState = 3;
            setTimer(3000);
        }
        break;
    case 3:
        if (frameTQ < 800 && m_uiAnimID == 5) {
            homingPlayerTargetY(m_pPlayer->getHandle(), 0.3f);
        }
        if (getTimer() <= 0) {
            if (m_uiAnimID != 1) changeMotion(1, 0, 1, -1, 0, 0, 0);
            m_iSubState = 4;
            setTimer(3000);
        }
        break;
    case 4:
        if (getTimer() <= 0) setExit(0);
        break;
    }
}

void CFriendPluto::endAction1() {
    if (!m_pPlayer->isBattleFlag(4)) {
        UINT flag = m_uiFlag;
        if (flag & 0x100000) dropDiscovery();
    }
}

void CFriendPluto::calcDiscovery() {
    INT n;
    CardBattle::CCardRevolver* rv = m_pPlayer->getRevolver();
    m_nTable = 0;
    if (rv && rv->getHave(0, 1, 0, 0, 0) < 7) m_nTable = 1;
    m_nDiscovery = 0;
    INT r = RND::genrand_int31() % 100;
    n = 0;
    for (m_nDiscovery = 0; m_nDiscovery < 3; m_nDiscovery++) {
        n += g_PlutoItemTable[m_nTable][m_nDiscovery];
        if (r < n) break;
    }
}

void CFriendPluto::dropDiscovery() {
    INT num = 0;
    INT i;
    PRIZE_PARAM param;
    CardBattle::CCardRevolver* pRV;

    m_uiFlag &= ~0x100000;
    param.uiFlag = 0;
    param.uiPrizeFlag = 0;
    getWeaponTopPos(param.vPos);
    param.fSpeed = 0.25f;
    switch (m_nDiscovery) {
    case 0:
        setDropBone();
        break;
    case 1:
        setEffect(PL::g_EffTbl_Skill[65][0].pEffectName, 1, &param.vPos, 0, 0.0f);
        param.vPos = Crd(0).getWorldMatrix().v[3];
        param.vPos.y += 0.5f;
        switch (m_uiLevel) {
        case 0: num = 3; break;
        case 1: num = 7; break;
        case 2: num = 10; break;
        }
        param.PrizeKind = 211;
        param.iNumber = 0;
        for (i = 0; i < num; i++) {
            FLOAT rx = (-70.0f - RND::genrand_real1() * 10.0f) * 0.017453292f;
            FLOAT ry = RND::genrand_real1() * 360.0f * 0.017453292f;
            param.vRot.set(rx, ry, 0.0f);
            Math3D::LimitPIVector(param.vRot);
            CEntityFactory::getInstance()->createPrize(param, 0);
        }
        param.PrizeKind = 213;
        for (i = 0; i < num; i++) {
            FLOAT rx = (-70.0f - RND::genrand_real1() * 10.0f) * 0.017453292f;
            FLOAT ry = RND::genrand_real1() * 360.0f * 0.017453292f;
            param.vRot.set(rx, ry, 0.0f);
            Math3D::LimitPIVector(param.vRot);
            CEntityFactory::getInstance()->createPrize(param, 0);
        }
        break;
    case 2:
        switch (m_uiLevel) {
        case 0: num = 3; break;
        case 1: num = 6; break;
        case 2: num = 9; break;
        }
        pRV = m_pPlayer->getRevolver();
        if (pRV && pRV->calcPlutoMark()) {
            setEffect(PL::g_EffTbl_Skill[65][0].pEffectName, 1, &param.vPos, 0, 0.0f);
            param.vPos = Crd(0).getWorldMatrix().v[3];
            param.vPos.y += 0.5f;
            for (i = 0; i < num; i++) {
                pRV->dropPlutoMarkCard(&param.vPos);
            }
        }
        break;
    }
}

void CFriendPluto::setDropBone() {
    BULLET_PARAM param;
    static BULLET_EFFECT_SHADOW_INFO g_ShadowInfo = { "nonos", 1 };

    param.init(2, 3);
    param.CollKind = 2;
    param.AtkKind = 24;
    param.AtkAttr = 1;
    param.iAttackPoint = 0;
    param.vPos = Crd(0).getWorldMatrix().v[3];
    param.vPos.y += 0.5f;
    param.fSpeed = 0.25f;
    param.fCollRadius = 0.4f;
    param.fShiftY = 0.2f;
    param.pEffectTbl = PL::g_EffTbl_Skill[65];
    param.pShadow = &g_ShadowInfo;
    param.uiFlag |= 0x40005c;
    param.tqHomingTime = 30000;
    FLOAT rx = (-70.0f - RND::genrand_real1() * 10.0f) * 0.017453292f;
    FLOAT ry = RND::genrand_real1() * 360.0f * 0.017453292f;
    param.vRot.set(rx, ry, 0.0f);
    Math3D::LimitPIVector(param.vRot);
    CEntityFactory::getInstance()->createBullet(param, 0);
}

} // namespace Friend

// SOURCE DATA: .data .rodata
