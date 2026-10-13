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

class CTaskMan {
public:
    static BOOL reqChangeMainTask(INT nID, MAINTASK_CALLMODE nMode); // PARTIAL: added for System/RootTask
    static CTask* getSysTask(UINT nID);
    static CTask* getTask(HANDLE hTask); // PARTIAL: added for Form/Menu/MoogleShop
};

#endif /* SYSTEM_TASKMAN_H */
