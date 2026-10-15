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
class CMainApp {
public:
    static void initialize(int mode); // PARTIAL: added for main
    static INT run();
    static void finalize();
    static void setFpsFull();
    static void setFpsHalf();
    static BOOL isFpsHalf();
    static BOOL isPAL();
    static void enableSoftReset(BOOL bEnable); // PARTIAL: added for System/RootTask
    static BOOL isSkipLogo();
    static void enableWarningPause(BOOL bEnable);
    // PARTIAL: added for System/DebugProcInfo
    static TIMEQ getMainCount();
    static INT getObjCount();
    static INT getProcCount();
    static INT getDrawCount();
    static INT getDraw2Count();
    static INT getProcCountMax();
    // PARTIAL: added for System/Task
    static TIMEQ getTickCount();
    static TIMEQ getDispFrameRate();
    static void incObjCount();
    static void decObjCount();
    // PARTIAL: added for System/Movie/MSVideo
    static INT getFrameCount();
    static BOOL isWarningPauseNow();
};
#endif

#endif /* SYSTEM_MAINAPP_H */
