
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemySoldier.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemySoldierEmptyString() {
    TRACE("");
}

#include "Game/Movement.h"
#include "System/Rnd.h"

namespace Soldier {
static CHAR* IdlingMotion = "e002_001";
static CHAR* MoveMotion = "e002_002";
static CHAR* JumpStartMotion = "e002_007";
static CHAR* JumpRiseMotion = "e002_008";
static CHAR* JumpTopMotion = "e002_009";
static CHAR* JumpFallMotion = "e002_010";
static CHAR* JumpLandMotion = "e002_011";
static CHAR* ScratchMotion = "e002_050";
static CHAR* RotKickMotion = "e002_051";
static CHAR* KickSpeedParamName = "KickSpeed";
}

CEnemySoldier::CEnemySoldier(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setFallMotionIndex(5);
    setLandMotionIndex(6);
}

CEnemySoldier::~CEnemySoldier() {}

void CEnemySoldier::beginStateMain() {
    commonMoveInit(Soldier::IdlingMotion, Soldier::MoveMotion, 2.0f);
}

void CEnemySoldier::stateMain() {
    commonMove();
}

void CEnemySoldier::endStateMain() {
    commonMoveEnd();
}

void CEnemySoldier::beginStateAttack1() {
    changeMotionFileName(Soldier::ScratchMotion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemySoldier::stateAttack1() {
    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.17453292f);
        if (isMotionEnd()) {
            if (!isBtlEnemyFlag(0x20000) && RND::genrand_int31() % 10 == 0) {
                changeMotionFileName(Soldier::JumpStartMotion, 0, FALSE, -1, 0, 0);
                m_iStep = 1;
            } else {
                endCardBattle(NULL, 6000);
            }
        }
        break;
    case 1:
        if (isMotionEnd()) {
            changeMotionFileName(Soldier::JumpRiseMotion, 0, FALSE, -1, 0, 0);
            m_pMove->calcOrbitVelocityFromDistance(1.2217305f, 3.0f);
            FVECTOR vel = m_pMove->getVel();
            vel.x = -vel.x;
            vel.z = -vel.z;
            m_pMove->setVel(vel);
            m_iStep = 2;
        }
        break;
    case 2:
        if (m_pMove->getVel().y <= 0.0f) {
            changeMotionFileName(Soldier::JumpTopMotion, 0, FALSE, -1, 0, 0);
            m_iStep = 3;
        }
        break;
    case 3:
        if (isMotionEnd()) {
            changeMotionFileName(Soldier::JumpFallMotion, 0, FALSE, -1, 0, 0);
            m_iStep = 4;
        }
        break;
    case 4:
        if (m_pMove->isGround()) {
            changeMotionFileName(Soldier::JumpLandMotion, 0, FALSE, -1, 0, 0);
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

void CEnemySoldier::beginStateAttack2() {
    changeMotionFileName(Soldier::RotKickMotion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemySoldier::stateAttack2() {
    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.17453292f);
        if (getMotionCurrentTime() >= 2500) {
            m_iStep = 1;
        }
        break;
    case 1:
        turnToPlayerY(0.17453292f);
        m_pMove->calcVelocityXZ(getExtraParam(Soldier::KickSpeedParamName));
        if (isBtlEnemyFlag(0x40) || getMotionCurrentTime() >= 5700) {
            m_iStep = 2;
        }
    case 2:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

// SOURCE DATA: .data .rodata
