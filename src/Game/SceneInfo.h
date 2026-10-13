// PARTIAL: added for Game/CardKind
#ifndef GAME_SCENEINFO_H
#define GAME_SCENEINFO_H

#include "khtypes.h"

// PARTIAL: getCardID added for Game/Enemy/MapEnemyShadow
struct RoomInfo {
    BYTE getCardID() const;
};

class CSceneInfo {
public:
    static SHORT getCurrentFloorNumber();
    static BOOL hasReqChangeScene(); // PARTIAL: added for Game/Gimmick/GimmickWarpPoint
    static INT getCurrentRoomID();
    static RoomInfo* getRoomInfo(INT nRoomID);
    static INT getRoomIndex(INT nRoomID); // PARTIAL: added for Form/Menu/MoogleShop
    static SHORT getCurrentSceneNum(); // PARTIAL: added for Game/Gimmick/GimmickRideOn
    static void clear(); // PARTIAL: added for System/RootTask
    static void dispose();
};

#endif /* GAME_SCENEINFO_H */
