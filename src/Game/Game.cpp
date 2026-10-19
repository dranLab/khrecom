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
#include "Game/Card.h"
#include "Game/Status.h"
#include "System/Fsm.h"
#include "Game/Atel/Atel.h"
#include "Game/GameSub.h"
#include "Game/SceneInfo.h"
#include "Game/NowLoading.h"
#include "System/MainApp.h"
#include "System/Sound/SoundMan.h"
#include "Form/Menu/MoogleShop.h"
#include "Game/Player/PlayerManager.h"
#include "System/Graphic/ScreenMask.h"
#include "System/PadHandler.h"
#include "System/DebugProcInfo.h"
#include "include_asm.h"

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
    BOOL reqPlayMovie(CHAR* pszMovieFile, BOOL bSkipEnable);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", checkExitPause__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqLoadConstFloorRsrc__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x8D837483\n.word 0xED8F4183\n.word 0x8A839392\n.word 0x5B815C83\n.word 0xCC825883\n.word 0xE892778E\n.word 0xCD94AA82\n.word 0x4F8ACD88\n.word 0xB782C582\n.word 0x000A4281\n.word 0x6147435B\n.word 0x614D656D\n.word 0x3A5D6E69\n.word 0x2D2D2D20\n.word 0x616F4C20\n.word 0x6F432064\n.word 0x2D74736E\n.word 0x6F6F6C46\n.word 0x73522D72\n.word 0x2D206372\n.word 0x3A202D2D\n.word 0x6F6C4620\n.word 0x3D20726F\n.word 0x0A642520\n.word 0x00000000\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", removeConstFloorRsrc__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x6147435B\n.word 0x614D656D\n.word 0x3A5D6E69\n.word 0x2D2D2D20\n.word 0x6D655220\n.word 0x2065766F\n.word 0x736E6F43\n.word 0x6C462D74\n.word 0x2D726F6F\n.word 0x63727352\n.word 0x2D2D2D20\n.word 0x46203A20\n.word 0x726F6F6C\n.word 0x25203D20\n.word 0x00000A64\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqLoadConstWolrdRsrc__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x5B818F83\n.word 0x68838B83\n.word 0x9392ED8F\n.word 0x5C838A83\n.word 0x58835B81\n.word 0x778ECC82\n.word 0xAA82E892\n.word 0xCD88CD94\n.word 0xC8824F8A\n.word 0xC582CC82\n.word 0x4C835883\n.word 0x76836283\n.word 0xDC82B582\n.word 0x4281B782\n.word 0x0000000A\n.word 0x00000000\n.word 0x6147435B\n.word 0x614D656D\n.word 0x3A5D6E69\n.word 0x2D2D2D20\n.word 0x616F4C20\n.word 0x6F432064\n.word 0x2D74736E\n.word 0x6C726F57\n.word 0x73522D64\n.word 0x2D206372\n.word 0x3A202D2D\n.word 0x726F5720\n.word 0x3D20646C\n.word 0x0A642520\n.word 0x00000000\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", removeConstWorldRsrc__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x6147435B\n.word 0x614D656D\n.word 0x3A5D6E69\n.word 0x2D2D2D20\n.word 0x6D655220\n.word 0x2065766F\n.word 0x736E6F43\n.word 0x6F572D74\n.word 0x2D646C72\n.word 0x63727352\n.word 0x2D2D2D20\n.word 0x57203A20\n.word 0x646C726F\n.word 0x25203D20\n.word 0x00000A64\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqLoadInconstGameRsrc__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x6147435B\n.word 0x614D656D\n.word 0x3A5D6E69\n.word 0x2D2D2D20\n.word 0x616F4C20\n.word 0x6E492064\n.word 0x736E6F63\n.word 0x61472074\n.word 0x7352656D\n.word 0x2D206372\n.word 0x000A2D2D\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqLoadInconstWorldRsrc__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x5B818F83\n.word 0x68838B83\n.word 0x27BC9427\n.word 0x9392ED8F\n.word 0x5C838A83\n.word 0x58835B81\n.word 0x778ECC82\n.word 0xAA82E892\n.word 0xCD88CD94\n.word 0xC8824F8A\n.word 0xC582CC82\n.word 0x4C835883\n.word 0x76836283\n.word 0xDC82B582\n.word 0x4281B782\n.word 0x0000000A\n.word 0x6147435B\n.word 0x614D656D\n.word 0x3A5D6E69\n.word 0x2D2D2D20\n.word 0x616F4C20\n.word 0x6E492064\n.word 0x736E6F63\n.word 0x6F572D74\n.word 0x2D646C72\n.word 0x63727352\n.word 0x2D2D2D20\n.word 0x57203A20\n.word 0x646C726F\n.word 0x25203D20\n.word 0x00000A64\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", removeInconstWorldRsrc__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x6147435B\n.word 0x614D656D\n.word 0x3A5D6E69\n.word 0x2D2D2D20\n.word 0x6D655220\n.word 0x2065766F\n.word 0x6F636E49\n.word 0x2D74736E\n.word 0x6C726F57\n.word 0x73522D64\n.word 0x2D206372\n.word 0x3A202D2D\n.word 0x726F5720\n.word 0x3D20646C\n.word 0x0A642520\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", removeInconstRsrc__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x6147435B\n.word 0x614D656D\n.word 0x3A5D6E69\n.word 0x2D2D2D20\n.word 0x6D655220\n.word 0x2065766F\n.word 0x6F636E49\n.word 0x2074736E\n.word 0x656D6147\n.word 0x63727352\n.word 0x2D2D2D20\n.word 0x0000000A\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preSetupCommon__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", setupCommon__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preSetupWorld__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", setupWorld__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x6147435B\n.word 0x614D656D\n.word 0x3A5D6E69\n.word 0x2D2D2D20\n.word 0x696B5320\n.word 0x6F4C2070\n.word 0x3A206461\n.word 0x74614220\n.word 0x20656C74\n.word 0x63727352\n.word 0x2D2D2D20\n.word 0x0000000A\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preSetupBattleRsrc__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", setupBattleRsrc__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x61472F65\n.word 0x632E656D\n.word 0x31287070\n.word 0x29363630\n.word 0x00000000\n.word 0x53705F6D\n.word 0x61426275\n.word 0x656C7474\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preSetupField__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preSetupField2__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", setupField__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x61472F65\n.word 0x632E656D\n.word 0x31287070\n.word 0x29363531\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", prePlayField__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", playField__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preEndField__9CGameMain);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", prePlayBattle__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", playBattle__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", gotoEndBattle__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preEndBattle__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", endBattle__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", endBattleSub__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", gotoRetryBattle__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preRetryBattle__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", retryBattle__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqPlayMovie__9CGameMainPCci);
