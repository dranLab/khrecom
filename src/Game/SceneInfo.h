// PARTIAL: added for Game/CardKind
#ifndef GAME_SCENEINFO_H
#define GAME_SCENEINFO_H

#include "khtypes.h"

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

enum DOOR_ID {
    DOOR_ID_RU = 0,
    DOOR_ID_RD = 1,
    DOOR_ID_LD = 2,
    DOOR_ID_LU = 3,
    _DOOR_ID_MAX = 4
};

enum DOOR_SHAPE {
    DOOR_SHAPE_NORMAL = 0,
    DOOR_SHAPE_EVENT = 1,
    DOOR_SHAPE_LAST = 2,
    DOOR_SHAPE_WORLD_ENTRANCE = 3,
    DOOR_SHAPE_WORLD_EXIT = 4,
    DOOR_SHAPE_WORLD_EXIT_CA = 5,
    _DOOR_SHAPE_MAX = 6
};

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
    _DOOR_TYPE_DEBUG = 13,
    _DOOR_TYPE_MAX = 14
};

struct FloorLayoutDataHeader { // 0x10
    CHAR _cSig[4];       /* 0x0 */
    USHORT _nVersion;    /* 0x4 */
    USHORT _nFloorCount; /* 0x6 */
    INT* _pFloorOfsTbl;  /* 0x8 */
    void* _pFloorData;   /* 0xc */
};

/* The namespaces FLOOR and RCNORMA come from the mangled names (the ref drops them). */
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

struct EventRoomData { // 0x18
    CHAR _cSig[4];           /* 0x00 */
    USHORT _nVersion;        /* 0x04 */
    USHORT _nWorldCount;     /* 0x06 */
    USHORT _nSceneTbl[1][8]; /* 0x08 */
};

enum RCNORMA_OP {
    RCNORMA_OP_NONE = 0,
    RCNORMA_OP_EQ_LESS = 1,
    RCNORMA_OP_EQ_GREATER = 2,
    RCNORMA_OP_EQUAL = 3,
    RCNORMA_OP_TOTAL = 4
};

enum RCNORMA_COLOR {
    RCNORMA_COLOR_NONE = 0,
    RCNORMA_COLOR_RED = 1,
    RCNORMA_COLOR_GREEN = 2,
    RCNORMA_COLOR_BLUE = 3
};

namespace RCNORMA {
struct NormaSrc { // 0x8
    SHORT _nNumber;  /* 0x0 */
    SHORT _nOp;      /* 0x2 */
    SHORT _nColor;   /* 0x4 */
    SHORT __nDummy;  /* 0x6 */
};
struct DoorNormaSrc { // 0x1c
    SHORT _nDoorType;      /* 0x00 */
    SHORT _nCount;         /* 0x02 */
    NormaSrc _normas[3];   /* 0x04 */
};
struct FloorNormaSrc { // 0x74
    SHORT _nID;            /* 0x00 */
    SHORT __nDummy;        /* 0x02 */
    DoorNormaSrc _normas[4]; /* 0x04 */
};
}

struct NormaDataHeader { // 0x7c
    CHAR _cSig[4];                       /* 0x00 */
    USHORT _nVersion;                    /* 0x04 */
    USHORT _nFloorCount;                 /* 0x06 */
    RCNORMA::FloorNormaSrc _FloorSrc[1]; /* 0x08 */
};

struct WorldSelTable { // 0x8
    UINT _nCount;
    WORLD_ID* _pTbl;
};

// PARTIAL: the ref's default constructor, operator=, operator==, operator!=, set (2), clear and isEmpty are
// missing (inline, never emitted: their parameters are unknown); AREA_SCENE(INT) is guessed for System/RsrcDB
union AREA_SCENE {
    struct {
        SHORT _nScene;
        SHORT _nArea;
    };
    INT _nAreaScene;
    AREA_SCENE(INT nAreaScene) { _nAreaScene = nAreaScene; }
    AREA_SCENE(SHORT nArea, SHORT nScene) {
        _nArea = nArea;
        _nScene = nScene;
    }
    BOOL operator==(const AREA_SCENE& in) const;
    BOOL operator!=(const AREA_SCENE& in) const;
    void set(/* parameters unknown */);
    void clear(/* parameters unknown */);
    BOOL isEmpty(/* parameters unknown */);
};

