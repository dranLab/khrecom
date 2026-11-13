#include "PlaySpeedCtrl.h"

namespace {
FLOAT s_fPlaySpeed = 1.f;
KH_DEBUG(FLOAT s_fDebugPlaySpeed = 1.f;)
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

KH_DEBUG(void CPlaySpeedCtrl::setDebugPlaySpeed(FLOAT fInSpeed) {)
    KH_DEBUG(if (fInSpeed > 0.f) {)
        KH_DEBUG(s_fDebugPlaySpeed = fInSpeed;)
    KH_DEBUG(} else {)
        KH_DEBUG(s_fDebugPlaySpeed = 0.f;)
    KH_DEBUG(})
KH_DEBUG(})

KH_DEBUG(TIMEQ CPlaySpeedCtrl::getSysProgressTime(TIMEQ nInTQ) {)
    KH_DEBUG(return nInTQ * s_fDebugPlaySpeed;)
KH_DEBUG(})

// SOURCE DATA: .data
// SOURCE DATA SLUS_217.99_retail: .data
