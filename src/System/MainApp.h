// PARTIAL: added for Game/CardKind
#ifndef SYSTEM_MAINAPP_H
#define SYSTEM_MAINAPP_H

#include "khtypes.h"

#ifdef __cplusplus
extern "C"
#endif
void AssertApp(char* pszFile, int nLine, char* pszMsg, char* pszExp);

// PARTIAL: added for System/Debug
extern int g_bTraceOnScreen;
#ifdef __cplusplus
extern "C"
#endif
void addTraceString(CHAR* pszSrc);

#ifdef __cplusplus
// PARTIAL: added for System/Movie/Movie
// PARTIAL: added for Form/Menu/TitleReportDllStarter
enum SOFTRESET_MODE {
    SOFTRESET_NORMAL = 0,
    SOFTRESET_SKIP_LOGO = 1
};

class CMainApp {
public:
    static void initialize(BOOL bNTSC);
    static void finalize();
    static INT run();
    static BOOL isEnableSoftReset();
    static BOOL reqSoftReset(SOFTRESET_MODE mode);
    static void enableSoftReset(BOOL bEnable);
    static BOOL isSkipLogo();
    static void resetPlayTime(UINT nTime);
    static UINT getPlayTime();
    static void startCountPlayTime();
    static void pauseCountPlayTime();
    static void setFpsFull();
    static void setFpsHalf();
    static BOOL isFpsHalf();
    static BOOL isNTSC();
    static BOOL isPAL();
    static void setBackToFullFpsLag(INT nLag);
    static INT getBackToFullFpsLag();
    static BOOL isSysPauseNow();
    static void setSysPause(BOOL bFlag);
    static BOOL isWarningPauseNow();
    static void enableWarningPause(BOOL bEnable);
    static BOOL isWarningPauseDisable();
    static BOOL isDbgUsrPauseNow();
    static void enableDbgUsrPause(BOOL bFlag, BOOL bPause);
    static BOOL isDbgUsrPauseDisable(); static BOOL isPauseNow() { return isWarningPauseNow() || isSysPauseNow() || isDbgUsrPauseNow(); } // PARTIAL: added for Game/Atel/VSME
    static BOOL reqBlackIn(USHORT nTime);
    static BOOL reqBlackOut(USHORT nTime);
    static BOOL reqWhiteIn(USHORT nTime);
    static BOOL reqWhiteOut(USHORT nTime);
    static void changeDrawModeNormal();
    static TIMEQ getTickCount();
    static TIMEQ getMainCount();
    static TIMEQ getDispFrameRate();
    static UINT getFrameCount();
    static UINT getVSyncCount();
    static INT getObjCount();
    static void incObjCount();
    static void decObjCount();
    static INT getProcCount();
    static INT getDrawCount();
    static INT getDraw2Count();
    static INT getProcCountMax();
    static void resetProcCountMax();
    static void halt(const CHAR* pszFile, INT nLine, const CHAR* pszMsg, const CHAR* pszExp);
};
#endif

#endif /* SYSTEM_MAINAPP_H */
