// PARTIAL: added for Game/Gimmick/GimmickHitMore
#ifndef GAME_GIMMICK_GIMMICKMANAGER_H
#define GAME_GIMMICK_GIMMICKMANAGER_H

#include "khtypes.h"
#include "System/Task.h"

class CGimmickManager : public CTask {
public:
    CGimmickManager(CTask* inParent);
    CTaskSLCList& getChildren() { return CTask::getChildren(); } // PARTIAL: added for Game/Gimmick/GimmickToldPipe
    INT getStorageGimmickState(INT inUniqueID);
    BOOL setStorageGimmickState(INT inUniqueID, BYTE inState);
};

#endif /* GAME_GIMMICK_GIMMICKMANAGER_H */