enum GAMEMODE {
    GAMEMODE_FIELD = 0,
    GAMEMODE_BATTLE = 1,
    GAMEMODE_MINIGAME = 2
};

enum DOOR_STAT {
    DOOR_STAT_NOTHING = 0,
    DOOR_STAT_CLOSED = 1,
    DOOR_STAT_OPENED = 2,
    DOOR_STAT_ONE_WAY = 3,
    DOOR_STAT_UNAVAILABLE = 4,
    _DOOR_STAT_MAX = 5
};

enum DOOR_RC_RESULT {
    DOOR_RC_RESULT_INVALID = -1,
    DOOR_RC_RESULT_NOP = 0,
    DOOR_RC_RESULT_USED = 1,
    DOOR_RC_RESULT_CLEAR_ONE = 2,
    DOOR_RC_RESULT_CLEAR_ALL = 3
};

struct CARD;

struct DoorInfo { // 0x18
    DOOR_ID _id;                           /* 0x00 */
    DOOR_SHAPE _shape;                     /* 0x04 */
    DOOR_TYPE _type;                       /* 0x08 */
    INT _nDstRoom;                         /* 0x0c */
    void* _pStorageInfo;                   /* 0x10 */
    RCNORMA::DoorNormaSrc* _pNormaSrcInfo; /* 0x14 */

    DOOR_STAT getDoorState() const;
    BOOL setDoorState(DOOR_STAT s);
    BOOL isSpecialDoor() const;
    BOOL isEventDoor() const {
        BOOL ret = FALSE;
        if (_type < DOOR_TYPE_OPEN_AFTER_EVENT) ret = _type > DOOR_TYPE_NORMAL;
        return ret;
    }
    BOOL isSystemDoor() const;
    BOOL canSelectWorld() const;
    BOOL canCreateRoom() const;
    BOOL isWorldEntrance() const { return _type == DOOR_TYPE_WORLD_ENTRANCE; }
    BOOL isWorldExit() const { return _shape == DOOR_SHAPE_WORLD_EXIT_CA; }
    INT getNormaKeyCardID() const;
    USHORT getNormaCount() const;
    RCNORMA_COLOR getNormaColor(USHORT nIndex) const;
    USHORT getNormaNumber(USHORT nIndex) const;
    RCNORMA_OP getNormaOperation(USHORT nIndex) const;
    BOOL isNormaCardUsed() const;
    void setFirstUsedCardInfo(BYTE nCardID, BYTE nCardNum);
    BYTE getFirstUsedCardIDForNorma() const;
    BYTE getFirstUsedCardNumForNorma() const;
    BOOL clearOneNorma();
    USHORT getNextNormaIndex() const;
    BOOL isNextNormaKeyCard() const;
    BOOL isNormaCleared() const;
    DOOR_RC_RESULT useNormaCard(const CARD& card);
    void initNorma();
    void resetNorma();
    BOOL isValidNormaIndex(USHORT nIndex) const;
};

struct RoomInfo { // 0x6c
    DoorInfo _doors[4];            /* 0x00 */
    FLOOR::RoomSrcInfo* _pSrcInfo; /* 0x60 */
    void* _pStorageInfo;           /* 0x64 */
    void* _pStorageInfo2;          /* 0x68 */

    BOOL isCreated() const;
    BOOL isFirstTimeAfterRC() const;
    BOOL hasEntered() const;
    void setScene(SHORT nArea, SHORT nScene);
    void setScene(SHORT nScene);
    void resetScene();
    SHORT getSceneNum() const;
    void setCardInfo(BYTE nID, BYTE nNum);
    BYTE getCardID() const;
    BYTE getCardNumber() const;
    BYTE getRoomNameCardID() const;
    BOOL isFixSceneRoom() const {
        BOOL b = FALSE;
        if (_pSrcInfo) b = _pSrcInfo->isFixSceneRoom();
        return b;
    }
    BOOL isStartRoom() const;
    BOOL isEntranceRoom() const;
    BOOL isExceptRoom() const { return !((UINT)_pSrcInfo | (UINT)_pStorageInfo | (UINT)_pStorageInfo2); }
};

