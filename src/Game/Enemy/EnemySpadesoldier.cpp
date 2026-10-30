#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemySpadesoldier.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemySpadesoldierEmptyString() {
    TRACE("");
}

/* Placeholders for an unknown header's inline functions (prize messages) that come next in .rodata. */
inline void _premiumPrizeMessage() {
    TRACE("\x83v\x83\x8c\x83~\x83" "A\x83\x80\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}
inline void _roulettePrizeMessage() {
    TRACE("\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}

#include "Game/BodyCollision.h"
#include "Game/Movement.h"
#include "Game/GameWork.h"
#include "System/TaskMan.h"

namespace Message {
class CMessageManager {
public:
    static CMessageManager& getInstance();
    BOOL sendMessage(TIMEQ inDelay, HANDLE inSender, HANDLE inReceiver, INT inMsg, void* inExtraInfo);
};
}

#include "Game/Enemy/BattleEnemyManager.h"







#include "Game/Enemy/BossDeadDemo.h"







inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

static inline FVECTOR _deadDemoPlayerPos(FLOAT inW) {
    FVECTOR v;
    v._q = 0;
    v.w = inW;
    return v;
}

namespace Spadesoldier {
static CHAR* IdlingMotion = "e103_001";
static CHAR* MoveMotion = "e103_002";
static CHAR* KyoroKyoroMotion = "e103_003";
static CHAR* Attack1Motion = "e103_050";
static CHAR* MoveAttackStartMotion = "e103_051";
static CHAR* MoveAttackMotion = "e103_052";
static FVECTOR g_vDeadDemoPlayerPos = _deadDemoPlayerPos(-0.7853982f);
}

CEnemySpadesoldier::CEnemySpadesoldier(CTask* inParent) : CHeartless(inParent, 0, 0) {}

CEnemySpadesoldier::~CEnemySpadesoldier() {}

void CEnemySpadesoldier::beginStateAppear() {
    changeMotionFileName(Spadesoldier::IdlingMotion, 0, FALSE, -1, 0, 0);
    setTexAnmSeq(0, getParam());
    setTexAnmSeq(1, getParam());
    setTexAnmSeq(2, getParam());
    if (CGameWork::getInt(1, 225) == 4) {
        setupFade(0.0f, 1.0f, 3000, -1);
        Message::CMessageManager::getInstance().sendMessage(150, getHandle(), getHandle(), 0x70F0F0F0, NULL);
    }
}

void CEnemySpadesoldier::stateAppear() {
    if (CGameWork::getInt(1, 225) == 4 && !isFade()) {
        changeDefaultState();
    }
}

void CEnemySpadesoldier::endStateAppear() {
    setEntityFlag(0x10000, 0);
}

void CEnemySpadesoldier::beginStateMain() {
    commonMoveInit(Spadesoldier::KyoroKyoroMotion, Spadesoldier::MoveMotion, 2.0f);
}

void CEnemySpadesoldier::stateMain() {
    commonMove();
}

void CEnemySpadesoldier::endStateMain() {
    commonMoveEnd();
}

void CEnemySpadesoldier::beginStateAttack1() {
    changeMotionFileName(Spadesoldier::Attack1Motion, 0, TRUE, -1, 0, 0);
}

void CEnemySpadesoldier::stateAttack1() {
    turnToPlayerY(0.06981317f);
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemySpadesoldier::endStateAttack1() {}

void CEnemySpadesoldier::beginStateAttack2() {
    changeMotionFileName(Spadesoldier::MoveAttackStartMotion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemySpadesoldier::stateAttack2() {
    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.08726646f);
        if (isMotionEnd()) {
            changeMotionFileName(Spadesoldier::MoveAttackMotion, 0, FALSE, -1, 0, 0);
            m_iStep++;
        }
        break;
    case 1:
        turnToPlayerY(0.08726646f);
        m_pMove->calcVelocityXZ(m_fSpeed);
        if (isBtlEnemyFlag(0x40)) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

void CEnemySpadesoldier::endStateAttack2() {}

#line 147
void CEnemySpadesoldier::beginStateDead() {
    CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(25);
    if (!bem) AssertApp(__FILE__, __LINE__, "bem", 0);
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
        demo = new (com_malloc_d(sizeof(CBossDeadDemo), __FILE__ "(180)", RET_ADDR())) CBossDeadDemo(this, 0);
        if (demo) {
            demo->setup(getHandle(), &Spadesoldier::g_vDeadDemoPlayerPos, -1);
        }
        m_iStep = 1;
    }
}

void CEnemySpadesoldier::stateDead() {
    if (m_iStep == 0) {
        CBattleEnemy::stateDead();
    }
}

void CEnemySpadesoldier::endStateDead() {
    if (m_iStep == 0) {
        CBattleEnemy::endStateDead();
    }
}

// SOURCE DATA: .data .rodata .bss
