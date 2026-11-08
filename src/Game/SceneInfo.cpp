#include "khtypes.h"
#define CARD_H_PATH "c:/KHCOM/MainApp/Game/Card.h"     /* MATCH: the original's Card.h/Status.h strings carry */
#define STATUS_H_PATH "c:/KHCOM/MainApp/Game/Status.h" /* the full path (SHA-1 fails without either) */

/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Card.h"
#include "Game/Status.h"
#include "Game/SceneInfo.h"
#include "Game/GameWork.h"
#include "Game/Game.h"
#include "System/Rcf.h"
#include "System/Debug.h"
#include "Form/Menu/JMemoFlag.h"
#include "Game/Status.h"
#include "Game/Atel/Atel.h"
#include "Form/Menu/MoogleShop.h"
#include "Game/Card.h"
#include "include_asm.h"

class CRsrcData;

#include "System/RsrcDB.h"










/* AREA_SCENE without its constructors (built in a register; PARTIAL) */
union _AREA_SCENE_REG {
    struct {
        SHORT _nScene;
        SHORT _nArea;
    };
    INT _nAreaScene;
};

struct _SceneInfo {
    _AREA_SCENE_REG _as;
    BYTE _nReverb;
    BYTE _nSeRangeRatio;
    void clear() {
        _as._nAreaScene = -1;
        _nReverb = 20;
        _nSeRangeRatio = 10;
    }
};

/* RoomInfo::_pStorageInfo, _pStorageInfo2 and DoorInfo::_pStorageInfo (names unknown) */
struct ROOM_STORAGE { // 0xb8, STORAGE::MCWork::_RoomInfo
    SHORT _nArea;               /* 0x00 */
    SHORT _nScene;              /* 0x02 */
    BYTE _bFirstTimeAfterRC;    /* 0x04 */
    BYTE _bEntered;             /* 0x05 */
    UBYTE __nFiller[2];         /* 0x06 */
    UBYTE _gimmickInfo[0x62];   /* 0x08 _GimmickRoomInfo */
    UBYTE _mapEnemyInfo[0x4a];  /* 0x6a _MapEnemyRoomInfo */
    UINT _moogleShopFlag;       /* 0xb4 */
};
struct ROOM_STORAGE2 {
    UBYTE _m_0x00[0x28];
    BYTE _nCardNumber; /* 0x28 */
    BYTE _nCardID;     /* 0x29 */
};
struct DOOR_STORAGE {
    BYTE _nStat;
    UBYTE __nFiller[3];
    UBYTE _nNormaNums[3]; /* 0x04 */
    BYTE _nNextNorma;    /* 0x07 _nClearedNorma */
    BYTE _nFirstCardID;  /* 0x08 */
    BYTE _nFirstCardNum; /* 0x09 */
};

struct _CardScene {
    BYTE _nCardID;
    BYTE _nScene;
};

/* The ref has it only in lost+found.h (owner unknown) */
enum RCNORMA_DOOR {
    RCNORMA_DOOR_EVENT1 = 0,
    RCNORMA_DOOR_EVENT2 = 1,
    RCNORMA_DOOR_EVENT3 = 2,
    RCNORMA_DOOR_EXTRA = 3,
    _RCNORMA_DOOR_MAX = 4
};
struct _SceneInfoWork {
    static STORAGE::MCWork* mc() { return CGameWork::m_pMCWork; }
    static STORAGE::MCWork2* mc2() { return CGameWork::m_pMCWork2; }
};

void _setGoalRoomScene(WORLD_ID w, USHORT nScene);
INT _getEventRoomSceneIndex(SHORT nRoomType);
DOOR_ID _openGoalDoor(const RoomInfo& roomGoal);
void _initEntranceSceneTbl(BOOL bSora);
void _initGoalRoomSceneTbl(BOOL bSora);
void _initAllEventDoorState(BOOL bSora);
void _updateRoomInfo();
void _setOpenFlagEventDoor(const DoorInfo& door);
void _setWorldEntrance(WORLD_ID world, SHORT nStartEntScene, SHORT nGoalEntScene, BOOL bContinue);
void _initRoomInfo(RoomInfo& roomInfo, STORAGE::MCWork::_RoomInfo& st_info, STORAGE::MCWork2::_RoomInfo2& st_info2, const FLOOR::RoomSrcInfo* pSrcInfo, const FLOOR::FloorLayout* pFloorLayout, const RCNORMA::FloorNormaSrc* pFloorNormaSrc, BOOL bContinue);
FloorLayoutDataHeader* _linkFloorData(const CHAR* pszFile);
EventRoomData* _linkEventRoomData(const CHAR* pszFile);
NormaDataHeader* _linkNormaData(const CHAR* pszFile);
USHORT _getGoalRoomScene(WORLD_ID w);
void _clearMapRsrcCache();
FloorLayoutDataHeader* _isValidFloorIdx(USHORT nIdx, BOOL bSora);
extern "C" void AssertApp(char* pszFile, int nLine, char* pszMsg, char* pszExp);
extern "C" int rand();

/* The file's statics, in the original order */
namespace {
INT s_nReqChangeScene = 0;
GAMEMODE s_nReqEnterBattle = GAMEMODE_FIELD;
INT s_nReqExitBattle = 0;
GAMEMODE s_nGameMode = GAMEMODE_FIELD;
INT s_nAtelSrcID = 0;
FloorLayoutDataHeader* s_pFloorFile_Sora = NULL;
FloorLayoutDataHeader* s_pFloorFile_Riku = NULL;
EventRoomData* s_pEventRoomFile_Sora = NULL;
EventRoomData* s_pEventRoomFile_Riku = NULL;
NormaDataHeader* s_pFloorNormaFile_Sora = NULL;
NormaDataHeader* s_pFloorNormaFile_Riku = NULL;
}
static _CardScene s_CardSceneTbl[26] = { { 0xa1, 0x16 }, { 0xa2, 0x03 }, { 0xa3, 0x12 }, { 0xa4, 0x10 }, { 0xa5, 0x0d }, { 0xa6, 0x13 }, { 0xa7, 0x11 }, { 0xa8, 0x14 }, { 0xa9, 0x0c }, { 0xaa, 0x1a }, { 0xab, 0x19 }, { 0xac, 0x15 }, { 0xad, 0x0f }, { 0xae, 0x0a }, { 0xaf, 0x0b }, { 0xb0, 0x09 }, { 0xb1, 0x0e }, { 0xb2, 0x17 }, { 0xb3, 0x05 }, { 0xb4, 0x07 }, { 0xb5, 0x08 }, { 0xb6, 0x06 }, { 0xb7, 0x00 }, { 0xb8, 0x04 }, { 0xb9, 0x00 }, { 0xda, 0x18 } };
namespace {
_AREA_SCENE_REG s_NextAreaScene;
SHORT s_nNextBattleScene;
SHORT s_nCurrentBattleScene;
SHORT s_nCurrentBattleRsrc;
SHORT s_nCurrentFieldScene;
SHORT s_nCurrentFloorNumber;
BYTE s_nNextRoomID;
BYTE s_nNextFloorIdx;
DOOR_ID s_nNextDoorID;
_SceneInfo s_SceneCacheTbl[7];
INT s_nAvailableSceneCache;
INT s_nCurrentSceneCacheID;
_SceneInfo s_GlobalScene;
INT s_nMapCacheField;
INT s_nMapCacheBattle;
FloorInfo s_CurrentFloor;
RoomInfo s_roomExcept;
}
static const WORLD_ID s_WorldTableSora1[1] = { WORLD_ID_TRAVERSE };
static const WORLD_ID s_WorldTableSora2[5] = { WORLD_ID_AGRABAH, WORLD_ID_OLYMPOS, WORLD_ID_WONDERLAND, WORLD_ID_MONSTRO, WORLD_ID_HALLOWEEN };
static const WORLD_ID s_WorldTableSora3[4] = { WORLD_ID_ATLANTICA, WORLD_ID_NEVERLAND, WORLD_ID_HOLLOWBASTION, WORLD_ID_100ACRE };
static const WORLD_ID s_WorldTableSora4[1] = { WORLD_ID_TWILIGHTTOWN };
static const WORLD_ID s_WorldTableSora5[1] = { WORLD_ID_DISTINYISLAND };
static const WORLD_ID s_WorldTableSora6[1] = { WORLD_ID_CASTLE };
static const WORLD_ID s_WorldTableRiku1[1] = { WORLD_ID_HOLLOWBASTION };
static const WORLD_ID s_WorldTableRiku2[4] = { WORLD_ID_TRAVERSE, WORLD_ID_AGRABAH, WORLD_ID_MONSTRO, WORLD_ID_NEVERLAND };
static const WORLD_ID s_WorldTableRiku3[4] = { WORLD_ID_OLYMPOS, WORLD_ID_WONDERLAND, WORLD_ID_HALLOWEEN, WORLD_ID_ATLANTICA };
static const WORLD_ID s_WorldTableRiku4[1] = { WORLD_ID_DISTINYISLAND };
static const WORLD_ID s_WorldTableRiku5[1] = { WORLD_ID_TWILIGHTTOWN };
static const WORLD_ID s_WorldTableRiku6[1] = { WORLD_ID_CASTLE };
static const WORLD_ID s_WorldTableJF1[1] = { WORLD_ID_HALLOWEEN };
static const WORLD_ID s_WorldTableTGS2[1] = { WORLD_ID_OLYMPOS };
static const WORLD_ID s_WorldTablePOO[1] = { WORLD_ID_100ACRE };
static const WorldSelTable s_pWorldTableSora[16] = {
    { 1, (WORLD_ID*)s_WorldTableSora1 },
    { 5, (WORLD_ID*)s_WorldTableSora2 },
    { 5, (WORLD_ID*)s_WorldTableSora2 },
    { 5, (WORLD_ID*)s_WorldTableSora2 },
    { 5, (WORLD_ID*)s_WorldTableSora2 },
    { 5, (WORLD_ID*)s_WorldTableSora2 },
    { 4, (WORLD_ID*)s_WorldTableSora3 },
    { 4, (WORLD_ID*)s_WorldTableSora3 },
    { 4, (WORLD_ID*)s_WorldTableSora3 },
    { 4, (WORLD_ID*)s_WorldTableSora3 },
    { 1, (WORLD_ID*)s_WorldTableSora4 },
    { 1, (WORLD_ID*)s_WorldTableSora5 },
    { 1, (WORLD_ID*)s_WorldTableSora6 },
    { 1, (WORLD_ID*)s_WorldTableJF1 },
    { 1, (WORLD_ID*)s_WorldTableTGS2 },
    { 1, (WORLD_ID*)s_WorldTablePOO }
};
static const WorldSelTable s_pWorldTableRiku[12] = {
    { 1, (WORLD_ID*)s_WorldTableRiku1 },
    { 4, (WORLD_ID*)s_WorldTableRiku2 },
    { 4, (WORLD_ID*)s_WorldTableRiku2 },
    { 4, (WORLD_ID*)s_WorldTableRiku2 },
    { 4, (WORLD_ID*)s_WorldTableRiku2 },
    { 4, (WORLD_ID*)s_WorldTableRiku3 },
    { 4, (WORLD_ID*)s_WorldTableRiku3 },
    { 4, (WORLD_ID*)s_WorldTableRiku3 },
    { 4, (WORLD_ID*)s_WorldTableRiku3 },
    { 1, (WORLD_ID*)s_WorldTableRiku4 },
    { 1, (WORLD_ID*)s_WorldTableRiku5 },
    { 1, (WORLD_ID*)s_WorldTableRiku6 }
};
extern "C" char D_00484A10[];
extern "C" char D_00484900[];
extern "C" char D_00484910[];
extern "C" char D_00484920[];
extern "C" char D_00484930[];
extern "C" char D_00484940[];
extern "C" char D_00484950[];
namespace PLM {
extern BOOL g_bIsLastSceneBattle;
}
extern "C" char D_00484E38[];
extern "C" int sprintf(char*, const char*, ...);

/* Parked: 113 lines, 4 bytes long, frame 144 for 128 - the first run of stores: the original reloads m_pMCWork before each store (the *(T*)& scalar
   stores give that) but keeps two -1 registers and seven s-registers for the %hi parts; statement orders
   (incl. the lui order) and 7 min of permuter (10k candidates, 104 -> 98) did not get closer. */
