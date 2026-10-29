#include "khtypes.h"
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
enum NAME_CASE { NAME_CASE_SENSITIVE = 0 }; /* PARTIAL: stand-in for System/RsrcDB NAME_CASE: gated by RSRCDATA_H_FULL (strings: System/RsrcData.h(156)) */
class CRsrcDB { /* PARTIAL: stand-in for System/RsrcDB CRsrcDB: strings: System/RsrcData.h(156) (owner partial; RsrcDB.h needs the full RsrcData.h classes) */
public:
    static BOOL isSceneAttachMode();
    static UINT getSceneAttachNum();
    static CRsrcData* getRsrc(INT nRsrcType, const CHAR* pszName, NAME_CASE nc);
    static void* getRsrcData(const CRsrcData& data);
    static void removeSceneAttachedData(UINT nNum);
    static void removeRsrc(INT nID);
    static void setSceneAttachMode(UINT nNum);
};

/* AREA_SCENE without its constructors (built in a register; PARTIAL) */
union _AREA_SCENE_REG {
    struct {
        SHORT _nScene;
        SHORT _nArea;
    };
    INT _nAreaScene;
};

enum GAMEMODE { GAMEMODE_FIELD = 0, GAMEMODE_BATTLE = 1 };

struct FloorLayoutDataHeader { // 0x10
    CHAR _cSig[4];       /* 0x0 */
    USHORT _nVersion;    /* 0x4 */
    USHORT nCount;       /* 0x6 */
    INT* _pFloorOfsTbl;  /* 0x8 */
    void* _pFloorData;   /* 0xc */
};
struct NormaDataHeader { // 0x7c
    CHAR _cSig[4];                     /* 0x04 */
    USHORT _nVersion;                  /* 0x04 */
    USHORT _nFloorCount;               /* 0x06 */
    RCNORMA::FloorNormaSrc _FloorSrc[1]; /* 0x08 */
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
struct EventRoomData { // 0x18
    CHAR _cSig[4];             /* 0x00 */
    USHORT _nVersion;          /* 0x04 */
    USHORT _nWorldCount;       /* 0x06 */
    USHORT _nSceneTbl[1][8];   /* 0x08 */
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

/* The file's statics; the data still comes from the original (no SOURCE DATA yet) */
namespace {
extern INT s_nReqChangeScene;
extern GAMEMODE s_nReqEnterBattle;
extern INT s_nReqExitBattle;
extern GAMEMODE s_nGameMode;
extern INT s_nAtelSrcID;
extern FloorLayoutDataHeader* s_pFloorFile_Sora;
extern FloorLayoutDataHeader* s_pFloorFile_Riku;
extern EventRoomData* s_pEventRoomFile_Sora;
extern EventRoomData* s_pEventRoomFile_Riku;
extern NormaDataHeader* s_pFloorNormaFile_Sora;
extern NormaDataHeader* s_pFloorNormaFile_Riku;
extern AREA_SCENE s_NextAreaScene;
extern SHORT s_nNextBattleScene;
extern SHORT s_nCurrentBattleScene;
extern SHORT s_nCurrentBattleRsrc;
extern SHORT s_nCurrentFieldScene;
extern SHORT s_nCurrentFloorNumber;
extern BYTE s_nNextRoomID;
extern BYTE s_nNextFloorIdx;
extern DOOR_ID s_nNextDoorID;
extern INT s_nCurrentSceneCacheID;
extern INT s_nMapCacheField;
extern INT s_nMapCacheBattle;
extern _SceneInfo s_SceneCacheTbl[7];
extern FloorInfo s_CurrentFloor;
extern RoomInfo s_roomExcept;
extern INT s_nAvailableSceneCache;
extern _SceneInfo s_GlobalScene;
}
extern _CardScene s_CardSceneTbl[26];
extern WorldSelTable s_pWorldTableSora[16];
extern WorldSelTable s_pWorldTableRiku[12];
extern "C" char D_00484980[]; /* __FILE__ */
extern "C" char D_00484A98[];
extern "C" char D_00484D90[];
extern "C" char D_00484EC8[];
extern "C" char D_00484F28[];
extern "C" char D_00484F68[];
extern "C" char D_004849C8[];
extern "C" char D_00484C18[];
extern "C" char D_00484C20[];
extern "C" char D_00484A10[];
extern "C" char D_00484C68[];
extern "C" char D_00484C88[];
extern "C" char D_00484B48[];
extern "C" char D_00484B98[];
extern "C" char D_00484AD0[];
extern "C" char D_00484AF8[];
extern "C" char D_00484B18[];
extern "C" char D_004849A8[];
extern "C" char D_00484900[];
extern "C" char D_00484910[];
extern "C" char D_00484920[];
extern "C" char D_00484930[];
extern "C" char D_00484940[];
extern "C" char D_00484950[];
extern "C" char D_00484BE0[];
extern "C" char D_00484CD8[];
namespace PLM {
extern BOOL g_bIsLastSceneBattle;
}
extern "C" char D_00484E28[];
extern "C" char D_00484CE8[];
extern "C" char D_00484D50[];
extern "C" char D_00484E08[];
extern "C" char D_00484D20[];
extern "C" char D_00484D70[];
extern "C" char D_00484E38[];
extern "C" char D_00484DC8[];
extern "C" char D_00484DD0[];
extern "C" char D_00484DD8[];
extern "C" int sprintf(char*, const char*, ...);

/* Parked: ~110 lines - the first run of stores: the original reloads m_pMCWork before each store (the *(T*)& scalar
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
        d._shape = 0;
        d._type = _DOOR_TYPE_DEBUG;
        d._nDstRoom = 0;
        d._pStorageInfo = NULL;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", clear__10CSceneInfo);
#endif
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _getEventRoomSceneIndex__Fs);
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
INT CSceneInfo::getFloorDataCount(BOOL bSora) {
    FloorLayoutDataHeader* p = bSora ? s_pFloorFile_Sora : s_pFloorFile_Riku;
    INT n = 0;
    if (p) n = p->nCount;
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
        for (USHORT i = 0; i < p->nCount; i++) {
            if (*(SHORT*)((BYTE*)p->_pFloorData + p->_pFloorOfsTbl[i]) == nFloor) return i;
        }
    }
    return 0;
}
WorldSelTable* CSceneInfo::getSelectableWorldTable(INT nFloorIndex, BOOL bSora) {
    FloorLayoutDataHeader* p = bSora ? s_pFloorFile_Sora : s_pFloorFile_Riku;
    if (p && nFloorIndex >= 0 && nFloorIndex < p->nCount) {
        WorldSelTable* pTbl = bSora ? s_pWorldTableSora : s_pWorldTableRiku;
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
    if (!(pFloorSrc && pFloorNormaSrc)) AssertApp(D_00484980, 586, D_004849A8, 0);
    _SceneInfoWork::mc()->_nFloorIdx = nFloorIdx;
    if (!(nIdx < 16)) AssertApp(D_00484980, 590, D_004849C8, 0);
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
/* Parked: 21 lines - the block after getFloorNumber: the original computes the s_CurrentFloor address and the
   clearEventRoomTbl loop pointer after the s_nCurrentFloorNumber store (lui order) with other registers; the loop
   body matches with the shared pSrc pointer, the tail with the *_nEventDoorStateTbl[i] form. Statement orders and
   5 min of permuter (7k candidates) gave nothing. */
#if 0
void CSceneInfo::initFloor() {
    BOOL bSora = CGame::isSoraGame();
    INT nFloorIdx = _SceneInfoWork::mc()->_nFloorIdx;
    INT nIdx = nFloorIdx;
    WORLD_ID world = getWorldID(nFloorIdx);
    if (world == 10) nIdx = getFloorIndex(100, TRUE);
    FLOOR::FloorLayout* pFloorSrc = getFloorDataSrc(nIdx, bSora);
    RCNORMA::FloorNormaSrc* pFloorNormaSrc = getFloorNormaDataSrc(nIdx, bSora);
    if (!(pFloorSrc && pFloorNormaSrc)) AssertApp(D_00484980, 691, D_004849A8, 0);
    s_nCurrentFloorNumber = getFloorNumber(nFloorIdx, bSora);
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
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", initFloor__10CSceneInfo);
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _initDoor__FR8DoorInfoPCQ25FLOOR11DoorSrcInfo7DOOR_IDPCQ25FLOOR11FloorLayoutPCQ27RCNORMA13FloorNormaSrcRQ37STORAGE7MCWork29_DoorInfoRC8RoomInfoi);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _setWorldEntrance__F8WORLD_IDssi);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", makeRoom__10CSceneInfoR8DoorInfoi);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _checkAvailableDoorID__FRC8DoorInfo);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", setEventRoomSceneNum__10CSceneInfoUis);
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
    if (!((0 < world) && (14 > world))) AssertApp(D_00484980, 1410, D_00484A98, 0);
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
            TRACE_COL(_TTY_RED, D_00484AD0, nRoomID);
            TRACE_COL(_TTY_RED, !pRoom ? D_00484AF8 : D_00484B18);
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
            TRACE_COL(_TTY_RED, D_00484B48);
            return FALSE;
        }
    }
    RoomInfo* pRoom;
    if (nScene > 0) pRoom = setEventRoomSceneNum(nEventRoomType, nScene);
    else pRoom = getEventRoomInfo(nEventRoomType);
    if (!pRoom || pRoom->isExceptRoom()) {
        TRACE_COL(_TTY_RED, D_00484B98);
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
void CSceneInfo::reqChangeGoalRoom() { reqChangeEventRoom(4, (DOOR_ID)4, -1); }
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
        TRACE_COL(_TTY_RED, D_00484BE0, nArea, nScene);
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
    if (!(pDoor)) AssertApp(D_00484980, 1632, D_00484C18, 0);
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
        TRACE(D_00484C20, pDoor->_id, pDoor->_type, stat, pDoor->_nDstRoom);
        RoomInfo* pDstRoom = getRoomInfo(pDoor->_nDstRoom);
        if (!(pDstRoom)) AssertApp(D_00484980, 1657, D_00484A10, 0);
        if (pDoor->_shape == 5 && (USHORT)pDstRoom->getSceneNum() == 2 && !pDstRoom->hasEntered()) {
            makeRoom(*pDstRoom, _getDstDoorID(door), 2, getCardIDFromSceneNum(2), CGameWork::m_pMCWork->_nNextRCNormaNum);
        } else if (!pDstRoom->isCreated() || (pDstRoom->isFixSceneRoom() && !pDstRoom->hasEntered())) {
            if (!pDstRoom->isCreated()) TRACE_COL(_TTY_RED, D_00484C68);
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
                if (pInfo) TRACE(D_00484C88, pInfo->_doors[0].getDoorState(), pInfo->_doors[1].getDoorState(), pInfo->_doors[2].getDoorState(), pInfo->_doors[3].getDoorState());
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
INT CSceneInfo::hasReqEnterBattle() { return s_nReqEnterBattle; }
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
INT CSceneInfo::getCurrentWorldID(BOOL bChkEntrance) {
    if (bChkEntrance && getCurrentAreaNum() == 0) return 0;
    USHORT nIdx = getCurrentFloorIndex();
    BOOL bSora = CGame::isSoraGame();
    FloorLayoutDataHeader* pHeader = _isValidFloorIdx(nIdx, bSora);
    if (pHeader) {
        INT n = !bSora;
        if (nIdx < pHeader->nCount) return _SceneInfoWork::mc2()->_nWorldOnFloor[n][nIdx];
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
    if (nFloorIndex >= p->nCount) return (WORLD_ID)0;
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", setEventDoorUnavailable__10CSceneInfoii);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getEnteredDoorID__10CSceneInfo);
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
    if (!(pRoomInfo)) AssertApp(D_00484980, 2540, D_00484CD8, 0);
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
UINT CSceneInfo::getCurrentSceneCacheID() { return s_nCurrentSceneCacheID; }
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
        TRACE(D_00484CE8);
        return -1;
    }
    for (INT i = 0; i < 7; i++) {
        if (s_SceneCacheTbl[i]._as._nAreaScene < 0) {
            s_SceneCacheTbl[i]._as._nAreaScene = as._nAreaScene;
            if (!(7 >= s_nAvailableSceneCache)) AssertApp(D_00484980, 2622, D_00484D20, 0);
            s_nAvailableSceneCache--;
            if (!(0 <= s_nAvailableSceneCache)) AssertApp(D_00484980, 2624, D_00484D50, 0);
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
            if (!(0 < s_nAvailableSceneCache)) AssertApp(D_00484980, 2643, D_00484D70, 0);
            s_nAvailableSceneCache++;
            if (!(7 >= s_nAvailableSceneCache)) AssertApp(D_00484980, 2645, D_00484D20, 0);
            return i;
        }
    }
    return -1;
}
BOOL CSceneInfo::removeFromSceneCacheWithRsrc(const AREA_SCENE& as) {
    INT nID = removeFromSceneCache(as);
    if (nID < 0) return FALSE;
    TRACE_USER_COL((USER)4, (TtyColor)0x22, D_00484D90, as._nArea, as._nScene);
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
        sprintf(szTmp, D_00484DC8, info._as._nArea, info._as._nScene);
    } else {
        s_GlobalScene._nSeRangeRatio = nRatio;
        pTmp = D_00484DD0;
    }
    TRACE_COL(_TTY_RED, D_00484DD8, pTmp, nRatio * 10);
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
void* CSceneInfo::getMapRsrcCache(INT nSlot) {
    switch (nSlot) {
    case 1: return (void*)s_nMapCacheField;
    case 2: return (void*)s_nMapCacheBattle;
    }
    return NULL;
}
void CSceneInfo::updateMapRsrcCache(INT nMapRsrcGrp, INT nSlot) {
    switch (nSlot) {
    case 0:
        {
            INT n = s_nMapCacheField;
            if (n && nMapRsrcGrp != n) {
                TRACE_USER_COL(TGOTO, _TTY_BLUE, D_00484E08, n);
                CRsrcDB::removeRsrc(n);
            }
        }
        s_nMapCacheField = nMapRsrcGrp;
    case 2:
        {
            INT n = s_nMapCacheBattle;
            if (n && nMapRsrcGrp != n) {
                TRACE_USER_COL(TGOTO, _TTY_BLUE, D_00484E08, n);
                CRsrcDB::removeRsrc(n);
            }
        }
        s_nMapCacheBattle = nMapRsrcGrp;
        break;
    case 1:
        {
            INT n = s_nMapCacheField;
            if (n && nMapRsrcGrp != n) {
                TRACE_USER_COL(TGOTO, _TTY_BLUE, D_00484E08, n);
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
            TRACE_USER_COL(TGOTO, _TTY_BLUE, D_00484E08, n);
            CRsrcDB::removeRsrc(n);
            s_nMapCacheField = 0;
        }
        n = s_nMapCacheBattle;
        if (n) {
            TRACE_USER_COL(TGOTO, _TTY_BLUE, D_00484E08, n);
            CRsrcDB::removeRsrc(n);
            s_nMapCacheBattle = 0;
        }
        break;
    }
    case 1:
        {
            INT n = s_nMapCacheField;
            if (n && s_nMapCacheBattle != n) {
                TRACE_USER_COL(TGOTO, _TTY_BLUE, D_00484E08, n);
                CRsrcDB::removeRsrc(n);
            }
        }
        s_nMapCacheField = 0;
        break;
    case 2:
        {
            INT n = s_nMapCacheBattle;
            if (n && n != s_nMapCacheField) {
                TRACE_USER_COL(TGOTO, _TTY_BLUE, D_00484E08, n);
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
    if (pHeader) pRet = nIdx < pHeader->nCount ? pHeader : NULL;
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
    if (!(pGoalDoor)) AssertApp(D_00484980, 3094, D_00484E28, 0);
    pGoalDoor->setDoorState(DOOR_STAT_OPENED);
    return (DOOR_ID)nDoor;
}
/* Parked: 2 lines - the nDoor copy (move s0,v0) is scheduled before the li a1,2 of setEventRoomSceneNum;
   the original issues li a1 first. nDoor declaration forms/types did not change it. */
#if 0
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
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", openGoalDoor__10CSceneInfo);
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", backupEventDoorState__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", rollbackEventDoorState__10CSceneInfoi);
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
    WORLD_ID w = (WORLD_ID)CSceneInfo::getCurrentWorldID(TRUE);
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
    TRACE(D_00484EC8);
    return FALSE;
}
/* Parked: 15 lines - the first `return 0` (key && idx+1 == count) stays an inline `b end` block after a bne in the original, ours branches straight to the epilogue; the bValid test comes out as movn here, as bnel + sltu there. */
#if 0
RCNORMA_COLOR DoorInfo::getNormaColor(USHORT nIndex) const {
    INT nKey = getNormaKeyCardID();
    if (nKey && nIndex + 1 == getNormaCount()) return (RCNORMA_COLOR)0;
    BOOL bValid = FALSE;
    if (isValidNormaIndex(nIndex) && _pNormaSrcInfo) bValid = nKey != 0;
    if (bValid) return (RCNORMA_COLOR)((RCNORMA::DoorNormaSrc*)_pNormaSrcInfo)->_normas[nIndex]._nColor;
    return (RCNORMA_COLOR)0;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getNormaColor__C8DoorInfoUs);
#endif
/* Parked: 3 lines - the first `return 0` stays an inline `b end` block after a bne in the original; ours inverts it into a beq to the epilogue. ~12 structural variants (nested, else-if, result var, !(..) &&) give the same; the permuter cannot find const methods. */
#if 0
USHORT DoorInfo::getNormaNumber(USHORT nIndex) const {
    if (getNormaKeyCardID() && nIndex + 1 == getNormaCount()) return 0;
    if (isValidNormaIndex(nIndex)) {
        DOOR_STORAGE* p = (DOOR_STORAGE*)_pStorageInfo;
        return p ? p->_nNormaNums[nIndex] : 0;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getNormaNumber__C8DoorInfoUs);
#endif
/* Parked: first `return 0` and bValid as in getNormaColor (same shape, not iterated further). */
#if 0
RCNORMA_OP DoorInfo::getNormaOperation(USHORT nIndex) const {
    INT nKey = getNormaKeyCardID();
    if (nKey && nIndex + 1 == getNormaCount()) return (RCNORMA_OP)0;
    BOOL bValid = FALSE;
    if (isValidNormaIndex(nIndex) && _pNormaSrcInfo) bValid = nKey != 0;
    if (bValid) return (RCNORMA_OP)((RCNORMA::DoorNormaSrc*)_pNormaSrcInfo)->_normas[nIndex]._nOp;
    return getNormaNumber(nIndex) ? (RCNORMA_OP)2 : (RCNORMA_OP)3;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getNormaOperation__C8DoorInfoUs);
#endif
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", useNormaCard__8DoorInfoRC4CARD);
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
    if (!n) TRACE_COL((TtyColor)0x1f, D_00484F28, nCardID);
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
    if (!nCardID) TRACE_COL(_TTY_RED, D_00484F68, nScene);
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