__asm__(".rdata\n.align 3\n.word 0xA982BD89\n.word 0xCC82BC91\n.word 0x4E838A83\n.word 0x58834783\n.word 0xAA826783\n.word 0xC182FC93\n.word 0xA282C482\n.word 0xCC82E982\n.word 0x8083C582\n.word 0x72835B81\n.word 0xC48D5B81\n.word 0xD682B690\n.word 0x738DDA88\n.word 0xAB82C582\n.word 0xB982DC82\n.word 0x000AF182\n.word 0x5B818083\n.word 0x5B817283\n.word 0x40837483\n.word 0x8B834383\n.word 0x778EAA82\n.word 0xB382E892\n.word 0xC482EA82\n.word 0xDC82A282\n.word 0xF182B982\n.word 0x0000000A\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", prePlayMovie__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", playMovie__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x5B818083\n.word 0x5B817283\n.word 0x4983CC82\n.word 0x76835B81\n.word 0xC9829383\n.word 0x7394B88E\n.word 0xDC82B582\n.word 0xBD82B582\n.word 0x58834281\n.word 0x62834C83\n.word 0xB5827683\n.word 0xB782DC82\n.word 0x000A4281\n.word 0x00000000\n.word 0x6D614743\n.word 0x69614D65\n.word 0x203A206E\n.word 0x69766F4D\n.word 0x3E2D2065\n.word 0x726F4E20\n.word 0x0A6C616D\n.word 0x00000000\n.word 0x00000000\n.word 0x00000000\n.word 0x001F8580\n.word 0x001F85E0\n.word 0x001F8638\n.word 0x001F865C\n.word 0x001F8694\n.word 0x001F8700\n.word 0x001F88A4\n.word 0x001F88A4\n.word 0x001F88A4\n.word 0x001F88A4\n.word 0x001F8720\n.word 0x001F8734\n.word 0x001F875C\n.word 0x001F8768\n.word 0x001F87E4\n.word 0x001F88A4\n.word 0x001F88A4\n.word 0x001F88A4\n.word 0x001F88A4\n.word 0x001F88A4\n.word 0x001F888C\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqSave__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x61472F65\n.word 0x632E656D\n.word 0x32287070\n.word 0x29303931\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqGameOver__9CGameMaini);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqContinue_GameOver__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqExit_GameOver__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preGameover__9CGameMain);
__asm__(".rdata\n.align 3\n.word 0x3D3D2030\n.word 0x5F6D2820\n.word 0x6D61476E\n.word 0x6F725065\n.word 0x616C4663\n.word 0x20262067\n.word 0x4F52505F\n.word 0x414C4643\n.word 0x41475F47\n.word 0x564F454D\n.word 0x505F5245\n.word 0x29434F52\n.word 0x00000000\n.word 0x00000000\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x61472F65\n.word 0x632E656D\n.word 0x32287070\n.word 0x29393332\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", gameover__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", resetSceneEnv__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preExit__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", exit__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqCompleteGame__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preCompleteGame__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", completeGame__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", preExitTheaterMode__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", exitTheaterMode__9CGameMain);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", setBlackBeforeEncount__9CGameMaini);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", isBlackBeforeEncount__C9CGameMain);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", isSoraGame__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", isRikuGame__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqPlayMovie__5CGamePCci);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqSave__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqGameOver__5CGamei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqContinue_GameOver__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqExit_GameOver__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqCompleteGame__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", getMapID__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", enablePause__5CGamei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", hasPauseRequest__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", checkPause__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", suspendGameTask__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", resumeGameTask__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", isSuspended__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", playConstEnvSE__5CGameiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", stopConstEnvSE__5CGamei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", recoverBGM__5CGamei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", isFadeMaskAvailable__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", setFadeMaskProcPrio__5CGameQ25CGame15_FADE_PROC_PRIO);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", getFadeMaskProcPrio__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqBlackFadeIn__5CGameUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqBlackFadeOut__5CGameUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqWhiteFadeIn__5CGameUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", reqWhiteFadeOut__5CGameUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", setBlack__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", setWhite__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", dispOffFadeMask__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", isFadeMaskActive__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", isFadeDone__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", setBlackBeforeEncount__5CGamei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", isBlackBeforeEncount__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", setupConfig__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", startFromNewGame__5CGame);
__asm__(".rdata\n.align 3\n.word 0x5B815183\n.word 0x4A8A8083\n.word 0xC9826E8E\n.word 0x7394B88E\n.word 0xDC82B582\n.word 0xBD82B582\n.word 0x414D4281\n.word 0x542D4E49\n.word 0x204B5341\n.word 0xD890F082\n.word 0xD691E882\n.word 0xE782A682\n.word 0xDC82EA82\n.word 0xF182B982\n.word 0x0000000A\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", startFromLoadGame__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", startTheaterMode__5CGameP5CTaskiss);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x61472F65\n.word 0x632E656D\n.word 0x32287070\n.word 0x29313639\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", isExitTheaterMode__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", getDbgGameMode__5CGame);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", dbgStartFromBattle__5CGamei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Game", isDbgStartFromBattle__5CGame);
// SOURCE DATA: .data .rodata
