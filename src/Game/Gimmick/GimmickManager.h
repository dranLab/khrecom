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
private:
    // PARTIAL: added for Game/Gimmick/GimmickCarry
    UBYTE _m_0x40[0xc8];   /* 0x040 m_iStep .. 0x0e8 m_MapCardProb */
    FLOAT m_ThrowDirPow;   /* 0x108 */
    FLOAT m_ThrowMaxAngle; /* 0x10c */
    FLOAT m_ThrowMinAngle; /* 0x110 */

public:
    // PARTIAL: added for Game/Gimmick/GimmickCarry
    FLOAT getThrowDirPow() { return m_ThrowDirPow; }
    FLOAT getThrowMaxAngle() { return m_ThrowMaxAngle; }
    FLOAT getThrowMinAngle() { return m_ThrowMinAngle; }
    CGimmickManager(CTask* inParent);
    CTaskSLCList& getChildren() { return CTask::getChildren(); } // PARTIAL: added for Game/Gimmick/GimmickToldPipe
    INT getStorageGimmickState(INT inUniqueID);
    BOOL setStorageGimmickState(INT inUniqueID, BYTE inState);
    INT DecidePrize(INT inGimID, const FVECTOR* inPos, const FVECTOR* inRot, INT inParam); // PARTIAL: added for Game/Gimmick/GimmickHitReturn
    // PARTIAL: added for Game/Gimmick/GimmickTutorial
    void createPrize(const DropPrize& in_Prize, INT in_Style, const FVECTOR* in_pPos, const FVECTOR* in_pRot);
    GimParam* getGimParam(INT inGimID); // PARTIAL: added for Game/Gimmick/GimmickFallTomb
    // PARTIAL: added for Game/Gimmick/GimmickTreasureBox
    DropPrize DecideTreasurePrize();
    void GimDebugTrace(const DropPrize* inPrize);
    void changeTreasureBoxLock(BOOL inLock);
    CEntity* getDoor(DOOR_ID inID, FVECTOR* outPos, FVECTOR* outRot); // PARTIAL: added for Game/Gimmick/GimmickPlantParent
};

#endif /* GAME_GIMMICK_GIMMICKMANAGER_H */
