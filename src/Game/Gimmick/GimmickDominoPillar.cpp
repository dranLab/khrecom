
#include "Game/Gimmick/GimmickDominoPillar.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "Game/Movement.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "System/Graphic/Camera.h"

CGimmickDominoPillar::CGimmickDominoPillar(CTask* inParent, INT inRsrcID) : CBaseGimmick(inParent) {
    m_ID = inRsrcID;
    m_Step = 0;
    m_Stat = 0;
    m_OnceFlag = 0;
    pChild[0] = NULL;
    pChild[1] = NULL;
}

CGimmickDominoPillar::~CGimmickDominoPillar() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

INT CGimmickDominoPillar::getTypeID() const {
    return 0x2001;
}

BOOL CGimmickDominoPillar::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    rideModelSetAttachMatrix(&Crd(getBoneIndex("col", 0)).getWorldMatrix());
    rideModelEnableAllCollision(TRUE);
    changeMotion(1, 0, FALSE, -1, 0, 0);
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    return TRUE;
}

void CGimmickDominoPillar::AfterInit() {
    SLContainerT<CTask>* pContainer;
    CGimmickManager* pTask = (CGimmickManager*)getSysTask(17);

    if (m_ID == 0x13C) {
        for (pContainer = pTask->getChildren().getHead(); pContainer != NULL; pContainer = pContainer->getNext()) {
            CGimmickDominoPillar* p = (CGimmickDominoPillar*)pContainer->getValue();
            if (p != NULL && p->getTypeID() == 0x2001 && p != this && getParam() == p->getParam()) {
                if (pChild[0] == NULL) {
                    pChild[0] = p;
                } else {
                    pChild[1] = p;
                }
            }
        }
    }
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat != 0) {
        changeMotion(3, 0, FALSE, 0, 0, 0);
        rideModelEnableAllCollision(FALSE);
        if (m_ID == 0x13C) {
            rideModelEnableAllCollision(TRUE);
            rideModelSetAttachMatrix(&Crd(0).getWorldMatrix());
            rideModelEnableChipCollision(1, TRUE);
            rideModelEnableChipCollision(0, FALSE);
        }
        bodyCollOffBody(-1);
        bodyCollOff(-1);
    } else {
        bodyCollOffBody(-1);
        if (m_ID == 0x13C) {
            getSysTask(32);
            LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
            rideModelEnableChipCollision(1, FALSE);
            bodyCollOnBody(0);
        }
        CModel* pModel = getModel();
        pModel[1].m_nFlag |= 0x100;
        pModel[2].m_nFlag |= 0x100;
        pModel[3].m_nFlag |= 0x100;
        pModel[4].m_nFlag |= 0x100;
    }
}

void CGimmickDominoPillar::beginStateMain() {}

void CGimmickDominoPillar::stateMain() {
    if (m_Stat == 2 && m_ID == 0x13C) {
        if (pChild[0] != NULL) pChild[0]->setNearTranslucence(getNearTranslucence());
        if (pChild[1] != NULL) pChild[1]->setNearTranslucence(getNearTranslucence());
    }
    if (isMotionEnd() && m_Stat == 1) {
        changeMotion(3, 0, FALSE, 0, 0, 0);
        m_Stat = 2;
        bodyCollOffBody(-1);
        CModel* pModel = getModel();
        pModel[1].m_nFlag &= ~0x100;
        pModel[2].m_nFlag &= ~0x100;
        pModel[3].m_nFlag &= ~0x100;
        pModel[4].m_nFlag &= ~0x100;
        if (m_ID == 0x13C) {
            rideModelSetAttachMatrix(&Crd(0).getWorldMatrix());
            rideModelEnableChipCollision(1, TRUE);
            rideModelEnableChipCollision(0, FALSE);
        } else {
            rideModelEnableAllCollision(FALSE);
        }
    }
}

void CGimmickDominoPillar::endStateMain() {}

void CGimmickDominoPillar::beginStateHitAttack() {
    if (!m_OnceFlag) {
        createPrize(0, 0);
        setEntityFlag(0, 0x10000);
        changeMotion(2, 0, FALSE, -1, 0, 0);
        for (int i = 0; i < 2; i++) {
            if (pChild[i] != NULL) {
                pChild[i]->changeMotion(2, 0, FALSE, -1, 0, 0);
                pChild[i]->setStat(1);
                pChild[i]->changeGimmickState(0);
                pChild[i]->bodyCollOnBody(-1);
                pChild[i]->rideModelEnableChipCollision(0, FALSE);
            }
        }
        rideModelEnableChipCollision(0, FALSE);
        bodyCollOnBody(-1);
        bodyCollOffBody(0);
        bodyCollOff(-1);
        m_Stat = 1;
        saveStat(m_Stat);
        m_OnceFlag = 1;
    }
    changeGimmickState(0);
}

void CGimmickDominoPillar::stateHitAttack() {}

void CGimmickDominoPillar::endStateHitAttack() {}
// SOURCE DATA: .rodata
