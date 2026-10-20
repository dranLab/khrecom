#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#define EFFECT_H_PATH "System/Graphic/Effect/Effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyBarrelspider.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyBarrelspiderEmptyString() {
    TRACE("");
}

#include "System/Graphic/Effect/effect.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"
#include "Game/Movement.h"

namespace Barrelspider {
static CHAR* BarrelMotion = "e028_001";
static CHAR* IdlingMotion = "e028_003";
static CHAR* MoveMotion = "e028_004";
static CHAR* ExplodeMotion = "e028_050";
static CHAR* Tackle1Motion = "e028_051";
static CHAR* Tackle2Motion = "e028_052";
static CHAR* Tackle3Motion = "e028_053";
static CHAR* headBoneName = "head";
}

CEnemyBarrelspider::CEnemyBarrelspider(CTask* inParent) : CHeartless(inParent, 0, 0) {}

CEnemyBarrelspider::~CEnemyBarrelspider() {}

BOOL CEnemyBarrelspider::initalize(INT inGroupID, INT inRsrcID) {
    CHeartless* pThis = this;
    if (!pThis->initalize(inGroupID, inRsrcID, 0)) return FALSE;
    changeMotionFileName(Barrelspider::BarrelMotion, 0, FALSE, -1, 0, 0);
    return TRUE;
}

void CEnemyBarrelspider::beginStateMain() {
    commonMoveInit(Barrelspider::IdlingMotion, Barrelspider::MoveMotion, 2.0f);
}

void CEnemyBarrelspider::stateMain() {
    commonMove();
}

void CEnemyBarrelspider::endStateMain() {
    commonMoveEnd();
}

void CEnemyBarrelspider::beginStateAttack1() {
    changeMotionFileName(Barrelspider::ExplodeMotion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
    setTimer(3000);
}

void CEnemyBarrelspider::stateAttack1() {
    switch (m_iStep) {
    case 0:
        if (getTimer() > 0) break;
        createExplode_(0);
        destroyCard();
        {
            register INT step asm("$3") = m_iStep + 1; /* the increment uses v1 (v0 with a plain m_iStep++) */
            m_iStep = step;
        }
        break;
    case 1:
        if (!isFade()) {
            out();
            endCardBattle(NULL, 6000);
        }
        break;
    }
}
void CEnemyBarrelspider::beginStateAttack2() {
    changeMotionFileName(Barrelspider::Tackle1Motion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyBarrelspider::stateAttack2() {
    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.17453292f);
        if (getMotionCurrentTime() >= 1750) m_iStep = 1;
    case 1:
        if (isMotionEnd()) {
            changeMotionFileName(Barrelspider::Tackle2Motion, 0, FALSE, -1, 0, 0);
            setTimer(12000);
            m_iStep = 2;
        }
        break;
    case 2:
        turnToPlayerY(0.17453292f);
        m_pMove->calcVelocityXZ(m_fSpeed * 3.0f);
        if (isBtlEnemyFlag(0x40)) {
            changeMotionFileName(Barrelspider::BarrelMotion, 0, FALSE, -1, 0, 0);
            setTimer(100);
            clearSpeed();
            m_iStep = 4;
        } else if (getTimer() <= 0) {
            changeMotionFileName(Barrelspider::Tackle3Motion, 0, FALSE, -1, 0, 0);
            clearSpeed();
            m_iStep = 3;
        }
        break;
    case 3:
        if (isMotionEnd()) {
            changeMotionFileName(Barrelspider::ExplodeMotion, 0, FALSE, -1, 0, 0);
            setTimer(3000);
            m_iStep = 4;
        }
        break;
    case 4:
        if (getTimer() > 0) break;
        destroyCard();
        createExplode_(0);
        setTimer(3000);
        m_iStep = 5;
        break;
    case 5:
        if (getTimer() > 0) break;
        out();
        endCardBattle(NULL, 6000);
        break;
    }
}

void CEnemyBarrelspider::beginStateTerror() {
    CBattleEnemy::beginStateTerror();
    changeMotionFileName(Barrelspider::MoveMotion, 0, FALSE, -1, 0, 0);
}

CEntity* CEnemyBarrelspider::createExplode_(INT inIndex) {
    static BULLET_EFFECT_INFO g_EffInfoSmall[4] = {
        {NULL, 0, -1, 0}, {"_ex_2180", 5, -1, 0}, {NULL, 0, -1, 0}, {"_ex_2180", 4, -1, 0}};
    static BULLET_EFFECT_INFO g_EffInfoLarge[4] = {
        {NULL, 0, -1, 0}, {"_ex_2180", 5, -1, 0}, {NULL, 0, -1, 0}, {"_ex_2180", 7, -1, 0}};
    static BULLET_EFFECT_INFO* g_pEffInfoTbl[2] = {g_EffInfoSmall, g_EffInfoLarge};
    static int g_tqLifeTime[2] = {3000, 2000};
    BULLET_PARAM param;
    CBullet* explode;
    CEffect* effect;

    param.init(1, 3);
    param.AtkAttr = getAttackAttr(inIndex);
    param.iAttackPoint = (INT)getAttackPoint(inIndex, (ATK_KIND)1, 0, NULL, 0);
    param.fSpeed = 0.0f;
    param.vPos = getBoneMatrix(Barrelspider::headBoneName)->v[3];
    param.vRot.init();
    param.pEffectTbl = g_pEffInfoTbl[inIndex];
    param.fCollRadius = 1.25f;
    param.tqHomingTime = g_tqLifeTime[inIndex];
    param.uiFlag = 6;
    explode = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
    if (!explode) {
        setupFade(1.0f, 0.0f, 1500, -1);
        return NULL;
    }
    effect = explode->getAttachEffectBullet();
    if (effect) {
        if (inIndex == 0) {
            setupFade(1.0f, 0.0f, effect->getTotalFrame() * 10, -1);
        } else {
            setupFade(1.0f, 0.0f, effect->getTotalFrame() * 25, -1);
        }
    } else {
        setupFade(1.0f, 0.0f, 1500, -1);
    }
    setHp(0);
    bodyCollOff(-1);
    bodyCollOffBody(-1);
    setEntityFlag(0, 0x10000);
    return explode;
}

// SOURCE DATA: .data .rodata
