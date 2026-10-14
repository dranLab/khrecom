#ifndef SYSTEM_FILEIO_H
#define SYSTEM_FILEIO_H

#include "khtypes.h"

class CBuf;

class CFileIO {
public:
    static void initialize();
    static void finalize();
    static INT loadFile(const CHAR* pszInFile, BYTE* pInBuf, INT nInLen);
    static INT loadFile(const CHAR* pszInFile, BYTE* pInBuf, INT nInSkip, INT nInLen);
    static INT loadFile(const CHAR* pszInFile, CBuf& outBuf, INT nInAllocAlign);
    static BOOL loadFileNoWait(UINT nStartSect, UINT nSectCnt, BYTE* pAddr);
    static INT syncLoadFile();
    static INT saveFile(const CHAR* pszInFile, const BYTE* pInData, INT nInLen);
    static INT saveFile(const CHAR* pszInFile, const CBuf& inBuf);
    static void makeupPath(CHAR* lpszOutName, const CHAR* lpszInName);
    static INT openFile(const CHAR* pszInFile, INT nInFlags, USHORT nInMode);
    static INT closeFile(INT nInFd);
    static INT readFile(INT nInFd, void* pInBuf, INT nInSize);
    static INT writeFile(INT nInFd, const void* pInData, INT nInSize);
    static INT getFileSize(const CHAR* pszInFile);
    static void lock();
    static void unlock();
    static BOOL isTrayOpened();
    static void setTrayOpened();
    static BOOL checkTrayOpened();
    static BOOL checkTrayClosed();
};

#endif // SYSTEM_FILEIO_H
