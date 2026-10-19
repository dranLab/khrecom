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
    static void initialize();
    static void finalize();
    static INT setupRcfAlias();
    static INT getMemberOption(UINT nGroup);
    static UINT getExtInfo(UINT nGroup, INT nIdx);
    static INT getRsrcFileSize(UINT nID);
    static INT getRsrcFileSizeOnMemory(UINT nID);
    static void initReadThreadProc();
    static INT procReadThread(UINT nGroup, INT nType, BYTE*& pExtBuff);
    static INT cancelReadThread();
    static BOOL getSectorInfo(UINT nIdx, SectorInfo& outInfo);
    static BOOL getSectorInfo(const CHAR* pszName, SectorInfo& outInfo, UINT nNameLen);
    static UINT getSectorInfoCount();
};

#endif /* SYSTEM_RCFCD_H */