enum EVENTROOM_TBLIDX {
    _EVENTROOM_START = 0,
    _EVENTROOM_1 = 1,
    _EVENTROOM_2 = 2,
    _EVENTROOM_3 = 3,
    _EVENTROOM_GOAL = 4,
    __EVENTROOM_MAX = 5,
    _EVENTROOM_START_ENTRANCE = 5,
    _EVENTROOM_GOAL_ENTRANCE = 6,
    _EVENTROOM_LOWER_START_ENTRANCE = 7,
    _EVENTROOM_LOWER_GOAL_ENTRANCE = 8,
    _EVENTROOM_UPPER_START_ENTRANCE = 9,
    _EVENTROOM_UPPER_GOAL_ENTRANCE = 10
};

struct FloorInfo { // 0xd8c
    RoomInfo _rooms[30];            /* 0x000 */
    RoomInfo _roomStartEnt;         /* 0xca8 */
    RoomInfo _roomGoalEnt;          /* 0xd14 */
    USHORT _nRoomCount;             /* 0xd80 */
    SHORT _nEventRoomIdxTbl[5];     /* 0xd82 */
    void clearEventRoomTbl() {
        for (INT i = 0; i < 5; i++) _nEventRoomIdxTbl[i] = -1;
    }
    SHORT getEventRoomIdx(EVENTROOM_TBLIDX i) { return (UINT)i < __EVENTROOM_MAX ? _nEventRoomIdxTbl[i] : -1; }
};


struct _GimmickInfo {
    BYTE _nStat;
    BYTE _nID;
};

struct _GimmickRoomInfo {
    _GimmickInfo _gimmicks[48];
    SHORT _cardCount;
};


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

