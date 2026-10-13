// PARTIAL: added for Game/Gimmick/GimmickHitMore
#ifndef GAME_GIMMICK_GIMMICKMANAGER_H
#define GAME_GIMMICK_GIMMICKMANAGER_H

#include "khtypes.h"
#include "System/Task.h"
#include "Game/SceneInfo.h" // PARTIAL: added for Game/Gimmick/GimmickPlantParent

class CEntity;
union FVECTOR;

class CGimmickManager : public CTask {
public:
    CGimmickManager(CTask* inParent);
    CTaskSLCList& getChildren() { return CTask::getChildren(); } // PARTIAL: added for Game/Gimmick/GimmickToldPipe
    INT getStorageGimmickState(INT inUniqueID);
    BOOL setStorageGimmickState(INT inUniqueID, BYTE inState);
    CEntity* getDoor(DOOR_ID inID, FVECTOR* outPos, FVECTOR* outRot); // PARTIAL: added for Game/Gimmick/GimmickPlantParent
};

#endif /* GAME_GIMMICK_GIMMICKMANAGER_H */
