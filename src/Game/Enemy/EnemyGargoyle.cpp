
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyGargoyle.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyGargoyleEmptyString() {
    TRACE("");
}

#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"

void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
extern "C" float atan2f(float y, float x);

extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */

#include "System/Math3D.h"





namespace Gargoyle {
static CHAR* IdlingMotion = "e013_001";
static CHAR* MoveMotion = "e013_002";
static CHAR* Attack1Motion = "e013_050";
static CHAR* Attack2Motion = "e013_051";
}

CEnemyGargoyle::CEnemyGargoyle(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setEntityFlag(0x80, 0);
    setBattleEnemyFlag(0x100, 0);
}

CEnemyGargoyle::~CEnemyGargoyle() {}

void CEnemyGargoyle::beginStateMain() {
    commonFlyInit(Gargoyle::IdlingMotion, Gargoyle::MoveMotion, 0.5f, 1.5f);
}

void CEnemyGargoyle::stateMain() {
    commonFly();
}

void CEnemyGargoyle::endStateMain() {
    commonFlyEnd();
}

void CEnemyGargoyle::beginStateAttack1() {
    changeMotionFileName(Gargoyle::Attack1Motion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyGargoyle::stateAttack1() {
    turnToPlayerY(0.17453292f);
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyGargoyle::beginStateAttack2() {
    changeMotionFileName(Gargoyle::Attack2Motion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyGargoyle::stateAttack2() {
    static const BULLET_EFFECT_INFO g_EffTbl[4] = {
        { NULL, 0, -1, 0 },
        { "_ex_2220", 8, -1, 0 },
        { "_ex_2220", 9, -1, 0 },
        { "_ex_2220", 7, -1, 0 },
    };

    if (m_iStep == 0) {
        turnToPlayerY(0.17453292f);
        if (getMotionCurrentTime() >= 3150) {
            BULLET_PARAM param;
            FVECTOR pos;
            FVECTOR dir;
            FLOAT ret;
            FLOAT _y;
            param.init(1, 3);
            param.AtkAttr = getAttackAttr(1);
            param.iAttackPoint = (INT)getAttackPoint(1, (ATK_KIND)1, 0, NULL, 0);
            param.fSpeed = 0.25f;
            param.vPos = getBoneMatrix("chin")->v[3];
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl;
            param.fCollRadius = 0.25f;
            param.uiFlag = 0x8800;
            param.hHomingTarget = getPlayerHandle();
            param.fMaxDist = 400.0f;
            param.tqHomingTime2 = 1500;
            param.fHomingRate = 2.5f;
            g_pPlayer->getLockOnPos(pos);
            SubVector(dir, pos, param.vPos);
            Normalize(dir, dir);
            ret = -atan2f(dir.y, _sqrtf(dir.z * dir.z + dir.x * dir.x));
            _y = getRot().y;
            param.vRot.set(ret, _y, 0.0f);
            CEntityFactory::getInstance()->createBullet(param, this);
            m_iStep++;
        }
    }
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data .rodata