// PARTIAL: all public (the ref makes clear .. getMapRsrcCache private; the friends that would need are not
// in the ref); hasSceneCache and getCurrentRoomInfo (inline or unused, never emitted) have guessed parameters
class CSceneInfo {
public:
    static void clear();
    static void dispose();
    static void doneChangeScene();
    static void update();
    static void updateBattleScene();
    static void setNextAreaScene(SHORT nArea, SHORT nField, SHORT nScene);
    static void setNextScene(SHORT nScene, SHORT nBtlScene);
    static void setNextBattleScene(SHORT nScene);
    static void initFloor(INT nFloorIndex, WORLD_ID nWorldID);
    static void initFloor();
    static void clearSceneCache();
    static INT addToSceneCache(const AREA_SCENE& as);
    static INT removeFromSceneCache(const AREA_SCENE& as);
    static BOOL removeFromSceneCacheWithRsrc(const AREA_SCENE& as);
    static void removeCurrentFieldSceneRsrc();
    static void removeCurrentBattleSceneRsrc();
    static INT checkMapRsrcCache(INT nMapID, INT nMode);
    static void updateMapRsrcCache(INT nMapID, INT nMode);
    static void removeMapRsrcCache(INT nMode);
    static INT getMapRsrcCache(INT nMode);
    static void initNewGame(BOOL bSora);
    static BOOL reqChangeFloor(SHORT nFloor, BOOL bFlag);
    static BOOL reqEnterWorld(WORLD_ID nWorldID);
    static BOOL reqChangeRoom(INT nRoomID, DOOR_ID nDoorID, SHORT nScene);
    static BOOL reqChangeEventRoom(UINT nIndex, DOOR_ID nDoorID, SHORT nScene);
    static BOOL reqEnterDoor(DOOR_ID inDoorID);
    static BOOL reqChangeGoalRoom();
    static BOOL reqChangeBackupRoom();
    static BOOL reqEnterBattle();
    static BOOL reqEnterBattle(SHORT inID);
    static BOOL reqExitBattle();
    static BOOL reqEnterMinigame();
    static BOOL reqEnterMinigame(SHORT inID);
    static BOOL reqExitMinigame(SHORT inNextScene);
    static BOOL reqRetryMinigame();
    static BOOL reqChangeScene(SHORT nArea, SHORT nScene, SHORT nBtlScene, BOOL bSkipIfSame, INT nAtelSrcID);
    static SHORT getCurrentAreaNum();
    static SHORT getCurrentSceneNum();
    static INT getCurrentAreaScene();
    static SHORT getCurrentFieldSceneNum();
    static SHORT getCurrentBattleSceneNum();
    static SHORT getNextAreaNum();
    static SHORT getNextSceneNum();
    static INT getNextAreaScene();
    static SHORT getNextBattleSceneNum();
    static BOOL isNextBattleRsrcUpdate();
    static SHORT getPrevAreaNum();
    static SHORT getPrevSceneNum();
    static INT getPrevAreaScene();
    static BOOL isFieldScene();
    static BOOL isBattleScene();
    static BOOL isBossBattleScene(INT nScene);
    static BOOL isEventScene();
    static BOOL isMinigameScene();
    static BOOL isLastFloorScene();
    static BOOL hasReqChangeScene();
    static BOOL hasReqChangeFloor();
    static INT getNextFloorIndex();
    static GAMEMODE hasReqEnterBattle();
    static BOOL hasReqExitBattle();
    static BOOL hasReqRetryBattle();
    static void cancelBattleRequest();
    static BOOL hasAnyRequest();
    static INT getNextAtelSrcID();
    static INT getCurrentSceneCacheID();
    static INT getSceneCacheID(const AREA_SCENE& as);
    static BOOL hasSceneCache(const AREA_SCENE& as);
    static SHORT getSceneNumFromCardID(BYTE nCardID);
    static BYTE getCardIDFromSceneNum(SHORT nScene);
    static INT getConstWorldRsrcID(INT nWorld, BOOL bSora);
    static INT getInconstWorldRsrcID(INT nWorld, BOOL bSora);
    static void setReverbVal(BYTE nRev);
    static BYTE getReverbVal();
    static BYTE getReverbVal(const AREA_SCENE& as);
    static void setSeRangeRatio(BYTE nRatio);
    static BYTE getSeRangeRatio();
    static BYTE getSeRangeRatio(const AREA_SCENE& as);
    static SHORT getCurrentFloorNumber();
    static USHORT getCurrentFloorIndex();
    static WORLD_ID getCurrentWorldID(BOOL bChkEntrance = FALSE);
    static WORLD_ID getWorldID(USHORT nFloorIndex);
    static void setupCompleteFloorWorld(WORLD_ID w);
    static void setEventDoorUnavailable(INT nFloorIndex, INT nRoomID);
    static DOOR_ID setTmpCurrentRoomID(SHORT nRoomID);
    static void ignoreFloorInfo();
    static USHORT getFloorDataCount(BOOL bSora);
    static SHORT getFloorNumber(USHORT nFloorIndex, BOOL bSora);
    static USHORT getFloorIndex(SHORT nFloor, BOOL bSora);
    static FLOOR::FloorLayout* getCurrentFloorDataSrc();
    static FLOOR::FloorLayout* getFloorDataSrc(USHORT nFloorIndex, BOOL bSora);
    static RCNORMA::FloorNormaSrc* getFloorNormaDataSrc(USHORT nIdx, BOOL bSora);
    static WorldSelTable* getSelectableWorldTable(INT nFloorIndex, BOOL bSora);
    static EventRoomData* getEventRoomData(BOOL bSora);
    static SHORT getEventRoomSceneNum(WORLD_ID w, const FLOOR::RoomSrcInfo& info, BOOL bSora);
    static void showAllHiddenDoorsOfCurrentFloor();
    static BOOL isTgsFloor();
    static BOOL isJFestaFloor();
    static INT getCurrentRoomID();
    static INT getNextRoomID();
    static INT getPrevRoomID();
    static _GimmickRoomInfo* getCurrentRoomGimmickInfo();
    static _MapEnemyRoomInfo* getCurrentRoomMapEnemyInfo();
    static RoomInfo* getRoomInfo(INT nRoomID);
    static RoomInfo* getCurrentRoomInfo();
    static RoomInfo* getEventRoomInfo(UINT nIndex);
    static INT getRoomIndex(INT nRoomID);
    static void makeRoom(const RoomInfo& room, DOOR_ID nDoorID, SHORT nScene, BYTE nCardID, BYTE nCardNum);
    static void makeRoom(DoorInfo& door, INT n);
    static RoomInfo* setEventRoomSceneNum(UINT nRoom, SHORT nScene);
    static DoorInfo* getDoorInfo(DOOR_ID inDoorID, INT inRoomID);
    static DOOR_ID getEnteredDoorID();
    static DOOR_ID getNextDoorID();
    static BOOL setDoorStat(DOOR_STAT stat, DOOR_ID id, INT nRoomID);
    static void openGoalDoor();
    static void backupEventDoorState();
    static void rollbackEventDoorState(INT n);
};

#endif /* GAME_SCENEINFO_H */
