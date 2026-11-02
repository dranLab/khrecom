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
    CGame(CTask* pParent);
    CGame(CTask* pParent, INT nMode);
    static BOOL reqSave();
    static BOOL isSoraGame();
    static INT getDbgGameMode();
    static BOOL isRikuGame();
    static void setupConfig();
    static void enablePause(BOOL bEnable);
    static BOOL reqContinue_GameOver();
    static BOOL reqExit_GameOver();
    static BOOL isSuspended();
    static BOOL isPause() { return isSuspended(); }
    static INT getMapID();
    static void dbgStartFromBattle(BOOL bBattle);
    static void startFromLoadGame();
    static void reqWhiteFadeOut(USHORT inTime);
    static void reqWhiteFadeIn(USHORT inTime);
    static BOOL isFadeDone();
    static void recoverBGM(INT inFlag);
    static BOOL reqGameOver(BOOL bHeartless);
    static void suspendGameTask();
    static void resumeGameTask();
    static BOOL reqCompleteGame();
    static BOOL hasPauseRequest();
    static void playConstEnvSE(INT nSeq, INT nVol, BOOL bForce);
    static void stopConstEnvSE(INT nTime);
    static BOOL isFadeMaskAvailable();
    static void checkPause();
    enum _FADE_PROC_PRIO { FADE_PROC_PRIO_0 = 0, FADE_PROC_PRIO_1 = 1 };
    static BOOL isDbgStartFromBattle();
    static BOOL isBlackBeforeEncount();
    static void reqBlackFadeIn(USHORT inTime);
    static void reqBlackFadeOut(USHORT inTime);
    static BOOL reqPlayMovie(const CHAR* pszName, INT nMode);
    static void setBlack();
    static void setFadeMaskProcPrio(_FADE_PROC_PRIO ePrio);
    static void setWhite();
    static void dispOffFadeMask();
    static _FADE_PROC_PRIO getFadeMaskProcPrio();
    static void startFromNewGame();
    static void startTheaterMode(CTask* pParent, BOOL bSora, SHORT nArea, SHORT nScene);
    static BOOL isExitTheaterMode();
    static BOOL isFadeMaskActive();
    static void setBlackBeforeEncount(BOOL bFlag);
};

#endif /* GAME_GAME_H */
