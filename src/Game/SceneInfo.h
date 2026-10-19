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

// PARTIAL: added for Form/Menu/RoomCreation
enum RCNORMA_OP {
    RCNORMA_OP_NONE = 0,
    RCNORMA_OP_EQ_LESS = 1,
    RCNORMA_OP_EQ_GREATER = 2,
    RCNORMA_OP_EQUAL = 3,
    RCNORMA_OP_TOTAL = 4
};

// PARTIAL: added for Form/Menu/RoomCreation
enum RCNORMA_COLOR {
    RCNORMA_COLOR_NONE = 0,
    RCNORMA_COLOR_RED = 1,
    RCNORMA_COLOR_GREEN = 2,
    RCNORMA_COLOR_BLUE = 3
};

// PARTIAL: added for Form/Menu/RoomCreation
namespace RCNORMA {
struct NormaSrc { // 0x8
    SHORT _nNumber;  /* 0x0 */
    SHORT _nOp;      /* 0x2 */
    SHORT _nColor;   /* 0x4 */
    SHORT __nDummy;  /* 0x6 */
};
}

// PARTIAL: added for Form/Menu/RoomCreation
enum DOOR_RC_RESULT {
    DOOR_RC_RESULT_INVALID = -1,
    DOOR_RC_RESULT_NOP = 0,
    DOOR_RC_RESULT_USED = 1,
    DOOR_RC_RESULT_CLEAR_ONE = 2,
    DOOR_RC_RESULT_CLEAR_ALL = 3
};

struct CARD;

struct DoorInfo { // 0x18
    DOOR_ID _id;               /* 0x00 */
    INT _shape;                /* 0x04 */ // PARTIAL: _shape/_type split out for Game/Player/NaviMap
    DOOR_TYPE _type;           /* 0x08 */
    INT _nDstRoom;             /* 0x0c PARTIAL: named for Form/Menu/Camp */
    void* _pStorageInfo;       /* 0x10 */
    void* _pNormaSrcInfo;      /* 0x14 */
    DOOR_STAT getDoorState() const;
    BOOL isSpecialDoor() const; // PARTIAL: added for Game/Player/NaviMap
    // PARTIAL: added for Form/Menu/RoomCreation
    RCNORMA_COLOR getNormaColor(USHORT idx) const;
    USHORT getNormaNumber(USHORT idx) const;
    RCNORMA_OP getNormaOperation(USHORT idx) const;
    void resetNorma();
    INT getNormaKeyCardID() const;
    USHORT getNormaCount() const;
    USHORT getNextNormaIndex() const;
    DOOR_RC_RESULT useNormaCard(const CARD& card);
    // PARTIAL: added for Game/Gimmick/GimmickDoor
    BOOL isEventDoor() const {
        BOOL ret = FALSE;
        if (_type < DOOR_TYPE_OPEN_AFTER_EVENT) ret = _type > DOOR_TYPE_NORMAL;
        return ret;
    }
    BOOL isWorldEntrance() const { return _type == DOOR_TYPE_WORLD_ENTRANCE; } // PARTIAL: added for Game/Player/NaviMap
    BOOL isWorldExit() const { return _shape == 5; } // PARTIAL: added for Game/Player/PlayerManager
};

// PARTIAL: added for Form/DebugMenu
namespace FLOOR {
enum ROOM_TYPE {
    ROOM_TYPE_CARD = 0,
    ROOM_TYPE_START = 1,
    ROOM_TYPE_EVENT1 = 2,
    ROOM_TYPE_EVENT2 = 3,
    ROOM_TYPE_EVENT3 = 4,
    ROOM_TYPE_GOAL = 5,
    ROOM_TYPE_EXTRA = 6,
    ROOM_TYPE_LAST = 7,
    ROOM_TYPE_FIX = 8
};

struct DoorSrcInfo { // 0x4
    SHORT _nDstRoom;
    BYTE _nDoorType;
    BYTE _nChkEventRoom;
};

struct RoomSrcInfo { // 0x1c
    SHORT _nID;
    SHORT _nRoomType;
    SHORT _nFixScene;
    SHORT _dummy;
    SHORT _nPosX;
    SHORT _nPosY;
    DoorSrcInfo _doors[4];

