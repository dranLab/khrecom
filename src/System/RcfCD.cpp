#define RSRCDATA_H_PATH "c:/KHCOM/MainApp/System/RsrcData.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/RcfCD.h"
#include "System/Buf.h"
#include "System/SJoint.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"
#include "System/mcd.h"
#include "System/Thread.h"
#include "System/FileIO.h"
#include "System/VramMan.h"
#include "System/Movie/MovieFiles.h"
#include "include_asm.h"

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)
#define SJ_FOREACH(T, pEntry, list) \
    for (T *pEntry = (list).getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry; \
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL)

extern "C" {
int DelayThread(int usec);
int DIntr();
int EIntr();
int GetThreadId();
int atoi(const char* s);
int strncmp(const char* s1, const char* s2, size_t n);
int strncasecmp(const char* s1, const char* s2, size_t n);
int strcasecmp(const char* s1, const char* s2);
char* strcpy(char* dst, const char* src);
char* strstr(const char* s1, const char* s2);
int sprintf(char* buf, const char* fmt, ...);
}

BYTE* ExpandData(BYTE* pSrc, BYTE* pDst);

enum NAME_CASE { NAME_CASE_0 };

class CRsrcData;

struct RsrcInfo {
    CHAR* _pszName;
    INT _nGroupID;
    INT _nRefGroupID;
    INT _nRsrcType;
    BOOL _bSceneManaged;
    BYTE _cOption;
    BYTE _cParam;

    RsrcInfo() : _nRefGroupID(0), _cOption(0), _cParam(1) {}
};

class CRsrcDB { /* System/RsrcDB.h */
public:
    static void addRsrcType(const CHAR* pszExt, INT nID);
    static INT getRsrcAlignmentSize(INT nType);
    static CRsrcData* getRsrc(INT nType, const CHAR* pszName, NAME_CASE nCase);
    static void addRsrcTag(CRsrcData& data, INT nGroupID, INT bSceneManaged);
    static INT addRsrc(const RsrcInfo& info, CBuf& buf, CRsrcData** ppData);
    static INT addRsrc_NoCheck(const RsrcInfo& info, CBuf& buf, CRsrcData** ppData);
    static void addStreamInfo(const CHAR* pszName, INT nGroupID, UINT nStart, UINT nSize, INT nType);
};

class CRcf { /* System/Rcf.h */
public:
    static void setLocale(INT nLocale);
    static INT getLocale();
    static BOOL isSceneManagedMode();
};

#include "Game/SceneInfo.h"

struct RcrsExtData {
    CHAR* _szExt;
    INT _nID;
};

struct SectBlockHeader {
    char _szSig[4];
    USHORT _nVersion;
    USHORT _nSectInfoCnt;
    UINT _nIndexSectCnt;
};

struct SectorInfoEntry {
    char _szName[16];
    UINT _nStartSect;
    UINT _nSectCnt;
    UINT _nAddress;
    UINT _nEntryCount;

    BOOL setup(UINT nTopAddress) {
        if (_nAddress) {
            _nAddress += nTopAddress;
            return TRUE;
        }
        return FALSE;
    }
};

struct CD_GROUP_SECTOR {
    UINT _nRCF;
    UINT _nStart;
    UINT _nSectSize;
    BYTE _nHedSize;
    BYTE _nSeRange;
    BYTE _nOption;
    BYTE _nReverb;
    unsigned int _nExtInfo[4];

    void setup(UINT nGroup, UINT nSectOffset) {
        _nRCF = nGroup * 10000 + _nRCF;
        _nStart += nSectOffset;
    }
};

struct CD_MEMBER_HED {
    char _szDataName[24];
    UINT _nSize;
    UINT _nGroupID;
    UINT _nLinkID;
    UINT _nSectSize;
    UINT _nPosition;
    BYTE _nType;
    BYTE _nCompress;
    USHORT _nLocale;
};

struct CD_MEMBER_DATA : public CSJointT<CD_MEMBER_DATA> {
    char _szDataName[24];
    UINT _nStart;
    UINT _nSectSize;
    UINT _nSize;
    UINT _nLinkID;
    UINT _nGroupID;
    BYTE _nType;
    BYTE _nCompress;
    USHORT _nLocale;
    CBuf* _pBuff;

