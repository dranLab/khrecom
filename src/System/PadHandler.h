#ifndef SYSTEM_PADHANDLER_H
#define SYSTEM_PADHANDLER_H

#include "khtypes.h"

class CPadHandler {
public:
    static INT isButtonTrg(UINT b, UINT nPad = 0);
    static INT isButtonDown(UINT b, UINT nPad = 0);
    static INT isButtonRTrg(UINT b, UINT nPad = 0);
    static INT isButtonRpt(UINT b, UINT nPad = 0);
    static UINT getButtonDown(UINT nPad = 0);
    static UINT getButtonDownOld(UINT nPad = 0);
    static INT getAnalogRX(UINT nPad = 0);
    static INT getAnalogRY(UINT nPad = 0);
    static INT getAnalogLX(UINT nPad = 0);
    static INT getAnalogLY(UINT nPad = 0);
    static FLOAT getAnalogFRX(/* parameters unknown */);
    static FLOAT getAnalogFRY(/* parameters unknown */);
    static FLOAT getAnalogFLX(/* parameters unknown */);
    static FLOAT getAnalogFLY(/* parameters unknown */);
    static BOOL isTrgOK(UINT nPad = 0);
    static BOOL isTrgTalk(UINT nPad = 0);
    static BOOL isTrgTalkMesNext(UINT nPad = 0);
    static BOOL isTrgSelect(UINT nPad = 0);
    static BOOL isTrgCANCEL(UINT nPad = 0);
    static BOOL isTrgACTION(UINT nPad = 0);
    static BOOL isTrgNEXT(UINT nPad = 0);
    static BOOL getControlType();
};

#endif // SYSTEM_PADHANDLER_H
