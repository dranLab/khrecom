
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyWyvern.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyWyvernEmptyString() {
    TRACE("");
}

#include "Game/Movement.h"
#include "System/Math3D.h"
#include "System/Rnd.h"

extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */
extern "C" float atan2f(float y, float x);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

/* (original form unknown: written inline, the constants load in another order) */
static inline FLOAT _frameRate(const CTask* inTask) {
    return inTask->getFrameRate() * 0.020000001f;
}

static inline void _unitMatrix(FMATRIX& m) {
    __asm__ volatile("vmr32.xyzw $vf1, $vf0\n"
                     "sqc2 $vf0, 0x30(%0)\n"
                     "vmr32.xyzw $vf2, $vf1\n"
                     "sqc2 $vf1, 0x20(%0)\n"
                     "vmr32.xyzw $vf3, $vf2\n"
                     "sqc2 $vf2, 0x10(%0)\n"
                     "sqc2 $vf3, 0x0(%0)"
                     :
                     : "r"(&m)
                     : "memory");
}

namespace Wyvern {
static CHAR* IdlingMotion = "e024_001";
static CHAR* MoveStartMotion = "e024_002";
static CHAR* MoveLoopMotion = "e024_003";
static CHAR* MoveEndMotion = "e024_004";
static CHAR* GlideRightMotion = "e024_005";
static CHAR* GlideLeftMotion = "e024_006";
static CHAR* KickMotion = "e024_050";
static CHAR* RushAttackStartMotion = "e024_051";
static CHAR* RushAttackLoopMotion = "e024_052";
static CHAR* RushAttackEndMotion = "e024_053";
static CHAR* RushAtkSpdParamName = "RushAtkSpd";
}

CEnemyWyvern::CEnemyWyvern(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setEntityFlag(0x80, 0);
    setBattleEnemyFlag(0x100, 0);
    setBodyRviseCoefficient(0.1f);
    m_fHomingDeg = 10.0f;
}

CEnemyWyvern::~CEnemyWyvern() {}

BOOL CEnemyWyvern::initalize(INT inGroupID, INT inRsrcID) {
    CHeartless* pThis = this;
    if (!pThis->initalize(inGroupID, inRsrcID, 0)) return FALSE;
    m_pMove->setShiftY(0.0f);
    return TRUE;
}

void CEnemyWyvern::beginStateMain() {
    commonFlyInit(Wyvern::IdlingMotion, Wyvern::MoveLoopMotion, 0.5f, 1.5f);
    m_iWalkStartMotIndex = getMotionIndex(Wyvern::MoveStartMotion);
    CHAR*& rMoveEnd = Wyvern::MoveEndMotion; /* through a reference: loaded after the store above */
    m_iWalkEndMotIndex = getMotionIndex(rMoveEnd);
    m_iRotFlyStep = 0;
}

void CEnemyWyvern::stateMain() {
    switch (m_iRotFlyStep) {
    case 0:
        if (m_iStep == 0 && RND::genrand_int31() % m_iWaitRate == 0) {
            clearSpeed();
            changeMotion(m_iWalkStartMotIndex, 0, FALSE, -1, 0, 0);
            FVECTOR vVel;
            vVel.set(0.0f, 0.01f, 0.0f);
            setBattleEnemyFlag(0x800000000ULL, 0);
            m_pMove->setVel(vVel);
            m_iRotFlyStep = 1;
        } else {
            commonFly();
        }
        break;
    case 1:
        if (getPos().y >= 3.0f) {
            FVECTOR vPos = getPos();
            vPos.y = 3.0f;
            setPos(vPos);
            clearSpeed();
        }
        if (isMotionEnd()) {
            clearSpeed();
            setTimer(RND::genrand_int31() % 3000 + 12000);
            CEntity*& rPlayer = g_pPlayer; /* through a reference: loaded after the timer store */
            m_fRot = calcDirection(rPlayer);
            m_fRotDir = (RND::genrand_int31() & 1) ? 1.0f : -1.0f;
            m_iRotFlyStep = 2;
            if (0.0f < m_fRotDir) {
                changeMotionFileName(Wyvern::GlideLeftMotion, 0, FALSE, -1, 0, 0);
            } else {
                changeMotionFileName(Wyvern::GlideRightMotion, 0, FALSE, -1, 0, 0);
            }
        }
        break;
    case 2: {
        m_fRot += m_fRotDir * 2.5f * 0.017453292f * (getFrameRate() * 0.020000001f);
        m_fRot = Math3D::LimitPI(m_fRot);
        FMATRIX m;
        _unitMatrix(m);
        sceVu0RotMatrixY(m.m, m.m, m_fRot);
        FVECTOR vVel;
        vVel.set(0.0f, 0.0f, m_fSpeed);
        ApplyMatrix(vVel, m, vVel);
        m_pMove->setVel(vVel);
        ((FVECTOR&)getRot()).y = Math3D::LimitPI(atan2f(vVel.x, vVel.z));
        if (getTimer() > 0) break;
        changeMotion(m_iWalkEndMotIndex, 0, FALSE, -1, 0, 0);
        clearSpeed();
        m_iRotFlyStep = 3;
        break;
    }
    case 3:
        if (isMotionEnd()) {
            commonFlyInit(Wyvern::IdlingMotion, Wyvern::MoveLoopMotion, 0.5f, 1.5f);
            changeMotion(m_iIdlingMotIndex, 0, FALSE, -1, 0, 0);
            m_iRotFlyStep = 0;
            setBattleEnemyFlag(0, 0x800000000ULL);
        }
        break;
    }
}

