
#include "Game/Gimmick/GimmickToldPipe.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "Game/Movement.h"
#include "Game/AttackMotion.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"

CGimmickToldPipe::CGimmickToldPipe(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_pChild = NULL;
}

CGimmickToldPipe::~CGimmickToldPipe() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickToldPipe::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    changeMotion(1, 0, FALSE, -1, 0, 0);
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    return TRUE;
}

void CGimmickToldPipe::AfterInit() {
    INT SearchID;
    CGimmickManager* pTask;
    SLContainerT<CTask>* pContainer;

    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    pTask = (CGimmickManager*)getSysTask(17);
    if (getID() % 10000U == 0x329) {
        SearchID = 0x32A;
    } else if (getID() % 10000U == 0x32A) {
        SearchID = 0x329;
    } else if (getID() % 10000U == 0x35C) {
        SearchID = 0x35D;
    } else if (getID() % 10000U == 0x35D) {
        SearchID = 0x35C;
    }
    for (pContainer = pTask->getChildren().getHead(); pContainer != NULL; pContainer = pContainer->getNext()) {
        CGimmickToldPipe* p = (CGimmickToldPipe*)pContainer->getValue();
        if (p != NULL && p->getTypeID() == 0x1005 && p->getID() % 10000U == SearchID &&
            getParam() == p->getParam()) {
            m_pChild = p;
        }
    }
}

void CGimmickToldPipe::beginStateMain() {}

void CGimmickToldPipe::stateMain() {}

void CGimmickToldPipe::endStateMain() {}

void CGimmickToldPipe::beginStateHitAttack() {
    changeMotion(2, 0, FALSE, 0, 0, 0);
    if (m_pChild != NULL) {
        m_pChild->reaction();
    }
    changeGimmickState(0);
}

void CGimmickToldPipe::stateHitAttack() {}

void CGimmickToldPipe::endStateHitAttack() {}

void CGimmickToldPipe::reaction() {
    changeMotion(3, 0, FALSE, 0, 0, 0);
    m_pAttack->setCollKind(0);
    if (m_Stat == 0) {
        createPrize(0, 0);
    }
    m_Stat = 1;
    saveStat(1);
}
// SOURCE DATA: .rodata
