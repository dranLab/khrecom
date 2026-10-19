#define TASK_H_NO_SLC_OVERFLOW_MSG
#define CARD_H_PATH "c:/KHCOM/MainApp/Game/Card.h"
#define STATUS_H_PATH "c:/KHCOM/MainApp/Game/Status.h"
#include "Game/Game.h"
#include "System/HeapMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
/* placeholder: 8 zero bytes follow CollUtil.h's strings in the original */
__asm__(".rdata\n.align 3\n.word 0\n.word 0\n"); /* stays in .rdata: gcc has not left it here */
/* placeholder: Task.h's container-overflow message comes here */
inline void _gameTaskSLCOverflow() {
    TRACE("\x83\x5e\x83\x58\x83\x4e\x97\x70\x83\x52\x83\x93\x83\x65\x83\x69\x82\xaa MAX \x82\xc9\x93\x9e\x92\x42\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
}
#ifndef KH_PLACEMENT_NEW_DEFINED
#define KH_PLACEMENT_NEW_DEFINED
inline void* operator new(size_t, void* p) throw() { return p; }
#endif
#include "Form/Menu/TitleReportDllStarter.h"
#include "Game/SceneInfo.h"
#include "Game/Card.h"
#include "Game/Status.h"
#include "System/Fsm.h"
#include "Game/Atel/Atel.h"
#include "Game/GameSub.h"
#include "Game/NowLoading.h"
#include "System/MainApp.h"
#include "System/Sound/SoundMan.h"
#include "Form/Menu/MoogleShop.h"
#include "Game/Player/PlayerManager.h"
#include "System/Graphic/ScreenMask.h"
#include "System/PadHandler.h"
#include "System/DebugProcInfo.h"
#include "System/TaskMan.h"
#include "Game/GameWork.h"
#include "Game/StorageWork.h"
#include "System/PadIO.h"
#include "System/Rcf.h"
#include "Game/MinigameManager.h"
#include "Game/SceneEnv.h"
#include "System/Graphic/WorldEnv.h"
#include "System/PlaySpeedCtrl.h"
#include "System/Movie/Movie.h"
#include "Game/GameSubBattle.h"
#include "Game/GameSubField.h"
#include "include_asm.h"

class CPackMan {
public:
    static void setFrameEffect(INT nA, INT nB, INT nC, INT nD);
};

// PARTIAL: System/RsrcDB.h would add RsrcData.h's string to .rodata
class CRsrcDB {
public:
    static void removeGroup(INT nGroupID, INT nMemberID);
    static void clearSceneAttachMode();
};

class CPause : public CTask {
private:
    BYTE _pad[0x100 - 0x40];

public:
    CPause(CTask* pParent, INT nMode);
    static INT m_nResult;
};

class CCamp : public CTask {
private:
    BYTE _pad[0x1b80 - 0x40];

public:
    CCamp(CTask* pParent, INT nMode);
};

enum SAVELOAD { SAVELOAD_SAVE = 0 };
enum GAMECLEAR { GAMECLEAR_NO = 0 };

class CSaveLoad : public CTask {
private:
    BYTE _pad[0x50 - 0x40];

public:
    CSaveLoad(CTask* pParent, SAVELOAD nMode, GAMECLEAR nClear);
    static BOOL isExited();
};

extern "C" void TsSdDebugPrint();
extern "C" char D_0047E328[];

class CGameOver : public CTask {
private:
    BYTE _pad[0xe0 - 0x40];

public:
    CGameOver(CTask* pParent, BOOL bHeartless);
};

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
#define GAME_NEW(line, T) new (com_malloc_d(sizeof(T), __FILE__ "(" #line ")", RET_ADDR())) T
#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

class CGameSub;

class CGameMain : public CTask, public CFSM, public CPadHandler {
private:
    enum { _PROCFLAG_GAMEOVER_PROC = 8 };
    CStateTemplate<CGameMain> m_State[17]; /* 0x07c */
    CFnt m_fntMsg;                         /* 0x29c */
    INT m_nState;                          /* 0x314 */
    INT m_nStep;                           /* 0x318 */
    INT m_nTimer;                          /* 0x31c */
    INT m_nConstFloorRsrcLoaded;           /* 0x320 */
    INT m_nConstWorldRsrcLoaded;           /* 0x324 */
    BOOL m_bInconstGameRsrcLoaded;         /* 0x328 */
    INT m_nInconstWorldRsrcLoaded;         /* 0x32c */
    BOOL m_bBlackBeforeEncount;            /* 0x330 */
    INT m_nGameProcFlag;                   /* 0x334 */
    BOOL m_bPauseEnable;                   /* 0x338 */
    HANDLE m_hCamp;                        /* 0x33c */
    HANDLE m_hPause;                       /* 0x340 */
    HANDLE m_hWorldMap;                    /* 0x344 */
    CGameSub* m_pSubField;                 /* 0x348 */
    CGameSub* m_pSubBattle;                /* 0x34c */
    char m_szMovieFileName[256];           /* 0x350 */
    IVECTOR m_ivFogColorBackup;            /* 0x450 */
    INT m_nCockpitDispBackup;              /* 0x460 */

public:
    CGameMain(CGame& game);
    virtual ~CGameMain();
    virtual void main();
    virtual void disp();
    BOOL reqPlayMovie(const CHAR* pszMovieFile, BOOL bSkipEnable);
    BOOL reqSave();
    BOOL reqGameOver(BOOL bHeartless);
    BOOL reqContinue_GameOver();
    BOOL reqExit_GameOver();
    BOOL reqCompleteGame();
    INT getMapID() const;
    BOOL isAtelFadeActive() const;
    void enablePause(BOOL bEnable) { m_bPauseEnable = bEnable; }
    BOOL isPauseEnable() const {
        BOOL bRes = FALSE;
        if (m_bPauseEnable) {
            if (CAtel::checkAtelPause() && !CSceneInfo::hasAnyRequest() && !(m_nGameProcFlag & 0x80201c01)) bRes = TRUE;
        }
        return bRes;
    }
    BOOL hasPauseRequest() const { return FALSE; }
    void checkStartFieldMenu();
    void checkStartBattleMenu();
    BOOL checkExitPause();
    void setBlackBeforeEncount(BOOL bBlack);
    BOOL isBlackBeforeEncount() const;
    void endFromTheaterMode();
    INT getState() const { return m_nState; }

private:
    void changeState(INT nState) {
        m_nState = nState;
        gotoState(m_State[nState]);
    }
    void suspendTaskInGame(INT nFlag);
    void resumeTaskInGame(INT nFlag);
    void pause(BOOL bOn, INT nMode);
    void camp(BOOL bOn);
    void worldMap(BOOL bOn);
    BOOL procFieldMenu();
    BOOL procBattleMenu();
    void preSetupCommon();
    void setupCommon();
    void preSetupWorld();
    void setupWorld();
    void preSetupBattleRsrc();
    void setupBattleRsrc();
    void preSetupField();
    void preSetupField2();
    void setupField();
    void prePlayField();
    void playField();
    void preEndField();
    void endField();
    void endFieldSub();
    void initNextFloor();
    void preRetryField();
    void retryField();
    void preSetupBattle();
    void setupBattle();
    void prePlayBattle();
    void playBattle();
    void gotoEndBattle();
    void preEndBattle();
    void endBattle();
    void endBattleSub();
    void gotoRetryBattle();
    void preRetryBattle();
    void retryBattle();
    void prePlayMovie();
    void playMovie();
    void preGameover();
    void gameover();
    void resetSceneEnv();
    void preExit();
    void exit();
    void preCompleteGame();
    void completeGame();
    void preExitTheaterMode();
    void exitTheaterMode();
    void reqLoadConstFloorRsrc();
    void removeConstFloorRsrc();
    void reqLoadConstWolrdRsrc();
    void removeConstWorldRsrc();
    void reqLoadInconstGameRsrc();
    void reqLoadInconstWorldRsrc();
    void removeInconstWorldRsrc();
    void removeInconstRsrc();
};

class CPreGameParent : public CTask {
public:
    CPreGameParent(CTask* pParent) : CTask(pParent, 0x8000) { declareSysTask(0xf); }
};

class CGameParent : public CTask {
public:
    CGameParent(CTask* pParent) : CTask(pParent, 0x8000) { declareSysTask(1); }
};

namespace {
CGameMain* s_pMain = NULL;
CGame::_FADE_PROC_PRIO s_nFadeProcPrio = CGame::FADE_PROC_PRIO_0;
CFadeIOMask* s_pFadeMask = NULL;
INT s_nGameStep = 0;
INT s_nDbgGameMode = 0;
BOOL s_bDbgStartFromBattle = 0;
}


CGameMain::CGameMain(CGame& game) : CTask(&game, 0x8000) {
    m_nConstFloorRsrcLoaded = -1;
    m_nConstWorldRsrcLoaded = -1;
    m_bInconstGameRsrcLoaded = FALSE;
    m_nInconstWorldRsrcLoaded = -1;
    m_bBlackBeforeEncount = TRUE;
    m_nGameProcFlag = 0;
    m_bPauseEnable = FALSE;
    m_hCamp = 0;
    m_hPause = 0;
    m_hWorldMap = 0;
    m_pSubField = NULL;
    m_pSubBattle = NULL;
    m_State[0].set(this, &CGameMain::preSetupCommon, &CGameMain::setupCommon, NULL);
    m_State[1].set(this, &CGameMain::preSetupWorld, &CGameMain::setupWorld, NULL);
    m_State[2].set(this, &CGameMain::preSetupBattleRsrc, &CGameMain::setupBattleRsrc, NULL);
    m_State[3].set(this, &CGameMain::preSetupField, &CGameMain::setupField, NULL);
    m_State[4].set(this, &CGameMain::preSetupField2, &CGameMain::setupField, NULL);
    m_State[5].set(this, &CGameMain::prePlayField, &CGameMain::playField, NULL);
    m_State[6].set(this, &CGameMain::preEndField, &CGameMain::endField, NULL);
    m_State[7].set(this, &CGameMain::preRetryField, &CGameMain::retryField, NULL);
    m_State[8].set(this, &CGameMain::preSetupBattle, &CGameMain::setupBattle, NULL);
    m_State[9].set(this, &CGameMain::prePlayBattle, &CGameMain::playBattle, NULL);
    m_State[10].set(this, &CGameMain::preEndBattle, &CGameMain::endBattle, NULL);
    m_State[11].set(this, &CGameMain::preRetryBattle, &CGameMain::retryBattle, NULL);
    m_State[12].set(this, &CGameMain::prePlayMovie, &CGameMain::playMovie, NULL);
    m_State[13].set(this, &CGameMain::preGameover, &CGameMain::gameover, NULL);
    m_State[14].set(this, &CGameMain::preExit, &CGameMain::exit, NULL);
    m_State[15].set(this, &CGameMain::preCompleteGame, &CGameMain::completeGame, NULL);
    m_State[16].set(this, &CGameMain::preExitTheaterMode, &CGameMain::exitTheaterMode, NULL);
    changeState(0);
}
CGameMain::~CGameMain() {
    CMoogleShopMain::finalize();
    if (m_pSubField) delete m_pSubField;
    if (m_pSubBattle) delete m_pSubBattle;
    PLM::removePlayerRsrc(0);
    PLM::removeBattlePlayerRsrc(0);
    CSound::stopConstEnvSE(0);
    s_pMain = NULL;
}
void CGameMain::main() {
    update();
    while (m_nGameProcFlag & 0x40000000) {
        m_nGameProcFlag &= ~0x40000000;
        update();
    }
}
void CGameMain::disp() {}
INT CGameMain::getMapID() const {
    if (m_pSubField && m_pSubField->getGameSubState() == 2) return m_pSubField->getMapID();
    if (m_pSubBattle && m_pSubBattle->getGameSubState() == 2) return m_pSubBattle->getMapID();
    return 0;
}
BOOL CGameMain::isAtelFadeActive() const {
    BOOL bRes = FALSE;
    if ((m_pSubField && m_pSubField->isFadeActive()) || (m_pSubBattle && m_pSubBattle->isFadeActive())) {
        bRes = TRUE;
    }
    return bRes;
}
void CGameMain::suspendTaskInGame(INT nFlag) {
    if (nFlag & 1) {
        INT nBit = ~0x40010000;
        if (nFlag & 4) nBit &= ~0x4000;
        CSceneEnv::setStop((ObjType::eObjBit)nBit);
    }
    if (nFlag & 2) CTaskMan::reqSuspendGameTask();
    CTask* p = getSysTask(0xb);
    if (p) p->setActive(FALSE);
}
void CGameMain::resumeTaskInGame(INT nFlag) {
    if (nFlag & 1) CSceneEnv::setStop((ObjType::eObjBit)0);
    if (nFlag & 2) {
        CGame::resumeGameTask();
        if (m_pSubField && m_pSubField->isAtelStarted()) m_pSubField->resumeAtel();
        if (m_pSubBattle && m_pSubBattle->isAtelStarted()) m_pSubBattle->resumeAtel();
    }
    CTask* p = getSysTask(0xb);
    if (p) p->setActive(TRUE);
}
void CGameMain::pause(BOOL bOn, INT nMode) {
    if (bOn) {
        CSound::pauseAllSE();
#line 523
        CPause* p = GAME_NEW(523, CPause)(this, nMode);
        ASSERT(p);
        m_hPause = p->getHandle();
        m_nGameProcFlag |= 0x400;
    } else {
        resumeTaskInGame(2);
        m_hPause = 0;
        CSound::startAllPausedSE();
        m_nGameProcFlag |= 0x800;
    }
}
void CGameMain::camp(BOOL bOn) {
    if (bOn) {
        CSound::pauseAllSE();
        if (CGame::isSoraGame()) PLM::removeBattlePlayerRsrc(0);
#line 553
        CCamp* p = GAME_NEW(553, CCamp)(this, 0);
        ASSERT(p);
        m_hCamp = p->getHandle();
        m_nGameProcFlag |= 0x400;
    } else {
        resumeTaskInGame(2);
        m_hCamp = 0;
        CSound::startAllPausedSE();
        m_nGameProcFlag |= 0x800;
    }
}
void CGameMain::worldMap(BOOL bOn) {
    if (bOn) {
        CSound::pauseAllSE();
#line 578
        CCamp* p = GAME_NEW(578, CCamp)(this, 1);
        ASSERT(p);
        m_hWorldMap = p->getHandle();
        m_nGameProcFlag |= 0x400;
    } else {
        resumeTaskInGame(2);
        m_hWorldMap = 0;
        CSound::startAllPausedSE();
        m_nGameProcFlag |= 0x800;
    }
}
static inline BOOL _isStartRoom(INT nRoom) { return nRoom == 0 || nRoom == 0x63; }

void CGameMain::checkStartFieldMenu() {
    BOOL bFlag = CGameWork::getFlag(1, 0x33);
    if (CPadHandler::isButtonTrg(0x800, 0)) {
        if (CGameWork::getFlag(1, 4) || bFlag) {
            pause(TRUE, !CGameWork::getFlag(1, 3));
        } else {
            camp(TRUE);
        }
    } else if (CPadHandler::isButtonTrg(0x100, 0)) {
        if (!CGameWork::getFlag(1, 4) && !_isStartRoom(CSceneInfo::getCurrentRoomID()) && !bFlag &&
            CSceneInfo::getCurrentFloorIndex() < 16) {
            worldMap(TRUE);
        }
    }
    if (m_nGameProcFlag & 0x400) suspendTaskInGame(2);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", procFieldMenu__9CGameMain);
void CGameMain::checkStartBattleMenu() {
    if (CPadHandler::isButtonTrg(0x800, 0)) {
        INT nMode = 0;
        if (CSceneInfo::isMinigameScene()) {
            nMode = 2;
        } else {
            if (CSceneInfo::getCurrentWorldID(TRUE) == 5) {
                nMode = 4;
                if (CSceneInfo::getCurrentSceneNum() != 0x41b) nMode = 0;
            }
            TsSdDebugPrint();
        }
        pause(TRUE, nMode);
        suspendTaskInGame(2);
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", procBattleMenu__9CGameMain);
BOOL CGameMain::checkExitPause() {
    if (m_nGameProcFlag & 0x800) {
        m_nGameProcFlag &= ~0xc00;
        return TRUE;
    }
    return FALSE;
}
static inline INT _getConstFloorRsrcID(UINT nIndex) {
    if (nIndex >= 16) {
        TRACE_COL(_TTY_RED, "\x83t\x83\x8d\x83" "A\x8f\xed\x92\x93\x83\x8a\x83\\\x81[\x83X\x82\xcc\x8ew\x92\xe8\x82\xaa\x94\xcd\x88\xcd\x8aO\x82\xc5\x82\xb7\x81" "B\n");
        return -1;
    }
    INT nID = nIndex + 300;
    if (!CGame::isSoraGame()) nID = nIndex + 1300;
    return nID;
}

void CGameMain::reqLoadConstFloorRsrc() {
    INT nIndex = CSceneInfo::getCurrentFloorIndex();
    if (nIndex != m_nConstFloorRsrcLoaded) {
        INT nID = _getConstFloorRsrcID(nIndex);
        if (nID >= 0) {
            if (m_nConstFloorRsrcLoaded >= 0) removeConstFloorRsrc();
            CRcf::reqLoadRCF(0xe, nID, 0x100, NULL, 0);
            m_nConstFloorRsrcLoaded = nIndex;
            TRACE_COL(_TTY_RED, "[CGameMain]: --- Load Const-Floor-Rsrc --- : Floor = %d\n",
                      CSceneInfo::getFloorNumber(nIndex, CGame::isSoraGame()));
        }
    }
}
void CGameMain::removeConstFloorRsrc() {
    if (m_nConstFloorRsrcLoaded >= 0) {
        INT nID = _getConstFloorRsrcID(m_nConstFloorRsrcLoaded);
        if (nID >= 0) {
            CRsrcDB::removeGroup(0xe, nID);
            TRACE_COL(_TTY_RED, "[CGameMain]: --- Remove Const-Floor-Rsrc --- : Floor = %d\n",
                      CSceneInfo::getFloorNumber(m_nConstFloorRsrcLoaded, CGame::isSoraGame()));
            m_nConstFloorRsrcLoaded = -1;
        }
    }
}
static inline INT _getConstWorldRsrcID(UINT nWorld) {
    if (nWorld >= 14) {
        if (nWorld == 23 || nWorld == 24) return -1;
        nWorld = 1;
    }
    return CSceneInfo::getConstWorldRsrcID(nWorld, CGame::isSoraGame());
}

void CGameMain::reqLoadConstWolrdRsrc() {
    INT nWorld = CSceneInfo::getCurrentWorldID(TRUE);
    if (nWorld != m_nConstWorldRsrcLoaded) {
        if (m_nConstWorldRsrcLoaded >= 0) removeConstWorldRsrc();
        INT nID = _getConstWorldRsrcID(nWorld);
        if (nID < 0) {
            TRACE_COL(_TTY_RED, "\x83\x8f\x81[\x83\x8b\x83h\x8f\xed\x92\x93\x83\x8a\x83\\\x81[\x83X\x82\xcc\x8ew\x92\xe8\x82\xaa\x94\xcd\x88\xcd\x8aO\x82\xc8\x82\xcc\x82\xc5\x83X\x83L\x83" "b\x83v\x82\xb5\x82\xdc\x82\xb7\x81" "B\n");
        } else {
            CRcf::reqLoadRCF(0xe, nID, 0x100, NULL, 0);
            m_nConstWorldRsrcLoaded = nWorld;
            TRACE_COL(_TTY_RED, "[CGameMain]: --- Load Const-World-Rsrc --- : World = %d\n", nWorld);
        }
    }
}
void CGameMain::removeConstWorldRsrc() {
    if (m_nConstWorldRsrcLoaded >= 0) {
        INT nID = _getConstWorldRsrcID(m_nConstWorldRsrcLoaded);
        if (nID >= 0) {
            CRsrcDB::removeGroup(0xe, nID);
            TRACE_COL(_TTY_RED, "[CGameMain]: --- Remove Const-World-Rsrc --- : World = %d\n", m_nConstWorldRsrcLoaded);
            m_nConstWorldRsrcLoaded = -1;
        }
    }
}
void CGameMain::reqLoadInconstGameRsrc() {
    if (!m_bInconstGameRsrcLoaded) {
        CRcf::reqLoadRCF(0, 3, 0x100, NULL, 0);
        m_bInconstGameRsrcLoaded = TRUE;
        TRACE_COL(_TTY_RED, "[CGameMain]: --- Load Inconst GameRsrc ---\n");
    }
}
static inline INT _getInconstWorldRsrcID(UINT nWorld) {
    if (nWorld >= 14) {
        if (nWorld == 23 || nWorld == 24) return -1;
        nWorld = 1;
    }
    return CSceneInfo::getInconstWorldRsrcID(nWorld, CGame::isSoraGame());
}

void CGameMain::reqLoadInconstWorldRsrc() {
    INT nWorld = CSceneInfo::getCurrentWorldID(TRUE);
    if (nWorld != m_nInconstWorldRsrcLoaded) {
        if (m_nInconstWorldRsrcLoaded >= 0) removeInconstWorldRsrc();
        INT nID = _getInconstWorldRsrcID(nWorld);
        if (nID < 0) {
            TRACE_COL(_TTY_RED, "\x83\x8f\x81[\x83\x8b\x83h'\x94\xbc'\x8f\xed\x92\x93\x83\x8a\x83\\\x81[\x83X\x82\xcc\x8ew\x92\xe8\x82\xaa\x94\xcd\x88\xcd\x8aO\x82\xc8\x82\xcc\x82\xc5\x83X\x83L\x83" "b\x83v\x82\xb5\x82\xdc\x82\xb7\x81" "B\n");
        } else {
            CRcf::reqLoadRCF(0xe, nID, 0x100, NULL, 0);
            m_nInconstWorldRsrcLoaded = nWorld;
            TRACE_COL(_TTY_RED, "[CGameMain]: --- Load Inconst-World-Rsrc --- : World = %d\n", nWorld);
        }
    }
}
void CGameMain::removeInconstWorldRsrc() {
    if (m_nInconstWorldRsrcLoaded >= 0) {
        INT nID = _getInconstWorldRsrcID(m_nInconstWorldRsrcLoaded);
        if (nID >= 0) {
            CRsrcDB::removeGroup(0xe, nID);
            TRACE_COL(_TTY_RED, "[CGameMain]: --- Remove Inconst-World-Rsrc --- : World = %d\n", m_nInconstWorldRsrcLoaded);
            m_nInconstWorldRsrcLoaded = -1;
        }
    }
}
void CGameMain::removeInconstRsrc() {
    removeInconstWorldRsrc();
    if (m_bInconstGameRsrcLoaded) {
        CRsrcDB::removeGroup(0, 3);
        m_bInconstGameRsrcLoaded = FALSE;
        TRACE_COL(_TTY_RED, "[CGameMain]: --- Remove Inconst GameRsrc ---\n");
    }
}
void CGameMain::preSetupCommon() {
    CMainApp::setFpsFull();
    CRsrcDB::clearSceneAttachMode();
    CEncountEffect::initialize();
    CRcf::reqLoadRCF(0, 2, 0, NULL, 0);
    CRcf::reqLoadRCF(0, CGame::isSoraGame() ? 5 : 6, 0, NULL, 0);
    CNowLoading::start();
}
void CGameMain::setupCommon() {
    if (CRcf::isCompleteLoadRCF(0, 0)) changeState(1);
}
void CGameMain::preSetupWorld() { m_nStep = 0; }
void CGameMain::setupWorld() {
    switch (m_nStep) {
    case 0:
        m_nStep = 1;
        break;
    case 1:
        if (CGame::isRikuGame()) {
            PLM::setReqLoadPlayer(0);
            PLM::setReqLoadBattlePlayer(0);
        }
        reqLoadConstFloorRsrc();
        reqLoadConstWolrdRsrc();
        if (!CSceneInfo::isBossBattleScene(-1)) CNowLoading::start();
        m_nStep++;
        break;
    case 2:
        if (CRcf::isCompleteLoadRCF(0, 0)) {
            INT nState;
            if ((USHORT)(CSceneInfo::getCurrentFieldSceneNum() - 2000) < 1000) {
                TRACE_COL(_TTY_RED, "[CGameMain]: --- Skip Load : Battle Rsrc ---\n");
                nState = 3;
                CSound::clearForEventScene();
            } else {
                CSound::clearForBattleScene();
                nState = 2;
            }
            changeState(nState);
        }
        break;
    }
}
void CGameMain::preSetupBattleRsrc() { m_nStep = 0; }
void CGameMain::setupBattleRsrc() {
    switch (m_nStep) {
    case 0:
        m_nStep = 1;
        break;
    case 1:
        if (m_nGameProcFlag & 0x10000000) {
            CEncountEffect::start();
            m_nGameProcFlag &= ~0x10000000;
        }
        if (!m_pSubBattle) {
#line 1066
            m_pSubBattle = GAME_NEW(1066, CGameSubBattle)();
            ASSERT(m_pSubBattle);
        }
        reqLoadInconstGameRsrc();
        if (!CSceneInfo::isBossBattleScene(-1)) {
            if (!CGame::isDbgStartFromBattle() || (USHORT)(CSceneInfo::getCurrentBattleSceneNum() - 1050) >= 149) {
                reqLoadInconstWorldRsrc();
            }
        }
        m_nStep++;
        break;
    case 2:
        m_pSubBattle->update();
        if (m_pSubBattle->getGameSubState() == 6) {
            changeState((CSceneInfo::isBattleScene() || CSceneInfo::isMinigameScene()) ? 8 : 3);
        }
        break;
    }
}
void CGameMain::preSetupField() {
    if ((USHORT)(CSceneInfo::getPrevSceneNum() - 1000) < 50) {
        m_nStep = 0;
        return;
    }
    INT nRoom = CSceneInfo::getCurrentRoomID();
    if (nRoom >= 0) {
        BOOL bStart = _isStartRoom(nRoom);
        INT nPrev = CSceneInfo::getPrevRoomID();
        if (nPrev < 0 || _isStartRoom(nPrev) != bStart) CCard::initCardWorld();
    }
    if (!CSceneInfo::isEventScene()) {
        CSceneInfo::backupEventDoorState();
        CGameWork::backup(CGameWork::BACKUP_PRIMARY);
    }
    m_nStep = 0;
}
void CGameMain::preSetupField2() { m_nStep = 2; }
void CGameMain::setupField() {
    switch (m_nStep) {
    case 0:
        m_nStep = 1;
        break;
    case 1:
        resumeTaskInGame(1);
        if (!m_pSubField) {
#line 1156
            m_pSubField = GAME_NEW(1156, CGameSubField)();
            if (!CSceneInfo::isEventScene()) {
                reqLoadInconstGameRsrc();
                reqLoadInconstWorldRsrc();
            }
        } else {
            m_pSubField->reqChangeGameSubState(4);
        }
        m_nStep++;
        break;
    case 2: {
        m_pSubField->update();
        INT nState = m_pSubField->getGameSubState();
        if (m_nGameProcFlag & 0x10000) {
            if (m_pSubField->isAtelStarted()) {
                if (nState != 2) {
                    nState = 2;
                    m_pSubField->reqChangeGameSubState(2);
                    m_pSubField->update();
                }
                m_nGameProcFlag |= 0x40000000;
            } else {
                m_nGameProcFlag |= 0x80000;
                changeState(12);
                break;
            }
        }
        if (nState == 2) changeState(5);
        break;
    }
    }
}
void CGameMain::prePlayField() {
    m_bPauseEnable = TRUE;
    CMainApp::enableDbgUsrPause(TRUE, FALSE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", playField__9CGameMain);
void CGameMain::preEndField() {}
void CGameMain::endField() {
    m_pSubField->update();
    if (m_pSubField->getGameSubState() == 6) endFieldSub();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", endFieldSub__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x41835683\n.word 0x5B815E83\n.word 0x5B818283\n.word 0x498F6883\n.word 0x6081B997\n.word 0x0000000A\n.text\n");
void CGameMain::initNextFloor() {
    INT nIndex = CSceneInfo::getNextFloorIndex();
    SHORT nFloor = CSceneInfo::getFloorNumber(nIndex, CGame::isSoraGame());
    TRACE_COL(_TTY_RED, " %d \x8aK\x82\xd6\x88\xda\x93\xae\x82\xb5\x82\xdc\x82\xb7\n", nFloor);
    WORLD_ID nWorld = CSceneInfo::getWorldID(nIndex);
    if (nWorld == 0) nWorld = (nFloor == 13) ? (WORLD_ID)13 : (WORLD_ID)0;
    CSceneInfo::initFloor(nIndex, nWorld);
}
void CGameMain::preRetryField() {
    BOOL bRes = CSceneInfo::reqChangeBackupRoom();
#line 1587
    ASSERT(bRes);
    m_pSubField->reqChangeGameSubState(5);
    m_bPauseEnable = FALSE;
    CMainApp::enableDbgUsrPause(FALSE, TRUE);
    suspendTaskInGame(1);
}
void CGameMain::retryField() {
    m_pSubField->update();
    if (m_pSubField->getGameSubState() == 6) {
        CSceneInfo::update();
        resetSceneEnv();
        CGameWork::setInt(1, 0xe2, 0);
        endFieldSub();
    }
}
void CGameMain::preSetupBattle() {
#line 1627
    ASSERT(m_pSubBattle);
    resumeTaskInGame(1);
    if (m_pSubBattle->getGameSubState() == 6) m_pSubBattle->reqChangeGameSubState(3);
}
void CGameMain::setupBattle() {
    m_pSubBattle->update();
    INT nState = m_pSubBattle->getGameSubState();
    if (m_pSubBattle->isAtelStarted() && (m_nGameProcFlag & 0x10000)) {
        if (nState != 2) {
            nState = 2;
            m_pSubBattle->reqChangeGameSubState(2);
            m_pSubBattle->update();
        }
        m_nGameProcFlag |= 0x40000000;
    }
    if (nState == 2) changeState(9);
}
void CGameMain::prePlayBattle() {
    m_bPauseEnable = TRUE;
    CMainApp::enableDbgUsrPause(TRUE, FALSE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", playBattle__9CGameMain);
void CGameMain::gotoEndBattle() {
    if (CSceneInfo::isMinigameScene()) CMinigameManager::sendEvent(1, NULL);
    changeState(10);
}
void CGameMain::preEndBattle() {
    m_bPauseEnable = FALSE;
    CMainApp::enableDbgUsrPause(FALSE, TRUE);
    suspendTaskInGame(1);
    m_pSubBattle->reqChangeGameSubState(4);
}
void CGameMain::endBattle() {
    m_pSubBattle->update();
    if (m_pSubBattle->getGameSubState() == 6) endBattleSub();
}
void CGameMain::endBattleSub() {
    BOOL bChangeWorld = FALSE;
    SHORT nCurArea = CSceneInfo::getCurrentAreaNum();
    SHORT nNextArea = CSceneInfo::getNextAreaNum();
    SHORT nNextScene = CSceneInfo::getNextSceneNum();
    BOOL bNextEvent = (USHORT)(nNextScene - 2000) < 1000;
    BOOL bChangeFloor = CSceneInfo::hasReqChangeFloor();
    BOOL bChangeArea = (nCurArea != nNextArea);
    if (bChangeArea && (nCurArea == 0 || nNextArea == 0)) bChangeWorld = TRUE;
    if (bNextEvent || bChangeFloor || bChangeArea ||
        (!CGame::isDbgStartFromBattle() && (USHORT)(CSceneInfo::getCurrentSceneNum() - 1050) < 149)) {
        m_pSubBattle->terminate();
        delete m_pSubBattle;
        m_pSubBattle = NULL;
        BOOL bRemovePlayer = FALSE;
        BOOL bRemoveBattlePlayer = FALSE;
        BOOL bWorld10 = CSceneInfo::getCurrentWorldID(TRUE) == 10;
        if (CSceneInfo::getCurrentSceneNum() != nNextScene || bChangeArea || bChangeFloor) {
            if (m_pSubField) {
                m_pSubField->terminate();
                delete m_pSubField;
                m_pSubField = NULL;
            }
            if (CSceneInfo::checkMapRsrcCache(CRcf::getMapID(nNextArea, nNextScene), 1) < 0) {
                CSceneInfo::removeMapRsrcCache(1);
            }
            bRemoveBattlePlayer = CGame::isSoraGame() != 0;
            if (bChangeWorld) {
                INT nWorld = CSceneInfo::getCurrentWorldID(FALSE);
                if (nWorld == 6 || nWorld == 10) {
                    bRemovePlayer = TRUE;
                    bRemoveBattlePlayer = TRUE;
                }
            }
        }
        if (!bWorld10) {
            CSceneInfo::removeMapRsrcCache(2);
            if (bNextEvent) {
                if (CGame::isSoraGame()) {
                    bRemovePlayer = TRUE;
                    bRemoveBattlePlayer = TRUE;
                }
                removeInconstRsrc();
            }
        }
        if (bRemovePlayer) PLM::removePlayerRsrc(0);
        if (bRemoveBattlePlayer) PLM::removeBattlePlayerRsrc(0);
        if (bChangeWorld) {
            removeInconstWorldRsrc();
            removeConstWorldRsrc();
        }
        if (bNextEvent) CSound::clearForBattleScene();
    }
    CSceneInfo::update();
    CPlaySpeedCtrl::setPlaySpeed(1.0f);
    CPackMan::setFrameEffect(0, 0, 0, 0);
    if (m_pSubField && CGame::isDbgStartFromBattle()) {
        CGameWork::rollback(CGameWork::BACKUP_PRIMARY);
    } else if (bChangeFloor) {
        removeConstFloorRsrc();
        initNextFloor();
    }
    INT nState;
    if (bChangeWorld) {
        nState = 1;
    } else if (m_pSubBattle || (USHORT)(nNextScene - 2000) < 1000) {
        nState = 3;
    } else {
        nState = 2;
    }
    changeState(nState);
}
void CGameMain::gotoRetryBattle() {
    if (CSceneInfo::isMinigameScene()) CMinigameManager::sendEvent(2, NULL);
    changeState(11);
}
void CGameMain::preRetryBattle() {
    m_bPauseEnable = FALSE;
    CMainApp::enableDbgUsrPause(FALSE, TRUE);
    suspendTaskInGame(1);
    CGameWork::setFlag(1, 0x2d, FALSE);
    m_pSubBattle->reqChangeGameSubState(4);
}
void CGameMain::retryBattle() {
    m_pSubBattle->update();
    if (m_pSubBattle->getGameSubState() == 6) {
        CPlaySpeedCtrl::setPlaySpeed(1.0f);
        CPackMan::setFrameEffect(0, 0, 0, 0);
        BOOL bMini = (USHORT)(CSceneInfo::getCurrentBattleSceneNum() - 1200) < 56;
        if (!bMini) resetSceneEnv();
        CGameWork::setInt(1, 0xe2, 0);
        if (bMini) {
            changeState(8);
        } else if (!CSceneInfo::reqChangeBackupRoom()) {
            CStatus::setHP(CStatus::getHPMax());
            changeState(8);
        } else {
            endBattleSub();
            PLM::g_bIsLastSceneBattle = FALSE;
        }
    }
}
BOOL CGameMain::reqPlayMovie(const CHAR* pszMovieFile, BOOL bSkipEnable) {
    if (m_nGameProcFlag & 0xc01f003f) {
        TRACE_USER(TGOTO, "\x89\xbd\x82\xa9\x91\xbc\x82\xcc\x83\x8a\x83N\x83G\x83X\x83g\x82\xaa\x93\xfc\x82\xc1\x82\xc4\x82\xa2\x82\xe9\x82\xcc\x82\xc5\x83\x80\x81[\x83r\x81[\x8d\xc4\x90\xb6\x82\xd6\x88\xda\x8ds\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\n");
        return FALSE;
    }
    if (!pszMovieFile || !*pszMovieFile) {
        TRACE_USER(TGOTO, "\x83\x80\x81[\x83r\x81[\x83t\x83@\x83" "C\x83\x8b\x82\xaa\x8ew\x92\xe8\x82\xb3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb9\x82\xf1\n");
        return FALSE;
    }
    strcpy(m_szMovieFileName, pszMovieFile);
    INT nFlag = 0x10000;
    if (bSkipEnable) nFlag = 0x30000;
    m_nGameProcFlag |= nFlag;
    return TRUE;
}
void CGameMain::prePlayMovie() {
    m_nGameProcFlag |= 0x300000;
    if (!CAtel::isFadeActive()) {
        CGame::reqBlackFadeOut(30);
        CSound::fadeoutAllBGM(20);
        m_nGameProcFlag &= ~0x1000000;
    } else if (CAtel::isFadeActiveWhite()) {
        m_nGameProcFlag |= 0x1000000;
    } else {
        CAtel::clearScreenFader(-1);
        CGame::setBlack();
    }
    CSound::fadeAllSE(20, 0);
    if (m_pSubField && m_pSubField->isAtelStarted()) m_pSubField->suspendAtel();
    if (m_pSubBattle && m_pSubBattle->isAtelStarted()) m_pSubBattle->suspendAtel();
    m_nCockpitDispBackup = CGameWork::getInt(1, 0xe2);
    CGameWork::setInt(1, 0xe2, -1);
    CMovie::setupScript(m_szMovieFileName);
    m_nStep = 0;
}
void CGameMain::playMovie() {
    switch (m_nStep) {
    case 0:
        if (!CGame::isFadeDone()) return;
        if (!CMovie::isCompleteSetupScript()) return;
        m_ivFogColorBackup = CWorldEnv::getFogColor();
        CWorldEnv::setFogColor(0, 0, 0);
        suspendTaskInGame(7);
        CSound::stopBGM(0);
        m_nStep++;
        break;
    case 1:
        if (CMovie::open(m_szMovieFileName)) {
            BOOL bSkip = m_nGameProcFlag & 0x20000;
            UINT nFlags = bSkip != FALSE;
            CMovie::play(CGameWork::getFlag(1, 0x36) ? nFlags | 6 : nFlags);
            m_nStep++;
        } else {
            TRACE("\x83\x80\x81[\x83r\x81[\x82\xcc\x83I\x81[\x83v\x83\x93\x82\xc9\x8e\xb8\x94s\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x81" "B\x83X\x83L\x83" "b\x83v\x82\xb5\x82\xdc\x82\xb7\x81" "B\n");
            m_nStep = 10;
        }
        break;
    case 2:
        m_nGameProcFlag &= ~0x200000;
        m_nStep++;
        break;
    case 3:
        if (m_nGameProcFlag & 0x1000000) {
            CAtel::reqWhiteFadeIn(20);
        } else {
            CGame::dispOffFadeMask();
        }
        m_nStep++;
        break;
    case 4: {
        INT nStatus = CMovie::getStatus();
        if (nStatus != 7 && nStatus != 9) return;
        if (CGameWork::getFlag(1, 0x36)) {
            if (nStatus == 7) {
                CGame::reqWhiteFadeOut(20);
            } else {
                CGame::setWhite();
            }
        } else {
            CGame::setBlack();
        }
        m_nStep++;
        break;
    }
    case 5:
        if (!CGame::isFadeDone()) return;
        CMovie::close();
        m_nStep = 10;
        break;
    case 10:
        CMovie::disposeScriptRsrc();
        m_nStep++;
        break;
    case 11:
        CWorldEnv::setFogColor(m_ivFogColorBackup.x, m_ivFogColorBackup.y, m_ivFogColorBackup.z);
        resumeTaskInGame(3);
        m_nStep++;
        break;
    case 12:
        m_nStep++;
        break;
    case 13:
        if (!CSceneInfo::isEventScene()) {
            if (m_nGameProcFlag & 0x80000) {
                CGame::setBlack();
                m_nTimer = 20;
                m_nStep = 20;
                return;
            }
            if (CGameWork::getFlag(1, 0x36)) {
                CGame::reqWhiteFadeIn(20);
            } else {
                CGame::reqBlackFadeIn(20);
            }
            CSound::fadeAllSE(20, 0x7f);
        }
        m_nStep++;
        break;
    case 14: {
        if (CSceneInfo::isEventScene() && !CGame::isFadeDone()) return;
        INT nState = 5;
        if (m_nGameProcFlag & 0x40000) {
            nState = 9;
        } else if (m_nGameProcFlag & 0x80000) {
            nState = 4;
        }
        changeState(nState);
        m_nGameProcFlag &= ~0x13f0000;
        if (!CSceneInfo::isEventScene()) CGameWork::setInt(1, 0xe2, m_nCockpitDispBackup);
        TRACE_USER(TGOTO, "CGameMain : Movie -> Normal\n");
        break;
    }
    case 20:
        if (--m_nTimer > 0) return;
        m_nStep = 14;
        break;
    }
}
BOOL CGameMain::reqSave() {
    if (!isPauseEnable()) return FALSE;
    m_nGameProcFlag |= 0x1000;
#line 2190
    GAME_NEW(2190, CSaveLoad)(this, (SAVELOAD)0, (GAMECLEAR)0);
    return TRUE;
}
BOOL CGameMain::reqGameOver(BOOL bHeartless) {
    m_nGameProcFlag |= 1;
    if (bHeartless) m_nGameProcFlag |= 2;
    return TRUE;
}
BOOL CGameMain::reqContinue_GameOver() {
    if (m_nGameProcFlag & 8) {
        m_nGameProcFlag |= 0x10;
        return TRUE;
    }
    return FALSE;
}
BOOL CGameMain::reqExit_GameOver() {
    if (m_nGameProcFlag & 8) {
        m_nGameProcFlag |= 0x20;
        return TRUE;
    }
    return FALSE;
}
void CGameMain::preGameover() {
#line 2236
    ASSERT(0 == (m_nGameProcFlag & _PROCFLAG_GAMEOVER_PROC));
    m_nGameProcFlag |= _PROCFLAG_GAMEOVER_PROC;
#line 2239
    GAME_NEW(2239, CGameOver)(this, (m_nGameProcFlag >> 1) & 1);
}
void CGameMain::gameover() {
    if (m_nGameProcFlag & 0x30) {
        if (m_nGameProcFlag & 0x10) {
            if (m_nGameProcFlag & 4) {
                gotoRetryBattle();
            } else {
                changeState(7);
            }
        } else {
            changeState(14);
        }
        m_nGameProcFlag = 0;
    }
}
void CGameMain::resetSceneEnv() {
    FVECTOR v;
    v.set(1.0f, 1.0f, 1.0f);
    CSceneEnv::setColor((ObjType::eObjBit)0xfffeffff, 0.0f, v, NULL, 0);
    CSceneEnv::setStop((ObjType::eObjBit)0);
}
void CGameMain::preExit() {
    CGame::setFadeMaskProcPrio(CGame::FADE_PROC_PRIO_1);
    CGame::reqBlackFadeOut(60);
}
void CGameMain::exit() {
    if (CGame::isFadeDone()) CMainApp::reqSoftReset(SOFTRESET_SKIP_LOGO);
}
BOOL CGameMain::reqCompleteGame() {
    m_nGameProcFlag |= 0x80000000;
    if (m_pSubField && m_pSubField->getAtel()) m_pSubField->getAtel()->setActive(FALSE);
    if (m_pSubBattle && m_pSubBattle->getAtel()) m_pSubBattle->getAtel()->setActive(FALSE);
    CWorldEnv::readyMike(FALSE, FALSE);
    return TRUE;
}
void CGameMain::preCompleteGame() {
    if (!CAtel::isFadeActive()) {
        CGame::reqBlackFadeOut(60);
        CSound::fadeoutAllBGM(40);
        CSound::fadeoutAllSE(40, TRUE);
        CSound::fadeoutAllVoice(40);
        m_nStep = 0;
    } else {
        m_nStep = 1;
    }
}
void CGameMain::completeGame() {
    switch (m_nStep) {
    case 0:
        if (!CGame::isFadeDone()) return;
        CWorldEnv::setFogColor(0, 0, 0);
        break;
    case 1:
        if (m_pSubField) {
            m_pSubField->terminate();
            delete m_pSubField;
            m_pSubField = NULL;
        }
        if (m_pSubBattle) {
            m_pSubBattle->terminate();
            delete m_pSubBattle;
            m_pSubBattle = NULL;
        }
        break;
    case 2:
        CAtel::clearScreenFader(-1);
        CGame::setFadeMaskProcPrio(CGame::FADE_PROC_PRIO_0);
        CGame::dispOffFadeMask();
        CTitleReportDllStarter::create(CTaskMan::getSysTask(1), 2, 0);
        break;
    default:
        return;
    }
    m_nStep++;
}
void CGameMain::preExitTheaterMode() { m_nStep = 0; }
void CGameMain::exitTheaterMode() {
    if (m_nStep == 0) {
        if (m_pSubField) {
            m_pSubField->terminate();
            delete m_pSubField;
            m_pSubField = NULL;
        }
        if (m_pSubBattle) {
            m_pSubBattle->terminate();
            delete m_pSubBattle;
            m_pSubBattle = NULL;
        }
        CRsrcDB::clearSceneAttachMode();
        CRsrcDB::removeGroup(0, 2);
        CRsrcDB::removeGroup(0, CGame::isSoraGame() ? 5 : 6);
        resetSceneEnv();
        out();
        m_nStep++;
    }
}
void CGameMain::setBlackBeforeEncount(BOOL bBlack) { m_bBlackBeforeEncount = bBlack; }
BOOL CGameMain::isBlackBeforeEncount() const { return m_bBlackBeforeEncount; }
CGame::CGame(CTask* pParent, INT nMode) : CTask(pParent, 0x8000) {
    ignorePlaySpeed(TRUE);
    CNowLoading::setMode(0);
#line 2505
    ASSERT(__null == s_pMain);
    s_nDbgGameMode = nMode;
    s_nGameStep = 0;
}
CGame::CGame(CTask* pParent) : CTask(pParent, 0x8000) {
    ignorePlaySpeed(TRUE);
    CNowLoading::setMode(0);
#line 2519
    ASSERT(__null == s_pMain);
    s_nGameStep = 0;
    s_nDbgGameMode = 0;
}
CGame::~CGame() {
    s_pFadeMask = NULL;
    CAtel::disposeScreenFader();
}
void CGame::main() {
    switch (s_nGameStep) {
    case 0:
        CMainApp::startCountPlayTime();
#line 2551
        s_pMain = GAME_NEW(2551, CGameMain)(*this);
        ASSERT(s_pMain);
#line 2555
        s_pFadeMask = GAME_NEW(2555, CFadeIOMask)(*this, 0xb);
        s_nFadeProcPrio = FADE_PROC_PRIO_0;
        CAtel::createScreenFader();
#line 2561
        GAME_NEW(2561, CPreGameParent)(this);
#line 2564
        GAME_NEW(2564, CGameParent)(this);
        s_nGameStep = 1;
        break;
    case 1:
        if (!s_pMain) out();
        break;
    }
}
BOOL CGame::isSoraGame() { return CGameWork::getByte(0, 0x60) == 0; }
BOOL CGame::isRikuGame() { return CGameWork::getByte(0, 0x60) != 0; }
BOOL CGame::reqPlayMovie(const CHAR* pszName, INT nMode) {
    if (s_pMain) return s_pMain->reqPlayMovie(pszName, nMode);
    return FALSE;
}
BOOL CGame::reqSave() {
    if (s_pMain) return s_pMain->reqSave();
    return FALSE;
}
BOOL CGame::reqGameOver(BOOL bHeartless) {
    if (s_pMain) return s_pMain->reqGameOver(bHeartless);
    return FALSE;
}
BOOL CGame::reqContinue_GameOver() {
    if (s_pMain) return s_pMain->reqContinue_GameOver();
    return FALSE;
}
BOOL CGame::reqExit_GameOver() {
    if (s_pMain) return s_pMain->reqExit_GameOver();
    return FALSE;
}
BOOL CGame::reqCompleteGame() {
    if (s_pMain) return s_pMain->reqCompleteGame();
    return FALSE;
}
INT CGame::getMapID() {
    if (s_pMain) return s_pMain->getMapID();
    return 0;
}
void CGame::enablePause(BOOL bEnable) {
    if (s_pMain) s_pMain->enablePause(bEnable);
}
BOOL CGame::hasPauseRequest() { return FALSE; }
void CGame::checkPause() {
    if (!s_pMain) return;
    if (s_pMain->isInactive()) return;
    if (s_pMain->checkExitPause()) return;
    if (CSceneInfo::hasAnyRequest()) return;
    if (CGameWork::getFlag(1, 0x33) && CGameWork::getFlag(1, 0x34)) return;
    if (!s_pMain->isPauseEnable()) return;
    switch (s_pMain->getState()) {
    case 5:
        s_pMain->checkStartFieldMenu();
        break;
    case 9:
        s_pMain->checkStartBattleMenu();
        break;
    }
}
void CGame::suspendGameTask() {
    CTask* p = CTaskMan::getSysTask(0xf);
    if (p) p->setActive(FALSE);
    p = CTaskMan::getSysTask(1);
    if (p) p->setActive(FALSE);
}
void CGame::resumeGameTask() {
    CTask* p = CTaskMan::getSysTask(0xf);
    if (p) p->setActive(TRUE);
    p = CTaskMan::getSysTask(1);
    if (p) p->setActive(TRUE);
}
BOOL CGame::isSuspended() {
    if (!s_pMain) return FALSE;
    return CTaskMan::getSysTask(1)->isInactive();
}
void CGame::playConstEnvSE(INT nSeq, INT nVol, BOOL bForce) {
    if (CSceneInfo::isMinigameScene() && !bForce) return;
    CSound::playConstEnvSE(nSeq, nVol, 30);
}
void CGame::stopConstEnvSE(INT nTime) { CSound::stopConstEnvSE(nTime < 0 ? 30 : nTime); }
void CGame::recoverBGM(INT nFlag) { CSound::setBGMVolume(0, 0x7f, 30); }
BOOL CGame::isFadeMaskAvailable() {
    if (!s_nFadeProcPrio) {
        if (!s_pMain) return TRUE;
        if (s_pMain->isAtelFadeActive()) return FALSE;
    }
    return TRUE;
}
void CGame::setFadeMaskProcPrio(_FADE_PROC_PRIO ePrio) { s_nFadeProcPrio = ePrio; }
CGame::_FADE_PROC_PRIO CGame::getFadeMaskProcPrio() { return s_nFadeProcPrio; }
void CGame::reqBlackFadeIn(USHORT nTime) {
    if (!isFadeMaskAvailable()) return;
    FLOAT fAlpha = isFadeMaskActive() ? s_pFadeMask->getAlpha() : 0.0f;
    s_pFadeMask->reqBlackIn(nTime, fAlpha);
}
void CGame::reqBlackFadeOut(USHORT nTime) {
    if (!isFadeMaskAvailable()) return;
    FLOAT fAlpha = isFadeMaskActive() ? s_pFadeMask->getAlpha() : 1.0f;
    s_pFadeMask->reqBlackOut(nTime, fAlpha);
}
void CGame::reqWhiteFadeIn(USHORT nTime) {
    if (!isFadeMaskAvailable()) return;
    FLOAT fAlpha = isFadeMaskActive() ? s_pFadeMask->getAlpha() : 1.0f;
    s_pFadeMask->reqWhiteIn(nTime, fAlpha);
}
void CGame::reqWhiteFadeOut(USHORT nTime) {
    if (!isFadeMaskAvailable()) return;
    FLOAT fAlpha = isFadeMaskActive() ? s_pFadeMask->getAlpha() : 0.0f;
    s_pFadeMask->reqWhiteOut(nTime, fAlpha);
}
void CGame::setBlack() {
    if (isFadeMaskAvailable()) s_pFadeMask->setBlack(0.0f);
}
void CGame::setWhite() {
    if (isFadeMaskAvailable()) s_pFadeMask->setWhite(1.0f);
}
void CGame::dispOffFadeMask() { s_pFadeMask->off(); }
BOOL CGame::isFadeMaskActive() { return !s_pFadeMask->isInactive() && s_pFadeMask->isVisible(); }
BOOL CGame::isFadeDone() {
    BOOL bRes = FALSE;
    if (!isFadeMaskActive() || s_pFadeMask->isFadeDone()) bRes = TRUE;
    return bRes;
}
void CGame::setBlackBeforeEncount(BOOL bFlag) {
    if (s_pMain) s_pMain->setBlackBeforeEncount(bFlag);
}
BOOL CGame::isBlackBeforeEncount() {
    if (s_pMain) return s_pMain->isBlackBeforeEncount();
    return TRUE;
}
void CGame::setupConfig() {
    INT nMode;
    switch (CGameWork::getByte(0, 0x62)) {
    case 1:
        nMode = 2;
        break;
    case 2:
        nMode = 1;
        break;
    default:
        nMode = 0;
        break;
    }
    CSound::setPlayMode(nMode);
    CPadIO::enableVibration(CGameWork::getByte(0, 0x63) == 0, 0);
}
void CGame::startFromNewGame() {
    if (!CTaskMan::reqChangeMainTask(2, (MAINTASK_CALLMODE)1)) {
        TRACE("\x83Q\x81[\x83\x80\x8aJ\x8en\x82\xc9\x8e\xb8\x94s\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x81" "BMAIN-TASK \x82\xf0\x90\xd8\x82\xe8\x91\xd6\x82\xa6\x82\xe7\x82\xea\x82\xdc\x82\xb9\x82\xf1\n");
        return;
    }
    setupConfig();
    USHORT nDeck = 1;
    if (!isSoraGame()) nDeck = 2;
    CCard::initCard(nDeck, (WORLD_ID)0, 0);
    CStatus::initStatus(nDeck, (WORLD_ID)0, 0);
    CSceneInfo::reqChangeRoom(0, (DOOR_ID)2, -1);
    CSceneInfo::update();
    CMainApp::enableDbgUsrPause(TRUE, FALSE);
}
void CGame::startFromLoadGame() {
    if (!CTaskMan::reqChangeMainTask(2, (MAINTASK_CALLMODE)1)) {
        TRACE("\x83Q\x81[\x83\x80\x8aJ\x8en\x82\xc9\x8e\xb8\x94s\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x81" "BMAIN-TASK \x82\xf0\x90\xd8\x82\xe8\x91\xd6\x82\xa6\x82\xe7\x82\xea\x82\xdc\x82\xb9\x82\xf1\n");
        return;
    }
    CMainApp::setFpsFull();
    setupConfig();
    CSceneInfo::initFloor();
    CSceneInfo::reqChangeRoom(CGameWork::progMCW()._nRoomID, (DOOR_ID)CGameWork::progMCW()._nDoorID, -1);
    CSceneInfo::update();
    CMainApp::enableDbgUsrPause(TRUE, FALSE);
}
void CGame::startTheaterMode(CTask* pParent, BOOL bSora, SHORT nArea, SHORT nScene) {
    CPadIO::enableVibration(FALSE, 0);
    CSceneInfo::clear();
    CSceneInfo::initNewGame(bSora);
    CSceneInfo::ignoreFloorInfo();
    CSceneInfo::reqChangeScene(nArea, nScene, -1, 1, 1);
    CSceneInfo::update();
#line 2961
    GAME_NEW(2961, CGame)(pParent);
}
BOOL CGame::isExitTheaterMode() {
    BOOL bRes = (s_pMain == NULL);
    if (bRes && CMainApp::isFpsHalf()) CMainApp::setFpsFull();
    return bRes;
}
INT CGame::getDbgGameMode() { return s_nDbgGameMode; }
void CGame::dbgStartFromBattle(BOOL bBattle) { s_bDbgStartFromBattle = bBattle; }
BOOL CGame::isDbgStartFromBattle() { return s_bDbgStartFromBattle; }
// SOURCE DATA: .data .rodata
