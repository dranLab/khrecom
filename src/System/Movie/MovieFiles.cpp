#include "System/Movie/MovieFiles.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"
#include "System/Debug.h"
#include <string.h>

static UINT s_nMovieFileCount;
static USHORT* s_pMovieIdxTbl;

#line 44
void CMovieFiles::initialize() {
    s_pMovieIdxTbl = (USHORT*)com_malloc_d(0x100, __FILE__ "(45)", -1);
    KH_DEBUG(if (!s_pMovieIdxTbl) AssertApp(__FILE__, __LINE__, "s_pMovieIdxTbl", 0);)
    s_nMovieFileCount = 0;
}

void CMovieFiles::finalize() {
    if (s_pMovieIdxTbl) {
        com_free(s_pMovieIdxTbl);
    }
    s_pMovieIdxTbl = NULL;
}

BOOL CMovieFiles::hasFile(const CHAR* pszFileName) {
    SectorInfo info;
    return getFileInfo(pszFileName, info);
}

UINT CMovieFiles::getFileCount() {
    return s_nMovieFileCount;
}

void CMovieFiles::addFileInfo(UINT nInfoIdx) {
    if (s_nMovieFileCount >= 0x80) {
        TRACE_USER(TGOTO, "CMovieFiles::addFilePath() -> \x93\x6f\x98\x5e\x90\x94\x83\x49\x81\x5b\x83\x6f\x81\x5b\n");
        return;
    }
    s_pMovieIdxTbl[s_nMovieFileCount] = nInfoIdx;
    s_nMovieFileCount++;
}

BOOL CMovieFiles::getFileInfo(const CHAR* pszFileName, SectorInfo& outInfo) {
    const CHAR* p;
    INT nLen;
    UINT nMax;
    USHORT* pIdx;
    UINT i;

    p = strrchr(pszFileName, '.');
    if (p) nLen = p - pszFileName;
    else nLen = strlen(pszFileName);
    nMax = s_nMovieFileCount;
    pIdx = s_pMovieIdxTbl;
    for (i = 0; i < nMax; i++, pIdx++) {
        if (CRcfCD::getSectorInfo(*pIdx, outInfo)) {
            CHAR* q = strrchr(outInfo._pszFileName, '\\');
            if (q) q++;
            else q = outInfo._pszFileName;
            if (strncasecmp(q, pszFileName, nLen) == 0) return TRUE;
        }
    }
    return FALSE;
}

CHAR* CMovieFiles::getFilePath(UINT nIdx) {
    SectorInfo info;
    if (nIdx >= s_nMovieFileCount) return NULL;
    BOOL bRet = CRcfCD::getSectorInfo(s_pMovieIdxTbl[nIdx], info);
    return bRet ? info._pszFileName : NULL;
}

// SOURCE DATA: .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss
