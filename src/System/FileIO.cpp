#include "System/FileIO.h"
#include "System/MainApp.h"
#include "System/Debug.h"
#include "System/mcd.h"
#include "System/Movie/MSStream.h"
#include <string.h>

struct ThreadParam {
    int status;
    void (*entry)(void*);
    void* stack;
    int stackSize;
    void* gpReg;
    int initPriority;
    int currentPriority;
    u_int attr;
    u_int option;
    int waitType;
    int waitId;
    int wakeupCount;
};

extern "C" {
int CreateThread(ThreadParam* param);
int StartThread(int tid, void* arg);
int DelayThread(int usec);
int sceOpen(const char* filename, int flag, ...);
int sceClose(int fd);
int sceRead(int fd, void* buf, int nbyte);
int sceWrite(int fd, const void* buf, int nbyte);
int sceLseek(int fd, int offset, int where);
int sprintf(char* buf, const char* fmt, ...);
extern const char _ctype_[];
}

extern char _gp;

#include "System/Buf.h"















#include "System/Rcf.h"




struct _BgLoadInfo {
    union {
        CHAR* _pszFile;
        UINT _nStartSect;
    };
    BYTE* _pAddr;
    INT _nLen;
    CBuf* _pBuf;
    INT _nAlign;
};

namespace {
INT s_nOpenedFd = 0;
BOOL s_bAccessLock = 0;
volatile BOOL s_bBgLoading = 0;
volatile BOOL s_bTrayOpened = 0;
unsigned char s_cBgLoadThreadStack[4096];
INT s_nBgLoadResult;
_BgLoadInfo s_BgLoadInfo;
}

static void threadBgLoad(void* p);

void CFileIO::initialize() {
    s_bBgLoading = 0;
    s_nOpenedFd = 0;
    s_bAccessLock = 0;
    s_bTrayOpened = 0;
}

void CFileIO::finalize() {
    mcdSync(0);
    s_bTrayOpened = 0;
    s_bBgLoading = 0;
}

void CFileIO::lock() {
    s_bAccessLock = 1;
}

void CFileIO::unlock() {
    s_bAccessLock = 0;
}

static void _exPathSlash2Yen(CHAR* lpszInOut) {
    CHAR* p;
    if (lpszInOut) {
        for (p = lpszInOut; *p; p++) {
            if (*p == '/') {
                *p = '\\';
            }
        }
    }
}

static void _exPathYen2Slash(CHAR* lpszInOut) {
    CHAR* p;
    if (lpszInOut) {
        for (p = lpszInOut; *p; p++) {
            if (*p == '\\') {
                *p = '/';
            }
        }
    }
}

static void _exPath2Upper(CHAR* lpszInOut) {
    CHAR* p;
    if (lpszInOut) {
        for (p = lpszInOut; *p; p++) {
            CHAR c = *p;
            if ((_ctype_ + 1)[c] & 2) {
                *p = c - 0x20;
            }
        }
    }
}

static INT _openFile(const CHAR* pszFile, INT nInFlags, USHORT nInMode) {
    INT fp;
    if (s_bAccessLock) {
        TRACE_COL(_TTY_RED, "\x83\x74\x83\x40\x83\x43\x83\x8B\x83\x41\x83\x4E\x83\x5A\x83\x58\x8B\xD6\x8E\x7E\x92\x86\x82\xC5\x82\xB7\x81\x42"
                            " CANCEL -> \"%s\"\n",
                  pszFile);
        fp = -6;
    } else {
        fp = sceOpen(pszFile, nInFlags, nInMode);
        if (fp >= 0) {
            return fp;
        }
        fp = -1;
    }
    return fp;
}

#line 194
INT CFileIO::openFile(const CHAR* pszInFile, INT nInFlags, USHORT nInMode) {
    char szPath[256];
    INT nRes;
    KH_DEBUG(if (!(0 == s_nOpenedFd)) AssertApp(__FILE__, __LINE__, "0 == s_nOpenedFd", 0);)
    if (!pszInFile) {
        return -1;
    }
    makeupPath(szPath, pszInFile);
    nRes = _openFile(szPath, nInFlags, nInMode);
    if (nRes > 0) {
        s_nOpenedFd = nRes;
    }
    return nRes;
}

#line 216
INT CFileIO::closeFile(INT nInFd) {
    KH_DEBUG(if (!(nInFd == s_nOpenedFd)) AssertApp(__FILE__, __LINE__, "nInFd == s_nOpenedFd", 0);)
    sceClose(nInFd);
    s_nOpenedFd = 0;
}

