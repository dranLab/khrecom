#define RSRCDATA_H_PATH "c:/KHCOM/MainApp/System/RsrcData.h" /* MATCH: this unit names RsrcData.h so (header default "System/RsrcData.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Memoca.h"
#include "System/Buf.h"
#include "System/Debug.h"
#include "System/MainApp.h"
#include "System/Util.h"
#include "Game/GameWork.h"
#include "Game/SceneInfo.h"
#include <string.h>
#include "include_asm.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

extern "C" {
int sceMcInit();
int sceMcEnd();
int sceMcSync(int mode, int* cmd, int* result);
int sceMcGetInfo(int port, int slot, int* type, int* free, int* format);
int sceMcGetEntSpace(int port, int slot, const char* path);
int sceMcGetDir(int port, int slot, const char* name, unsigned int mode, int maxent, sceMcTblGetDir* table);
int sceMcFormat(int port, int slot);
int sceMcUnformat(int port, int slot);
int sceMcMkdir(int port, int slot, const char* name);
int sceMcOpen(int port, int slot, const char* name, int mode);
int sceMcClose(int fd);
int sceMcRead(int fd, void* buff, int size);
int sceMcWrite(int fd, const void* buff, int size);
int sceMcFlush(int fd);
int sceMcDelete(int port, int slot, const char* name);
int sprintf(char* buf, const char* fmt, ...);
int atoi(const char* s);
}
















#include "System/RsrcDB.h"




#include "Form/Menu/SaveLoadUtil.h"




struct DebugParam {
    INT _nUserIndex;
};

enum _CHECKTYPE {
    _CHECKTYPE_NONE = 0,
    _CHECKTYPE_CHECKSUM = 1,
    _CHECKTYPE_CRC32 = 2
};

struct _MC_FILE_HEADER {
    UINT _nVersion;
    UINT _nCheckCode;
    UINT _nDataLen;
    UINT _Filler;
};

typedef int _iconVu0IVECTOR[4];
typedef float _iconVu0FVECTOR[4];
typedef struct {
    unsigned char Head[4];
    unsigned short Reserv1;
    unsigned short OffsLF;
    unsigned int Reserv2;
    unsigned int TransRate;
    _iconVu0IVECTOR BgColor[4];
    _iconVu0FVECTOR LightDir[3];
    _iconVu0FVECTOR LightColor[3];
    _iconVu0FVECTOR Ambient;
    unsigned char TitleName[68];
    unsigned char FnameView[64];
    unsigned char FnameCopy[64];
    unsigned char FnameDel[64];
    unsigned char Reserve3[512];
} sceMcIconSys;

namespace {
SHORT s_nLatestSlot = 0;
}

/* INCLUDE_ASM: declared extern (static in the original) */
void _setupIconSys(const CHAR* pszName);
INT _McCheckDataFileExist(INT nMode, const CHAR* pszName);
INT _McWriteIcons(const CHAR* pszName);

static const int _ivBgColor[4][4] = {
    { 13, 37, 98, 0 },
    { 13, 37, 98, 0 },
    { 93, 151, 215, 0 },
    { 93, 151, 215, 0 },
};

static const float _vLightDir[3][4] = {
    { 0.5f, 0.5f, 0.5f, 0.f },
    { 0.f, -0.4f, -0.1f, 0.f },
    { -0.5f, -0.5f, 0.5f, 0.f },
};

static const float _vLightCol[3][4] = {
    { 0.48f, 0.48f, 0.03f, 0.f },
    { 0.5f, 0.33f, 0.2f, 0.f },
    { 0.14f, 0.14f, 0.38f, 0.f },
};

static const float _vAmbient[4] = { 0.5f, 0.5f, 0.5f, 0.f };

namespace {
INT s_nRootDirCnt;
sceMcIconSys s_Icon __attribute__((aligned(64)));
sceMcTblGetDir s_DirInfo[5] __attribute__((aligned(64)));
sceMcTblGetDir* s_pRootDirTbl;
sceMcStDateTime s_LastCheckFileTime;
CBuf* s_pTmpBuf;
CBuf* s_pSaveData;
CBuf* s_pLoadData;
CRsrcData* s_pIconRsrc;
INT s_nDirentCount;
INT s_nMcType;
INT s_nMcFree;
INT s_nMcFormat;
INT s_nMcEntSpace;
INT s_nMcHandle;
INT s_nMcLatestCmnd;
char s_szMcFileName[128];
INT s_nMcPort;
BYTE* s_pMcBuff;
INT s_nMcFileSize;
INT s_nResultSize;
MC_DATATYPE s_nDataType;
BOOL s_bExistFlag;
CHAR* s_pszTitle;
USHORT s_nStatus;
USHORT s_nOffsLF;
SHORT s_nPhase;
SHORT s_nStep;
SHORT s_nIconStep;
SHORT s_nIconCount;
SHORT s_nMcCheckStep;
SHORT s_nMaxEnt;
SHORT s_nIdleStep;
char s_szSysFileName[32];
char s_szGameFileName[32];
}

void CMemoca::initialize() {
    sceMcInit();
    s_nIconCount = 0;
    s_pIconRsrc = __null;
    s_pSaveData = __null;
    s_pLoadData = __null;
    s_nPhase = 0;
    s_nStep = 0;
    s_nIconStep = 0;
    s_nMcCheckStep = 0;
    s_nIdleStep = 0;
    s_nMcHandle = -1;
    s_nMcLatestCmnd = 0;
    CMemocaDataEntries::initialize();
}

void CMemoca::setupMcName(BOOL bNTSC) {
    strcpy(s_szSysFileName, "BASLUS-21799COM-SYS");
    strcpy(s_szGameFileName, "BASLUS-21799COM-");
}

