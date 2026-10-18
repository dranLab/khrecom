#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyLoudness.h"

/* Placeholders for an unknown header's inline functions (prize messages) that come next in .rodata. */
inline void _premiumPrizeMessage() {
    TRACE("\x83v\x83\x8c\x83~\x83" "A\x83\x80\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}
inline void _roulettePrizeMessage() {
    TRACE("\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}

#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyLoudnessEmptyString() {
    TRACE("");
}

#include "Game/Enemy/EnemyParam.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "Game/Bullet/Bullet.h"
#include "Game/Collision/CollMap.h"
#include "System/TaskMan.h"
#include "System/Math3D.h"
#include "System/Rnd.h"
#include "Game/Movement.h"
#include "Game/Debug/DebugWork.h"
#include <math.h>

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

class CBattleEnemyManager { /* a CTask (Game/Enemy/BattleEnemyManager.h) */
private:
    UBYTE _m_0x00[0x74];
    INT m_iTotalEnemyNum; /* 0x74 */
public:
    INT getTotalEnemyNum() { return m_iTotalEnemyNum; } /* name unknown */
    SLContainerT<CTask>* getFirstBattleEnemy();
    INT createRandomFormationEnemy(const FVECTOR& inPos, INT inID);
};

namespace Loudness {
static CHAR* IdlingMotion = "e030_001";
static CHAR* MoveMotion = "e030_002";
static CHAR* FallMotion = "e030_010";
static CHAR* LandMotion = "e030_011";
static CHAR* BugleAttackMotion = "e030_050";
static CHAR* HealBugleMotion = "e030_051";
static CHAR* SallyBugleMotion = "e030_052";
}

CEnemyLoudness::CEnemyLoudness(CTask* inParent) : CHeartless(inParent, NULL, NULL) {
    setFallMotionIndex(10);
    setLandMotionIndex(11);
}

CEnemyLoudness::~CEnemyLoudness() {}

void CEnemyLoudness::beginStateMain() {
    commonMoveInit(Loudness::IdlingMotion, Loudness::MoveMotion, 2.0f);
}

void CEnemyLoudness::stateMain() {
    commonMove();
}

void CEnemyLoudness::endStateMain() {
    commonMoveEnd();
}

void CEnemyLoudness::beginStateAttack1() {
    changeMotionFileName(Loudness::BugleAttackMotion, 0, TRUE, -1, 0, 0);
    m_pMove->enableGravity(FALSE);
    m_iStep = 0;
}

void CEnemyLoudness::stateAttack1() {
    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.17453292f);
        if (isMotionEnd()) {
            changeMotionFileName(Loudness::FallMotion, 0, FALSE, -1, 0, 0);
            m_pMove->enableGravity(TRUE);
            m_iStep++;
        }
        break;
    case 1:
        if (m_pMove->isGround()) {
            changeMotionFileName(Loudness::LandMotion, 0, FALSE, -1, 0, 0);
            m_iStep++;
        }
        break;
    case 2:
        if (isMotionEnd()) endCardBattle(NULL, 6000);
        break;
    }
}

void CEnemyLoudness::endStateAttack1() {
    m_pMove->enableGravity(TRUE);
}

void CEnemyLoudness::beginStateAttack2() {
    changeMotionFileName(Loudness::HealBugleMotion, 0, TRUE, -1, 0, 0);
    m_pMove->enableGravity(FALSE);
    m_iStep = 0;
}

void CEnemyLoudness::stateAttack2() {
    static const BULLET_EFFECT_INFO g_RecoverTbl[4] = {
        {NULL, 0, -1, 0}, {"m_ex740", 8, -1, 0}, {NULL, 0, -1, 0}, {NULL, 0, -1, 0}};
    switch (m_iStep) {
    case 0:
        if (isMotionEnd()) {
            CBattleEnemy* target = (CBattleEnemy*)CTaskMan::getTask(searchRecoverEnemy_());
            if (!target) target = this;
            FVECTOR pos;
            pos._q = 0;
            pos.w = 1.0f;
            INT point = (INT)getAttackPoint(1, (ATK_KIND)25, 0, NULL, 0);
            CEffect* effect = CEffectMan::getEffect(target->recover(point, "m_ex740", 8, pos, &target->Crd(1)));
            if (effect) effect->setAttachNoRot(TRUE);
            changeMotionFileName(Loudness::FallMotion, 0, FALSE, -1, 0, 0);
            m_pMove->enableGravity(TRUE);
            m_iStep++;
        }
        break;
    case 1:
        if (m_pMove->isGround()) {
            changeMotionFileName(Loudness::LandMotion, 0, FALSE, -1, 0, 0);
            m_iStep++;
        }
        break;
    case 2:
        if (isMotionEnd()) endCardBattle(NULL, 6000);
        break;
    }
}

void CEnemyLoudness::endStateAttack2() {
    m_pMove->enableGravity(TRUE);
}

void CEnemyLoudness::beginStateAttack3() {
    changeMotionFileName(Loudness::SallyBugleMotion, 0, TRUE, -1, 0, 0);
    m_pMove->enableGravity(FALSE);
    m_iStep = 0;
}

void CEnemyLoudness::stateAttack3() {
    switch (m_iStep) {
    case 0:
        if (isMotionEnd()) {
            FVECTOR pos;
            FVECTOR target = getPos();
            CCollMap* coll = m_pMove->getMapCollision();
#line 207
            ASSERT(coll);
            for (FLOAT deg = 0.0f; deg < 360.0f; deg += 45.0f) {
                FLOAT theta = Math3D::LimitPI(deg * 0.017453292f);
                pos = target;
                pos.x += sinf(theta) * -4.0f;
                pos.z += cosf(theta) * -4.0f;
                coll->setStartPos(target);
                coll->setEndPos(pos);
                if (coll->collideMap()) pos = *coll->getOutPos();
                coll->setStartPos(pos);
                FLOAT d = 5.0f;
                pos.y -= d;
                coll->setEndPos(pos);
                if (coll->collideMap()) pos = *coll->getOutPos();
                else pos.y += d;
                if (canSetEnemy_(pos)) break;
                pos = target;
            }
            CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
#line 268
            ASSERT(bem);
            bem->createRandomFormationEnemy(pos, m_iID);
            changeMotionFileName(Loudness::FallMotion, 0, FALSE, -1, 0, 0);
            m_pMove->enableGravity(TRUE);
            m_iStep++;
        }
        break;
    case 1:
        if (m_pMove->isGround()) {
            changeMotionFileName(Loudness::LandMotion, 0, FALSE, -1, 0, 0);
            m_iStep++;
        }
        break;
    case 2:
        if (isMotionEnd()) endCardBattle(NULL, 6000);
        break;
    }
}

void CEnemyLoudness::endStateAttack3() {
    m_pMove->enableGravity(TRUE);
}

void CEnemyLoudness::aiCardBattle() {
    if (RND::genrand_int31() % getAttackRate() != 0) return;
    ENEMY_PARAM* param = getEnemyParam();
    INT count;
    int act[3];
    count = 0;
    CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
#line 327
    ASSERT(bem);
    INT num = bem->getTotalEnemyNum();
    if (num > 0 && num < 15) {
        if (RND::genrand_int31() % num == 0 && canAttack(2)) {
            act[count++] = param->pActs[2].iActNo;
        }
    }
    if (canRecoverEnemy_() && canAttack(1)) {
        act[count++] = param->pActs[1].iActNo;
    }
    if (canAttack(0)) {
        act[count++] = param->pActs[0].iActNo;
    }
    if (count == 0) return;
    if (!canCardBattle()) return;
    if (DebugWork::g_dw.enemy.uiFlag & 8) {
        for (INT i = 0; i < param->iActCount; i++) {
            if (param->pActs[i].iActNo == DebugWork::g_dw.enemy.sActNo) {
                setBtlEnemyFlag(0x200);
                setAttackState(DebugWork::g_dw.enemy.sActNo + 2);
                return;
            }
        }
        return;
    }
    setBtlEnemyFlag(0x200);
    setAttackState(act[RND::genrand_int31() % count] + 2);
}

HANDLE CEnemyLoudness::searchRecoverEnemy_() {
    CBattleEnemyManager* man = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
    INT remaindHp;
    CBattleEnemy* target;
#line 408
    ASSERT(man);
    remaindHp = 0;
    target = NULL;
    for (SLContainerT<CTask>* container = man->getFirstBattleEnemy(); container; container = container->getNext()) {
        CBattleEnemy* enemy = (CBattleEnemy*)container->getValue();
        if (!enemy) continue;
        if (enemy == this) continue;
        if (enemy->getTypeID() != 0x1004) continue;
        INT remaind = enemy->getHpMax() - enemy->getHp();
        if (remaind > 0 && remaindHp < remaind) {
            remaindHp = remaind;
            target = enemy;
        }
    }
    return target ? target->getHandle() : 0;
}

BOOL CEnemyLoudness::canRecoverEnemy_() {
    CBattleEnemyManager* man = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
#line 444
    ASSERT(man);
    for (SLContainerT<CTask>* container = man->getFirstBattleEnemy(); container; container = container->getNext()) {
        CBattleEnemy* enemy = (CBattleEnemy*)container->getValue();
        if (!enemy) continue;
        if (enemy->getTypeID() != 0x1004) continue;
        if (enemy->getHp() * 100 / enemy->getHpMax() <= 50) return TRUE;
    }
    return FALSE;
}

BOOL CEnemyLoudness::canSetEnemy_(const FVECTOR& inTarget) {
    CBattleEnemyManager* man = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
#line 469
    ASSERT(man);
    for (SLContainerT<CTask>* container = man->getFirstBattleEnemy(); container; container = container->getNext()) {
        CBattleEnemy* enemy = (CBattleEnemy*)container->getValue();
        if (!enemy) continue;
        if (enemy == this) continue;
        if (enemy->getTypeID() != 0x1004) continue;
        if (enemy->calcDistanceXZSq(inTarget) <= 6.25f) return FALSE;
    }
    return TRUE;
}

// SOURCE DATA: .data .rodata
