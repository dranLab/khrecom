// PARTIAL: added for Game/Gimmick/GimmickSavePoint
#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "khtypes.h"
#include "System/Task.h"

class CGame : public CTask {
public:
    virtual ~CGame();
    virtual void main();
    CGame();
    CGame(CTask* pParent);              // PARTIAL: added for System/MainTaskFactory
    CGame(CTask* pParent, INT nMode);   // PARTIAL: added for System/MainTaskFactory
    static BOOL reqSave();
    static BOOL isSoraGame(); // PARTIAL: added for Game/Gimmick/GimmickWarpPoint
    static INT getDbgGameMode(); // PARTIAL: added for Game/Status
    static BOOL isRikuGame(); // PARTIAL: added for Game/Player/NaviMap
    static void setupConfig(); // PARTIAL: added for Form/Menu/Camp
    static void enablePause(BOOL bEnable); // PARTIAL: added for Form/Menu/GameOver
    static BOOL reqContinue_GameOver();    // PARTIAL: added for Form/Menu/GameOver
    static BOOL reqExit_GameOver();        // PARTIAL: added for Form/Menu/GameOver
    static BOOL isSuspended();             // PARTIAL: added for Game/SceneEnv
    static BOOL isPause() { return isSuspended(); } // PARTIAL: added for Game/Game
    static INT getMapID();                 // PARTIAL: added for System/DebugProcInfo
    static void dbgStartFromBattle(BOOL bBattle); // PARTIAL: added for Form/DebugMenu
    static void startFromLoadGame();              // PARTIAL: added for Form/DebugMenu
    // PARTIAL: added for Game/Player/PlayerManager
    static void reqWhiteFadeOut(USHORT inTime);
    static void reqWhiteFadeIn(USHORT inTime);
    static BOOL isFadeDone();
    static void recoverBGM(INT inFlag);
    static void reqGameOver(INT inFlag);
    static void suspendGameTask(); // PARTIAL: added for System/TaskMan
    static void checkPause();
    // PARTIAL: added for Game/GameSubField
    enum _FADE_PROC_PRIO { FADE_PROC_PRIO_0 = 0, FADE_PROC_PRIO_1 = 1 };
    static BOOL isDbgStartFromBattle();
    static BOOL isBlackBeforeEncount();
    static void reqBlackFadeIn(USHORT inTime);
    static void reqBlackFadeOut(USHORT inTime);
    static BOOL reqPlayMovie(const CHAR* pszName, INT nMode);
    static void setBlack();
    static void setFadeMaskProcPrio(_FADE_PROC_PRIO ePrio);
    // PARTIAL: added for Game/GameSubBattle
    static void setWhite();
    static void dispOffFadeMask();
    static INT getFadeMaskProcPrio(); // PARTIAL: added for Game/Atel/Atel
    static void setBlackBeforeEncount(BOOL bFlag);
};

#endif /* GAME_GAME_H */
