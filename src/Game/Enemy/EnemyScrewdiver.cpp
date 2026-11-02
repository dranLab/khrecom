
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyScrewdiver.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyScrewdiverEmptyString() {
    TRACE("");
}

#include "Game/Movement.h"
#include "System/Rnd.h"

namespace Screwdiver {
static CHAR* IdlingMotion = "e019_001";
static CHAR* MoveMotion = "e019_002";
static CHAR* BackMoveMotion = "e019_003";
static CHAR* Attack1Motion = "e019_050";
static CHAR* Attack2Motion = "e019_051";
}

CEnemyScrewdiver::CEnemyScrewdiver(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setEntityFlag(0x80, 0);
    setBtlEnemyFlag(0x100);
}

CEnemyScrewdiver::~CEnemyScrewdiver() {}

BOOL CEnemyScrewdiver::initalize(INT inGroupID, INT inRsrcID) {
    if (!CHeartless::initalize(inGroupID, inRsrcID)) {
        return FALSE;
    }
    m_pMove->setShiftY(0.0f);
    return TRUE;
}

void CEnemyScrewdiver::beginStateMain() {
    commonFlyInit(Screwdiver::IdlingMotion, Screwdiver::MoveMotion, 0.25f, 0.75f);
}

void CEnemyScrewdiver::stateMain() {
    commonFly();
}

void CEnemyScrewdiver::endStateMain() {
    commonFlyEnd();
}

void CEnemyScrewdiver::beginStateAttack1() {
    changeMotionFileName(Screwdiver::Attack1Motion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyScrewdiver::stateAttack1() {
    switch (m_iStep) {
    case 0:
        playerHomingY(0.25f);
        if (getMotionCurrentTime() >= 1400) {
            m_iStep = 1;
        }
    case 1:
        if (isMotionEnd()) {
            if (RND::genrand_int31() % 10 == 0) {
                changeMotionFileName(Screwdiver::BackMoveMotion, 0, TRUE, -1, 0, 0);
                m_iStep = 2;
            } else {
                endCardBattle(NULL, 6000);
            }
        }
        break;
    case 2:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

void CEnemyScrewdiver::beginStateAttack2() {
    changeMotionFileName(Screwdiver::Attack2Motion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyScrewdiver::stateAttack2() {
    turnToPlayerY(0.17453292f);
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

// SOURCE DATA: .data .rodata
