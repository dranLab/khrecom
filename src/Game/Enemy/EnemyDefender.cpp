
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyDefender.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyDefenderEmptyString() {
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

namespace Defender {
static CHAR* IdlingMotion = "e026_001";
static CHAR* MoveMotion = "e026_002";
static CHAR* ShieldBiteMotion = "e026_050";
static CHAR* Figa1Motion = "e026_051";
static CHAR* Figa2Motion = "e026_052";
}

CEnemyDefender::CEnemyDefender(CTask* inParent) : CHeartless(inParent, 0, 0) {
    m_fBodyRviseCoefficient = 0.1f;
}

CEnemyDefender::~CEnemyDefender() {}

void CEnemyDefender::beginStateMain() {
    commonMoveInit(Defender::IdlingMotion, Defender::MoveMotion, 2.0f);
}

void CEnemyDefender::stateMain() {
    commonMove();
}

void CEnemyDefender::endStateMain() {
    commonMoveEnd();
}

void CEnemyDefender::beginStateAttack1() {
    changeMotionFileName(Defender::ShieldBiteMotion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyDefender::stateAttack1() {
    if (m_iStep == 0) {
        turnToPlayerY(0.17453292f);
        if (getMotionCurrentTime() >= 500) {
            m_iStep++;
        }
    }
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyDefender::endStateAttack1() {}

void CEnemyDefender::beginStateAttack2() {
    changeMotionFileName(Defender::Figa1Motion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyDefender::stateAttack2() {
    static const BULLET_EFFECT_INFO g_EffTbl[4] = {
        { NULL, 0, -1, 0 },
        { "_ex_2340", 12, -1, 0 },
        { "_ex_2340", 13, -1, 0 },
        { "_ex_2340", 11, -1, 0 },
    };

    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.17453292f);
        if (isMotionEnd()) {
            changeMotionFileName(Defender::Figa2Motion, 0, FALSE, -1, 0, 0);
            m_iStep = 1;
        }
        break;
    case 1:
        turnToPlayerY(0.17453292f);
        if (getMotionCurrentTime() >= 150) {
            BULLET_PARAM param;
            FVECTOR pos;
            FVECTOR dir;
            FLOAT ret;
            FLOAT _y;
            param.init(1, 3);
            param.AtkAttr = getAttackAttr(1);
            param.iAttackPoint = (INT)getAttackPoint(1, (ATK_KIND)1, 0, NULL, 0);
            param.vPos = getBoneMatrix("lb_wep0")->v[3];
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl;
            param.fCollRadius = 0.25f;
            param.fHomingRate = 2.0f;
            param.fSpeed = 0.3f;
            param.tqHomingTime2 = 1500;
            param.hHomingTarget = g_pPlayer->getHandle();
            param.uiFlag = 0x8800;
            g_pPlayer->getLockOnPos(pos);
            SubVector(dir, pos, param.vPos);
            Normalize(dir, dir);
            ret = -atan2f(dir.y, _sqrtf(dir.z * dir.z + dir.x * dir.x));
            _y = getRot().y;
            param.vRot.set(ret, _y, 0.0f);
            CEntityFactory::getInstance()->createBullet(param, this);
            m_iStep = 2;
        }
    case 2:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

void CEnemyDefender::endStateAttack2() {}

// SOURCE DATA: .data .rodata
