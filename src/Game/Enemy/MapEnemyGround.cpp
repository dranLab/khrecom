#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/Enemy/MapEnemyGround.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _mapEnemyGroundEmptyString() {
    TRACE("");
}

#include "Game/Enemy/EnemyParam.h"
#include "Game/Movement.h"
#include "System/Rnd.h"

inline INT CEntity::getMotionCurrentID() {
    return m_pAnim->getAnimID();
}

extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */

CMapEnemyGround::CMapEnemyGround(CTask* inParent) : CMapEnemy(inParent) {}

CMapEnemyGround::~CMapEnemyGround() {}

void CMapEnemyGround::beginStateAppear() {
    changeMapEnemyMotion(0, 0, 0, 0);
    setMapEnemyFlag(1, TRUE);
    setEntityFlag(0, 1);
    bodyCollOn(-1);
}

void CMapEnemyGround::stateAppear() {
    if (RND::genrand_int31() % 30 == 0) {
        changeMapEnemyState(1);
    }
}

void CMapEnemyGround::endStateAppear() {}

void CMapEnemyGround::beginStateMain() {
    m_iStep = 1;
    getRandomTargetPos(m_vTarget, RND::genrand_real1() * 5.0f + 5.0f, 1);
    changeMapEnemyMotion(1, 0, 0, -1);
    setTimer(12000);
}

void CMapEnemyGround::stateMain() {
    switch (m_iStep) {
    case 0:
        if (canPlayerChase(1.5f, 1) && checkGround()) {
            m_iStep = 2;
            changeMapEnemyMotion(2, 0, 0, -1);
        } else if (RND::genrand_int31() % 30 == 0) {
            m_iStep = 1;
            getRandomTargetPos(m_vTarget, RND::genrand_real1() * 5.0f + 3.0f, 1);
            ((FVECTOR&)getRot()).y = calcDirection(m_vTarget);
            changeMapEnemyMotion(1, 0, 0, -1);
            setTimer(12000);
        }
        break;
    case 1:
        if (canPlayerChase(1.5f, 1) && checkGround()) {
            if (getRunMotion() != getMotionCurrentID()) {
                changeMapEnemyMotion(2, 0, 0, -1);
            }
            m_iStep = 2;
            break;
        }
        m_pMove->homingMoveXZ(m_vTarget, 0.1f, m_fWalkSpeed);
        if (getTimer() <= 0 || arriveTarget(m_vTarget, 1.5f) || !checkGround()) {
            m_iStep = 0;
            clearSpeed();
            changeMapEnemyMotion(0, 0, 0, -1);
        }
        break;
    case 2:
        m_pMove->chase(g_pPlayer, 0.1f, m_fRunSpeed);
        if (!canPlayerChase(1.5f, 1) || !checkGround()) {
            changeMapEnemyMotion(0, 0, 0, -1);
            clearSpeed();
            setTimer(6000);
            m_iStep = 3;
        }
        break;
    case 3:
        if (getTimer() <= 0) {
            m_iStep = 0;
        }
        break;
    }
}

void CMapEnemyGround::endStateMain() {
    endTargetChase();
}

// SOURCE DATA: .rodata