void CMemoca::finalize() {
    INT nRes;
    INT nCmd;

    if (s_nMcHandle == -9999 && s_nMcLatestCmnd == 2) {
        nCmd = s_nMcLatestCmnd;
        if (sceMcSync(0, &nCmd, &nRes) == 1) {
            TRACE_USER((USER)4, "\x83\x81\x83\x82\x83J\x83t\x83@\x83" "C\x83\x8b\x81" "E\x83I\x81[\x83v\x83\x93\x83\x8a\x83N\x83G\x83X\x83g\x82\xaa\x82\xa9\x82\xa9\x82\xc1\x82\xbd\x82\xdc\x82\xdc\x82\xc5\x82\xb7...\n");
            s_nMcHandle = nRes;
        }
    }
    if (s_nMcHandle >= 0) {
        TRACE_USER((USER)4, "\x83\x81\x83\x82\x83J\x83t\x83@\x83" "C\x83\x8b\x81" "E\x83I\x81[\x83v\x83\x93\x92\x86\x82\xc5\x82\xb7\x81" "B\x8b\xad\x90\xa7\x93I\x82\xc9\x95\xc2\x82\xb6\x82\xdc\x82\xb7...");
        sceMcSync(0, __null, &nRes);
        nRes = sceMcClose(s_nMcHandle);
        if (nRes == 0) {
            nRes = sceMcSync(0, __null, &nRes);
        }
        TRACE_USER((USER)4, (CHAR*)(nRes >= 0 ? "\x81\x83\x95\xc2\x82\xb6\x82\xdc\x82\xb5\x82\xbd\x81\x84\n" : "\x81\x83\x8e\xb8\x94s\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x81\x84\n"));
    }
    if (s_pSaveData) delete s_pSaveData;
    s_pSaveData = __null;
    if (s_pLoadData) delete s_pLoadData;
    s_pLoadData = __null;
    if (s_pTmpBuf) delete s_pTmpBuf;
    s_pTmpBuf = __null;
    s_nMcHandle = -1;
    s_nMcLatestCmnd = 0;
    sceMcEnd();
}

BOOL CMemoca::reqLoad(INT nPort, const CHAR* pszName, BYTE* pOutBuf, INT nSize, MC_DATATYPE type) {
    if (s_nPhase != 0) {
        return FALSE;
    }
    strcpy(s_szMcFileName, pszName);
    s_nMcPort = nPort;
    s_pMcBuff = pOutBuf;
    s_nMcFileSize = nSize;
    s_nDataType = type;
    s_nPhase = 1;
    return TRUE;
}

BOOL CMemoca::reqLoad(INT nPort, const CHAR* pszName, CBuf& dst, MC_DATATYPE type) {
    return reqLoad(nPort, pszName, dst.getBufPtr(), dst.getBufSize(), type);
}

BOOL CMemoca::reqSave(INT nPort, const CHAR* pszName, const BYTE* pBuf, INT nSize, MC_DATATYPE type,
                      const CHAR* pszTitle, USHORT nOffsLF) {
    INT nHeaderSize;

    if (s_nPhase != 0) {
        return FALSE;
    }
    nHeaderSize = 0;
    switch (type) {
    case MC_DATATYPE_SYS:
        break;
    case MC_DATATYPE_GAME:
    case MC_DATATYPE_DEBUG:
        nHeaderSize = 0x10;
        break;
    default:
        return FALSE;
    }
    strcpy(s_szMcFileName, pszName);
    s_nMcPort = nPort;
#line 450
    s_pSaveData = new (__FILE__ "(450)") CBuf;
    s_nMcFileSize = nSize + nHeaderSize;
    s_nDataType = type;
    s_pszTitle = (CHAR*)pszTitle;
    s_nOffsLF = nOffsLF;
    s_pSaveData->allocBuf_A64(nSize + nHeaderSize);
    memcpy(s_pSaveData->getBufPtr() + nHeaderSize, pBuf, nSize);
    s_nPhase = 5;
    return TRUE;
}

BOOL CMemoca::reqSave(INT nPort, const CHAR* pszName, const CBuf& src, MC_DATATYPE type, const CHAR* pszTitle,
                      USHORT nOffsLF) {
    return reqSave(nPort, pszName, src.getBufPtr(), src.getBufSize(), type, pszTitle, nOffsLF);
}

static INT _McSync(INT nFunc) {
    INT nFuncResult;
    INT nRet = sceMcSync(1, &nFunc, &nFuncResult);

    if (nRet == 1) {
        nRet = 0;
        if (nFuncResult < -10) {
            nRet = -2;
        } else {
            switch (nFuncResult) {
            case -1:
                nRet = -7;
                break;
            case -2:
                nRet = -3;
                break;
            case -4:
                nRet = -8;
                break;
            case -5:
                nRet = -10;
                break;
            case -3:
                nRet = -4;
                break;
            case -8:
                nRet = -1;
                break;
            case -7:
                nRet = -14;
                break;
            }
            switch (nFunc) {
            case 11:
                if (nFuncResult == -4) {
                    nRet = -11;
                }
                if (nFuncResult == -3) {
                    nRet = -4;
                }
                break;
            case 2:
            case 5:
                if (nFuncResult >= 0) {
                    nRet = nFuncResult;
                }
                break;
            }
        }
    } else {
        nRet = nRet == -1 ? -13 : -9999;
    }
    return nRet;
}

static void _convertMcTitle(CHAR* pszOut, const CHAR* pszSrc) {
    static const char szHan2Zen[513] = {
129, 64, 129, 64, 129, 64, 129, 64, 129, 64, 129, 64, 129, 64, 129, 64,
129, 64, 129, 64, 129, 64, 129, 64, 129, 64, 129, 64, 129, 64, 129, 64,
129, 64, 129, 64, 129, 64, 129, 64, 129, 64, 129, 64, 129, 64, 129, 64,
129, 64, 129, 64, 129, 64, 129, 64, 129, 64, 129, 64, 129, 64, 129, 64,
129, 64, 129, 73, 129, 104, 129, 148, 129, 144, 129, 147, 129, 149, 129, 102,
129, 105, 129, 106, 129, 150, 129, 123, 129, 67, 129, 124, 129, 68, 129, 94,
130, 79, 130, 80, 130, 81, 130, 82, 130, 83, 130, 84, 130, 85, 130, 86,
130, 87, 130, 88, 129, 70, 129, 71, 129, 131, 129, 129, 129, 132, 129, 72,
129, 151, 130, 96, 130, 97, 130, 98, 130, 99, 130, 100, 130, 101, 130, 102,
130, 103, 130, 104, 130, 105, 130, 106, 130, 107, 130, 108, 130, 109, 130, 110,
130, 111, 130, 112, 130, 113, 130, 114, 130, 115, 130, 116, 130, 117, 130, 118,
130, 119, 130, 120, 130, 121, 129, 109, 129, 95, 129, 110, 129, 79, 129, 81,
129, 101, 130, 129, 130, 130, 130, 131, 130, 132, 130, 133, 130, 134, 130, 135,
130, 136, 130, 137, 130, 138, 130, 139, 130, 140, 130, 141, 130, 142, 130, 143,
130, 144, 130, 145, 130, 146, 130, 147, 130, 148, 130, 149, 130, 150, 130, 151,
130, 152, 130, 153, 130, 154, 129, 111, 129, 98, 129, 112, 129, 96, 32, 32,
32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
32, 32, 129, 66, 129, 117, 129, 118, 129, 65, 129, 69, 131, 146, 131, 64,
131, 66, 131, 68, 131, 70, 131, 72, 131, 131, 131, 133, 131, 135, 131, 98,
129, 91, 131, 65, 131, 67, 131, 69, 131, 71, 131, 73, 131, 74, 131, 76,
131, 78, 131, 80, 131, 73, 131, 84, 131, 86, 131, 88, 131, 90, 131, 92,
131, 94, 131, 96, 131, 99, 131, 101, 131, 103, 131, 105, 131, 106, 131, 107,
131, 108, 131, 109, 131, 110, 131, 113, 131, 116, 131, 119, 131, 122, 131, 125,
131, 126, 131, 128, 131, 129, 131, 130, 131, 132, 131, 134, 131, 136, 131, 137,
131, 138, 131, 139, 131, 140, 131, 141, 131, 143, 131, 147, 129, 74, 129, 75,
32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
0
    };
    INT nLen = strlen(pszSrc);
    INT i;
    INT j = 0;
    BYTE c;

    for (i = 0; i < nLen; i++, j++) {
        c = pszSrc[i];
        if ((BYTE)(c + 0x7f) < 0x1f || (BYTE)(c + 0x20) < 0x1d) {
            pszOut[j++] = c;
            i++;
            pszOut[j] = pszSrc[i];
        } else if ((BYTE)(c - 0x20) < 0x60) {
            pszOut[j++] = szHan2Zen[c * 2];
            pszOut[j] = szHan2Zen[c * 2 + 1];
        } else if ((BYTE)(c + 0x5f) < 0x3f) {
            pszOut[j++] = szHan2Zen[c * 2];
            pszOut[j] = szHan2Zen[c * 2 + 1];
        } else if (c == '\n') {
            j--;
        } else {
            pszOut[j] = c;
        }
    }
}