#line 236
INT CFileIO::readFile(INT nInFd, void* pInBuf, INT nInSize) {
    KH_DEBUG(if (!(nInFd == s_nOpenedFd)) AssertApp(__FILE__, __LINE__, "nInFd == s_nOpenedFd", 0);)
    return sceRead(nInFd, pInBuf, nInSize);
}

#line 253
INT CFileIO::writeFile(INT nInFd, const void* pInData, INT nInSize) {
    KH_DEBUG(if (!(nInFd == s_nOpenedFd)) AssertApp(__FILE__, __LINE__, "nInFd == s_nOpenedFd", 0);)
    return sceWrite(nInFd, pInData, nInSize);
}

static INT _getFileSize(INT nInFp) {
    INT nResult = sceLseek(nInFp, 0, 2);
    sceLseek(nInFp, 0, 0);
    return nResult;
}

INT CFileIO::getFileSize(const CHAR* pszInFile) {
    INT nRet;
    char szPath[256];
    INT fp;
    if (!pszInFile) {
        return -2;
    }
    makeupPath(szPath, pszInFile);
    fp = _openFile(szPath, 1, 0);
    if (fp < 0) {
        nRet = fp;
    } else {
        nRet = _getFileSize(fp);
        sceClose(fp);
    }
    return nRet;
}

INT CFileIO::loadFile(const CHAR* pszInFile, BYTE* pInBuf, INT nInLen) {
    INT nResult;
    char szPath[256];
    INT fp;
    if (!pszInFile) {
        return -2;
    }
    makeupPath(szPath, pszInFile);
    fp = _openFile(szPath, 1, 0);
    if (fp < 0) {
        nResult = fp;
    } else {
        nResult = sceRead(fp, pInBuf, nInLen);
        if (nResult < 0) {
            nResult = -2;
        }
        sceClose(fp);
    }
    return nResult;
}

INT CFileIO::loadFile(const CHAR* pszInFile, BYTE* pInBuf, INT nInSkip, INT nInLen) {
    INT nResult;
    char szPath[256];
    INT fp;
    if (!pszInFile) {
        return -2;
    }
    makeupPath(szPath, pszInFile);
    fp = _openFile(szPath, 1, 0);
    if (fp < 0) {
        nResult = fp;
    } else {
        sceLseek(fp, nInSkip, 0);
        nResult = sceRead(fp, pInBuf, nInLen);
        if (nResult < 0) {
            nResult = -2;
        }
        sceClose(fp);
    }
    return nResult;
}

INT CFileIO::loadFile(const CHAR* pszInFile, CBuf& outBuf, INT nInAllocAlign) {
    INT nResult;
    char szPath[256];
    INT fp;
    if (!pszInFile) {
        return -2;
    }
    makeupPath(szPath, pszInFile);
    fp = _openFile(szPath, 1, 0);
    if (fp < 0) {
        nResult = fp;
    } else {
        INT nLen = _getFileSize(fp);
        if (nLen < 0) {
            nResult = -2;
        } else {
            BYTE* pBuf = outBuf.getBufPtr();
            if (!pBuf) {
                switch (nInAllocAlign) {
                case 1:
                    pBuf = outBuf.allocBuf_A128(nLen);
                    break;
                case 2:
                    pBuf = outBuf.allocBuf_A256(nLen);
                    break;
                default:
                    pBuf = outBuf.allocBuf_A64(nLen);
                    break;
                }
            }
            if (pBuf) {
                nResult = sceRead(fp, pBuf, nLen);
                if (nResult < 0) {
                    nResult = -2;
                }
            } else {
                TRACE_COL(_TTY_RED, "CFileIO::loadFile() -> File:%s 0x%08xbyte "
                                    "\x83\x81\x83\x82\x83\x8A\x8A\x6D\x95\xDB\x8E\xB8\x94\x73\x81\x49\x81\x49\x81\x49\n",
                          pszInFile, nLen);
                nResult = -5;
            }
        }
        sceClose(fp);
    }
    return nResult;
}

static void _startLoadNoWait(_BgLoadInfo& info) {
    ThreadParam tp;
    s_bBgLoading = 1;
    tp.entry = threadBgLoad;
    tp.stack = s_cBgLoadThreadStack;
    tp.stackSize = sizeof(s_cBgLoadThreadStack);
    tp.initPriority = 8;
    tp.gpReg = &_gp;
    StartThread(CreateThread(&tp), &info);
}

