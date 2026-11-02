
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Gimmick/GimmickRideOn.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in GimmickWindowLid/Bell); which
 * headers they come from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() {
    TRACE("");
}
inline void _emptyStringPlaceholder2() {
    TRACE("");
}

#include "Game/Movement.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"
#include "Game/SceneInfo.h"
#include "System/Graphic/Camera.h"

CGimmickRideOn::CGimmickRideOn(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_WaitCounter = 0;
}

CGimmickRideOn::~CGimmickRideOn() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickRideOn::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    return TRUE;
}

void CGimmickRideOn::AfterInit() {
    INT BoneIndex;
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    BoneIndex = getBoneIndex("col", 0);
    if (BoneIndex != -1) rideModelSetAttachMatrix(&Crd(BoneIndex).getWorldMatrix());
    UINT id = (UINT)getID() % 10000;
    if (id == 1212 || id == 1223) {
        getSysTask(32);
        LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
        rideModelEnableChipCollision(1, TRUE);
    }
    SHORT scene = CSceneInfo::getCurrentSceneNum();
    if (!(scene >= 1000 && scene < 2000)) {
        UINT id = (UINT)getID() % 10000;
        if (id == 1212 || id == 1223) {
            getSysTask(32);
            LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
        }
    }
    changeMotion(1, 0, FALSE, -1, 0, 0);
}

void CGimmickRideOn::beginStateMain() {}

void CGimmickRideOn::stateMain() {
    UINT id = (UINT)getID() % 10000;
    if (id == 1212 || id == 1223) {
        YashiMain();
    } else {
        defaultMain();
    }
}

void CGimmickRideOn::endStateMain() {}

void CGimmickRideOn::beginStateHitAttack() {
    UINT id = (UINT)getID() % 10000;
    if (id == 1212 || id == 1223) {
        changeMotion(2, 0, FALSE, -1, 0, 0);
        m_Step = 10;
    }
    changeGimmickState(0);
}

void CGimmickRideOn::stateHitAttack() {}

void CGimmickRideOn::endStateHitAttack() {}

void CGimmickRideOn::defaultMain() {
    switch (m_Step) {
    case 0:
        if (isRideOnPlayer() && g_pPlayer->getPlayerState() != HANG) {
            m_Step = 10;
            if (m_pAnim->getAnimID() != 2) changeMotion(2, 0, FALSE, -1, 0, 0);
            if (!m_Stat) {
                createPrize(0, 0);
                m_Stat = 1;
            }
        }
        break;
    case 10:
        if (isMotionEnd() && !isRideOnPlayer()) {
            changeMotion(1, 0, FALSE, -1, 0, 0);
            m_Step = 0;
        }
        break;
    }
}

void CGimmickRideOn::YashiMain() {
    rideModelSetChipLocalMatrix(0, &Crd(getBoneIndex("col", 0)).m_Local);
    if (isRideOnPlayer() && g_pPlayer->getPlayerState() != HANG && !m_Stat) {
        createPrize(0, 0);
        m_Stat = 1;
        saveStat(1);
    }
    switch (m_Step) {
    case 0:
        break;
    case 10:
        if (isMotionEnd()) {
            changeMotion(1, 0, FALSE, -1, 0, 0);
            m_Step = 0;
        }
        break;
    }
}
// SOURCE DATA: .rodata
