// PARTIAL: added for Game/CardKind
#ifndef GAME_SCENEINFO_H
#define GAME_SCENEINFO_H

#include "khtypes.h"

// PARTIAL: added for Game/Gimmick/GimmickPlantParent
enum DOOR_ID {
    DOOR_ID_RU = 0,
    DOOR_ID_RD = 1,
    DOOR_ID_LD = 2,
    DOOR_ID_LU = 3,
    _DOOR_ID_MAX = 4
};

enum DOOR_STAT {
    DOOR_STAT_NOTHING = 0,
    DOOR_STAT_CLOSED = 1,
    DOOR_STAT_OPENED = 2,
    DOOR_STAT_ONE_WAY = 3,
    DOOR_STAT_UNAVAILABLE = 4,
    _DOOR_STAT_MAX = 5
};

struct DoorInfo { // 0x18
    DOOR_ID _id;               /* 0x00 */
    UBYTE _m_0x04[0x14];       /* 0x04 _shape .. _pNormaSrcInfo */
    DOOR_STAT getDoorState() const;
};

// PARTIAL: getCardID added for Game/Enemy/MapEnemyShadow
struct RoomInfo {
    DoorInfo _doors[4]; /* 0x00 */ // PARTIAL: added for Game/Gimmick/GimmickPlantParent
    BYTE getCardID() const;
};

// PARTIAL: added for Game/Enemy/MapEnemyManager
struct _MapEnemyInfo {
    BYTE _byID;
    BYTE _byDead;
};

struct _MapEnemyRoomInfo {
    _MapEnemyInfo _MapEnemies[32];
    SHORT _sEncountUniqueID;
    SHORT _sEncountEnemyID;
    BYTE _byFormationNo1;
    BYTE _byFormationNo2;
    BYTE _byHitPlayerAttack;
    BYTE _byAppearType;
};

class CSceneInfo {
public:
    static SHORT getCurrentFloorNumber();
    static SHORT getCurrentAreaNum(); // PARTIAL: added for System/DebugProcInfo
    static BOOL hasReqChangeScene(); // PARTIAL: added for Game/Gimmick/GimmickWarpPoint
    static INT getCurrentRoomID();
    static RoomInfo* getRoomInfo(INT nRoomID);
    static _MapEnemyRoomInfo* getCurrentRoomMapEnemyInfo(); // PARTIAL: added for Game/Enemy/MapEnemyManager
    static INT getRoomIndex(INT nRoomID); // PARTIAL: added for Form/Menu/MoogleShop
    static SHORT getCurrentSceneNum(); // PARTIAL: added for Game/Gimmick/GimmickRideOn
    static INT getCurrentWorldID(BOOL bChkEntrance = FALSE); // PARTIAL: added for Game/Gimmick/BaseGimmick (returns WORLD_ID)
    static void clear(); // PARTIAL: added for System/RootTask
    static void dispose();
    static BOOL reqEnterBattle(SHORT inID); // PARTIAL: added for Game/Gimmick/GimmickDeceiveEnemy
    static BOOL isFieldScene(); // PARTIAL: added for Game/Player/PlayerWeapon
};

#endif /* GAME_SCENEINFO_H */
