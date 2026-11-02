
#include "Game/Gimmick/GimmickHitMore.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"

CGimmickHitMore::CGimmickHitMore(CTask* inParent) : CBaseGimmick(inParent) {
    m_HitPoint = 3;
    m_Step = 0;
}

CGimmickHitMore::~CGimmickHitMore() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickHitMore::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    return TRUE;
}

void CGimmickHitMore::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
}

void CGimmickHitMore::beginStateMain() {}

void CGimmickHitMore::stateMain() {}

void CGimmickHitMore::endStateMain() {}

void CGimmickHitMore::beginStateHitAttack() {
    if (--m_HitPoint == 0) {
        m_Stat = 1;
        saveStat(1);
        out();
    }
}

void CGimmickHitMore::stateHitAttack() {}

void CGimmickHitMore::endStateHitAttack() {}
// SOURCE DATA: .rodata
