// PARTIAL: added for Game/CardKind
#ifndef GAME_SCENEINFO_H
#define GAME_SCENEINFO_H

#include "khtypes.h"

// PARTIAL: added for Form/Menu/WorldSelect
enum WORLD_ID {
    WORLD_ID_COMMON = 0,
    WORLD_ID_TRAVERSE = 1,
    WORLD_ID_AGRABAH = 2,
    WORLD_ID_OLYMPOS = 3,
    WORLD_ID_WONDERLAND = 4,
    WORLD_ID_MONSTRO = 5,
    WORLD_ID_HALLOWEEN = 6,
    WORLD_ID_ATLANTICA = 7,
    WORLD_ID_NEVERLAND = 8,
    WORLD_ID_HOLLOWBASTION = 9,
    WORLD_ID_100ACRE = 10,
    WORLD_ID_TWILIGHTTOWN = 11,
    WORLD_ID_DISTINYISLAND = 12,
    WORLD_ID_CASTLE = 13,
    _WORLD_ID_MAX = 14
};

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

// PARTIAL: added for Game/Player/NaviMap
enum DOOR_TYPE {
    DOOR_TYPE_NORMAL = 0,
    DOOR_TYPE_TO_EVENT1 = 1,
    DOOR_TYPE_TO_EVENT2 = 2,
    DOOR_TYPE_TO_EVENT3 = 3,
    DOOR_TYPE_OPEN_AFTER_EVENT = 4,
    DOOR_TYPE_TO_GOAL = 5,
    DOOR_TYPE_TO_EXTRA = 6,
    DOOR_TYPE_ONEWAY = 7,
    DOOR_TYPE_WORLD_ENTRANCE = 8,
    DOOR_TYPE_SELECT_WORLD = 9,
    DOOR_TYPE_FLOOR_CHANGE = 10,
    DOOR_TYPE_TO_LAST = 11,
    DOOR_TYPE_EVENTROOM = 12,
    _DOOR_TYPE_DEBUG = 13
};

struct DoorInfo { // 0x18
    DOOR_ID _id;               /* 0x00 */
    INT _shape;                /* 0x04 */ // PARTIAL: _shape/_type split out for Game/Player/NaviMap
    DOOR_TYPE _type;           /* 0x08 */
    UBYTE _m_0x0c[0xc];        /* 0x0c _nDstRoom .. _pNormaSrcInfo */
    DOOR_STAT getDoorState() const;
    BOOL isSpecialDoor() const; // PARTIAL: added for Game/Player/NaviMap
    // PARTIAL: added for Game/Gimmick/GimmickDoor
    BOOL isEventDoor() const {
        BOOL ret = FALSE;
        if (_type < DOOR_TYPE_OPEN_AFTER_EVENT) ret = _type > DOOR_TYPE_NORMAL;
        return ret;
    }
    BOOL isWorldEntrance() const { return _type == DOOR_TYPE_WORLD_ENTRANCE; } // PARTIAL: added for Game/Player/NaviMap
};

// PARTIAL: getCardID added for Game/Enemy/MapEnemyShadow
struct RoomInfo {
    DoorInfo _doors[4]; /* 0x00 */ // PARTIAL: added for Game/Gimmick/GimmickPlantParent
    BYTE getCardID() const;
    BYTE getRoomNameCardID() const; // PARTIAL: added for Game/Player/NaviMap
    SHORT getSceneNum() const;      // PARTIAL: added for Form/Menu/SaveLoadUtil
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
    static SHORT getCurrentBattleSceneNum(); // PARTIAL: added for Game/MinigameManager
    static void reqExitMinigame(SHORT inNextScene); // PARTIAL: added for Game/MinigameManager
    static void reqRetryMinigame(); // PARTIAL: added for Game/MinigameManager
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
    static DoorInfo* getDoorInfo(DOOR_ID inDoorID, INT inRoomID); // PARTIAL: added for Game/Player/NaviMap
    static BOOL reqEnterDoor(DOOR_ID inDoorID); // PARTIAL: added for Game/Gimmick/GimmickDoor
    // PARTIAL: added for Form/Menu/WorldSelect
    static USHORT getCurrentFloorIndex();
    static WORLD_ID getWorldID(USHORT nFloorIndex);
    static BOOL reqEnterWorld(WORLD_ID nWorldID);
    // PARTIAL: added for Form/Menu/SaveLoadUtil
    static USHORT getFloorIndex(SHORT nFloor, BOOL bSora);
    static void initFloor(INT nFloorIndex, WORLD_ID nWorldID);
    static INT getCurrentAreaScene();
    static void backupEventDoorState();
    // PARTIAL: added for Form/Menu/WorldWarp
    static SHORT getFloorNumber(USHORT nFloorIndex, BOOL bSora);
    static void reqChangeFloor(SHORT nFloor, BOOL bFlag);
};

#endif /* GAME_SCENEINFO_H */
