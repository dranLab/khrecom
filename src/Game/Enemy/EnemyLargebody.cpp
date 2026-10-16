#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyLargebody.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyLargebodyEmptyString() {
    TRACE("");
}

#include "Game/AttackMotion.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/WorldEnv.h"

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

namespace Largebody {
static CHAR* IdlingMotion = "e009_001";
static CHAR* MoveMotion = "e009_002";
static CHAR* Attack1Motion = "e009_050";
static CHAR* Attack2Motion = "e009_053";
}

CEnemyLargebody::CEnemyLargebody(CTask* inParent) : CHeartless(inParent, 0, 0) {
    m_fBodyRviseCoefficient = 0.1f;
}

CEnemyLargebody::~CEnemyLargebody() {}

void CEnemyLargebody::beginStateMain() {
    commonMoveInit(Largebody::IdlingMotion, Largebody::MoveMotion, 2.0f);
}

void CEnemyLargebody::stateMain() {
    commonMove();
}

void CEnemyLargebody::endStateMain() {
    commonMoveEnd();
}

void CEnemyLargebody::beginStateAttack1() {
    changeMotionFileName(Largebody::Attack1Motion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyLargebody::stateAttack1() {
    if (m_iStep == 0) {
        playerHomingY(0.35f);
        if (getMotionCurrentTime() >= 4600) {
            m_iStep++;
        }
    }
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyLargebody::beginStateAttack2() {
    changeMotionFileName(Largebody::Attack2Motion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
    m_fAtkCollRadius = 0.0f;
}

void CEnemyLargebody::stateAttack2() {
    switch (m_iStep) {
    case 0:
        if (getMotionCurrentTime() >= 4500) {
            CCamera* pCamera = CWorldEnv::getActiveCamera();
            CameraQuakeParam param;
            param._fCycle = 10.0f;
            param._nTime = 50;
            param._fPower = 0.25f;
            new (com_malloc_d(sizeof(CCameraQuakeV), __FILE__ "(105)", RET_ADDR())) CCameraQuakeV(*pCamera, &param, 0.25f);
            m_iStep = 1;
        }
        break;
    case 1:
        m_fAtkCollRadius += getFrameRateF() * 0.3f;
        m_pAttack->setRadius(m_fAtkCollRadius);
        if (getMotionCurrentTime() >= 6500) {
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
