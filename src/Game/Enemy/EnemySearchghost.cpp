
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemySearchghost.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemySearchghostEmptyString() {
    TRACE("");
}

#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Effect/EffectMan.h"

namespace Searchghost {
static CHAR* IdlingMotion = "e014_001";
static CHAR* HandslamMotion = "e014_050";
static CHAR* HpAbsorbMotion = "e014_051";
}

CEnemySearchghost::CEnemySearchghost(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setEntityFlag(0x80, 0);
    setBattleEnemyFlag(0x100, 0);
    m_hAbsorb = 0;
}

CEnemySearchghost::~CEnemySearchghost() {
    CEffectMan::killEffect(m_hAbsorb, 0.0f);
}

void CEnemySearchghost::beginStateMain() {
    commonFlyInit(Searchghost::IdlingMotion, Searchghost::IdlingMotion, 0.5f, 1.0f);
}

void CEnemySearchghost::stateMain() {
    commonFly();
}

void CEnemySearchghost::endStateMain() {
    commonFlyEnd();
}

void CEnemySearchghost::beginStateAttack1() {
    changeMotionFileName(Searchghost::HandslamMotion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemySearchghost::stateAttack1() {
    turnToPlayerY(0.17453292f);
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemySearchghost::beginStateAttack2() {
    changeMotionFileName(Searchghost::HpAbsorbMotion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemySearchghost::stateAttack2() {
    if (isHitAttack()) {
        FVECTOR pos;
        CEffectMan::killEffect(m_hAbsorb, 0.0f);
        getLockOnPos(pos);
        recover(getHpMax() / 8, NULL, 11, pos, NULL);
        pos.init();
        m_hAbsorb = attachEffect("ex_2230", 11, &Crd(1), &pos, NULL, 0.0f);
    }
    turnToPlayerY(0.17453292f);
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

// SOURCE DATA: .data .rodata
