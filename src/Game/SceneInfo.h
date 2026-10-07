// PARTIAL: added for Game/CardKind
#ifndef GAME_SCENEINFO_H
#define GAME_SCENEINFO_H

#include "khtypes.h"

struct RoomInfo;

class CSceneInfo {
public:
    static SHORT getCurrentFloorNumber();
    static INT getCurrentRoomID();
    static RoomInfo* getRoomInfo(INT nRoomID);
};

#endif /* GAME_SCENEINFO_H */
