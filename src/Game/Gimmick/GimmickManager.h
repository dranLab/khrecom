// PARTIAL: added for Game/Gimmick/GimmickHitMore
#ifndef GAME_GIMMICK_GIMMICKMANAGER_H
#define GAME_GIMMICK_GIMMICKMANAGER_H

#include "khtypes.h"
#include "System/Task.h"
#include "Game/SceneInfo.h" // PARTIAL: added for Game/Gimmick/GimmickPlantParent

class CEntity;
union FVECTOR;
struct DropPrize; // PARTIAL: added for Game/Gimmick/GimmickTutorial
struct GimParam; // PARTIAL: added for Game/Gimmick/GimmickFallTomb

class CGimmickManager : public CTask {
public:
    CGimmickManager(CTask* inParent);
    CTaskSLCList& getChildren() { return CTask::getChildren(); } // PARTIAL: added for Game/Gimmick/GimmickToldPipe
    INT getStorageGimmickState(INT inUniqueID);
    BOOL setStorageGimmickState(INT inUniqueID, BYTE inState);
    INT DecidePrize(INT inGimID, const FVECTOR* inPos, const FVECTOR* inRot, INT inParam); // PARTIAL: added for Game/Gimmick/GimmickHitReturn
    // PARTIAL: added for Game/Gimmick/GimmickTutorial
    void createPrize(const DropPrize& in_Prize, INT in_Style, const FVECTOR* in_pPos, const FVECTOR* in_pRot);
    GimParam* getGimParam(INT inGimID); // PARTIAL: added for Game/Gimmick/GimmickFallTomb
    CEntity* getDoor(DOOR_ID inID, FVECTOR* outPos, FVECTOR* outRot); // PARTIAL: added for Game/Gimmick/GimmickPlantParent
};

#endif /* GAME_GIMMICK_GIMMICKMANAGER_H */
