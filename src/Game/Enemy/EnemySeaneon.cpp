
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemySeaneon.h"
#include "Game/Movement.h"

namespace Seaneon {
static CHAR* IdlingMotion = "e018_001";
static CHAR* Attack1Motion = "e018_050";
static CHAR* Attack2Motion = "e018_051";
}

CEnemySeaneon::CEnemySeaneon(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setEntityFlag(0x80, 0);
    setBtlEnemyFlag(0x100);
}

CEnemySeaneon::~CEnemySeaneon() {}

void CEnemySeaneon::beginStateMain() {
    commonFlyInit(Seaneon::IdlingMotion, Seaneon::IdlingMotion, 0.0f, 0.5f);
}

void CEnemySeaneon::stateMain() {
    commonFly();
}

void CEnemySeaneon::endStateMain() {
    commonFlyEnd();
}

void CEnemySeaneon::beginStateAttack1() {
    changeMotionFileName(Seaneon::Attack1Motion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
    m_fAtk1Spd = 0.0f;
}

void CEnemySeaneon::stateAttack1() {
    switch (m_iStep) {
    case 0:
        m_fAtk1Spd += getFrameRateF() * 0.0005f;
        if (m_fAtk1Spd > 0.05f) {
            m_fAtk1Spd = 0.05f;
            m_iStep = 1;
        }
        break;
    case 1:
        if (getMotionCurrentTime() >= 9000) {
            m_fAtk1Spd -= getFrameRateF() * 0.0005f;
            if (m_fAtk1Spd < 0.0f) {
                m_fAtk1Spd = 0.0f;
                m_iStep++;
            }
        }
        break;
    }
    turnToPlayerY(0.08726646f);
    m_pMove->calcVelocityXZ(m_fAtk1Spd);
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemySeaneon::beginStateAttack2() {
    changeMotionFileName(Seaneon::Attack2Motion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemySeaneon::stateAttack2() {
    if (m_iStep == 0) {
        playerHomingY(0.25f);
        if (getMotionCurrentTime() >= 3500) {
            m_iStep++;
        }
    }
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

// SOURCE DATA: .data .rodata
