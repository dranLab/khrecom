
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyBluerhapsody.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyBluerhapsodyEmptyString() {
    TRACE("");
}

#include "Game/Movement.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"

namespace Bluerhapsody {
static CHAR* IdlingMotion = "e003_001";
static CHAR* MoveMotion = "e003_002";
static CHAR* Attack1Motion = "e004_050";
static CHAR* Attack2Motion = "e004_051";
}

CEnemyBluerhapsody::CEnemyBluerhapsody(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setEntityFlag(0x80, 0);
    setBtlEnemyFlag(0x100);
    setHeartlessFlag(4, 0);
}

CEnemyBluerhapsody::~CEnemyBluerhapsody() {}

void CEnemyBluerhapsody::beginStateMain() {
    commonMoveInit(Bluerhapsody::IdlingMotion, Bluerhapsody::MoveMotion, 2.0f);
    m_pMove->enableGravity(TRUE);
}

void CEnemyBluerhapsody::stateMain() {
    switch (m_iStep) {
    case 2:
    case 5:
    case 8:
        if (m_pMove->getVel().y <= 0.0f && m_pMove->isGround()) {
            FVECTOR vel = m_pMove->getVel();
            vel.y = 0.15f;
            m_pMove->setVel(vel);
        }
        break;
    }
    commonMove();
}

void CEnemyBluerhapsody::endStateMain() {
    commonMoveEnd();
}

void CEnemyBluerhapsody::beginStateAttack1() {
    changeMotionFileName(Bluerhapsody::Attack1Motion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyBluerhapsody::stateAttack1() {
    static BULLET_EFFECT_INFO g_EffTbl[4] = {
        { NULL, 0, -1, 0 },
        { "_ex_2120", 9, -1, 0 },
        { "_ex_2120", 10, -1, 0 },
        { "_ex_2120", 8, -1, 0 },
    };

    if (m_iStep == 0) {
        playerHomingY(0.25f);
        if (getMotionCurrentTime() >= 4250) {
            BULLET_PARAM param;
            FLOAT _y;
            param.init(1, 3);
            param.AtkAttr = getAttackAttr(0);
            param.iAttackPoint = (INT)getAttackPoint(0, (ATK_KIND)1, 0, NULL, 0);
            param.fSpeed = 0.4f;
            getLockOnPos(param.vPos);
            param.pEffectTbl = g_EffTbl;
            param.fCollRadius = 0.2f;
            _y = getRot().y;
            param.vRot.set(0.0f, _y, 0.0f);
            CEntityFactory::getInstance()->createBullet(param, this);
            m_iStep++;
        }
    }
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyBluerhapsody::beginStateAttack2() {
    changeMotionFileName(Bluerhapsody::Attack2Motion, 0, FALSE, -1, 0, 0);
}

void CEnemyBluerhapsody::stateAttack2() {
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

// SOURCE DATA: .data .rodata
