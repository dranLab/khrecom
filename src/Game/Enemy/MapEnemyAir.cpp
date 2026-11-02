
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Enemy/MapEnemyAir.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _mapEnemyAirEmptyString() {
    TRACE("");
}

#include "Game/Enemy/EnemyParam.h"
#include "Game/Movement.h"
#include "System/Rnd.h"

inline INT CEntity::getMotionCurrentID() {
    return m_pAnim->getAnimID();
}

extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */

/* (original form unknown: a plain -r folds into a subtraction) */
static inline FLOAT _neg(FLOAT inValue) {
    return -inValue;
}

CMapEnemyAir::CMapEnemyAir(CTask* inParent) : CMapEnemy(inParent) {}

CMapEnemyAir::~CMapEnemyAir() {}

void CMapEnemyAir::beginStateAppear() {
    changeMapEnemyMotion(0, 0, 0, -1);
    setMapEnemyFlag(1, 0);
    setEntityFlag(0, 1);
    bodyCollOn(-1);
}

void CMapEnemyAir::stateAppear() {
    if (RND::genrand_int31() % 30 == 0) {
        changeMapEnemyState(1);
    }
}

void CMapEnemyAir::endStateAppear() {}

void CMapEnemyAir::beginStateMain() {
    m_iStep = 1;
    getRandomTargetPos(m_vTarget, RND::genrand_real1() * 5.0f + 5.0f, 0);
    changeMapEnemyMotion(1, 0, 0, -1);
    setTimer(12000);
}

void CMapEnemyAir::stateMain() {
    if (isLoomingDarkness()) {
        switch (m_iStep) {
        case 0:
            m_pMove->homingMove(g_pPlayer, 0.1f, m_fRunSpeed);
            break;
        case 1:
            m_pMove->homingMove(m_vTarget, 0.1f, m_fRunSpeed);
            if (calcDistanceXZSq(g_pPlayer) > 12.25f) {
                changeMapEnemyMotion(2, 0, 0, -1);
                m_iStep = 0;
            } else if (getTimer() <= 0) {
                getRandomTargetPos(m_vTarget, RND::genrand_real1() + 2.0f, 0);
                FLOAT r = RND::genrand_real1();
                FLOAT y = m_vTarget.y;
                m_vTarget.y = y + ((RND::genrand_int31() & 1) ? r : _neg(r));
                setTimer(12000);
            }
            break;
        }
        return;
    }
    switch (m_iStep) {
    case 0:
        if (canPlayerChase(1.5f, 0)) {
            m_iStep = 2;
            changeMapEnemyMotion(2, 0, 0, -1);
        } else if (RND::genrand_int31() % 30 == 0) {
            m_iStep = 1;
            getRandomTargetPos(m_vTarget, RND::genrand_real1() * 5.0f + 3.0f, 0);
            ((FVECTOR&)getRot()).y = calcDirection(m_vTarget);
            changeMapEnemyMotion(1, 0, 0, -1);
            setTimer(12000);
        }
        break;
    case 1:
        if (canPlayerChase(1.5f, 0)) {
            if (getRunMotion() != getMotionCurrentID()) {
                changeMapEnemyMotion(2, 0, 0, -1);
            }
            m_iStep = 2;
            break;
        }
        m_pMove->homingMove(m_vTarget, 0.1f, m_fWalkSpeed);
        if (getTimer() <= 0 || arriveTarget(m_vTarget, 1.5f)) {
            m_iStep = 0;
            clearSpeed();
            changeMapEnemyMotion(0, 0, 0, -1);
        }
        break;
    case 2:
        m_pMove->homingMove(g_pPlayer, 0.1f, m_fRunSpeed);
        if (!canPlayerChase(1.5f, 0)) {
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

void CMapEnemyAir::endStateMain() {
    endTargetChase();
}

// SOURCE DATA: .rodata
