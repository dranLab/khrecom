#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/TaskMan.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/BattleEnemy.h"
#include "Game/LockOnManager.h"
#include "System/PadHandler.h"
#include "System/Math3D.h"
#include "System/Rnd.h"
#include <math.h>
extern "C" float atan2f(float y, float x);




#include "System/Graphic/WorldEnv.h"
#include "System/HeapMan.h"
#line 24

void Normalize(FVECTOR& v0, const FVECTOR& v1);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void RotTransPers(FVECTOR& v0, const FMATRIX& m, const FVECTOR& v1);

namespace LockOnManager {

CLockOnManager* CLockOnManager::m_pInstance = NULL;

CLockOnManager::CLockOnManager() : CTask(getSysTask(1), 0x4000), CPoolList<LOCK_ON_LIST>(128) {
    m_hSecondNearest = 0;
    declareSysTask(32);
}

CLockOnManager::~CLockOnManager() {
    CLockOnManager*& rInstance = m_pInstance;
    rInstance = NULL;
}

#line 53
CLockOnManager& CLockOnManager::getInstance() {
    if (m_pInstance == NULL) {
        m_pInstance = new KH_AT("(55)") CLockOnManager;
        if (!m_pInstance) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
    }
    return *m_pInstance;
}

void CLockOnManager::main() {
    LOCK_ON_LIST* p;
    for (p = m_Top.pNext; p->pNext != NULL; p = p->pNext) {
        if (!CTaskMan::getTask(p->hLockOnObject)) {
            releaseList(p);
        }
    }
}

BOOL CLockOnManager::registerLockOnObject(HANDLE inHandle) {
    return getInstance().registerLockOnObject_(inHandle);
}

HANDLE CLockOnManager::getLockOnObject(CEntity& inEntity) {
    return getInstance().getLockOnObject_(inEntity);
}

HANDLE CLockOnManager::getLockOnMapEnemy(CEntity& inEntity) {
    return getInstance().getLockOnMapEnemy_(inEntity);
}

HANDLE CLockOnManager::getRandomLockOnObject() {
    return getInstance().getRandomLockOnObject_();
}

HANDLE CLockOnManager::changeLockOnObject(HANDLE inHandle) {
    return getInstance().changeLockOnObject_(inHandle);
}

BOOL CLockOnManager::registerLockOnObject_(HANDLE inHandle) {
    CTask* task = CTaskMan::getTask(inHandle);
    if (task) {
        INT typeID = task->getTypeID();
        if (typeID == 0x1000 || typeID == 0x1002 || typeID == 0x1004 || typeID == 0x1005 || typeID == 0x2001 ||
            typeID == 0x2000 || typeID == 0x1006 || typeID == 0x2003 || typeID == 0x2004 || typeID == 0x2005 ||
            typeID == 0x4000) {
            LOCK_ON_LIST* p = getList();
            if (p) {
                ((CEntity*)task)->setEntityFlag(0x10000, 0);
                p->hLockOnObject = inHandle;
                return TRUE;
            }
            return FALSE;
        }
    }
    return FALSE;
}

HANDLE CLockOnManager::getLockOnObject_(CEntity& inEntity) {
    FLOAT nearest = 1e8f;
    HANDLE handle = 0;
    FVECTOR pos0;
    FVECTOR pos1;
    m_hSecondNearest = 0;
    inEntity.getLockOnPos(pos0);
    for (LOCK_ON_LIST* p = m_Top.pNext; p->pNext != NULL; p = p->pNext) {
        CEntity* entity = (CEntity*)CTaskMan::getTask(p->hLockOnObject);
        if (entity && entity != &inEntity && entity->isPermissionLockOn()) {
            entity->getLockOnPos(pos1);
            FLOAT dist = Math3D::PointToPointDistanceSq(pos0, pos1);
            if (dist < nearest) {
                m_hSecondNearest = handle;
                nearest = dist;
                handle = p->hLockOnObject;
            }
        }
    }
    if (!m_hSecondNearest) {
        nearest = 1e8f;
        for (LOCK_ON_LIST* p = m_Top.pNext; p->pNext != NULL; p = p->pNext) {
            CEntity* entity = (CEntity*)CTaskMan::getTask(p->hLockOnObject);
            if (entity && entity != &inEntity && entity != CTaskMan::getTask(handle) && entity->isPermissionLockOn()) {
                entity->getLockOnPos(pos1);
                FLOAT dist = Math3D::PointToPointDistanceSq(pos0, pos1);
                if (dist < nearest) {
                    nearest = dist;
                    m_hSecondNearest = p->hLockOnObject;
                }
            }
        }
    }
    return handle;
}

HANDLE CLockOnManager::getLockOnMapEnemy_(CEntity& inEntity) {
    FLOAT nearest = 1e8f;
    HANDLE handle = 0;
    FVECTOR pos0;
    FVECTOR pos1;
    m_hSecondNearest = 0;
    inEntity.getLockOnPos(pos0);
    for (LOCK_ON_LIST* p = m_Top.pNext; p->pNext != NULL; p = p->pNext) {
        CEntity* entity = (CEntity*)CTaskMan::getTask(p->hLockOnObject);
        if (entity && entity != &inEntity && entity->getTypeID() == 0x1002 && entity->isPermissionLockOn()) {
            entity->getLockOnPos(pos1);
            FLOAT dist = Math3D::PointToPointDistanceSq(pos0, pos1);
            if (dist < nearest) {
                m_hSecondNearest = handle;
                nearest = dist;
                handle = p->hLockOnObject;
            }
        }
    }
    if (!m_hSecondNearest) {
        nearest = 1e8f;
        for (LOCK_ON_LIST* p = m_Top.pNext; p->pNext != NULL; p = p->pNext) {
            CEntity* entity = (CEntity*)CTaskMan::getTask(p->hLockOnObject);
            if (entity && entity != &inEntity && entity->getTypeID() == 0x1002 &&
                entity != CTaskMan::getTask(handle) && entity->isPermissionLockOn()) {
                entity->getLockOnPos(pos1);
                FLOAT dist = Math3D::PointToPointDistanceSq(pos0, pos1);
                if (dist < nearest) {
                    nearest = dist;
                    m_hSecondNearest = p->hLockOnObject;
                }
            }
        }
    }
    return handle;
}

HANDLE CLockOnManager::getRandomLockOnObject_() {
    int handle[128];
    INT count = 0;
    for (LOCK_ON_LIST* p = m_Top.pNext; p->pNext != NULL; p = p->pNext) {
        CEntity* entity = (CEntity*)CTaskMan::getTask(p->hLockOnObject);
        if (entity && entity->isPermissionLockOn()) {
            handle[count++] = p->hLockOnObject;
        }
    }
    if (count <= 0) return 0;
    return handle[RND::genrand_int31() % count];
}

HANDLE CLockOnManager::changeLockOnObject_(HANDLE inHandle) {
    CEntity* entity = (CEntity*)CTaskMan::getTask(inHandle);
    if (!entity) return 0;
    FLOAT x = CPadHandler::getAnalogRX(0) * (1.0f / 4096.0f);
    FLOAT y = CPadHandler::getAnalogRY(0) * (1.0f / 4096.0f);
    if (x == 0.0f && y == 0.0f) return 0;
    FVECTOR dir;
    dir.set(x, y, 0.0f, 1.0f);



    Normalize(dir, dir);
    FLOAT deg = atan2f(dir.x, dir.y);
    FMATRIX& m = CWorldEnv::getWorldScreenMatrix();
    FVECTOR center;
    entity->getLockOnPos(center);
    FVECTOR pos0 = center;
    RotTransPers(center, m, center);
    CEntity* object[128];
    INT count = 0;
    for (LOCK_ON_LIST* p = m_Top.pNext; p->pNext != NULL; p = p->pNext) {
        CEntity* obj = (CEntity*)CTaskMan::getTask(p->hLockOnObject);
        FLOAT limit = 1.5707964f; /* set before the tests, as in the original's loop */
        if (obj && obj != entity && obj->isPermissionLockOn()) {
            FVECTOR pos;
            obj->getLockOnPos(pos);
            RotTransPers(pos, m, pos);
            if (1.0f < pos.z && pos.z < 16777000.0f) {
                SubVectorXYZ(pos, pos, center);
                pos.z = 0.0f;
                Normalize(pos, pos);
                FLOAT ret = Math3D::LimitPI(deg - atan2f(pos.x, pos.y));
                if (_fabsf(ret) <= limit) {
                    object[count++] = obj;
                }
            }
        }
    }
    if (count <= 0) return 0;
    FLOAT nearest = 1e8f;
    CEntity* nearestObj = NULL;
    for (INT i = 0; i < count; i++) {
        FVECTOR pos1;
        object[i]->getLockOnPos(pos1);
        FLOAT dist = Math3D::PointToPointDistanceSq(pos0, pos1);
        if (dist < nearest) {
            nearest = dist;
            nearestObj = object[i];
        }
    }
    return nearestObj->getHandle();
}

} // namespace LockOnManager

// SOURCE DATA: .data .rodata
