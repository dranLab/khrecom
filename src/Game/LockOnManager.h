#ifndef GAME_LOCKONMANAGER_H
#define GAME_LOCKONMANAGER_H

#include "khtypes.h"
#include "System/Task.h"
#include "Game/PoolList.h"

class CEntity;

namespace LockOnManager {

struct LOCK_ON_LIST {
    HANDLE hLockOnObject;
    LOCK_ON_LIST* pPrev;
    LOCK_ON_LIST* pNext;
};

class CLockOnManager : public CTask, public CPoolList<LOCK_ON_LIST> {
private:
    static CLockOnManager* m_pInstance;
    HANDLE m_hSecondNearest; /* 0x74 */

public:
    virtual ~CLockOnManager();
    static CLockOnManager& getInstance();
    virtual void main();
    HANDLE getSecondNearestObject() { return m_hSecondNearest; }
    static BOOL registerLockOnObject(HANDLE inHandle);
    static HANDLE getLockOnObject(CEntity& inEntity);
    static HANDLE getLockOnMapEnemy(CEntity& inEntity);
    static HANDLE getRandomLockOnObject();
    static HANDLE changeLockOnObject(HANDLE inHandle);

private:
    BOOL registerLockOnObject_(HANDLE inHandle);
    HANDLE getLockOnObject_(CEntity& inEntity);
    HANDLE getLockOnMapEnemy_(CEntity& inEntity);
    HANDLE getRandomLockOnObject_();
    HANDLE changeLockOnObject_(HANDLE inHandle);
    CLockOnManager();
};

} // namespace LockOnManager

#endif /* GAME_LOCKONMANAGER_H */
