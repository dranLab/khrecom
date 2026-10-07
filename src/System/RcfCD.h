// PARTIAL: added for System/Movie/MovieFiles
#ifndef SYSTEM_RCFCD_H
#define SYSTEM_RCFCD_H

#include "khtypes.h"

struct SectorInfo {
    CHAR* _pszFileName;
    ULONG _nStartSector;
    ULONG _nEndSector;
    ULONG _nSectorLength;
};

class CRcfCD {
public:
    static BOOL getSectorInfo(UINT nIdx, SectorInfo& outInfo);
};

#endif /* SYSTEM_RCFCD_H */
