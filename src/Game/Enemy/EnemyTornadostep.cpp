
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyTornadostep.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyTornadostepEmptyString() {
    TRACE("");
}

#include "Game/Movement.h"

namespace Tornadostep {
static CHAR* IdlingMotion = "e029_001";
static CHAR* MoveMotion = "e029_002";
static CHAR* Attack1Motion = "e029_050";
static CHAR* Attack2Motion = "e029_051";
static CHAR* PropellerChargeMotion = "e029_052";
static CHAR* PropellerChargeEndMotion = "e029_054";
static CHAR* FlyingSpeedParamName = "FlyingSpeed";
}

CEnemyTornadostep::CEnemyTornadostep(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setHeartlessFlag(1, 0);
    setFallMotionIndex(3);
    setLandMotionIndex(4);
    m_iPropellerStep = 0;
}

CEnemyTornadostep::~CEnemyTornadostep() {}

void CEnemyTornadostep::beginStateMain() {
    commonMoveInit(Tornadostep::IdlingMotion, Tornadostep::MoveMotion, 2.0f);
}

void CEnemyTornadostep::stateMain() {
    switch (m_iPropellerStep) {
    case 0: {
        FVECTOR target;
        getPlayerPosition(target);
        if (calcDistanceXZSq(target) <= 56.25f) {
            commonMove();
            break;
        }
        clearSpeed();
        changeMotionFileName(Tornadostep::PropellerChargeMotion, 0, TRUE, -1, 0, 0);
        m_pMove->enableGravity(FALSE);
        setBattleEnemyFlag(0, 0x10);
        ((FVECTOR&)getRot()).y = calcDirection(target);
        m_iPropellerStep++;
        break;
    }
    case 1:
        if (isMotionEnd()) {
            m_iPropellerStep++;
        }
        break;
    case 2: {
        FVECTOR target;
        getPlayerPosition(target);
        m_pMove->turnToTargetY(target, 0.08726646f);
        m_pMove->calcVelocityXZ(getExtraParam(Tornadostep::FlyingSpeedParamName));
        if (calcDistanceXZSq(target) <= 4.0f) {
            FLOAT spd;
            FVECTOR v;
            clearSpeed();
            changeMotionFileName(Tornadostep::PropellerChargeEndMotion, 0, FALSE, -1, 0, 0);
            spd = getHeight() / (FLOAT)(getMotionTotalTime() / 50);
            v.set(0.0f, -spd, 0.0f);
            m_pMove->setVel(v);
            m_iPropellerStep++;
        }
        break;
    }
    case 3:
        if (isMotionEnd()) {
            clearSpeed();
            setBattleEnemyFlag(0x10, 0);
            m_pMove->enableGravity(TRUE);
            commonMoveInit(Tornadostep::IdlingMotion, Tornadostep::MoveMotion, 2.0f);
            m_iPropellerStep = 0;
        }
        break;
    }
}

void CEnemyTornadostep::endStateMain() {
    commonMoveEnd();
    m_pMove->enableGravity(TRUE);
    m_iPropellerStep = 0;
}

void CEnemyTornadostep::beginStateAttack1() {
    changeMotionFileName(Tornadostep::Attack1Motion, 0, TRUE, -1, 0, 0);
    m_pMove->enableGravity(FALSE);
    m_iStep = 0;
}

void CEnemyTornadostep::stateAttack1() {
    turnToPlayerY(0.17453292f);
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyTornadostep::endStateAttack1() {
    m_pMove->enableGravity(TRUE);
}

void CEnemyTornadostep::beginStateAttack2() {
    changeMotionFileName(Tornadostep::Attack2Motion, 0, TRUE, -1, 0, 0);
    m_pMove->enableGravity(FALSE);
    m_iStep = 0;
}

void CEnemyTornadostep::stateAttack2() {
    if (m_iStep == 0) {
        turnToPlayerY(0.17453292f);
        if (getMotionCurrentTime() >= 500) {
            m_iStep++;
        }
    }
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyTornadostep::endStateAttack2() {
    m_pMove->enableGravity(TRUE);
}

// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data .rodata
