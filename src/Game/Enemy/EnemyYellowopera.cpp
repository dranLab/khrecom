
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyYellowopera.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyYellowoperaEmptyString() {
    TRACE("");
}

#include "Game/Movement.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"

FVECTOR getPlayerPos(INT inIndex);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT t);

extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */

namespace Yellowopera {
static CHAR* IdlingMotion = "e003_001";
static CHAR* MoveMotion = "e003_002";
static CHAR* ThunderMotion = "e005_052";
static CHAR* RushAttackStartMotion = "e005_053";
static CHAR* RushAttackLoopMotion = "e005_054";
static CHAR* RushAttackEndMotion = "e005_055";
static CHAR* RushAtkTimeParamName = "RushAtkTime";
static CHAR* RushSpeedParamName = "RushSpeed";
}

CEnemyYellowopera::CEnemyYellowopera(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setEntityFlag(0x80, 0);
    setBtlEnemyFlag(0x100);
}

CEnemyYellowopera::~CEnemyYellowopera() {}

void CEnemyYellowopera::beginStateMain() {
    commonFlyInit(Yellowopera::IdlingMotion, Yellowopera::MoveMotion, 0.5f, 2.5f);
}

void CEnemyYellowopera::stateMain() {
    commonFly();
}

void CEnemyYellowopera::endStateMain() {
    commonFlyEnd();
}

void CEnemyYellowopera::beginStateAttack1() {
    changeMotionFileName(Yellowopera::ThunderMotion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyYellowopera::stateAttack1() {
    static const BULLET_EFFECT_INFO g_EffTbl[4] = {
        { "_ex_2130", 11, -1, 0 },
        { "_ex_2130", 12, -1, 0 },
        { NULL, 0, -1, 0 },
        { NULL, 0, -1, 0 },
    };

    if (m_iStep == 0 && getMotionCurrentTime() >= 3350) {
        BULLET_PARAM param;
        CBullet* pBullet;
        param.init(1, 3);
        param.AtkKind = 28;
        param.AtkAttr = getAttackAttr(0);
        param.iAttackPoint = (INT)getAttackPoint(0, (ATK_KIND)1, 0, NULL, 0);
        param.fSpeed = 5.0f;
        param.vPos = getPlayerPos(0);
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl;
        param.fCollRadius = 0.25f;
        param.vRot.init();
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (pBullet) {
            param.vPos.y += 4.0f;
            pBullet->warp(param.vPos);
            pBullet->setCollAttackPos(param.vPos);
            ((FVECTOR&)pBullet->getRot()).x = 1.5707964f;
        }
        m_iStep++;
    }
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyYellowopera::beginStateAttack2() {
    changeMotionFileName(Yellowopera::RushAttackStartMotion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyYellowopera::stateAttack2() {
    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.17453292f);
        if (isMotionEnd()) {
            FVECTOR vel;
            changeMotionFileName(Yellowopera::RushAttackLoopMotion, 0, FALSE, -1, 0, 0);
            setTimer((TIMEQ)(getExtraParam(Yellowopera::RushAtkTimeParamName) * 60.0f * 50.0f));
            CEntity*& rPlayer = g_pPlayer; /* through a reference: keeps the load after the timer store */
            rPlayer->getLockOnPos(vel);
            SubVectorXYZ(vel, vel, getPos());
            Normalize(vel, vel);
            ScaleVectorXYZ(vel, vel, getExtraParam(Yellowopera::RushSpeedParamName));
            m_pMove->setVel(vel);
            m_iStep++;
        }
        break;
    case 1:
        if (getTimer() <= 0 || isBtlEnemyFlag(0x40) || m_pMove->isGround() || m_pMove->isWall()) {
            changeMotionFileName(Yellowopera::RushAttackEndMotion, 0, FALSE, -1, 0, 0);
            clearSpeed();
            m_iStep++;
        }
        break;
    case 2:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

void CEnemyYellowopera::endStateAttack2() {}

// SOURCE DATA: .data .rodata
