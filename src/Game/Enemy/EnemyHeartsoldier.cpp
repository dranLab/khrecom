#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Task.h"
#include "System/TaskMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyHeartsoldier.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyHeartsoldierEmptyString() {
    TRACE("");
}

/* MATCH: Placeholders for an unknown header's inline functions (prize messages) that come next in .rodata. */
inline void _premiumPrizeMessage() {
    TRACE("\x83v\x83\x8c\x83~\x83" "A\x83\x80\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}
inline void _roulettePrizeMessage() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}

#include "Game/BodyCollision.h"
#include "Game/Movement.h"
#include "Game/GameWork.h"
#include "Game/Collision/CollBody.h"
#include "Game/MessageManager.h"
namespace Message {




}

#include "Game/Enemy/BattleEnemyManager.h"







#include "Game/Enemy/BossDeadDemo.h"
#include "System/HeapMan.h"










static inline FVECTOR _deadDemoPlayerPos(FLOAT inW) {
    FVECTOR v;
    v._q = 0;
    v.w = inW;
    return v;
}

namespace Heartsoldier {
static CHAR* IdlingMotion = "e104_001";
static CHAR* MoveMotion = "e104_002";
static CHAR* AttackMotion = "e104_050";
static FVECTOR g_vDeadDemoPlayerPos = _deadDemoPlayerPos(-0.7853982f);
}

CEnemyHeartsoldier::CEnemyHeartsoldier(CTask* inParent) : CHeartless(inParent, 0, 0) {}

CEnemyHeartsoldier::~CEnemyHeartsoldier() {}

void CEnemyHeartsoldier::beginStateAppear() {
    changeMotionFileName(Heartsoldier::IdlingMotion, 0, FALSE, -1, 0, 0);
    setTexAnmSeq(0, getParam());
    setTexAnmSeq(1, getParam());
    setTexAnmSeq(2, getParam());
    if (CGameWork::getInt(1, 225) == 4) {
        setupFade(0.0f, 1.0f, 3000, -1);
        Message::CMessageManager::getInstance().sendMessage(150, getHandle(), getHandle(), 0x70F0F0F0, NULL);
    }
}

void CEnemyHeartsoldier::stateAppear() {
    if (CGameWork::getInt(1, 225) == 4 && !isFade()) {
        changeDefaultState();
    }
}

void CEnemyHeartsoldier::endStateAppear() {
    setEntityFlag(0x10000, 0);
}

void CEnemyHeartsoldier::beginStateMain() {
    commonMoveInit(Heartsoldier::IdlingMotion, Heartsoldier::MoveMotion, 2.0f);
}

void CEnemyHeartsoldier::stateMain() {
    commonMove();
}

void CEnemyHeartsoldier::endStateMain() {
    commonMoveEnd();
}

void CEnemyHeartsoldier::beginStateAttack1() {
    changeMotionFileName(Heartsoldier::AttackMotion, 0, TRUE, -1, 0, 0);
}

void CEnemyHeartsoldier::stateAttack1() {
    turnToPlayerY(0.06981317f);
    if (getMotionCurrentTime() >= 1900) {
        m_pMove->calcVelocityXZ(0.025f);
    }
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyHeartsoldier::endStateAttack1() {}

#line 115
void CEnemyHeartsoldier::beginStateDead() {
    CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(25);
    KH_DEBUG(if (!bem) AssertApp(__FILE__, __LINE__, "bem", 0);)
    if (bem->getTotalEnemyNum() >= 2) {
        CBattleEnemy::beginStateDead();
        m_iStep = 0;
    } else {
        CCollBody* pHit;
        CBossDeadDemo* demo;
        bodyCollOff(-1);
        setBossDeadEffect();
        changeMotion(selectDamageMotion_(), 0, FALSE, -1, 0, TRUE);
        pHit = m_pBodyColl->getHitCollBody();
        if (pHit) {
            if (pHit->getDamageKind() == 3) {
                playBigDamageSE();
            } else {
                playSmallDamageSE();
            }
        }
        demo = new KH_AT("(148)") CBossDeadDemo(this, 0);
        if (demo) {
            demo->setup(getHandle(), &Heartsoldier::g_vDeadDemoPlayerPos, -1);
        }
        m_iStep = 1;
    }
}

void CEnemyHeartsoldier::stateDead() {
    if (m_iStep == 0) {
        CBattleEnemy::stateDead();
    }
}

void CEnemyHeartsoldier::endStateDead() {
    if (m_iStep == 0) {
        CBattleEnemy::endStateDead();
    }
}

// SOURCE DATA: .data .rodata .bss