    CD_MEMBER_DATA() : _pBuff(NULL) {}
    virtual ~CD_MEMBER_DATA() { delete _pBuff; }
    void setData(const CD_MEMBER_HED& src, UINT nSectOffset) {
        strcpy(_szDataName, src._szDataName);
        _nStart = src._nPosition + nSectOffset;
        _nSectSize = src._nSectSize;
        _nSize = src._nSize;
        _nLinkID = src._nLinkID;
        _nGroupID = src._nGroupID;
        _nType = src._nType;
        _nCompress = src._nCompress;
        _nLocale = src._nLocale;
        _pBuff = NULL;
    }
};

typedef CSJointListT<CD_MEMBER_DATA> CDMemberList;

static RcrsExtData s_RcrsExtTbl[70] = {
    { "ABC", 37 },
    { "abc", 38 },
    { "AFF", 13 },
    { "aff", 13 },
    { "ALF", 22 },
    { "ASD", 36 },
    { "BIN", 23 },
    { "CAP", 27 },
    { "cam", 28 },
    { "OCD", 30 },
    { "MCD", 4 },
    { "mcd", 4 },
    { "CSD", 20 },
    { "csd", 20 },
    { "CTD", 54 },
    { "DLL", 31 },
    { "DPD", 10 },
    { "dpd", 10 },
    { "DPX", 5 },
    { "dpx", 5 },
    { "EET", 26 },
    { "eet", 26 },
    { "EFF", 14 },
    { "EPD", 7 },
    { "epd", 21 },
    { "ESD", 18 },
    { "esd", 18 },
    { "ESD", 32 },
    { "ESE", 62 },
    { "ese", 62 },
    { "fo2", 25 },
    { "GSD", 33 },
    { "GSD", 34 },
    { "imd", 16 },
    { "imz", 17 },
    { "lad", 60 },
    { "LIN", 39 },
    { "MAD", 35 },
    { "AMF", 6 },
    { "AZF", 9 },
    { "MDL", 11 },
    { "MEP", 40 },
    { "MLF", 41 },
    { "MMP", 51 },
    { "MMP", 52 },
    { "AAF", 3 },
    { "ANM", 8 },
    { "ANN", 29 },
    { "PSS", 61 },
    { "PTD", 49 },
    { "PTD", 50 },
    { "RSD", 46 },
    { "RSD", 47 },
    { "RTM", 12 },
    { "SES", 48 },
    { "SMD", 42 },
    { "SMT", 43 },
    { "SND", 44 },
    { "SND", 45 },
    { "spd", 56 },
    { "SPR", 55 },
    { "sqd", 59 },
    { "tm2", 2 },
    { "TXA", 19 },
    { "txa", 24 },
    { "VSM", 57 },
    { "VSM", 58 },
    { "VSV", 53 },
    { "VTM", 15 },
    { "END", 0 },
};

namespace {
SectorInfoEntry* s_pSectInfoBlock = NULL;
unsigned char s_pCdBuff[32768];
unsigned char s_pCbStack[16384];
SectBlockHeader s_SectBlockHeader;
SectorInfoEntry* s_pGroupDataInfoTbl[64];
unsigned short s_nGroupNumTbl[64];
INT s_nGroupCount;
CD_GROUP_SECTOR* s_pCurrentGrp;
CDMemberList* s_pCdMemberDataList;
CD_MEMBER_DATA* s_pCdMember;
ULONG s_nStartSector;
INT s_nEndSectorCnt;
INT s_nCdReadLoop;
INT s_nCdReadOdd;
BYTE* s_pDecodedBuf;
BYTE* s_pDecodedBuf_R;
INT s_nReadResult;
ULONG s_nStartSector_R;
INT s_nSectCnt_R;
INT s_nReadStep;
INT s_nCurrentGrp;
INT s_nNoCompression;
SHORT s_nModeFlag;
USHORT s_nAllocCounter;
} // namespace

template <class T> static inline T& _ref(T& v) { return v; }

