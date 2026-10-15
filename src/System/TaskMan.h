// PARTIAL: added for Game/Bullet/CreateBullet
#ifndef SYSTEM_TASKMAN_H
#define SYSTEM_TASKMAN_H

#include "khtypes.h"
#include "System/Task.h"

// PARTIAL: added for System/RootTask
enum MAINTASK_CALLMODE {
    MAINTASK_REPLACE = 0,
    MAINTASK_CALL_REPLACE = 1,
    MAINTASK_CALL_HIDE = 2
};

// PARTIAL: added for System/Task
template <class T> class CVector;
struct TaskMainParam {
    CVector<int>& _SecondaryArray;
    TIMEQ _nSysTickCount;
    TIMEQ _nSysLag;
    TIMEQ _nLag;
    INT _nRnd;
    INT _nProcLevel;
    INT _nFlag;
};

class CTaskMan {
public:
    // PARTIAL: added for System/Task
    static ISLCManagerT<CTask>& getSLCManager();
    static BOOL registerSysTask(UINT nID, CTask& inTask);
    static BOOL unregisterSysTask(CTask& inTask);
    static void removeMainTask(CTask& inTask);
    static BOOL reqChangeMainTask(INT nID, MAINTASK_CALLMODE nMode); // PARTIAL: added for System/RootTask
    static CTask* getSysTask(UINT nID);
    static CTask* getTask(HANDLE hTask); // PARTIAL: added for Form/Menu/MoogleShop
};

#endif /* SYSTEM_TASKMAN_H */
