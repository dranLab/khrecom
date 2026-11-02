
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyPowerwild.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyPowerwildEmptyString() {
    TRACE("");
}

#include "Game/Movement.h"
#include "System/Rnd.h"

namespace Powerwild {
static CHAR* IdlingMotion = "e007_001";
static CHAR* MoveMotion = "e007_002";
static CHAR* JumpStartMotion = "e007_003";
static CHAR* JumpRiseMotion = "e007_004";
static CHAR* JumpTopMotion = "e007_005";
static CHAR* JumpFallMotion = "e007_006";
static CHAR* JumpLandMotion = "e007_007";
static CHAR* ScratchMotion = "e007_050";
static CHAR* SlidingStartMotion = "e007_051";
static CHAR* SlidingEndMotion = "e007_052";
}

CEnemyPowerwild::CEnemyPowerwild(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setHeartlessFlag(1, 0);
    setFallMotionIndex(6);
    setLandMotionIndex(7);
}

CEnemyPowerwild::~CEnemyPowerwild() {}

void CEnemyPowerwild::beginStateMain() {
    commonMoveInit(Powerwild::IdlingMotion, Powerwild::MoveMotion, 1.5f);
}

void CEnemyPowerwild::stateMain() {
    commonMove();
}

void CEnemyPowerwild::endStateMain() {
    commonMoveEnd();
}

void CEnemyPowerwild::beginStateAttack1() {
    changeMotionFileName(Powerwild::SlidingStartMotion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyPowerwild::stateAttack1() {
    switch (m_iStep) {
    case 0:
        playerHomingY(0.25f);
        if (getMotionCurrentTime() >= 1600) {
            m_pMove->calcVelocity(0.075f);
            m_iStep = 1;
        }
    case 1:
        if (isMotionEnd()) {
            changeMotionFileName(Powerwild::SlidingEndMotion, 0, FALSE, -1, 0, 0);
            clearSpeed();
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

void CEnemyPowerwild::beginStateAttack2() {
    changeMotionFileName(Powerwild::ScratchMotion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyPowerwild::stateAttack2() {
    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.17453292f);
        if (isMotionEnd()) {
            if (!isBtlEnemyFlag(0x20000) && RND::genrand_int31() % 10 == 0) {
                changeMotionFileName(Powerwild::JumpStartMotion, 0, FALSE, -1, 0, 0);
                m_iStep++;
            } else {
                endCardBattle(NULL, 6000);
            }
        }
        break;
    case 1:
        if (isMotionEnd()) {
            changeMotionFileName(Powerwild::JumpRiseMotion, 0, FALSE, -1, 0, 0);
            m_pMove->calcOrbitVelocityFromDistance(1.0471976f, 3.0f);
            FVECTOR vel = m_pMove->getVel();
            vel.x = -vel.x;
            vel.z = -vel.z;
            m_pMove->setVel(vel);
            m_iStep++;
        }
        break;
    case 2:
        if (m_pMove->getVel().y <= 0.0f) {
            changeMotionFileName(Powerwild::JumpTopMotion, 0, FALSE, -1, 0, 0);
            m_iStep++;
        }
        break;
    case 3:
        if (isMotionEnd()) {
            changeMotionFileName(Powerwild::JumpFallMotion, 0, FALSE, -1, 0, 0);
            m_iStep++;
        }
        break;
    case 4:
        if (m_pMove->isGround()) {
            changeMotionFileName(Powerwild::JumpLandMotion, 0, FALSE, -1, 0, 0);
            clearSpeed();
            m_iStep++;
        }
        break;
    case 5:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

// SOURCE DATA: .data .rodata
