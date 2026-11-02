
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyWightnight.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyWightnightEmptyString() {
    TRACE("");
}

#include "Game/Movement.h"
#include "System/Rnd.h"

extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */

namespace Wightnight {
static CHAR* IdlingMotion = "e012_001";
static CHAR* MoveMotion = "e012_002";
static CHAR* JumpStartMotion = "e012_003";
static CHAR* JumpRiseMotion = "e012_004";
static CHAR* JumpTopMotion = "e012_005";
static CHAR* JumpFallMotion = "e012_006";
static CHAR* JumpLandMotion = "e012_007";
static CHAR* AttackMotion = "e012_052";
static CHAR* RotAttack1Motion = "e012_053";
static CHAR* RotAttack2Motion = "e012_054";
static CHAR* RotAttack3Motion = "e012_055";
}

CEnemyWightnight::CEnemyWightnight(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setHeartlessFlag(1, 0);
    setFallMotionIndex(6);
    setLandMotionIndex(7);
}

CEnemyWightnight::~CEnemyWightnight() {}

void CEnemyWightnight::beginStateMain() {
    commonMoveInit(Wightnight::IdlingMotion, Wightnight::MoveMotion, 2.0f);
}

void CEnemyWightnight::stateMain() {
    commonMove();
}

void CEnemyWightnight::endStateMain() {
    commonMoveEnd();
}

void CEnemyWightnight::beginStateAttack1() {
    changeMotionFileName(Wightnight::AttackMotion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyWightnight::stateAttack1() {
    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.17453292f);
        if (isMotionEnd()) {
            if (!isBtlEnemyFlag(0x20000) && RND::genrand_int31() % 10 == 0) {
                changeMotionFileName(Wightnight::JumpStartMotion, 0, FALSE, -1, 0, 0);
                m_iStep = 1;
            } else {
                endCardBattle(NULL, 6000);
            }
        }
        break;
    case 1:
        if (isMotionEnd()) {
            changeMotionFileName(Wightnight::JumpRiseMotion, 0, FALSE, -1, 0, 0);
            m_pMove->calcOrbitVelocityFromSpeedY(0.25f, 3.0f);
            FVECTOR vel = m_pMove->getVel();
            vel.x = -vel.x;
            vel.z = -vel.z;
            m_pMove->setVel(vel);
            m_iStep = 2;
        }
        break;
    case 2:
        if (m_pMove->getVel().y <= 0.0f) {
            changeMotionFileName(Wightnight::JumpTopMotion, 0, FALSE, -1, 0, 0);
            m_iStep = 3;
        }
        break;
    case 3:
        if (isMotionEnd()) {
            changeMotionFileName(Wightnight::JumpFallMotion, 0, FALSE, -1, 0, 0);
            m_iStep = 4;
        }
        break;
    case 4:
        if (m_pMove->isGround()) {
            changeMotionFileName(Wightnight::JumpLandMotion, 0, FALSE, -1, 0, 0);
            clearSpeed();
            m_iStep = 5;
        }
        break;
    case 5:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

void CEnemyWightnight::endStateAttack1() {}

void CEnemyWightnight::beginStateAttack2() {
    changeMotionFileName(Wightnight::RotAttack1Motion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyWightnight::stateAttack2() {
    switch (m_iStep) {
    case 0:
        m_pMove->turnToTargetY(g_pPlayer, 0.08726646f);
        if (isMotionEnd()) {
            changeMotionFileName(Wightnight::RotAttack2Motion, 0, TRUE, -1, 0, 0);
            m_iStep = 1;
        }
        break;
    case 1:
        if (isMotionEnd()) {
            changeMotionFileName(Wightnight::RotAttack3Motion, 0, FALSE, -1, 0, 0);
            m_iStep = 2;
        }
        break;
    case 2:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

void CEnemyWightnight::endStateAttack2() {}

// SOURCE DATA: .data .rodata
