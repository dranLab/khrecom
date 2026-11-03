
#ifndef SYSTEM_MEMOCA_H
#define SYSTEM_MEMOCA_H

#include "khtypes.h"

class CBuf;
struct DebugParam;

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
    static void initialize();
    static void setupMcName(BOOL bNTSC);
    static void finalize();
    static BOOL reqLoad(INT nPort, const CHAR* pszName, BYTE* pOutBuf, INT nSize, MC_DATATYPE type);
    static BOOL reqLoad(INT nPort, const CHAR* pszName, CBuf& dst, MC_DATATYPE type);
    static BOOL reqSave(INT nPort, const CHAR* pszName, const BYTE* pBuf, INT nSize, MC_DATATYPE type,
                        const CHAR* pszTitle, USHORT nOffsLF);
    static BOOL reqSave(INT nPort, const CHAR* pszName, const CBuf& src, MC_DATATYPE type, const CHAR* pszTitle,
                        USHORT nOffsLF);
    static BOOL reqGetFreeSize(INT nPort, sceMcTblGetDir* pOutBuf, INT nMaxEnt);
    static INT getFreeSize();
    static INT getRootDirCount();
    static INT getFreeEntSpace();
    static BOOL reqFormat(INT nPort);
    static INT formatProc();
    static INT saveProc();
    static INT loadProc();
    static INT checkDataFileAttr(INT nMode, const CHAR* pszName, INT nPort);
    static BOOL reqGetDirSize(INT nPort, const CHAR* pszDir);
    static INT getDirSize();
    static INT getDirSizeAfterCheckDir();
    static sceMcStDateTime getLastCheckFileTime();
    static INT idleCheckCard(INT nPort);
    static CHAR* getSysFileNameCOM();
    static CHAR* getGameDataFileName();
    static BOOL reqSave(const DebugParam& param);
    static BOOL reqLoad(DebugParam& param);
    static BOOL reqSaveSysTest(BOOL bSora, BOOL bRiku, BOOL bFlag);
    static BOOL reqLoadSysTest(CBuf& buf);
    static BOOL reqUnformat(INT nPort);
    static INT unformatProc();
};

#endif /* SYSTEM_MEMOCA_H */
