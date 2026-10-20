#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/Gimmick/GimmickPlantParent.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholders: three empty strings and the task container overflow message (see System/Task.h) follow
 * CollUtil.h's strings in .rodata (as in GimmickWarpPoint); which headers they come from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() {
    TRACE("");
}
inline void _emptyStringPlaceholder2() {
    TRACE("");
}
inline void _taskSLCOverflowPlaceholder() {
    TRACE("\x83\x5e\x83\x58\x83\x4e\x97\x70\x83\x52\x83\x93\x83\x65\x83\x69\x82\xaa MAX \x82\xc9\x93\x9e\x92\x42\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
}

#include "System/Graphic/Model.h"
#include "Game/Movement.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/Gimmick/GimmickDoor.h"
#include "Game/Gimmick/GimmickPlantChild.h"
#include "Game/LockOnManager.h"
#include "Game/EntityFactory.h"
#include "Game/SceneInfo.h"
#include "System/TaskMan.h"
#include "include_asm.h"

CGimmickPlantParent::CGimmickPlantParent(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_hChild[0] = -1;
    m_hChild[1] = -1;
    m_hChild[2] = -1;
    m_hChild[3] = -1;
}

CGimmickPlantParent::~CGimmickPlantParent() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickPlantParent::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, TRUE);
    m_pMove->setCollisionFlag(0xF, FALSE);
    bodyCollOffBody(-1);
    m_pMove->setFlag(8, TRUE);
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    return TRUE;
}

void CGimmickPlantParent::AfterInit() {
    CGimmickManager* pGM;
    CEntity* entity;
    INT i;
    CEntity* pDoor;
    SHORT scene = CSceneInfo::getCurrentSceneNum();

    if (!(scene >= 1000 && scene < 2000)) {
        m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
        if (m_Stat == -1) m_Stat = 0;
    }
    if (m_Stat) {
        m_Step = 2;
        return;
    }
    bodyCollOnBody(-1);
    pGM = (CGimmickManager*)getSysTask(17);
    RoomInfo* pRoom = CSceneInfo::getRoomInfo(-1);
    for (i = 0; i < 4; i++) {
        if (pRoom->_doors[i].getDoorState() == DOOR_STAT_CLOSED) {
            pDoor = pGM->getDoor(pRoom->_doors[i]._id, NULL, NULL);
            if (pDoor && m_hChild[i] == -1) {
                ((CGimmickDoor*)pDoor)->setLockOnMarker(FALSE);
                entity = CEntityFactory::getInstance()->createEntity(11, 0x19F, getSysTask(17), NULL);
                entity->warp(pDoor->getPos());
                entity->setRot(((const CEntity*)pDoor)->getRot());
                m_hChild[i] = entity->getHandle();
            }
        }
    }
}

void CGimmickPlantParent::beginStateMain() {
    if (m_Step == 2) out();
}

void CGimmickPlantParent::stateMain() {
    FLOAT al;
    INT i;

    switch (m_Step) {
    case 0:
        break;
    case 1:
        for (i = 0; i < getModelCount(); i++) {
            al = getModel()[i].getAlphaScale() - getFrameRateF() * 0.005;
            if (al <= 0.0f) {
                al = 0.0f;
                m_Step = 2;
            }
            getModel(i)->setAlphaScale(al);
        }
        break;
    case 2:
        out();
        break;
    }
}

void CGimmickPlantParent::endStateMain() {}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/GimmickPlantParent", beginStateHitAttack__19CGimmickPlantParent);
#if 0
/* Only the scheduling before saveStat differs: ours puts "sb m_Stat" before "li a1,1" (delay slot gets li a1).
 * Both issue in the same cycle (sched2 log); the emission order is sched1's, where the store with its dying
 * source goes first. Tried (2026-10-20): saveStat(m_Stat = 1), an INT/BYTE local for both, saveStat(m_Stat),
 * a comma expression, a volatile store, _ref(m_Stat) = 1 - none moves the store after li a1. */
void CGimmickPlantParent::beginStateHitAttack() {
    CGimmickManager* pGM;
    RoomInfo* pRoom;
    INT i;
    CEntity* pDoor;
    SHORT scene = CSceneInfo::getCurrentSceneNum();

    if (!(scene >= 1000 && scene < 2000)) {
        if (m_pBodyColl->getHitCollBody()->getDamageKind() == 2) {
        createPrize(0, 0);
        changeMotion(2, 0, FALSE, -1, 0, 0);
        bodyCollOffBody(-1);
        setEntityFlag(0, 0x10000);
        m_Step = 1;
        pGM = (CGimmickManager*)getSysTask(17);
        pRoom = CSceneInfo::getRoomInfo(-1);
        if (pRoom) {
            for (i = 0; i < 4; i++) {
                if (pRoom->_doors[i].getDoorState() == DOOR_STAT_CLOSED) {
                    pDoor = pGM->getDoor(pRoom->_doors[i]._id, NULL, NULL);
                    if (pDoor) {
                        ((CGimmickDoor*)pDoor)->setLockOnMarker(TRUE);
                        ((CGimmickPlantChild*)CTaskMan::getTask(m_hChild[i]))->killPlant();
                    }
                }
            }
        }
        m_Stat = 1;
        saveStat(1);
        } else {
            goto other;
        }
    }
    changeGimmickState(0);
    return;
other:
    changeMotion(3, 0, FALSE, -1, 0, 0);
}
#endif

void CGimmickPlantParent::stateHitAttack() {
    changeGimmickState(0);
}

void CGimmickPlantParent::endStateHitAttack() {}
// SOURCE DATA: .rodata
