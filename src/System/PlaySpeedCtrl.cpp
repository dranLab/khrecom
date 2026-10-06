#include "PlaySpeedCtrl.h"

namespace {
FLOAT s_fPlaySpeed = 1.f;
FLOAT s_fDebugPlaySpeed = 1.f;
}

void CPlaySpeedCtrl::setPlaySpeed(FLOAT fInSpeed) {
    if (fInSpeed > 0.f) {
        s_fPlaySpeed = fInSpeed;
    } else {
        s_fPlaySpeed = 0.f;
    }
}

TIMEQ CPlaySpeedCtrl::getProgressTime(TIMEQ nInTQ) {
    return nInTQ * s_fPlaySpeed;
}

void CPlaySpeedCtrl::setDebugPlaySpeed(FLOAT fInSpeed) {
    if (fInSpeed > 0.f) {
        s_fDebugPlaySpeed = fInSpeed;
    } else {
        s_fDebugPlaySpeed = 0.f;
    }
}

TIMEQ CPlaySpeedCtrl::getSysProgressTime(TIMEQ nInTQ) {
    return nInTQ * s_fDebugPlaySpeed;
}
