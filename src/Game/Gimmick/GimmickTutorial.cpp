
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/Gimmick/GimmickTutorial.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Card.h"
#include "Game/Technic.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in GimmickBreakable); which header
 * it comes from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

inline void* operator new(size_t, void* p) throw() { return p; }
#include "Game/Atel/ChatMan.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "Game/Collision/CollBody.h"

#include "System/TaskMan.h"



#include "System/Graphic/Effect/EffectMan.h"
#include "Game/Movement.h"
#include "Game/AttackMotion.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"
#include "Game/SceneInfo.h"
#include "Game/GameWork.h"
#include "Game/Enemy/MapEnemyManager.h"
#include "System/Graphic/Camera.h"

#include "Game/Gimmick/GimmickManager.h"






CGimmickBreakableTu::CGimmickBreakableTu(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_Stat = 0;
}

CGimmickBreakableTu::~CGimmickBreakableTu() {}

BOOL CGimmickBreakableTu::initalize(INT inGroupID, INT inRsrcID) {
    FVECTOR v;
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    bodyCollOnBody(-1);
    rideModelEnableAllCollision(TRUE);
    v.set(0.0f, 0.1f, -2.0f, 1.0f);
    warp(v);
    CEffectMan::setEffect("barrel_app", 0, ObjType::OBJ_TYPE_GIMMICK, &v, NULL, 0.0f);
    return TRUE;
}

void CGimmickBreakableTu::beginStateMain() {
    if (m_Stat == 1) {
        out();
    } else {
        getModel()->setAlphaScale(0.0f);
    }
}

void CGimmickBreakableTu::stateMain() {
    if (m_Step == 0) {
        FLOAT alpha = getModel()->getAlphaScale();
        alpha += getFrameRateF() * 0.05f;
        if (alpha >= 1.0f) {
            alpha = 1.0f;
            m_Step = 10;
        }
        getModel()->setAlphaScale(alpha);
    }
}

void CGimmickBreakableTu::endStateMain() {}

void CGimmickBreakableTu::beginStateHitAttack() {
    DropPrize prize;
    FVECTOR v;
    prize.StockKind = 0;
    prize.Kind = CARD_KIND_None;
    prize.Num = 0;
    prize.No = 0;
    if (m_Stat == 0) {
        CGimmickManager* mgr = (CGimmickManager*)getSysTask(17);
        prize.Kind = CARD_KIND_Blizzard;
        prize.No = 5;
        prize.Num = 1;
        v.set(0.0f, 0.0f, 0.0f, 1.0f);
        v.y = calcDirection(g_pPlayer);
        mgr->createPrize(prize, 2, &Crd(getBoneIndex("prize", 0)).getWorldMatrix().v[3], &v);
        if (prize.Num != 0) {
            CEffectMan::setEffect("rec_priz_app", 0, ObjType::OBJ_TYPE_GIMMICK,
                                  &Crd(getBoneIndex("prize", 0)).getWorldMatrix(), 0.0f);
        }
    }
    rideModelEnableAllCollision(FALSE);
    changeMotion(2, 0, FALSE, -1, 0, 0);
    m_Stat = 1;
    setEntityFlag(1, 0);
    changeGimmickState(0);
}

void CGimmickBreakableTu::stateHitAttack() {}

void CGimmickBreakableTu::endStateHitAttack() {}

CGimmickCarryTu::CGimmickCarryTu(CTask* inParent) : CGimmickCarry(inParent) {
    m_Step = 0;
}

CGimmickCarryTu::~CGimmickCarryTu() {}

BOOL CGimmickCarryTu::initalize(INT inGroupID, INT inRsrcID) {
    FVECTOR v;
    CEffect* pEfx;
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    v.set(0.0f, 3.0f, -17.0f, 1.0f);
    warp(v);
    pEfx = CEffectMan::getEffect(CEffectMan::setEffect("barrel_app", 0, ObjType::OBJ_TYPE_GIMMICK, &v, NULL, 0.0f));
    pEfx->setScale(1.4f);
    rideModelEnableAllCollision(TRUE);
    changeMotion(1, 0, FALSE, -1, 0, 0);
    if (m_pAttack != NULL) m_pAttack->setCollKind(0);
    setGimmickFlag(0x10, TRUE);
    bodyCollOff(-1);
    bodyCollOffBody(-1);
    m_FlagSetInfo = 1;
    return TRUE;
}