void _setupIconSys(const CHAR* pszName) {
    CHAR* pszTitle = s_pszTitle;
    USHORT nLF = s_nOffsLF;
    char szTmp[80];
    CHAR* pszIcon;
    INT nLen;
    INT nID;

    switch (s_nDataType) {
    case MC_DATATYPE_SYS:
        pszTitle = "\x82j\x82g\x81@\x82q\x82" "d\x81" "F\x81@\x82" "b\x82\x8f\x82l\x82r\x82\x99\x82\x93\x82\x94\x82\x85\x82\x8d\x81@\x82" "c\x82\x81\x82\x94\x82\x81";
        pszIcon = "frec_sys.ico";
        nLF = 20;
        break;
    case MC_DATATYPE_GAME:
#line 593
        ASSERT(pszTitle);
        nLen = strlen(pszName);
        nID = 0;
        if (nLen >= 2) {
            nID = atoi(pszName + nLen - 2);
        }
        sprintf(szTmp, "\x82j\x82g\x81@\x82q\x82" "d\x81" "F\x81@\x82" "b\x82\x8f\x82l\x81@[%02d]%s", nID, pszTitle);
        nLF = 30;
        pszTitle = szTmp;
        pszIcon = "frec_dat.ico";
        break;
    case MC_DATATYPE_DEBUG:
        if (!pszTitle) {
            nLF = 32;
            pszTitle = "\x81uKHCOM\x81v\x83" "f\x83o\x83" "b\x83O\x8a\xc2\x8b\xab";
        }
        pszIcon = "slime1.ico";
        break;
    default:
        return;
    }
    memset(&s_Icon, 0, sizeof(s_Icon));
    memcpy(s_Icon.Head, "PS2D", 5);
    s_Icon.TransRate = 0x60;
    s_Icon.OffsLF = nLF;
    _convertMcTitle((CHAR*)s_Icon.TitleName, pszTitle);
    strcpy((char*)s_Icon.FnameView, pszIcon);
    strcpy((char*)s_Icon.FnameCopy, pszIcon);
    strcpy((char*)s_Icon.FnameDel, pszIcon);
    memcpy(s_Icon.BgColor, _ivBgColor, sizeof(_ivBgColor));
    memcpy(s_Icon.LightDir, _vLightDir, sizeof(_vLightDir));
    memcpy(s_Icon.LightColor, _vLightCol, sizeof(_vLightCol));
    memcpy(s_Icon.Ambient, _vAmbient, sizeof(_vAmbient));
}

static INT _checkCard(INT nPort) {
    INT r = -9999;
    INT nRes;
    INT nCmd;
    INT nFuncResult;

    switch (s_nMcCheckStep) {
    case 0:
        if (sceMcGetInfo(nPort, 0, &s_nMcType, &s_nMcFree, &s_nMcFormat) >= 0) {
            s_nMcLatestCmnd = 1;
            s_nMcCheckStep = 1;
        }
        break;
    case 1:
        nCmd = 1;
        nRes = sceMcSync(1, &nCmd, &nFuncResult);
        if (nRes == 0) {
            break;
        }
        s_nMcLatestCmnd = 0;
        if (nRes == -1) {
            s_nMcCheckStep = 0;
            break;
        }
        if (nRes != 1) {
            break;
        }
        r = -2;
        switch (nFuncResult) {
        case 0:
        case -1:
        case -2:
            if (s_nMcType == 2) {
                r = s_nMcFormat == 0 ? -3 : 1;
            }
            break;
        }
        s_nMcCheckStep = 0;
        break;
    }
    return r;
}

static INT _checkSpaceCard(INT nPort, INT* pnOutEntCnt) {
    INT r = -9999;
    INT nRes;
    INT nCmd;
    INT nFuncResult;

    switch (s_nMcCheckStep) {
    case 0:
        if (sceMcGetEntSpace(nPort, 0, ".") >= 0) {
            s_nMcLatestCmnd = 0x12;
            s_nMcCheckStep = 1;
        }
        break;
    case 1:
        nCmd = 0x12;
        nRes = sceMcSync(1, &nCmd, &nFuncResult);
        if (nRes == 0) {
            break;
        }
        s_nMcLatestCmnd = 0;
        if (nRes == -1) {
            s_nMcCheckStep = 0;
            break;
        }
        if (nRes != 1) {
            break;
        }
        s_nMcCheckStep = 0;
        if (nFuncResult >= 0) {
            *pnOutEntCnt = nFuncResult;
            r = 0;
        } else {
            *pnOutEntCnt = 0;
            r = -1;
        }
        break;
    }
    return r;
}

