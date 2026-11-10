#include "System/Movie/MSStream.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Movie/MovieFiles.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include "System/Thread.h"
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
int DeleteThread(int tid);
int StartThread(int tid, void* arg);
int SleepThread();
int WakeupThread(int tid);
int CancelWakeupThread(int tid);
int DelayThread(int usec);
int mcdRead2(u_int lsn, u_int sectors, void* buf);
int mcdBeginStream();
int mcdEndStream();
extern char _gp;
}

#include "System/FileIO.h"

static CMSStream* s_pMSS = NULL;
static INT s_nMssThreadStat = 0;

extern "C" unsigned char s_mssThreadStack[4096];
asm(".section .bss\n"
    "\t.align 3\n"
    "s_mssThreadStack:\n"
    "\t.space 0x1000\n"
    "\t.text");
static INT s_nMssThreadID;
static volatile SHORT s_nMssPtr;
static volatile SHORT s_nMssPutCnt;
static BYTE* s_pMssBufBank[6];
static unsigned int s_nMssSizeTbl[6];
static UINT s_nMssSector;
static UINT s_nChunkSize;


static void threadStream(void* p);
static void _initStreamer(UINT nChunkSize, UINT nStartSector);
static void _terminateStreamer();
static UINT _readStream(void* pOutBuf);

#line 77
CMSStream::CMSStream() {
    m_pbyIopBuffer = NULL;
    m_iLength = 0;
    m_iHost = -1;
    m_bPauseStatus = 0;
    /* read through a reference: the load must not be scheduled above the member stores */
    CMSStream*& rpMSS = s_pMSS;
    KH_DEBUG(if (!(__null == rpMSS)) AssertApp(__FILE__, __LINE__, "__null == s_pMSS", 0);)
    s_nMssPtr = 0;
    s_pMSS = this;
    s_nMssThreadID = -1;
    s_nMssThreadStat = 0;
    s_nMssPutCnt = 0;
    s_nChunkSize = 0;
    s_pMssBufBank[0] = NULL;
}

CMSStream::~CMSStream() {
    Close();
    s_pMSS = NULL;
}

BOOL CMSStream::Open(const CHAR* pszFile, INT nChunkSize) {
    SectorInfo info;
    Close();
    m_bPauseStatus = 0;
    m_iHost = -1;
    if (!CMovieFiles::getFileInfo(pszFile, info)) {
        TRACE_COL(_TTY_RED, "CMSStream::Open() ... not found file [%s]\n", pszFile);
        return FALSE;
    }
    m_iLength = info._nSectorLength << 11;
    TRACE_COL(_TTY_RED, "CMSStream::Open(%s): ... %d\n", pszFile, info._nStartSector);
    CFileIO::lock();
    mcdBeginStream();
    _initStreamer(nChunkSize, info._nStartSector);
    m_iHost = info._nStartSector;
    s_nMssThreadStat = 1;
    return TRUE;
}

void CMSStream::Close() {
    _terminateStreamer();
    mcdEndStream();
    CFileIO::unlock();
    m_pbyIopBuffer = NULL;
    s_nMssThreadStat = 0;
}

UINT CMSStream::Read(void* pvBuffer) {
    UINT size;
    if (m_iHost < 0 || m_iLength <= 0) {
        return 0;
    }
    size = _readStream(pvBuffer);
    m_iLength -= size;
    return size;
}

INT CMSStream::Pause() {
    if (s_nMssThreadStat == 1) {
        s_nMssThreadStat = 2;
    }
    return s_nMssThreadStat == 2;
}

INT CMSStream::Resume() {
    if (s_nMssThreadStat == 2) {
        WakeupThread(s_nMssThreadID);
        CancelWakeupThread(s_nMssThreadID);
        s_nMssThreadStat = 1;
    }
    return s_nMssThreadStat == 1;
}

BOOL CMSStream::IsEOF() {
    return m_iLength <= 0;
}

BOOL CMSStream::IsRunning() {
    return s_nMssThreadStat == 1;
}

static void threadStream(void* p) {
    INT nErrCnt = 0;
    INT nSect;
    INT r;
    BOOL bTrayOpened;
    while (s_nMssThreadStat < 3) {
        if (s_nMssThreadStat == 2 || s_nMssPutCnt == 6) {
            SleepThread();
            continue;
        }
        INT idx = (s_nMssPtr + s_nMssPutCnt) % 6;
        nSect = s_nChunkSize >> 11;
        r = mcdRead2(s_nMssSector, nSect, s_pMssBufBank[idx]);
        if (r == 0) {
            s_nMssSizeTbl[idx] = s_nChunkSize;
            s_nMssPutCnt++;
            s_nMssSector += nSect;
            nErrCnt = 0;
        } else {
            TRACE_USER(TGOTO, "stream read error... %d\n", r);
            if (r < -1) {
                if (r < -3) {
                    nErrCnt++;
                    bTrayOpened = nErrCnt >= 2;
                } else {
                    bTrayOpened = TRUE;
                }
            } else {
                nErrCnt++;
                bTrayOpened = nErrCnt >= 2;
            }
            if (bTrayOpened) {
                CFileIO::setTrayOpened();
                s_nMssThreadStat = 2;
            }
        }
        DelayThread(2000);
    }
    s_nMssThreadStat = 4;
}

#line 462
static void _initStreamer(UINT nChunkSize, UINT nStartSector) {
    if (s_pMssBufBank[0]) com_free(s_pMssBufBank[0]);
    s_pMssBufBank[0] = (BYTE*)com_malloc_d(nChunkSize * 6, __FILE__ "(464)", (UINT)-1);
    for (INT i = 1; i < 6; i++) {
        s_pMssBufBank[i] = s_pMssBufBank[0] + nChunkSize * i;
    }
    for (INT i = 5; i >= 0; i--) {
        s_nMssSizeTbl[i] = 0;
    }
    s_nMssPtr = 0;
    s_nMssSector = nStartSector;
    s_nChunkSize = nChunkSize;
    s_nMssPutCnt = 0;
    if (s_nMssThreadID < 0) {
        ThreadParam tp;
        tp.entry = threadStream;
        tp.stack = s_mssThreadStack;
        tp.stackSize = sizeof(s_mssThreadStack);
        tp.gpReg = &_gp;
        tp.initPriority = 2;
        s_nMssThreadID = StartThread(CreateThread(&tp), NULL);
    }
}

static void _terminateStreamer() {
    if (s_nMssThreadID >= 0) {
        s_nMssThreadStat = 3;
        WakeupThread(s_nMssThreadID);
        do {
            Thread::delayThread_1Frame();
        } while (s_nMssThreadStat != 4);
        DeleteThread(s_nMssThreadID);
        s_nMssThreadID = -1;
    }
    if (s_pMssBufBank[0]) com_free(s_pMssBufBank[0]);
    s_pMssBufBank[0] = NULL;
}

static UINT _readStream(void* pOutBuf) {
    UINT size = 0;
    if (s_nMssPutCnt > 0) {
        memcpy(pOutBuf, s_pMssBufBank[s_nMssPtr], size = s_nMssSizeTbl[s_nMssPtr]);
        INT r = s_nMssPutCnt;
        s_nMssPutCnt--;
        s_nMssPtr = (s_nMssPtr + 1) % 6;
        if (r == 6) {
            WakeupThread(s_nMssThreadID);
            CancelWakeupThread(s_nMssThreadID);
        }
    }
    return size;
}
// SOURCE DATA: .data .rodata .bss
