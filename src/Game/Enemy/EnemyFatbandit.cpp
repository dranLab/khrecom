
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyFatbandit.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyFatbanditEmptyString() {
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

namespace Fatbandit {
static CHAR* IdlingMotion = "e010_001";
static CHAR* MoveMotion = "e010_002";
static CHAR* NagiharaiMotion = "e010_050";
static CHAR* FireBallMotion = "e010_051";
}

CEnemyFatbandit::CEnemyFatbandit(CTask* inParent) : CHeartless(inParent, 0, 0) {
    m_fBodyRviseCoefficient = 0.1f;
}

CEnemyFatbandit::~CEnemyFatbandit() {}

void CEnemyFatbandit::beginStateMain() {
    commonMoveInit(Fatbandit::IdlingMotion, Fatbandit::MoveMotion, 2.0f);
}

void CEnemyFatbandit::stateMain() {
    commonMove();
}

void CEnemyFatbandit::endStateMain() {
    commonMoveEnd();
}

void CEnemyFatbandit::beginStateAttack1() {
    changeMotionFileName(Fatbandit::NagiharaiMotion, 0, FALSE, -1, 0, 0);
}

void CEnemyFatbandit::stateAttack1() {
    turnToPlayerY(0.08726646f);
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyFatbandit::beginStateAttack2() {
    changeMotionFileName(Fatbandit::FireBallMotion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyFatbandit::stateAttack2() {
    static const BULLET_EFFECT_INFO g_EffTbl[4] = {
        { NULL, 0, -1, 0 },
        { "_ex_2060", 7, -1, 0 },
        { "_ex_2060", 15, -1, 0 },
        { "_ex_2060", 5, -1, 0 },
    };
    static const FVECTOR g_vRotTbl[4] = {
        { { 0.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.0f, 3.1415927f, 0.0f, 1.0f } },
    };

    if (m_iStep == 0) {
        turnToPlayerY(0.08726646f);
        if (getMotionCurrentTime() >= 2850) {
            BULLET_PARAM param;
            FVECTOR pos;
            FVECTOR dir;
            FLOAT ret;
            FLOAT _y;
            param.init(1, 3);
            param.AtkAttr = getAttackAttr(1);
            param.iAttackPoint = (INT)getAttackPoint(1, (ATK_KIND)1, 0, NULL, 0);
            param.fSpeed = 0.2f;
            param.vPos = getBoneMatrix("head2")->v[3];
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl;
            param.pRotTbl = (FVECTOR*)g_vRotTbl;
            param.fCollRadius = 0.25f;
            param.uiFlag = 0x8800;
            param.hHomingTarget = getPlayerHandle();
            param.fHomingRate = 2.5f;
            param.tqHomingTime2 = 1500;
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