static INT _checkDirEnt(INT nPort, const CHAR* pszPath, sceMcTblGetDir* pOutBuf, INT nMaxEnt) {
    INT nFuncResult;
    INT nRes;
    INT nCmd;
    INT r = -9999;

    switch (s_nMcCheckStep) {
    case 0:
        nFuncResult = sceMcGetDir(nPort, 0, pszPath, 0, nMaxEnt, pOutBuf);
        if (nFuncResult == 0) {
            s_nMcCheckStep = 1;
            s_nMcLatestCmnd = 0xd;
            break;
        }
        s_nMcCheckStep = 0;
        r = -1;
        break;
    case 1:
        nCmd = 0xd;
        nRes = sceMcSync(1, &nCmd, &nFuncResult);
        if (nRes == 0) {
            break;
        }
        s_nMcLatestCmnd = 0;
        if (nRes == -1) {
            s_nMcCheckStep = 0;
            r = -1;
            break;
        }
        if (nRes != 1) {
            break;
        }
        r = nFuncResult;
        s_nMcCheckStep = 0;
        if (r < 0) {
            switch (r) {
            case -6:
            case -5:
            case -4:
                r = -1;
                break;
            case -2:
                r = -3;
                break;
            default:
                r = -2;
                break;
            }
        }
        break;
    }
    return r;
}

BOOL CMemoca::reqGetFreeSize(INT nPort, sceMcTblGetDir* pOutBuf, INT nMaxEnt) {
    if (s_nPhase != 0) {
        return FALSE;
    }
    s_nMcPort = nPort;
    s_nPhase = 0xd;
    s_nStep = 0;
    s_pRootDirTbl = pOutBuf;
    s_nMaxEnt = pOutBuf ? nMaxEnt : 0;
    s_nMcEntSpace = 0;
    return TRUE;
}

INT CMemoca::getFreeSize() {
    INT r = -9999;
    INT nRes;
    INT n;

    switch (s_nPhase) {
    case 0xd:
        nRes = _checkCard(s_nMcPort);
        if (nRes == -9999) {
            break;
        }
        if (nRes != 1) {
            r = nRes;
            s_nPhase = 0;
            break;
        }
        if (s_nMaxEnt == 0) {
            if (s_pTmpBuf) delete s_pTmpBuf;
#line 868
            s_pTmpBuf = new (__FILE__ "(868)") CBuf;
            s_pTmpBuf->allocBuf_A64(0x4000);
            s_pRootDirTbl = (sceMcTblGetDir*)s_pTmpBuf->getBufPtr();
        }
#line 872
        ASSERT(s_pRootDirTbl);
        s_nPhase = 0xe;
        break;
    case 0xe:
        nRes = _checkDirEnt(s_nMcPort, "/*", s_pRootDirTbl, s_nMaxEnt ? s_nMaxEnt : 0x100);
        if (nRes == -9999) {
            break;
        }
        if (s_pTmpBuf) delete s_pTmpBuf;
        s_pTmpBuf = __null;
        s_pRootDirTbl = __null;
        if (nRes < 0) {
            r = nRes;
            s_nPhase = 0;
            break;
        }
        s_nRootDirCnt = nRes;
        s_nPhase = 0xf;
        break;
    case 0xf:
        nRes = _checkSpaceCard(s_nMcPort, &n);
        if (nRes == -9999) {
            break;
        }
        if (nRes == 0) {
            s_nMcEntSpace = n;
            r = s_nMcFree * 2;
        } else {
            r = nRes;
        }
        s_nPhase = 0;
        break;
    }
    return r;
}

INT CMemoca::getRootDirCount() {
    return s_nRootDirCnt;
}

INT CMemoca::getFreeEntSpace() {
    return s_nMcEntSpace;
}

BOOL CMemoca::reqFormat(INT nPort) {
    if (s_nPhase != 0) {
        return FALSE;
    }
    s_nMcPort = nPort;
    s_nPhase = 0x13;
    s_nStep = 0;
    return TRUE;
}

static INT _McFormat(INT nPort) {
    INT r = -9999;
    INT nRes;

    switch (s_nStep) {
    case 0:
        nRes = _checkCard(nPort);
        if (nRes == -9999) {
            break;
        }
        if (nRes != 1 && nRes != -3) {
            r = nRes;
            s_nStep = 0;
        } else {
            s_nStep = 1;
        }
        break;
    case 1:
        if (sceMcFormat(nPort, 0) < 0) {
            s_nStep = 0;
        } else {
            s_nMcLatestCmnd = 0x10;
            s_nStep = 2;
        }
        break;
    case 2:
        nRes = _McSync(0x10);
        if (nRes == -9999) {
            break;
        }
        s_nMcLatestCmnd = 0;
        if (nRes == 0) {
            r = 0;
            s_nStep = 0;
        } else {
            s_nStep = 0;
            r = -1;
        }
        if (nRes == -13) {
            s_nStep = 0;
            r = -9999;
        }
        break;
    }
    return r;
}

INT CMemoca::formatProc() {
    INT r = -9999;
    INT nRes;

    if (s_nPhase == 0x13) {
        nRes = _McFormat(s_nMcPort);
        if (nRes != -9999) {
            s_nPhase = 0;
            r = nRes;
        }
    }
    return r;
}

static INT _McMkdir(const CHAR* pszName) {
    INT r = -9999;
    INT nRes;

    switch (s_nStep) {
    case 0:
        nRes = _checkCard(s_nMcPort);
        if (nRes == -9999) {
            break;
        }
        if (nRes != 1) {
            r = nRes;
            s_nStep = 0;
        } else {
            s_nStep = 1;
        }
        break;
    case 1:
        if (sceMcMkdir(s_nMcPort, 0, pszName) != 0) {
            s_nStep = 0;
        } else {
            s_nMcLatestCmnd = 0xb;
            s_nStep = 2;
        }
        break;
    case 2:
        nRes = _McSync(0xb);
        if (nRes == -9999) {
            break;
        }
        s_nMcLatestCmnd = 0;
        if (nRes == 0 || nRes == -11 || nRes == -13) {
            r = 0;
        } else {
            r = nRes;
        }
        s_nStep = 0;
        break;
    }
    return r;
}

static INT _McOpen(const CHAR* pszName, INT nMode) {
    INT r = -9999;
    INT nRes;

    switch (s_nStep) {
    case 0:
        nRes = _checkCard(s_nMcPort);
        if (nRes == -9999) {
            break;
        }
        if (nRes != 1) {
            r = nRes;
            s_nStep = 0;
        } else {
            s_nStep = 1;
        }
        break;
    case 1:
        nRes = sceMcOpen(s_nMcPort, 0, pszName, nMode);
        if (nRes < 0) {
            if (nRes == -200) {
                s_nStep = 0;
            } else {
                s_nStep = 0;
                r = -1;
            }
        } else {
            s_nMcLatestCmnd = 2;
            s_nStep = 2;
            s_nMcHandle = r;
        }
        break;
    case 2:
        nRes = _McSync(2);
        if (nRes == -9999) {
            break;
        }
        s_nMcHandle = nRes;
        s_nMcLatestCmnd = 0;
        if (nRes >= 0) {
            r = nRes;
        } else {
            switch (nRes) {
            case -3:
            case -4:
            case -7:
            case -8:
            case -14:
                r = nRes;
                break;
            default:
                r = -2;
                break;
            }
        }
        s_nStep = 0;
        break;
    }
    return r;
}

