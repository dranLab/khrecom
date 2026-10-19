#include "khtypes.h"
#include "Game/SceneInfo.h"
#include "Game/GameWork.h"
#include "Game/Game.h"
#include "System/Rcf.h"
#include "System/Debug.h"
#include "include_asm.h"

/* PARTIAL: System/RsrcDB.h needs the full RsrcData.h classes */
class CRsrcData;
enum NAME_CASE { NAME_CASE_SENSITIVE = 0 };
class CRsrcDB {
public:
    static BOOL isSceneAttachMode();
    static UINT getSceneAttachNum();
    static CRsrcData* getRsrc(INT nRsrcType, const CHAR* pszName, NAME_CASE nc);
    static void* getRsrcData(const CRsrcData& data);
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
struct EventRoomData;

/* RoomInfo::_pStorageInfo, _pStorageInfo2 and DoorInfo::_pStorageInfo (names unknown) */
struct ROOM_STORAGE {
    SHORT _nArea;
    SHORT _nScene;
    BYTE _bFirstTimeAfterRC;
    BYTE _bEntered;
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

struct _SceneInfoWork {
    static STORAGE::MCWork* mc() { return CGameWork::m_pMCWork; }
    static STORAGE::MCWork2* mc2() { return CGameWork::m_pMCWork2; }
};

void _setGoalRoomScene(WORLD_ID w, USHORT nScene);
USHORT _getGoalRoomScene(WORLD_ID w);
void _clearMapRsrcCache();

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
extern SBYTE s_nNextRoomID;
extern SBYTE s_nNextFloorIdx;
extern DOOR_ID s_nNextDoorID;
extern INT s_nCurrentSceneCacheID;
extern INT s_nMapCacheField;
extern INT s_nMapCacheBattle;
extern _SceneInfo s_SceneCacheTbl[7];
extern INT s_nAvailableSceneCache;
extern _SceneInfo s_GlobalScene;
}
extern "C" char D_00484EC8[];

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", clear__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", initNewGame__10CSceneInfoi);
void CSceneInfo::dispose() {
    s_pFloorFile_Sora = NULL;
    s_pFloorFile_Riku = NULL;
    s_pEventRoomFile_Sora = NULL;
    s_pEventRoomFile_Riku = NULL;
    s_pFloorNormaFile_Sora = NULL;
    s_pFloorNormaFile_Riku = NULL;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _getEventRoomSceneIndex__Fs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _initEntranceSceneTbl__Fi);
void _setGoalRoomScene(WORLD_ID w, USHORT nScene) {
    if ((UINT)(w - 1) < 13) _SceneInfoWork::mc()->_GoalRoomSceneTbl[w - 1] = nScene;
}
USHORT _getGoalRoomScene(WORLD_ID w) {
    if ((UINT)(w - 1) >= 13) return 2;
    return _SceneInfoWork::mc()->_GoalRoomSceneTbl[w - 1];
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _initGoalRoomSceneTbl__Fi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _initAllEventDoorState__Fi);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getFloorIndex__10CSceneInfosi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getSelectableWorldTable__10CSceneInfoii);
EventRoomData* CSceneInfo::getEventRoomData(BOOL bSora) { return bSora ? s_pEventRoomFile_Sora : s_pEventRoomFile_Riku; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getEventRoomSceneNum__10CSceneInfo8WORLD_IDRCQ25FLOOR11RoomSrcInfoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", showAllHiddenDoorsOfCurrentFloor__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", initFloor__10CSceneInfoi8WORLD_ID);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", initFloor__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _initDoor__FR8DoorInfoPCQ25FLOOR11DoorSrcInfo7DOOR_IDPCQ25FLOOR11FloorLayoutPCQ27RCNORMA13FloorNormaSrcRQ37STORAGE7MCWork29_DoorInfoRC8RoomInfoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _initRoomInfo__FR8RoomInfoRQ37STORAGE6MCWork9_RoomInfoRQ37STORAGE7MCWork210_RoomInfo2PCQ25FLOOR11RoomSrcInfoPCQ25FLOOR11FloorLayoutPCQ27RCNORMA13FloorNormaSrci);
DOOR_ID _getDstDoorID(DOOR_ID door) {
    switch (door) {
    case 1: return (DOOR_ID)3;
    case 2: return (DOOR_ID)0;
    case 3: return (DOOR_ID)1;
    }
    return (DOOR_ID)2;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _setWorldEntrance__F8WORLD_IDssi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", makeRoom__10CSceneInfoRC8RoomInfo7DOOR_IDsUcUc);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _checkAvailableDoorID__FRC8DoorInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _checkAvailableDoorID__Fi7DOOR_ID);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", setEventRoomSceneNum__10CSceneInfoUis);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", reqChangeFloor__10CSceneInfosi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", reqEnterWorld__10CSceneInfo8WORLD_ID);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", reqChangeRoom__10CSceneInfoi7DOOR_IDs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", reqChangeEventRoom__10CSceneInfoUi7DOOR_IDs);
