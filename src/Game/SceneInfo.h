// PARTIAL: added for Game/CardKind
#ifndef GAME_SCENEINFO_H
#define GAME_SCENEINFO_H

#include "khtypes.h"

struct RoomInfo;

class CSceneInfo {
public:
    static SHORT getCurrentFloorNumber();
    static BOOL hasReqChangeScene(); // PARTIAL: added for Game/Gimmick/GimmickWarpPoint
    static INT getCurrentRoomID();
    static RoomInfo* getRoomInfo(INT nRoomID);
    static INT getRoomIndex(INT nRoomID); // PARTIAL: added for Form/Menu/MoogleShop
    static void clear(); // PARTIAL: added for System/RootTask
    static void dispose();
};

#endif /* GAME_SCENEINFO_H */