static INT _McClose(INT nFd) {
    INT r = -9999;
    INT nRes;

    switch (s_nStep) {
    case 0:
        nRes = _checkCard(s_nMcPort);
        if (nRes == -9999) {
            break;
        }
        if (nRes != 1) {
            r = nRes;
            s_nStep = 0;
        } else {
            s_nStep = 1;
        }
        break;
    case 1:
        nRes = sceMcClose(nFd);
        if (nRes < 0) {
            if (nRes == -200) {
                s_nStep = 0;
            } else {
                s_nStep = 0;
                r = -1;
            }
        } else {
            s_nMcHandle = -1;
            s_nMcLatestCmnd = 3;
            s_nStep = 2;
        }
        break;
    case 2:
        nRes = _McSync(3);
        if (nRes == -9999) {
            break;
        }
        s_nMcLatestCmnd = 0;
        r = 0;
        if (nRes != 0) {
            r = nRes;
            if (r == -8) {
                r = -9;
            }
        }
        s_nStep = 0;
        break;
    }
    return r;
}

static INT _McRead(INT nFd, BYTE* pDst, INT nSize) {
    INT r = -9999;
    INT nRes;

    switch (s_nStep) {
    case 0:
        nRes = _checkCard(s_nMcPort);
        if (nRes == -9999) {
            break;
        }
        if (nRes == 1) {
            s_nStep = 1;
        } else {
            r = nRes;
            s_nStep = 0;
        }
        break;
    case 1:
        nRes = sceMcRead(nFd, pDst, nSize);
        if (nRes == 0) {
            s_nMcLatestCmnd = 5;
            s_nStep = 2;
        } else {
            s_nStep = 0;
            if (nRes != -200) {
                r = -1;
            }
        }
        break;
    case 2:
        nRes = _McSync(5);
        if (nRes == -9999) {
            break;
        }
        s_nMcLatestCmnd = 0;
        switch (nRes) {
        case -4:
            r = -15;
            break;
        case -8:
            r = -9;
            break;
        default:
            r = nRes;
            break;
        }
        s_nStep = 0;
        break;
    }
    return r;
}

static INT _McWrite(INT nFd, const BYTE* pSrc, INT nSize) {
    INT r = -9999;
    INT nRes;

    switch (s_nStep) {
    case 0:
        nRes = _checkCard(s_nMcPort);
        if (nRes == -9999) {
            break;
        }
        if (nRes != 1) {
            r = nRes;
            s_nStep = 0;
        } else {
            s_nStep = 1;
        }
        break;
    case 1:
        nRes = sceMcWrite(nFd, pSrc, nSize);
        if (nRes == 0) {
            s_nMcLatestCmnd = 6;
            s_nStep = 2;
        } else {
            s_nStep = 0;
            if (nRes != -200) {
                r = -1;
            }
        }
        break;
    case 2:
        nRes = _McSync(6);
        if (nRes == -9999) {
            break;
        }
        s_nMcLatestCmnd = 0;
        if (nRes == 0) {
            s_nStep = 3;
            break;
        }
        r = nRes;
        if (r == -8) {
            r = -9;
        }
        s_nStep = 0;
        break;
    case 3:
        if (sceMcFlush(nFd) != 0) {
            break;
        }
        s_nMcLatestCmnd = 10;
        s_nStep = 4;
        break;
    case 4:
        nRes = _McSync(10);
        if (nRes == -9999) {
            break;
        }
        s_nStep = 0;
        s_nMcLatestCmnd = 0;
        r = nRes;
        break;
    }
    return r;
}

static INT _McDelete(const CHAR* pszName) {
    INT r = -9999;
    INT nRes;

    switch (s_nStep) {
    case 0:
        nRes = _checkCard(s_nMcPort);
        if (nRes == -9999) {
            break;
        }
        if (nRes != 1) {
            r = nRes;
            s_nStep = 0;
        } else {
            s_nStep = 1;
        }
        break;
    case 1:
        if (sceMcDelete(s_nMcPort, 0, pszName) == 0) {
            s_nMcLatestCmnd = 0xf;
            s_nStep = 2;
        } else {
            s_nStep = 0;
            r = -1;
        }
        break;
    case 2:
        nRes = _McSync(0xf);
        if (nRes == -9999) {
            break;
        }
        s_nStep = 0;
        s_nMcLatestCmnd = 0;
        r = nRes;
        break;
    }
    return r;
}

static INT _McGetDirFileEntries(const CHAR* pszName) {
    INT r = -9999;
    INT nRes;
    INT nFuncResult;
    INT nCmd;

    switch (s_nStep) {
    case 0:
        nRes = _checkCard(s_nMcPort);
        if (nRes == -9999) {
            break;
        }
        if (nRes != 1) {
            r = nRes;
            s_nStep = 0;
        } else {
            memset(s_DirInfo, 0, sizeof(s_DirInfo));
            s_nStep = 1;
        }
        break;
    case 1:
        if (sceMcGetDir(s_nMcPort, 0, pszName, 0, 5, s_DirInfo) == 0) {
            s_nStep = 2;
            s_nDirentCount = 0;
        } else {
            s_nStep = 0;
            r = -1;
        }
        break;
    case 2:
        nCmd = 0xd;
        nRes = sceMcSync(1, &nCmd, &nFuncResult);
        if (nRes == -1) {
            r = -13;
        } else {
            if (nRes != 1) {
                break;
            }
            if (nFuncResult >= 0) {
                r = 0;
                s_nDirentCount = nFuncResult;
            } else {
                switch (nFuncResult) {
                case -2:
                    r = -3;
                    break;
                case -4:
                    r = -8;
                    break;
                case -6:
                    r = -15;
                    break;
                default:
                    r = -1;
                    break;
                }
            }
        }
        s_nStep = 0;
        break;
    }
    return r;
}

static UINT _MakeCheckCode(const BYTE* pBuff, INT nSize, _CHECKTYPE type) {
    UINT nCode = 0;

    if (type == _CHECKTYPE_CHECKSUM) {
        while (nSize--) {
            nCode += *pBuff++;
        }
    } else if (type == _CHECKTYPE_CRC32) {
        nCode = crc1(nSize, pBuff);
    }
    return nCode;
}

