
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyPirate.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyPirateEmptyString() {
    TRACE("");
}

#include "Game/Movement.h"
#include "System/Rnd.h"

namespace Pirate {
static CHAR* IdlingMotion = "e021_001";
static CHAR* MoveMotion = "e021_002";
static CHAR* JumpStartMotion = "e021_003";
static CHAR* JumpTopMotion = "e021_004";
static CHAR* JumpFallMotion = "e021_005";
static CHAR* JumpLandMotion = "e021_006";
static CHAR* MoveAttackMotion = "e021_050";
static CHAR* ChargeAttackMotion = "e021_051";
}

CEnemyPirate::CEnemyPirate(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setHeartlessFlag(1, 0);
    setFallMotionIndex(5);
    setLandMotionIndex(6);
}

CEnemyPirate::~CEnemyPirate() {}

void CEnemyPirate::beginStateMain() {
    commonMoveInit(Pirate::IdlingMotion, Pirate::MoveMotion, 2.0f);
}

void CEnemyPirate::stateMain() {
    commonMove();
}

void CEnemyPirate::endStateMain() {
    commonMoveEnd();
}

void CEnemyPirate::beginStateAttack1() {
    changeMotionFileName(Pirate::MoveAttackMotion, 0, FALSE, -1, 0, 0);
    setTimer(12000);
}

void CEnemyPirate::stateAttack1() {
    turnToPlayerY(0.17453292f);
    m_pMove->calcVelocityXZ(getExtraParam("Speed"));
    if (getTimer() <= 0) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyPirate::beginStateAttack2() {
    changeMotionFileName(Pirate::ChargeAttackMotion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyPirate::stateAttack2() {
    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.08726646f);
        if (isMotionEnd()) {
            if (!isBtlEnemyFlag(0x20000) && RND::genrand_int31() % 10 == 0) {
                changeMotionFileName(Pirate::JumpStartMotion, 0, FALSE, -1, 0, 0);
                m_pMove->calcOrbitVelocityFromDistance(1.2217305f, 3.0f);
                FVECTOR vel = m_pMove->getVel();
                vel.x = -vel.x;
                vel.z = -vel.z;
                m_pMove->setVel(vel);
                m_iStep++;
            } else {
                endCardBattle(NULL, 6000);
            }
        }
        break;
    case 1:
        if (m_pMove->getVel().y <= 0.0f) {
            changeMotionFileName(Pirate::JumpTopMotion, 0, FALSE, -1, 0, 0);
            m_iStep++;
        }
        break;
    case 2:
        if (isMotionEnd()) {
            changeMotionFileName(Pirate::JumpFallMotion, 0, FALSE, -1, 0, 0);
            m_iStep++;
        }
        break;
    case 3:
        if (m_pMove->isGround()) {
            changeMotionFileName(Pirate::JumpLandMotion, 0, FALSE, -1, 0, 0);
            clearSpeed();
            m_iStep++;
        }
        break;
    case 4:
        if (isMotionEnd()) {
            changeMotionFileName(Pirate::IdlingMotion, 0, FALSE, -1, 0, 0);
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

// SOURCE DATA: .data .rodata
