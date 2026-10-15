#ifndef SYSTEM_PADIO_H
#define SYSTEM_PADIO_H

#include "khtypes.h"

class CPadIO {
public:
    static void initialize();
    static void finalize();
    static void mainProc(TIMEQ nTickCount, TIMEQ nRate);
    static INT isButtonDown(UINT nBtn, UINT nPad = 0);
    static INT isButtonTrg(UINT nBtn, UINT nPad = 0);
    static INT isButtonRTrg(UINT nBtn, UINT nPad = 0);
    static INT isButtonRpt(UINT nBtn, UINT nPad = 0);
    static UINT getButtonDown(UINT nPad = 0);
    static UINT getButtonDownOld(UINT nPad = 0);
    static INT getAnalogRX(UINT nPad = 0);
    static INT getAnalogRY(UINT nPad = 0);
    static INT getAnalogLX(UINT nPad = 0);
    static INT getAnalogLY(UINT nPad = 0);
    static BYTE getVibMotorL(UINT nPad = 0);
    static BYTE getVibMotorS(UINT nPad = 0);
    static INT startVibration(INT motorL, INT motorS, TIMEQ tq, UINT nPad = 0);
    static void stopVibration(INT handle, UINT nPad = 0);
    static void resetVibration();
    static void enableVibration(BOOL f, UINT nPad = 0);
    static void suspendVibration();
    static void resumeVibration();
    static BOOL isVibrationSuspended();
    static BOOL isConnected(UINT nPad = 0);
    static BOOL isStandby(UINT nPad = 0);
    static BOOL isRobotEnable(UINT nPad = 0);
    static void setRobotEnableFlag(BOOL bEnableFlag, UINT nPad = 0);
    static void setRobotPadRate(INT nRobotPad, INT nTrgRate, INT nPressRate, UINT nPad = 0);
};

#endif // SYSTEM_PADIO_H