void CGimmickCarryTu::beginStateMain() {
    CGimmickCarry::beginStateMain();
    if (m_Step == 0) getModel()->setAlphaScale(0.0f);
}

void CGimmickCarryTu::stateMain() {
    CGimmickCarry::stateMain();
    if (m_Step == 0) {
        FLOAT alpha = getModel()->getAlphaScale();
        alpha += getFrameRateF() * 0.05f;
        if (alpha >= 1.0f) {
            alpha = 1.0f;
            m_Step = 10;
        }
        getModel()->setAlphaScale(alpha);
    }
}

BOOL CGimmickCarryTu::checkCarryPermission() {
    if (CGimmickCarry::checkCarryPermission()) {
        CGameWork::setFlag(1, 25, TRUE);
        return TRUE;
    }
    return FALSE;
}

CGimmickShadowTu::CGimmickShadowTu(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_Atk = 0;
}

CGimmickShadowTu::~CGimmickShadowTu() {}

INT CGimmickShadowTu::getTypeID() const {
    return 0x1002;
}

BOOL CGimmickShadowTu::initalize(INT inGroupID, INT inRsrcID) {
    FVECTOR v;
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    bodyCollOnBody(-1);
    rideModelEnableAllCollision(TRUE);
    v.set(0.0f, 3.0f, -25.0f, 1.0f);
    warp(v);
    v.set(1.0f, 0.1f, 1.0f, 1.0f);
    setScale(v);
    changeMotion(4, 0, FALSE, -1, 0, 0);
    return TRUE;
}

void CGimmickShadowTu::beginStateMain() {}

void CGimmickShadowTu::stateMain() {
    if (getGimmickFlag() & 2) m_Step = 60;
    switch (m_Step) {
    case 0: {
        FVECTOR v = getScale();
        v.y += getFrameRateF() * 0.02f;
        if (v.y >= 1.0f) {
            v.y = 1.0f;
            setScale(v);
            m_Step = 3;
        } else {
            setScale(v);
        }
        break;
    }
    case 3:
        if (isMotionEnd()) {
            changeMotion(1, 0, FALSE, -1, 0, 0);
            m_Step = 10;
        }
        break;
    case 15:
        changeMotion(2, 0, FALSE, -1, 0, 0);
        m_Step = 20;
        break;
    case 20:
        if (isMotionEnd()) {
            changeMotion(3, 0, FALSE, -1, 0, 0);
            m_Step = 10;
            m_WaitCounter = 30.0f;
        }
        break;
    case 30:
        CGameWork::setFlag(1, 5, TRUE);
        changeMotion(2, 0, FALSE, -1, 0, 0);
        m_Step = 40;
        break;
    case 40:
        if (isMotionEnd()) {
            changeMotion(3, 0, FALSE, -1, 0, 0);
            m_Step = 60;
        }
        break;
    case 60:
        TRACE("\x83\x60\x83\x85\x81\x5b\x83\x67\x83\x8a\x83\x41\x83\x8b\x81\x40\x83\x6f\x83\x67\x83\x8b\x93\xcb\x93\xfc\x81\x40\x83\x82\x81\x5b\x83\x68:%d\n", m_Atk);
        CGameWork::setFlag(1, 5, FALSE);
        CMapEnemyManager::setEncountEventEnemy(24, m_Atk);
        CSceneInfo::reqEnterBattle(0x406);
        break;
    }
}

void CGimmickShadowTu::endStateMain() {}

void CGimmickShadowTu::beginStateHitAttack() {
    if (m_pBodyColl->getHitCollBody()->getDamageKind() == ATK_KIND_GM_DAMAGE) {
        m_Step = 15;
    } else {
        m_Step = 30;
    }
    attachEffect("rec_tutorial.dpx", 0, &Crd(0), NULL, NULL, 0.0f);
    m_Atk = 2;
    changeGimmickState(0);
}

void CGimmickShadowTu::stateHitAttack() {}

void CGimmickShadowTu::endStateHitAttack() {}
// SOURCE DATA: .rodata
