#include "khtypes.h"
#include "Game/SceneInfo.h"
#include "Game/GameWork.h"
#include "include_asm.h"

/* AREA_SCENE without its constructors (built in a register; PARTIAL) */
union _AREA_SCENE_REG {
    struct {
        SHORT _nScene;
        SHORT _nArea;
    };
    INT _nAreaScene;
};

enum GAMEMODE { GAMEMODE_FIELD = 0, GAMEMODE_BATTLE = 1 };

struct FloorLayoutDataHeader {
    UBYTE _m_0x00[6];
    USHORT nCount; /* 0x06 */
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
    UBYTE _m_0x01[6];
    BYTE _nNextNorma;    /* 0x07 */
    BYTE _nFirstCardID;  /* 0x08 */
    BYTE _nFirstCardNum; /* 0x09 */
};

struct _SceneInfoWork {
    static STORAGE::MCWork* mc() { return CGameWork::m_pMCWork; }
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
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", clear__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", initNewGame__10CSceneInfoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", dispose__10CSceneInfo);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _getDstDoorID__F7DOOR_ID);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _setWorldEntrance__F8WORLD_IDssi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", makeRoom__10CSceneInfoRC8RoomInfo7DOOR_IDsUcUc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", makeRoom__10CSceneInfoR8DoorInfoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", hasAnyRequest__10CSceneInfo);
void CSceneInfo::setNextAreaScene(SHORT nArea, SHORT nField, SHORT nScene) {
    s_NextAreaScene._nScene = nField;
    s_NextAreaScene._nArea = nArea;
    setNextBattleScene(nScene);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", setNextScene__10CSceneInfoss);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", setNextBattleScene__10CSceneInfos);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _acceptReqEnterBattle__Fv);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", reqRetryMinigame__10CSceneInfo);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", isTgsFloor__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", isJFestaFloor__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getCurrentWorldID__10CSceneInfoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getWorldID__10CSceneInfoUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", setupCompleteFloorWorld__10CSceneInfo8WORLD_ID);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", setEventDoorUnavailable__10CSceneInfoii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", setTmpCurrentRoomID__10CSceneInfos);
void CSceneInfo::ignoreFloorInfo() {
    CGameWork::m_pMCWork->_nFloorIdx = 0xff;
    s_nCurrentFloorNumber = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getCurrentRoomID__10CSceneInfo);
INT CSceneInfo::getNextRoomID() { return s_nNextRoomID; }
INT CSceneInfo::getPrevRoomID() { return CGameWork::m_pMCWork->_nPrevRoomID; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getEnteredDoorID__10CSceneInfo);
DOOR_ID CSceneInfo::getNextDoorID() { return s_nNextDoorID; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", isFieldScene__10CSceneInfo);
BOOL CSceneInfo::isBattleScene() { return s_nGameMode == GAMEMODE_BATTLE; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", isBossBattleScene__10CSceneInfoi);
BOOL CSceneInfo::isEventScene() { return (USHORT)(getCurrentSceneNum() - 2000) < 1000; }
BOOL CSceneInfo::isMinigameScene() { return (USHORT)(getCurrentSceneNum() - 1200) < 56; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", isLastFloorScene__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getCurrentRoomGimmickInfo__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getCurrentRoomMapEnemyInfo__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", doneChangeScene__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", update__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", updateBattleScene__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _updateRoomInfo__Fv);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", clearSceneCache__10CSceneInfo);
UINT CSceneInfo::getCurrentSceneCacheID() { return s_nCurrentSceneCacheID; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getSceneCacheID__10CSceneInfoRC10AREA_SCENE);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", addToSceneCache__10CSceneInfoRC10AREA_SCENE);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", removeFromSceneCache__10CSceneInfoRC10AREA_SCENE);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", removeFromSceneCacheWithRsrc__10CSceneInfoRC10AREA_SCENE);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", removeCurrentFieldSceneRsrc__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", removeCurrentBattleSceneRsrc__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", setReverbVal__10CSceneInfoUc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getReverbVal__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getReverbVal__10CSceneInfoRC10AREA_SCENE);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", setSeRangeRatio__10CSceneInfoUc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getSeRangeRatio__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getSeRangeRatio__10CSceneInfoRC10AREA_SCENE);
void _clearMapRsrcCache() {
    s_nMapCacheField = 0;
    s_nMapCacheBattle = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", checkMapRsrcCache__10CSceneInfoii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getMapRsrcCache__10CSceneInfoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", updateMapRsrcCache__10CSceneInfoii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", removeMapRsrcCache__10CSceneInfoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _linkFloorData__FPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _linkEventRoomData__FPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _linkNormaData__FPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _isValidFloorIdx__FUsi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getCurrentFloorDataSrc__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getFloorDataSrc__10CSceneInfoUsi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getFloorNormaDataSrc__10CSceneInfoUsi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getRoomInfo__10CSceneInfoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getEventRoomInfo__10CSceneInfoUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getRoomIndex__10CSceneInfoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getDoorInfo__10CSceneInfo7DOOR_IDi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", setDoorStat__10CSceneInfo9DOOR_STAT7DOOR_IDi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _openGoalDoor__FRC8RoomInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", openGoalDoor__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", backupEventDoorState__10CSceneInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", rollbackEventDoorState__10CSceneInfoi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _setOpenFlagEventDoor__FRC8DoorInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", _getOpenFlagEventDoor__FRC8DoorInfo);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", isSpecialDoor__C8DoorInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", canSelectWorld__C8DoorInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", canCreateRoom__C8DoorInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getNormaKeyCardID__C8DoorInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", getNormaCount__C8DoorInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", isValidNormaIndex__C8DoorInfoUs);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", clearOneNorma__8DoorInfo);
USHORT DoorInfo::getNextNormaIndex() const {
    DOOR_STORAGE* p = (DOOR_STORAGE*)_pStorageInfo;
    USHORT n = 3;
    if (p) n = p->_nNextNorma;
    return n;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", isNextNormaKeyCard__C8DoorInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", isNormaCleared__C8DoorInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", useNormaCard__8DoorInfoRC4CARD);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SceneInfo", initNorma__8DoorInfo);
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
