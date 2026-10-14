// PARTIAL: added for System/PadHandler
#ifndef SYSTEM_PADIO_H
#define SYSTEM_PADIO_H

#include "khtypes.h"

class CPadIO {
public:
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
    static BOOL isConnected(UINT nPad); // PARTIAL: added for System/RootTask
    static void suspendVibration(); // PARTIAL: added for Form/Menu/Pause
    static void resumeVibration();  // PARTIAL: added for Form/Menu/Pause
    static BYTE getVibMotorL(UINT nPad); // PARTIAL: added for System/DebugProcInfo
    static BYTE getVibMotorS(UINT nPad); // PARTIAL: added for System/DebugProcInfo
    static BOOL isStandby(UINT nPad);    // PARTIAL: added for System/DebugProcInfo
};

#endif // SYSTEM_PADIO_H