BOOL CFileIO::loadFileNoWait(UINT nStartSect, UINT nSectCnt, BYTE* pAddr) {
    if (s_bBgLoading) {
        return 0;
    }
    if (s_bTrayOpened) {
        return 0;
    }
    s_BgLoadInfo._nStartSect = nStartSect;
    s_BgLoadInfo._pAddr = pAddr;
    s_BgLoadInfo._nLen = nSectCnt;
    s_BgLoadInfo._pBuf = 0;
    _startLoadNoWait(s_BgLoadInfo);
    return 1;
}

INT CFileIO::syncLoadFile() {
    if (s_bBgLoading) {
        return 0;
    }
    return s_nBgLoadResult;
}

INT CFileIO::saveFile(const CHAR* pszInFile, const BYTE* pInData, INT nInLen) {
    INT nResult;
    char szPath[256];
    INT fp;
    if (!pszInFile) {
        return -3;
    }
    makeupPath(szPath, pszInFile);
    fp = _openFile(szPath, 0x602, 0x1FF);
    if (fp < 0) {
        nResult = fp;
    } else {
        nResult = sceWrite(fp, pInData, nInLen);
        if (nResult < 0) {
            nResult = -3;
        }
        sceClose(fp);
    }
    return nResult;
}

INT CFileIO::saveFile(const CHAR* pszInFile, const CBuf& inBuf) {
    return saveFile(pszInFile, inBuf.getBufPtr(), inBuf.getBufSize());
}

void CFileIO::makeupPath(CHAR* lpszOutName, const CHAR* lpszInName) {
    char szTmp[256];
    if (lpszInName && lpszOutName) {
        strncpy(szTmp, lpszInName, 253);
        szTmp[253] = 0;
        _exPathSlash2Yen(szTmp);
        if (strncmp(szTmp, "cdrom0:\\", 8)) {
            sprintf(szTmp, "cdrom0:\\%s", lpszInName);
            _exPathSlash2Yen(szTmp);
        }
        _exPath2Upper(szTmp + 8);
        sprintf(lpszOutName, "%s;1", szTmp);
    }
}

void CFileIO::setTrayOpened() {
    s_bTrayOpened = 1;
}

BOOL CFileIO::isTrayOpened() {
    return s_bTrayOpened;
}

BOOL CFileIO::checkTrayOpened() {
    if (CRcf::isCompleteLoadRCF(0, 0) && !CMSStream::IsRunning() && !s_bBgLoading && !mcdCheckStat() &&
        mcdCheckCDTray() == -2) {
        s_bTrayOpened = 1;
    }
    return s_bTrayOpened;
}

BOOL CFileIO::checkTrayClosed() {
    sceCdlFILE f;
    if (!mcdCheckCDTray() && !mcdSearchFile(&f, "\\SLUS_217.99;1")) {
        s_bTrayOpened = 0;
    }
    return !s_bTrayOpened;
}

static void threadBgLoad(void* p) {
    _BgLoadInfo* pInfo = (_BgLoadInfo*)p;
    UINT nLoadLen;
    UINT nSect;
    BYTE* pBuf;
    INT nResult;
    UINT nLen;

    TRACE("Background Load Start...\n");
    s_nBgLoadResult = 0;
    if (pInfo) {
        nLen = pInfo->_nLen;
        nResult = 0;
        nSect = pInfo->_nStartSect;
        pBuf = pInfo->_pAddr;
        while (nLen && !s_bTrayOpened && s_bBgLoading) {
            nLoadLen = nLen > 32 ? 32 : nLen;
            while (mcdAsyncRead2(nSect, nLoadLen, pBuf) && s_bBgLoading) {
                DelayThread(4000);
            }
            if (!s_bBgLoading) {
                break;
            }
            nResult = mcdSync(0);
            switch (nResult) {
            case 0:
                nSect += nLoadLen;
                pBuf += nLoadLen * 2048;
                nLen -= nLoadLen;
                if (nLen != 0) {
                    goto wait;
                }
                nResult = pInfo->_nLen;
                break;
            case -3:
            case -2:
                nResult = -1;
                s_bTrayOpened = 1;
                break;
            default:
            wait:
                DelayThread(10000);
                break;
            }
        }
        s_nBgLoadResult = nResult;
    }
    s_bBgLoading = 0;
    TRACE("Background Load End... [%d]\n", s_nBgLoadResult);
}
// SOURCE DATA: .data .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss .data .rodata
