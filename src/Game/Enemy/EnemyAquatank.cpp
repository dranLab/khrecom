
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyAquatank.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyAquatankEmptyString() {
    TRACE("");
}

#include "Game/Movement.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"
#include "Game/SceneEnv.h"

FVECTOR getPlayerPos(INT inIndex);

namespace Aquatank {
static CHAR* IdlingMotion = "e020_001";
static CHAR* MoveMotion = "e020_002";
static CHAR* RotateAttackMotion = "e020_050";
static CHAR* Thundra1Motion = "e020_051";
static CHAR* Thundra2Motion = "e020_052";
static CHAR* Thundra3Motion = "e020_053";
static CHAR* Thundra4Motion = "e020_054";
}

CEnemyAquatank::CEnemyAquatank(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setEntityFlag(0x80, 0);
    setBattleEnemyFlag(0x100, 0);
    m_fBodyRviseCoefficient = 0.1f;
}

CEnemyAquatank::~CEnemyAquatank() {}

BOOL CEnemyAquatank::initalize(INT inGroupID, INT inRsrcID) {
    return CHeartless::initalize(inGroupID, inRsrcID) != 0;
}

void CEnemyAquatank::beginStateMain() {
    commonFlyInit(Aquatank::IdlingMotion, Aquatank::MoveMotion, 0.5f, 1.25f);
}

void CEnemyAquatank::stateMain() {
    commonFly();
}

void CEnemyAquatank::endStateMain() {
    commonFlyEnd();
}

void CEnemyAquatank::beginStateAttack1() {
    changeMotionFileName(Aquatank::RotateAttackMotion, 0, FALSE, -1, 0, 0);
}

void CEnemyAquatank::stateAttack1() {
    turnToPlayerY(0.08726646f);
    m_pMove->calcVelocityXZ(m_fSpeed);
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyAquatank::endStateAttack1() {}

void CEnemyAquatank::beginStateAttack2() {
    FVECTOR col;
    changeMotionFileName(Aquatank::Thundra1Motion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
    col.set(0.5f, 0.5f, 0.5f, 1.0f);
    CSceneEnv::setColor((ObjType::eObjBit)0x1800, 30.0f, col, NULL, 0);
}

void CEnemyAquatank::stateAttack2() {
    static const BULLET_EFFECT_INFO g_EffTbl[4] = {
        { "_thunder_all", 3, -1, 0 },
        { "_thunder_all", 6, -1, 0 },
        { NULL, 0, -1, 0 },
        { NULL, 0, -1, 0 },
    };

    switch (m_iStep) {
    case 0:
        if (isMotionEnd()) {
            changeMotionFileName(Aquatank::Thundra2Motion, 0, FALSE, -1, 0, 0);
            m_iStep++;
        }
        break;
    case 1:
        if (isMotionEnd()) {
            BULLET_PARAM param;
            CBullet* pBullet;
            param.init(1, 3);
            param.AtkKind = 29;
            param.AtkAttr = getAttackAttr(1);
            param.iAttackPoint = (INT)getAttackPoint(1, (ATK_KIND)1, 0, NULL, 0);
            param.fSpeed = 5.0f;
            param.vPos = getPlayerPos(0);
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl;
            param.fCollRadius = 0.5f;
            param.vRot.init();
            pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
            if (pBullet) {
                param.vPos.y += 4.0f;
                pBullet->warp(param.vPos);
                pBullet->setCollAttackPos(param.vPos);
                ((FVECTOR&)pBullet->getRot()).x = 1.5707964f;
            }
            killLoopEffect(1);
            changeMotionFileName(Aquatank::Thundra3Motion, 0, FALSE, -1, 0, 0);
            m_iStep++;
        }
        break;
    case 2:
        if (isMotionEnd()) {
            changeMotionFileName(Aquatank::Thundra4Motion, 0, FALSE, -1, 0, 0);
            m_iStep++;
        }
        break;
    case 3:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

void CEnemyAquatank::endStateAttack2() {
    FVECTOR col;
    col.set(1.0f, 1.0f, 1.0f, 1.0f);
    CSceneEnv::setColor((ObjType::eObjBit)0x1800, 30.0f, col, NULL, 0);
    killLoopEffect(1);
}

// SOURCE DATA: .data .rodata