static void _MakeMcHeader(BYTE* pOutBuff, INT nSize, _CHECKTYPE type) {
    _MC_FILE_HEADER* p = (_MC_FILE_HEADER*)pOutBuff;
    UINT nCode = _MakeCheckCode(pOutBuff + sizeof(_MC_FILE_HEADER), nSize - sizeof(_MC_FILE_HEADER), type);
    p->_nVersion = 7;
    p->_nCheckCode = nCode;
    p->_nDataLen = nSize - sizeof(_MC_FILE_HEADER);
    p->_Filler = 0;
}

static INT _McCheckData(const BYTE* pBuff, _CHECKTYPE type, UINT nChkSize) {
    const BYTE* p = pBuff + sizeof(_MC_FILE_HEADER);

    if (type == _CHECKTYPE_CRC32 && ((const _MC_FILE_HEADER*)pBuff)->_nVersion != 7) {
        return -50;
    }
    if (((const _MC_FILE_HEADER*)pBuff)->_nDataLen != nChkSize) {
        return -12;
    }
    if (((const _MC_FILE_HEADER*)pBuff)->_nCheckCode != _MakeCheckCode(p, ((const _MC_FILE_HEADER*)pBuff)->_nDataLen, type)) {
        return -12;
    }
    return 0;
}

static void _getMcIconSysPath(CHAR* pszOutPath, const CHAR* pszFolder) {
    sprintf(pszOutPath, "/%s/icon.sys", pszFolder);
}

static INT _McWriteIconSys(const CHAR* pszName) {
    INT nRes;
    INT r = -9999;
    char szSrc[128];

    switch (s_nIconStep) {
    case 0:
        _getMcIconSysPath(szSrc, pszName);
        nRes = _McOpen(szSrc, 0x203);
        if (nRes == 0) {
            s_nIconStep = 1;
            break;
        }
        if (nRes == -9999) {
            break;
        }
        r = nRes;
        s_nIconStep = 0;
        break;
    case 1:
        nRes = _McWrite(s_nMcHandle, (const BYTE*)&s_Icon, sizeof(s_Icon));
        if (nRes == 0) {
            s_nIconStep = 2;
            break;
        }
        if (nRes == -9999) {
            break;
        }
        r = nRes;
        s_nIconStep = 0;
        break;
    case 2:
        nRes = _McClose(s_nMcHandle);
        if (nRes == 0) {
            s_nIconStep = 0;
            r = 0;
            break;
        }
        if (nRes == -9999) {
            break;
        }
        s_nIconStep = 0;
        r = nRes;
        break;
    }
    return r;
}

INT _McCheckDataFileExist(INT nMode, const CHAR* pszName) {
    INT nRes;
    char szTmp[256];
    INT r = -9999;
    sceMcTblGetDir* pInfo;
    INT i;

    sprintf(szTmp, "/%s/*", pszName);
    nRes = _McGetDirFileEntries(szTmp);
    if (nRes != r) {
    if (nRes == 0) {
        asm volatile("" : : : "$16"); /* MATCH: s0 is dead here, for reorg: the bnez slot takes r = nRes unannulled */
        s_nStatus = 0;
        for (i = nRes, pInfo = s_DirInfo; i < s_nDirentCount; i++, pInfo++) {
            if (pInfo->EntryName[0] == '.') {
                continue;
            }
            if (strcmp((const char*)pInfo->EntryName, "icon.sys") == 0) {
                if (pInfo->FileSizeByte == sizeof(sceMcIconSys)) {
                    s_nStatus |= 1;
                }
            } else if (strcmp((const char*)pInfo->EntryName, pszName) == 0) {
                s_nStatus |= 4;
                s_LastCheckFileTime = pInfo->_Modify;
            }
        }
        if ((s_nStatus & 5) == 5 && s_nDirentCount == 5) {
            r = 0;
        } else {
            r = nMode == 1 ? -8 : -12;
        }
    } else {
        r = nRes;
    }
    return r;
    }
    return -9999;
}

static void _getMcDirPath(CHAR* pszOutPath, const CHAR* pszDir) {
    sprintf(pszOutPath, "/%s", pszDir);
}

static void _getMcFilePath(CHAR* pszOutPath, const CHAR* pszFolder, const CHAR* pszFile) {
    sprintf(pszOutPath, "/%s/%s", pszFolder, pszFile ? pszFile : pszFolder);
}

INT _McWriteIcons(const CHAR* pszName) {
    INT r = -9999;
    INT nRes;
    char szTmp[256];
    CHAR* pszFile;

    switch (s_nIconStep) {
    case 0:
        switch (s_nIconCount) {
        case 0:
            pszFile = (CHAR*)s_Icon.FnameView;
            break;
        case 1:
            pszFile = (CHAR*)s_Icon.FnameCopy;
            break;
        case 2:
            pszFile = (CHAR*)s_Icon.FnameDel;
            break;
        }
        s_pIconRsrc = CRsrcDB::getRsrc(1, pszFile, (NAME_CASE)0);
        if (!s_pIconRsrc || !s_pIconRsrc->getRsrcData()) {
            TRACE("\x83\x81\x83\x82\x83J\x83" "A\x83" "C\x83R\x83\x93\x81" "E\x83\x8a\x83\\\x81[\x83X\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\n");
            break;
        }
        s_nIconStep = 1;
        break;
    case 1:
        switch (s_nIconCount) {
        case 0:
            pszFile = (CHAR*)s_Icon.FnameView;
            break;
        case 1:
            pszFile = (CHAR*)s_Icon.FnameCopy;
            break;
        case 2:
            pszFile = (CHAR*)s_Icon.FnameDel;
            break;
        }
        _getMcFilePath(szTmp, pszName, pszFile);
        nRes = _McOpen(szTmp, 0x202);
        if (nRes >= 0) {
            s_nIconStep = 2;
            break;
        }
        if (nRes == -9999) {
            break;
        }
        r = nRes;
        s_nIconStep = 0;
        break;
    case 2:
        {
            CRsrcData* p = s_pIconRsrc;
            /* MATCH: the empty asm keeps the two m_pDataSrc tests apart: threaded, the first skips the second */
            nRes = _McWrite(s_nMcHandle, p->getRsrcData(), ({ asm volatile(""); p->getDataSize(); }));
        }
        if (nRes == 0) {
            s_nIconStep = 3;
            break;
        }
        if (nRes == -9999) {
            break;
        }
        r = nRes;
        s_nIconStep = 0;
        break;
    case 3:
        nRes = _McClose(s_nMcHandle);
        if (nRes == 0) {
            s_nIconStep = 0;
            s_nIconCount = 0;
            r = 0;
            break;
        }
        if (nRes == -9999) {
            break;
        }
        s_nIconStep = 0;
        r = nRes;
        break;
    }
    return r;
}

