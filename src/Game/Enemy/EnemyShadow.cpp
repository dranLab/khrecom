#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyShadow.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyShadowEmptyString() {
    TRACE("");
}

#include "Game/Enemy/EnemyParam.h"

/* Placeholders for an unknown header's inline functions (prize messages) that come next in .rodata. */
inline void _premiumPrizeMessage() {
    TRACE("\x83v\x83\x8c\x83~\x83" "A\x83\x80\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}
inline void _roulettePrizeMessage() {
    TRACE("\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}

#include "Game/BodyCollision.h"
#include "Game/Movement.h"
#include "Game/Enemy/MonstroEventBattle.h"
#include "System/TaskMan.h"
#include "System/Math3D.h"
#include "System/Rnd.h"
#include <math.h>
extern "C" int rand();

extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */
FVECTOR getPlayerPos(BOOL flag);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

class CBattleEnemyManager { /* a CTask (Game/Enemy/BattleEnemyManager.h) */
public:
    void monstroEventBattleGaugeUp();
};

namespace Shadow {
static CHAR* IdlingMotion = "e001_001";
static CHAR* MoveMotion = "e001_003";
static CHAR* JumpMotion = "e001_004";
static CHAR* FallMotion = "e001_005";
static CHAR* LandMotion = "e001_006";
static CHAR* DigMotion = "e001_010";
static CHAR* AppearMotion = "e001_011";
static CHAR* ScratchMotion = "e001_050";
static CHAR* JumpAttackMotion = "e001_052";
}

CEnemyShadow::CEnemyShadow(CTask* inParent) : CHeartless(inParent, NULL, NULL) {
    setFallMotionIndex(5);
    setLandMotionIndex(6);
    m_iDigStep = 0;
    m_hDeadDemo = 0;
}

CEnemyShadow::~CEnemyShadow() {}

BOOL CEnemyShadow::initalize(INT inGroupID, INT inRsrcID) {
    return CHeartless::initalize(inGroupID, inRsrcID) != FALSE;
}

void CEnemyShadow::updateAfter() {
    CBattleEnemy::updateAfter();
    FVECTOR s0;
    FVECTOR s1;
    FVECTOR s2;
    switch (m_iDigStep) {
    case 0:
        return;
    case 1:
        s1.set(1.0f, 1.0f, 1.0f, 1.0f);
        s2.set(1.0f, 0.1f, 1.0f, 1.0f);
        break;
    case 2:
        s1.set(1.0f, 0.1f, 1.0f, 1.0f);
        s2.set(1.0f, 1.0f, 1.0f, 1.0f);
        break;
    }
    FLOAT t = 1.0f - (FLOAT)m_tqDigTimer / (FLOAT)m_tqDigTime;
    if (t <= 0.0f) t = 0.0f;
    Math3D::VectorLerp(s0, s1, s2, t);
    m_pMove->setScale(s0);
    m_tqDigTimer -= getFrameRate();
    if (m_tqDigTimer <= 0) {
        m_iDigStep = 0;
        m_pMove->setScale(s2);
    }
}

void CEnemyShadow::beginStateAppear() {
    setEntityFlag(0, 1);
    if (isBtlEnemyFlag(8) || isBtlEnemyFlag(0x20000000000ULL)) {
        setEntityFlag(0x10000, 0);
        changeMotion(getEnemyParam()->iFaintMotionIndex[1], 0, FALSE, -1, 0, 0);
        if (isBtlEnemyFlag(8)) {
            setTimer(15000);
            setBadStatusEffect(1, 0.0f);
        } else {
            setTimer(18000);
            setBadStatusEffect(5, 0.0f);
        }
    } else {
        changeMotionFileName(Shadow::AppearMotion, 0, FALSE, -1, 0, 0);
        bodyCollOff(-1);
        bodyCollOffBody(-1);
        setupDig_(2, getMotionTotalTime());
    }
    m_iStep = 0;
}

void CEnemyShadow::stateAppear() {
    if (isBtlEnemyFlag(8) || isBtlEnemyFlag(0x20000000000ULL)) {
        switch (m_iStep) {
        case 0:
            if (getTimer() <= 0) {
                changeMotion(getEnemyParam()->iFaintMotionIndex[2], 0, FALSE, -1, 0, 0);
                killBadStatusEffect();
                m_iStep++;
            }
            break;
        case 1:
            if (isMotionEnd()) changeDefaultState();
            break;
        }
    } else {
        switch (m_iStep) {
        case 0:
            m_pMove->homingY(g_pPlayer, 0.1f);
            if (isMotionEnd()) {
                if (!CMonstroEventBattle::isMonstroEventBattleScene() || !CMonstroEventBattle::isHelp()) {
                    changeDefaultState();
                } else {
                    CMonstroEventBattle::addAppearEnemyCount();
                    changeMotionFileName(Shadow::IdlingMotion, 0, FALSE, -1, 0, 0);
                    m_iStep++;
                }
            }
            break;
        case 1:
            if (!CMonstroEventBattle::isHelp()) changeDefaultState();
            break;
        }
    }
}

void CEnemyShadow::endStateAppear() {
    bodyCollOn(-1);
    bodyCollOnBody(-1);
}

void CEnemyShadow::beginStateMain() {
    commonMoveInit(Shadow::IdlingMotion, Shadow::MoveMotion, 1.5f);
}

void CEnemyShadow::stateMain() {
    FVECTOR aim;
    switch (m_iStep) {
    case 0:
        if (rand() % m_iWaitRate == 0) {
            changeMotion(m_iWalkMotIndex, 0, FALSE, -1, 0, 0);
            randAimPos(m_vAimPos, m_fRandMax);
            m_iCount = 0;
            m_iStep = 1;
        } else if (!isBtlEnemyFlag(0x800) && !isBtlEnemyFlag(1)) {
            if (rand() % (INT)getExtraParam("DigRate") == 0) {
                changeMotionFileName(Shadow::DigMotion, 0, FALSE, -1, 0, 0);
                bodyCollOff(-1);
                bodyCollOffBody(-1);
                setupDig_(1, 1000);
                clearBtlEnemyFlag(0x10);
                m_iStep = 2;
            }
        }
        break;
    case 1:
        if (m_pMove->isWall() && m_iCount <= 0) {
            FLOAT theta = Math3D::LimitPI(m_pMove->getRot().y + 3.1415927f);
            FLOAT len = m_fRandMax + m_fRandMax + rndArea(-m_fRandMax, m_fRandMax);
            m_vAimPos.x = len * sinf(theta);
            m_vAimPos.y = 0;
            m_vAimPos.z = len * cosf(theta);
            m_iCount = 60;
        } else if (m_iCount > 0) {
            m_iCount--;
        }
        if (rand() % 100 == 0) randAimPos(m_vAimPos, m_fRandMax);
        if (calcDistanceXZSq(g_pPlayer) > 2.25f) {
            aim = getPlayerPos(FALSE);
            AddVectorXYZ(aim, aim, m_vAimPos);
            if (!arriveTarget(aim, 1.0f)) {
                m_pMove->homingMoveXZ(aim, 0.1f, m_fSpeed);
                break;
            }
        }
        changeMotion(m_iIdlingMotIndex, 0, FALSE, -1, 0, 0);
        clearSpeed();
        m_iStep = 0;
        break;
    case 2:
        if (getMotionCurrentTime() >= 1000) {
            FLOAT theta = Math3D::LimitPI(RND::genrand_real1() * 360.0f * 0.017453292f);
            FLOAT len = RND::genrand_real1() + 2.0f;
            FLOAT x = len * sinf(theta);
            FLOAT z = len * cosf(theta);
            FVECTOR v;
            v.set(x, 0.0f, z);
            AddVectorXYZ(m_vAimPos, v, getPos());
            setTimer(6000);
            m_iStep = 3;
        }
        break;
    case 3:
        m_pMove->homingMoveXZ(m_vAimPos, 0.1f, m_fSpeed);
        if (arriveTarget(m_vAimPos, 1.0f) || getTimer() <= 0) {
            m_vAimPos = getPlayerPos(FALSE);
            setTimer(6000);
            m_iStep = 4;
        }
        break;
    case 4:
        m_pMove->homingMoveXZ(m_vAimPos, 0.1f, m_fSpeed);
        if (arriveTarget(m_vAimPos, 1.5f) || getTimer() <= 0) {
            changeMotionFileName("e001_011", 0, FALSE, -1, 0, 0);
            setupDig_(2, getMotionTotalTime());
            bodyCollOnBody(-1);
            clearSpeed();
            m_iStep = 5;
        }
        break;
    case 5:
        if (isMotionEnd()) {
            changeMotionFileName(Shadow::IdlingMotion, 0, FALSE, -1, 0, 0);
            bodyCollOn(-1);
            m_iStep = 0;
            setBtlEnemyFlag(0x10);
        }
        break;
    }
}

void CEnemyShadow::endStateMain() {}

void CEnemyShadow::beginStateAttack1() {
    changeMotionFileName(Shadow::ScratchMotion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyShadow::stateAttack1() {
    if (m_iStep == 0) {
        turnToPlayerY(0.17453292f);
        if (getMotionCurrentTime() >= 1700) m_iStep++;
    }
    if (isMotionEnd()) endCardBattle(NULL, 6000);
}

void CEnemyShadow::beginStateAttack2() {
    changeMotionFileName(Shadow::JumpMotion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyShadow::stateAttack2() {
    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.17453292f);
        if (getMotionCurrentTime() < 1750) break;
        changeMotionFileName(Shadow::JumpAttackMotion, 0, FALSE, -1, 0, 0);
        if (isBtlEnemyFlag(0x20000)) {
            m_iStep = 3;
        } else {
            m_pMove->calcOrbitVelocityFromSpeedY(0.15f, calcDistanceXZ(g_pPlayer) - 0.5f);
            m_iStep = 2;
        }
        break;
    case 1:
        if (isMotionEnd() || m_pMove->getVel().y <= 0.0f) {
            changeMotionFileName(Shadow::JumpAttackMotion, 0, FALSE, -1, 0, 0);
            m_iStep = 2;
        }
        break;
    case 2:
        if (m_pMove->isGround()) {
            changeMotionFileName(Shadow::LandMotion, 0, FALSE, -1, 0, 0);
            clearSpeed();
            m_iStep = 3;
        }
        break;
    case 3:
        if (isMotionEnd()) endCardBattle(NULL, 6000);
        break;
    }
}

#line 525
void CEnemyShadow::beginStateDead() {
    if (CMonstroEventBattle::isMonstroEventBattleScene()) {
        CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(25);
        if (!bem) AssertApp(__FILE__, 531, "bem", 0);
        bem->monstroEventBattleGaugeUp();
    }
    CBattleEnemy::beginStateDead();
}

void CEnemyShadow::stateDead() {
    if (!CTaskMan::getTask(m_hDeadDemo)) CBattleEnemy::stateDead();
}

void CEnemyShadow::endStateDead() {
    if (!CTaskMan::getTask(m_hDeadDemo)) CBattleEnemy::endStateDead();
}

// SOURCE DATA: .data .rodata
