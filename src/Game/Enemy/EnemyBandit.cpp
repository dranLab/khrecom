#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyBandit.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyBanditEmptyString() {
    TRACE("");
}

#include "Game/Movement.h"
#include "Game/Collision/CollMap.h"
#include "System/Graphic/Camera.h"

void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

namespace Bandit {
static CHAR* IdlingMotion = "e011_001";
static CHAR* MoveMotion = "e011_002";
static CHAR* FurioroshiMotion = "e011_050";
static CHAR* RollingTackle1Motion = "e011_051";
static CHAR* RollingTackle2Motion = "e011_052";
static CHAR* RollingTackle3Motion = "e011_053";
}

CEnemyBandit::CEnemyBandit(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setHeartlessFlag(1, 0);
}

CEnemyBandit::~CEnemyBandit() {}

void CEnemyBandit::beginStateMain() {
    commonMoveInit(Bandit::IdlingMotion, Bandit::MoveMotion, 2.0f);
}

void CEnemyBandit::stateMain() {
    commonMove();
}

void CEnemyBandit::endStateMain() {
    commonMoveEnd();
}

void CEnemyBandit::beginStateAttack1() {
    changeMotionFileName(Bandit::FurioroshiMotion, 0, TRUE, -1, 0, 0);
}

void CEnemyBandit::stateAttack1() {
    turnToPlayerY(0.17453292f);
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyBandit::beginStateAttack2() {
    changeMotionFileName(Bandit::RollingTackle1Motion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyBandit::stateAttack2() {
    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.17453292f);
        if (isMotionEnd()) {
            changeMotionFileName(Bandit::RollingTackle2Motion, 0, FALSE, -1, 0, 0);
            setTimer(750);
            m_iStep++;
        }
        break;
    case 1:
        turnToPlayerY(0.04363323f);
        m_pMove->calcVelocityXZ(0.3f);
        if (getTimer() <= 0) {
            setTimer(20000);
            m_iStep++;
        }
        break;
    case 2:
        if (getTimer() <= 0 || checkFront_()) {
            changeMotionFileName(Bandit::RollingTackle3Motion, 0, FALSE, -1, 0, 0);
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

BOOL CEnemyBandit::checkFront_() {
    FVECTOR start;
    FVECTOR end;
    CCollMap* coll;
    INT result;

    start = getPos();
    end.set(0.0f, 0.0f, 1.5f);
    ApplyMatrix(end, Crd(0).getWorldMatrix(), end);
    coll = m_pMove->getMapCollision();
    coll->setRadius(0.0f);
    coll->setStartPos(start);
    coll->setEndPos(end);
    result = coll->collideMap();
    coll->setRadius(0.5f);
    if (result) {
        if (coll->getDetectNormal()->y < 0.64278764f) return TRUE;
        if (coll->getOutStatus() == 1) return TRUE;
    }
    return FALSE;
}

// SOURCE DATA: .data .rodata
