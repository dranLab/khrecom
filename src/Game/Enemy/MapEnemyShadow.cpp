
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Enemy/MapEnemyShadow.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in GimmickSteamTrap); which
 * headers they come from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}
inline void _emptyStringPlaceholder2() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}

#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "Game/Movement.h"
#include "Game/Player/Player.h"
#include "Game/SceneInfo.h"
#include "System/Rnd.h"
#include "System/Math3D.h"

CMapEnemyShadow::CMapEnemyShadow(CTask* inParent) : CMapEnemy(inParent) {}

CMapEnemyShadow::~CMapEnemyShadow() {}

BOOL CMapEnemyShadow::initalize(INT inGroupID, INT inRsrcID) {
    RoomInfo* room;
    INT i;
    if (!CMapEnemy::initalize(inGroupID, inRsrcID)) return FALSE;
    room = CSceneInfo::getRoomInfo(-1);
    if (room != NULL && room->getCardID() == 0xAA) {
        for (i = 0; i < getModelCount(); i++) {
            getModel()->changeTexture("m_ex020_01", "m_ex020_02");
        }
    }
    return TRUE;
}

void CMapEnemyShadow::beginStateAppear() {
    m_iStep = 0;
    m_iAppearType = 1;
    bodyCollOff(-1);
    bodyCollOffBody(-1);
    setEntityFlag(1, 0);
    setMapEnemyFlag(0, 1);
    setEntityFlag(0, 0x10000);
    m_pMove->setFlag(9, 0);
}

void CMapEnemyShadow::stateAppear() {
    switch (m_iStep) {
    case 0: {
        m_pMove->homingY(g_pPlayer, 0.1f);
        if (calcDistanceSq(g_pPlayer) <= getAppearDistance()) {
            if (canAppear()) {
                FVECTOR s = {{1.0f, 0.0f, 1.0f, 1.0f}};
                m_pMove->setScl(s);
                setEntityFlag(0, 1);
                setMapEnemyFlag(1, 0);
                changeMotion(11, 0, FALSE, -1, 0, 0);
                setTimer(getMotionTotalTime());
                m_pMove->setFlag(0, 9);
                m_iStep++;
            }
        }
        break;
    }
    case 1: {
        FVECTOR s0;
        FVECTOR s1 = {{1.0f, 0.0f, 1.0f, 1.0f}};
        FVECTOR s2 = {{1.0f, 1.0f, 1.0f, 1.0f}};
        FLOAT rate = 1.0f - (FLOAT)getTimer() / (FLOAT)getMotionTotalTime();
        Math3D::VectorLerp(s0, s1, s2, rate);
        m_pMove->setScl(s0);
        if (isMotionEnd()) {
            m_pMove->setScl(s2);
            changeMapEnemyState(1);
        }
        break;
    }
    }
}

void CMapEnemyShadow::endStateAppear() {
    bodyCollOn(-1);
    bodyCollOnBody(-1);
    setEntityFlag(0x10000, 0);
}

void CMapEnemyShadow::beginStateMain() {
    changeMapEnemyMotion(0, 0, 0, -1);
    m_iStep = 0;
}

void CMapEnemyShadow::stateMain() {
    TIMEQ exeTime;
    if (calcDistanceSq(g_pPlayer) > getDisappearDistance()) {
        changeMapEnemyState(8);
        return;
    }
    switch (m_iStep) {
    case 0:
        if (RND::genrand_int31() % 40 == 0) {
            exeTime = (RND::genrand_int31() % 4 + 3) * 3000;
            if (canPlayerChase(1.5f, 1) && checkGround()) {
                changeMapEnemyMotion(2, 0, 0, -1);
                setTimer(exeTime);
                m_iStep = 2;
            } else {
                getRandomTargetPos(m_vTarget, RND::genrand_real1() * 5.0f + 3.0f, 1);
                changeMapEnemyMotion(1, 0, 0, -1);
                setTimer(exeTime);
                m_iStep = 1;
            }
        }
        break;
    case 1:
        m_pMove->homingMoveXZ(m_vTarget, 0.1f, m_fWalkSpeed);
        if (getTimer() > 0 && !arriveTarget(m_vTarget, 1.5f) && checkGround()) break;
        if (RND::genrand_int31() % 40 == 0) {
            changeMapEnemyState(8);
        } else {
            changeMapEnemyMotion(0, 0, 0, -1);
            clearSpeed();
            m_iStep = 0;
        }
        break;
    case 2:
        m_pMove->homingMoveXZ(g_pPlayer, 0.1f, m_fRunSpeed);
        if (getTimer() <= 0) {
            changeMapEnemyMotion(0, 0, 0, -1);
            clearSpeed();
            m_iStep = 0;
        } else if (!canPlayerChase(1.5f, 1) || !checkGround()) {
            changeMapEnemyState(8);
        }
        break;
    }
}

void CMapEnemyShadow::endStateMain() {}

void CMapEnemyShadow::beginStateDisappear() {
    bodyCollOff(-1);
    bodyCollOffBody(-1);
    setTimer(1500);
    changeMapEnemyMotion(0, 0, 0, -1);
    m_pMove->enableGravity(FALSE);
    setEntityFlag(0, 0x10000);
    setMapEnemyFlag(0, 0x40);
}

void CMapEnemyShadow::stateDisappear() {
    FVECTOR s0;
    FVECTOR s1 = {{1.0f, 1.0f, 1.0f, 1.0f}};
    FVECTOR s2 = {{1.0f, 0.0f, 1.0f, 1.0f}};
    Math3D::VectorLerp(s0, s1, s2, 1.0f - (FLOAT)getTimer() / 1500.0f);
    m_pMove->setScl(s0);
    if (getTimer() <= 0) changeMapEnemyState(0);
}

void CMapEnemyShadow::endStateDisappear() {
    warp(m_vStartPos);
    setEntityFlag(1, 0);
    m_pMove->enableGravity(TRUE);
}
// SOURCE DATA: .rodata
// SOURCE DATA SLUS_217.99_retail: .rodata
