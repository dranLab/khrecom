
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyNeoshadow.h"

#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyNeoshadowEmptyString() {
    TRACE("");
}

#include "Game/Enemy/EnemyParam.h"
#include "Game/Collision/CollMap.h"
#include "Game/Movement.h"
#include "System/Math3D.h"
#include "System/Rnd.h"
#include <math.h>
extern "C" int rand();

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */
FVECTOR getPlayerPos(BOOL flag);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

namespace Neoshadow {
static CHAR* IdlingMotion = "e027_001";
static CHAR* MoveMotion = "e027_002";
static CHAR* ShadowMoveMotion = "e027_010";
static CHAR* AppearMotion = "e027_011";
static CHAR* DigMotion = "e027_012";
static CHAR* ScratchMotion = "e027_050";
static CHAR* ShadowOut1Motion = "e027_051";
static CHAR* ShadowOut2Motion = "e027_052";
static CHAR* ShadowOut3Motion = "e027_053";
static CHAR* DigRateParamName = "DigRate";
}

CEnemyNeoshadow::CEnemyNeoshadow(CTask* inParent) : CHeartless(inParent, NULL, NULL) {
    setEntityFlag(1, 0);
    m_iDigStep = 0;
}

CEnemyNeoshadow::~CEnemyNeoshadow() {}

void CEnemyNeoshadow::updateAfter() {
    CBattleEnemy::updateAfter();
    FVECTOR vScale;
    FVECTOR vStart;
    FVECTOR vEnd;
    switch (m_iDigStep) {
    case 0:
        return;
    case 1:
        vStart.set(1.0f, 1.0f, 1.0f, 1.0f);
        vEnd.set(1.0f, 0.1f, 1.0f, 1.0f);
        break;
    case 2:
        vStart.set(1.0f, 0.1f, 1.0f, 1.0f);
        vEnd.set(1.0f, 1.0f, 1.0f, 1.0f);
        break;
    }
    FLOAT t = 1.0f - (FLOAT)getTimer() / (FLOAT)m_tqDigTime;
    Math3D::VectorLerp(vScale, vStart, vEnd, t);
    m_pMove->setScl(vScale);
    if (getTimer() <= 0) {
        m_iDigStep = 0;
        m_pMove->setScl(vEnd);
    }
}

void CEnemyNeoshadow::beginStateAppear() {
    setEntityFlag(0, 1);
    if (isStartFaint() || isStartSleep()) {
        setEntityFlag(0x10000, 0);
        changeMotion(getEnemyParam()->iFaintMotionIndex[1], 0, FALSE, -1, 0, 0);
        if (isStartFaint()) {
            setTimer(15000);
            setBadStatusEffect(1, 0.0f);
        } else {
            setTimer(18000);
            setBadStatusEffect(5, 0.0f);
        }
        m_iStep = 0;
    } else {
        changeMotionFileName(Neoshadow::AppearMotion, 0, FALSE, -1, 0, 0);
        userBodyCollOff(-1);
        userBodyCollOffBody(-1);
        setupDig_(2, getMotionTotalTime());
    }
    m_pMove->enableGravity(TRUE);
}

void CEnemyNeoshadow::stateAppear() {
    if (isStartFaint() || isStartSleep()) {
        switch (m_iStep) {
        case 0:
            if (getTimer() <= 0) {
                changeMotion(getEnemyParam()->iFaintMotionIndex[2], 0, FALSE, -1, 0, 0);
                killBadStatusEffect();
                m_iStep++;
            }
            break;
        case 1:
            if (isMotionEnd()) changeDefaultState();
            break;
        }
    } else {
        m_pMove->homingY(g_pPlayer, 0.1f);
        if (isMotionEnd()) changeDefaultState();
    }
}

void CEnemyNeoshadow::endStateAppear() {
    userBodyCollOn(-1);
    userBodyCollOnBody(-1);
}

void CEnemyNeoshadow::beginStateMain() {
    commonMoveInit(Neoshadow::IdlingMotion, Neoshadow::MoveMotion, 1.5f);
}

