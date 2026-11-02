
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyDarkball.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyDarkballEmptyString() {
    TRACE("");
}

#include "Game/Movement.h"

namespace Darkball {
static CHAR* IdlingMotion = "e023_001";
static CHAR* BiteAttack1Motion = "e023_050";
static CHAR* BiteAttack2Motion = "e023_051";
static CHAR* BiteAttack3Motion = "e023_052";
static CHAR* RanbuMotion = "e023_053";
static CHAR* BiteSpeedParamName = "BiteSpeed";
}

CEnemyDarkball::CEnemyDarkball(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setEntityFlag(0x80, 0);
    setBtlEnemyFlag(0x100);
}

CEnemyDarkball::~CEnemyDarkball() {}

BOOL CEnemyDarkball::initalize(INT inGroupID, INT inRsrcID) {
    CHeartless* pThis = this;
    return pThis->initalize(inGroupID, inRsrcID, 0) != 0;
}

void CEnemyDarkball::beginStateMain() {
    commonFlyInit(Darkball::IdlingMotion, Darkball::IdlingMotion, 0.0f, 1.0f);
}

void CEnemyDarkball::stateMain() {
    commonFly();
}

void CEnemyDarkball::endStateMain() {
    commonFlyEnd();
}

void CEnemyDarkball::beginStateAttack1() {
    changeMotionFileName(Darkball::BiteAttack1Motion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyDarkball::stateAttack1() {
    switch (m_iStep) {
    case 0:
        if (isMotionEnd()) {
            changeMotionFileName(Darkball::BiteAttack2Motion, 0, FALSE, -1, 0, 0);
            setTimer(12000);
            m_iStep++;
        }
        break;
    case 1:
        turnToPlayer(0.17453292f);
        m_pMove->calcVelocity(getExtraParam(Darkball::BiteSpeedParamName));
        if (getTimer() <= 0 || isBtlEnemyFlag(0x40)) {
            clearSpeed();
            changeMotionFileName(Darkball::BiteAttack3Motion, 0, FALSE, -1, 0, 0);
            m_iStep++;
        }
        break;
    case 2:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

void CEnemyDarkball::beginStateAttack2() {
    changeMotionFileName(Darkball::RanbuMotion, 0, TRUE, -1, 0, 0);
}

void CEnemyDarkball::stateAttack2() {
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

// SOURCE DATA: .data .rodata
