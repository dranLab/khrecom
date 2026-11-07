
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Gimmick/GimmickPlantParent.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
#include "Game/Collision/CollBody.h"

/* MATCH: Placeholders: three empty strings, then System/TaskMan.h's container overflow message, follow
 * CollUtil.h's strings in .rodata (as in GimmickWarpPoint); which headers they come from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}
inline void _emptyStringPlaceholder2() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}
#include "System/TaskMan.h"



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
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    bodyCollOffBody(-1);
    m_pMove->setFlag(8, 0);
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
        /* MATCH: holds the original's order (store source, this, argument, then the store in the jal slot): a plain
           m_Stat = 1; saveStat(1); schedules the store first (sched1's register weight). */
        register INT iStat asm("$5");
        register CGimmickPlantParent* pThis asm("$4");
        register INT iByte asm("$2") = 1;
        __asm__ volatile("" : : "r"(iByte));
        pThis = this;
        __asm__ volatile("" : : "r"(pThis));
        iStat = 1;
        __asm__ volatile("" : : "r"(iStat));
        m_Stat = iByte;
        pThis->saveStat(iStat);
        } else {
            goto other;
        }
    }
    changeGimmickState(0);
    return;
other:
    changeMotion(3, 0, FALSE, -1, 0, 0);
}

void CGimmickPlantParent::stateHitAttack() {
    changeGimmickState(0);
}

void CGimmickPlantParent::endStateHitAttack() {}
// SOURCE DATA: .rodata
