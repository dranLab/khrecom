#include "System/PadHandler.h"
#include "System/PadIO.h"

INT CPadHandler::isButtonTrg(UINT b, UINT nPad) {
    return CPadIO::isButtonTrg(b, nPad);
}

INT CPadHandler::isButtonDown(UINT b, UINT nPad) {
    return CPadIO::isButtonDown(b, nPad);
}

INT CPadHandler::isButtonRTrg(UINT b, UINT nPad) {
    return CPadIO::isButtonRTrg(b, nPad);
}

INT CPadHandler::isButtonRpt(UINT b, UINT nPad) {
    return CPadIO::isButtonRpt(b, nPad);
}

UINT CPadHandler::getButtonDown(UINT nPad) {
    return CPadIO::getButtonDown(nPad);
}

UINT CPadHandler::getButtonDownOld(UINT nPad) {
    return CPadIO::getButtonDown(nPad);
}

INT CPadHandler::getAnalogLX(UINT nPad) {
    return CPadIO::getAnalogLX(nPad);
}

INT CPadHandler::getAnalogLY(UINT nPad) {
    return CPadIO::getAnalogLY(nPad);
}

INT CPadHandler::getAnalogRX(UINT nPad) {
    return CPadIO::getAnalogRX(nPad);
}

INT CPadHandler::getAnalogRY(UINT nPad) {
    return CPadIO::getAnalogRY(nPad);
}

BOOL CPadHandler::isTrgOK(UINT nPad) {
    UINT nKey = getControlType() ? 0x20 : 0x40;
    return isButtonTrg(nKey, nPad) != 0;
}

BOOL CPadHandler::isTrgTalk(UINT nPad) {
    getControlType();
    return isButtonTrg(0x10, nPad) != 0;
}

BOOL CPadHandler::isTrgTalkMesNext(UINT nPad) {
    getControlType();
    return isButtonTrg(0x70, nPad) != 0;
}

BOOL CPadHandler::isTrgSelect(UINT nPad) {
    UINT nKey = getControlType() ? 0x20 : 0x40;
    return isButtonTrg(nKey, nPad) != 0;
}

BOOL CPadHandler::isTrgCANCEL(UINT nPad) {
    UINT nKey = getControlType() ? 0x40 : 0x20;
    return isButtonTrg(nKey, nPad) != 0;
}

BOOL CPadHandler::isTrgACTION(UINT nPad) {
    getControlType();
    return isButtonTrg(0x80, nPad) != 0;
}

BOOL CPadHandler::isTrgNEXT(UINT nPad) {
    BOOL ret = FALSE;
    if (isTrgOK(nPad) || isTrgCANCEL(nPad)) {
        ret = TRUE;
    }
    return ret;
}

BOOL CPadHandler::getControlType() {
    return FALSE;
}