    BOOL isFixSceneRoom() const {
        return (_nRoomType == ROOM_TYPE_FIX || _nRoomType == ROOM_TYPE_EXTRA || _nRoomType == ROOM_TYPE_LAST) &&
               _nFixScene > 0;
    }
};

struct FloorLayout { // 0x2c
    SHORT _nID;
    SHORT _nRoomCount;
    SHORT _nFloorWidth;
    SHORT _nFloorDepth;
    SHORT _nSceneStartEnt;
    SHORT _nSceneGoalEnt;
    SHORT __nDummy00;
    SHORT __nDummy01;
    RoomSrcInfo _rooms[1];
};
} // namespace FLOOR

// PARTIAL: added for Form/DebugMenu
struct WorldSelTable { // 0x8
    UINT _nCount;
    WORLD_ID* _pTbl;
};

// PARTIAL: getCardID added for Game/Enemy/MapEnemyShadow
struct RoomInfo {
    DoorInfo _doors[4]; /* 0x00 */ // PARTIAL: added for Game/Gimmick/GimmickPlantParent
    FLOOR::RoomSrcInfo* _pSrcInfo; /* 0x60 */ // PARTIAL: added for Form/DebugMenu
    void* _pStorageInfo;           /* 0x64 */
    void* _pStorageInfo2;          /* 0x68 */
    BYTE getCardID() const;
    BYTE getRoomNameCardID() const; // PARTIAL: added for Game/Player/NaviMap
    SHORT getSceneNum() const;      // PARTIAL: added for Form/Menu/SaveLoadUtil
    BYTE getCardNumber() const;     // PARTIAL: added for Game/Gimmick/GimmickManager
    BOOL hasEntered() const;        // PARTIAL: added for Form/Menu/Camp
    BOOL isFirstTimeAfterRC() const; // PARTIAL: added for Game/GameSubField
};

// PARTIAL: added for Game/Gimmick/GimmickManager
struct _GimmickInfo {
    BYTE _nStat;
    BYTE _nID;
};

