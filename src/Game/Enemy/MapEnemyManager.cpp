
#define MAPENEMY_H_NO_COLL_ANIM /* MATCH: the original has no BodyCollision.h/Anim.h strings */
#include <string.h>

#include "Game/Enemy/MapEnemyManager.h"
#include "Game/OsdDef.h"
#include "Game/MapCardDef.h"
#include "Game/Enemy/MapEnemy.h"
#include "Game/EntityFactory.h"
#include "Game/GameWork.h"
#include "Game/SceneEnv.h"
#include "Game/SceneInfo.h"
#include "System/Debug.h"
#include "System/MainApp.h"







class CRsrcData;

#include "System/RsrcDB.h"






#define CLASS_TYPE_CMAP_ENEMY 0x1002
#define MAP_ENEMY_ROOM_MAX 32
#define FORMATION_EVENT1 22
#define FORMATION_EVENT5 26

CMapEnemyManager::CMapEnemyManager(CTask* inParent) : CTask(inParent, 0x4000) {
    m_iStep = 0;
    m_pMapEnemyParam = NULL;
    m_fAppearDistance = 16.0f;
    m_fDisappearDistance = 56.25f;
    m_iEnemyAppearNum = 3;
    declareSysTask(0x18);
    TRACE("---- Constructor CMapEnemyManager\n");
}

CMapEnemyManager::~CMapEnemyManager() {
    TRACE("---- Destructor CMapEnemyManager\n");
    CGameWork::setFlag(1, 5, FALSE);
    CSceneEnv::setStop(ObjType::OBJ_BIT_CLEAR);
}

void CMapEnemyManager::main() {
}

void CMapEnemyManager::disp() {
}

void CMapEnemyManager::setup() {
    initMapEnemyParam_();
    initMapCardParam_();
    init_();
}

void CMapEnemyManager::initRoomCreate() {
    _MapEnemyRoomInfo* info = CSceneInfo::getCurrentRoomMapEnemyInfo();
    if (info == NULL) {
        return;
    }
    INT index = 0;
    OSD_HEADER* header = (OSD_HEADER*)CRsrcDB::getFirstData_Scene(0x20);
    if (header == NULL || !header->addrMapping()) {
        return;
    }
    if (header->pFormation[0] == NULL) {
        return;
    }
    for (UINT i = 0; i < 16; i++) {
        OBJ_GROUP* group = header->pFormation[0]->getEnemyGroup(i);
        if (group != NULL) {
            OBJ_INFO* objInfo = group->pObjInfo;
            for (INT j = 0; j < group->iObjCount; j++, objInfo++) {
                info->_MapEnemies[index]._byID = objInfo->iUniqueID;
                info->_MapEnemies[index]._byDead = 0;
                index++;
#line 117
                KH_DEBUG(if (!(index < MAP_ENEMY_ROOM_MAX)) AssertApp(__FILE__, __LINE__, "index < MAP_ENEMY_ROOM_MAX", 0);)
            }
        }
    }
    for (INT k = index; k < MAP_ENEMY_ROOM_MAX; k++) {
        info->_MapEnemies[k]._byID = 0;
        info->_MapEnemies[k]._byDead = 0;
    }
}

#line 152
TIMEQ CMapEnemyManager::getFaintTime() const {
    KH_DEBUG(if (!(m_pMapEnemyParam)) AssertApp(__FILE__, __LINE__, "m_pMapEnemyParam", 0);)
    return (TIMEQ)(m_pMapEnemyParam->fFaintTime * 60.0f * 50.0f);
}

#line 160
TIMEQ CMapEnemyManager::getStopTime() const {
    KH_DEBUG(if (!(m_pMapEnemyParam)) AssertApp(__FILE__, __LINE__, "m_pMapEnemyParam", 0);)
    return (TIMEQ)(m_pMapEnemyParam->fStopTime * 60.0f * 50.0f);
}

#line 168
TIMEQ CMapEnemyManager::getConfuseTime() const {
    KH_DEBUG(if (!(m_pMapEnemyParam)) AssertApp(__FILE__, __LINE__, "m_pMapEnemyParam", 0);)
    return (TIMEQ)(m_pMapEnemyParam->fConfuseTime * 60.0f * 50.0f);
}

