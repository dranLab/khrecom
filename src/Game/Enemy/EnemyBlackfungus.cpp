#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyBlackfungus.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyBlackfungusEmptyString() {
    TRACE("");
}

#include "Game/BodyCollision.h"
#include "Game/Movement.h"
#include "System/Rnd.h"

namespace Blackfungus {
static CHAR* IdlingMotion = "e017_001";
static CHAR* MoveMotion = "e017_002";
static CHAR* DarkPoisonMotion = "e017_050";
static CHAR* PoisonTouchMotion = "e017_051";
static CHAR* PoisonTouchFailedMotion = "e017_052";
static CHAR* PoisonTouchSuccessMotion = "e017_053";
static CHAR* MetalMotion = "e017_054";
static CHAR* ReturnMotion = "e017_055";
static CHAR* MetalmodeParamName = "Metalmode";
}

CEnemyBlackfungus::CEnemyBlackfungus(CTask* inParent) : CHeartless(inParent, 0, 0) {
    m_bMetal = FALSE;
    m_tqMetalTimer = 0;
}

CEnemyBlackfungus::~CEnemyBlackfungus() {}

BOOL CEnemyBlackfungus::initalize(INT inGroupID, INT inRsrcID) {
    CHeartless* pThis = this;
    if (!pThis->initalize(inGroupID, inRsrcID, 0)) return FALSE;
    setDispOffFlag(2, 0);
    m_pBodyColl->setMuteki(-1);
    m_pBodyColl->setMutekiDir(3.1415927f);
    return TRUE;
}

void CEnemyBlackfungus::updateBefore() {
    CBattleEnemy::updateBefore();
    if (m_tqMetalTimer > 0) {
        m_tqMetalTimer -= getFrameRate();
    }
}

void CEnemyBlackfungus::beginStateMain() {
    commonMoveInit(Blackfungus::IdlingMotion, Blackfungus::MoveMotion, 1.5f);
    m_iBlackfungusStep = 0;
}

/* _ref: a global read through a reference stays after a member store (the original loads the motion names
 * after the flag store and the parameter name after the step store) */
template <class T> static inline T& _ref(T& x) { return x; }

void CEnemyBlackfungus::stateMain() {
    switch (m_iBlackfungusStep) {
    case 0:
        commonMove();
        if (m_bMetal) {
            if (m_tqMetalTimer > 0) break;
            changeMotionFileName(Blackfungus::ReturnMotion, 0, FALSE, -1, 0, 0);
            clearBtlEnemyFlag(0x10);
            m_iBlackfungusStep = 3;
            clearSpeed();
        } else if (RND::genrand_int31() % 100 == 0) {
            changeMotionFileName(Blackfungus::MetalMotion, 0, FALSE, -1, 0, 0);
            m_iBlackfungusStep = 1;
            clearSpeed();
        }
        break;
    case 1:
        if (getMotionCurrentTime() >= 3850) {
            clearBtlEnemyFlag(0x10);
            setBtlEnemyFlag(0x2000);
            clearBtlEnemyFlag(0x10);
            m_pBodyColl->setMuteki(0);
            m_pBodyColl->setMuteki(1);
            m_pBodyColl->setMuteki(2);
            m_pBodyColl->setMuteki(3);
            m_pBodyColl->setMuteki(4);
            m_pBodyColl->setMuteki(5);
            m_bMetal = TRUE;
            m_iBlackfungusStep = 2;
            setDispOffFlag(1, 2);
        }
    case 2:
        if (isMotionEnd()) {
            setBtlEnemyFlag(0x10);
            commonMoveInit(_ref(Blackfungus::IdlingMotion), _ref(Blackfungus::MoveMotion), 1.5f);
            m_iBlackfungusStep = 0;
            m_tqMetalTimer = (TIMEQ)(getExtraParam(_ref(Blackfungus::MetalmodeParamName)) * 60.0f * 50.0f);
        }
        break;
    case 3:
        if (getMotionCurrentTime() >= 2750) {
            setDispOffFlag(2, 1);
            setBtlEnemyFlag(0x10);
            clearBtlEnemyFlag(0x2000);
            m_pBodyColl->setMuteki(-1);
            m_bMetal = FALSE;
            m_iBlackfungusStep = 4;
        }
    case 4:
        if (isMotionEnd()) {
            commonMoveInit(Blackfungus::IdlingMotion, Blackfungus::MoveMotion, 1.5f);
            m_iBlackfungusStep = 0;
        }
        break;
    }
}

void CEnemyBlackfungus::endStateMain() {
    if (!m_bMetal) {
        setBtlEnemyFlag(0x10);
        clearBtlEnemyFlag(0x2000);
    }
}

void CEnemyBlackfungus::beginStateAttack1() {
    changeMotionFileName(Blackfungus::DarkPoisonMotion, 0, FALSE, -1, 0, 0);
}

void CEnemyBlackfungus::stateAttack1() {
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyBlackfungus::beginStateAttack2() {
    changeMotionFileName(Blackfungus::PoisonTouchMotion, 0, FALSE, -1, 0, 0);
    m_bPoisonTouch = FALSE;
    m_iStep = 0;
}

void CEnemyBlackfungus::stateAttack2() {
    if (isBtlEnemyFlag(0x40)) {
        m_bPoisonTouch = TRUE;
    }
    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.17453292f);
        if (isMotionEnd()) {
            if (m_bPoisonTouch) {
                changeMotionFileName(Blackfungus::PoisonTouchSuccessMotion, 0, FALSE, -1, 0, 0);
            } else {
                changeMotionFileName(Blackfungus::PoisonTouchFailedMotion, 0, FALSE, -1, 0, 0);
            }
            m_iStep++;
        }
        break;
    case 1:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

// SOURCE DATA: .data .rodata