struct _GimmickRoomInfo {
    _GimmickInfo _gimmicks[48];
    SHORT _cardCount;
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

// PARTIAL: added for Form/DebugMenu, Game/GameSubBattle
union AREA_SCENE {
    struct {
        SHORT _nScene;
        SHORT _nArea;
    };
    INT _nAreaScene;
    AREA_SCENE(INT nAreaScene) { _nAreaScene = nAreaScene; } // PARTIAL: added for System/RsrcDB
    AREA_SCENE(SHORT nArea, SHORT nScene) {
        _nArea = nArea;
        _nScene = nScene;
    }
};

class CSceneInfo {
public:
    static SHORT getCurrentFloorNumber();
    static BYTE getSeRangeRatio(); // PARTIAL: added for System/Graphic/WorldEnv
    static BYTE getReverbVal();
    static SHORT getCurrentAreaNum(); // PARTIAL: added for System/DebugProcInfo
    static void reqExitBattle(); // PARTIAL: added for Game/Enemy/BattleEnemyManager
    static SHORT getCurrentBattleSceneNum(); // PARTIAL: added for Game/MinigameManager
    static void reqExitMinigame(SHORT inNextScene); // PARTIAL: added for Game/MinigameManager
    static void reqRetryMinigame(); // PARTIAL: added for Game/MinigameManager
    static BOOL hasReqChangeScene(); // PARTIAL: added for Game/Gimmick/GimmickWarpPoint
    static INT getCurrentRoomID();
    static RoomInfo* getRoomInfo(INT nRoomID);
    static _GimmickRoomInfo* getCurrentRoomGimmickInfo(); // PARTIAL: added for Game/Gimmick/GimmickManager
    static _MapEnemyRoomInfo* getCurrentRoomMapEnemyInfo(); // PARTIAL: added for Game/Enemy/MapEnemyManager
    static INT getRoomIndex(INT nRoomID); // PARTIAL: added for Form/Menu/MoogleShop
    static SHORT getCurrentSceneNum(); // PARTIAL: added for Game/Gimmick/GimmickRideOn
    // PARTIAL: added for Game/Player/PlayerManager
    static SHORT getNextBattleSceneNum();
    static BOOL isBattleScene();
    static BOOL isMinigameScene();
    static DOOR_ID getEnteredDoorID();
    static INT getCurrentWorldID(BOOL bChkEntrance = FALSE); // PARTIAL: added for Game/Gimmick/BaseGimmick (returns WORLD_ID)
    static void clear(); // PARTIAL: added for System/RootTask
    static void dispose();
    static BOOL reqEnterBattle(SHORT inID); // PARTIAL: added for Game/Gimmick/GimmickDeceiveEnemy
    static BOOL reqEnterBattle(); // PARTIAL: added for Game/Enemy/MapEnemy
    static BOOL isFieldScene(); // PARTIAL: added for Game/Player/PlayerWeapon
    static DoorInfo* getDoorInfo(DOOR_ID inDoorID, INT inRoomID); // PARTIAL: added for Game/Player/NaviMap
    static BOOL reqEnterDoor(DOOR_ID inDoorID); // PARTIAL: added for Game/Gimmick/GimmickDoor
    static void makeRoom(DoorInfo& door, INT n); // PARTIAL: added for Form/Menu/RoomCreation
    // PARTIAL: added for Form/Menu/WorldSelect
    static USHORT getCurrentFloorIndex();
    static WORLD_ID getWorldID(USHORT nFloorIndex);
    static BOOL reqEnterWorld(WORLD_ID nWorldID);
    // PARTIAL: added for Form/Menu/SaveLoadUtil
    static USHORT getFloorIndex(SHORT nFloor, BOOL bSora);
    static void initFloor(INT nFloorIndex, WORLD_ID nWorldID);
    static void initFloor(); // PARTIAL: added for Game/Game
    static INT getCurrentAreaScene();
    static void backupEventDoorState();
    // PARTIAL: added for Form/Menu/WorldWarp
    static SHORT getFloorNumber(USHORT nFloorIndex, BOOL bSora);
    static void reqChangeFloor(SHORT nFloor, BOOL bFlag);
    // PARTIAL: added for Form/DebugMenu
    static void doneChangeScene();
    static FLOOR::FloorLayout* getFloorDataSrc(USHORT nFloorIndex, BOOL bSora);
    static FLOOR::FloorLayout* getCurrentFloorDataSrc(); // PARTIAL: added for Form/Menu/Camp
    static SHORT getEventRoomSceneNum(WORLD_ID w, const FLOOR::RoomSrcInfo& info, BOOL bSora);
    static INT getFloorDataCount(BOOL bSora);
    static WorldSelTable* getSelectableWorldTable(INT nFloorIndex, BOOL bSora);
    static SHORT getNextSceneNum();
    // PARTIAL: added for Game/GameSubBattle
    static void removeCurrentBattleSceneRsrc();
    static INT getSceneCacheID(const union AREA_SCENE& as);
    static INT addToSceneCache(const union AREA_SCENE& as);
    static BOOL isBossBattleScene(INT nScene);
    static INT hasReqEnterBattle();
    // PARTIAL: added for Game/GameSubField
    static void removeCurrentFieldSceneRsrc();
    static BOOL isEventScene();
    static INT checkMapRsrcCache(INT nMapID, INT nMode);
    static void updateMapRsrcCache(INT nMapID, INT nMode);
    static SHORT getPrevSceneNum();
    static UINT getCurrentSceneCacheID();
    static BOOL hasAnyRequest(); // PARTIAL: added for Game/Game
    static INT getNextFloorIndex(); // PARTIAL: added for Game/Game
    static BOOL reqChangeBackupRoom(); // PARTIAL: added for Game/Game
    static INT getNextRoomID();
    static BOOL isLastFloorScene();
    static SHORT getNextAreaNum();
    static void setNextAreaScene(SHORT nArea, SHORT nField, SHORT nScene);
    static void update();
    static void initNewGame(BOOL bSora);
    static void setEventDoorUnavailable(INT nFloorIndex, INT nRoomID);
    static void setupCompleteFloorWorld(WORLD_ID w);
    static DOOR_ID setTmpCurrentRoomID(SHORT nRoomID);
    static void openGoalDoor();
    static RoomInfo* getEventRoomInfo(UINT nIndex);
    static void reqChangeGoalRoom();
    static void reqChangeRoom(INT nRoomID, DOOR_ID nDoorID, SHORT nScene);
    static void ignoreFloorInfo();
    static void reqChangeScene(SHORT nArea, SHORT nScene, SHORT nEntrance, INT nMode, INT nAtel);
};

#endif /* GAME_SCENEINFO_H */
