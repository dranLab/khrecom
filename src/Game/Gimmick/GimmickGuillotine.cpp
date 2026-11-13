
#include "Game/Gimmick/GimmickGuillotine.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in GimmickWindowLid/Bell); which
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

#include "Game/BodyCollision.h"
#include "Game/Movement.h"
#include "Game/AttackMotion.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/Collision/CollBody.h"
CGimmickGuillotine::CGimmickGuillotine(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_Stat = 0;
    m_WaitCounter = 0.0f;
}

CGimmickGuillotine::~CGimmickGuillotine() {}

BOOL CGimmickGuillotine::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    bodyCollOnBody(-1);
    bodyCollOn(-1);
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    return TRUE;
}

void CGimmickGuillotine::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    rideModelEnableChipCollision(0, TRUE);
    rideModelEnableChipCollision(1, TRUE);
    changeMotion(1, 0, FALSE, -1, 0, 0);
}

void CGimmickGuillotine::beginStateMain() {}

void CGimmickGuillotine::stateMain() {
    rideModelSetChipLocalMatrix(0, (const FMATRIX*)&Crd(getBoneIndex("col", 0)));
    switch (m_Step) {
    case 0:
        m_Step = 3;
        m_WaitCounter = 120.0f;
        break;
    case 3:
        m_WaitCounter -= (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_WaitCounter <= 0.0f) m_Step = 5;
        break;
    case 5: {
        changeMotion(2, 0, FALSE, -1, 0, 0);
        if (m_pAttack != NULL) {
            GimParam* param = getGimParam();
            if (param != NULL) m_pAttack->setAttackParam(param->AtkPow, (ATK_ATTR)param->AtkAttr);
        }
        m_Step = 10;
        break;
    }
    case 10:
        if (isMotionEnd()) {
            m_Step = 15;
            m_WaitCounter = 60.0f;
        }
        break;
    case 15:
        m_WaitCounter -= (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_WaitCounter <= 0.0f) {
            changeMotion(3, 0, FALSE, -1, 0, 0);
            m_Step = 20;
        }
        break;
    case 20:
        if (isMotionEnd()) {
            changeMotion(1, 0, FALSE, -1, 0, 0);
            m_Step = 0;
        }
        break;
    }
}

void CGimmickGuillotine::endStateMain() {}

void CGimmickGuillotine::beginStateHitAttack() {
    if (m_pBodyColl->getHitCollBody()->getDamageKind() == ATK_KIND_DMG_BIG && m_Stat == 0) {
        createPrize(0, 0);
        m_Stat = 1;
        saveStat(1);
    }
    changeGimmickState(0);
}

void CGimmickGuillotine::stateHitAttack() {}

void CGimmickGuillotine::endStateHitAttack() {}
// SOURCE DATA: .rodata
// SOURCE DATA SLUS_217.99_retail: .rodata
