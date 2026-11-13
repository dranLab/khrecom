
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#define RSRCDATA_H_PATH "c:/KHCOM/MainApp/System/RsrcData.h" /* MATCH: RsrcData.h strings in .rodata use the full path */
#include "System/RsrcData.h"
#include "System/Buf.h"
#include "System/Debug.h"
#include "System/Rcf.h"
#include "include_asm.h"

extern "C" const char _ctype_[];
#define isupper(c) ((_ctype_ + 1)[(int)(c)] & 1)
#define isdigit(c) ((_ctype_ + 1)[(int)(c)] & 4)


#define ASSERT(e) KH_DEBUG(if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0))

struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
};

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
int CreateSema(SemaParam* param);
int DeleteSema(int sema_id);
int SignalSema(int sema_id);
int WaitSema(int sema_id);
int CreateThread(ThreadParam* param);
int DeleteThread(int thread_id);
int StartThread(int thread_id, void* arg);
int TerminateThread(int thread_id);
int GetThreadId();
int ChangeThreadPriority(int thread_id, int priority);
int sprintf(char* s, const char* fmt, ...);
char* strrchr(const char* s, int c);
char* strcpy(char* d, const char* s);
int strncmp(const char* a, const char* b, unsigned int n);
int atoi(const char* s);
unsigned int strlen(const char* s);
char* strncpy(char* d, const char* s, unsigned int n);
char* strcat(char* d, const char* s);
int SleepThread();
int WakeupThread(int thread_id);
int CancelWakeupThread(int thread_id);
}

extern char _gp;

#include "System/VramMan.h"





namespace Thread {
void delayThread_1Frame();
}

#include "System/Movie/MovieFiles.h"



















class CRsrcData;

#include "System/RsrcDB.h"








#include "System/FileIO.h"

#include "Game/SceneInfo.h"

#include "System/RcfCD.h"

#include "Game/NowLoading.h"
#include "System/HeapMan.h"

class CRCFQueueData {
private:
    volatile INT m_nStep; /* MATCH: volatile (thread-shared step; the original reloads it after every store) */
    SHORT m_nGroupID;
    SHORT m_nMemeberID;
    INT m_nOwnerID;
    INT m_nType;
    BYTE* m_pBuff;
    CRCFQueueData* m_pNext;

public:
    CRCFQueueData(INT nGroupID, INT nMemeberID, INT nType, BYTE* pBuff, INT nLinkOwner) {
        m_nStep = 0;
        m_pNext = __null;
        init(nGroupID, nMemeberID, nType, pBuff, nLinkOwner);
    }
    void init(INT nGroupID, INT nMemeberID, INT nType, BYTE* pBuff, INT nLinkOwner);
    void linkTail();
    void linkFront();
    INT getID() { return m_nGroupID * 10000 + m_nMemeberID; }
    INT getGroupID() { return m_nGroupID; }
    INT getMemberID() { return m_nMemeberID; }
    BOOL hasLinkOwner() { return m_nOwnerID != 0; }
    INT getLinkOwnerID() { return m_nOwnerID; }
    INT getType() { return *(INT*)&m_nType; } /* MATCH: not an in-struct access: the original reloads it after global stores */
    BYTE* getBuff() { return m_pBuff; }
    CRCFQueueData* getNextData() { return m_pNext; }
    void setNextData(CRCFQueueData* p) { m_pNext = p; }
    BOOL isNOP() { return m_nStep == 0; }
    void setExec() { m_nStep = 1; }
    void setEnd() { m_nStep = 2; }
    void setCancel() { m_nStep = 3; }
    BOOL isCANCEL() { return *(INT*)&m_nStep == 3; } /* MATCH: not volatile here: reorg steals the load for the loop's bnel slot */
};

/* PARTIAL: CRCFAliasData lacks the ref's init, linkTail, setSrc, setDst, setID (parameters unknown, no users here) */
class CRCFAliasData {
private:
    char m_szSrc[33];
    char m_szDst[129];
    INT m_nSrcLen;
    INT m_nID;
    CRCFAliasData* m_pNext;

public:
    CHAR* getSrc() { return m_szSrc; }
    CHAR* getDst() { return m_szDst; }
    INT getID() { return m_nID; }
    CRCFAliasData* getNextData() { return m_pNext; }
    BOOL isMatchSrc(const CHAR* pszLabel) { return !strncmp(pszLabel, m_szSrc, m_nSrcLen); }
};

void _loadDbgRcfSceneInfo();
void _disposeRCF();
void _clearAliasList();
void _clearQueue();

namespace {
unsigned char s_cReadThreadStack[8192];
KH_DEBUG(RcfSceneInfo* s_pRcfSceneArray;)
KH_DEBUG(RcfMapInfo* s_pRcfMapArray;)
KH_DEBUG(SHORT s_nRcfSceneCount;)
KH_DEBUG(SHORT s_nRcfMapCount;)
BOOL s_bSceneManagedMode;
BYTE* s_pExtBuf;
INT s_nLocale;
BYTE s_nModeFlag;
BYTE s_nParentReverb;
BYTE s_nChildReverb;
BYTE __dummy;
INT s_nRsrcFileSize;
volatile INT s_nReadStep;
volatile INT s_nReadThreadInfo;
volatile BOOL s_bThreadEnable;
volatile ULONG s_nReadRsrcID; /* MATCH: volatile (thread-shared; the original never moves its store into a delay slot) */
volatile INT s_nReadGroup; /* MATCH: volatile (thread-shared; _parseRCF_EntryFile loads it before the CBuf ctor stores) */
volatile INT s_nReadFileSize;
volatile INT s_nLinkOwnerGroup; /* volatile: _parseRCF_EntryFile reloads it in a bnezl slot (thread-shared, like s_nReadStep) */
INT s_nFilePtr;
INT s_nReadThreadID;
INT s_RcfSemaID;
CBuf* s_pFileBuf;
CRCFQueueData* s_pReadQueue;
CRCFAliasData* s_pAliasList;
CBuf* s_pDbgFileBuf;
INT s_nDbgFilePtr;
INT s_nDbgReadStep;
} // namespace

template <class T> static INT _seekRcfSceneInfo(SHORT nGroup, SHORT nMember, T* pArray, SHORT nArraySize) {
    INT nTarget = (nGroup << 16) | nMember;
    INT nIdx = -1;

    if (pArray && nArraySize > 0) {
        INT s = 0;
        INT e = nArraySize;
        while (s < e) {
            INT m = (s + e) >> 1;
            INT k = (pArray[m]._nGroup << 16) | pArray[m]._nMember;
            if (nTarget < k) {
                e = m;
            } else if (k < nTarget) {
                s = m + 1;
            } else {
                nIdx = m;
                break;
            }
        }
    }
    return nIdx;
}

inline void CRCFQueueData::linkTail() {
    if (s_pReadQueue) {
        CRCFQueueData* p = s_pReadQueue;
        while (p->m_pNext) {
            p = p->m_pNext;
        }
        p->m_pNext = this;
    } else {
        s_pReadQueue = this;
    }
}

inline void CRCFQueueData::linkFront() {
    if (s_pReadQueue) {
        CRCFQueueData* pBackData = s_pReadQueue;
        if (pBackData->isNOP()) {
            CRCFQueueData* p;
            *(CRCFQueueData**)&s_pReadQueue = this; /* MATCH: not a fixed scalar MEM: stored before p's copy, and
                                                        p = this gets its own register (pBackData stays in v1) */
            p = this;
            while (p->m_pNext) {
                p = p->m_pNext;
            }
            p->m_pNext = pBackData;
        } else {
            CRCFQueueData* pNext = pBackData->m_pNext;
            pBackData->m_pNext = this;
            m_pNext = pNext;
        }
    } else {
        s_pReadQueue = this;
    }
}


