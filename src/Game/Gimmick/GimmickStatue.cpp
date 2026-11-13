
#include "Game/Gimmick/GimmickStatue.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in GimmickGuillotine); which
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

#include "Game/Movement.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"

CGimmickStatue::CGimmickStatue(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_Stat = 0;
    m_WaitCounter = 0.0f;
}

CGimmickStatue::~CGimmickStatue() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickStatue::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    bodyCollOnBody(-1);
    bodyCollOn(-1);
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    return TRUE;
}

void CGimmickStatue::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat != 0) {
        changeMotion(3, 0, FALSE, -1, 0, 0);
        rideModelEnableChipCollision(1, FALSE);
        rideModelEnableChipCollision(2, TRUE);
        bodyCollOff(-1);
        rideModelEnableChipCollision(3, TRUE);
        rideModelEnableChipCollision(4, TRUE);
        rideModelEnableChipCollision(5, TRUE);
        setCheckEyeOffFlag(8, 0);
    } else {
        getSysTask(32);
        LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
        rideModelEnableChipCollision(0, TRUE);
        rideModelEnableChipCollision(1, TRUE);
        rideModelEnableChipCollision(2, FALSE);
        rideModelEnableChipCollision(3, TRUE);
        rideModelEnableChipCollision(4, TRUE);
        rideModelEnableChipCollision(5, TRUE);
        setCheckEyeOffFlag(8, 0);
        changeMotion(1, 0, FALSE, -1, 0, 0);
    }
    bodyCollOffBody(5);
    bodyCollOffBody(6);
    bodyCollOffBody(10);
}

void CGimmickStatue::beginStateMain() {}

void CGimmickStatue::stateMain() {
    rideModelSetChipLocalMatrix(0, (const FMATRIX*)&Crd(getBoneIndex("col2", 0)));
    rideModelSetChipLocalMatrix(3, (const FMATRIX*)&Crd(getBoneIndex("col2", 0)));
    rideModelSetChipLocalMatrix(4, (const FMATRIX*)&Crd(getBoneIndex("col2", 0)));
    rideModelSetChipLocalMatrix(5, (const FMATRIX*)&Crd(getBoneIndex("col2", 0)));
    switch (m_Step) {
    case 0:
        break;
    case 10:
        if (isMotionEnd()) {
            changeMotion(3, 0, FALSE, -1, 0, 0);
            rideModelEnableChipCollision(1, FALSE);
            rideModelEnableChipCollision(2, TRUE);
            rideModelEnableChipCollision(0, TRUE);
            bodyCollOffBody(5);
            bodyCollOffBody(6);
            bodyCollOffBody(10);
            m_Step = 20;
        }
        break;
    }
}

void CGimmickStatue::endStateMain() {}

void CGimmickStatue::beginStateHitAttack() {
    if (m_Stat == 0) {
        setEntityFlag(0, 0x10000);
        g_pPlayer->cancelLockon();
        createPrize(0, 0);
    }
    changeMotion(2, 0, FALSE, -1, 0, 0);
    m_Stat = 1;
    saveStat(1);
    m_Step = 10;
    bodyCollOff(-1);
    bodyCollOnBody(5);
    bodyCollOnBody(6);
    bodyCollOnBody(10);
    rideModelEnableChipCollision(0, FALSE);
    changeGimmickState(0);
}

void CGimmickStatue::stateHitAttack() {}

void CGimmickStatue::endStateHitAttack() {}
// SOURCE DATA: .rodata
// SOURCE DATA SLUS_217.99_retail: .rodata
