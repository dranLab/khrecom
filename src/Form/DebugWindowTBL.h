#ifndef FORM_DEBUGWINDOWTBL_H
#define FORM_DEBUGWINDOWTBL_H

#include "Form/DebugWindow.h"

namespace DWT {
extern char* szOnOff[2];
extern char* szOffOn[2];
extern char* szFalseTrue[2];
extern char* szCamSpeed[4];
extern char* szPlayDemo[4];
extern char* szAfterEffect[3];
extern char* szFps[2];
extern char* szDefOnOff[3];
extern char* szCollDispRef[2];
extern DEBUG_WINDOW_TBL debugWinYesNo;
extern DEBUG_WINDOW_TBL debugWinName;
extern DEBUG_WINDOW_TBL debugWinSeTest;
extern DEBUG_WINDOW_TBL debugWinFocusTest;
extern DEBUG_WINDOW_TBL debugWinShadowTest;
extern DEBUG_WINDOW_TBL debugWinEnemy;
extern DEBUG_WINDOW_LINE debugWinMini100AcreLine[6];
extern DEBUG_WINDOW_LINE debugWinMini100AcreWDLine[5];
extern DEBUG_WINDOW_LINE debugWinMini100AcreBLLine[9];
extern char* Mini100AcreTJ_Mugen[4];
extern DEBUG_WINDOW_LINE debugWinMini100AcreTJLine[22];
extern DEBUG_WINDOW_LINE debugWinMini100AcreVPLine[6];
extern DEBUG_WINDOW_LINE debugWinMini100AcreBBLine[11];
extern DEBUG_WINDOW_TBL debugWinMini100Acre;
extern DEBUG_WINDOW_TBL debugWinMini100AcreWD;
extern DEBUG_WINDOW_TBL debugWinMini100AcreBL;
extern DEBUG_WINDOW_TBL debugWinMini100AcreTJ;
extern DEBUG_WINDOW_TBL debugWinMini100AcreVP;
extern DEBUG_WINDOW_TBL debugWinMini100AcreBB;
extern DEBUG_WINDOW_TBL debugWinDispTest;
extern DEBUG_WINDOW_LINE debugWinMenuAddCardLineSora[9];
extern DEBUG_WINDOW_LINE debugWinMenuAddCardLineRiku[7];
extern DEBUG_WINDOW_TBL debugWinMenuConfig;
extern DEBUG_WINDOW_TBL debugWinMenuAddCardSora;
extern DEBUG_WINDOW_TBL debugWinMenuAddCardRiku;
extern char* szRobotPadMode[3];
extern DEBUG_WINDOW_TBL debugWinRobotPadTrg;
extern DEBUG_WINDOW_TBL debugWinRobotPadPress;
extern DEBUG_WINDOW_TBL debugWinRobotPad;
extern DEBUG_WINDOW_TBL debugWinTop;
namespace DWTGimThrow {
extern char* szPrizeKind[2];
extern char* szPrizeDrop[5];
}
}

#endif /* FORM_DEBUGWINDOWTBL_H */
