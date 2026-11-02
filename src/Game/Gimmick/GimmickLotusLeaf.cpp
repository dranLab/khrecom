#define ANIM_H_GETFLAG_DECL_ONLY /* MATCH: the original calls CAnim::getFlag out of line */
#line 1

#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Gimmick/GimmickLotusLeaf.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in GimmickWindowLid); which
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

#include "System/Graphic/modeldata.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Movement.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"
#include "System/Graphic/Camera.h"

CGimmickKLotusLeaf::CGimmickKLotusLeaf(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_Stat = 0;
}

CGimmickKLotusLeaf::~CGimmickKLotusLeaf() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickKLotusLeaf::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    return TRUE;
}

void CGimmickKLotusLeaf::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat & 1) {
        changeMotion(3, 0, FALSE, 0, 0, 0);
    } else {
        changeMotion(1, 0, FALSE, 0, 0, 0);
    }
    rideModelEnableChipCollision(0, TRUE);
    rideModelEnableChipCollision(1, TRUE);
}

void CGimmickKLotusLeaf::beginStateMain() {}

void CGimmickKLotusLeaf::stateMain() {
    rideModelSetChipLocalMatrix(0, &Crd(getBoneIndex("g417_L1", 0)).m_Local);
    rideModelSetChipLocalMatrix(1, &Crd(getBoneIndex("g417_R1", 0)).m_Local);
}

void CGimmickKLotusLeaf::endStateMain() {}

void CGimmickKLotusLeaf::beginStateHitAttack() {
    if (!(m_pAnim->getFlag(0) & 3)) {
        changeGimmickState(0);
        return;
    }
    m_Stat = (m_Stat | 0x10) ^ 1;
    if (m_Stat & 1) {
        changeMotion(2, 0, FALSE, 0, 0, 0);
    } else {
        changeMotion(4, 0, FALSE, 0, 0, 0);
    }
    saveStat(m_Stat);
    changeGimmickState(0);
}

void CGimmickKLotusLeaf::stateHitAttack() {}

void CGimmickKLotusLeaf::endStateHitAttack() {}

void CGimmickKLotusLeaf::getLockOnPos(FVECTOR& outPos) {
    INT BoneIndex;
    if (m_pAnim->getAnimID() == 1 || m_pAnim->getAnimID() == 4) {
        BoneIndex = getBoneIndex("g417_L1", 0);
    } else {
        BoneIndex = getBoneIndex("g417_R1", 0);
    }
    outPos = Crd(BoneIndex).getWorldMatrix().v[3];
}
// SOURCE DATA: .rodata
