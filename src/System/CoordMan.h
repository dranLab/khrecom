#ifndef SYSTEM_COORDMAN_H
#define SYSTEM_COORDMAN_H

#include "khtypes.h"
#include "System/Coord.h"

class CCoordMan {
public:
    static void initialize();
    static void finalize();
    static void update(TIMEQ nTQ, CCoords::_PROC_PRIO pp);
    static HANDLE add(CCoords& crds, CCoords::_PROC_PRIO pp);
    static BOOL del(CCoords& crds);
    static BOOL del(HANDLE hCoords);
    static CCoords* getCoords(HANDLE hCoords);
    static void resetTimeStamp();
    static CCoord& getExceptCoord(BOOL bVerbose);
};

#endif /* SYSTEM_COORDMAN_H */