#if 0
void CSceneInfo::clear() {
    *(SHORT*)&CGameWork::m_pMCWork->_nArea = -1;
    *(SHORT*)&CGameWork::m_pMCWork->_nScene = -1;
    *(SHORT*)&CGameWork::m_pMCWork->_nPrevArea = -1;
    *(SHORT*)&CGameWork::m_pMCWork->_nPrevScene = -1;
    s_nCurrentFieldScene = -1;
    s_NextAreaScene._nAreaScene = -1;
    s_nNextBattleScene = -1;
    s_nCurrentBattleScene = -1;
    s_nCurrentBattleRsrc = -1;
    *(BYTE*)&CGameWork::m_pMCWork->_nFloorIdx = 0xff;
    *(BYTE*)&CGameWork::m_pMCWork->_nRoomID = 0xff;
    *(BYTE*)&CGameWork::m_pMCWork->_nPrevRoomID = 0xff;
    *(BYTE*)&CGameWork::m_pMCWork->_nDoorID = 0xff;
    s_nNextDoorID = (DOOR_ID)2;
    s_nNextFloorIdx = 0xff;
    s_nNextRoomID = 0xff;
    s_nCurrentFloorNumber = 0;
    s_nReqChangeScene = 0;
    s_nReqEnterBattle = GAMEMODE_FIELD;
    s_nReqExitBattle = 0;
    s_nGameMode = GAMEMODE_FIELD;
    s_CurrentFloor._nRoomCount = 0;
    s_CurrentFloor.clearEventRoomTbl();
    clearSceneCache();
    s_GlobalScene.clear();
    _clearMapRsrcCache();
    s_pFloorFile_Sora = _linkFloorData(D_00484900);
    s_pFloorFile_Riku = _linkFloorData(D_00484910);
    s_pEventRoomFile_Sora = _linkEventRoomData(D_00484920);
    s_pEventRoomFile_Riku = _linkEventRoomData(D_00484930);
    s_pFloorNormaFile_Sora = _linkNormaData(D_00484940);
    s_pFloorNormaFile_Riku = _linkNormaData(D_00484950);
    CGame::dbgStartFromBattle(FALSE);
    s_roomExcept._pSrcInfo = NULL;
    s_roomExcept._pStorageInfo = NULL;
    s_roomExcept._pStorageInfo2 = NULL;
    for (UINT i = 0; i < 4; i++) {
        DoorInfo& d = s_roomExcept._doors[i];
        d._id = (DOOR_ID)i;
        d._shape = DOOR_SHAPE_NORMAL;
        d._type = _DOOR_TYPE_DEBUG;
        d._nDstRoom = 0;
        d._pStorageInfo = NULL;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", clear__10CSceneInfo);
#endif
__asm__(".rdata\n.align 3\n.word 0x6D6F6F52\n.word 0x422E3173\n.word 0x00004E49\n.word 0x00000000\n.word 0x6D6F6F52\n.word 0x422E3273\n.word 0x00004E49\n.word 0x00000000\n.word 0x6E657645\n.word 0x6F6F5274\n.word 0x2E31736D\n.word 0x004E4942\n.word 0x6E657645\n.word 0x6F6F5274\n.word 0x2E32736D\n.word 0x004E4942\n.word 0x6F4E4352\n.word 0x31616D72\n.word 0x4E49422E\n.word 0x00000000\n.word 0x6F4E4352\n.word 0x32616D72\n.word 0x4E49422E\n.word 0x00000000\n.text\n");
void CSceneInfo::initNewGame(BOOL bSora) {
    clear();
    CGameWork::setByte(0, 0x60, !bSora);
    CGameWork::setInt(0, 0x35, -666);
    CGameWork::setupGameClearInfo();
    _initEntranceSceneTbl(bSora);
    _initGoalRoomSceneTbl(bSora);
    _initAllEventDoorState(bSora);
    initFloor(0, (WORLD_ID)0);
    CJiminyMemoFlag::checkEvent(0, 0, 0);
}
void CSceneInfo::dispose() {
    s_pFloorFile_Sora = NULL;
    s_pFloorFile_Riku = NULL;
    s_pEventRoomFile_Sora = NULL;
    s_pEventRoomFile_Riku = NULL;
    s_pFloorNormaFile_Sora = NULL;
    s_pFloorNormaFile_Riku = NULL;
}
INT _getEventRoomSceneIndex(SHORT nRoomType) {
    INT nTblIdx;
    switch (nRoomType) {
    case 1:
        nTblIdx = 0;
        break;
    case 2:
        nTblIdx = 1;
        break;
    case 3:
        nTblIdx = 2;
        break;
    case 4:
        nTblIdx = 3;
        break;
    case 5:
    case 7:
        nTblIdx = 4;
        break;
    default:
        nTblIdx = -1;
        break;
    }
    return nTblIdx;
}
void _initEntranceSceneTbl(BOOL bSora) {
    for (INT i = 0; i < 16; i++) {
        FLOOR::FloorLayout* pFloorSrc = CSceneInfo::getFloorDataSrc(i, bSora);
        SHORT nScene0;
        SHORT nScene1;
        if (pFloorSrc) {
            nScene0 = pFloorSrc->_nSceneStartEnt;
            nScene1 = pFloorSrc->_nSceneGoalEnt;
        } else {
            nScene0 = 0;
            nScene1 = 0;
        }
        SHORT* pTbl = _SceneInfoWork::mc()->_nEntranceSceneTbl[i];
        pTbl[0] = nScene0;
        pTbl[1] = nScene1;
    }
}
void _setGoalRoomScene(WORLD_ID w, USHORT nScene) {
    if ((UINT)(w - 1) < 13) _SceneInfoWork::mc()->_GoalRoomSceneTbl[w - 1] = nScene;
}
USHORT _getGoalRoomScene(WORLD_ID w) {
    if ((UINT)(w - 1) >= 13) return 2;
    return _SceneInfoWork::mc()->_GoalRoomSceneTbl[w - 1];
}
void _initGoalRoomSceneTbl(BOOL bSora) {
    EventRoomData* pEventData = CSceneInfo::getEventRoomData(bSora);
    INT nTblIdx = pEventData ? _getEventRoomSceneIndex(5) : -1;
    for (INT i = 0; i < 13; i++) {
        USHORT n = 2;
        if (pEventData != NULL) n = pEventData->_nSceneTbl[i][nTblIdx];
        _setGoalRoomScene((WORLD_ID)(i + 1), n);
    }
}
void _initAllEventDoorState(BOOL bSora) {
    for (INT i = 0; i < 16; i++) {
        RCNORMA::FloorNormaSrc* pFloorNormaSrc = CSceneInfo::getFloorNormaDataSrc(i, bSora);
        if (pFloorNormaSrc) {
            for (INT j = 0; j < 4; j++) {
                RCNORMA::DoorNormaSrc& src = pFloorNormaSrc->_normas[j];
                STORAGE::_EventDoorInfo& dst = _SceneInfoWork::mc()->_nEventDoorInfoTbl[i][j];
                for (INT k = 0; k < 3; k++) dst._nNormaNums[k] = src._normas[k]._nNumber;
                dst._nClearedNorma = 0;
            }
        }
        BYTE* p = _SceneInfoWork::mc()->_nEventDoorStateTbl[i];
        p[0] = 1;
        p[1] = 1;
        p[2] = 1;
        p[3] = 1;
        p[4] = 4;
        p[5] = 4;
        p = _SceneInfoWork::mc2()->_nEventDoorOpenTbl[i];
        for (INT j = 0; j < 3; j++) *p++ = 0;
    }
}
USHORT CSceneInfo::getFloorDataCount(BOOL bSora) {
    FloorLayoutDataHeader* p = bSora ? s_pFloorFile_Sora : s_pFloorFile_Riku;
    INT n = 0;
    if (p) n = p->_nFloorCount;
    return n;
}
SHORT CSceneInfo::getFloorNumber(USHORT nFloorIndex, BOOL bSora) {
    FLOOR::FloorLayout* p = getFloorDataSrc(nFloorIndex, bSora);
    if (!p) return 0;
    return *(SHORT*)p;
}
USHORT CSceneInfo::getFloorIndex(SHORT nFloor, BOOL bSora) {
    FloorLayoutDataHeader* p = bSora ? s_pFloorFile_Sora : s_pFloorFile_Riku;
    if (p) {
        for (USHORT i = 0; i < p->_nFloorCount; i++) {
            if (*(SHORT*)((BYTE*)p->_pFloorData + p->_pFloorOfsTbl[i]) == nFloor) return i;
        }
    }
    return 0;
}
WorldSelTable* CSceneInfo::getSelectableWorldTable(INT nFloorIndex, BOOL bSora) {
    FloorLayoutDataHeader* p = bSora ? s_pFloorFile_Sora : s_pFloorFile_Riku;
    if (p && nFloorIndex >= 0 && nFloorIndex < p->_nFloorCount) {
        WorldSelTable* pTbl = (WorldSelTable*)(bSora ? s_pWorldTableSora : s_pWorldTableRiku);
        return &pTbl[nFloorIndex];
    }
    return NULL;
}
EventRoomData* CSceneInfo::getEventRoomData(BOOL bSora) { return bSora ? s_pEventRoomFile_Sora : s_pEventRoomFile_Riku; }
SHORT CSceneInfo::getEventRoomSceneNum(WORLD_ID world, const FLOOR::RoomSrcInfo& info, BOOL bSora) {
    EventRoomData* pEventData = getEventRoomData(bSora);
    INT nTblIdx = -1;
    if (pEventData) nTblIdx = _getEventRoomSceneIndex(info._nRoomType);
    return nTblIdx >= 0 ? pEventData->_nSceneTbl[world - 1][nTblIdx] : 0;
}
void CSceneInfo::showAllHiddenDoorsOfCurrentFloor() {
    for (INT i = 0; i < s_CurrentFloor._nRoomCount; i++) {
        RoomInfo& room = s_CurrentFloor._rooms[i];
        for (INT j = 0; j < 4; j++) {
            DoorInfo& door = room._doors[j];
            if (door._type == DOOR_TYPE_OPEN_AFTER_EVENT) {
                door._type = DOOR_TYPE_NORMAL;
                if (door.getDoorState() == DOOR_STAT_UNAVAILABLE) {
                    door.setDoorState(DOOR_STAT_CLOSED);
                    DOOR_STORAGE* pInfo = (DOOR_STORAGE*)door._pStorageInfo;
                    if (pInfo) {
                        BYTE n = room.getCardNumber() + 1;
                        pInfo->_nNormaNums[0] = n < 10 ? n : 0;
                    }
                }
            }
        }
    }
    BYTE* p = _SceneInfoWork::mc()->_nEventDoorStateTbl[getCurrentFloorIndex()];
    p[5] = 1;
}
void CSceneInfo::initFloor(INT nFloorIdx, WORLD_ID world) {
    BOOL bSora = CGame::isSoraGame();
    INT nIdx = nFloorIdx;
    if (world == 10) nIdx = getFloorIndex(100, TRUE);
    FLOOR::FloorLayout* pFloorSrc = getFloorDataSrc(nIdx, bSora);
    RCNORMA::FloorNormaSrc* pFloorNormaSrc = getFloorNormaDataSrc(nIdx, bSora);
    if (!(pFloorSrc && pFloorNormaSrc)) AssertApp(__FILE__, 586, "pFloorSrc && pFloorNormaSrc", 0);
    _SceneInfoWork::mc()->_nFloorIdx = nFloorIdx;
    if (!(nIdx < 16)) AssertApp(__FILE__, 590, "nIdx < FLOOR_COUNT_MAX", 0);
    s_nCurrentFloorNumber = getFloorNumber(nFloorIdx, bSora);
    if (CGameWork::getInt(0, 0x35) < s_nCurrentFloorNumber) CGameWork::setInt(0, 0x35, s_nCurrentFloorNumber);
    FloorInfo& floor = s_CurrentFloor;
    STORAGE::MCWork2* p = _SceneInfoWork::mc2();
    BYTE* pTbl = p->_nWorldOnFloor[!bSora];
    pTbl[nFloorIdx] = world;
    floor._nRoomCount = pFloorSrc->_nRoomCount;
    floor.clearEventRoomTbl();
    EventRoomData* pEventData = bSora ? s_pEventRoomFile_Sora : s_pEventRoomFile_Riku;
    for (INT i = 0; i < floor._nRoomCount; i++) {
        FLOOR::RoomSrcInfo& roomSrc = pFloorSrc->_rooms[i];
        RoomInfo& room = floor._rooms[i];
        _initRoomInfo(room, _SceneInfoWork::mc()->_roomInfo[i], _SceneInfoWork::mc2()->_roomInfo2[i], &roomSrc, pFloorSrc, pFloorNormaSrc, FALSE);
        INT nTblIdx = _getEventRoomSceneIndex(roomSrc._nRoomType);
        SHORT nScene = roomSrc.isFixSceneRoom() ? roomSrc._nFixScene : 0;
        if (nTblIdx >= 0) {
            floor._nEventRoomIdxTbl[nTblIdx] = i;
            if (world) {
                INT nEvTblIdx = world - 1;
                if (roomSrc._nRoomType == 5 || roomSrc._nRoomType == 7) {
                    nScene = _getGoalRoomScene(world);
                } else {
                    nScene = pEventData ? pEventData->_nSceneTbl[nEvTblIdx][nTblIdx] : 0;
                }
                if (!bSora && world == 12 && nTblIdx == 1) {
                    if (CGameWork::getInt(0, 0x2a) & 0x1000000) nScene = 30;
                }
            }
        }
        if (nScene > 0) room.setScene(world, nScene);
    }
    rollbackEventDoorState(1);
    SHORT* pEntSceneTbl = _SceneInfoWork::mc()->_nEntranceSceneTbl[nFloorIdx];
    _setWorldEntrance(world, pEntSceneTbl[0], pEntSceneTbl[1], FALSE);
    if (bSora && world == 13 && *_SceneInfoWork::mc()->_nEventDoorStateTbl[nFloorIdx] == 4) showAllHiddenDoorsOfCurrentFloor();
    _SceneInfoWork::mc()->_nNextRCNormaNum = 0;
    if (world) CCard::setCardPossibleWorld(world);
}
void CSceneInfo::initFloor() {
    BOOL bSora = CGame::isSoraGame();
    INT nFloorIdx = _SceneInfoWork::mc()->_nFloorIdx;
    INT nIdx = nFloorIdx;
    WORLD_ID world = getWorldID(nFloorIdx);
    if (world == 10) nIdx = getFloorIndex(100, TRUE);
    FLOOR::FloorLayout* pFloorSrc = getFloorDataSrc(nIdx, bSora);
    RCNORMA::FloorNormaSrc* pFloorNormaSrc = getFloorNormaDataSrc(nIdx, bSora);
    if (!(pFloorSrc && pFloorNormaSrc)) AssertApp(__FILE__, 691, "pFloorSrc && pFloorNormaSrc", 0);
    *(SHORT*)&s_nCurrentFloorNumber = getFloorNumber(nFloorIdx, bSora); // MATCH: not a fixed scalar store: the pFloorSrc loads stay after it
    FloorInfo& floor = s_CurrentFloor;
    floor._nRoomCount = pFloorSrc->_nRoomCount;
    floor.clearEventRoomTbl();
    for (INT i = 0; i < floor._nRoomCount; i++) {
        FLOOR::RoomSrcInfo* pSrc = &pFloorSrc->_rooms[i];
        _initRoomInfo(floor._rooms[i], _SceneInfoWork::mc()->_roomInfo[i], _SceneInfoWork::mc2()->_roomInfo2[i], pSrc, pFloorSrc, pFloorNormaSrc, TRUE);
        INT nTblIdx = _getEventRoomSceneIndex(pSrc->_nRoomType);
        if (nTblIdx >= 0) floor._nEventRoomIdxTbl[nTblIdx] = i;
    }
    rollbackEventDoorState(0);
    SHORT* pEntSceneTbl = _SceneInfoWork::mc()->_nEntranceSceneTbl[nFloorIdx];
    _setWorldEntrance(world, pEntSceneTbl[0], pEntSceneTbl[1], TRUE);
    if (bSora && world == 13 && *_SceneInfoWork::mc()->_nEventDoorStateTbl[nFloorIdx] == 4) showAllHiddenDoorsOfCurrentFloor();
}
void _initDoor(DoorInfo& door, const FLOOR::DoorSrcInfo* pSrcInfo, DOOR_ID id, const FLOOR::FloorLayout* pFloorLayout, const RCNORMA::FloorNormaSrc* pFloorNormaSrc, STORAGE::MCWork2::_DoorInfo& st_info, const RoomInfo& room, BOOL bContinue) {
    BYTE* pDoorState = bContinue ? NULL : &st_info._nStat;
    door._id = id;
    door._pStorageInfo = &st_info;
    if (!pSrcInfo) {
        door._type = DOOR_TYPE_NORMAL;
        door._nDstRoom = -1;
        if (pDoorState) *pDoorState = DOOR_STAT_NOTHING;
        return;
    }
    door._type = (DOOR_TYPE)pSrcInfo->_nDoorType;
    if (door._type == DOOR_TYPE_NORMAL && room._pSrcInfo && room._pSrcInfo->_nRoomType < FLOOR::ROOM_TYPE_GOAL &&
        room._pSrcInfo->_nRoomType >= FLOOR::ROOM_TYPE_EVENT1)
        door._type = DOOR_TYPE_EVENTROOM;
    door._nDstRoom = pSrcInfo->_nDstRoom;
    if (door._nDstRoom <= 0 || (pFloorLayout && pFloorLayout->_nRoomCount < door._nDstRoom)) {
        door._nDstRoom = -1;
        if (pDoorState) *pDoorState = pSrcInfo->_nDoorType == DOOR_TYPE_ONEWAY ? DOOR_STAT_ONE_WAY : DOOR_STAT_NOTHING;
        door._pNormaSrcInfo = NULL;
        door._shape = DOOR_SHAPE_NORMAL;
        return;
    }
    DOOR_STAT stat;
    RCNORMA_DOOR nd = _RCNORMA_DOOR_MAX;
    if (pFloorLayout && pFloorLayout->_nID == 100) {
        stat = DOOR_STAT_OPENED;
    } else if (room._pSrcInfo && room._pSrcInfo->_nRoomType == FLOOR::ROOM_TYPE_LAST) {
        stat = DOOR_STAT_OPENED;
    } else {
        stat = pSrcInfo->_nDoorType == DOOR_TYPE_ONEWAY ? DOOR_STAT_ONE_WAY : DOOR_STAT_CLOSED;
        switch (pSrcInfo->_nDoorType) {
        case DOOR_TYPE_TO_EVENT1:
            nd = RCNORMA_DOOR_EVENT1;
            break;
        case DOOR_TYPE_TO_EVENT2:
            nd = RCNORMA_DOOR_EVENT2;
            break;
        case DOOR_TYPE_TO_EVENT3:
            nd = RCNORMA_DOOR_EVENT3;
            break;
        case DOOR_TYPE_TO_EXTRA:
            nd = RCNORMA_DOOR_EXTRA;
            break;
        case DOOR_TYPE_OPEN_AFTER_EVENT:
            stat = DOOR_STAT_UNAVAILABLE;
            break;
        case DOOR_TYPE_TO_GOAL: /* MATCH: separate copy (cross-jumped); its extra stat set wins stat the register priority over nd */
            stat = DOOR_STAT_UNAVAILABLE;
            break;
        }
    }
    if (pDoorState) *pDoorState = stat;
    door._pNormaSrcInfo = (pFloorNormaSrc && nd != _RCNORMA_DOOR_MAX) ? (RCNORMA::DoorNormaSrc*)&pFloorNormaSrc->_normas[nd] : NULL;
    if (!bContinue) door.initNorma();
    door._shape = door.isSpecialDoor() ? DOOR_SHAPE_EVENT : DOOR_SHAPE_NORMAL;
}
void _initDoor(DoorInfo& door, const FLOOR::DoorSrcInfo* pSrcInfo, DOOR_ID id, const FLOOR::FloorLayout* pFloorLayout, const RCNORMA::FloorNormaSrc* pFloorNormaSrc, STORAGE::MCWork2::_DoorInfo& st_info, const RoomInfo& room, BOOL bContinue);
void _initRoomInfo(RoomInfo& roomInfo, STORAGE::MCWork::_RoomInfo& st_info, STORAGE::MCWork2::_RoomInfo2& st_info2, const FLOOR::RoomSrcInfo* pSrcInfo, const FLOOR::FloorLayout* pFloorLayout, const RCNORMA::FloorNormaSrc* pFloorNormaSrc, BOOL bContinue) {
    roomInfo._pStorageInfo = &st_info;
    roomInfo._pStorageInfo2 = &st_info2;
    roomInfo._pSrcInfo = (FLOOR::RoomSrcInfo*)pSrcInfo;
    if (!bContinue) {
        st_info2._nCreatedCardNum = 0;
        st_info2._nCreatedCardID = 0;
        ROOM_STORAGE& st = (ROOM_STORAGE&)st_info;
        st._bFirstTimeAfterRC = 1;
        st._bEntered = 0;
        roomInfo.resetScene();
    }
    for (INT i = 0; i < 4; i++) {
        _initDoor(roomInfo._doors[i], pSrcInfo ? &pSrcInfo->_doors[i] : NULL, (DOOR_ID)i, pFloorLayout, pFloorNormaSrc, st_info2._doors[i], roomInfo, bContinue);
    }
}
DOOR_ID _getDstDoorID(DOOR_ID door) {
    switch (door) {
    case 1: return (DOOR_ID)3;
    case 2: return (DOOR_ID)0;
    case 3: return (DOOR_ID)1;
    }
    return (DOOR_ID)2;
}
void _setWorldEntrance(WORLD_ID world, SHORT nStartEntScene, SHORT nGoalEntScene, BOOL bContinue) {
    FloorInfo& floor = s_CurrentFloor;
    BOOL bWorldOK = world != 0;
    _initRoomInfo(floor._roomStartEnt, _SceneInfoWork::mc()->_roomInfoStartEnt, _SceneInfoWork::mc2()->_roomInfo2StartEnt, NULL, NULL, NULL, bContinue);
    floor._roomStartEnt.setScene(0, nStartEntScene);
    _SceneInfoWork::mc2()->_roomInfo2StartEnt._nCreatedCardID = 0xdc;
    INT nIdx = floor._nEventRoomIdxTbl[0];
    DOOR_ID nDID;
    RoomInfo* pRoom = NULL;
    if (nIdx >= 0) {
        pRoom = &floor._rooms[nIdx];
        nIdx = -1;
        for (INT i = 0; i < 4; i++) {
            if (pRoom->_doors[i].getDoorState() != DOOR_STAT_NOTHING) {
                nIdx = i;
                break;
            }
        }
        if (!(nIdx >= 0)) AssertApp(__FILE__, 923, "0 <= nIdx", 0);
        pRoom = CSceneInfo::getRoomInfo(pRoom->_doors[nIdx]._nDstRoom);
        if (!(pRoom)) AssertApp(__FILE__, 925, "pRoom", 0);
        nDID = _getDstDoorID((DOOR_ID)nIdx);
    } else {
        RoomInfo* p = floor._rooms;
        for (INT i = 0; i < floor._nRoomCount; i++, p++) {
            if (p->getSceneNum() == 1) {
                pRoom = p;
                break;
            }
        }
        if (pRoom) nDID = (DOOR_ID)2;
    }
    if (pRoom) {
        DoorInfo& d = pRoom->_doors[nDID];
        d.setDoorState(DOOR_STAT_OPENED);
        d._nDstRoom = 0;
        d._shape = DOOR_SHAPE_WORLD_ENTRANCE;
        d._type = DOOR_TYPE_WORLD_ENTRANCE;
        DoorInfo& s = floor._roomStartEnt._doors[0];
        s.setDoorState(bWorldOK ? DOOR_STAT_OPENED : DOOR_STAT_CLOSED);
        s._shape = DOOR_SHAPE_WORLD_ENTRANCE;
        s._type = DOOR_TYPE_SELECT_WORLD;
        s._nDstRoom = pRoom->_pSrcInfo->_nID;
    }
    if (CSceneInfo::getCurrentFloorIndex()) {
        DoorInfo& d = floor._roomStartEnt._doors[2];
        d.setDoorState(DOOR_STAT_OPENED);
        d._type = DOOR_TYPE_FLOOR_CHANGE;
        d._shape = DOOR_SHAPE_NORMAL;
        d._nDstRoom = 99;
    }
    _initRoomInfo(floor._roomGoalEnt, _SceneInfoWork::mc()->_roomInfoGoalEnt, _SceneInfoWork::mc2()->_roomInfo2GoalEnt, NULL, NULL, NULL, bContinue);
    floor._roomGoalEnt.setScene(0, nGoalEntScene);
    _SceneInfoWork::mc2()->_roomInfo2GoalEnt._nCreatedCardID = 0xdc;
    nIdx = floor._nEventRoomIdxTbl[4];
    if (nIdx < 0) {
        INT nMaxID = floor._nRoomCount;
        RoomInfo* p = floor._rooms;
        for (INT i = 0; i < floor._nRoomCount; i++, p++) {
            if (nMaxID == p->_pSrcInfo->_nID) {
                nIdx = i;
                break;
            }
        }
    }
    pRoom = nIdx >= 0 ? &floor._rooms[nIdx] : NULL;
    if (pRoom) {
        DoorInfo& d = floor._rooms[nIdx]._doors[0];
        d.setDoorState(DOOR_STAT_OPENED);
        d._shape = DOOR_SHAPE_WORLD_EXIT;
        d._type = DOOR_TYPE_WORLD_ENTRANCE;
        d._nDstRoom = 99;
        DoorInfo& g = floor._roomGoalEnt._doors[2];
        g.setDoorState(bWorldOK ? DOOR_STAT_OPENED : DOOR_STAT_CLOSED);
        g._type = DOOR_TYPE_WORLD_ENTRANCE;
        g._shape = DOOR_SHAPE_WORLD_EXIT_CA;
        g._nDstRoom = pRoom->_pSrcInfo->_nID;
        if (pRoom->_pSrcInfo->_nRoomType == 7) {
            for (INT i = 0; i < 4; i++) {
                if (i == 0) continue;
                DoorInfo& dd = pRoom->_doors[i];
                if (dd.getDoorState() != DOOR_STAT_OPENED) continue;
                RoomInfo* pDst = CSceneInfo::getRoomInfo(dd._nDstRoom);
                DoorInfo& x = pDst->_doors[_getDstDoorID((DOOR_ID)i)];
                x._type = DOOR_TYPE_TO_LAST;
                x.setDoorState(DOOR_STAT_OPENED);
            }
        }
        SHORT nFloor = CSceneInfo::getCurrentFloorNumber();
        if (nFloor != 13 && nFloor != -1) {
            DoorInfo& f = floor._roomGoalEnt._doors[0];
            f.setDoorState(DOOR_STAT_OPENED);
            f._nDstRoom = 0;
            f._shape = DOOR_SHAPE_NORMAL;
            f._type = DOOR_TYPE_FLOOR_CHANGE;
        }
    }
}
void CSceneInfo::makeRoom(const RoomInfo& room, DOOR_ID nDoorID, SHORT nScene, BYTE nCardID, BYTE nCardNum) {
    RoomInfo& r = (RoomInfo&)room;
    ROOM_STORAGE* p = (ROOM_STORAGE*)r._pStorageInfo;
    if (p) p->_bFirstTimeAfterRC = 1;
    r.setScene(getCurrentWorldID(FALSE), nScene);
    r.setCardInfo(nCardID, nCardNum);
    nCardNum++;
    if (nCardNum >= 10) nCardNum = 0;
    CGameWork::m_pMCWork->_nNextRCNormaNum = nCardNum;
    if (!(nScene >= 2000 && nScene < 3000)) {
        DoorInfo& door = r._doors[nDoorID];
        if (door.getDoorState() == DOOR_STAT_CLOSED) door.setDoorState(DOOR_STAT_OPENED);
    }
    for (INT i = 0; i < 4; i++) {
        DoorInfo& d = r._doors[i];
        if (getRoomInfo(d._nDstRoom) && d.canCreateRoom() && !d.getNormaKeyCardID()) {
            DOOR_STORAGE* pInfo = (DOOR_STORAGE*)d._pStorageInfo;
            if (pInfo) pInfo->_nNormaNums[0] = nCardNum;
        }
    }
    if ((USHORT)nScene == 4 && r._pStorageInfo) CMoogleShopMain::initMcWork((STORAGE::MCWork::_RoomInfo*)r._pStorageInfo);
}
void CSceneInfo::makeRoom(DoorInfo& door, BOOL bFromEventRoom) {
    if (door.getDoorState() == DOOR_STAT_CLOSED) door.setDoorState(bFromEventRoom ? DOOR_STAT_UNAVAILABLE : DOOR_STAT_OPENED);
    RoomInfo* pDstRoom = getRoomInfo(door._nDstRoom);
    if (!(pDstRoom)) AssertApp(__FILE__, 1094, "pDstRoom", 0);
    INT nCardNumber;
    SHORT nScene;
    BYTE nCardID;
    if ((!door.isNormaCardUsed() && pDstRoom->isFixSceneRoom()) ||
        (pDstRoom->_pSrcInfo->_nRoomType == FLOOR::ROOM_TYPE_EXTRA && !pDstRoom->hasEntered())) {
        nScene = pDstRoom->getSceneNum();
        nCardID = getCardIDFromSceneNum(nScene);
        nCardNumber = CGameWork::m_pMCWork->_nNextRCNormaNum;
        if (pDstRoom->_pSrcInfo->_nRoomType == FLOOR::ROOM_TYPE_EXTRA) {
            nCardNumber = door.getFirstUsedCardNumForNorma();
            CGameWork::m_pMCWork->_nNextRCNormaNum = nCardNumber + 1 < 10 ? nCardNumber + 1 : 0;
        }
    } else {
        nCardNumber = door.getFirstUsedCardNumForNorma();
        if ((UINT)nCardNumber >= 10) nCardNumber = CGameWork::m_pMCWork->_nNextRCNormaNum;
        nCardID = door.getFirstUsedCardIDForNorma();
        if (door.isEventDoor()) {
            for (INT i = 0; i < 4; i++) {
                DoorInfo& d = pDstRoom->_doors[i];
                if (d.getDoorState() == DOOR_STAT_CLOSED) d.setFirstUsedCardInfo(nCardID, nCardNumber);
            }
            return;
        }
        nScene = getSceneNumFromCardID(nCardID);
        if (nScene == 0) {
            static const char s_RndSceneTblSora[23] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 25, 26 };
            static const char s_RndSceneTblRiku[15] = { 3, 6, 9, 11, 12, 13, 14, 16, 18, 19, 21, 22, 23, 25, 26 };
            const char* pTbl;
            INT nNum;
            if (CGame::isSoraGame()) {
                pTbl = s_RndSceneTblSora;
                nNum = 23;
                nScene = pTbl[rand() % nNum];
            } else {
                nNum = 15;
                INT w = getCurrentWorldID(0);
                pTbl = s_RndSceneTblRiku;
                do {
                    nScene = pTbl[rand() % nNum];
                } while (w == 9 && nScene == 6);
            }
            TRACE_COL(_TTY_RED, "*** Create Room At Random ... [Scene:%d]\n", nScene);
            if (nCardID == 0xb7 || nCardID == 0xb9) nCardID = getCardIDFromSceneNum(nScene);
        }
        if (bFromEventRoom) nCardID = 0xd8;
    }
    DOOR_ID dstDoorID = _getDstDoorID(door._id);
    if (bFromEventRoom) {
        for (INT i = 0; i < 4; i++) {
            if (dstDoorID == i) continue;
            DoorInfo& d = pDstRoom->_doors[i];
            DOOR_STAT st = d.getDoorState();
            if (st != DOOR_STAT_CLOSED) continue;
            if (d.isSpecialDoor()) continue;
            if (d.isWorldEntrance()) continue;
            RoomInfo* pRoom = getRoomInfo(d._nDstRoom);
            if (!pRoom) continue;
            if (pRoom->isExceptRoom()) continue;
            if (!pRoom->isCreated()) continue;
            d.setDoorState(DOOR_STAT_OPENED);
            DoorInfo& d2 = pRoom->_doors[_getDstDoorID((DOOR_ID)i)];
            if (d2.getDoorState() != st) continue;
            d2.setDoorState(DOOR_STAT_OPENED);
        }
    }
    makeRoom(*pDstRoom, dstDoorID, nScene, nCardID, nCardNumber);
}
BOOL CSceneInfo::hasAnyRequest() {
    BOOL b = FALSE;
    if (hasReqChangeScene() || s_nReqEnterBattle || s_nReqExitBattle) b = TRUE;
    return b;
}
void CSceneInfo::setNextAreaScene(SHORT nArea, SHORT nField, SHORT nScene) {
    s_NextAreaScene._nScene = nField;
    s_NextAreaScene._nArea = nArea;
    setNextBattleScene(nScene);
}
void CSceneInfo::setNextScene(SHORT nScene, SHORT nBtlScene) {
    SHORT nArea = getCurrentAreaNum();
    s_NextAreaScene._nScene = nScene;
    s_NextAreaScene._nArea = nArea;
    setNextBattleScene(nBtlScene);
}
void CSceneInfo::setNextBattleScene(SHORT nScene) {
    if (nScene <= 0) {
        if (s_nNextBattleScene > 0) return;
        nScene = CRcf::getBattleScene(s_NextAreaScene._nArea, s_NextAreaScene._nScene);
    }
    s_nNextBattleScene = nScene;
}
BOOL _checkAvailableDoorID(const DoorInfo& door);
BOOL _checkAvailableDoorID(const DoorInfo& d) {
    switch (d.getDoorState()) {
    case DOOR_STAT_OPENED:
    case DOOR_STAT_ONE_WAY:
    case DOOR_STAT_UNAVAILABLE:
        return TRUE;
    case DOOR_STAT_CLOSED:
        if (!d.isSpecialDoor() && d._type != DOOR_TYPE_OPEN_AFTER_EVENT && d._type != DOOR_TYPE_TO_GOAL) return TRUE;
        break;
    }
    return FALSE;
}
DOOR_ID _checkAvailableDoorID(INT nRoomID, DOOR_ID nDoor) {
    if (nDoor >= 4) return (DOOR_ID)4;
    RoomInfo* pRoom = CSceneInfo::getRoomInfo(nRoomID);
    if (!pRoom) return (DOOR_ID)4;
    SHORT nScene = pRoom->getSceneNum();
    if (nScene >= 2000 && nScene < 3000) return nDoor;
    INT id = nRoomID == 0 || nRoomID == 99;
    if (id) return nDoor;
    if (_checkAvailableDoorID(pRoom->_doors[nDoor])) return nDoor;
    for (INT i = 3; i >= 0; i--) {
        if (i != nDoor && _checkAvailableDoorID(pRoom->_doors[i])) return (DOOR_ID)i;
    }
    return (DOOR_ID)4;
}
RoomInfo* CSceneInfo::setEventRoomSceneNum(UINT nEventRoomType, SHORT nScene) {
    FloorInfo& floor = s_CurrentFloor;
    USHORT nFloorIdx = getCurrentFloorIndex();
    RoomInfo* pRoom = NULL;
    INT nEntrance = -1;
    if ((INT)nEventRoomType < __EVENTROOM_MAX) {
        INT nIdx = floor.getEventRoomIdx((EVENTROOM_TBLIDX)nEventRoomType);
        if (nIdx >= 0) pRoom = &floor._rooms[nIdx];
        if (nEventRoomType == _EVENTROOM_GOAL) _setGoalRoomScene((WORLD_ID)getCurrentWorldID(FALSE), nScene);
    } else {
        switch (nEventRoomType) {
        case _EVENTROOM_START_ENTRANCE:
            pRoom = &floor._roomStartEnt;
            nEntrance = 0;
            break;
        case _EVENTROOM_GOAL_ENTRANCE:
            pRoom = &floor._roomGoalEnt;
            nEntrance = 1;
            break;
        case _EVENTROOM_LOWER_START_ENTRANCE:
            if (nFloorIdx) {
                nEntrance = 0;
                nFloorIdx--;
            }
            break;
        case _EVENTROOM_LOWER_GOAL_ENTRANCE:
            if (nFloorIdx) {
                nEntrance = 1;
                nFloorIdx--;
            }
            break;
        case _EVENTROOM_UPPER_START_ENTRANCE:
            if (nFloorIdx < 15) {
                nEntrance = 0;
                nFloorIdx++;
            }
            /* MATCH: no break in the original (falls into the upper goal entrance) */
        case _EVENTROOM_UPPER_GOAL_ENTRANCE:
            if (nFloorIdx < 15) {
                nEntrance = 1;
                nFloorIdx++;
            }
            break;
        }
        if (nEntrance >= 0) {
            SHORT* pEntSceneTbl = _SceneInfoWork::mc()->_nEntranceSceneTbl[nFloorIdx];
            pEntSceneTbl[nEntrance] = nScene;
        }
    }
    if (pRoom) {
        pRoom->setScene(nScene);
        if (getCurrentFloorIndex() == nFloorIdx) {
            INT nRoomID;
            if (nEntrance < 0) nRoomID = pRoom->_pSrcInfo->_nID;
            else nRoomID = nEntrance == 0 ? 0 : 99;
            if (getCurrentRoomID() == nRoomID && getCurrentSceneNum() == getCurrentBattleSceneNum()) s_nCurrentFieldScene = nScene;
        }
    }
    return pRoom;
}
BOOL CSceneInfo::reqChangeFloor(SHORT nFloor, BOOL bWarp) {
    if (hasAnyRequest()) return FALSE;
    BOOL bSora = CGame::isSoraGame();
    USHORT nCurrentFloorIdx = getCurrentFloorIndex();
    USHORT nNextFloorIdx = getFloorIndex(nFloor, bSora);
    if (nCurrentFloorIdx == nNextFloorIdx) return FALSE;
    SHORT* pEntSceneTbl = _SceneInfoWork::mc()->_nEntranceSceneTbl[nNextFloorIdx];
    INT nRoomID;
    DOOR_ID nDoor;
    SHORT nScene;
    if (bWarp || nCurrentFloorIdx < nNextFloorIdx) {
        nScene = pEntSceneTbl[0];
        nRoomID = 0;
        nDoor = (DOOR_ID)2;
    } else {
        nScene = pEntSceneTbl[1];
        nRoomID = 99;
        nDoor = (DOOR_ID)0;
    }
    BOOL bResult = reqChangeScene(0, nScene, -1, FALSE, 1);
    if (bResult) {
        s_nNextRoomID = nRoomID;
        s_nNextDoorID = nDoor;
        s_nNextFloorIdx = nNextFloorIdx;
    }
    return bResult;
}
BOOL CSceneInfo::reqEnterWorld(WORLD_ID world) {
    if (!((0 < world) && (14 > world))) AssertApp(__FILE__, 1410, "(WORLD_ID_COMMON < world) && (_WORLD_ID_MAX > world)", 0);
    initFloor(getCurrentFloorIndex(), world);
    reqChangeRoom(1, (DOOR_ID)2, -1);
    return TRUE;
}
BOOL CSceneInfo::reqChangeRoom(INT nRoomID, DOOR_ID nDoor, SHORT nScene) {
    if (hasAnyRequest()) return FALSE;
    SHORT nArea = getCurrentWorldID(FALSE);
    INT id = nRoomID == 0 || nRoomID == 99;
    if (id) nArea = 0;
    nDoor = _checkAvailableDoorID(nRoomID, nDoor);
    if (nScene <= 0) {
        RoomInfo* pRoom = getRoomInfo(nRoomID);
        if (!pRoom || !pRoom->isCreated()) {
            TRACE_COL(_TTY_RED, "CSceneInfo::reqChangeRoom( %d ) ...", nRoomID);
            TRACE_COL(_TTY_RED, (char*)(!pRoom ? "\x82\xbb\x82\xf1\x82\xc8\x83\x8b\x81[\x83\x80\x82\xcd\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\n" : "\x82\xbb\x82\xcc\x83\x8b\x81[\x83\x80\x82\xc9\x82\xcd\x83V\x81[\x83\x93\x81\x94\x82\xaa\x90\xdd\x92\xe8\x82\xb3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb9\x82\xf1\n"));
            return FALSE;
        }
        nScene = pRoom->getSceneNum();
    } else {
        RoomInfo* pRoom = getRoomInfo(nRoomID);
        if (!pRoom) return FALSE;
        if (!pRoom->_pSrcInfo) {
            nArea = 0;
        } else if (!pRoom->isCreated() || pRoom->isFixSceneRoom()) {
            BYTE nCardID = getCardIDFromSceneNum(nScene);
            if (!nCardID && !pRoom->isFixSceneRoom()) nCardID = 0xb7;
            makeRoom(*pRoom, nDoor, nScene, nCardID, 0);
        } else {
            pRoom->setScene(nScene);
        }
    }
    BOOL bResult = reqChangeScene(nArea, nScene, -1, FALSE, 1);
    if (bResult) {
        s_nNextRoomID = nRoomID;
        s_nNextDoorID = nDoor;
        if (isBossBattleScene(-1)) s_nReqExitBattle = 1;
    }
    return bResult;
}
BOOL CSceneInfo::reqChangeEventRoom(UINT nEventRoomType, DOOR_ID nDoor, SHORT nScene) {
    if (hasAnyRequest()) return FALSE;
    if (s_nGameMode != GAMEMODE_FIELD && !((UINT)(s_nCurrentFieldScene - 2000) < 1000)) {
        SHORT nCur = getCurrentSceneNum();
        if (!(nCur >= 0x41a && nCur <= 0x4ae)) {
            TRACE_COL(_TTY_RED, "\x83" "C\x83x\x83\x93\x83g\x83\x8b\x81[\x83\x80\x95\xcf\x8dX\x82\xaa\x8ds\x82\xed\x82\xea\x82\xe6\x82\xa4\x82\xc6\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x82\xaa\x81" "A\x83" "C\x83x\x83\x93\x83g\x83V\x81[\x83\x93\x88\xc8\x8aO\x82\xa9\x82\xe7\x82\xcd\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\n");
            return FALSE;
        }
    }
    RoomInfo* pRoom;
    if (nScene > 0) pRoom = setEventRoomSceneNum(nEventRoomType, nScene);
    else pRoom = getEventRoomInfo(nEventRoomType);
    if (!pRoom || pRoom->isExceptRoom()) {
        TRACE_COL(_TTY_RED, "\x81u\x83\x8b\x81[\x83\x80" "de\x83Z\x83\x8c\x83N\x83g\x81v\x88\xc8\x8aO\x82\xc5\x93\xfc\x82\xc1\x82\xc4\x82\xe9\x82\xcc\x82\xc5\x81" "A\x83\x8b\x81[\x83\x80\x83`\x83" "F\x83\x93\x83W\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\n");
        if (s_nGameMode == GAMEMODE_FIELD) reqChangeScene(getCurrentAreaNum(), getCurrentSceneNum(), -1, FALSE, 1);
        else reqExitBattle();
        return FALSE;
    }
    if ((UINT)nDoor >= 4) nDoor = getEnteredDoorID();
    if (nEventRoomType == 4) nDoor = _openGoalDoor(*pRoom);
    INT nRoomID;
    SHORT nArea = 0;
    switch (nEventRoomType) {
    case 5:
        nRoomID = 0;
        break;
    case 6:
        nRoomID = 99;
        break;
    default:
        nRoomID = pRoom->_pSrcInfo->_nID;
        nArea = getCurrentWorldID(FALSE);
        break;
    }
    nDoor = _checkAvailableDoorID(nRoomID, nDoor);
    if (nScene > 0 && !(nScene >= 2000 && nScene < 3000)) {
        INT id = nRoomID == 0 || nRoomID == 99;
        if (!id) makeRoom(*pRoom, nDoor, nScene, getCardIDFromSceneNum(nScene), CGameWork::m_pMCWork->_nNextRCNormaNum);
    }
    BOOL bResult = reqChangeScene(nArea, pRoom->getSceneNum(), -1, FALSE, 1);
    if (bResult) {
        s_nNextRoomID = nRoomID;
        s_nNextDoorID = nDoor;
        if (s_nGameMode != GAMEMODE_FIELD) s_nReqExitBattle = 1;
    }
    return bResult;
}
BOOL CSceneInfo::reqChangeGoalRoom() { return reqChangeEventRoom(4, (DOOR_ID)4, -1); }
BOOL CSceneInfo::reqChangeBackupRoom() {
    if (hasAnyRequest()) return FALSE;
    CGameWork::backup(CGameWork::BACKUP_SECONDARY);
    CGameWork::rollback(CGameWork::BACKUP_PRIMARY);
    USHORT nFloorIdx = getCurrentFloorIndex();
    INT nRoomID = getCurrentRoomID();
    DOOR_ID nDoorID = getEnteredDoorID();
    SHORT nArea = getCurrentAreaNum();
    SHORT nScene = getCurrentSceneNum();
    CGameWork::rollback(CGameWork::BACKUP_SECONDARY);
    if (nArea < 0 || nScene <= 0) {
        TRACE_COL(_TTY_RED, "\x83\x8d\x81[\x83\x8b\x83o\x83" "b\x83N\x90\xe6\x82\xcc AREA or SCENE \x82\xaa\x95s\x90\xb3\x82\xc5\x82\xb7\x81i%d, %d\x81j\n", nArea, nScene);
        return FALSE;
    }
    BOOL bResult = reqChangeScene(nArea, nScene, -1, FALSE, 1);
    if (bResult) {
        s_nNextRoomID = nRoomID;
        s_nNextDoorID = nDoorID;
        s_nReqChangeScene |= 2;
        if (getCurrentFloorIndex() != nFloorIdx) s_nNextFloorIdx = nFloorIdx;
        if (s_nGameMode != GAMEMODE_FIELD) s_nReqExitBattle = 1;
    }
    return bResult;
}
BOOL CSceneInfo::reqEnterDoor(DOOR_ID door) {
    if (hasAnyRequest()) return FALSE;
    DoorInfo* pDoor = getDoorInfo(door, -1);
    if (!(pDoor)) AssertApp(__FILE__, 1632, "pDoor", 0);
    if (pDoor->_type == DOOR_TYPE_SELECT_WORLD) {
        WORLD_ID w = (WORLD_ID)getCurrentWorldID(FALSE);
        if ((UINT)(w - 1) < 13) {
            INT nStartFlag = CGame::isSoraGame() ? 1 << (w - 1) : 1 << (w + 15);
            if (!(CGameWork::getInt(0, 0xc1) & nStartFlag)) return reqEnterWorld(w);
        }
    }
    BOOL bResult = FALSE;
    DOOR_STAT stat = pDoor->getDoorState();
    if (stat == DOOR_STAT_CLOSED || stat == DOOR_STAT_OPENED) {
        TRACE(" >>> Try Enter The Door [%d], type = [%d], stat = [%d], dstRoom = [%d]\n", pDoor->_id, pDoor->_type, stat, pDoor->_nDstRoom);
        RoomInfo* pDstRoom = getRoomInfo(pDoor->_nDstRoom);
        if (!(pDstRoom)) AssertApp(__FILE__, 1657, D_00484A10, 0);
        if (pDoor->_shape == 5 && (USHORT)pDstRoom->getSceneNum() == 2 && !pDstRoom->hasEntered()) {
            makeRoom(*pDstRoom, _getDstDoorID(door), 2, getCardIDFromSceneNum(2), CGameWork::m_pMCWork->_nNextRCNormaNum);
        } else if (!pDstRoom->isCreated() || (pDstRoom->isFixSceneRoom() && !pDstRoom->hasEntered())) {
            if (!pDstRoom->isCreated()) TRACE_COL(_TTY_RED, ">>>>> Create New Room !!\n");
            makeRoom(*pDoor, !pDstRoom->isCreated());
        }
        if (pDoor->_type == DOOR_TYPE_FLOOR_CHANGE) {
            SHORT nFloor = getCurrentFloorNumber();
            if (getCurrentRoomID()) nFloor++;
            else nFloor--;
            bResult = reqChangeFloor(nFloor, FALSE);
        } else {
            bResult = reqChangeRoom(pDoor->_nDstRoom, _getDstDoorID(door), -1);
        }
        if (bResult) {
            if (pDoor->isEventDoor()) {
                backupEventDoorState();
                CGameWork::m_pMCWork->_nDoorID = pDoor->_id;
                CGameWork::backup(CGameWork::BACKUP_PRIMARY);
                pDoor->setDoorState(DOOR_STAT_UNAVAILABLE);
                _setOpenFlagEventDoor(*pDoor);
            } else if (!isEventScene()) {
                CGameWork::m_pMCWork->_nDoorID = pDoor->_id;
                CGameWork::backup(CGameWork::BACKUP_PRIMARY);
            }
            if (dbgCheckUser(TGOTO)) {
                RoomInfo* pInfo = getRoomInfo(pDoor->_nDstRoom);
                if (pInfo) TRACE("*** DoorState : RU[%d] RD[%d] LD[%d] LU[%d]\n", pInfo->_doors[0].getDoorState(), pInfo->_doors[1].getDoorState(), pInfo->_doors[2].getDoorState(), pInfo->_doors[3].getDoorState());
            }
        }
    }
    return bResult;
}
void _acceptReqEnterBattle() {
    s_nReqEnterBattle = CSceneInfo::getCurrentWorldID(TRUE) != 10 ? GAMEMODE_BATTLE : (GAMEMODE)2;
}
BOOL CSceneInfo::reqEnterBattle() {
    if (hasAnyRequest()) return FALSE;
    if (s_nGameMode != GAMEMODE_FIELD) return FALSE;
    SHORT nScene = getCurrentBattleSceneNum();
    if (nScene <= 0) nScene = s_nCurrentFieldScene;
    if (reqChangeScene(getCurrentAreaNum(), nScene, nScene, TRUE, 1)) _acceptReqEnterBattle();
    return s_nReqEnterBattle != GAMEMODE_FIELD;
}
BOOL CSceneInfo::reqEnterBattle(SHORT nBtlScene) {
    if (getCurrentWorldID(TRUE) == 10) return reqEnterMinigame();
    if (hasAnyRequest()) return FALSE;
    if (s_nGameMode != GAMEMODE_FIELD) return FALSE;
    if (nBtlScene <= 0) {
        nBtlScene = getCurrentBattleSceneNum();
        if (nBtlScene <= 0) nBtlScene = s_nCurrentFieldScene;
    }
    if (nBtlScene > 0) {
        if (reqChangeScene(getCurrentAreaNum(), nBtlScene, nBtlScene, TRUE, 1)) _acceptReqEnterBattle();
    }
    return s_nReqEnterBattle != GAMEMODE_FIELD;
}
BOOL CSceneInfo::reqExitBattle() {
    if (hasAnyRequest()) return FALSE;
    if (s_nGameMode == GAMEMODE_FIELD) return FALSE;
    reqChangeScene(getCurrentAreaNum(), s_nCurrentFieldScene, -1, 1, 1);
    s_nReqExitBattle = 1;
    return TRUE;
}
GAMEMODE CSceneInfo::hasReqEnterBattle() { return s_nReqEnterBattle; }
BOOL CSceneInfo::hasReqExitBattle() { return s_nReqExitBattle != 0; }
BOOL CSceneInfo::hasReqRetryBattle() { return s_nReqExitBattle == 2; }
void CSceneInfo::cancelBattleRequest() {
    s_nReqEnterBattle = GAMEMODE_FIELD;
    s_nReqExitBattle = 0;
}
BOOL CSceneInfo::reqEnterMinigame() { return reqEnterBattle(); }
BOOL CSceneInfo::reqEnterMinigame(SHORT inID) { return reqEnterBattle(inID); }
BOOL CSceneInfo::reqExitMinigame(SHORT nScene) {
    if (hasAnyRequest()) return FALSE;
    if (s_nGameMode == GAMEMODE_FIELD) return FALSE;
    if (nScene < 0) return FALSE;
    if (nScene == 0) nScene = s_nCurrentFieldScene;
    reqChangeScene(getCurrentAreaNum(), nScene, -1, 1, 1);
    s_nReqExitBattle = 1;
    return TRUE;
}
BOOL CSceneInfo::reqRetryMinigame() {
    BOOL b = reqExitBattle();
    if (b) s_nReqExitBattle = 2;
    return b;
}
BOOL CSceneInfo::reqChangeScene(SHORT nArea, SHORT nScene, SHORT nBtlScene, BOOL bSkipIfSame, INT nAtelSrcID) {
    if (hasAnyRequest()) return FALSE;
    if (!bSkipIfSame || (!hasReqChangeScene() && (getCurrentAreaNum() != nArea || getCurrentSceneNum() != nScene))) {
        setNextAreaScene(nArea, nScene, nBtlScene);
        s_nAtelSrcID = nAtelSrcID;
        s_nReqChangeScene |= 1;
    }
    return hasReqChangeScene();
}
SHORT CSceneInfo::getCurrentAreaNum() { return CGameWork::m_pMCWork->_nArea; }
SHORT CSceneInfo::getCurrentSceneNum() { return CGameWork::m_pMCWork->_nScene; }
INT CSceneInfo::getCurrentAreaScene() {
    _AREA_SCENE_REG as;
    as._nArea = CGameWork::m_pMCWork->_nArea;
    as._nScene = CGameWork::m_pMCWork->_nScene;
    return as._nAreaScene;
}
SHORT CSceneInfo::getCurrentFieldSceneNum() { return s_nCurrentFieldScene; }
SHORT CSceneInfo::getCurrentBattleSceneNum() { return s_nCurrentBattleScene; }
SHORT CSceneInfo::getNextAreaNum() { return s_NextAreaScene._nArea; }
SHORT CSceneInfo::getNextSceneNum() { return s_NextAreaScene._nScene; }
INT CSceneInfo::getNextAreaScene() { return s_NextAreaScene._nAreaScene; }
SHORT CSceneInfo::getNextBattleSceneNum() { return s_nNextBattleScene; }
BOOL CSceneInfo::isNextBattleRsrcUpdate() {
    BOOL bRet = FALSE;
    if (s_nNextBattleScene > 0) bRet = s_nCurrentBattleRsrc != s_nNextBattleScene;
    return bRet;
}
SHORT CSceneInfo::getPrevAreaNum() { return CGameWork::m_pMCWork->_nPrevArea; }
SHORT CSceneInfo::getPrevSceneNum() { return CGameWork::m_pMCWork->_nPrevScene; }
INT CSceneInfo::getPrevAreaScene() {
    _AREA_SCENE_REG as;
    as._nArea = CGameWork::m_pMCWork->_nPrevArea;
    as._nScene = CGameWork::m_pMCWork->_nPrevScene;
    return as._nAreaScene;
}
BOOL CSceneInfo::hasReqChangeScene() { return s_nReqChangeScene & 1; }
BOOL CSceneInfo::hasReqChangeFloor() { return (SBYTE)s_nNextFloorIdx >= 0; }
INT CSceneInfo::getNextFloorIndex() { return (SBYTE)s_nNextFloorIdx; }
INT CSceneInfo::getNextAtelSrcID() { return s_nAtelSrcID; }
SHORT CSceneInfo::getCurrentFloorNumber() { return s_nCurrentFloorNumber; }
USHORT CSceneInfo::getCurrentFloorIndex() { return CGameWork::m_pMCWork->_nFloorIdx; }
BOOL CSceneInfo::isTgsFloor() {
    BOOL b = FALSE;
    USHORT n = getCurrentFloorIndex();
    if (CGame::isSoraGame()) b = n == 14;
    return b;
}
BOOL CSceneInfo::isJFestaFloor() {
    BOOL b = FALSE;
    USHORT n = getCurrentFloorIndex();
    if (CGame::isSoraGame()) b = n == 13;
    return b;
}
WORLD_ID CSceneInfo::getCurrentWorldID(BOOL bChkEntrance) {
    if (bChkEntrance && getCurrentAreaNum() == 0) return WORLD_ID_COMMON;
    USHORT nIdx = getCurrentFloorIndex();
    BOOL bSora = CGame::isSoraGame();
    FloorLayoutDataHeader* pHeader = _isValidFloorIdx(nIdx, bSora);
    if (pHeader) {
        INT n = !bSora;
        if (nIdx < pHeader->_nFloorCount) return (WORLD_ID)_SceneInfoWork::mc2()->_nWorldOnFloor[n][nIdx];
    }
    WORLD_ID w = (WORLD_ID)getCurrentAreaNum();
    if ((UINT)w >= 14) {
        INT nMapGroup = (UINT)CRcf::getMapID(w, getCurrentSceneNum()) / 10000 - 50;
        if ((UINT)nMapGroup <= 13) w = (WORLD_ID)nMapGroup;
    }
    return w;
}
WORLD_ID CSceneInfo::getWorldID(USHORT nFloorIndex) {
    BOOL bSora = CGame::isSoraGame();
    FloorLayoutDataHeader* p = _isValidFloorIdx(nFloorIndex, bSora);
    if (!p) return (WORLD_ID)0;
    INT n = !bSora;
    if (nFloorIndex >= p->_nFloorCount) return (WORLD_ID)0;
    return (WORLD_ID)_SceneInfoWork::mc2()->_nWorldOnFloor[n][nFloorIndex];
}
void CSceneInfo::setupCompleteFloorWorld(WORLD_ID excludeWorld) {
    USHORT i = 0;
    USHORT nFloorIdx = getCurrentFloorIndex();
    BOOL bSora = CGame::isSoraGame();
    STORAGE::MCWork2* p = _SceneInfoWork::mc2();
    BYTE* pFloorTbl = !bSora ? p->_nWorldOnFloor[1] : p->_nWorldOnFloor[0];
    WORLD_ID w;
    for (; i < nFloorIdx; i++) {
        WorldSelTable* pSel = getSelectableWorldTable(i, bSora);
        if (pSel->_nCount == 1) {
            w = pSel->_pTbl[0];
        } else {
            for (UINT j = 0; j < pSel->_nCount; j++) {
                w = pSel->_pTbl[j];
                if (excludeWorld == w) {
                    w = (WORLD_ID)0;
                    continue;
                }
                for (USHORT f = 0; f < nFloorIdx; f++) {
                    if (pFloorTbl[f] == w) {
                        w = (WORLD_ID)0;
                        break;
                    }
                }
                if (w) break;
            }
        }
        pFloorTbl[i] = w;
        CAtel::setWorldCompleteFlag(w);
        _setGoalRoomScene(w, 2);
    }
}
void CSceneInfo::setEventDoorUnavailable(INT nFloorIdx, INT nRoomID) {
    FLOOR::FloorLayout* pFloorSrc = getFloorDataSrc(nFloorIdx, CGame::isSoraGame());
    if (!pFloorSrc) return;
    INT nEvIdx;
    BOOL bAll = nRoomID == 0 || nRoomID == 99;
    if (bAll) {
        nEvIdx = nRoomID == 99 ? 4 : -1;
    } else {
        FLOOR::RoomSrcInfo* pRoom = NULL;
        for (INT i = 0; i < pFloorSrc->_nRoomCount; i++) {
            if (pFloorSrc->_rooms[i]._nID == nRoomID) {
                pRoom = &pFloorSrc->_rooms[i];
                break;
            }
        }
        if (!pRoom) return;
        switch (pRoom->_nRoomType) {
        case 2:
            nEvIdx = 0;
            break;
        case 3:
            nEvIdx = 1;
            break;
        case 4:
            nEvIdx = 2;
            break;
        case 5:
        case 7:
            nEvIdx = 4;
            break;
        default:
            {
                BOOL bSora = CGame::isSoraGame();
                nEvIdx = -1;
                if (bSora && nFloorIdx == 12 && nRoomID == 19) nEvIdx = 0;
            }
            break;
        }
    }
    for (INT i = 0; i < nFloorIdx; i++) {
        BYTE* pTbl = _SceneInfoWork::mc()->_nEventDoorStateTbl[i];
        pTbl[1] = 4;
        pTbl[2] = 4;
        pTbl[4] = 1;
        pTbl[0] = 4;
        pTbl = _SceneInfoWork::mc2()->_nEventDoorOpenTbl[i];
        pTbl[0] = 1;
        pTbl[1] = 1;
        pTbl[2] = 1;
    }
    BYTE* pTbl = _SceneInfoWork::mc()->_nEventDoorStateTbl[nFloorIdx];
    BYTE* pTbl2 = _SceneInfoWork::mc2()->_nEventDoorOpenTbl[nFloorIdx];
    for (INT r = 0; r <= nEvIdx; r++) {
        if (r == 3) continue;
        BYTE st = r == 4 ? 2 : 4;
        pTbl[r] = st;
        if (r < 3) pTbl2[r] = 1;
    }
}
DOOR_ID CSceneInfo::setTmpCurrentRoomID(SHORT nRoomType) {
    DOOR_TYPE nType;
    switch (nRoomType) {
    case 2:
        nType = DOOR_TYPE_TO_EVENT1;
        break;
    case 3:
        nType = DOOR_TYPE_TO_EVENT2;
        break;
    case 4:
        nType = DOOR_TYPE_TO_EVENT3;
        break;
    default:
        return (DOOR_ID)4;
    }
    DOOR_ID doorID = (DOOR_ID)4;
    FloorInfo& floor = s_CurrentFloor;
    for (USHORT i = 0; i < floor._nRoomCount; i++) {
        RoomInfo& room = floor._rooms[i];
        for (INT j = 0; j < 4; j++) {
            if (nType == room._doors[j]._type) {
                if (room._pSrcInfo) {
                    CGameWork::m_pMCWork->_nRoomID = room._pSrcInfo->_nID;
                    doorID = _getDstDoorID((DOOR_ID)j);
                }
                break;
            }
        }
    }
    return doorID;
}
void CSceneInfo::ignoreFloorInfo() {
    CGameWork::m_pMCWork->_nFloorIdx = 0xff;
    s_nCurrentFloorNumber = 0;
}
INT CSceneInfo::getCurrentRoomID() {
    INT n = CGameWork::m_pMCWork->_nRoomID;
    BOOL b = FALSE;
    if (n == 0 || n == 99) b = TRUE;
    if (b) return n;
    return (UINT)n < 32 ? n : -1;
}
INT CSceneInfo::getNextRoomID() { return (SBYTE)s_nNextRoomID; }
INT CSceneInfo::getPrevRoomID() { return CGameWork::m_pMCWork->_nPrevRoomID; }
DOOR_ID CSceneInfo::getEnteredDoorID() {
    DOOR_ID id = (DOOR_ID)CGameWork::m_pMCWork->_nDoorID;
    return (UINT)id >= _DOOR_ID_MAX ? DOOR_ID_LD : id;
}
DOOR_ID CSceneInfo::getNextDoorID() { return s_nNextDoorID; }
BOOL CSceneInfo::isFieldScene() {
    BOOL b = FALSE;
    if (s_nGameMode == GAMEMODE_FIELD) b = !isEventScene();
    return b;
}
BOOL CSceneInfo::isBattleScene() { return s_nGameMode == GAMEMODE_BATTLE; }
BOOL CSceneInfo::isBossBattleScene(INT nEnemyID) {
    SHORT s = getCurrentSceneNum();
    WORLD_ID w = (WORLD_ID)getCurrentWorldID(TRUE);
    BOOL bBoss = FALSE;
    if (s_nGameMode == GAMEMODE_BATTLE) {
        if (nEnemyID == 124) {
            if (w == 12 && s == 0x47f) bBoss = TRUE;
        } else {
            bBoss = s >= 0x41a && s <= 0x4ae;
        }
    }
    return bBoss;
}
BOOL CSceneInfo::isEventScene() { return (USHORT)(getCurrentSceneNum() - 2000) < 1000; }
BOOL CSceneInfo::isMinigameScene() { return (USHORT)(getCurrentSceneNum() - 1200) < 56; }
BOOL CSceneInfo::isLastFloorScene() {
    BOOL b = FALSE;
    if (CGame::isSoraGame() && getCurrentWorldID(TRUE) == 13) b = (USHORT)getCurrentSceneNum() == 90;
    return b;
}
_GimmickRoomInfo* CSceneInfo::getCurrentRoomGimmickInfo() {
    INT nID = getCurrentRoomID();
    if (nID < 0) return NULL;
    INT i = getRoomIndex(nID);
    if (i < 0) return NULL;
    return (_GimmickRoomInfo*)((BYTE*)&CGameWork::m_pMCWork->_roomInfo[i] + 0x8);
}
_MapEnemyRoomInfo* CSceneInfo::getCurrentRoomMapEnemyInfo() {
    INT nID = getCurrentRoomID();
    if (nID < 0) return NULL;
    INT i = getRoomIndex(nID);
    if (i < 0) return NULL;
    return (_MapEnemyRoomInfo*)((BYTE*)&CGameWork::m_pMCWork->_roomInfo[i] + 0x6a);
}
void CSceneInfo::doneChangeScene() {
    s_nReqChangeScene = 0;
    s_nReqEnterBattle = GAMEMODE_FIELD;
    s_nReqExitBattle = 0;
    s_NextAreaScene._nAreaScene = -1;
    s_nNextBattleScene = -1;
    s_nNextRoomID = 0xff;
    s_nNextFloorIdx = 0xff;
    RoomInfo* pRoom = getRoomInfo(-1);
    if (pRoom) {
        ROOM_STORAGE* p = (ROOM_STORAGE*)pRoom->_pStorageInfo;
        if (p) {
            p->_bFirstTimeAfterRC = 0;
            p->_bEntered = 1;
        }
    }
}
void CSceneInfo::update() {
    if (s_NextAreaScene._nAreaScene < 0) return;
    STORAGE::MCWork* p = CGameWork::m_pMCWork;
    *(SHORT*)&p->_nPrevArea = p->_nArea; // MATCH: scalar store, the original reloads m_pMCWork after it
    CGameWork::m_pMCWork->_nPrevScene = p->_nScene;
    p->_nArea = s_NextAreaScene._nArea;
    p->_nScene = s_NextAreaScene._nScene;
    _AREA_SCENE_REG as;
    as._nArea = p->_nArea;
    as._nScene = p->_nScene;
    s_nCurrentSceneCacheID = addToSceneCache((AREA_SCENE&)as);
    CRsrcDB::setSceneAttachMode(s_nCurrentSceneCacheID);
    _updateRoomInfo();
    if (s_nReqEnterBattle) {
        s_nGameMode = s_nReqEnterBattle;
        if (s_nNextBattleScene > 0) updateBattleScene();
    } else if (s_nReqExitBattle == 1) {
        *(GAMEMODE*)&s_nGameMode = GAMEMODE_FIELD; // MATCH: scalar store, the original reads p->_nScene after it
        s_nCurrentFieldScene = p->_nScene;
        if (!CGame::isDbgStartFromBattle()) updateBattleScene();
    } else {
        s_nCurrentFieldScene = p->_nScene;
        updateBattleScene();
    }
    if (s_nReqChangeScene & 2) {
        CGameWork::rollback(CGameWork::BACKUP_PRIMARY);
        rollbackEventDoorState(0);
        CStatus::setHP(CStatus::getHPMax());
    }
}
void CSceneInfo::updateBattleScene() {
    if ((USHORT)s_nNextBattleScene == 0x4af) {
        removeCurrentBattleSceneRsrc();
        removeMapRsrcCache(2);
        s_nCurrentBattleRsrc = -1;
        s_nCurrentBattleScene = -1;
        return;
    }
    if (isNextBattleRsrcUpdate()) {
        removeCurrentBattleSceneRsrc();
        if (checkMapRsrcCache(CRcf::getMapID(s_NextAreaScene._nArea, s_nNextBattleScene), 2) < 0) removeMapRsrcCache(2);
        s_nCurrentBattleRsrc = s_nNextBattleScene;
    }
    s_nCurrentBattleScene = s_nNextBattleScene;
}
void _updateRoomInfo() {
    if ((SBYTE)s_nNextRoomID < 0) return;
    BYTE nRoom = CSceneInfo::getCurrentRoomID();
    if (nRoom != s_nNextRoomID) {
        _SceneInfoWork::mc()->_nPrevRoomID = nRoom;
        _SceneInfoWork::mc()->_nRoomID = s_nNextRoomID;
        PLM::g_bIsLastSceneBattle = FALSE;
    }
    _SceneInfoWork::mc()->_nDoorID = s_nNextDoorID;
    RoomInfo* pRoomInfo = CSceneInfo::getRoomInfo(-1);
    if (!(pRoomInfo)) AssertApp(__FILE__, 2540, "pRoomInfo", 0);
    for (INT i = 0; i < 4; i++) {
        DoorInfo& door = pRoomInfo->_doors[i];
        if (door._type == DOOR_TYPE_TO_EXTRA && _SceneInfoWork::mc()->_KeyRoomsTresureBox[CSceneInfo::getCurrentWorldID(FALSE)]) {
            door.setDoorState(DOOR_STAT_UNAVAILABLE);
            RoomInfo* pExtraRoom = CSceneInfo::getRoomInfo(door._nDstRoom);
            if (pExtraRoom) pExtraRoom->_doors[_getDstDoorID((DOOR_ID)i)].setDoorState(DOOR_STAT_UNAVAILABLE);
        }
    }
}
void CSceneInfo::clearSceneCache() {
    for (INT i = 0; i < 7; i++) s_SceneCacheTbl[i].clear();
    s_nAvailableSceneCache = 7;
    s_nCurrentSceneCacheID = -1;
}
INT CSceneInfo::getCurrentSceneCacheID() { return s_nCurrentSceneCacheID; }
INT CSceneInfo::getSceneCacheID(const AREA_SCENE& as) {
    if (as._nAreaScene < 0) return -1;
    for (INT i = 0; i < 7; i++) {
        if (s_SceneCacheTbl[i]._as._nAreaScene == as._nAreaScene) return i;
    }
    return -1;
}
INT CSceneInfo::addToSceneCache(const AREA_SCENE& as) {
    if (as._nAreaScene < 0) return -1;
    INT nID = getSceneCacheID(as);
    if (nID >= 0) return nID;
    if (s_nAvailableSceneCache == 0) {
        TRACE("CSceneInfo::addSceneCacheID() : \x82\xb1\x82\xea\x88\xc8\x8f\xe3\x92\xc7\x89\xc1\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\n");
        return -1;
    }
    for (INT i = 0; i < 7; i++) {
        if (s_SceneCacheTbl[i]._as._nAreaScene < 0) {
            s_SceneCacheTbl[i]._as._nAreaScene = as._nAreaScene;
            if (!(7 >= s_nAvailableSceneCache)) AssertApp(__FILE__, 2622, "_SCENE_CACHE_SIZE >= s_nAvailableSceneCache", 0);
            s_nAvailableSceneCache--;
            if (!(0 <= s_nAvailableSceneCache)) AssertApp(__FILE__, 2624, "0 <= s_nAvailableSceneCache", 0);
            return i;
        }
    }
    return -1;
}
INT CSceneInfo::removeFromSceneCache(const AREA_SCENE& as) {
    if (as._nAreaScene < 0) return -1;
    for (INT i = 0; i < 7; i++) {
        if (s_SceneCacheTbl[i]._as._nAreaScene == as._nAreaScene) {
            s_SceneCacheTbl[i].clear();
            if (!(0 < s_nAvailableSceneCache)) AssertApp(__FILE__, 2643, "0 < s_nAvailableSceneCache", 0);
            s_nAvailableSceneCache++;
            if (!(7 >= s_nAvailableSceneCache)) AssertApp(__FILE__, 2645, "_SCENE_CACHE_SIZE >= s_nAvailableSceneCache", 0);
            return i;
        }
    }
    return -1;
}
BOOL CSceneInfo::removeFromSceneCacheWithRsrc(const AREA_SCENE& as) {
    INT nID = removeFromSceneCache(as);
    if (nID < 0) return FALSE;
    TRACE_USER_COL((USER)4, (TtyColor)0x22, "=== Delete Scene (Attached) Resources (%d-%d) ===\n", as._nArea, as._nScene);
    CRsrcDB::removeSceneAttachedData(nID);
    return TRUE;
}
void CSceneInfo::removeCurrentFieldSceneRsrc() {
    if (s_nCurrentFieldScene > 0) {
        AREA_SCENE as(getCurrentAreaNum(), s_nCurrentFieldScene);
        removeFromSceneCacheWithRsrc(as);
    }
}
void CSceneInfo::removeCurrentBattleSceneRsrc() {
    if (s_nCurrentBattleRsrc > 0) {
        AREA_SCENE as(getCurrentAreaNum(), s_nCurrentBattleRsrc);
        removeFromSceneCacheWithRsrc(as);
        s_nCurrentBattleRsrc = 0;
    }
}
void CSceneInfo::setReverbVal(BYTE nRev) {
    if (CRsrcDB::isSceneAttachMode()) s_SceneCacheTbl[CRsrcDB::getSceneAttachNum()]._nReverb = nRev;
    else s_GlobalScene._nReverb = nRev;
}
BYTE CSceneInfo::getReverbVal() {
    INT i = getCurrentSceneCacheID();
    return i < 0 ? s_GlobalScene._nReverb : s_SceneCacheTbl[i]._nReverb;
}
BYTE CSceneInfo::getReverbVal(const AREA_SCENE& as) {
    INT i = getSceneCacheID(as);
    return i < 0 ? s_GlobalScene._nReverb : s_SceneCacheTbl[i]._nReverb;
}
void CSceneInfo::setSeRangeRatio(BYTE nRatio) {
    char szTmp[64];
    CHAR* pTmp = szTmp;
    if (CRsrcDB::isSceneAttachMode()) {
        _SceneInfo& info = s_SceneCacheTbl[CRsrcDB::getSceneAttachNum()];
        info._nSeRangeRatio = nRatio;
        sprintf(szTmp, "%d-%d", info._as._nArea, info._as._nScene);
    } else {
        s_GlobalScene._nSeRangeRatio = nRatio;
        pTmp = "Global";
    }
    TRACE_COL(_TTY_RED, "=*=*=*=* SE_RANGE (%s) = [%d%%] *=*=*=*=\n", pTmp, nRatio * 10);
}
BYTE CSceneInfo::getSeRangeRatio() {
    INT i = getCurrentSceneCacheID();
    return i < 0 ? s_GlobalScene._nSeRangeRatio : s_SceneCacheTbl[i]._nSeRangeRatio;
}
BYTE CSceneInfo::getSeRangeRatio(const AREA_SCENE& as) {
    INT i = getSceneCacheID(as);
    return i < 0 ? s_GlobalScene._nSeRangeRatio : s_SceneCacheTbl[i]._nSeRangeRatio;
}
void _clearMapRsrcCache() {
    s_nMapCacheField = 0;
    s_nMapCacheBattle = 0;
}
INT CSceneInfo::checkMapRsrcCache(INT nMapRsrcGrp, INT nSlot) {
    if (nMapRsrcGrp) {
        switch (nSlot) {
        case 0:
            if (nMapRsrcGrp == s_nMapCacheField) {
                if (nMapRsrcGrp != s_nMapCacheBattle) return 1;
                return 0;
            }
        case 2:
            if (nMapRsrcGrp == s_nMapCacheBattle) return 2;
            break;
        case 1:
            if (nMapRsrcGrp == s_nMapCacheField) return 1;
            break;
        }
    }
    return -1;
}
INT CSceneInfo::getMapRsrcCache(INT nSlot) {
    switch (nSlot) {
    case 1: return s_nMapCacheField;
    case 2: return s_nMapCacheBattle;
    }
    return 0;
}
void CSceneInfo::updateMapRsrcCache(INT nMapRsrcGrp, INT nSlot) {
    switch (nSlot) {
    case 0:
        {
            INT n = s_nMapCacheField;
            if (n && nMapRsrcGrp != n) {
                TRACE_USER_COL(TGOTO, _TTY_BLUE, ">>> REMOVE MAP-RSRC [[[%d]]]\n", n);
                CRsrcDB::removeRsrc(n);
            }
        }
        s_nMapCacheField = nMapRsrcGrp;
    case 2:
        {
            INT n = s_nMapCacheBattle;
            if (n && nMapRsrcGrp != n) {
                TRACE_USER_COL(TGOTO, _TTY_BLUE, ">>> REMOVE MAP-RSRC [[[%d]]]\n", n);
                CRsrcDB::removeRsrc(n);
            }
        }
        s_nMapCacheBattle = nMapRsrcGrp;
        break;
    case 1:
        {
            INT n = s_nMapCacheField;
            if (n && nMapRsrcGrp != n) {
                TRACE_USER_COL(TGOTO, _TTY_BLUE, ">>> REMOVE MAP-RSRC [[[%d]]]\n", n);
                CRsrcDB::removeRsrc(n);
            }
        }
        s_nMapCacheField = nMapRsrcGrp;
        break;
    }
}
void CSceneInfo::removeMapRsrcCache(INT nSlot) {
    switch (nSlot) {
    case 0: {
        INT n = s_nMapCacheField;
        if (n) {
            TRACE_USER_COL(TGOTO, _TTY_BLUE, ">>> REMOVE MAP-RSRC [[[%d]]]\n", n);
            CRsrcDB::removeRsrc(n);
            s_nMapCacheField = 0;
        }
        n = s_nMapCacheBattle;
        if (n) {
            TRACE_USER_COL(TGOTO, _TTY_BLUE, ">>> REMOVE MAP-RSRC [[[%d]]]\n", n);
            CRsrcDB::removeRsrc(n);
            s_nMapCacheBattle = 0;
        }
        break;
    }
    case 1:
        {
            INT n = s_nMapCacheField;
            if (n && s_nMapCacheBattle != n) {
                TRACE_USER_COL(TGOTO, _TTY_BLUE, ">>> REMOVE MAP-RSRC [[[%d]]]\n", n);
                CRsrcDB::removeRsrc(n);
            }
        }
        s_nMapCacheField = 0;
        break;
    case 2:
        {
            INT n = s_nMapCacheBattle;
            if (n && n != s_nMapCacheField) {
                TRACE_USER_COL(TGOTO, _TTY_BLUE, ">>> REMOVE MAP-RSRC [[[%d]]]\n", n);
                CRsrcDB::removeRsrc(n);
            }
        }
        s_nMapCacheBattle = 0;
        break;
    }
}
FloorLayoutDataHeader* _linkFloorData(const CHAR* pszFile) {
    CRsrcData* pData = CRsrcDB::getRsrc(1, pszFile, NAME_CASE_SENSITIVE);
    FloorLayoutDataHeader* p = pData ? (FloorLayoutDataHeader*)CRsrcDB::getRsrcData(*pData) : NULL;
    if (p && (UINT)p->_pFloorOfsTbl <= 0xfffff) {
        p->_pFloorOfsTbl = (INT*)((UINT)p->_pFloorOfsTbl + (UINT)p);
        p->_pFloorData = (void*)((UINT)p->_pFloorData + (UINT)p);
    }
    return p;
}
EventRoomData* _linkEventRoomData(const CHAR* pszFile) {
    CRsrcData* pData = CRsrcDB::getRsrc(1, pszFile, NAME_CASE_SENSITIVE);
    if (!pData) return NULL;
    return (EventRoomData*)CRsrcDB::getRsrcData(*pData);
}
NormaDataHeader* _linkNormaData(const CHAR* pszFile) {
    CRsrcData* pData = CRsrcDB::getRsrc(1, pszFile, NAME_CASE_SENSITIVE);
    if (!pData) return NULL;
    return (NormaDataHeader*)CRsrcDB::getRsrcData(*pData);
}
FloorLayoutDataHeader* _isValidFloorIdx(USHORT nIdx, BOOL bSora) {
    FloorLayoutDataHeader* pHeader = bSora ? s_pFloorFile_Sora : s_pFloorFile_Riku;
    FloorLayoutDataHeader* pRet = NULL;
    if (pHeader) pRet = nIdx < pHeader->_nFloorCount ? pHeader : NULL;
    return pRet;
}
FLOOR::FloorLayout* CSceneInfo::getCurrentFloorDataSrc() {
    USHORT nFloor = getCurrentFloorNumber();
    if (!nFloor) return NULL;
    BOOL bSora = CGame::isSoraGame();
    return getFloorDataSrc(getFloorIndex(nFloor, bSora), bSora);
}
FLOOR::FloorLayout* CSceneInfo::getFloorDataSrc(USHORT nIdx, BOOL bSora) {
    FloorLayoutDataHeader* pHeader = _isValidFloorIdx(nIdx, bSora);
    if (!pHeader) return NULL;
    FLOOR::FloorLayout* pSrc = (FLOOR::FloorLayout*)((BYTE*)pHeader->_pFloorData + pHeader->_pFloorOfsTbl[nIdx]);
    return pHeader->_nVersion == 4 ? pSrc : NULL;
}
RCNORMA::FloorNormaSrc* CSceneInfo::getFloorNormaDataSrc(USHORT nIdx, BOOL bSora) {
    NormaDataHeader* pHeader = bSora ? s_pFloorNormaFile_Sora : s_pFloorNormaFile_Riku;
    if (!pHeader) return NULL;
    return &pHeader->_FloorSrc[nIdx];
}
RoomInfo* CSceneInfo::getRoomInfo(INT nRoomID) {
    if (getCurrentFloorIndex() == 0xff) return &s_roomExcept;
    if (nRoomID < 0) nRoomID = getCurrentRoomID();
    if (nRoomID == 0) return &s_CurrentFloor._roomStartEnt;
    if (nRoomID == 99) return &s_CurrentFloor._roomGoalEnt;
    RoomInfo* pRoom = s_CurrentFloor._rooms;
    for (INT i = 0; i < s_CurrentFloor._nRoomCount; i++, pRoom++) {
        if (pRoom->_pSrcInfo && pRoom->_pSrcInfo->_nID == nRoomID) return pRoom;
    }
    return NULL;
}
RoomInfo* CSceneInfo::getEventRoomInfo(UINT nEventRoomType) {
    if (getCurrentFloorIndex() == 0xff) return &s_roomExcept;
    RoomInfo* pRoom = NULL;
    EVENTROOM_TBLIDX i = (EVENTROOM_TBLIDX)nEventRoomType;
    if (i < __EVENTROOM_MAX) {
        INT nIdx = s_CurrentFloor.getEventRoomIdx(i);
        if (nIdx >= 0) pRoom = &s_CurrentFloor._rooms[nIdx];
    } else {
        switch (i) {
        case _EVENTROOM_START_ENTRANCE:
            pRoom = &s_CurrentFloor._roomStartEnt;
            break;
        case _EVENTROOM_GOAL_ENTRANCE:
            pRoom = &s_CurrentFloor._roomGoalEnt;
            break;
        }
    }
    return pRoom;
}
INT CSceneInfo::getRoomIndex(INT nRoomID) {
    if (getCurrentFloorIndex() >= 0xff) return -1;
    if (nRoomID < 0) nRoomID = getCurrentRoomID();
    RoomInfo* pRoom = s_CurrentFloor._rooms;
    for (INT i = 0; i < s_CurrentFloor._nRoomCount; i++, pRoom++) {
        if (pRoom->_pSrcInfo && pRoom->_pSrcInfo->_nID == nRoomID) return i;
    }
    return -1;
}
DoorInfo* CSceneInfo::getDoorInfo(DOOR_ID id, INT nRoomID) {
    RoomInfo* pRoom = getRoomInfo(nRoomID);
    if (!pRoom) return NULL;
    return &pRoom->_doors[id];
}
BOOL CSceneInfo::setDoorStat(DOOR_STAT stat, DOOR_ID id, INT nRoomID) {
    DoorInfo* pDoor = getDoorInfo(id, nRoomID);
    if (pDoor) return pDoor->setDoorState(stat);
    return FALSE;
}
DOOR_ID _openGoalDoor(const RoomInfo& roomGoal) {
    DoorInfo* pGoalDoor = NULL;
    INT nDoor = 0;
    while (nDoor < 4) {
        const DoorInfo& d = roomGoal._doors[nDoor];
        if (d.getDoorState() != DOOR_STAT_NOTHING) {
            RoomInfo* pDstRoom = CSceneInfo::getRoomInfo(d._nDstRoom);
            if (pDstRoom) {
                DoorInfo& dd = pDstRoom->_doors[_getDstDoorID((DOOR_ID)nDoor)];
                if (dd._type == DOOR_TYPE_TO_GOAL || dd._type == DOOR_TYPE_TO_LAST) {
                    pGoalDoor = &dd;
                    break;
                }
            }
        }
        nDoor++;
    }
    if (!(pGoalDoor)) AssertApp(__FILE__, 3094, "pGoalDoor", 0);
    pGoalDoor->setDoorState(DOOR_STAT_OPENED);
    return (DOOR_ID)nDoor;
}
void CSceneInfo::openGoalDoor() {
    RoomInfo* pRoom = getEventRoomInfo(_EVENTROOM_GOAL);
    if (!pRoom || pRoom->isExceptRoom()) {
        TRACE(D_00484E38);
        return;
    }
    DOOR_ID nDoor = _openGoalDoor(*pRoom);
    setEventRoomSceneNum(_EVENTROOM_GOAL, 2);
    makeRoom(*pRoom, nDoor, 2, getCardIDFromSceneNum(2), CGameWork::m_pMCWork->_nNextRCNormaNum);
}
__asm__(".rdata\n.align 3\n.word 0x8B837581\n.word 0x80835B81\n.word 0x5A836564\n.word 0x4E838C83\n.word 0x76816783\n.word 0x4F8AC888\n.word 0xFC93C582\n.word 0xC482C182\n.word 0xCC82E982\n.word 0x4181C582\n.word 0x5B815383\n.word 0xE0948B83\n.word 0x4A8AF082\n.word 0xDC82AF82\n.word 0xF182B982\n.word 0x0000000A\n.word 0x00000000\n.word 0x00000000\n.text\n");
void CSceneInfo::backupEventDoorState() {
    USHORT nFloorIdx = getCurrentFloorIndex();
    if (nFloorIdx == 0xff) return;
    FloorInfo& floor = s_CurrentFloor;
    STORAGE::_EventDoorInfo* pInfoBackupTbl = _SceneInfoWork::mc()->_nEventDoorInfoTbl[nFloorIdx];
    BYTE* pStateBackupTbl = _SceneInfoWork::mc()->_nEventDoorStateTbl[nFloorIdx];
    for (USHORT i = 0; i < floor._nRoomCount; i++) {
        RoomInfo& room = floor._rooms[i];
        for (INT j = 0; j < 4; j++) {
            DoorInfo& door = room._doors[j];
            if (door._nDstRoom < 0) continue;
            INT nDoorType = door._type;
            INT nType = -1;
            switch (nDoorType) {
            case 1:
                nType = 0;
                break;
            case 2:
                nType = 1;
                break;
            case 3:
                nType = 2;
                break;
            case 6:
                nType = 3;
                break;
            case 5:
                nType = 4;
                break;
            case 4:
                nType = 5;
                break;
            }
            if (nType < 0) continue;
            pStateBackupTbl[nType] = door.getDoorState();
            if (nType < 4) {
                DOOR_STORAGE* pSrc = (DOOR_STORAGE*)door._pStorageInfo;
                if (pSrc) {
                    STORAGE::_EventDoorInfo& info = pInfoBackupTbl[nType];
                    for (INT k = 0; k < 3; k++) info._nNormaNums[k] = pSrc->_nNormaNums[k];
                    info._nClearedNorma = pSrc->_nNextNorma;
                }
            }
        }
    }
}
void CSceneInfo::rollbackEventDoorState(BOOL bInitFloor) {
    USHORT nFloorIdx = getCurrentFloorIndex();
    if (nFloorIdx == 0xff) return;
    FloorInfo& floor = s_CurrentFloor; /* not in the ref: it got no register (all s-regs taken), so .mdebug has no entry */
    STORAGE::_EventDoorInfo* pInfoBackupTbl = _SceneInfoWork::mc()->_nEventDoorInfoTbl[nFloorIdx];
    BYTE* pStateBackupTbl = _SceneInfoWork::mc()->_nEventDoorStateTbl[nFloorIdx];
    for (USHORT i = 0; i < floor._nRoomCount; i++) {
        RoomInfo& room = floor._rooms[i];
        for (INT j = 0; j < 4; j++) {
            DoorInfo& door = room._doors[j];
            if (door._nDstRoom < 0) continue;
            DOOR_STAT stat;
            INT nType = -1;
            INT nDoorType = door._type;
            switch (nDoorType) {
            case 1:
                nType = 0;
                break;
            case 2:
                nType = 1;
                break;
            case 3:
                nType = 2;
                break;
            case 6:
                nType = 3;
                break;
            case 5:
                nType = 4;
                break;
            case 4:
                nType = 5;
                break;
            }
            if (nType < 0) continue;
            if (nType < 4) {
                DOOR_STORAGE* pDst = (DOOR_STORAGE*)door._pStorageInfo;
                if (pDst) {
                    STORAGE::_EventDoorInfo& info = pInfoBackupTbl[nType];
                    for (INT k = 0; k < 3; k++) pDst->_nNormaNums[k] = info._nNormaNums[k];
                    pDst->_nNextNorma = info._nClearedNorma;
                }
            }
            stat = (DOOR_STAT)pStateBackupTbl[nType];
            if (nType == 5 && stat != DOOR_STAT_UNAVAILABLE) continue;
            if (bInitFloor) {
                BOOL bResetNorma = FALSE;
                if (stat == DOOR_STAT_OPENED) {
                    if (door.isSpecialDoor() || door._type == 5) {
                        door.initNorma();
                        stat = DOOR_STAT_CLOSED;
                        if (door._type == 6) bResetNorma = TRUE;
                        else bResetNorma = FALSE;
                    }
                } else if (stat == DOOR_STAT_CLOSED) {
                    if (door.isSpecialDoor()) {
                        door.initNorma();
                        bResetNorma = TRUE;
                    }
                }
                if (bResetNorma) {
                    DOOR_STORAGE* pDst = (DOOR_STORAGE*)door._pStorageInfo;
                    if (pDst) {
                        if (!(door._pNormaSrcInfo)) AssertApp(__FILE__, 3249, "door._pNormaSrcInfo", 0);
                        RCNORMA::DoorNormaSrc& src = *(RCNORMA::DoorNormaSrc*)door._pNormaSrcInfo;
                        for (INT k = 0; k < 3; k++) pDst->_nNormaNums[k] = src._normas[k]._nNumber;
                    }
                }
            }
            door.setDoorState(stat);
        }
    }
}
void _setOpenFlagEventDoor(const DoorInfo& door) {
    USHORT nFloor = CSceneInfo::getCurrentFloorIndex();
    INT nEvIdx = door._type - 1;
    _SceneInfoWork::mc2()->_nEventDoorOpenTbl[nFloor][nEvIdx] = 1;
}
BYTE _getOpenFlagEventDoor(const DoorInfo& door) {
    USHORT nFloor = CSceneInfo::getCurrentFloorIndex();
    INT nEvIdx = door._type - 1;
    return _SceneInfoWork::mc2()->_nEventDoorOpenTbl[nFloor][nEvIdx];
}
BOOL RoomInfo::isCreated() const {
    if (!_pSrcInfo) return TRUE;
    ROOM_STORAGE info = *(ROOM_STORAGE*)_pStorageInfo;
    return info._nArea >= 0 && info._nScene >= 0;
}
BOOL RoomInfo::isFirstTimeAfterRC() const {
    ROOM_STORAGE* p = (ROOM_STORAGE*)_pStorageInfo;
    if (!p) return FALSE;
    return p->_bFirstTimeAfterRC != 0;
}
BOOL RoomInfo::hasEntered() const {
    ROOM_STORAGE* p = (ROOM_STORAGE*)_pStorageInfo;
    if (!p) return FALSE;
    return p->_bEntered != 0;
}
void RoomInfo::setScene(SHORT nArea, SHORT nScene) {
    ROOM_STORAGE* p = (ROOM_STORAGE*)_pStorageInfo;
    if (p) {
        p->_nArea = nArea;
        p->_nScene = nScene;
    }
}
void RoomInfo::setScene(SHORT nScene) {
    ROOM_STORAGE* p = (ROOM_STORAGE*)_pStorageInfo;
    if (p) p->_nScene = nScene;
}
void RoomInfo::resetScene() {
    ROOM_STORAGE* p = (ROOM_STORAGE*)_pStorageInfo;
    if (p) {
        p->_nArea = -1;
        p->_nScene = -1;
    }
}
SHORT RoomInfo::getSceneNum() const {
    ROOM_STORAGE* p = (ROOM_STORAGE*)_pStorageInfo;
    if (!p) return -1;
    return p->_nScene;
}
void RoomInfo::setCardInfo(BYTE nCardID, BYTE nCardNumber) {
    ROOM_STORAGE2* p = (ROOM_STORAGE2*)_pStorageInfo2;
    if (p) {
        p->_nCardNumber = nCardNumber;
        p->_nCardID = nCardID;
    }
}
BYTE RoomInfo::getCardNumber() const {
    ROOM_STORAGE2* p = (ROOM_STORAGE2*)_pStorageInfo2;
    BYTE n = 0;
    if (p) n = p->_nCardNumber;
    return n;
}
BYTE RoomInfo::getCardID() const { return CSceneInfo::getCardIDFromSceneNum(getSceneNum()); }
BYTE RoomInfo::getRoomNameCardID() const {
    WORLD_ID w = CSceneInfo::getCurrentWorldID(TRUE);
    if (w == 10) return 0xdb;
    if (_pSrcInfo && _pSrcInfo->_nRoomType == 7) return 0xd8;
    if (w != 0) {
        switch (getSceneNum()) {
        case 1: return 0xd8;
        case 2: return 0xd9;
        }
    }
    ROOM_STORAGE2* pInfo = (ROOM_STORAGE2*)_pStorageInfo2;
    if (!pInfo) return 0;
    return pInfo->_nCardID;
}
DOOR_STAT DoorInfo::getDoorState() const {
    DOOR_STORAGE* p = (DOOR_STORAGE*)_pStorageInfo;
    DOOR_STAT n = DOOR_STAT_CLOSED;
    if (p) n = (DOOR_STAT)p->_nStat;
    return n;
}
BOOL DoorInfo::setDoorState(DOOR_STAT nStat) {
    DOOR_STORAGE* p = (DOOR_STORAGE*)_pStorageInfo;
    if (!p) return FALSE;
    p->_nStat = nStat;
    return TRUE;
}
BOOL DoorInfo::isSpecialDoor() const {
    if (getNormaKeyCardID()) return TRUE;
    return isEventDoor();
}
BOOL DoorInfo::canSelectWorld() const {
    if (CSceneInfo::hasReqChangeScene()) return FALSE;
    if (_type != DOOR_TYPE_SELECT_WORLD) return FALSE;
    return getDoorState() == 1;
}
BOOL DoorInfo::canCreateRoom() const {
    switch (_type) {
    case DOOR_TYPE_OPEN_AFTER_EVENT:
    case DOOR_TYPE_WORLD_ENTRANCE:
    case DOOR_TYPE_SELECT_WORLD:
    case DOOR_TYPE_FLOOR_CHANGE:
    case DOOR_TYPE_TO_LAST:
    case _DOOR_TYPE_DEBUG:
        return FALSE;
    }
    if (CSceneInfo::hasReqChangeScene()) return FALSE;
    if (CSceneInfo::hasReqEnterBattle()) return FALSE;
    if (CSceneInfo::getCurrentWorldID(TRUE) == 10) return FALSE;
    DOOR_STAT s = getDoorState();
    switch (_type) {
    case DOOR_TYPE_TO_EVENT1:
    case DOOR_TYPE_TO_EVENT2:
    case DOOR_TYPE_TO_EVENT3:
    case DOOR_TYPE_TO_EXTRA:
        return s == DOOR_STAT_CLOSED;
    }
    return s == DOOR_STAT_CLOSED || s == DOOR_STAT_OPENED;
}
INT DoorInfo::getNormaKeyCardID() const {
    if (isEventDoor() && _getOpenFlagEventDoor(*this)) return 0;
    RCNORMA::DoorNormaSrc* p = (RCNORMA::DoorNormaSrc*)_pNormaSrcInfo;
    if (!p) return 0;
    return p->_nDoorType + 0xba;
}
USHORT DoorInfo::getNormaCount() const {
    if (!getNormaKeyCardID()) return 1;
    return ((RCNORMA::DoorNormaSrc*)_pNormaSrcInfo)->_nCount + 1;
}
BOOL DoorInfo::isValidNormaIndex(USHORT nIndex) const {
    if (nIndex < getNormaCount()) return TRUE;
    TRACE("\x83\x8b\x81[\x83\x80\x83N\x83\x8a\x83G\x83" "C\x83V\x83\x87\x83\x93\x83m\x83\x8b\x83}\x90\x94\x82\xc9\x91\xce\x82\xb7\x82\xe9 INDEX \x92l\x82\xaa\x95s\x90\xb3\x82\xc5\x82\xb7\n");
    return FALSE;
}
RCNORMA_COLOR DoorInfo::getNormaColor(USHORT nIndex) const {
    INT nKey = getNormaKeyCardID();
    if (nKey && nIndex + 1 == getNormaCount()) return (RCNORMA_COLOR)0;
    BOOL bValid = isValidNormaIndex(nIndex) && _pNormaSrcInfo && nKey; // MATCH: one && value (bnel + sltu) and a final ternary (first return stays a stub)
    return bValid ? (RCNORMA_COLOR)((RCNORMA::DoorNormaSrc*)_pNormaSrcInfo)->_normas[nIndex]._nColor : (RCNORMA_COLOR)0;
}
USHORT DoorInfo::getNormaNumber(USHORT nIndex) const {
    if (getNormaKeyCardID() && nIndex + 1 == getNormaCount()) return 0;
    return (isValidNormaIndex(nIndex) && _pStorageInfo) ? ((DOOR_STORAGE*)_pStorageInfo)->_nNormaNums[nIndex] : 0; // MATCH: a final ternary keeps the first return 0 a separate stub (its (use v0) stops the cross-jump)
}
RCNORMA_OP DoorInfo::getNormaOperation(USHORT nIndex) const {
    INT nKey = getNormaKeyCardID();
    if (nKey && nIndex + 1 == getNormaCount()) return (RCNORMA_OP)0;
    BOOL bValid = isValidNormaIndex(nIndex) && _pNormaSrcInfo && nKey; // MATCH: as getNormaColor
    if (bValid) return (RCNORMA_OP)((RCNORMA::DoorNormaSrc*)_pNormaSrcInfo)->_normas[nIndex]._nOp;
    return !getNormaNumber(nIndex) ? (RCNORMA_OP)3 : (RCNORMA_OP)2;
}
BOOL DoorInfo::isNormaCardUsed() const {
    DOOR_STORAGE* p = (DOOR_STORAGE*)_pStorageInfo;
    BOOL b = FALSE;
    if (p) b = p->_nFirstCardID != 0;
    return b;
}
void DoorInfo::setFirstUsedCardInfo(BYTE nCardID, BYTE nCardNum) {
    DOOR_STORAGE* p = (DOOR_STORAGE*)_pStorageInfo;
    if (p) {
        p->_nFirstCardID = nCardID;
        p->_nFirstCardNum = nCardNum;
    }
}
BYTE DoorInfo::getFirstUsedCardIDForNorma() const {
    DOOR_STORAGE* p = (DOOR_STORAGE*)_pStorageInfo;
    BYTE n = 0;
    if (p) n = p->_nFirstCardID;
    return n;
}
BYTE DoorInfo::getFirstUsedCardNumForNorma() const {
    DOOR_STORAGE* p = (DOOR_STORAGE*)_pStorageInfo;
    BYTE n = 0xff;
    if (p) n = p->_nFirstCardNum;
    return n;
}
BOOL DoorInfo::clearOneNorma() {
    DOOR_STORAGE* p = (DOOR_STORAGE*)_pStorageInfo;
    if (p) {
        if (!isValidNormaIndex(p->_nNextNorma)) return FALSE;
        p->_nNextNorma++;
        return isNormaCleared();
    }
    return FALSE;
}
USHORT DoorInfo::getNextNormaIndex() const {
    DOOR_STORAGE* p = (DOOR_STORAGE*)_pStorageInfo;
    USHORT n = 3;
    if (p) n = p->_nNextNorma;
    return n;
}
BOOL DoorInfo::isNextNormaKeyCard() const {
    if (!getNormaKeyCardID()) return FALSE;
    DOOR_STORAGE* p = (DOOR_STORAGE*)_pStorageInfo;
    BOOL b;
    if (p) b = p->_nNextNorma == ((RCNORMA::DoorNormaSrc*)_pNormaSrcInfo)->_nCount;
    else b = FALSE;
    return b;
}
BOOL DoorInfo::isNormaCleared() const {
    DOOR_STORAGE* p = (DOOR_STORAGE*)_pStorageInfo;
    BOOL b = FALSE;
    if (p) b = getNormaCount() == p->_nNextNorma;
    return b;
}
DOOR_RC_RESULT DoorInfo::useNormaCard(const CARD& card) {
    if (isNormaCleared()) return DOOR_RC_RESULT_NOP;
    BOOL bClear = FALSE;
    BOOL bUsed = FALSE;
    if (isNextNormaKeyCard()) {
        if (getNormaKeyCardID() != card.m_nKind) return DOOR_RC_RESULT_INVALID;
        bClear = TRUE;
    } else {
        if (card.m_nCategory0 != 9) return DOOR_RC_RESULT_INVALID;
        USHORT nIdx = getNextNormaIndex();
        RCNORMA_COLOR colNorma = getNormaColor(nIdx);
        if (card.m_nKind == 0xb9) colNorma = RCNORMA_COLOR_NONE;
        if (colNorma != RCNORMA_COLOR_NONE) {
            RCNORMA_COLOR col = RCNORMA_COLOR_NONE;
            switch (card.m_nCategory1) {
            case 9:
                col = RCNORMA_COLOR_RED;
                break;
            case 10:
                col = RCNORMA_COLOR_GREEN;
                break;
            case 11:
                col = RCNORMA_COLOR_BLUE;
                break;
            }
            if (colNorma != col) return DOOR_RC_RESULT_INVALID;
        }
        RCNORMA_OP op = getNormaOperation(nIdx);
        if (card.m_nKind == 0xb9) op = RCNORMA_OP_NONE;
        USHORT nNormaNum = getNormaNumber(nIdx);
        USHORT num = card.m_nNumber;
        switch (op) {
        case RCNORMA_OP_NONE:
            bClear = TRUE;
            bUsed = TRUE;
            break;
        case RCNORMA_OP_EQ_LESS:
            bClear = bUsed = num <= nNormaNum;
            break;
        case RCNORMA_OP_EQUAL:
            bClear = bUsed = nNormaNum == num;
            break;
        case RCNORMA_OP_EQ_GREATER:
            bClear = num >= nNormaNum;
            if (!bClear && num == 0) bClear = getNormaKeyCardID() == 0;
            bUsed = bClear;
            break;
        case RCNORMA_OP_TOTAL:
            if (num == 0) return DOOR_RC_RESULT_INVALID;
            nNormaNum = nNormaNum >= num ? (USHORT)(nNormaNum - num) : 0;
            ((DOOR_STORAGE*)_pStorageInfo)->_nNormaNums[nIdx] = nNormaNum;
            bClear = nNormaNum == 0;
            bUsed = TRUE;
            break;
        }
    }
    if (bUsed && !isNormaCardUsed()) setFirstUsedCardInfo(card.m_nKind, card.m_nNumber);
    if (bClear) return clearOneNorma() ? DOOR_RC_RESULT_CLEAR_ALL : DOOR_RC_RESULT_CLEAR_ONE;
    return bUsed ? DOOR_RC_RESULT_USED : DOOR_RC_RESULT_INVALID;
}
void DoorInfo::initNorma() {
    DOOR_STORAGE* p = (DOOR_STORAGE*)_pStorageInfo;
    if (p) {
        for (INT i = 0; i < 3; i++) p->_nNormaNums[i] = 0;
        p->_nNextNorma = 0;
        p->_nFirstCardID = 0;
        p->_nFirstCardNum = 0xff;
    }
}
void DoorInfo::resetNorma() {
    if (getDoorState() != DOOR_STAT_OPENED) return;
    if (isSpecialDoor()) return;
    initNorma();
    RoomInfo* pCurrentRoom = CSceneInfo::getRoomInfo(-1);
    if (!pCurrentRoom) return;
    RoomInfo* pDstRoom = CSceneInfo::getRoomInfo(_nDstRoom);
    if (!pDstRoom || !pDstRoom->_pSrcInfo) return;
    switch (pDstRoom->_pSrcInfo->_nRoomType) {
    case 0:
    case 6:
    case 8: {
        DOOR_STORAGE* pInfo = (DOOR_STORAGE*)_pStorageInfo;
        if (!pInfo) return;
        USHORT n = pCurrentRoom->getCardNumber() + 1;
        pInfo->_nNormaNums[0] = n < 10 ? n : 0;
        break;
    }
    }
}
SHORT CSceneInfo::getSceneNumFromCardID(BYTE nCardID) {
    BYTE n = 0;
    _CardScene* p = s_CardSceneTbl;
    for (INT i = 0; i < 26; i++, p++) {
        if (p->_nCardID == nCardID) {
            n = p->_nScene;
            break;
        }
    }
    if (!n) TRACE_COL((TtyColor)0x1f, "CSceneInfo::getSceneNumFromCardID( %d ) ... invalid Card-ID\n", nCardID);
    return n;
}
BYTE CSceneInfo::getCardIDFromSceneNum(SHORT nScene) {
    BYTE nCardID = 0;
    if (nScene >= 1 && nScene <= 0x7f) {
        BYTE c = nScene;
        _CardScene* p = s_CardSceneTbl;
        for (INT i = 0; i < 26; i++, p++) {
            if (p->_nScene == c) {
                nCardID = p->_nCardID;
                break;
            }
        }
    }
    if (!nCardID) TRACE_COL(_TTY_RED, "CSceneInfo::getCardIDFromSceneNum( %d ) ... invalid Scene-Num\n", nScene);
    return nCardID;
}
INT CSceneInfo::getConstWorldRsrcID(INT nWorld, BOOL bSora) {
    if ((UINT)nWorld < 14) return !bSora ? nWorld + 1100 : nWorld + 100;
    return 0;
}
INT CSceneInfo::getInconstWorldRsrcID(INT nWorld, BOOL bSora) {
    if ((UINT)nWorld < 14) return !bSora ? nWorld + 1200 : nWorld + 200;
    return 0;
}
// SOURCE DATA: .data .rodata .bss
