
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyAirpirate.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyAirpirateEmptyString() {
    TRACE("");
}

#include "Game/Enemy/EnemyParam.h"
#include "Game/Movement.h"
#include "Game/Debug/DebugWork.h"
#include "System/Rnd.h"

FVECTOR getPlayerPos(BOOL flag);

namespace Airpirate {
static CHAR* IdlingMotion = "e022_001";
static CHAR* MoveMotion = "e022_002";
static CHAR* UppercutMotion = "e022_050";
static CHAR* ChargePunchMotion = "e022_051";
}

CEnemyAirpirate::CEnemyAirpirate(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setEntityFlag(0x80, 0);
    setBattleEnemyFlag(0x100, 0);
    setHeartlessFlag(1, 0);
}

CEnemyAirpirate::~CEnemyAirpirate() {}

void CEnemyAirpirate::beginStateMain() {
    commonFlyInit(Airpirate::IdlingMotion, Airpirate::MoveMotion, 0.5f, 1.25f);
}

void CEnemyAirpirate::stateMain() {
    commonFly();
}

void CEnemyAirpirate::endStateMain() {
    commonFlyEnd();
}

void CEnemyAirpirate::beginStateAttack1() {
    changeMotionFileName(Airpirate::UppercutMotion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyAirpirate::stateAttack1() {
    switch (m_iStep) {
    case 0:
        if (getMotionCurrentTime() >= 1000) m_iStep = 1;
    case 1:
        m_pMove->calcVelocityXZ(0.05f);
        if (getMotionCurrentTime() >= 2500) {
            clearSpeed();
            m_iStep = 2;
        }
        turnToPlayerY(0.17453292f);
    case 2:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

void CEnemyAirpirate::beginStateAttack2() {
    changeMotionFileName(Airpirate::ChargePunchMotion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyAirpirate::stateAttack2() {
    switch (m_iStep) {
    case 0:
        if (getMotionCurrentTime() >= 1800) m_iStep = 1;
    case 1:
        m_pMove->calcVelocityXZ(0.075f);
        if (getMotionCurrentTime() >= 2500) {
            clearSpeed();
            m_iStep = 2;
        }
        turnToPlayerY(0.17453292f);
    case 2:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

void CEnemyAirpirate::aiCardBattle() {
    if (RND::genrand_int31() % getAttackRate() != 0) return;
    FVECTOR plPos = getPlayerPos(FALSE);
    ENEMY_PARAM* param = getEnemyParam();
    INT count;
    int act[2];
    count = 0;
    double dy = plPos.y - getPos().y;
    if (dy < 0) dy = 0 - dy;
    if (dy < 1.0) {
        if (canAttack(0)) {
            act[count++] = param->pActs[0].iActNo;
        }
    } else {
        if (canAttack(1)) {
            act[count++] = param->pActs[1].iActNo;
        }
    }
    if (count == 0) return;
    if (!canCardBattle()) return;
    if (KH_DW(DebugWork::g_dw.enemy.uiFlag) & 8) {
        for (INT i = 0; i < param->iActCount; i++) {
            if (param->pActs[i].iActNo == KH_DW(DebugWork::g_dw.enemy.sActNo)) {
                setBattleEnemyFlag(0x200, 0);
                setAttackState(KH_DW(DebugWork::g_dw.enemy.sActNo) + 2);
                return;
            }
        }
        return;
    }
    setBattleEnemyFlag(0x200, 0);
    setAttackState(act[RND::genrand_int31() % count] + 2);
}

// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data .rodata