static INT _McDataWrite() {
    INT r = -9999;
    INT nRes;
    char szSrc[128];
    BOOL bError = FALSE;

    switch (s_nPhase) {
    case 5:
        nRes = _McCheckDataFileExist(1, s_szMcFileName);
        if (nRes == -9999) {
            break;
        }
        if (nRes == 0 || nRes == -8) {
            if (nRes == -8) {
                s_nStatus = 0;
            }
            if (s_nStatus == 0) {
                s_bExistFlag = FALSE;
                s_nPhase = 6;
            } else if (s_nStatus & 4) {
                s_bExistFlag = TRUE;
                s_nPhase = 7;
            }
            break;
        }
        r = nRes;
        bError = TRUE;
        break;
    case 6:
        _getMcDirPath(szSrc, s_szMcFileName);
        nRes = _McMkdir(szSrc);
        if (nRes == 0) {
            s_nPhase = 8;
            break;
        }
        if (nRes == -9999) {
            break;
        }
        r = nRes;
        bError = TRUE;
        break;
    case 7:
        _getMcIconSysPath(szSrc, s_szMcFileName);
        nRes = _McDelete(szSrc);
        if (nRes == -9999) {
            break;
        }
        if (nRes != 0) {
            r = nRes;
            bError = TRUE;
            break;
        }
        s_nPhase = 8;
        break;
    case 8:
        _getMcFilePath(szSrc, s_szMcFileName, __null);
        nRes = _McOpen(szSrc, 0x203);
        if (nRes != 0) {
            if (nRes == -9999) {
                break;
            }
            r = nRes == -10 ? -2 : nRes;
            bError = TRUE;
            break;
        }
        if (s_nDataType != MC_DATATYPE_SYS) {
            _MakeMcHeader(s_pSaveData->getBufPtr(), s_nMcFileSize,
                          s_nDataType == MC_DATATYPE_DEBUG ? _CHECKTYPE_CHECKSUM : _CHECKTYPE_CRC32);
        }
        s_nPhase = 9;
        break;
    case 9:
        nRes = _McWrite(s_nMcHandle, s_pSaveData->getBufPtr(), s_nMcFileSize);
        if (nRes == 0) {
            if (s_pSaveData) delete s_pSaveData;
            s_pSaveData = __null;
            s_nPhase = 10;
            break;
        }
        if (nRes == -9999) {
            break;
        }
        if (s_pSaveData) delete s_pSaveData;
        s_pSaveData = __null;
        r = nRes == -10 ? -2 : nRes;
        bError = TRUE;
        break;
    case 10:
        nRes = _McClose(s_nMcHandle);
        if (nRes == 0) {
            _setupIconSys(s_szMcFileName);
            s_nPhase = 11;
            break;
        }
        if (nRes == -9999) {
            break;
        }
        r = nRes;
        bError = TRUE;
        break;
    case 11:
        nRes = s_bExistFlag ? 0 : _McWriteIcons(s_szMcFileName);
        if (nRes == 0) {
            s_nPhase = 12;
            break;
        }
        if (nRes == -9999) {
            break;
        }
        r = nRes == -10 ? -2 : nRes;
        bError = TRUE;
        break;
    case 12:
        nRes = _McWriteIconSys(s_szMcFileName);
        if (nRes == -9999) {
            break;
        }
        if (nRes == 0) {
            r = 0;
        } else {
            r = nRes == -10 ? -2 : nRes;
            bError = TRUE;
        }
        s_nPhase = 0;
        break;
    }
    if (bError) {
        s_nPhase = 0;
    }
    return r;
}

INT CMemoca::saveProc() {
    INT nResult = _McDataWrite();
    if (nResult != -9999) {
        if (s_pSaveData) delete s_pSaveData;
        s_pSaveData = __null;
    }
    return nResult;
}

static INT _McDataRead() {
    INT r = -9999;
    INT nRes;
    char szSrc[128];
    _CHECKTYPE checkType;
    INT nHeaderSize;

    switch (s_nPhase) {
    case 1:
        _getMcFilePath(szSrc, s_szMcFileName, __null);
        nRes = _McOpen(szSrc, 1);
        if (nRes != 0) {
            if (nRes != -9999) {
                r = nRes == -10 ? -2 : nRes;
                s_nPhase = 0;
            }
            break;
        }
        if (s_pLoadData) delete s_pLoadData;
#line 2266
        s_pLoadData = new (__FILE__ "(2266)") CBuf;
        if (s_nDataType == MC_DATATYPE_SYS) {
            s_pLoadData->allocBuf_A64(s_nMcFileSize);
        } else {
            s_pLoadData->allocBuf_A64(s_nMcFileSize + 0x10);
            s_nMcFileSize += 0x10;
        }
        s_nPhase = 2;
        break;
    case 2:
        nRes = _McRead(s_nMcHandle, s_pLoadData->getBufPtr(), s_nMcFileSize);
        if (nRes >= 0) {
            s_nPhase = 3;
            s_nResultSize = nRes;
            break;
        }
        if (nRes == -9999) {
            break;
        }
        switch (nRes) {
        case -9:
        case -15:
            r = -12;
            break;
        case -10:
            r = -2;
            break;
        default:
            r = nRes;
            break;
        }
        s_nPhase = 0;
        break;
    case 3:
        nRes = _McClose(s_nMcHandle);
        if (nRes == 0) {
            s_nPhase = 4;
            break;
        }
        if (nRes != -9999) {
            r = nRes == -10 ? -2 : nRes;
            s_nPhase = 0;
        }
        break;
    case 4:
        nHeaderSize = 0x10;
        switch (s_nDataType) {
        case MC_DATATYPE_SYS:
            checkType = _CHECKTYPE_NONE;
            nHeaderSize = 0;
            break;
        case MC_DATATYPE_GAME_HEAD:
            checkType = _CHECKTYPE_NONE;
            break;
        case MC_DATATYPE_GAME:
            checkType = _CHECKTYPE_CRC32;
            break;
        default:
            checkType = _CHECKTYPE_CHECKSUM;
            break;
        }
        if (checkType == _CHECKTYPE_NONE) {
            r = 0;
        } else {
            r = _McCheckData(s_pLoadData->getBufPtr(), checkType, s_nMcFileSize - nHeaderSize);
        }
        if (r == 0) {
            memcpy(s_pMcBuff, s_pLoadData->getBufPtr() + nHeaderSize, s_nMcFileSize - nHeaderSize);
        }
        if (s_pLoadData) delete s_pLoadData;
        s_pLoadData = __null;
        s_nPhase = 0;
        break;
    }
    return r;
}

INT CMemoca::loadProc() {
    return _McDataRead();
}

INT CMemoca::checkDataFileAttr(INT nMode, const CHAR* pszName, INT nPort) {
    s_nMcPort = nPort;
    return _McCheckDataFileExist(nMode, pszName);
}