void CRcf::initialize() {
    SemaParam sp;

    s_nReadStep = 0;
    s_nReadThreadInfo = 0;
    s_pReadQueue = __null;
    s_bThreadEnable = TRUE;
    s_pFileBuf = __null;
    s_nParentReverb = 0;
    s_nChildReverb = 0;
    s_pAliasList = __null;
    sp.initCount = 1;
    sp.maxCount = 1;
    s_nReadThreadID = -1;
    s_RcfSemaID = CreateSema(&sp);
    CMovieFiles::initialize();
    CRcfCD::initialize();
    s_pDbgFileBuf = __null;
    KH_DEBUG(s_pRcfSceneArray = __null;)
    KH_DEBUG(s_pRcfMapArray = __null;)
    KH_DEBUG(s_nRcfSceneCount = 0;)
    KH_DEBUG(s_nRcfMapCount = 0;)
    CNowLoading::enable(TRUE);
    CNowLoading::setMode(0);
    _loadRCFAlias();
    KH_DEBUG(_loadDbgRcfSceneInfo();)
}
BOOL _hasReadQueue();
void _setReadThreadEnd();
INT _loadRCF(ULONG nInGroup);
INT _atoi(const CHAR* lpszInStr);
INT _parseFilePath(const CHAR* pData, INT nRest, CHAR* pszOutPath, INT nType);
void _makeDataName(const CHAR* lpszInPath, CHAR* lpszOutName);
void _parseRCF_FirstEntryFile(CHAR* szFilePath, CHAR* szName, INT nGroupID, INT nRsrcID);
CHAR* _getRCFAlias(const CHAR* pInData, CHAR* szOut, INT* pOutID);
void _makeLocalizePath(CHAR* pszOut, const CHAR* pszSrc, INT nLocale);
void _countRcfSceneAndMap();
BOOL _isCompleteLoadRCF(INT nGroup, INT nMember);
INT _LoadRCFProc(ULONG nGroup, INT nType);
INT _parseFileType(const CHAR* pInData);
INT _atolen(const CHAR* lpszInStr);
void _parseRcfSceneAndMapInfo();
BOOL _loadRcfInfoFile(const CHAR* pszFile);
void _disposeRcfInfoFile();
BOOL isTokenSCENE(const CHAR* p);
BOOL isTokenMAP(const CHAR* p);
BOOL isTokenEND(const CHAR* p);
void _getStageInfo(SHORT& nOutGroup, SHORT& nOutMember, CHAR* pOutLabel, INT* pOutAtelCnt);
CHAR* _getDbgParsePtr();
void _skipDbgLine();
BOOL _isStr_CR(const CHAR* p);
INT _parseRCF_Tag(INT nInType);
INT _parseRCF_EntryFile(ULONG nGroup, INT nType);
void _setLinkreqRead();

BOOL _isReadThreadRunning() {
    return s_nReadThreadInfo > 0;
}
BOOL _isReadThreadEndInterval() {
    return s_nReadThreadInfo < 0;
}
BOOL _isReadThreadSleeping() {
    return s_nReadThreadInfo == 0;
}
void _setReadThreadRunning() {
    s_nReadThreadInfo = 2;
}
BOOL _doReadThreadEndInterval() {
    s_nReadThreadInfo++;
    return s_nReadThreadInfo == 0;
}
/* Parked: 0 or 6 of 153 lines (2026-11-07), random per compile (matched 4 of 6 rebuilds): s_nRcfMapCount/s_nRcfSceneCount's
 * hoisted %hi pseudos tie in global-alloc priority (3 refs / 120 insns each), the tie goes to the lower pseudo, and PRE numbers
 * them in expression-hash bucket order; the SYMBOL_REF hash is over the name, whose anonymous-namespace suffix is random per
 * compile (src_System_Rcf.cpp5vVlaa, ...PaOlaa). Needs a deterministic namespace name in the build, not source. The map delete[]
 * loop's bound reload is fixed (cast pointer); tried: the two count stores swapped (wrong store order, still random).
 * 2026-11-07, the suffix: cc1plus's value from its PID and the time (GetCurrentProcessId + GetSystemTimeAsFileTime); a PID
 * namespace fixes the PID, a .ii with rewritten line markers gives the original c__KHCOM_MainApp_System_Rcf.cpp prefix, but
 * wibo 1.2.0 can't fix the time: the original name (...cppA0aaaa) needs a patched wibo. Parked by the user's decision. */
