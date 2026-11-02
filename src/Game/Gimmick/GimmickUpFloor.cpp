
#include "Game/Gimmick/GimmickUpFloor.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Movement.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "System/Graphic/Camera.h"

CGimmickUpFloorSwitch::CGimmickUpFloorSwitch(CTask* inParent, INT in_GimID) : CBaseGimmick(inParent), m_ID(in_GimID) {
    m_Step = 0;
}

CGimmickUpFloorSwitch::~CGimmickUpFloorSwitch() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickUpFloorSwitch::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    return TRUE;
}

void CGimmickUpFloorSwitch::AfterInit() {
    SLContainerT<CTask>* pContainer;
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat) {
        changeMotion(3, 0, FALSE, -1, 0, 0);
    } else {
        changeMotion(1, 0, FALSE, -1, 0, 0);
    }
    for (pContainer = ((CGimmickManager*)getSysTask(17))->getChildren().getHead(); pContainer != NULL;
         pContainer = pContainer->getNext()) {
        CGimmickUpFloor* p = (CGimmickUpFloor*)pContainer->getValue();
        switch (p->getID() % 10000U) {
        case 0x38C:
        case 0x38D:
        case 0x390:
            p->changeState(m_Stat);
            break;
        }
    }
}

void CGimmickUpFloorSwitch::beginStateMain() {}

void CGimmickUpFloorSwitch::stateMain() {}

void CGimmickUpFloorSwitch::endStateMain() {}

void CGimmickUpFloorSwitch::beginStateHitAttack() {
    SLContainerT<CTask>* pContainer;
    if (m_Stat ^= 1) {
        changeMotion(4, 0, FALSE, -1, 0, 0);
    } else {
        changeMotion(2, 0, FALSE, -1, 0, 0);
    }
    for (pContainer = ((CGimmickManager*)getSysTask(17))->getChildren().getHead(); pContainer != NULL;
         pContainer = pContainer->getNext()) {
        CGimmickUpFloor* p = (CGimmickUpFloor*)pContainer->getValue();
        switch (p->getID() % 10000U) {
        case 0x38C:
        case 0x38D:
        case 0x390:
            p->changeState(m_Stat);
            break;
        }
    }
    changeGimmickState(0);
}

void CGimmickUpFloorSwitch::stateHitAttack() {}

void CGimmickUpFloorSwitch::endStateHitAttack() {}

CGimmickUpFloor::CGimmickUpFloor(CTask* inParent, INT in_GimID) : CBaseGimmick(inParent), m_ID(in_GimID) {
    m_Stat = 0;
    m_Step = 0;
    m_Wait = 0.0f;
}

CGimmickUpFloor::~CGimmickUpFloor() {}

BOOL CGimmickUpFloor::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    return TRUE;
}

void CGimmickUpFloor::AfterInit() {
    INT BoneIndex;
    m_DefaultY = getPos().y;
    switch (getParam()) {
    case 0:
        m_targetY = getPos().y - 4.5f;
        break;
    case 1:
        m_targetY = getPos().y - 6.0f;
        break;
    case 2:
        m_targetY = getPos().y - 7.5f;
        break;
    case 3:
        m_targetY = getPos().y - 9.0f;
        break;
    case 4:
        m_targetY = getPos().y - 10.5f;
        break;
    case 5:
        m_targetY = getPos().y - 12.0f;
        break;
    case 6:
        m_targetY = getPos().y - 13.5f;
        break;
    case 7:
        m_targetY = getPos().y - 15.0f;
        break;
    }
    changeMotion(1, 0, FALSE, -1, 0, 0);
    BoneIndex = getBoneIndex("col", 0);
    if (BoneIndex != -1) rideModelSetAttachMatrix(&Crd(BoneIndex).getWorldMatrix());
}

void CGimmickUpFloor::beginStateMain() {}

void CGimmickUpFloor::stateMain() {
    switch (m_Step) {
    case 0:
        break;
    case 3:
        if (downFloor()) {
            m_Step = 4;
            m_Wait = 60.0f;
        }
        break;
    case 4:
        m_Wait -= getFrameRateF();
        if (m_Wait <= 0.0f) m_Step = 1;
        break;
    case 1:
        if (upFloor()) {
            m_Step = 2;
            m_Wait = 60.0f;
        }
        break;
    case 2:
        m_Wait -= getFrameRateF();
        if (m_Wait <= 0.0f) {
            if (m_Stat) {
                m_Step = 3;
            } else {
                m_Step = 0;
            }
        }
        break;
    }
}

void CGimmickUpFloor::endStateMain() {}

BOOL CGimmickUpFloor::upFloor() {
    INT ret;
    FVECTOR v = getPos();
    ret = FALSE;
    v.y += getFrameRateF() * 0.03f;
    if (m_DefaultY <= v.y) {
        v.y = m_DefaultY;
        ret = TRUE;
    }
    setPos(v);
    return ret;
}

BOOL CGimmickUpFloor::downFloor() {
    INT ret;
    FVECTOR v = getPos();
    ret = FALSE;
    v.y -= getFrameRateF() * 0.03f;
    if (v.y <= m_targetY) {
        v.y = m_targetY;
        ret = TRUE;
    }
    setPos(v);
    return ret;
}

void CGimmickUpFloor::changeState(INT in_Stat) {
    if (in_Stat) {
        m_Stat = 3;
        m_Step = 3;
    } else {
        m_Stat = 0;
    }
    saveStat(m_Stat);
}
// SOURCE DATA: .rodata
