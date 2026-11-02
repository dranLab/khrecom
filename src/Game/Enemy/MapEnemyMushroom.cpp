
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Enemy/MapEnemyMushroom.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _mapEnemyMushroomEmptyString() {
    TRACE("");
}

#include "Game/Enemy/EnemyParam.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "Game/Movement.h"
#include "System/Rnd.h"

extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */

/* (original form unknown: written inline, the constants load in another order) */
static inline FLOAT _frameRate(const CTask* inTask) {
    return inTask->getFrameRate() * 0.020000001f;
}

CMapEnemyMushroom::CMapEnemyMushroom(CTask* inParent) : CMapEnemy(inParent) {}

CMapEnemyMushroom::~CMapEnemyMushroom() {}

void CMapEnemyMushroom::beginStateAppear() {
    changeMapEnemyMotion(0, 0, 0, -1);
    setMapEnemyFlag(1, 0);
    setEntityFlag(0, 0x10001);
    bodyCollOff(-1);
    bodyCollOffBody(-1);
    setTimer(12000);
    setModelAlphaScale(0.0f, -1);
    m_iStep = 0;
}

void CMapEnemyMushroom::stateAppear() {
    switch (m_iStep) {
    case 0:
        if (getTimer() > 0) break;
        if (calcDistanceXZSq(g_pPlayer) > 16.0f) {
            setupFade(0.0f, 1.0f, 3000, -1);
            setAppearEffect();
            m_iStep++;
        }
        break;
    case 1:
        if (!isFade()) {
            changeMapEnemyState(1);
        }
        break;
    }
}

void CMapEnemyMushroom::endStateAppear() {
    bodyCollOn(-1);
    bodyCollOnBody(-1);
    setEntityFlag(0x10000, 0);
}

void CMapEnemyMushroom::beginStateMain() {
    m_iStep = 1;
    getRandomTargetPos(m_vTarget, RND::genrand_real1() * 5.0f + 5.0f, 1);
    setTimer(RND::genrand_int31() % 30 * 50 + 1500);
    changeMapEnemyMotion(1, 0, 0, -1);
}

void CMapEnemyMushroom::stateMain() {
    switch (m_iStep) {
    case 0:
        if (isWhitemushroom()) {
            if (canPlayerChase(1.5f, 1)) {
                changeMapEnemyState(8);
                break;
            }
        } else if (RND::genrand_int31() % 100 == 0) {
            changeMapEnemyState(8);
            break;
        }
        if (getTimer() > 0) break;
        m_iStep = 1;
        getRandomTargetPos(m_vTarget, RND::genrand_real1() * 5.0f + 2.5f, 1);
        changeMapEnemyMotion(1, 0, 0, -1);
        break;
    case 1:
        if (!m_pMove->turnToTargetY(m_vTarget, 0.17453292f)) {
            setTimer(15000);
            m_iStep = 2;
        }
        break;
    case 2:
        if (isWhitemushroom() && canPlayerChase(1.5f, 1)) {
            changeMapEnemyState(8);
            break;
        }
        m_pMove->turnToTargetY(m_vTarget, 0.34906584f);
        m_pMove->calcVelocityXZ(m_fWalkSpeed);
        if (arriveTargetXZ(m_vTarget, m_fWalkSpeed * 2.0f * _frameRate(this)) || !checkGround() || getTimer() <= 0) {
            m_iStep = 0;
            clearSpeed();
            setTimer(RND::genrand_int31() % 30 * 50 + 1500);
            changeMapEnemyMotion(0, 0, 0, -1);
        }
        break;
    }
}

void CMapEnemyMushroom::endStateMain() {}

void CMapEnemyMushroom::beginStateDisappear() {
    m_iStep = 0;
    bodyCollOff(-1);
    bodyCollOffBody(-1);
    changeMotion(m_pEnemyParam->iDisappearMotionIndex, 0, FALSE, -1, 0, 0);
    setupFade(1.0f, 0.0f, 3000, -1);
    setDisappearEffect();
    setMapEnemyFlag(0, 1);
    setEntityFlag(0, 0x10000);
    setMapEnemyFlag(0, 0x40);
}

void CMapEnemyMushroom::stateDisappear() {
    switch (m_iStep) {
    case 0:
        if (!isFade()) {
            m_iStep = 1;
            setTimer(6000);
        }
        break;
    case 1:
        if (getTimer() > 0) break;
        {
            FVECTOR pos;
            getWarpPos(pos);
            warp(pos);
        }
        m_iStep = 2;
        break;
    case 2:
        if (canAppear()) {
            changeMotion(m_pEnemyParam->iAppearMotionIndex, 0, FALSE, -1, 0, 0);
            setupFade(0.0f, 1.0f, 3000, -1);
            setAppearEffect();
            bodyCollOnBody(-1);
            m_iStep = 3;
        }
        break;
    case 3:
        if (isMotionEnd()) {
            changeMapEnemyState(1);
        }
        break;
    }
}

void CMapEnemyMushroom::endStateDisappear() {
    bodyCollOn(-1);
    setupFade(getModel()->getAlphaScale(), 1.0f, 3000, -1);
    setEntityFlag(0x10000, 0);
}

BOOL CMapEnemyMushroom::isWhitemushroom() const {
    return (UINT)((CMapEnemyMushroom*)this)->getID() % 10000 == 16;
}

// SOURCE DATA: .rodata
