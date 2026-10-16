#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyBouncywild.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyBouncywildEmptyString() {
    TRACE("");
}

#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"

void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

namespace Bouncywild {
static CHAR* IdlingMotion = "e008_001";
static CHAR* MoveMotion = "e008_002";
static CHAR* Pachinko1Motion = "e008_050";
static CHAR* Pachinko2Motion = "e008_051";
static CHAR* Pachinko3Motion = "e008_052";
static CHAR* BananaMotion = "e008_053";
}

CEnemyBouncywild::CEnemyBouncywild(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setHeartlessFlag(2, 0);
}

CEnemyBouncywild::~CEnemyBouncywild() {}

void CEnemyBouncywild::beginStateMain() {
    commonMoveInit(Bouncywild::IdlingMotion, Bouncywild::MoveMotion, 1.5f);
}

void CEnemyBouncywild::stateMain() {
    commonMove();
}

void CEnemyBouncywild::endStateMain() {
    commonMoveEnd();
}

void CEnemyBouncywild::beginStateAttack1() {
    changeMotionFileName(Bouncywild::Pachinko1Motion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyBouncywild::stateAttack1() {
    static BULLET_EFFECT_INFO g_EffTbl[4] = {
        { NULL, 0, -1, 0 },
        { "_ex_2040", 6, -1, 0 },
        { "_ex_2040", 15, -1, 0 },
        { "_ex_2040", 13, -1, 0 },
    };
    static FVECTOR g_vOffset = { { 0.25f, 0.0f, 0.0f, 1.0f } };

    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.17453292f);
        if (isMotionEnd()) {
            changeMotionFileName(Bouncywild::Pachinko2Motion, 0, FALSE, -1, 0, 0);
            m_iStep++;
        }
        break;
    case 1:
        turnToPlayerY(0.17453292f);
        if (isMotionEnd()) {
            changeMotionFileName(Bouncywild::Pachinko3Motion, 0, FALSE, -1, 0, 0);
            m_iStep++;
        }
        break;
    case 2:
        playerHomingY(0.25f);
        if (getMotionCurrentTime() >= 3800) {
            BULLET_PARAM param;
            FLOAT _y;
            param.init(1, 3);
            param.AtkAttr = getAttackAttr(0);
            param.iAttackPoint = (INT)getAttackPoint(0, (ATK_KIND)1, 0, NULL, 0);
            param.fSpeed = 0.6f;
            ApplyMatrix(param.vPos, *getBoneMatrix("wep0"), g_vOffset);
            param.fCollRadius = 0.2f;
            param.fMaxDist = 20.0f;
            param.pEffectTbl = g_EffTbl;
            _y = getRot().y;
            param.vRot.set(0.0f, _y, 0.0f);
            CEntityFactory::getInstance()->createBullet(param, this);
            m_iStep = 3;
        }
    case 3:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

void CEnemyBouncywild::beginStateAttack2() {
    changeMotionFileName(Bouncywild::BananaMotion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyBouncywild::stateAttack2() {
    static BULLET_EFFECT_INFO g_EffTbl[4] = {
        { NULL, 0, -1, 0 },
        { "_ex_2040", 5, -1, 0 },
        { NULL, 0, -1, 0 },
        { "_ex_2040", 10, -1, 0 },
    };
    static FVECTOR g_vOffset = { { 0.07f, -0.04f, 0.0f, 1.0f } };
    static BULLET_EFFECT_SHADOW_INFO g_ShadowInfo = { "_ex_2040", 11 };

    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.17453292f);
        if (getMotionCurrentTime() >= 3800) {
            BULLET_PARAM param;
            FLOAT rotX = -0.5235988f;
            param.init(1, 3);
            param.CollKind = 2;
            param.AtkAttr = getAttackAttr(1);
            param.iAttackPoint = (INT)getAttackPoint(1, (ATK_KIND)1, 0, NULL, 0);
            param.fSpeed = 0.2f;
            param.AtkKind = 24;
            ApplyMatrix(param.vPos, *getBoneMatrix("l_fing1a"), g_vOffset);
            param.fCollRadius = 0.4f;
            param.pEffectTbl = g_EffTbl;
            param.pShadow = &g_ShadowInfo;
            param.fShiftY = 0.4f;
            param.uiFlag |= 0x5D;
            param.tqHomingTime = 15000;
            param.vRot.set(rotX, getRot().y, 0.0f);
            CEntityFactory::getInstance()->createBullet(param, this);
            m_iStep = 1;
        }
    case 1:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

// SOURCE DATA: .data .rodata
