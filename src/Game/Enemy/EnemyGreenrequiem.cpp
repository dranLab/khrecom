#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Task.h"
#include "System/TaskMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyGreenrequiem.h"

/* MATCH: Placeholders for an unknown header's inline functions (prize messages) that come next in .rodata. */
inline void _premiumPrizeMessage() {
    TRACE("\x83v\x83\x8c\x83~\x83" "A\x83\x80\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}
inline void _roulettePrizeMessage() {
    TRACE("\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}

#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyGreenrequiemEmptyString() {
    TRACE("");
}

#include "Game/Bullet/Bullet.h"
#include "System/Math3D.h"
#include "System/Rnd.h"
#include "Game/Movement.h"
#include <math.h>

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

#include "Game/Enemy/BattleEnemyManager.h"




namespace Greenrequiem {
static CHAR* IdlingMotion = "e003_001";
static CHAR* MoveMotion = "e003_002";
static CHAR* KealMotion = "e006_051";
}

/* (original form unknown: written inline, the constants load in another order) */
static inline FLOAT _frameRate(const CTask* inTask) {
    return inTask->getFrameRate() * 0.020000001f;
}

CEnemyGreenrequiem::CEnemyGreenrequiem(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setEntityFlag(0x80, 0);
    setBtlEnemyFlag(0x100);
}

CEnemyGreenrequiem::~CEnemyGreenrequiem() {}

void CEnemyGreenrequiem::beginStateMain() {
    commonFlyInit(Greenrequiem::IdlingMotion, Greenrequiem::MoveMotion, 0.5f, 2.0f);
}

void CEnemyGreenrequiem::stateMain() {
    commonFly();
}

void CEnemyGreenrequiem::endStateMain() {
    commonFlyEnd();
}

void CEnemyGreenrequiem::beginStateAttack1() {
    changeMotionFileName(Greenrequiem::KealMotion, 0, FALSE, -1, 0, 0);
    m_hCureTarget = searchRecoverEnemy_();
    CEntity* target = (CEntity*)CTaskMan::getTask(m_hCureTarget);
    if (target) {
        target->getLockOnPos(m_vAimPos);
        m_vAimPos.y += 1.25f;
    } else {
        m_hCureTarget = getHandle();
        m_vAimPos = getPos();
    }
    m_iStep = 0;
}

void CEnemyGreenrequiem::stateAttack1() {
    if (m_iStep == 0 && getMotionCurrentTime() >= 6000) {
        recover_();
        m_iStep++;
    }
    if (arriveTarget(m_vAimPos, (m_fSpeed + 0.1f) * 1.5f * _frameRate(this))) {
        CEntity* target = (CEntity*)CTaskMan::getTask(m_hCureTarget);
        if (target && target != this) {
            target->getLockOnPos(m_vAimPos);
            m_vAimPos.y += 1.25f;
        }
        FLOAT rot = Math3D::LimitPI(RND::genrand_real1() * 360.0f * 0.017453292f);
        FLOAT len = rndArea(2.0f, 4.0f);
        m_vAimPos.x += len * sinf(rot);
        m_vAimPos.z += len * cosf(rot);
    }
    m_pMove->turnToTarget(m_vAimPos, 0.17453292f);
    m_pMove->calcVelocity(m_fSpeed * 1.5f);
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

#line 122
HANDLE CEnemyGreenrequiem::searchRecoverEnemy_() {
    CBattleEnemyManager* man = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
    INT remaindHp;
    CBattleEnemy* target;
    ASSERT(man);
    remaindHp = 0;
    target = NULL;
    for (SLContainerT<CTask>* container = man->getFirstBattleEnemy(); container; container = container->getNext()) {
        CBattleEnemy* enemy = (CBattleEnemy*)container->getValue();
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

void CEnemyGreenrequiem::recover_() {
    static const BULLET_EFFECT_INFO g_Recover1Tbl[4] = {
        {NULL, 0, -1, 0}, {"_ex_2140", 7, -1, 0}, {NULL, 0, -1, 0}, {NULL, 0, -1, 0}};
    static const BULLET_EFFECT_INFO g_Recover2Tbl[4] = {
        {NULL, 0, -1, 0}, {"_ex_2140", 8, -1, 0}, {NULL, 0, -1, 0}, {NULL, 0, -1, 0}};
    static const BULLET_EFFECT_INFO g_Recover3Tbl[4] = {
        {NULL, 0, -1, 0}, {"_ex_2140", 9, -1, 0}, {NULL, 0, -1, 0}, {NULL, 0, -1, 0}};
    static const BULLET_EFFECT_INFO g_Recover4Tbl[4] = {
        {NULL, 0, -1, 0}, {"_ex_2140", 10, -1, 0}, {NULL, 0, -1, 0}, {NULL, 0, -1, 0}};
    CBattleEnemy* target = (CBattleEnemy*)CTaskMan::getTask(m_hCureTarget);
    if (!target) return;
    FVECTOR pos;
    pos._q = 0;
    pos.w = 1.0f;
    INT no = 7;
    switch ((UINT)target->getID() % 10000) {
    case 3:
        no = 8;
        break;
    case 4:
        no = 9;
        break;
    case 5:
        no = 10;
        break;
    }
    INT point = (INT)getAttackPoint(0, (ATK_KIND)25, 0, NULL, 0);
    target->recover(point, "_ex_2140", no, pos, &target->Crd(1));
}

// SOURCE DATA: .data .rodata
