
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyRednocturne.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyRednocturneEmptyString() {
    TRACE("");
}

#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"

void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
extern "C" float atan2f(float y, float x);

extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */

static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}

namespace Rednocturne {
static CHAR* IdlingMotion = "e003_001";
static CHAR* MoveMotion = "e003_002";
static CHAR* FireMotion = "e003_051";
static CHAR* FiraMotion = "e003_053";
}

CEnemyRednocturne::CEnemyRednocturne(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setEntityFlag(0x80, 0);
    setBattleEnemyFlag(0x100, 0);
}

CEnemyRednocturne::~CEnemyRednocturne() {}

void CEnemyRednocturne::beginStateMain() {
    commonFlyInit(Rednocturne::IdlingMotion, Rednocturne::MoveMotion, 0.5f, 1.5f);
}

void CEnemyRednocturne::stateMain() {
    commonFly();
}

void CEnemyRednocturne::endStateMain() {
    commonFlyEnd();
}

void CEnemyRednocturne::beginStateAttack1() {
    changeMotionFileName(Rednocturne::FireMotion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyRednocturne::stateAttack1() {
    static const BULLET_EFFECT_INFO g_EffTbl[4] = {
        { NULL, 0, -1, 0 },
        { "_ex_2110", 8, -1, 0 },
        { "_ex_2110", 9, -1, 0 },
        { "_ex_2110", 7, -1, 0 },
    };

    if (m_iStep == 0) {
        playerHomingY(0.25f);
        if (getMotionCurrentTime() >= 1950) {
            FVECTOR pos;
            getLockOnPos(pos);
            createFire_(0, g_EffTbl, pos, 0.2f, 0.25f);
            m_iStep++;
        }
    }
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyRednocturne::beginStateAttack2() {
    changeMotionFileName(Rednocturne::FiraMotion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyRednocturne::stateAttack2() {
    static const BULLET_EFFECT_INFO g_EffTbl[4] = {
        { NULL, 0, -1, 0 },
        { "_ex_2110", 12, -1, 0 },
        { "_ex_2110", 13, -1, 0 },
        { "_ex_2110", 11, -1, 0 },
    };

    if (m_iStep == 0) {
        playerHomingY(0.25f);
        if (getMotionCurrentTime() >= 4150) {
            FVECTOR pos;
            getLockOnPos(pos);
            createFire_(1, g_EffTbl, pos, 0.25f, 0.25f);
            m_iStep++;
        }
    }
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyRednocturne::createFire_(INT inAtkIndex, const BULLET_EFFECT_INFO* inInfo, const FVECTOR& inPos, FLOAT inSpeed,
                                    FLOAT inRadius) {
    BULLET_PARAM param;
    FVECTOR pos;
    FVECTOR dir;
    FLOAT ret;
    FLOAT _y;
    param.init(1, 3);
    param.AtkAttr = getAttackAttr(inAtkIndex);
    param.iAttackPoint = (INT)getAttackPoint(inAtkIndex, (ATK_KIND)1, 0, NULL, 0);
    param.vPos = inPos;
    param.fSpeed = inSpeed;
    param.uiFlag = 0x8800;
    param.pEffectTbl = (BULLET_EFFECT_INFO*)inInfo;
    param.fCollRadius = inRadius;
    param.hHomingTarget = getPlayerHandle();
    param.fMaxDist = 20.0f;
    param.tqHomingTime2 = 1500;
    param.fHomingRate = 2.5f;
    g_pPlayer->getLockOnPos(pos);
    SubVector(dir, pos, param.vPos);
    Normalize(dir, dir);
    ret = -atan2f(dir.y, _sqrtf(dir.z * dir.z + dir.x * dir.x));
    _y = getRot().y;
    param.vRot.set(ret, _y, 0.0f);
    CEntityFactory::getInstance()->createBullet(param, this);
}

// SOURCE DATA: .data .rodata
