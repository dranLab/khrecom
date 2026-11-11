
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyAirsoldier.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyAirsoldierEmptyString() {
    TRACE("");
}

#include "Game/Enemy/EnemyParam.h"
#include "Game/Movement.h"
#include "Game/Debug/DebugWork.h"
#include "System/Rnd.h"

FVECTOR getPlayerPos(BOOL flag);
extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */

namespace Airsoldier {
static CHAR* IdlingMotion = "e015_001";
static CHAR* MoveMotion = "e015_002";
static CHAR* KakatootoshiMotion = "e015_050";
static CHAR* JigokutukiMotion = "e015_051";
static CHAR* SpeedParamName = "Speed";
}

CEnemyAirsoldier::CEnemyAirsoldier(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setEntityFlag(0x80, 0);
    setBattleEnemyFlag(0x100, 0);
    setHeartlessFlag(1, 0);
}

CEnemyAirsoldier::~CEnemyAirsoldier() {}

BOOL CEnemyAirsoldier::initalize(INT inGroupID, INT inRsrcID) {
    CHeartless* pThis = this;
    if (!pThis->initalize(inGroupID, inRsrcID, 0)) return FALSE;
    m_pMove->setShiftY(0.25f);
    return TRUE;
}

void CEnemyAirsoldier::beginStateMain() {
    commonFlyInit(Airsoldier::IdlingMotion, Airsoldier::MoveMotion, 0.5f, 1.25f);
}

void CEnemyAirsoldier::stateMain() {
    commonFly();
}

void CEnemyAirsoldier::endStateMain() {
    commonFlyEnd();
}

void CEnemyAirsoldier::beginStateAttack1() {
    changeMotionFileName(Airsoldier::KakatootoshiMotion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyAirsoldier::stateAttack1() {
    switch (m_iStep) {
    case 0:
        if (calcDistanceXZSq(g_pPlayer) > 1.0f) {
            turnToPlayerY(0.17453292f);
            m_pMove->calcVelocityXZ(0.025f);
        }
        if (getMotionCurrentTime() >= 1600) {
            FVECTOR dir;
            dir.set(0.0f, -1.0f, 0.0f);
            m_pMove->calcVelocity(0.1f, dir);
            m_iStep++;
        }
    case 1:
        if (m_pMove->isGround()) {
            clearSpeed();
        }
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

void CEnemyAirsoldier::beginStateAttack2() {
    changeMotionFileName(Airsoldier::JigokutukiMotion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyAirsoldier::stateAttack2() {
    switch (m_iStep) {
    case 0:
        turnToPlayerY(0.17453292f);
        m_pMove->calcVelocityXZ(0.05f);
        if (getMotionCurrentTime() >= 1500) {
            m_iStep++;
        }
        break;
    case 1:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

void CEnemyAirsoldier::aiCardBattle() {
    if (RND::genrand_int31() % getAttackRate() != 0) return;
    FVECTOR plPos = getPlayerPos(FALSE);
    ENEMY_PARAM* param = getEnemyParam();
    INT count;
    int act[2];
    count = 0;
    double dy = plPos.y - getPos().y;
    if (dy < 0) dy = 0 - dy;
    if (dy > 0.75) {
        if (canAttack(0)) {
            act[count++] = param->pActs[0].iActNo;
        }
    } else {
        if (canAttack(1)) {
            act[count++] = param->pActs[1].iActNo;
        }
    }
    if (count == 0) return;
    if (!canCardBattle()) return;
    if (KH_DW(DebugWork::g_dw.enemy.uiFlag) & 8) {
        for (INT i = 0; i < param->iActCount; i++) {
            if (param->pActs[i].iActNo == KH_DW(DebugWork::g_dw.enemy.sActNo)) {
                setBattleEnemyFlag(0x200, 0);
                setAttackState(KH_DW(DebugWork::g_dw.enemy.sActNo) + 2);
                return;
            }
        }
        return;
    }
    setBattleEnemyFlag(0x200, 0);
    setAttackState(act[RND::genrand_int31() % count] + 2);
}

// SOURCE DATA: .data .rodata
