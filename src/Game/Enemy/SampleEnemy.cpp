
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/SampleEnemy.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _sampleEnemyEmptyString() {
    TRACE("");
}

#include "Game/Movement.h"

CSampleEnemy::CSampleEnemy(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setHp(1);
}

CSampleEnemy::~CSampleEnemy() {}

BOOL CSampleEnemy::initalize(INT inGroupID, INT inRsrcID) {
    CHeartless* pThis = this;
    if (!pThis->initalize(inGroupID, inRsrcID, 0)) return FALSE;
    m_pMove->setFlag(1, 0);
    return TRUE;
}

void CSampleEnemy::beginStateMain() {
    setBattleEnemyFlag(0, 0x10);
    changeMotion(1, 0, FALSE, -1, 0, 0);
}

void CSampleEnemy::stateMain() {}

void CSampleEnemy::endStateMain() {}

void CSampleEnemy::beginStateAttack1() {}

void CSampleEnemy::stateAttack1() {}

void CSampleEnemy::endStateAttack1() {}

void CSampleEnemy::beginStateAttack2() {}

void CSampleEnemy::stateAttack2() {}

void CSampleEnemy::endStateAttack2() {}

CSampleEnemy2::CSampleEnemy2(CTask* inParent) : CHeartless(inParent, m_State, m_State) {
    m_State[0].set(this, &CSampleEnemy2::beginIdle, &CSampleEnemy2::idle, &CSampleEnemy2::endIdle);
    m_State[1].set(this, &CSampleEnemy2::beginMove, &CSampleEnemy2::move, &CSampleEnemy2::endMove);
}

CSampleEnemy2::~CSampleEnemy2() {}

BOOL CSampleEnemy2::initalize(INT inGroupID, INT inRsrcID) {
    CHeartless* pThis = this;
    return pThis->initalize(inGroupID, inRsrcID, 0) != 0;
}

void CSampleEnemy2::beginIdle() {}

void CSampleEnemy2::idle() {}

void CSampleEnemy2::endIdle() {}

void CSampleEnemy2::beginMove() {}

void CSampleEnemy2::move() {}

void CSampleEnemy2::endMove() {}

void CSampleEnemy2::beginStateAttack1() {}

void CSampleEnemy2::stateAttack1() {}

void CSampleEnemy2::endStateAttack1() {}

void CSampleEnemy2::beginStateAttack2() {}

void CSampleEnemy2::stateAttack2() {}

void CSampleEnemy2::endStateAttack2() {}

// SOURCE DATA: .data .rodata