void CEnemyWyvern::endStateMain() {
    commonFlyEnd();
    setBattleEnemyFlag(0, 0x800000000ULL);
}

void CEnemyWyvern::beginStateAttack1() {
    FVECTOR vPos;
    getPlayerPosition(vPos);
    if (calcDistanceXZSq(vPos) <= 4.0f) {
        changeMotionFileName(Wyvern::KickMotion, 0, FALSE, -1, 0, 0);
        m_tqAtkTimer = 500;
        m_iStep = 2;
    } else {
        changeMotionFileName(Wyvern::MoveStartMotion, 0, FALSE, -1, 0, 0);
        m_tqAtkTimer = 0;
        m_iStep = 0;
    }
}

void CEnemyWyvern::stateAttack1() {
    FVECTOR vPos;
    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.08726646f);
        if (isMotionEnd()) {
            changeMotionFileName(Wyvern::MoveLoopMotion, 0, FALSE, -1, 0, 0);
            m_iStep++;
        }
        break;
    case 1:
        getPlayerPosition(vPos);
        vPos.y += 0.5f;
        m_pMove->turnToTarget(vPos, 0.17453292f);
        m_pMove->calcVelocity(0.3f);
        if (calcDistanceXZSq(vPos) <= 1.0f) {
            m_tqAtkTimer = 500;
            CHAR*& rKick = Wyvern::KickMotion; /* through a reference: loaded after the timer store */
            changeMotionFileName(rKick, 0, FALSE, -1, 0, 0);
            m_iStep++;
        }
        break;
    case 2:
        if (isHitAttack()) {
            clearSpeed();
            m_iStep++;
            break;
        }
        m_pMove->calcVelocity(0.075f);
    case 3:
        turnToPlayerY(0.08726646f);
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

void CEnemyWyvern::endStateAttack1() {}

void CEnemyWyvern::beginStateAttack2() {
    changeMotionFileName(Wyvern::RushAttackStartMotion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyWyvern::stateAttack2() {
    FVECTOR vPos;
    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.17453292f);
        if (isMotionEnd()) {
            changeMotionFileName(Wyvern::RushAttackLoopMotion, 0, FALSE, -1, 0, 0);
            setTimer(750);
            m_iStep++;
        }
        break;
    case 1: {
        FLOAT fSpeed = getExtraParam(Wyvern::RushAtkSpdParamName);
        getPlayerPosition(vPos);
        vPos.y += 0.5f;
        m_pMove->turnToTarget(vPos, 0.08726646f);
        m_pMove->calcVelocity(fSpeed);
        if (getTimer() > 0) break;
        m_iStep++;
        break;
    }
    case 2:
        if (m_pMove->isWall()) {
            changeMotionFileName(Wyvern::RushAttackEndMotion, 0, FALSE, -1, 0, 0);
            clearSpeed();
            m_iStep++;
        }
        break;
    case 3:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

void CEnemyWyvern::endStateAttack2() {}

// SOURCE DATA: .data .rodata