void CEnemyNeoshadow::stateMain() {
    FVECTOR pos;
    switch (m_iStep) {
    case 0:
        if (rand() % m_iWaitRate == 0) {
            changeMotion(m_iWalkMotIndex, 0, FALSE, -1, 0, 0);
            randAimPos(m_vAimPos, m_fRandMax);
            m_iCount = 0;
            m_iStep = 1;
        } else if (!isStartCardBattle() && !isCardBattle()) {
            if (rand() % (INT)getExtraParam(Neoshadow::DigRateParamName) == 0) {
                changeMotionFileName(Neoshadow::DigMotion, 0, FALSE, -1, 0, 0);
                userBodyCollOff(-1);
                userBodyCollOffBody(-1);
                TIMEQ tq = getMotionTotalTime();
                setupDig_(1, tq);
                setBattleEnemyFlag(0, 0x10);
                m_iStep = 2;
            }
        }
        break;
    case 1:
        if (m_pMove->isWall() && m_iCount <= 0) {
            FLOAT rot = Math3D::LimitPI(m_pMove->getRot().y + 3.1415927f);
            FLOAT len = m_fRandMax + m_fRandMax + rndArea(-m_fRandMax, m_fRandMax);
            m_vAimPos.x = len * sinf(rot);
            m_vAimPos.y = 0;
            m_vAimPos.z = len * cosf(rot);
            m_iCount = 60;
        } else if (m_iCount > 0) {
            m_iCount--;
        }
        if (rand() % 100 == 0) randAimPos(m_vAimPos, m_fRandMax);
        if (calcDistanceXZSq(g_pPlayer) > 2.25f) {
            pos = getPlayerPos(FALSE);
            AddVectorXYZ(pos, pos, m_vAimPos);
            if (!arriveTarget(pos, 1.0f)) {
                m_pMove->homingMoveXZ(pos, 0.1f, m_fSpeed);
                break;
            }
        }
        changeMotion(m_iIdlingMotIndex, 0, FALSE, -1, 0, 0);
        clearSpeed();
        m_iStep = 0;
        break;
    case 2:
        if (isMotionEnd()) {
            changeMotionFileName(Neoshadow::ShadowMoveMotion, 0, FALSE, -1, 0, 0);
            FLOAT rot = Math3D::LimitPI(RND::genrand_real1() * 360.0f * 0.017453292f);
            FLOAT len = RND::genrand_real1() + 2.0f;
            FLOAT x = len * sinf(rot);
            FLOAT z = len * cosf(rot);
            FVECTOR v;
            v.set(x, 0.0f, z);
            AddVectorXYZ(m_vAimPos, v, getPos());
            setTimer(6000);
            m_iStep = 3;
        }
        break;
    case 3:
        m_pMove->homingMoveXZ(m_vAimPos, 0.1f, m_fSpeed);
        if (arriveTarget(m_vAimPos, 1.0f) || getTimer() <= 0) {
            m_vAimPos = getPlayerPos(FALSE);
            setTimer(6000);
            m_iStep = 4;
        }
        break;
    case 4:
        m_pMove->homingMoveXZ(m_vAimPos, 0.1f, m_fSpeed);
        if (arriveTarget(m_vAimPos, 1.5f) || getTimer() <= 0) {
            changeMotionFileName(Neoshadow::ShadowMoveMotion, 0, FALSE, -1, 0, 0);
            setupDig_(2, getMotionTotalTime());
            userBodyCollOnBody(-1);
            clearSpeed();
            m_iStep = 5;
        }
        break;
    case 5:
        if (isMotionEnd()) {
            changeMotionFileName(Neoshadow::IdlingMotion, 0, FALSE, -1, 0, 0);
            userBodyCollOn(-1);
            m_iStep = 0;
            setBattleEnemyFlag(0x10, 0);
        }
        break;
    }
}

void CEnemyNeoshadow::endStateMain() {}

void CEnemyNeoshadow::beginStateAttack1() {
    changeMotionFileName(Neoshadow::ScratchMotion, 0, TRUE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyNeoshadow::stateAttack1() {
    if (m_iStep == 0) {
        turnToPlayerY(0.06981317f);
        if (getMotionCurrentTime() >= 1700) m_iStep++;
    }
    if (isMotionEnd()) endCardBattle(NULL, 6000);
}

void CEnemyNeoshadow::endStateAttack1() {}

void CEnemyNeoshadow::beginStateAttack2() {
    changeMotionFileName(Neoshadow::ShadowOut1Motion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
    setEntityFlag(0, 0x10000);
    userBodyCollOffBody(-1);
}

void CEnemyNeoshadow::stateAttack2() {
    switch (m_iStep) {
    case 0:
        if (isMotionEnd()) {
            setTimer(250);
            m_iStep++;
        }
        break;
    case 1: {
        if (getTimer() > 0) break;
        FVECTOR pos = getPlayerPos(FALSE);
        FVECTOR target;
        FLOAT rot = Math3D::LimitPI(RND::genrand_real1() * 360.0f * 0.017453292f);
        target = pos;
        target.x += cosf(rot) * 2.0f;
        target.z += sinf(rot) * 2.0f;
        target.y = getPos().y;
        CCollMap* coll = m_pMove->getMapCollision();
#line 369
        ASSERT(coll);
        coll->setStartPos((FVECTOR&)getPos());
        coll->setEndPos(target);
        if (coll->collideMap()) pos = *coll->getOutPos();
        else pos = target;
        warp(pos);
        m_pMove->land(4.5f, 1.0f, FALSE);
        changeMotionFileName(Neoshadow::ShadowOut2Motion, 0, FALSE, -1, 0, 0);
        setTimer(12000);
        userBodyCollOnBody(-1);
        m_iStep++;
        break;
    }
    case 2:
        if (getMotionCurrentTime() >= 2000) m_iStep++;
        break;
    case 3:
        turnToPlayerY(0.08726646f);
        m_pMove->calcVelocityXZ(0.125f);
        if (isHitAttack() || getTimer() <= 0) {
            changeMotionFileName(Neoshadow::ShadowOut3Motion, 0, FALSE, -1, 0, 0);
            clearSpeed();
            m_iStep++;
        }
        break;
    case 4:
        if (isMotionEnd()) endCardBattle(NULL, 6000);
        break;
    }
}

void CEnemyNeoshadow::endStateAttack2() {
    setEntityFlag(0x10000, 0);
    userBodyCollOnBody(-1);
}

// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data .rodata