void CMapEnemyManager::setEntityFlagAllEnemies(UINT inSet, UINT inMask) {
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer != NULL;
         pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task != NULL && task->getTypeID() == CLASS_TYPE_CMAP_ENEMY) {
            ((CEntity*)task)->setEntityFlag(inSet, inMask);
        }
    }
}

BOOL CMapEnemyManager::canAppear() const {
    INT count = 0;
    for (SLContainerT<CTask>* pContainer = ((CMapEnemyManager*)this)->getChildren().getHead(); pContainer != NULL;
         pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task != NULL && task->getTypeID() == CLASS_TYPE_CMAP_ENEMY) {
            count += ((CMapEnemy*)task)->isAppear();
        }
    }
    return count < m_iEnemyAppearNum;
}

void CMapEnemyManager::deleteEnemies() {
    for (SLContainerT<CTask>* pContainer = getChildren().getHead(); pContainer != NULL;
         pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task != NULL) {
            task->out();
        }
    }
}

void CMapEnemyManager::setEncountEnemy(SHORT inEnemyID, SHORT inUniqueID, BYTE inHitPlayerAttack, BYTE inAppearType) {
    _MapEnemyRoomInfo* info = CSceneInfo::getCurrentRoomMapEnemyInfo();
    if (info != NULL && info->_byHitPlayerAttack == 0) {
        info->_sEncountUniqueID = inUniqueID;
        info->_sEncountEnemyID = inEnemyID;
        if (inEnemyID == 0x1C) {
            info->_byFormationNo1 = 0x1B;
            info->_byFormationNo2 = 0x1D;
        }
        info->_byHitPlayerAttack = inHitPlayerAttack;
        info->_byAppearType = inAppearType;
    }
}

#line 255
void CMapEnemyManager::setEncountEventEnemy(INT inFormationNo, BYTE inHitPlayerAttack) {
    _MapEnemyRoomInfo* info = CSceneInfo::getCurrentRoomMapEnemyInfo();
    if (info != NULL) {
        KH_DEBUG(if (!(FORMATION_EVENT1 <= inFormationNo && inFormationNo <= FORMATION_EVENT5)) AssertApp(__FILE__, __LINE__, "FORMATION_EVENT1 <= inFormationNo && inFormationNo <= FORMATION_EVENT5", 0);)
        info->_sEncountEnemyID = 0;
        info->_sEncountUniqueID = 0;
        info->_byFormationNo1 = inFormationNo;
        info->_byFormationNo2 = inFormationNo;
        info->_byHitPlayerAttack = inHitPlayerAttack;
        info->_byAppearType = 0;
    }
}

void CMapEnemyManager::cleanupRoomMapEnemyInfo() {
    _MapEnemyRoomInfo* info = CSceneInfo::getCurrentRoomMapEnemyInfo();
    if (info != NULL) {
        for (INT i = 0; i < MAP_ENEMY_ROOM_MAX; i++) {
            if (info->_MapEnemies[i]._byDead == 2) {
                info->_MapEnemies[i]._byDead = 0;
            }
        }
    }
}

BOOL CMapEnemyManager::init_() {
    OSD_HEADER* header = (OSD_HEADER*)CRsrcDB::getFirstData_Scene(0x20);
    if (header == NULL) {
        return FALSE;
    }
    if (!header->addrMapping()) {
        return FALSE;
    }
    if (header->pFormation[0] == NULL) {
        return FALSE;
    }
    for (UINT i = 0; i < 16; i++) {
        OBJ_GROUP* group = header->pFormation[0]->getEnemyGroup(i);
        if (group != NULL) {
            OBJ_INFO* info = group->pObjInfo;
            for (INT j = 0; j < group->iObjCount; j++, info++) {
                if (canCreateMapEnemy_(info->iUniqueID)) {
                    CEntity* entity = CEntityFactory::getInstance()->createMapEnemy(*info, this, &info->iParam);
                    if (entity != NULL) {
#line 313
                        KH_DEBUG(if (!(entity->getTypeID() == CLASS_TYPE_CMAP_ENEMY)) AssertApp(__FILE__, __LINE__, "entity->getTypeID() == CLASS_TYPE_CMAP_ENEMY", 0);)
                        ((CMapEnemy*)entity)->setUniqueID(info->iUniqueID);
                    }
                }
            }
        }
    }
    return TRUE;
}

