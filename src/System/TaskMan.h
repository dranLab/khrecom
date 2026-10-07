// PARTIAL: added for Game/Bullet/CreateBullet
#ifndef SYSTEM_TASKMAN_H
#define SYSTEM_TASKMAN_H

#include "khtypes.h"
#include "System/Task.h"

class CTaskMan {
public:
    static CTask* getSysTask(UINT nID);
    static CTask* getTask(HANDLE hTask); // PARTIAL: added for Form/Menu/MoogleShop
};

#endif /* SYSTEM_TASKMAN_H */
