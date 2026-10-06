#ifndef SYSTEM_PLAYSPEEDCTRL_H
#define SYSTEM_PLAYSPEEDCTRL_H

#include "khtypes.h"

/* Speed multipliers for game time and debug ("system") time. */
class CPlaySpeedCtrl {
public:
    static void setPlaySpeed(FLOAT fInSpeed);
    static void reset();        // not in PlaySpeedCtrl.o: presumably inline
    static void stop();         // not in PlaySpeedCtrl.o: presumably inline
    static TIMEQ getProgressTime(TIMEQ nInTQ);
    static void setDebugPlaySpeed(FLOAT fInSpeed);
    static void resetDebug();   // not in PlaySpeedCtrl.o: presumably inline
    static void stopDebug();    // not in PlaySpeedCtrl.o: presumably inline
    static TIMEQ getSysProgressTime(TIMEQ nInTQ);
};

#endif /* SYSTEM_PLAYSPEEDCTRL_H */
