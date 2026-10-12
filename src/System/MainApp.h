// PARTIAL: added for Game/CardKind
#ifndef SYSTEM_MAINAPP_H
#define SYSTEM_MAINAPP_H

#include "khtypes.h"

#ifdef __cplusplus
extern "C"
#endif
void AssertApp(char* pszFile, int nLine, char* pszMsg, char* pszExp);

#ifdef __cplusplus
// PARTIAL: added for System/Movie/Movie
class CMainApp {
public:
    static void setFpsFull();
    static void setFpsHalf();
    static BOOL isFpsHalf();
    static BOOL isPAL();
};
#endif

#endif /* SYSTEM_MAINAPP_H */