void CSceneInfo::reqChangeGoalRoom() { reqChangeEventRoom(4, (DOOR_ID)4, -1); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", reqChangeBackupRoom__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", reqEnterDoor__10CSceneInfo7DOOR_ID);
void _acceptReqEnterBattle() {
    s_nReqEnterBattle = CSceneInfo::getCurrentWorldID(TRUE) != 10 ? GAMEMODE_BATTLE : (GAMEMODE)2;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", reqEnterBattle__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", reqEnterBattle__10CSceneInfos);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", reqExitBattle__10CSceneInfo);
INT CSceneInfo::hasReqEnterBattle() { return s_nReqEnterBattle; }
BOOL CSceneInfo::hasReqExitBattle() { return s_nReqExitBattle != 0; }
BOOL CSceneInfo::hasReqRetryBattle() { return s_nReqExitBattle == 2; }
void CSceneInfo::cancelBattleRequest() {
    s_nReqEnterBattle = GAMEMODE_FIELD;
    s_nReqExitBattle = 0;
}
BOOL CSceneInfo::reqEnterMinigame() { return reqEnterBattle(); }
BOOL CSceneInfo::reqEnterMinigame(SHORT inID) { return reqEnterBattle(inID); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", reqExitMinigame__10CSceneInfos);
BOOL CSceneInfo::reqRetryMinigame() {
    BOOL b = reqExitBattle();
    if (b) s_nReqExitBattle = 2;
    return b;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", reqChangeScene__10CSceneInfosssii);
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
BOOL CSceneInfo::hasReqChangeFloor() { return s_nNextFloorIdx >= 0; }
INT CSceneInfo::getNextFloorIndex() { return s_nNextFloorIdx; }
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getCurrentWorldID__10CSceneInfoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getWorldID__10CSceneInfoUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", setupCompleteFloorWorld__10CSceneInfo8WORLD_ID);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", setEventDoorUnavailable__10CSceneInfoii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", setTmpCurrentRoomID__10CSceneInfos);
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
INT CSceneInfo::getNextRoomID() { return s_nNextRoomID; }
INT CSceneInfo::getPrevRoomID() { return CGameWork::m_pMCWork->_nPrevRoomID; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getEnteredDoorID__10CSceneInfo);
DOOR_ID CSceneInfo::getNextDoorID() { return s_nNextDoorID; }
BOOL CSceneInfo::isFieldScene() {
    BOOL b = FALSE;
    if (s_nGameMode == GAMEMODE_FIELD) b = !isEventScene();
    return b;
}
BOOL CSceneInfo::isBattleScene() { return s_nGameMode == GAMEMODE_BATTLE; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", isBossBattleScene__10CSceneInfoi);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", doneChangeScene__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", update__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", updateBattleScene__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _updateRoomInfo__Fv);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", addToSceneCache__10CSceneInfoRC10AREA_SCENE);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", removeFromSceneCache__10CSceneInfoRC10AREA_SCENE);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", removeFromSceneCacheWithRsrc__10CSceneInfoRC10AREA_SCENE);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", setSeRangeRatio__10CSceneInfoUc);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", checkMapRsrcCache__10CSceneInfoii);
void* CSceneInfo::getMapRsrcCache(INT nSlot) {
    switch (nSlot) {
    case 1: return (void*)s_nMapCacheField;
    case 2: return (void*)s_nMapCacheBattle;
    }
    return NULL;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", updateMapRsrcCache__10CSceneInfoii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", removeMapRsrcCache__10CSceneInfoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _linkFloorData__FPCc);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getCurrentFloorDataSrc__10CSceneInfo);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getRoomInfo__10CSceneInfoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getEventRoomInfo__10CSceneInfoUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getRoomIndex__10CSceneInfoi);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _openGoalDoor__FRC8RoomInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", openGoalDoor__10CSceneInfo);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", isCreated__C8RoomInfo);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getRoomNameCardID__C8RoomInfo);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", canCreateRoom__C8DoorInfo);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getNormaColor__C8DoorInfoUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getNormaNumber__C8DoorInfoUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getNormaOperation__C8DoorInfoUs);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", resetNorma__8DoorInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getSceneNumFromCardID__10CSceneInfoUc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getCardIDFromSceneNum__10CSceneInfos);
INT CSceneInfo::getConstWorldRsrcID(INT nWorld, BOOL bSora) {
    if ((UINT)nWorld < 14) return !bSora ? nWorld + 1100 : nWorld + 100;
    return 0;
}
INT CSceneInfo::getInconstWorldRsrcID(INT nWorld, BOOL bSora) {
    if ((UINT)nWorld < 14) return !bSora ? nWorld + 1200 : nWorld + 200;
    return 0;
}
