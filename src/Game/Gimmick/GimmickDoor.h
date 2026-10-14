// PARTIAL: added for Game/Gimmick/GimmickPlantParent
#ifndef GAME_GIMMICK_GIMMICKDOOR_H
#define GAME_GIMMICK_GIMMICKDOOR_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"
#include "Game/SceneInfo.h" // PARTIAL: added for Form/Menu/WorldSelect (DOOR_ID)

class CGimmickDoor : public CBaseGimmick {
private: // PARTIAL: members added for Form/Menu/WorldSelect
    INT m_iStep;           /* 0x220 */
    int m_hEffect[2];      /* 0x224 */
    DOOR_ID m_DoorID;      /* 0x22c */
    DOOR_STAT m_DoorState; /* 0x230 */

public:
    DOOR_ID getDoorID() { return m_DoorID; } // PARTIAL: added for Form/Menu/WorldSelect
    CGimmickDoor(CTask* inParent);
    void setLockOnMarker(BOOL inOn);
};

#endif /* GAME_GIMMICK_GIMMICKDOOR_H */
