// PARTIAL: added for Form/Menu/SaveLoadUtil
#ifndef SYSTEM_MEMOCA_H
#define SYSTEM_MEMOCA_H

#include "khtypes.h"

class CBuf;
struct DebugParam; // PARTIAL: added for Form/DebugMenu

typedef struct {
    unsigned char Resv2;
    unsigned char Sec;
    unsigned char Min;
    unsigned char Hour;
    unsigned char Day;
    unsigned char Month;
    unsigned short Year;
} sceMcStDateTime;

typedef struct {
    sceMcStDateTime _Create;
    sceMcStDateTime _Modify;
    unsigned int FileSizeByte;
    unsigned short AttrFile;
    unsigned short Reserve1;
    unsigned int Reserve2;
    unsigned int PdaAplNo;
    unsigned char EntryName[32];
} sceMcTblGetDir;

enum MC_DATATYPE {
    MC_DATATYPE_SYS = 0,
    MC_DATATYPE_GAME = 1,
    MC_DATATYPE_GAME_HEAD = 2,
    MC_DATATYPE_DEBUG = 3
};

class CMemoca {
public:
    static BOOL reqLoad(INT nPort, const CHAR* pszFile, CBuf& buf, MC_DATATYPE type);
    static INT loadProc();
    static BOOL reqSave(INT nPort, const CHAR* pszFile, const CBuf& buf, MC_DATATYPE type, const CHAR* pszTitle,
                        USHORT nIconID);
    static INT saveProc();
    static BOOL reqGetFreeSize(INT nPort, sceMcTblGetDir* pDirBuf, INT nMax);
    static INT getFreeSize();
    static INT getRootDirCount();
    static INT getFreeEntSpace();
    static BOOL reqGetDirSize(INT nPort, const CHAR* pszDir);
    static INT getDirSize();
    static INT checkDataFileAttr(INT nPort, const CHAR* pszDir, INT nSlot);
    static INT getDirSizeAfterCheckDir();
    static sceMcStDateTime getLastCheckFileTime();
    static INT idleCheckCard(INT nPort);
    static CHAR* getSysFileNameCOM();
    static CHAR* getGameDataFileName();
    // PARTIAL: added for Form/DebugMenu
    static BOOL reqLoad(DebugParam& param);
    static BOOL reqSave(const DebugParam& param);
    static BOOL reqSaveSysTest(BOOL bSora, BOOL bRiku, BOOL bFlag);
    static BOOL reqLoadSysTest(CBuf& buf);
};

#endif /* SYSTEM_MEMOCA_H */
