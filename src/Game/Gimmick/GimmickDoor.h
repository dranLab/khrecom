// PARTIAL: added for Game/Gimmick/GimmickPlantParent
#ifndef GAME_GIMMICK_GIMMICKDOOR_H
#define GAME_GIMMICK_GIMMICKDOOR_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickDoor : public CBaseGimmick {
public:
    CGimmickDoor(CTask* inParent);
    void setLockOnMarker(BOOL inOn);
};

#endif /* GAME_GIMMICK_GIMMICKDOOR_H */
