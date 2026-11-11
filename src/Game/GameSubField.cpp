#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h" /* MATCH: this unit names CollUtil.h by its full original path */
#define COLLDISP_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollDisp.h" /* MATCH: this unit names CollDisp.h by its full original path */
/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "System/Task.h"
#include "System/TaskMan.h"
#include "Game/Collision/CollUtilA.h"
#include "Game/Collision/CollUtil.h"
#include "Game/Collision/CollDisp.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"

/* MATCH: Placeholder: an empty string follows Str.h's strings in .rodata; which header it comes from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}

/* MATCH: Placeholders for an unknown header's inline functions (prize messages) that come next in .rodata
 * (as in Game/Player/PlayerManager). */
inline void _premiumPrizeMessage() {
    TRACE("\x83v\x83\x8c\x83~\x83" "A\x83\x80\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}
inline void _roulettePrizeMessage() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}

#include "System/Graphic/Effect/effect.h"
#include "System/Fsm.h"
#include "Game/GameSubField.h"
#include "System/MainApp.h"
#include "System/Debug.h"
#include "System/Graphic/WorldEnv.h"
#include "System/Graphic/DeFocus.h"
#include "System/Graphic/ShadowModel.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/Graphic/2d/2DSpr.h"
#include "System/Sound/SoundMan.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "Game/SceneInfo.h"
#include "Game/NowLoading.h"
#include "Game/Map.h"
#include "Game/MessageManager.h"
#include "Game/LockOnManager.h"
#include "Game/EntityFactory.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/Enemy/MapEnemyManager.h"
#include "Game/Player/PlayerManager.h"
#include "include_asm.h"
extern "C" int sprintf(char* s, const char* fmt, ...);



#include "System/RsrcDB.h"





#include "System/Rcf.h"







#include "Game/Enemy/BattleEnemyManager.h"




#include "Game/Atel/Atel.h"





#include "System/Graphic/PackMan.h"
#include "System/HeapMan.h"




/* Game/Collision/CollUtil.h's dispose accessor (line 202) */
static inline void _collUtilDispose() {
    CCollUtil*& instance = CCollUtil::m_pInstance;
    KH_DEBUG(if (!instance) AssertApp(COLLUTIL_H_PATH, 202, "m_pInstance", 0);)
    instance->dispose();
}

#define ASSERT(e) KH_DEBUG(if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0))

static int s_preEventMovieFlags[2] = {0x20000, 0x40000};
static int s_preEventMovieNums[2] = {132, 133};

CGameSubField::CGameSubField() : CGameSub((CStateTemplate<CGameSub>*)m_State) {
    INT nStartState;
    m_State[0].set(this, &CGameSubField::preLoadField, &CGameSubField::loadField, NULL);
    m_State[1].set(this, &CGameSubField::preLoadPlayer, &CGameSubField::loadPlayer, NULL);
    m_State[2].set(this, &CGameSubField::preProcPreEvent, &CGameSubField::procPreEvent, NULL);
    m_State[3].set(this, &CGameSubField::preSetupAtel, &CGameSubField::setupAtel, NULL);
    m_State[4].set(this, &CGameSubField::preSetupTasks, &CGameSubField::setupTasks, NULL);
    m_State[5].set(this, &CGameSubField::preWaitStartAtel, &CGameSubField::waitStartAtel, NULL);
    m_State[6].set(this, &CGameSubField::prePlay, &CGameSubField::playing, NULL);
    m_State[7].set(this, &CGameSubField::preToBattle, &CGameSubField::toBattle, NULL);
    m_State[8].set(this, &CGameSubField::preToBattleDispose, &CGameSubField::toBattleDispose, NULL);
    m_State[9].set(this, &CGameSubField::preFromBattle, &CGameSubField::fromBattle, NULL);
    m_State[10].set(this, &CGameSubField::preToNextRoom, &CGameSubField::toNextRoom, NULL);
    m_State[11].set(this, &CGameSubField::preToNextRoomDispose, &CGameSubField::toNextRoomDispose, NULL);
    m_State[12].set(this, &CGameSubField::preSleep, &CGameSubField::sleeping, NULL);
    nStartState = 0;
    if (KH_DW(CGame::isDbgStartFromBattle())) {
        nStartState = (USHORT)(CSceneInfo::getCurrentBattleSceneNum() - 1050) < 149;
    }
    changeState(nStartState);
}

CGameSubField::~CGameSubField() {
    CSceneInfo::removeCurrentFieldSceneRsrc();
}

INT CGameSubField::getGameSubState() const {
    switch (getCurrentStateID()) {
    case 2:
    case 3:
    case 4:
    case 5:
        return 1;
    case 6:
        return 2;
    case 12:
        return 6;
    }
    return 0;
}

BOOL CGameSubField::reqChangeGameSubState(INT nState) {
    INT nNewState = -1;
    switch (getCurrentStateID()) {
    case 12:
        if (nState == 4) {
            nNewState = 9;
            break;
        }
    case 6:
        if (nState == 3) {
            nNewState = 7;
        } else if (nState == 5) {
            nNewState = 10;
        }
        break;
    case 5:
        if (nState == 2) nNewState = 6;
        break;
    }
    if (nNewState < 0) {
        return FALSE;
    }
    changeState(nNewState);
    return TRUE;
}

void CGameSubField::terminate() {
    TRACE_COL((TtyColor)31, "[CGameSubField]: --- Terminate ---\n");
    disposeTasks();
}

void CGameSubField::setupMapCollision() {
    void** pMCD = (void**)CRsrcDB::getRsrcMCD(getMapID());
    if (pMCD) {
        CCollUtil::initializeData(*pMCD, NULL);
    }
}

void CGameSubField::preLoadField() {
    CMainApp::setFpsFull();
    CWorldEnv::readyMike(0, 0);
    CRsrcDB::setSceneAttachMode(CSceneInfo::getCurrentSceneCacheID());
    if (CSceneInfo::getCurrentWorldID(1) != 10 && !CSceneInfo::isEventScene()) {
        PLM::mallocBattleRsrcBuffer(0);
    }
    INT nArea = CSceneInfo::getCurrentAreaNum();
    INT nScene = CSceneInfo::getCurrentSceneNum();
    INT nMapID = CRcf::getMapID(nArea, nScene);
    setMapID(nMapID);
    BOOL bCached = CSceneInfo::checkMapRsrcCache(nMapID, 0) >= 0;
    CSceneInfo::updateMapRsrcCache(nMapID, 1);
    if (!bCached && nMapID) {
        TRACE_USER_COL((USER)4, (TtyColor)34, ">>> LOAD MAP-RSRC [[[%d]]]\n", nMapID);
        CRcf::reqLoadRCF((UINT)nMapID / 10000, (UINT)nMapID % 10000, 0x100, NULL, 0);
    }
    CRcf::reqLoadScene(nArea, nScene);
    if (nMapID) {
        if ((UINT)((SHORT)nScene - 2000) >= 1000 || (USHORT)(CSceneInfo::getPrevSceneNum() - 2000) >= 1000) {
            CNowLoading::start();
        }
    }
}

void CGameSubField::loadField() {
    if (CRcf::isCompleteLoadRCF(0, 0)) {
        setupMapCollision();
        changeState(1);
    }
}

void CGameSubField::preLoadPlayer() {
    if (!CSceneInfo::isEventScene()) {
        PLM::setReqLoadPlayer(0);
    }
}

void CGameSubField::loadPlayer() {
    if (CRcf::isCompleteLoadRCF(0, 0)) {
        if (getPreEventIndex() >= 0) {
            changeState(2);
        } else {
            changeState(3);
        }
    }
    CNowLoading::end(0);
}

INT CGameSubField::getPreEventIndex() const {
    INT nIndex = -1;
    if (CGame::isSoraGame() && CSceneInfo::getCurrentWorldID(1) == 13) {
        switch (CSceneInfo::getCurrentRoomID()) {
        case 5:
            nIndex = 0;
            break;
        case 7:
            nIndex = 1;
            break;
        }
        if (nIndex >= 0) {
            if ((CGameWork::getInt(0, 0x28) & s_preEventMovieFlags[nIndex]) ||
                (CGameWork::getInt(0, 0xC8) & s_preEventMovieFlags[nIndex])) {
                nIndex = -1;
            }
        }
    }
    return nIndex;
}

void CGameSubField::preProcPreEvent() {
    m_nStep = 0;
}

void CGameSubField::procPreEvent() {
    switch (m_nStep) {
    case 0: {
        INT nIndex = getPreEventIndex();
        if (nIndex >= 0) {
            CHAR szName[16];
            sprintf(szName, "%d", s_preEventMovieNums[nIndex]);
            if (CGame::reqPlayMovie(szName, 1)) {
                CGameWork::setInt(0, 0x28, CGameWork::getInt(0, 0x28) | s_preEventMovieFlags[nIndex]);
                CGameWork::setInt(0, 0xC8, CGameWork::getInt(0, 0xC8) | s_preEventMovieFlags[nIndex]);
            }
        }
        m_nStep++;
        break;
    }
    case 1:
        changeState(3);
        break;
    }
}

void CGameSubField::preSetupAtel() {
    CMainApp::setFpsHalf();
    CGameSub::preSetupAtel();
    CGame::setBlack();
}

void CGameSubField::setupAtel() {
    if (CGameSub::setupAtel()) {
        changeState(4);
    }
}

void CGameSubField::preSetupTasks() {}

#line 376
void CGameSubField::setupTasks() {
    if (KH_DW(CGame::isDbgStartFromBattle())) {
        changeState(6);
        return;
    }
    CTask* pSysTask = CTaskMan::getSysTask(1);
    CMap* pMap = new KH_AT("(382)") CMap(pSysTask, CRcf::getMapID(CSceneInfo::getCurrentAreaNum(), CSceneInfo::getCurrentSceneNum()));
    ASSERT(pMap);
    Message::CMessageManager::getInstance();
    LockOnManager::CLockOnManager::getInstance();
    CEntityFactory::getInstance();
    CGimmickManager* pGimmickManager = new KH_AT("(395)") CGimmickManager(pSysTask);
    CMapEnemyManager* pMapEnemyManager = NULL;
    if ((USHORT)(CSceneInfo::getCurrentBattleSceneNum() - 1000) < 50) {
        pMapEnemyManager = new KH_AT("(400)") CMapEnemyManager(pSysTask);
    }
    new KH_AT("(403)") PLM::CPlayerManager(pSysTask);
    PLM::createPlayer(0, NULL, 0);
    pMap->init(1, 1);
    CShadowModelMan::setAutomatic();
    CShadowModelMan::setUpPer(0.5f);
    CShadowModelMan::setDownLen(2.8f);
    RoomInfo* pRoomInfo = CSceneInfo::getRoomInfo(-1);
    if (pRoomInfo && pRoomInfo->isFirstTimeAfterRC()) {
        pGimmickManager->initRoomCreate();
        if (pMapEnemyManager) {
            pMapEnemyManager->initRoomCreate();
        }
        CBattleEnemyManager::clearBeforeFormation();
    }
    if (pMapEnemyManager) {
        pMapEnemyManager->setup();
    }
    CDeFocus::create(pSysTask);
    CDeFocus::offNear();
    KH_DEBUG(CCollDisp::initialize(pSysTask, dbgButtonDown(USR_SYSTEM, 0x100, 1) | CCollDisp::m_fgDisp, CCollDisp::m_fgMapZoneDisp);)
    changeState(5);
}

static CHAR* s_pszBgmTbl[14] = {
    "20_Event_13thFloor", "01F_Town_F",  "25_Aladdin_F",      "03F_Hercul",  "19_Alice_Field",  "24_Pinocchio_Field",
    "17_Halloween_F",     "38_UnderTheSea", "22_PeterPan_Field", "42_Hollow_F", "36_WinnieThePooh", "30_Twilight_F",
    "32_Destiny_F",       "28_Forget_F",
};

void CGameSubField::disposeTasks() {
    exitAtel();
    PLM::destroyPlayer();
    CTask* pTask;
    if ((pTask = CTaskMan::getSysTask(2))) pTask->out();
    if ((pTask = CTaskMan::getSysTask(31))) pTask->out();
    if ((pTask = CTaskMan::getSysTask(4))) pTask->out();
    if ((pTask = CTaskMan::getSysTask(17))) pTask->out();
    if ((pTask = CTaskMan::getSysTask(24))) pTask->out();
    if ((pTask = CTaskMan::getSysTask(29))) pTask->out();
    if ((pTask = CTaskMan::getSysTask(32))) pTask->out();
    if ((pTask = CTaskMan::getSysTask(23))) pTask->out();
    CWorldEnv::removeAllCamera();
    CDeFocus::remove();
    CEffectMan::killAll((ObjType::eObjBit)0xFFFFFFFF, 0.0f);
    if (!CGameWork::getFlag(1, 0x33)) {
        D2::C2DSpr::destroyAllForSceneChange();
    }
    KH_DEBUG(CCollDisp::finalize();)
    _collUtilDispose();
}

void CGameSubField::preWaitStartAtel() {
    m_nStep = 2;
    CWorldEnv::readyMike(1, 1);
}

void CGameSubField::waitStartAtel() {
    INT w;
    CHAR* pszBgm;
    if (m_nStep > 0) {
        if (--m_nStep == 0) {
            startAtel();
            BOOL bBGM = TRUE;
            CGame::reqBlackFadeIn(20);
            do { /* MATCH: a jump out of a loop is predicted not taken (reorg fills from the fall-through) */
                if (CSceneInfo::isEventScene()) goto ev; /* MATCH: the event-scene block between the ternary arms */
            } while (0);
            if (CGameWork::getFlag(1, 0x32) || CSceneInfo::isLastFloorScene()) {
                bBGM = FALSE;
            } else if (CGame::isRikuGame() && CSceneInfo::getCurrentWorldID(0) == 12 && CSceneInfo::getCurrentSceneNum() == 30) {
                bBGM = FALSE;
            }
        check:
            if (bBGM) {
                w = CSceneInfo::getCurrentWorldID(1);
                if (w < 14) {
                    pszBgm = s_pszBgmTbl[w];
                    goto have;
                ev:
                    bBGM = FALSE;
                    goto check;
                }
                pszBgm = NULL;
            have:
                if (pszBgm) CSound::crossFadeinBGM(pszBgm, 60, -1, 0);
            }
        }
    } else if (CGame::isFadeDone()) {
        changeState(6);
    }
}

void CGameSubField::prePlay() {
    if (!CWorldEnv::isReadyMike()) {
        CWorldEnv::readyMike(1, 0);
    }
    CSceneInfo::doneChangeScene();
    if (KH_DW(CGame::isDbgStartFromBattle())) {
        CSceneInfo::reqEnterBattle(CSceneInfo::getCurrentBattleSceneNum());
    }
}

void CGameSubField::playing() {}

void CGameSubField::preToBattle() {
    if (!CGameWork::getFlag(1, 0x25) || !CAtel::isFadeActive()) {
        CGame::setFadeMaskProcPrio((CGame::_FADE_PROC_PRIO)1);
        if (CGame::isBlackBeforeEncount()) {
            CGame::reqBlackFadeOut(10);
        } else {
            CPackMan::changeDrawMode(1);
        }
    }
    if (!CSceneInfo::isEventScene() && !CGameWork::getFlag(1, 0x32)) {
        CSound::fadeoutAllBGM(30);
    }
    CSound::fadeoutAllSE(60, 1);
    CSound::fadeoutAllVoice(60);
}

void CGameSubField::toBattle() {
    if (!CGameWork::getFlag(1, 0x25) || !CAtel::isFadeActive()) {
        if (CGame::isBlackBeforeEncount() && !CGame::isFadeDone()) {
            return;
        }
        CAtel::clearScreenFader(-1);
        CGame::setFadeMaskProcPrio((CGame::_FADE_PROC_PRIO)0);
    }
    changeState(8);
}

void CGameSubField::preToBattleDispose() {}

void CGameSubField::toBattleDispose() {
    disposeTasks();
    changeState(12);
}

void CGameSubField::preFromBattle() {
    setupMapCollision();
}

void CGameSubField::fromBattle() {
    if (getPreEventIndex() >= 0) {
        changeState(2);
    } else {
        changeState(3);
    }
}

void CGameSubField::preToNextRoom() {
    INT nNextScene = CSceneInfo::getNextSceneNum();
    INT nScene = CSceneInfo::getCurrentSceneNum();
    if (CSceneInfo::isEventScene() && (USHORT)(nNextScene - 2000) >= 1000) {
        CGame::setFadeMaskProcPrio((CGame::_FADE_PROC_PRIO)1);
    }
    CGame::reqBlackFadeOut(20);
    BOOL bFlag = CGameWork::getFlag(1, 0x34);
    BOOL bCurRoom = FALSE;
    INT nRoom = CSceneInfo::getCurrentRoomID();
    if (nRoom == 0 || nRoom == 99) bCurRoom = TRUE;
    BOOL bNextRoom = FALSE;
    nRoom = CSceneInfo::getNextRoomID();
    if (nRoom == 0 || nRoom == 99) bNextRoom = TRUE;
    BOOL bFade = FALSE;
    if (CGame::isSoraGame() && CSceneInfo::getCurrentWorldID(1) == 13 && ((USHORT)nNextScene == 90 || (USHORT)nScene == 90)) {
        bFade = TRUE;
    }
    if (bFlag || bCurRoom != bNextRoom || bFade) {
        CSound::fadeoutAllBGM(30);
    }
}

void CGameSubField::toNextRoom() {
    if (CGame::isFadeDone()) {
        if (CSceneInfo::isEventScene() && (USHORT)(CSceneInfo::getNextSceneNum() - 2000) >= 1000) {
            CAtel::clearScreenFader(-1);
            CGame::setFadeMaskProcPrio((CGame::_FADE_PROC_PRIO)0);
        }
        changeState(11);
    }
}

void CGameSubField::preToNextRoomDispose() {}

void CGameSubField::toNextRoomDispose() {
    CMapEnemyManager::cleanupRoomMapEnemyInfo();
    disposeTasks();
    CSound::stopAllSE();
    CWorldEnv::readyMike(0, 0);
    changeState(12);
}

void CGameSubField::preSleep() {}

void CGameSubField::sleeping() {}
// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data .rodata
