// PARTIAL: added for Game/Bullet/CreateBullet
#ifndef SYSTEM_TASKMAN_H
#define SYSTEM_TASKMAN_H

#include "khtypes.h"
#include "System/Task.h"

class CTaskMan {
public:
    static CTask* getSysTask(UINT nID);
};

#endif /* SYSTEM_TASKMAN_H */