static INT _checkCdContents();
static void _loadMovieInfo();
// INCLUDE_ASM'd below (local in the original; declared extern so the calls resolve to the asm labels)
void _startCdRead(INT nStartSect, INT nSectCnt);
INT _analyzeCdMemberHeader(BYTE*& pExtBuff);
void _addRsrcFileToDB(UINT nGroup, INT nType, BYTE*& pExtBuff);

/* Placeholder: the C of initialize__6CRcfCD (not matching yet); its strings and vtable uses keep the data in order. */
__attribute__((section(".sdata"))) static void _initialize_c() {
    INT nLocale;
    SectorInfoEntry* p;
    USHORT i;

    s_nGroupCount = 0;
#line 282
    s_pCdMemberDataList = new (com_malloc_d(sizeof(CDMemberList), __FILE__ "(282)", RET_ADDR())) CDMemberList;
    s_pCdMember = NULL;
    while ((nLocale = _checkCdContents()) == -1) DelayThread(1000);
    CRcf::setLocale(nLocale);
    while (mcdRead(0x244, 1, s_pCdBuff)) DelayThread(1000);
    s_SectBlockHeader = *(SectBlockHeader*)s_pCdBuff;
#line 305
    s_pSectInfoBlock = (SectorInfoEntry*)com_memalign_d(64, s_SectBlockHeader._nIndexSectCnt << 11, __FILE__ "(305)", (UINT)-1);
    while (mcdRead(0x245, s_SectBlockHeader._nIndexSectCnt, s_pSectInfoBlock)) DelayThread(1000);
    p = s_pSectInfoBlock;
    for (i = 0; i < s_SectBlockHeader._nSectInfoCnt; i++, p++) {
        if (!p->setup((UINT)s_pSectInfoBlock)) continue;
        if (p->_szName[0] != 'g') continue;
        USHORT nGroup = atoi(&p->_szName[1]);
        s_nGroupNumTbl[s_nGroupCount] = nGroup;
        s_pGroupDataInfoTbl[s_nGroupCount] = p;
        s_nGroupCount++;
        CD_GROUP_SECTOR* pMember = (CD_GROUP_SECTOR*)p->_nAddress;
        for (UINT m = 0; m < p->_nEntryCount; m++, pMember++) {
            pMember->setup(nGroup, p->_nStartSect);
        }
    }
    CRcfCD::setupRcfAlias();
    _loadMovieInfo();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/RcfCD", initialize__6CRcfCD);

void CRcfCD::finalize() {
    if (s_pSectInfoBlock) com_free(s_pSectInfoBlock);
    s_pSectInfoBlock = NULL;
    s_nGroupCount = 0;
    if (s_pCdMemberDataList) {
        SJ_FOREACH(CD_MEMBER_DATA, __p, *s_pCdMemberDataList) {
            delete __p;
        }
        _ref(s_pCdMemberDataList)->resetParam();
        delete _ref(s_pCdMemberDataList);
        s_pCdMemberDataList = NULL;
    }
}

INT CRcfCD::setupRcfAlias() {
    RcrsExtData* p;

    for (p = s_RcrsExtTbl; strncmp(p->_szExt, "END", 3); p++) {
        CRsrcDB::addRsrcType(p->_szExt, p->_nID);
    }
    return TRUE;
}

static void _copyBuf(BYTE* pDst, const BYTE* pSrc, size_t nSize) {
    INT* pDstI;
    const INT* pSrcI;
    size_t nSizeI;

#line 431
    ASSERT(pDst && pSrc && (0 == ((INT)pDst & 3)) && (0 == ((INT)pSrc & 3)));
    pDstI = (INT*)pDst;
    pSrcI = (const INT*)pSrc;
    nSizeI = nSize >> 2;
    for (size_t i = 0; i < nSizeI; i++) *pDstI++ = *pSrcI++;
    nSize &= 3;
    if (nSize) {
        pDst = (BYTE*)pDstI;
        pSrc = (const BYTE*)pSrcI;
        for (size_t i = 0; i < nSize; i++) *pDst++ = *pSrc++;
    }
}

static INT _checkCdContents() {
    return 0;
}

static void _initCdRead(BYTE* pDecodedBuf) {
    s_pDecodedBuf = pDecodedBuf;
    s_nStartSector_R = 0;
    s_nSectCnt_R = 0;
    s_pDecodedBuf_R = NULL;
}

/* Placeholder: the C of _startCdRead__Fii (not matching yet); its strings and vtable uses keep the data in order. */
__attribute__((section(".sdata"))) static void _startCdRead_c(INT nStartSect, INT nSectCnt) {
    s_nReadResult = 0xFFFF;
    while (mcdAsyncRead2(nStartSect, nSectCnt, s_pCdBuff) != 0) {
        Thread::delayThread_1Frame();
    }
    s_nStartSector_R = nStartSect;
    s_nSectCnt_R = nSectCnt;
    s_pDecodedBuf_R = s_pDecodedBuf;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/RcfCD", _startCdRead__Fii);

static void _retryCdRead() {
    s_pDecodedBuf = s_pDecodedBuf_R;
    _startCdRead(s_nStartSector_R, s_nSectCnt_R);
}

static BOOL _syncCdRead() {
    BOOL bEnd = FALSE;
    BYTE* pSrc;
    INT i;
    INT ret;

    _ref(s_nReadResult) = mcdSync(1);
    switch (*(volatile INT*)&s_nReadResult) {
    case 0:
        bEnd = TRUE;
        break;
    case 1:
        break;
    case -3:
    case -2:
        CFileIO::setTrayOpened();
    default:
        _retryCdRead();
        break;
    }
    if (bEnd && s_pDecodedBuf) {
        pSrc = s_pCdBuff;
        if (s_nNoCompression) {
            _copyBuf(s_pDecodedBuf, pSrc, s_nNoCompression);
            return bEnd;
        }
        for (i = s_nSectCnt_R; i > 0; i -= 2) {
            ret = DIntr();
            s_pDecodedBuf = ExpandData(pSrc, s_pDecodedBuf);
            if (ret) EIntr();
            pSrc += 0x1000;
        }
    }
    return bEnd;
}

static CD_GROUP_SECTOR* _getCdGroupData(ULONG nID) {
    USHORT nGroup = nID / 10000;
    INT nGrpIdx = -1;
    SectorInfoEntry* pSectInfo;
    CD_GROUP_SECTOR* pMember;

    for (INT i = 0; i < s_nGroupCount; i++) {
        if (nGroup == s_nGroupNumTbl[i]) {
            nGrpIdx = i;
            break;
        }
    }
    if (nGrpIdx < 0) return NULL;
    pSectInfo = s_pGroupDataInfoTbl[nGrpIdx];
    if (!pSectInfo) return NULL;
    pMember = (CD_GROUP_SECTOR*)pSectInfo->_nAddress;
    for (UINT i = 0; i < pSectInfo->_nEntryCount; i++, pMember++) {
        if (pMember->_nRCF == nID) return pMember;
    }
    return NULL;
}

INT CRcfCD::getMemberOption(UINT nGroup) {
    CD_GROUP_SECTOR* pData = _getCdGroupData(nGroup);
    return pData ? pData->_nOption : -1;
}

UINT CRcfCD::getExtInfo(UINT nGroup, INT nIdx) {
    CD_GROUP_SECTOR* pData = _getCdGroupData(nGroup);
    return pData ? pData->_nExtInfo[nIdx] : 0;
}

INT CRcfCD::getRsrcFileSize(UINT nID) {
    CD_GROUP_SECTOR* pGrp = _getCdGroupData(nID);
    return pGrp ? pGrp->_nSectSize << 11 : 0;
}

INT CRcfCD::getRsrcFileSizeOnMemory(UINT nID) {
    return 0;
}

static void _allocBuf(CD_MEMBER_DATA& info, INT nAlignSize, BYTE*& pExtBuff) {
    BYTE* p = NULL;
    char szTmp[256];

    s_nAllocCounter = 0;
    while (!p) {
        if (!info._pBuff) {
#line 658
            info._pBuff = new (com_malloc_d(sizeof(CBuf), __FILE__ "(658)", RET_ADDR())) CBuf;
            if (!info._pBuff) continue;
        }
        if (pExtBuff) {
            pExtBuff = (BYTE*)(((UINT)pExtBuff + (nAlignSize - 1)) & ~(nAlignSize - 1));
            p = pExtBuff;
            info._pBuff->attachBuf(p, -1);
            pExtBuff += info._nSize;
        } else {
            switch (nAlignSize) {
            case 128:
                p = info._pBuff->allocBuf_A128(info._nSize);
                break;
            case 256:
                p = info._pBuff->allocBuf_A256(info._nSize);
                break;
            default:
                p = info._pBuff->allocBuf_A64(info._nSize);
                break;
            }
            if (!p) {
                if (++s_nAllocCounter > 120) {
                    sprintf(szTmp, "\x83\x81\x83\x82\x83\x8a\x8am\x95\xdb\x8e\xb8\x94s: %s -> SIZE:%x\n", info._szDataName, info._nSize);
#line 691
                    AssertApp(__FILE__, __LINE__, "0", szTmp);
                }
                Thread::delayThread_1Frame();
            }
        }
    }
}

static void _addStreamInfoToDB(const CD_MEMBER_HED& hed, UINT nSectOffset);

/* Placeholder: the C of _analyzeCdMemberHeader__FRPUc (not matching yet); its strings and vtable uses keep the data in order. */
__attribute__((section(".sdata"))) static INT _analyzeCdMemberHeader_c(BYTE*& pExtBuff) {
    CD_MEMBER_HED* pCdHed = (CD_MEMBER_HED*)s_pCdBuff;
    CD_MEMBER_HED* pCdHedNext;
    INT nDataCount = 0;
    INT nAlignSize;
    BOOL bStreamSkip;
    INT nLocale;

    while (pCdHed->_szDataName[0]) {
        nAlignSize = CRsrcDB::getRsrcAlignmentSize(pCdHed->_nType);
        bStreamSkip = FALSE;
        if (pCdHed->_nType == 0x35 || pCdHed->_nType == 0x3D) bStreamSkip = TRUE;
        nLocale = CRcf::getLocale();
        if (nLocale == 0) {
            pCdHedNext = NULL;
        } else if (nLocale != pCdHed->_nLocale) {
            CD_MEMBER_HED* pCdHedBase = NULL;
            CD_MEMBER_HED* pCdHedOK;
            if (pCdHed->_nLocale == 0 || (CMainApp::isPAL() && pCdHed->_nLocale == 2)) pCdHedBase = pCdHed;
            pCdHedOK = NULL;
            while (!strncasecmp(pCdHed[1]._szDataName, pCdHed->_szDataName, 23)) {
                pCdHed++;
                if (nLocale == pCdHed->_nLocale) pCdHedOK = pCdHed;
            }
            pCdHedNext = pCdHed + 1;
            if (pCdHedOK) {
                pCdHed = pCdHedOK;
            } else if (pCdHedBase) {
                pCdHed = pCdHedBase;
            } else {
                pCdHed = pCdHedNext;
                continue;
            }
        } else {
            pCdHedNext = pCdHed + 1;
            while (!strncasecmp(pCdHedNext->_szDataName, pCdHed->_szDataName, 23)) pCdHedNext++;
        }
        if (bStreamSkip) {
            _addStreamInfoToDB(*pCdHed, s_pCurrentGrp->_nStart);
            pCdHed++;
        } else {
            CRsrcData* pData = CRsrcDB::getRsrc(pCdHed->_nType, pCdHed->_szDataName, NAME_CASE_0);
            if (pData) {
                INT nGroupID = pCdHed->_nGroupID;
                if (pCdHed->_nLinkID) nGroupID = _ref(s_nCurrentGrp);
                pCdHed++;
                CRsrcDB::addRsrcTag(*pData, nGroupID, CRcf::isSceneManagedMode());
            } else {
#line 794
                CD_MEMBER_DATA* pInfo = new (com_malloc_d(sizeof(CD_MEMBER_DATA), __FILE__ "(794)", RET_ADDR())) CD_MEMBER_DATA;
                ASSERT(pInfo);
                pInfo->setData(*pCdHed, s_pCurrentGrp->_nStart);
                if (!pInfo->_nGroupID) pInfo->_nGroupID = s_nCurrentGrp;
                _allocBuf(*pInfo, nAlignSize, pExtBuff);
                s_pCdMemberDataList->pushBack(*pInfo);
                nDataCount++;
                pCdHed++;
            }
        }
        if (pCdHedNext) pCdHed = pCdHedNext;
    }
    return nDataCount;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/RcfCD", _analyzeCdMemberHeader__FRPUc);

static void _startLoadMemberData() {
    INT nSect;

    _ref(s_nStartSector) = s_pCdMember->_nStart;
    _ref(s_nEndSectorCnt) = s_nStartSector + s_pCdMember->_nSectSize;
    _ref(s_nCdReadLoop) = s_pCdMember->_nSectSize >> 4;
    _ref(s_nCdReadOdd) = s_pCdMember->_nSectSize & 0xF;
    _initCdRead(s_pCdMember->_pBuff->getBufPtr());
    if (s_pCdMember->_nCompress) {
        s_nNoCompression = 0;
    } else {
        s_nNoCompression = s_pCdMember->_nSize;
    }
    nSect = 16;
    if (!s_nCdReadLoop) {
        nSect = s_nCdReadOdd;
        s_nCdReadOdd = 0;
    }
    _startCdRead(s_nStartSector, nSect);
}

static INT _loadCdMember() {
    INT result = 0;
    INT nSectSize;

    if (_syncCdRead()) {
        if (s_nCdReadLoop) s_nCdReadLoop--;
        s_nStartSector += 16;
        nSectSize = 0;
        if (!s_nCdReadLoop) {
            if (!s_nCdReadOdd) {
                s_pCdMember = s_pCdMember->getNext();
                result = s_pCdMember ? 1 : 0xFF;
            } else {
                nSectSize = s_nCdReadOdd;
                s_nCdReadOdd = 0;
            }
        } else {
            nSectSize = 16;
        }
        if (nSectSize) _startCdRead(s_nStartSector, nSectSize);
    }
    return result;
}


void CRcfCD::initReadThreadProc() {
    s_nModeFlag = 0;
    s_nReadStep = 0;
}

INT CRcfCD::procReadThread(UINT nGroup, INT nType, BYTE*& pExtBuff) {
    INT nResult = 0;
    INT ret;

    switch (s_nReadStep) {
    case 0:
        if (!(s_pCurrentGrp = _getCdGroupData(nGroup))) {
            nResult = 2;
            break;
        }
        _initCdRead(NULL);
        _startCdRead(s_pCurrentGrp->_nStart, s_pCurrentGrp->_nHedSize);
        s_nCurrentGrp = nGroup;
        if (s_pCurrentGrp->_nReverb) CSceneInfo::setReverbVal(s_pCurrentGrp->_nReverb);
        if (s_pCurrentGrp->_nSeRange) CSceneInfo::setSeRangeRatio(s_pCurrentGrp->_nSeRange);
        s_nReadStep = 1;
        break;
    case 1:
        if (!_syncCdRead()) break;
        if (!_analyzeCdMemberHeader(pExtBuff)) {
            nResult = 2;
            break;
        }
        _ref(s_nReadStep) = 2;
        s_pCdMember = s_pCdMemberDataList->getHead();
        _startLoadMemberData();
        break;
    case 2:
        ret = _loadCdMember();
        if (ret == 0xFF) {
            s_nReadStep = 3;
        } else if (ret) {
            _startLoadMemberData();
        }
        break;
    case 3:
        _addRsrcFileToDB(nGroup, nType, pExtBuff);
        nResult = 2;
        break;
    default:
        nResult = 2;
        break;
    }
    return nResult;
}

INT CRcfCD::cancelReadThread() {
    INT nResult = 0;

    switch (s_nReadStep) {
    case 2:
        if (!_syncCdRead()) break;
    case 3:
        SJ_FOREACH(CD_MEMBER_DATA, __p, *s_pCdMemberDataList) {
            delete __p;
        }
        s_pCdMemberDataList->resetParam();
        s_pCdMember = NULL;
    default:
        nResult = 2;
        break;
    }
    return nResult;
}

/* Placeholder: the C of _addRsrcFileToDB__FUiiRPUc (not matching yet); its strings and vtable uses keep the data in order. */
__attribute__((section(".sdata"))) static void _addRsrcFileToDB_c(UINT nGroup, INT nType, BYTE*& pExtBuff) {
    BOOL bDeletable = pExtBuff == NULL;
    CBuf buf;
    char szName[16];

    SJ_FOREACH(CD_MEMBER_DATA, pInfo, *s_pCdMemberDataList) {
        if (pInfo->_nGroupID != s_nCurrentGrp) {
            sprintf(szName, "%07d%07d", pInfo->_nLinkID, pInfo->_nGroupID);
            RsrcInfo info;
            info._pszName = szName;
            info._nGroupID = pInfo->_nGroupID;
            info._nRefGroupID = s_nCurrentGrp;
            info._nRsrcType = 0x27;
            info._bSceneManaged = CRcf::isSceneManagedMode();
            CRsrcDB::addRsrc(info, buf, NULL);
        }
        if (s_nModeFlag == 1 && CMainApp::isPAL() && pInfo->_nType == 0xF) pInfo->_nType = 0xC;
        RsrcInfo info;
        info._pszName = pInfo->_szDataName;
        info._nGroupID = pInfo->_nGroupID;
        info._nRefGroupID = s_nCurrentGrp;
        info._nRsrcType = pInfo->_nType;
        info._bSceneManaged = CRcf::isSceneManagedMode();
        INT nResult = CRsrcDB::addRsrc_NoCheck(info, *pInfo->_pBuff, NULL);
        if (nResult >= 0) {
            if (nResult & 2) CVramMan::waitTransTexDone(GetThreadId());
            if ((nResult & 4) && bDeletable) pInfo->_pBuff->releaseBuf(FALSE);
        }
        delete pInfo->_pBuff;
        pInfo->_pBuff = NULL;
    }
    SJ_FOREACH(CD_MEMBER_DATA, __p, *s_pCdMemberDataList) {
        delete __p;
    }
    s_pCdMemberDataList->resetParam();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/RcfCD", _addRsrcFileToDB__FUiiRPUc);

static void _addStreamInfoToDB(const CD_MEMBER_HED& hed, UINT nSectOffset) {
    CRsrcDB::addStreamInfo(hed._szDataName, hed._nGroupID, hed._nPosition + nSectOffset, hed._nSize, hed._nType);
}

static void _loadMovieInfo() {
    SectorInfoEntry* pSectInfo = s_pSectInfoBlock;

    if (!pSectInfo) return;
    for (USHORT i = 0; i < s_SectBlockHeader._nSectInfoCnt; i++, pSectInfo++) {
        if (strstr(pSectInfo->_szName, ".pss")) CMovieFiles::addFileInfo(i);
    }
}

BOOL CRcfCD::getSectorInfo(UINT nIdx, SectorInfo& outInfo) {
    SectorInfoEntry* pInfo;

    if (nIdx < getSectorInfoCount() && s_pSectInfoBlock) {
        pInfo = &s_pSectInfoBlock[nIdx];
        outInfo._pszFileName = pInfo->_szName;
        outInfo._nStartSector = pInfo->_nStartSect;
        outInfo._nEndSector = pInfo->_nStartSect + pInfo->_nSectCnt;
        outInfo._nSectorLength = pInfo->_nSectCnt;
        return TRUE;
    }
    return FALSE;
}

BOOL CRcfCD::getSectorInfo(const CHAR* pszName, SectorInfo& outInfo, UINT nNameLen) {
    UINT nCnt;
    BOOL bMatch;
    UINT i;
    SectorInfoEntry* pInfo;

    if (!s_pSectInfoBlock) return FALSE;
    nCnt = getSectorInfoCount();
    for (i = 0; i < nCnt; i++) {
        pInfo = &s_pSectInfoBlock[i];
        if (nNameLen) {
            bMatch = !strncasecmp(pszName, pInfo->_szName, nNameLen);
        } else {
            bMatch = !strcasecmp(pszName, pInfo->_szName);
        }
        if (bMatch) {
            outInfo._pszFileName = pInfo->_szName;
            outInfo._nStartSector = pInfo->_nStartSect;
            outInfo._nEndSector = pInfo->_nStartSect + pInfo->_nSectCnt;
            outInfo._nSectorLength = pInfo->_nSectCnt;
            return TRUE;
        }
    }
    return FALSE;
}

UINT CRcfCD::getSectorInfoCount() {
    return s_SectBlockHeader._nSectInfoCnt;
}
// SOURCE DATA: .data .rodata .bss