#if 0
void CRcf::finalize() {
    CMovieFiles::finalize();
    s_bThreadEnable = FALSE;
    while (!_isReadThreadSleeping() && _isReadThreadRunning()) {
        Thread::delayThread_1Frame();
    }
    TerminateThread(s_nReadThreadID);
    DeleteThread(s_nReadThreadID);
    s_nReadThreadID = -1;
    _disposeRCF();
    _clearAliasList();
    _clearQueue();
    if (s_pRcfSceneArray) delete[] s_pRcfSceneArray;
    s_pRcfSceneArray = __null;
    if (s_pRcfMapArray) delete[] *(RcfMapInfo**)&s_pRcfMapArray; /* MATCH: the loop reloads the bound after the inline dtor's vptr store (not a fixed scalar) */
    s_pRcfMapArray = __null;
    s_nRcfSceneCount = 0;
    s_nRcfMapCount = 0;
    if (s_pDbgFileBuf) delete s_pDbgFileBuf;
    s_pDbgFileBuf = __null;
    DeleteSema(s_RcfSemaID);
    CRcfCD::finalize();
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Rcf", finalize__4CRcf);
#endif
BOOL CRcf::isTerminated() {
    return !s_bThreadEnable;
}
void _waitsema() {
    WaitSema(s_RcfSemaID);
}
void _signalsema() {
    SignalSema(s_RcfSemaID);
}
void _clearQueue() {
    CRCFQueueData* pData;

    _waitsema();
    pData = s_pReadQueue;
    while (pData) {
        CRCFQueueData* p = pData;
        pData = pData->getNextData();
        delete p;
    }
    s_pReadQueue = __null;
    _signalsema();
    KH_DEBUG(if (CNowLoading::getMode() == 1) {)
        KH_DEBUG(CNowLoading::end(FALSE);)
    KH_DEBUG(})
}
BOOL _hasReadQueue() {
    return s_pReadQueue != __null;
}
void _clearAliasList() {
    CRCFAliasData* pData = s_pAliasList;
    while (pData) {
        CRCFAliasData* p = pData;
        pData = pData->getNextData();
        delete p;
    }
    s_pAliasList = __null;
}
void CRcf::reqLoadRCF(INT nGroup, INT nMember, INT nType, BYTE* pBuff, INT nLinkOwner) {
    CRCFQueueData* pPrev;
    CRCFQueueData* pData;

    if (isTerminated()) {
        return;
    }
    KH_DEBUG(pPrev = s_pReadQueue;)
    _waitsema();
#line 527
    pData = new KH_AT("(527)") CRCFQueueData(nGroup, nMember, nType, pBuff, nLinkOwner);
    ASSERT(pData);
    pData->linkTail();
    _signalsema();
    KH_DEBUG(if (CNowLoading::getMode() == 1 && !pPrev) {)
        KH_DEBUG(CNowLoading::start();)
    KH_DEBUG(})
}
void CRcf::reqLoadRCFFront(INT nGroup, INT nMember, INT nType, BYTE* pBuff, INT nLinkOwner) {
    CRCFQueueData* pPrev;
    CRCFQueueData* pData;

    if (isTerminated()) {
        return;
    }
    KH_DEBUG(pPrev = s_pReadQueue;)
    _waitsema();
#line 553
    pData = new KH_AT("(553)") CRCFQueueData(nGroup, nMember, nType, pBuff, nLinkOwner);
    ASSERT(pData);
    pData->linkFront();
    _signalsema();
    KH_DEBUG(if (CNowLoading::getMode() == 1 && !pPrev) {)
        KH_DEBUG(CNowLoading::start();)
    KH_DEBUG(})
}

inline void CRCFQueueData::init(INT nGroupID, INT nMemeberID, INT nType, BYTE* pBuff, INT nLinkOwner) {
    volatile CRCFQueueData* p = this;
    p->m_nGroupID = nGroupID;
    p->m_nMemeberID = nMemeberID;
    p->m_nOwnerID = nLinkOwner;
    p->m_pBuff = pBuff;
    p->m_nType = nType;
}
void CRcf::_threadReadProc(void* p) {
    volatile ULONG nReadID;
    CRCFQueueData* pReadEntry;
    volatile INT res;

    while (s_bThreadEnable) {
        s_nReadStep = 0;
        CRcfCD::initReadThreadProc();
        nReadID = s_nReadRsrcID;
        pReadEntry = s_pReadQueue;
#line 608
        ASSERT(pReadEntry);
        if (!(pReadEntry->getType() & 0x20)) {
            s_nLinkOwnerGroup = 0;
        }
        s_pExtBuf = (pReadEntry->getType() & 0x40) ? pReadEntry->getBuff() : __null;
        s_bSceneManagedMode = TRUE;
        if (pReadEntry->getType() & 0x100) {
            s_bSceneManagedMode = FALSE;
        }
        res = 0;
        while (s_bThreadEnable && res != 2) {
            if (pReadEntry->isCANCEL()) {
                res = CRcfCD::cancelReadThread();
            } else {
                res = CRcfCD::procReadThread(nReadID, pReadEntry->getType(), s_pExtBuf);
            }
        }
        _setReadThreadEnd();
        SleepThread();
    }
}
BOOL CRcf::_launchReadThread(ULONG nData, INT nType) {
    INT ra;
    ThreadParam tp;

    if (!s_bThreadEnable) {
        return FALSE;
    }
    __asm__ volatile("daddu %0, $31, $0" : "=r"(ra)); /* MATCH: inline asm, __builtin_return_address(0) reloads ra */
    if (!_isReadThreadSleeping()) {
        TRACE_COL(_TTY_PURPLE, "!!!!!!!!! ERROR READ THREAD Activation !!!!!!!!! %x\n", ra);
        return FALSE;
    }
    s_nReadRsrcID = nData;
    if (s_nReadThreadID < 0) {
        tp.entry = _threadReadProc;
        tp.stack = s_cReadThreadStack;
        tp.stackSize = sizeof(s_cReadThreadStack);
        tp.gpReg = &_gp;
        tp.initPriority = 8;
        s_nReadThreadID = CreateThread(&tp);
        if (StartThread(s_nReadThreadID, __null) < 0) {
            return FALSE;
        }
    } else {
        WakeupThread(s_nReadThreadID);
        CancelWakeupThread(s_nReadThreadID);
    }
    if (s_pReadQueue->hasLinkOwner()) {
        TRACE_COL(_TTY_PURPLE, "***** Start Load RCF #%05d (in #%05d) *****\n", nData, s_pReadQueue->getLinkOwnerID());
    } else {
        TRACE_COL(_TTY_PURPLE, "***** Start Load RCF #%05d *****\n", nData);
    }
    _setReadThreadRunning();
    return TRUE;
}
void _setReadThreadEnd() {
    if (_isReadThreadRunning()) {
        s_nReadThreadInfo = -s_nReadThreadInfo;
    }
    if (s_pReadQueue) {
        s_pReadQueue->setEnd();
    }
}
INT _setNextRcfQueue() {
    INT nRes = -1;
    if (s_pReadQueue) {
        CRCFQueueData* pNext = s_pReadQueue->getNextData();
        delete s_pReadQueue;
        s_pReadQueue = pNext;
        nRes = pNext != __null;
    }
    return nRes;
}
void _checkReadThreadEnd() {
    INT nRes;
    if (_isReadThreadEndInterval() && _doReadThreadEndInterval()) {
        _waitsema();
        nRes = _setNextRcfQueue();
        _signalsema();
        KH_DEBUG(if (CNowLoading::getMode() == 1 && !nRes) {)
            KH_DEBUG(CNowLoading::end(FALSE);)
        KH_DEBUG(})
    }
}
void CRcf::mainProc() {
    _checkReadThreadEnd();
    if (_isReadThreadSleeping() && s_pReadQueue && s_pReadQueue->isNOP()) {
        if (_launchReadThread(s_pReadQueue->getID(), s_pReadQueue->getType())) {
            s_pReadQueue->setExec();
        }
    }
}
INT CRcf::getMemberOption(ULONG nGroup) {
    return CRcfCD::getMemberOption(nGroup);
}
int CRcf::isSceneManagedMode() {
    return s_bSceneManagedMode;
}
void _setLinkreqRead() {
}
BOOL _isCompleteLoadRCF(INT nGroup, INT nMember) {
    BOOL bResult;
    INT nID;
    CRCFQueueData* pData;

    if ((nGroup | nMember) == 0) {
        bResult = FALSE;
        if (_isReadThreadSleeping() && CVramMan::isTransTexDone()) {
            bResult = !_hasReadQueue();
        }
    } else {
        nID = nGroup * 10000 + nMember;
        bResult = TRUE;
        for (pData = s_pReadQueue; pData; pData = pData->getNextData()) {
            if (pData->getGroupID() == nGroup && pData->getMemberID() == nMember) {
                bResult = FALSE;
                break;
            }
            if (pData->getLinkOwnerID() == nID) {
                bResult = FALSE;
                break;
            }
        }
    }
    return bResult;
}
BOOL CRcf::isCompleteLoadRCF(INT nGroup, INT nMember) {
    BOOL result;

    if (isTerminated()) {
        return TRUE;
    }
    _waitsema();
    result = _isCompleteLoadRCF(nGroup, nMember);
    _signalsema();
    return result;
}
INT _LoadRCFProc(ULONG nGroup, INT nType) {
    INT nResult;

    switch (s_nReadStep) {
    case 0:
        nResult = _loadRCF(nGroup);
        if (nResult == 1) {
            s_nFilePtr = 0;
            s_nReadGroup = nGroup;
            s_nReadStep = nResult;
        }
        break;
    case 1:
        nResult = _parseRCF_Tag(nType);
        break;
    case 2:
        nResult = _parseRCF_EntryFile(nGroup, nType);
        break;
    default:
        return 2;
    }
    if (s_nFilePtr >= s_nReadFileSize || nResult == 2) {
        _setLinkreqRead();
        s_nReadStep = 3;
        _disposeRCF();
        nResult = 2;
    }
    return nResult;
}
INT CRcf::_loadRCFAlias() {
    CRcfCD::setupRcfAlias();
    return TRUE;
}
INT _loadRCF(ULONG nInGroup) {
    char szFileName[256];

    if (s_pFileBuf) delete s_pFileBuf;
    s_pFileBuf = __null;
#line 1051
    s_pFileBuf = new KH_AT("(1051)") CBuf;
    sprintf(szFileName, "cdrom0:\\RCF\\G%03d.RCF", nInGroup / 10000);
    s_nReadFileSize = CFileIO::loadFile(szFileName, *s_pFileBuf, 0);
    if (s_nReadFileSize > -1) return 1;
    return -2;
}
void _disposeRCF() {
    if (s_pFileBuf) delete s_pFileBuf;
    s_pFileBuf = __null;
}
void CRcf::reqLoadScene(INT nArea, INT nScene) {
    reqLoadRCF(nArea + 100, nScene, 0, __null, 0);
}
void CRcf::reqLoadStage(INT nArea, INT nScene) {
    reqLoadScene(nArea, nScene);
    reqLoadStageMap(nArea, nScene);
}
void CRcf::reqLoadStageMap(INT nArea, INT nScene) {
    UINT nMapID = getMapID(nArea, nScene);
    if (nMapID) {
        reqLoadRCF(nMapID / 10000, nMapID % 10000, 0, __null, 0);
    }
}
void CRcf::reqLoadEnemy(INT nInEnemyID) {
}
BOOL CRcf::isCompleteLoadEnemy(INT nInEnemyID) {
    return TRUE;
}
void CRcf::reqLoadNpc(INT nInNpcID) {
    reqLoadRCF(10, nInNpcID, 0, __null, 0);
}
BOOL CRcf::isCompleteLoadNpc(INT nInNpcID) {
    return isCompleteLoadRCF(10, nInNpcID);
}
void CRcf::reqLoadPlayer(INT nID) {
    reqLoadRCF(1, nID, 0x100, __null, 0);
}
void CRcf::cancelLoadRCF(INT nGroup, INT nMember) {
    CRCFQueueData* pData;
    INT nID;
    CRCFQueueData* pPrev;

    if (isTerminated()) {
        return;
    }
    _waitsema();
    pData = s_pReadQueue;
    if (pData) {
        nID = nGroup * 10000 + nMember;
        pPrev = __null;
        if (!pData->isNOP()) {
            if (((pData->getGroupID() & 0x0FFFFFFF) == nGroup && pData->getMemberID() == nMember) || pData->getLinkOwnerID() == nID) {
                pData->setCancel();
                TRACE_COL(_TTY_RED, "CRcf::cancelLoadRCF - ABORT (%d, %d) in #%d\n", pData->getGroupID(),
                          pData->getMemberID(), pData->getLinkOwnerID());
            }
            pPrev = pData;
            pData = pData->getNextData();
        }
        while (pData) {
            CRCFQueueData* pNext = pData->getNextData();
            if (((pData->getGroupID() & 0x0FFFFFFF) == nGroup && pData->getMemberID() == nMember) || pData->getLinkOwnerID() == nID) {
                TRACE_COL(_TTY_RED, "CRcf::cancelLoadRCF - CANCEL (%d, %d) in #%d\n", pData->getGroupID(),
                          pData->getMemberID(), pData->getLinkOwnerID());
                delete pData;
                if (pPrev) {
                    pPrev->setNextData(__null);
                }
            } else {
                if (pPrev) {
                    pPrev->setNextData(pData);
                } else {
                    s_pReadQueue = pData;
                }
                pPrev = pData;
            }
            pData = pNext;
        }
        if (!pPrev) {
            s_pReadQueue = __null;
        }
    }
    _signalsema();
}
CHAR* _getParsePtr() {
    return (CHAR*)s_pFileBuf->getBufPtr() + s_nFilePtr;
}
INT _getParseRestSize() {
    return s_nReadFileSize - s_nFilePtr;
}
INT _skipLine(const CHAR* pInData, INT nInRest) {
    INT i;
    for (i = 0; nInRest > 0 && *pInData != '\n'; i++, nInRest--, pInData++) {
    }
    return i + 1;
}
void _skipLine() {
    s_nFilePtr += _skipLine(_getParsePtr(), _getParseRestSize());
}
BOOL _isStr_Blank(const CHAR* p) {
    return *p == ' ' || *p == '\t';
}
BOOL _isStr_BlankCR(const CHAR* p) {
    return _isStr_Blank(p) || *p == '\r' || *p == '\n';
}
INT _skipBlank(const CHAR* pInData, INT nInRest) {
    INT i;
    for (i = 0; nInRest > 0 && _isStr_Blank(pInData); i++, pInData++, nInRest--) {
    }
    return i;
}
void _skipBlank() {
    s_nFilePtr += _skipBlank(_getParsePtr(), _getParseRestSize());
}
BOOL _isStr_Comment(const CHAR* p) {
    return (p[0] == '/' && p[1] == '/') || p[0] == ';';
}
BOOL _isStr_Comment(INT nInPtr) {
    return _isStr_Comment((CHAR*)s_pFileBuf->getBufPtr() + s_nFilePtr);
}
BOOL _isStr_CR(const CHAR* p) {
    return p[0] == '\r' && p[1] == '\n';
}
BOOL _isStr_CR(INT nInPtr) {
    return _isStr_CR((CHAR*)s_pFileBuf->getBufPtr() + nInPtr);
}
BOOL _isStr_PAL(INT nInPtr) {
    CHAR* p = (CHAR*)s_pFileBuf->getBufPtr() + nInPtr;
    return p[0] == 'P' && p[1] == 'A' && p[2] == 'L';
}
BOOL _isStr_REVERB(INT nInPtr) {
    CHAR* p = (CHAR*)s_pFileBuf->getBufPtr() + nInPtr;
    return p[0] == 'R' && p[1] == 'e' && p[2] == 'V';
}
BOOL _isStr_SERANGE(INT nInPtr) {
    CHAR* p = (CHAR*)s_pFileBuf->getBufPtr() + nInPtr;
    return p[0] == 'S' && p[1] == 'E' && p[2] == 'R';
}
BOOL _isStr_END(INT nInPtr) {
    CHAR* p = (CHAR*)s_pFileBuf->getBufPtr() + nInPtr;
    return p[0] == '#' && p[1] == 'e' && p[2] == 'n' && p[3] == 'd';
}
INT _parseRCF_Tag(INT nInType) {
    INT nDataID;

    if (!_isStr_Comment(s_nFilePtr) && !_isStr_CR(s_nFilePtr) && *_getParsePtr() == '#') {
        nDataID = _atoi(_getParsePtr());
        INT nGroupNo = (ULONG)s_nReadGroup % 10000; /* MATCH: not in the ref's locals; the modulo goes before both stores */
        s_nModeFlag = 0;
        s_nFilePtr++;
        if (nDataID == nGroupNo) {
            while (*_getParsePtr() != '\n') {
                if (_isStr_PAL(s_nFilePtr)) {
                    s_nModeFlag = 1;
                    break;
                }
                if (_isStr_REVERB(s_nFilePtr)) {
                    s_nFilePtr += 7;
                    CSceneInfo::setReverbVal(_atoi(_getParsePtr()));
                }
                if (_isStr_SERANGE(s_nFilePtr)) {
                    s_nFilePtr += 8;
                    CSceneInfo::setSeRangeRatio(_atoi(_getParsePtr()) / 10);
                }
                s_nFilePtr++;
            }
            _skipLine();
            s_nReadStep = 2;
            return 0;
        }
        do {
            _skipLine();
        } while (!_isStr_END(s_nFilePtr));
    }
    _skipLine();
    return 0;
}
void _makeDataName(const CHAR* lpszInPath, CHAR* lpszOutName) {
    CHAR* p;
    CHAR* p2;

    if (lpszInPath && lpszOutName) {
        p = strrchr(lpszInPath, '/');
        p2 = strrchr(lpszInPath, '\\');
        if (p || p2) {
            if (p < p2) {
                p = p2;
            }
            strcpy(lpszOutName, p + 1);
        }
    }
}
void CRcf::initRsrcFileSize() {
    s_nRsrcFileSize = 0;
}
INT CRcf::getRsrcFileSize(ULONG nID) {
    return CRcfCD::getRsrcFileSize(nID);
}
INT CRcf::getCdStatus() {
    return 0;
}
INT _parseRCF_EntryFile(ULONG nGroup, INT nType) {
    char szFilePath[256];
    INT nRsrcID;
    INT nGroupID;
    CRsrcData* pData;

    if (_isStr_Comment(s_nFilePtr) || _isStr_CR(s_nFilePtr)) {
        _skipLine();
        return 0;
    }
    if (_isStr_END(s_nFilePtr)) {
        return 2;
    }
    nRsrcID = _parseFilePath(_getParsePtr(), _getParseRestSize(), szFilePath, nType);
    if (nRsrcID == 0x7F) {
        sprintf(szFilePath, "%07d%07d", s_nReadGroup, atoi(szFilePath));
        CBuf buf;
        RsrcInfo info;
        info._pszName = szFilePath;
        info._nGroupID = s_nReadGroup;
        info._nRefGroupID = s_nLinkOwnerGroup;
        info._nRsrcType = 0x27;
        info._bSceneManaged = s_bSceneManagedMode;
        CRsrcDB::addRsrc(info, buf, __null);
        _skipLine();
        return 0;
    }
    char szName[128];
    if (s_nModeFlag == 1 && CMainApp::isPAL() && nRsrcID == 0xF) {
        nRsrcID = 0xC;
    }
    _makeDataName(szFilePath, szName);
    nGroupID = s_nReadGroup;
    pData = CRsrcDB::getRsrc(nRsrcID, szName, NAME_CASE_SENSITIVE);
    if (!pData) {
        _parseRCF_FirstEntryFile(szFilePath, szName, nGroupID, nRsrcID);
    } else {
        if (s_nLinkOwnerGroup) {
            nGroupID = s_nLinkOwnerGroup;
        }
        CRsrcDB::addRsrcTag(*pData, nGroupID, s_bSceneManagedMode);
    }
    _skipLine();
    return 0;
}
void _parseRCF_FirstEntryFile(CHAR* szFilePath, CHAR* szName, INT nGroupID, INT nRsrcID) {
    CBuf buf;
    INT nFileSize;
    INT nAllocAlign;
    UINT align;
    BYTE* pBuf;

    nAllocAlign = 0;
    align = CRsrcDB::getRsrcAlignmentSize(nRsrcID);
    switch (align) {
    case 0x80:
        nAllocAlign = 1;
        break;
    case 0x100:
        nAllocAlign = 2;
        break;
    }
    if (s_pExtBuf) {
        s_pExtBuf = (BYTE*)(((UINT)s_pExtBuf + (align - 1)) & -align); // MATCH: (align - 1) grouped: the -1 is applied to align, not the sum (a0 keeps align)
        buf.attachBuf(s_pExtBuf, -1);
    }
    if (nRsrcID == 0x35 || nRsrcID == 0x3D) {
        nFileSize = strlen(szFilePath) + 1;
        pBuf = buf.getBufPtr();
        if (!pBuf) {
            pBuf = buf.allocBuf(nFileSize);
        }
#line 1534
        ASSERT(0 < nFileSize);
        strcpy((char*)pBuf, szFilePath);
    } else if (s_nLocale == 0) {
        nFileSize = CFileIO::loadFile(szFilePath, buf, nAllocAlign);
    } else {
        char szFile[255];
        INT hFile;
        _makeLocalizePath(szFile, szFilePath, s_nLocale);
        hFile = CFileIO::openFile(szFile, 1, 0);
        if (hFile < 0) {
            if (s_nLocale >= 3) {
                _makeLocalizePath(szFile, szFilePath, 2);
                hFile = CFileIO::openFile(szFile, 1, 0);
                if (hFile >= 0) {
                    CFileIO::closeFile(hFile);
                } else {
                    strcpy(szFile, szFilePath);
                }
            } else {
                strcpy(szFile, szFilePath);
            }
        } else {
            CFileIO::closeFile(hFile);
        }
        nFileSize = CFileIO::loadFile(szFile, buf, nAllocAlign);
    }
    if (nFileSize > 0) {
        RsrcInfo info;
        INT nResult;
        info._pszName = szName;
        info._nGroupID = nGroupID;
        info._nRefGroupID = s_nLinkOwnerGroup;
        info._nRsrcType = nRsrcID;
        info._bSceneManaged = s_bSceneManagedMode;
        if (s_pExtBuf) {
            buf.attachBuf(buf.getBufPtr(), nFileSize);
        }
        nResult = CRsrcDB::addRsrc_NoCheck(info, buf, __null);
        if (nResult >= 0) {
            if (nResult & 2) {
                CVramMan::waitTransTexDone(s_nReadThreadID);
            }
            if (s_pExtBuf) {
                s_pExtBuf += (nFileSize + 15) & ~15;
            }
        }
    } else if (nFileSize == -5) {
        TRACE_COL(_TTY_PURPLE, "\x93\xc7\x8d\x9e\x82\xdd\x83o\x83" "b\x83t\x83@\x82\xaa\x8am\x95\xdb\x8fo\x97\x88\x82\xc8\x82\xa2\x82\xe6\x81`\x83w(\x81@T\x84" "D)\xc9\x81@\xb3\xdc\xa7\xa7\xa7\xdd\n\x82\xaa\x82\xf1\x82\xaa\x82\xc1\x82\xc4\xa5\xa5\xa5\x93\xc7\x8d\x9e\x82\xe0\x82\xa4\x82\xc6\xa5\xa5\xa5\x82\xb5\x82\xbd\x83t\x83@\x83" "C\x83\x8b\x82\xcc\x96\xbc\x91O -> %s\n", szFilePath);
#line 1627
        ASSERT(0);
    }
}
INT _parseFilePath(const CHAR* pData, INT nRest, CHAR* pszOutPath, INT nType) {
    return 0;
}
CHAR* _getRCFAlias(const CHAR* pInData, CHAR* szOut, INT* pOutID) {
    const CHAR* p;
    char szLabel[128];
    CHAR* o;
    CHAR* pRet;
    CRCFAliasData* pData;

    if (!s_pAliasList) {
        return (CHAR*)pInData;
    }
    p = pInData;
    o = szLabel;
    while (*p && *p != ')') {
        *o++ = *p++;
    }
    pRet = __null;
    for (pData = s_pAliasList; pData; pData = pData->getNextData()) {
        if (pData->isMatchSrc(szLabel)) {
            strcpy(szOut, pData->getDst());
            pRet = (CHAR*)p + 1;
            if (pOutID) {
                *pOutID = pData->getID();
            }
            break;
        }
    }
    return pRet;
}
INT _parseFileType(const CHAR* pInData) {
    INT nType = 0xFF;
    const CHAR* p;
    char szTmp[16];
    INT nID;

    for (p = pInData; *p; p++) {
        if (*p == '.') {
            nType = 0;
            break;
        }
    }
    if (!nType) {
        p++;
        nType = 0xFF;
        if (*p == '(') {
            p++;
            if (_getRCFAlias(p, szTmp, &nID)) {
                nType = nID;
            }
        }
    }
    return nType;
}
INT CRcf::getMapID(SHORT nArea, SHORT nScene) {
    KH_DEBUG(RcfSceneInfo* pScene = getSceneInfo(nArea, nScene);)
    KH_DEBUG(RcfMapInfo* pMap;)

    KH_DEBUG(if (!pScene) {)
        KH_DEBUG(return 0;)
    KH_DEBUG(})
    KH_DEBUG(pMap = getMapInfo(pScene->_nRcfMapIdx);)
    KH_DEBUG(if (!pMap) {)
        KH_DEBUG(return 0;)
    KH_DEBUG(})
    return KH_VER((pMap->_nGroup + 50) * 10000 + pMap->_nMember, CRcfCD::getExtInfo((nArea + 100) * 10000 + nScene, 0));
}
SHORT CRcf::getBattleScene(SHORT nArea, SHORT nScene) {
    SHORT nBtl = CRcfCD::getMemberOption((nArea + 100) * 10000 + nScene);
    if (nBtl > 0) {
        return nBtl + 999;
    }
    return 0;
}
static inline INT _tolower(INT c) {
    INT l = c + 0x20;
    if (isupper(c)) {
    } else {
        __asm__ volatile(""); /* MATCH: keeps the branch: jump.c would make this a movz */
        l = c;
    }
    return l;
}

static inline INT _hexValue(INT c) {
    INT n = -1;
    if (isdigit(c)) {
        n = c - '0';
    } else {
        INT l = _tolower(c);
        if ((UINT)(l - 'a') <= 5) {
            n = l - 'a' + 10;
        }
    }
    return n;
}

INT _atoi(const CHAR* lpszInStr) {
    INT nRet = 0;
    const CHAR* p = lpszInStr;
    INT n;
    INT c;

    if (p) {
        if (*p == '$') {
            p++;
        } else if (*p == '0' && p[1] == 'x') {
            p += 2;
        } else {
            goto dec;
        }
        if (!*p) goto end;
        while (*p) {
            n = _hexValue(*p);
            if (n < 0) {
                goto end;
            }
            p++;
            nRet = nRet * 16 + n;
        }
        return nRet;
    dec:
        p = lpszInStr + 1;
        if (*lpszInStr != '#') p = lpszInStr;
        /* MATCH: both set up before a goto loop: a real loop hoists them too, but its top gets the align nop */
        INT ct = (INT)_ctype_ + 1;
        INT ten = 10;
    top:
        c = *p;
        if (!c) goto end;
        if (!(*(const unsigned char*)(c + ct) & 4)) goto end;
        nRet = nRet * ten + c - '0';
        asm volatile(""); /* MATCH: p++ last, for the b's slot */
        p++;
        goto top;
    }
end:
    {
        register INT r asm("$2") = nRet;
        asm volatile("" : : "r"(r)); /* MATCH: the move stays above the jr: its slot is a nop */
        return r;
    }
}
void CRcf::setLocale(INT nID) {
    s_nLocale = nID;
}
INT CRcf::getLocale() {
    return s_nLocale;
}
void _makeLocalizePath(CHAR* pszOut, const CHAR* pszSrc, INT nLocale) {
    CHAR* pszSig = __null;
    CHAR* p;
    INT n;

    switch (nLocale) {
    case 0:
        break;
    case 1:
        pszSig = "US";
        break;
    case 2:
        pszSig = "UK";
        break;
    case 3:
        pszSig = "FR";
        break;
    case 4:
        pszSig = "SP";
        break;
    case 5:
        pszSig = "GE";
        break;
    case 6:
        pszSig = "IT";
        break;
    case 7:
        pszSig = "DU";
        break;
    case 8:
        pszSig = "PO";
        break;
    }
    if (!pszSig) {
        strcpy(pszOut, pszSrc);
        return;
    }
    p = strrchr(pszSrc, '/');
    if (p) {
        n = p - pszSrc + 1;
        strncpy(pszOut, pszSrc, n);
        pszOut[n] = 0;
        strcat(pszOut, pszSig);
        strcat(pszOut, p);
    } else {
        strcpy(pszOut, pszSrc);
    }
}
INT _atolen(const CHAR* lpszInStr) {
    INT nRet = 0;
    const CHAR* p = lpszInStr;

    if (p) {
        if (*p == '$' || (*p == '0' && p[1] == 'x')) {
            if (*p == '$') {
                p++;
                nRet = 1;
            } else {
                p = lpszInStr + 2;
                nRet = 2;
            }
        } else {
            goto dec;
        }
        if (!*p) goto end;
        while (*p) {
            if (_hexValue(*p) < 0) {
                goto end;
            }
            p++;
            nRet++;
        }
        return nRet;
    dec:
        if (*p == '#') {
            p++;
            nRet = 1;
        }
        while (*p && isdigit(*p)) {
            nRet++;
            p++;
        }
    }
end:
    {
        register INT r asm("$2") = nRet;
        asm volatile("" : : "r"(r)); /* MATCH: the move stays above the jr: its slot is a nop (as in _atoi) */
        return r;
    }
}
CHAR* _getDbgParsePtr() {
    return (CHAR*)s_pDbgFileBuf->getBufPtr() + s_nDbgFilePtr;
}
INT _getDbgParseRestSize() {
    return s_pDbgFileBuf->getBufSize() - s_nDbgFilePtr;
}
void _skipDbgLine() {
    s_nDbgFilePtr += _skipLine(_getDbgParsePtr(), _getDbgParseRestSize());
}
KH_DEBUG(void CRcf::prepareLoadCategoryInfo() {)
    KH_DEBUG(s_nDbgReadStep = 0;)
KH_DEBUG(})
KH_DEBUG(INT CRcf::loadNextCategoryInfo(INT nCategory, CHAR* pOutLabel, INT nLabelMax, INT* pOutMemberID) {)
    KH_DEBUG(switch (s_nDbgReadStep) {)
    KH_DEBUG(case 0: {)
        KH_DEBUG(char szFileName[256];)
        KH_DEBUG(if (s_pDbgFileBuf) delete s_pDbgFileBuf;)
        KH_DEBUG(s_pDbgFileBuf = __null;)
#line 2113
        KH_DEBUG(s_pDbgFileBuf = new KH_AT("(2113)") CBuf;)
        KH_DEBUG(ASSERT(s_pDbgFileBuf);)
        KH_DEBUG(sprintf(szFileName, "cdrom0:\\rcf\\g%03d.rcf", nCategory);)
        KH_DEBUG(if (CFileIO::loadFile(szFileName, *s_pDbgFileBuf, 0) < 0) {)
            KH_DEBUG(if (s_pDbgFileBuf) delete s_pDbgFileBuf;)
            KH_DEBUG(s_pDbgFileBuf = __null;)
            KH_DEBUG(return 2;)
        KH_DEBUG(})
        KH_DEBUG(s_nDbgFilePtr = 0;)
        KH_DEBUG(s_nDbgReadStep++;)
        KH_DEBUG(break;)
    KH_DEBUG(})
    KH_DEBUG(case 1: {)
        KH_DEBUG(CHAR* p = _getDbgParsePtr();)
        KH_DEBUG(if (p[0] == ';' && p[1] == ' ') {)
            KH_DEBUG(char szDataName[256];)
            KH_DEBUG(INT i;)
            KH_DEBUG(INT nMember;)
            KH_DEBUG(char szTmp[1024];)
            KH_DEBUG(p += 2;)
            KH_DEBUG(s_nDbgFilePtr += 2;)
            KH_DEBUG(for (i = 0; *p && *p != '$' && i < 255 && !_isStr_CR(p); p++, i++) {)
                KH_DEBUG(szDataName[i] = *p;)
            KH_DEBUG(})
            KH_DEBUG(szDataName[i] = 0;)
            KH_DEBUG(_skipDbgLine();)
            KH_DEBUG(p = _getDbgParsePtr();)
            KH_DEBUG(while (*p && *p != '#') {)
                KH_DEBUG(p++;)
                KH_DEBUG(s_nDbgFilePtr++;)
            KH_DEBUG(})
            KH_DEBUG(nMember = _atoi(p);)
            KH_DEBUG(if (pOutMemberID) {)
                KH_DEBUG(*pOutMemberID = nMember;)
            KH_DEBUG(})
            KH_DEBUG(if (pOutLabel) {)
                KH_DEBUG(sprintf(szTmp, "%04d %s", nMember, szDataName);)
                KH_DEBUG(strncpy(pOutLabel, szTmp, nLabelMax - 2);)
                KH_DEBUG(pOutLabel[nLabelMax - 1] = 0;)
            KH_DEBUG(})
            KH_DEBUG(return 1;)
        KH_DEBUG(})
        KH_DEBUG(_skipDbgLine();)
        KH_DEBUG(break;)
    KH_DEBUG(})
    KH_DEBUG(})
    KH_DEBUG(if (s_nDbgFilePtr < s_pDbgFileBuf->getBufSize()) {)
        KH_DEBUG(return 0;)
    KH_DEBUG(})
    KH_DEBUG(s_nDbgReadStep = 0;)
    KH_DEBUG(if (s_pDbgFileBuf) delete s_pDbgFileBuf;)
    KH_DEBUG(s_pDbgFileBuf = __null;)
    KH_DEBUG(return 2;)
KH_DEBUG(})
BOOL isTokenSCENE(const CHAR* p) {
    return strncmp("$SCENE", p, 6) == 0;
}
BOOL isTokenMAP(const CHAR* p) {
    return strncmp("$MAP", p, 4) == 0;
}
BOOL isTokenEND(const CHAR* p) {
    return strncmp("$END", p, 4) == 0;
}
void _getStageInfo(SHORT& nOutGroup, SHORT& nOutMember, CHAR* pOutLabel, INT* pOutAtelCnt) {
    CHAR* p;
    char szDataName[128];
    INT i = 0;

    s_nDbgFilePtr += 2;
    p = _getDbgParsePtr();
    nOutGroup = _atoi(p);
    p += 3;
    s_nDbgFilePtr += 3;
    nOutMember = _atoi(p);
    p += 5;
    s_nDbgFilePtr += 5;
    while (*p && i < 127 && !_isStr_CR(p)) {
        if (*p == ',') {
            *pOutAtelCnt = atoi(p + 1);
            break;
        }
        szDataName[i++] = *p++;
    }
    szDataName[i] = 0;
    if (pOutLabel) {
        strcpy(pOutLabel, szDataName);
    }
    _skipDbgLine();
}
KH_DEBUG(BOOL _loadRcfInfoFile(const CHAR* pszFile) {)
    KH_DEBUG(INT nLen;)
    KH_DEBUG(BYTE* pBuf;)

    KH_DEBUG(if (s_pDbgFileBuf) delete s_pDbgFileBuf;)
    KH_DEBUG(nLen = CFileIO::getFileSize(pszFile);)
    KH_DEBUG(if (nLen <= 0) {)
        KH_DEBUG(s_pDbgFileBuf = __null;)
        KH_DEBUG(return FALSE;)
    KH_DEBUG(})
#line 2257
    KH_DEBUG(s_pDbgFileBuf = new KH_AT("(2257)") CBuf;)
    KH_DEBUG(ASSERT(s_pDbgFileBuf);)
    KH_DEBUG(pBuf = s_pDbgFileBuf->allocBuf_A64(nLen + 1);)
    KH_DEBUG(ASSERT(pBuf);)
    KH_DEBUG(if (CFileIO::loadFile(pszFile, pBuf, nLen) < 0) {)
        KH_DEBUG(if (s_pDbgFileBuf) delete s_pDbgFileBuf;)
        KH_DEBUG(s_pDbgFileBuf = __null;)
        KH_DEBUG(return FALSE;)
    KH_DEBUG(})
    KH_DEBUG(pBuf[nLen] = 0;)
    KH_DEBUG(return TRUE;)
KH_DEBUG(})
KH_DEBUG(void _disposeRcfInfoFile() {)
    KH_DEBUG(if (s_pDbgFileBuf) delete s_pDbgFileBuf;)
    KH_DEBUG(s_pDbgFileBuf = __null;)
KH_DEBUG(})
KH_DEBUG(void _countRcfSceneAndMap() {)
    KH_DEBUG(BOOL bCont = TRUE;)
    KH_DEBUG(CHAR* p = _getDbgParsePtr();)

    KH_DEBUG(while (*p && bCont) {)
        KH_DEBUG(_skipDbgLine();)
        KH_DEBUG(bCont = !isTokenSCENE(p);)
        KH_DEBUG(p = _getDbgParsePtr();)
    KH_DEBUG(})
    KH_DEBUG(bCont = TRUE;)
    KH_DEBUG(while (*p && bCont) {)
        KH_DEBUG(_skipDbgLine();)
        KH_DEBUG(if (*p == ';') {)
            KH_DEBUG(s_nRcfSceneCount++;)
        KH_DEBUG(} else if (isTokenEND(p)) {)
            KH_DEBUG(bCont = FALSE;)
        KH_DEBUG(})
        KH_DEBUG(p = _getDbgParsePtr();)
    KH_DEBUG(})
    KH_DEBUG(bCont = TRUE;)
    KH_DEBUG(while (*p && bCont) {)
        KH_DEBUG(_skipDbgLine();)
        KH_DEBUG(bCont = !isTokenMAP(p);)
        KH_DEBUG(p = _getDbgParsePtr();)
    KH_DEBUG(})
    KH_DEBUG(bCont = TRUE;)
    KH_DEBUG(while (*p && bCont) {)
        KH_DEBUG(_skipDbgLine();)
        KH_DEBUG(if (*p == '#') {)
            KH_DEBUG(s_nRcfMapCount++;)
        KH_DEBUG(} else if (isTokenEND(p)) {)
            KH_DEBUG(bCont = FALSE;)
        KH_DEBUG(})
        KH_DEBUG(p = _getDbgParsePtr();)
    KH_DEBUG(})
KH_DEBUG(})
KH_DEBUG(void _parseRcfSceneAndMapInfo() {)
    KH_DEBUG(CHAR* p;)
    KH_DEBUG(BOOL bCont;)
    KH_DEBUG(USHORT nSceneIdx;)
    KH_DEBUG(RcfSceneInfo* pScene;)
    KH_DEBUG(INT nAtelCnt;)
    KH_DEBUG(SHORT nGroup;)
    KH_DEBUG(SHORT nMember;)
    KH_DEBUG(SHORT nMapIdx;)
    KH_DEBUG(RcfMapInfo* pMap;)
    KH_DEBUG(RcfMapInfo* pCurrentMap;)
    KH_DEBUG(INT i;)

    KH_DEBUG(if (!s_pDbgFileBuf) {)
        KH_DEBUG(return;)
    KH_DEBUG(})
    KH_DEBUG(s_nDbgFilePtr = 0;)
    KH_DEBUG(p = _getDbgParsePtr();)
    KH_DEBUG(if (s_pRcfSceneArray && s_nRcfSceneCount > 0) {)
        KH_DEBUG(bCont = TRUE;)
        KH_DEBUG(while (*p && bCont) {)
            KH_DEBUG(_skipDbgLine();)
            KH_DEBUG(bCont = !isTokenSCENE(p);)
            KH_DEBUG(p = _getDbgParsePtr();)
        KH_DEBUG(})
        KH_DEBUG(pScene = s_pRcfSceneArray;)
        KH_DEBUG(nSceneIdx = 0;)
        KH_DEBUG(while (*p && nSceneIdx < s_nRcfSceneCount) {)
            KH_DEBUG(if (isTokenEND(p)) {)
                KH_DEBUG(_skipDbgLine();)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(if (*p == ';') {)
                KH_DEBUG(_getStageInfo(pScene->_nGroup, pScene->_nMember, pScene->_szTitle, &nAtelCnt);)
                KH_DEBUG(pScene->_nAtelCount = nAtelCnt;)
                KH_DEBUG(pScene->_nRcfMapIdx = -1;)
                KH_DEBUG(nSceneIdx++;)
                KH_DEBUG(pScene++;)
            KH_DEBUG(})
            KH_DEBUG(p = _getDbgParsePtr();)
        KH_DEBUG(})
    KH_DEBUG(})
    KH_DEBUG(if (s_pRcfMapArray && s_nRcfMapCount > 0) {)
        KH_DEBUG(p = _getDbgParsePtr();)
        KH_DEBUG(bCont = TRUE;)
        KH_DEBUG(while (*p && bCont) {)
            KH_DEBUG(_skipDbgLine();)
            KH_DEBUG(bCont = !isTokenMAP(p);)
            KH_DEBUG(p = _getDbgParsePtr();)
        KH_DEBUG(})
        KH_DEBUG(pMap = s_pRcfMapArray;)
        KH_DEBUG(nMapIdx = -1;)
        KH_DEBUG(pCurrentMap = __null;)
        KH_DEBUG(while (*p && nMapIdx < s_nRcfMapCount) {)
            KH_DEBUG(if (isTokenEND(p)) {)
                KH_DEBUG(_skipDbgLine();)
                KH_DEBUG(break;)
            KH_DEBUG(})
            KH_DEBUG(if (*p == '#') {)
                KH_DEBUG(nMapIdx++;)
                KH_DEBUG(_getStageInfo(pMap->_nGroup, pMap->_nMember, pMap->_szTitle, __null);)
                KH_DEBUG(pCurrentMap = pMap;)
                KH_DEBUG(pMap++;)
            KH_DEBUG(} else if (*p == ';' && pCurrentMap) {)
                KH_DEBUG(_getStageInfo(nGroup, nMember, __null, __null);)
                KH_DEBUG(i = _seekRcfSceneInfo(nGroup, nMember, s_pRcfSceneArray, s_nRcfSceneCount);)
                KH_DEBUG(if (i >= 0) {)
                    KH_DEBUG(RcfSceneInfo& s = s_pRcfSceneArray[i];)
                    KH_DEBUG(pCurrentMap->_SceneList.pushBack(s);)
                    KH_DEBUG(s._nRcfMapIdx = nMapIdx;)
                KH_DEBUG(})
            KH_DEBUG(} else {)
                KH_DEBUG(_skipDbgLine();)
            KH_DEBUG(})
            KH_DEBUG(p = _getDbgParsePtr();)
        KH_DEBUG(})
    KH_DEBUG(})
KH_DEBUG(})
KH_DEBUG(void _loadDbgRcfSceneInfo() {)
    KH_DEBUG(s_nRcfSceneCount = 0;)
    KH_DEBUG(s_nDbgFilePtr = 0;)
    KH_DEBUG(s_nRcfMapCount = 0;)
    KH_DEBUG(if (_loadRcfInfoFile("cdrom0:\\rcf\\scene.dat")) {)
        KH_DEBUG(_countRcfSceneAndMap();)
    KH_DEBUG(})
    KH_DEBUG(if (s_nRcfSceneCount > 0) {)
#line 2475
        KH_DEBUG(ASSERT(__null == s_pRcfSceneArray);)
        KH_DEBUG(s_pRcfSceneArray = new KH_AT("(2476)") RcfSceneInfo[s_nRcfSceneCount];)
    KH_DEBUG(})
    KH_DEBUG(if (s_nRcfMapCount > 0) {)
#line 2480
        KH_DEBUG(ASSERT(__null == s_pRcfMapArray);)
        KH_DEBUG(s_pRcfMapArray = new KH_AT("(2481)") RcfMapInfo[s_nRcfMapCount];)
    KH_DEBUG(})
    KH_DEBUG(_parseRcfSceneAndMapInfo();)
    KH_DEBUG(_disposeRcfInfoFile();)
KH_DEBUG(})

KH_DEBUG(RcfSceneInfo* CRcf::getSceneInfo(SHORT nIndex) {)
    KH_DEBUG(if (!s_pRcfSceneArray || s_nRcfSceneCount <= 0 || nIndex < 0 || nIndex >= s_nRcfSceneCount) {)
        KH_DEBUG(return __null;)
    KH_DEBUG(})
    KH_DEBUG(return &s_pRcfSceneArray[nIndex];)
KH_DEBUG(})
KH_DEBUG(RcfSceneInfo* CRcf::getSceneInfo(SHORT nArea, SHORT nScene) {)
    KH_DEBUG(INT nIdx = _seekRcfSceneInfo(nArea, nScene, s_pRcfSceneArray, s_nRcfSceneCount);)
    KH_DEBUG(return nIdx >= 0 ? &s_pRcfSceneArray[nIdx] : __null;)
KH_DEBUG(})
KH_DEBUG(SHORT CRcf::getSceneInfoIndex(SHORT nArea, SHORT nScene) {)
    KH_DEBUG(return _seekRcfSceneInfo(nArea, nScene, s_pRcfSceneArray, s_nRcfSceneCount);)
KH_DEBUG(})
KH_DEBUG(SHORT CRcf::getSceneInfoCount() {)
    KH_DEBUG(return s_nRcfSceneCount;)
KH_DEBUG(})
KH_DEBUG(RcfMapInfo* CRcf::getMapInfo(SHORT nIndex) {)
    KH_DEBUG(if (!s_pRcfMapArray || s_nRcfMapCount <= 0 || nIndex < 0 || nIndex >= s_nRcfMapCount) {)
        KH_DEBUG(return __null;)
    KH_DEBUG(})
    KH_DEBUG(return &s_pRcfMapArray[nIndex];)
KH_DEBUG(})
KH_DEBUG(RcfMapInfo* CRcf::getMapInfo(SHORT nArea, SHORT nScene) {)
    KH_DEBUG(INT nIdx = _seekRcfSceneInfo(nArea, nScene, s_pRcfMapArray, s_nRcfMapCount);)
    KH_DEBUG(return nIdx >= 0 ? &s_pRcfMapArray[nIdx] : __null;)
KH_DEBUG(})
KH_DEBUG(SHORT CRcf::getMapInfoIndex(SHORT nArea, SHORT nScene) {)
    KH_DEBUG(return _seekRcfSceneInfo(nArea, nScene, s_pRcfMapArray, s_nRcfMapCount);)
KH_DEBUG(})
KH_DEBUG(SHORT CRcf::getMapInfoCount() {)
    KH_DEBUG(return s_nRcfMapCount;)
KH_DEBUG(})


// SOURCE DATA: .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss .rodata
