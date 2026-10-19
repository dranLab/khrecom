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
#include "include_asm.h"

// PARTIAL: System/RsrcDB.h would add RsrcData.h's string to .rodata
class CRsrcDB {
public:
    static void removeGroup(INT nGroupID, INT nMemberID);
};

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
#define GAME_NEW(line, T) new (com_malloc_d(sizeof(T), __FILE__ "(" #line ")", RET_ADDR())) T
/* PARTIAL: __FILE__ is emitted with pause() (INCLUDE_ASM): reference that copy until it is C */
extern "C" char D_0047DFD0[];
#define ASSERT(e) if (!(e)) AssertApp(D_0047DFD0, __LINE__, #e, 0)

class CGameSub;

class CGameMain : public CTask, public CFSM, public CPadHandler {
private:
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", getMapID__C9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", isAtelFadeActive__C9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", suspendTaskInGame__9CGameMaini);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", resumeTaskInGame__9CGameMaini);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", pause__9CGameMainii);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x61472F65\n.word 0x632E656D\n.word 0x35287070\n.word 0x00293332\n.word 0x00000000\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x61472F65\n.word 0x632E656D\n.word 0x00007070\n.word 0x00000070\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", camp__9CGameMaini);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x61472F65\n.word 0x632E656D\n.word 0x35287070\n.word 0x00293335\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", worldMap__9CGameMaini);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x61472F65\n.word 0x632E656D\n.word 0x35287070\n.word 0x00293837\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", checkStartFieldMenu__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", procFieldMenu__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", checkStartBattleMenu__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", procBattleMenu__9CGameMain);
BOOL CGameMain::checkExitPause() {
    if (m_nGameProcFlag & 0x800) {
        m_nGameProcFlag &= ~0xc00;
        return TRUE;
    }
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqLoadConstFloorRsrc__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x8D837483\n.word 0xED8F4183\n.word 0x8A839392\n.word 0x5B815C83\n.word 0xCC825883\n.word 0xE892778E\n.word 0xCD94AA82\n.word 0x4F8ACD88\n.word 0xB782C582\n.word 0x000A4281\n.word 0x6147435B\n.word 0x614D656D\n.word 0x3A5D6E69\n.word 0x2D2D2D20\n.word 0x616F4C20\n.word 0x6F432064\n.word 0x2D74736E\n.word 0x6F6F6C46\n.word 0x73522D72\n.word 0x2D206372\n.word 0x3A202D2D\n.word 0x6F6C4620\n.word 0x3D20726F\n.word 0x0A642520\n.word 0x00000000\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", removeConstFloorRsrc__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x6147435B\n.word 0x614D656D\n.word 0x3A5D6E69\n.word 0x2D2D2D20\n.word 0x6D655220\n.word 0x2065766F\n.word 0x736E6F43\n.word 0x6C462D74\n.word 0x2D726F6F\n.word 0x63727352\n.word 0x2D2D2D20\n.word 0x46203A20\n.word 0x726F6F6C\n.word 0x25203D20\n.word 0x00000A64\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqLoadConstWolrdRsrc__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x5B818F83\n.word 0x68838B83\n.word 0x9392ED8F\n.word 0x5C838A83\n.word 0x58835B81\n.word 0x778ECC82\n.word 0xAA82E892\n.word 0xCD88CD94\n.word 0xC8824F8A\n.word 0xC582CC82\n.word 0x4C835883\n.word 0x76836283\n.word 0xDC82B582\n.word 0x4281B782\n.word 0x0000000A\n.word 0x00000000\n.word 0x6147435B\n.word 0x614D656D\n.word 0x3A5D6E69\n.word 0x2D2D2D20\n.word 0x616F4C20\n.word 0x6F432064\n.word 0x2D74736E\n.word 0x6C726F57\n.word 0x73522D64\n.word 0x2D206372\n.word 0x3A202D2D\n.word 0x726F5720\n.word 0x3D20646C\n.word 0x0A642520\n.word 0x00000000\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", removeConstWorldRsrc__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x6147435B\n.word 0x614D656D\n.word 0x3A5D6E69\n.word 0x2D2D2D20\n.word 0x6D655220\n.word 0x2065766F\n.word 0x736E6F43\n.word 0x6F572D74\n.word 0x2D646C72\n.word 0x63727352\n.word 0x2D2D2D20\n.word 0x57203A20\n.word 0x646C726F\n.word 0x25203D20\n.word 0x00000A64\n.word 0x00000000\n.text\n");
void CGameMain::reqLoadInconstGameRsrc() {
    if (!m_bInconstGameRsrcLoaded) {
        CRcf::reqLoadRCF(0, 3, 0x100, NULL, 0);
        m_bInconstGameRsrcLoaded = TRUE;
        TRACE_COL(_TTY_RED, "[CGameMain]: --- Load Inconst GameRsrc ---\n");
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqLoadInconstWorldRsrc__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x5B818F83\n.word 0x68838B83\n.word 0x27BC9427\n.word 0x9392ED8F\n.word 0x5C838A83\n.word 0x58835B81\n.word 0x778ECC82\n.word 0xAA82E892\n.word 0xCD88CD94\n.word 0xC8824F8A\n.word 0xC582CC82\n.word 0x4C835883\n.word 0x76836283\n.word 0xDC82B582\n.word 0x4281B782\n.word 0x0000000A\n.word 0x6147435B\n.word 0x614D656D\n.word 0x3A5D6E69\n.word 0x2D2D2D20\n.word 0x616F4C20\n.word 0x6E492064\n.word 0x736E6F63\n.word 0x6F572D74\n.word 0x2D646C72\n.word 0x63727352\n.word 0x2D2D2D20\n.word 0x57203A20\n.word 0x646C726F\n.word 0x25203D20\n.word 0x00000A64\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", removeInconstWorldRsrc__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x6147435B\n.word 0x614D656D\n.word 0x3A5D6E69\n.word 0x2D2D2D20\n.word 0x6D655220\n.word 0x2065766F\n.word 0x6F636E49\n.word 0x2D74736E\n.word 0x6C726F57\n.word 0x73522D64\n.word 0x2D206372\n.word 0x3A202D2D\n.word 0x726F5720\n.word 0x3D20646C\n.word 0x0A642520\n.word 0x00000000\n.text\n");
void CGameMain::removeInconstRsrc() {
    removeInconstWorldRsrc();
    if (m_bInconstGameRsrcLoaded) {
        CRsrcDB::removeGroup(0, 3);
        m_bInconstGameRsrcLoaded = FALSE;
        TRACE_COL(_TTY_RED, "[CGameMain]: --- Remove Inconst GameRsrc ---\n");
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preSetupCommon__9CGameMain);
void CGameMain::setupCommon() {
    if (CRcf::isCompleteLoadRCF(0, 0)) changeState(1);
}
void CGameMain::preSetupWorld() { m_nStep = 0; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", setupWorld__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x6147435B\n.word 0x614D656D\n.word 0x3A5D6E69\n.word 0x2D2D2D20\n.word 0x696B5320\n.word 0x6F4C2070\n.word 0x3A206461\n.word 0x74614220\n.word 0x20656C74\n.word 0x63727352\n.word 0x2D2D2D20\n.word 0x0000000A\n.text\n");
void CGameMain::preSetupBattleRsrc() { m_nStep = 0; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", setupBattleRsrc__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x61472F65\n.word 0x632E656D\n.word 0x31287070\n.word 0x29363630\n.word 0x00000000\n.word 0x53705F6D\n.word 0x61426275\n.word 0x656C7474\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preSetupField__9CGameMain);
void CGameMain::preSetupField2() { m_nStep = 2; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", setupField__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x61472F65\n.word 0x632E656D\n.word 0x31287070\n.word 0x29363531\n.word 0x00000000\n.text\n");
void CGameMain::prePlayField() {
    m_bPauseEnable = TRUE;
    CMainApp::enableDbgUsrPause(TRUE, FALSE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", playField__9CGameMain);
void CGameMain::preEndField() {}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", endField__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", endFieldSub__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x41835683\n.word 0x5B815E83\n.word 0x5B818283\n.word 0x498F6883\n.word 0x6081B997\n.word 0x0000000A\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", initNextFloor__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x20642520\n.word 0xD6824B8A\n.word 0xAE93DA88\n.word 0xDC82B582\n.word 0x000AB782\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preRetryField__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x73655262\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", retryField__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preSetupBattle__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", setupBattle__9CGameMain);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", endBattle__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", endBattleSub__9CGameMain);
void CGameMain::gotoRetryBattle() {
    if (CSceneInfo::isMinigameScene()) CMinigameManager::sendEvent(2, NULL);
    changeState(11);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preRetryBattle__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", retryBattle__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqPlayMovie__9CGameMainPCci);
__asm__(".rdata\n.align 3\n.word 0xA982BD89\n.word 0xCC82BC91\n.word 0x4E838A83\n.word 0x58834783\n.word 0xAA826783\n.word 0xC182FC93\n.word 0xA282C482\n.word 0xCC82E982\n.word 0x8083C582\n.word 0x72835B81\n.word 0xC48D5B81\n.word 0xD682B690\n.word 0x738DDA88\n.word 0xAB82C582\n.word 0xB982DC82\n.word 0x000AF182\n.word 0x5B818083\n.word 0x5B817283\n.word 0x40837483\n.word 0x8B834383\n.word 0x778EAA82\n.word 0xB382E892\n.word 0xC482EA82\n.word 0xDC82A282\n.word 0xF182B982\n.word 0x0000000A\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", prePlayMovie__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", playMovie__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x5B818083\n.word 0x5B817283\n.word 0x4983CC82\n.word 0x76835B81\n.word 0xC9829383\n.word 0x7394B88E\n.word 0xDC82B582\n.word 0xBD82B582\n.word 0x58834281\n.word 0x62834C83\n.word 0xB5827683\n.word 0xB782DC82\n.word 0x000A4281\n.word 0x00000000\n.word 0x6D614743\n.word 0x69614D65\n.word 0x203A206E\n.word 0x69766F4D\n.word 0x3E2D2065\n.word 0x726F4E20\n.word 0x0A6C616D\n.word 0x00000000\n.word 0x00000000\n.word 0x00000000\n.word 0x001F8580\n.word 0x001F85E0\n.word 0x001F8638\n.word 0x001F865C\n.word 0x001F8694\n.word 0x001F8700\n.word 0x001F88A4\n.word 0x001F88A4\n.word 0x001F88A4\n.word 0x001F88A4\n.word 0x001F8720\n.word 0x001F8734\n.word 0x001F875C\n.word 0x001F8768\n.word 0x001F87E4\n.word 0x001F88A4\n.word 0x001F88A4\n.word 0x001F88A4\n.word 0x001F88A4\n.word 0x001F88A4\n.word 0x001F888C\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqSave__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x61472F65\n.word 0x632E656D\n.word 0x32287070\n.word 0x29303931\n.word 0x00000000\n.text\n");
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preGameover__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x3D3D2030\n.word 0x5F6D2820\n.word 0x6D61476E\n.word 0x6F725065\n.word 0x616C4663\n.word 0x20262067\n.word 0x4F52505F\n.word 0x414C4643\n.word 0x41475F47\n.word 0x564F454D\n.word 0x505F5245\n.word 0x29434F52\n.word 0x00000000\n.word 0x00000000\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x61472F65\n.word 0x632E656D\n.word 0x32287070\n.word 0x29393332\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", gameover__9CGameMain);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqCompleteGame__9CGameMain);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", completeGame__9CGameMain);
void CGameMain::preExitTheaterMode() { m_nStep = 0; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", exitTheaterMode__9CGameMain);
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