BOOL CMapEnemyManager::initMapCardParam_() {
    CRsrcData* pData = CRsrcDB::getRsrc(1, "MapCardParam.bin", NAME_CASE_SENSITIVE);
    MAP_CARD_PARAM_HEADER* header = pData != NULL ? (MAP_CARD_PARAM_HEADER*)CRsrcDB::getRsrcData(*pData) : NULL;
    if (header == NULL) {
        return FALSE;
    }
    if (!header->addrMapping()) {
        return FALSE;
    }
    RoomInfo* room = CSceneInfo::getRoomInfo(-1);
    if (room == NULL) {
        return FALSE;
    }
    MAP_CARD_PARAM* param = header->getMapCardParam(room->getCardID());
    if (param == NULL) {
        return FALSE;
    }
    m_iEnemyAppearNum = param->iEnemyNum;
    _MapEnemyRoomInfo* info = CSceneInfo::getCurrentRoomMapEnemyInfo();
    if (info == NULL) {
        return FALSE;
    }
    info->_byFormationNo1 = param->iFormation1;
    info->_byFormationNo2 = param->iFormation2;
    info->_byHitPlayerAttack = 0;
    info->_byAppearType = 0;
    return TRUE;
}

BOOL CMapEnemyManager::initMapEnemyParam_() {
    CRsrcData* pData = CRsrcDB::getRsrc(1, "MapEnemyParam.bin", NAME_CASE_SENSITIVE);
    MAP_ENEMY_PARAM* param = pData != NULL ? (MAP_ENEMY_PARAM*)CRsrcDB::getRsrcData(*pData) : NULL;
    if (param == NULL || strncmp(param->szCode, "@MEP", 4) != 0) {
        return FALSE;
    }
    m_pMapEnemyParam = param;
    m_fAppearDistance = param->fAppearDist * param->fAppearDist;
    m_fDisappearDistance = param->fDisappearDist * param->fDisappearDist;
    RoomInfo* room = CSceneInfo::getRoomInfo(-1);
    if (room != NULL) {
        switch (room->getCardID()) {
        case 0xA6:
            m_fAppearDistance = m_pMapEnemyParam->fLoomingDarknessAppearDist * m_pMapEnemyParam->fLoomingDarknessAppearDist;
            m_fDisappearDistance = m_pMapEnemyParam->fLoomingDarknessDisappearDist * m_pMapEnemyParam->fLoomingDarknessDisappearDist;
            break;
        case 0xAA:
            m_fAppearDistance = m_pMapEnemyParam->fDeepDarknessAppearDist * m_pMapEnemyParam->fDeepDarknessAppearDist;
            m_fDisappearDistance = m_pMapEnemyParam->fDeepDarknessDisappearDist * m_pMapEnemyParam->fDeepDarknessDisappearDist;
            break;
        }
    }
    return TRUE;
}

BOOL CMapEnemyManager::canCreateMapEnemy_(BYTE inUniqueID) {
    _MapEnemyRoomInfo* info = CSceneInfo::getCurrentRoomMapEnemyInfo();
    if (info == NULL) {
        return FALSE;
    }
    for (INT i = 0; i < MAP_ENEMY_ROOM_MAX; i++) {
        if (info->_MapEnemies[i]._byID == inUniqueID) {
            return !(info->_MapEnemies[i]._byDead == 1 || info->_MapEnemies[i]._byDead == 2);
        }
    }
    TRACE("warning : find not unique ID = %d\n", inUniqueID);
    return TRUE;
}

FLOAT CMapEnemyManager::getAppearDistance() const {
    return m_fAppearDistance;
}

FLOAT CMapEnemyManager::getDisappearDistance() const {
    return m_fDisappearDistance;
}
// SOURCE DATA: .rodata