BOOL CMemoca::reqGetDirSize(INT nPort, const CHAR* pszDir) {
    if (s_nPhase != 0) {
        return FALSE;
    }
    sprintf(s_szMcFileName, "/%s/*", pszDir);
    s_nMcPort = nPort;
    s_nPhase = 0x12;
    s_nStep = 0;
    return TRUE;
}

INT CMemoca::getDirSize() {
    INT r = -9999;
    INT nRes;

    if (s_nPhase == 0x12) {
        nRes = _McGetDirFileEntries(s_szMcFileName);
        if (nRes != -9999) {
            if (nRes == 0) {
                nRes = getDirSizeAfterCheckDir();
            }
            s_nPhase = 0;
            r = nRes;
        }
    }
    return r;
}

INT CMemoca::getDirSizeAfterCheckDir() {
    INT nSize;
    INT nFileCount;
    INT i;

    nFileCount = s_nDirentCount < 2 ? 0 : s_nDirentCount - 2;
    nSize = nFileCount * 0x200 + 0x800;
    for (i = 0; i < s_nDirentCount; i++) {
        if (*s_DirInfo[i].EntryName != '.') {
            INT value = s_DirInfo[i].FileSizeByte + 0x3ff;
            nSize += value & ~0x3ff;
        }
    }
    return nSize;
}

sceMcStDateTime CMemoca::getLastCheckFileTime() {
    return s_LastCheckFileTime;
}

INT CMemoca::idleCheckCard(INT nPort) {
    INT nRes;
    INT r = -9999;
    INT nCmd;
    INT nFuncResult;

    switch (s_nIdleStep) {
    case 0:
        if (sceMcGetInfo(nPort, 0, &s_nMcType, __null, __null) >= 0) {
            s_nIdleStep = 1;
        }
        break;
    case 1:
        nCmd = 1;
        nRes = sceMcSync(1, &nCmd, &nFuncResult);
        if (nRes == -1) {
            s_nIdleStep = 0;
            break;
        }
        if (nRes != 1) {
            break;
        }
        s_nIdleStep = 0;
        r = (nFuncResult == 0 && s_nMcType == 2) ? 0 : -2;
        break;
    }
    return r;
}

CHAR* CMemoca::getSysFileNameCOM() {
    return s_szSysFileName;
}

CHAR* CMemoca::getGameDataFileName() {
    return s_szGameFileName;
}

BOOL CMemoca::reqSave(const DebugParam& param) {
    return reqSave(0, "BISLPS-99999-BF2", (const BYTE*)&param, 0x40, MC_DATATYPE_DEBUG, __null, 0);
}

BOOL CMemoca::reqLoad(DebugParam& param) {
    return reqLoad(0, "BISLPS-99999-BF2", (BYTE*)&param, 0x40, MC_DATATYPE_DEBUG);
}

BOOL CMemoca::reqSaveSysTest(BOOL bSora, BOOL bRiku, BOOL bFlag) {
    SysDataCOM s;
    INT i;

    TRACE("sizeof(SysDataCOM) = %d\n", sizeof(SysDataCOM));
    memset(&s, 0, sizeof(s));
    s.Data.SoraClear = (bSora | bRiku) != 0;
    s.Data.RikuClear = (bRiku != 0);
    s.Header.Id = 0x4d4f43;
    s.Header.Area = 0x504a;
    s.Header.Version = 1;
    if (s.Data.SoraClear) {
        if (bFlag) {
            for (i = 0; i < 6; i++) {
                s.Data.PoohClearIntro[i] = 1;
                s.Data.PoohClear[i] = 3;
            }
            s.Data.LastPoohEvent = 0;
            s.Data.PoohIntro = 0xff;
        } else {
            CGameWork::getPoohSysFlag(s);
        }
        s.Data.LatestGameType = 2;
        s.Data.JiminyMemoComplete = 1;
        for (i = 0; i < 13;) {
            WorldSelTable* pTbl = CSceneInfo::getSelectableWorldTable(i, TRUE);
            UINT j;
            for (j = 0; j < pTbl->_nCount; j++) {
                s.Data.WorldTblSora[i++] = pTbl->_pTbl[j];
            }
        }
    }
    if (s.Data.RikuClear) {
        s.Data.LatestGameType = 1;
        s.Data.DReportComplete = 1;
        for (i = 0; i < 12;) {
            WorldSelTable* pTbl = CSceneInfo::getSelectableWorldTable(i, FALSE);
            UINT j;
            for (j = 0; j < pTbl->_nCount; j++) {
                s.Data.WorldTblRiku[i++] = pTbl->_pTbl[j];
            }
        }
    }
    return reqSave(0, s_szSysFileName, (const BYTE*)&s, sizeof(s), MC_DATATYPE_SYS, __null, 0);
}

BOOL CMemoca::reqLoadSysTest(CBuf& buf) {
    if (buf.getBufSize() < 0x400) {
        buf.releaseBuf(0);
        buf.allocBuf(0x400);
    }
    return reqLoad(0, s_szSysFileName, buf.getBufPtr(), 0x400, MC_DATATYPE_SYS);
}

BOOL CMemoca::reqUnformat(INT nPort) {
    if (s_nPhase != 0) {
        return FALSE;
    }
    s_nMcPort = nPort;
    s_nPhase = 0x14;
    s_nStep = 0;
    return TRUE;
}

static INT _McUnformat(INT nPort) {
    INT r = -9999;
    INT nRes;

    switch (s_nStep) {
    case 0:
        nRes = _checkCard(nPort);
        if (nRes == -9999) {
            break;
        }
        if (nRes != 1) {
            r = nRes;
            s_nStep = 0;
        } else {
            s_nStep = 1;
        }
        break;
    case 1:
        if (sceMcUnformat(nPort, 0) < 0) {
            s_nStep = 0;
        } else {
            s_nMcLatestCmnd = 0xf;
            s_nStep = 2;
        }
        break;
    case 2:
        nRes = _McSync(0x11);
        if (nRes == -9999) {
            break;
        }
        s_nMcLatestCmnd = 0;
        if (nRes == 0) {
            r = 0;
            s_nStep = 0;
        } else {
            s_nStep = 0;
            r = -1;
        }
        if (nRes == -13) {
            s_nStep = 0;
            r = -9999;
        }
        break;
    }
    return r;
}

INT CMemoca::unformatProc() {
    INT r = -9999;
    INT nRes;

    if (s_nPhase == 0x14) {
        nRes = _McUnformat(s_nMcPort);
        if (nRes != -9999) {
            s_nPhase = 0;
            r = nRes;
        }
    }
    return r;
}
// SOURCE DATA: .data .rodata .bss
