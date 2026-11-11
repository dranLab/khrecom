#define CTDMAN_H_PATH "c:/KHCOM/MainApp/System/CTD/CtdMan.h" // MATCH: the original spelled this include with the full path ("c:/KHCOM/...", "System/CTD")
#define ATELCROSSFADE_H_PATH "Game/ATEL/AtelCrossFade.h" // MATCH: the original spelled the directory "ATEL"
/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "System/Task.h"
#include "System/TaskMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Ctd/CtdMan.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/Effect/effect.h"

#include "Game/Card.h"
#include "Game/Technic.h"
#include "Game/Status.h"

#include "Game/Atel/AtelCrossFade.h"
#include "System/MainApp.h"
#include "System/Debug.h"
#include "System/DebugProcInfo.h"
#include "System/Graphic/ColorFader.h"
#include "System/Spad.h"
#include "System/Rnd.h"
#include "System/mcd.h"
#include "Game/Debug/DebugWork.h"
#include "include_asm.h"


struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    u_int attr;
    u_int option;
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

typedef struct {
    u_char trycount;
    u_char spindlctrl;
    u_char datapattern;
    u_char pad;
} sceCdRMode;

typedef struct {
    u_int data;
    u_int addr;
    int size;
    int mode;
} sceSifDmaData;

extern "C" {
int CreateSema(SemaParam* param);
int DeleteSema(int sid);
int SignalSema(int sid);
int WaitSema(int sid);
int ReferSemaStatus(int sid, SemaParam* param); int iReferSemaStatus(int sid, SemaParam* param); int iSignalSema(int sid);
int GetThreadId();
int ChangeThreadPriority(int tid, int prio);
int CreateThread(ThreadParam* param);
int StartThread(int tid, void* arg);
int SleepThread(); int iWakeupThread(int tid); int iCancelWakeupThread(int tid);
int TerminateThread(int tid);
int DeleteThread(int tid);
int SuspendThread(int tid);
int DIntr();
int EIntr();
int AddIntcHandler(int cause, int (*handler)(int), int next);
int RemoveIntcHandler(int cause, int hid);
int EnableIntc(int cause);
int DisableIntc(int cause);
void FlushCache(int op);
int sceGsSyncPath(int mode, u_short timeout);
int sceGsSyncV(int mode);
void sceSifExitCmd();
void sceSifInitRpc(unsigned int mode);
int sceCdInit(int init_mode);
int sceCdMmode(int media);
int sceSifRebootIop(const char* img);
int sceSifSyncIop();
int sceSifInitIopHeap();
int sceSifLoadFileReset();
int sceFsReset();
int sceCdRead(u_int lsn, u_int sectors, void* buf, sceCdRMode* mode);
int sceCdSync(int mode);
int sceCdGetError();
void* sceSifAllocIopHeap(unsigned int size);
int sceSifFreeIopHeap(void* addr);
unsigned int sceSifSetDma(sceSifDmaData* sdd, int len);
int sceSifDmaStat(unsigned int id);
int sceSifLoadModuleBuffer(void* addr, int args, const char* argp);
int snInitDllSystem(void* p);
void srand(unsigned int seed);
int sprintf(char* buf, const char* fmt, ...);
char* strrchr(const char* s, int c);
}

extern "C" char _end[];
extern "C" char _gp[];

#define T0_COUNT ((volatile u_int*)0x10000000)
#define T0_MODE ((volatile u_int*)0x10000010)
#define T1_COUNT ((volatile u_int*)0x10000800)
#define T1_MODE ((volatile u_int*)0x10000810)

#include "System/Graphic/PackMan.h"



















#include "System/Graphic/ModelMan.h"


#include "System/PadIO.h"

#include "System/VramMan.h"








#include "System/CoordMan.h"

#include "System/Rcf.h"

#include "Game/SceneEnv.h"
#line 218

#include "System/Sound/SoundMan.h"
#line 229

#include "System/Movie/Movie.h"

#include "Game/GameWork.h"

#include "System/FileIO.h"

#include "System/RsrcDB.h"





#include "System/Graphic/FrecFont.h"

#include "System/Memoca.h"





#include "System/Graphic/WorldEnv.h"
#line 257

#include "System/HeapMan.h"








#include "Form/DebugWindow.h"

#include "System/PlaySpeedCtrl.h"

#include "Game/Collision/CollObj.h"

#include "Game/Collision/CollUtilA.h"

/* the singletons' inline creation/deletion (from their headers) */
struct MainAppSingletons {
    static void initCollUtil() {
        KH_DEBUG(if (!(__null == CCollUtil::m_pInstance)) AssertApp("Game/Collision/CollUtil.h", 184, "__null == m_pInstance", 0);)
        CCollUtil::m_pInstance = new ("Game/Collision/CollUtil.h(185)") CCollUtil;
        KH_DEBUG(if (!(CCollUtil::m_pInstance)) AssertApp("Game/Collision/CollUtil.h", 186, "m_pInstance", 0);)
        CCollUtil::m_pInstance->setup();
    }
    static void finalizeCollUtil() {
        if (CCollUtil::m_pInstance) delete CCollUtil::m_pInstance;
        CCollUtil::m_pInstance = __null;
    }
};

struct IopmodInfo {
    char _szName[16];
    INT _nOffset;
    INT _nSize;
    int __filler[2];
};

struct IopmodFileHeader {
    char _sig[4];
    SHORT _nVersion;
    SHORT _nModCount;
    SHORT _nInfoEntrySize;
    short int __filler[3];
    IopmodInfo _infoTbl[1];
};

namespace {
USHORT _nBootMode = 0;
INT _nSnapShotCount = 0;
BOOL _bHalt = 0;
}
FLOAT GRAVITY = -0.0816666707f;
INT SCAN_RATE_H = 15734;
INT VSYNC_PER_SEC = 60;
INT HLINE_PER_FRAME = 263;
INT SCREEN_HEIGHT = 384;
INT DISP_HEIGHT = 448;
FLOAT ASPECT_RATE_X = 0.8f;
FLOAT ASPECT_RATE_Y = 0.8f;
static UINT s_nTrScrWriteIndex = 0;
static UINT s_nTrScrWriteIndexX = 0;
static UINT s_nTrScrDispIndex = 0;
int g_bTraceOnScreen = 0;

namespace EERAM {
static INT _HEAP_START = (((INT)_end - 1) / 64 + 1) * 64;
static INT _HEAP_SIZE = 0x1FC0000 - _HEAP_START;
}
INT VSYNC_TIMEQ = 3000 / VSYNC_PER_SEC;
char s_FlipThreadStack[4096];
static char s_szTrScrBuf[24][57];
static CFnt* s_pTrScrStrTbl;
static CTile* s_pTrScrZabu;
namespace {
TIMEQ _nTickCount;
TIMEQ _nMainCount;
BOOL _bTickCountOverflow;
TIMEQ _nDispFrameRate;
CColorFader* _pMasterFader;
UINT _nFrameCount;
volatile UINT _nVSyncCount;
volatile INT _nVProcCount;
volatile INT _nVWaitCount;
volatile INT _nDrawEnd;
BOOL _bForceHalfFps;
INT _nBackToFullFpsTimer;
INT _nBackToFullFpsLag;
BOOL _bPAL;
INT _nFlipThreadID;
volatile INT _VsyncSema;
volatile INT _FlipSema;
INT _nVBlankHandle;
INT _nMainThreadID;
USHORT _nFlag;
TIMEQ _nPlayTimeCounter;
SHORT _nSysPauseReq;
SHORT _nSysPause;
SHORT _nUsrPauseReq;
SHORT _nUsrPause;
USHORT _nHardError;
USHORT _nHardErrorMsg;
INT _nObjCount;
INT _nProcCount;
INT _nDrawCount;
INT _nDraw2Count;
INT _nProcCountMax;
BOOL _bSetupManagers;
}

static INT _mainLoop();
static INT _mainProc(INT nTcount, TIMEQ nTQ);
static void _coldReset(BOOL bNTSC);
static void _initPS2();
static void _loadIopModules();
static void _setupManagers();
static void _disposeManagers();
static void _warmReset();
static void _closeWarningMessageCallback();
static void _checkHardError();
static BOOL _checkPause();
static void _softReset();
static void _setProcCount(USHORT q);
static void _setDrawCount(USHORT q);
static void _setDraw2Count(USHORT q);
static void _countPlayTime(TIMEQ nTQ);
static void _initThread();
static void _startFlipThread();
static INT _installIntcHandler(INT cause, INT (*handler)(INT), INT next);
static INT _uninstallIntcHandler(INT cause, INT hid);
static void _disposeThread();
static TIMEQ _getTimePerFrame(INT nHCount);
static INT _VBlankStartHandler(INT nCH);
static void procFlipThread(void*);
static void _checkSnapShot();
static void _checkReportMemory();
static void _setVideoMode(BOOL bNTSC);
static void _halt(const CHAR* pszMsg, const CHAR* pszExp);
static void _initTraceOnScreen();
static void _disposeTraceOnScreen();
static void _initFntTbl();
static void _dispTraceOnScreen();

void CMainApp::initialize(BOOL bNTSC) {
    TRACE("CMainApp START\n");
    _pMasterFader = __null;
    _coldReset(bNTSC);
}

void CMainApp::finalize() {
    sceSifExitCmd();
    TRACE("CMainApp END\n");
}

INT CMainApp::run() {
    DebugWork::initialize();
    for (;;) {
        _warmReset();
        _mainLoop();
        _softReset();
    }
}

static INT _mainLoop() {
    INT nResult;
    INT nTcount;
    INT nHcount;
    TIMEQ nTQ;

    _nVSyncCount = 0;
    _nVProcCount = 0;
    _nBackToFullFpsTimer = 0;
    _nBackToFullFpsLag = 0;
    _nFrameCount = 0;
    _nObjCount = 0;
    WaitSema(_VsyncSema);
    nTcount = (USHORT)*T1_COUNT;
    WaitSema(_VsyncSema);
    nHcount = (USHORT)*T1_COUNT - nTcount;
    nTcount = (USHORT)*T1_COUNT;
    nTQ = _getTimePerFrame(nHcount);
    for (;;) {
        _nDispFrameRate = nTQ;
        _nTickCount += nTQ;
        _bTickCountOverflow = (UINT)_nTickCount >> 31;
        if (_bTickCountOverflow) {
            _nTickCount &= 0x7FFFFFFF;
        }
        SPAD::freeAllSpad();
        CPadIO::mainProc(_nTickCount, nTQ);
        nResult = _mainProc(nTcount, nTQ);
        if (nResult == 1) goto end;
        _nDrawEnd = 1;
        WaitSema(_FlipSema);
        sceGsSyncPath(0, 0);
        CPackMan::swapDBuf(0);
        if (_nFlag & 1) {
            _countPlayTime(nTQ);
        }
        nHcount = (USHORT)*T1_COUNT - nTcount;
        nTcount = (USHORT)*T1_COUNT;
        TIMEQ nMax = 200;
        nTQ = _getTimePerFrame(nHcount);
        if (nTQ > nMax) {
            nTQ = nMax;
        }
        _checkSnapShot();
        _checkReportMemory();
        CPackMan::kickDMA(++_nFrameCount);
    }
end:
    return nResult;
}

static INT _mainProc(INT nTcount, TIMEQ nTQ) {
    BOOL bPause;
    USHORT pc;
    USHORT dc;
    INT i;
    BOOL bSoftReset;

    CPackMan::beginScene();
    CModelMan::clearTexCache();
    CGTexPrim::clearTexCache();
    CGTexPrim2D::clearTexCache();
    if (CMainApp::isEnableSoftReset()) {
        bSoftReset = 0;
        if (_nFlag & 4) {
            bSoftReset = 1;
        } else if (CPadIO::isButtonDown(0x90F, 0) && CPadIO::isButtonTrg(0x90F, 0)) {
            bSoftReset = 1;
            _nBootMode = 1;
        }
        if (bSoftReset) {
            if (!_bPAL) {
                CRcf::setLocale(0);
            }
            return 1;
        }
    }
    CSceneEnv::updateSys();
    CSound::mainProc();
    if (_bTickCountOverflow) {
        CCoordMan::resetTimeStamp();
        TRACE("-*-*-*- [TickCount Overflow] -*-*-*-\n");
    }
    _checkHardError();
    bPause = _checkPause();
    if (!bPause && CSceneEnv::getStopTypeBit() != 0xFFFEFFFFL) {
        _nMainCount += nTQ;
    }
    CEffectMan::update();
    CTaskMan::update(_nTickCount, _nDispFrameRate, bPause);
    CVramMan::transTexProc();
    CRcf::mainProc();
    _dispTraceOnScreen();
    if (CPadIO::isButtonDown(1, 1) && CPadIO::isButtonTrg(0x400, 1)) {
        g_bTraceOnScreen = !g_bTraceOnScreen;
    }
    CPackMan::endScene();
    CModelMan::drawDispListFG();
    CEffectMan::setup();
    if (KH_DW(DebugWork::g_dw.sys.m_HeapDispDetail)) {
        dbgReportHeapPerFile();
        KH_DEBUG(DebugWork::g_dw.sys.m_HeapDispDetail = 0;)
    }
    if (CPadIO::isButtonDown(0x80, 1)) {
        _nProcCountMax = 0;
    }
    pc = *(USHORT*)0x10000800 - nTcount;
    if (_bPAL) {
        pc = (INT)(pc * 1.00699198f);
    }
    if (pc > 0x1EC3) pc = 0x1EC3;
    _setProcCount(pc);
    sceGsSyncPath(0, 0);
    dc = *(USHORT*)0x10000800 - nTcount;
    if (_bPAL) {
        dc = (INT)(dc * 1.00699198f);
    }
    if (dc > 0x1EC3) dc = 0x1EC3;
    _setDrawCount(dc);
    if (CPackMan::getDrawMode() == 2) {
        CMovie::draw();
    }
    if (CEffectMan::isLastDraw()) {
        CPackMan::kickDMAFG(_nFrameCount);
        sceGsSyncPath(0, 0);
    }
    CEffectMan::draw();
    if (ATEL::CAtelCrossFade::m_pInstance) {
        if (ATEL::CAtelCrossFade::m_pInstance->isCrossFadeStoreImage()) {
            ATEL::CAtelCrossFade::m_pInstance->storeCrossFadeImage();
        }
        if (ATEL::CAtelCrossFade::m_pInstance && ATEL::CAtelCrossFade::m_pInstance->isCrossFadeDrawImage() &&
            (CPackMan::getDrawMode() != 1 || CPackMan::isStartStillMode())) {
            ATEL::CAtelCrossFade::m_pInstance->drawCrossFadeImage();
        }
    }
    CPackMan::transFrameEffect();
    if (CPackMan::isStartStillMode()) {
        CPackMan::saveScreen();
        sceGsSyncPath(0, 0);
    }
    if (!CEffectMan::isLastDraw()) {
        CPackMan::kickDMAFG(_nFrameCount);
    }
    CModelMan::drawDispListSemi();
    CTaskMan::postprocess();
    CEffectMan::destroyer();
    sceGsSyncPath(0, 0);
    dc = *(USHORT*)0x10000800 - nTcount;
    if (_bPAL) {
        dc = (INT)(dc * 1.00699198f);
    }
    if (dc > 0x1EC3) dc = 0x1EC3;
    _setDraw2Count(dc);
    return 0;
}

static void _coldReset(BOOL bNTSC) {
    ChangeThreadPriority(GetThreadId(), 1);
    if (snInitDllSystem(0)) {
        TRACE("DLL\x83V\x83X\x83" "e\x83\x80\x8f\x89\x8a\xfa\x89\xbb\x83G\x83\x89\x81[\n");
    }
    _setVideoMode(bNTSC);
    CVramMan::mapping(bNTSC);
    _initPS2();
    SPAD::freeAllSpad();
    _loadIopModules();
    mcdInit();
    mcdSetCheckFile("\\SLUS_217.99;1");
    CSound::coldReset();
    CGameWork::coldReset();
}

static void _initPS2() {
    sceSifInitRpc(0);
    sceCdInit(0);
    sceCdMmode(2);
    while (!sceSifRebootIop("cdrom0:\\MODULES\\IOPRP310.IMG;1")) {
    }
    while (!sceSifSyncIop()) {
    }
    sceSifInitRpc(0);
    sceSifInitIopHeap();
    sceSifLoadFileReset();
    sceCdInit(0);
    sceCdMmode(2);
    sceFsReset();
}

static void _loadIopModules() {
    BYTE* p;
    sceCdRMode mode;
    void* pIopTmp;
    IopmodInfo* pInfo;
    INT i;
    sceSifDmaData sdd;
    INT q;
    UINT value;

#line 778
    p = (BYTE*)com_memalign_d(0x50000, 64, __FILE__ "(778)", (UINT)-1);
    memset(&mode, 0, sizeof(mode));
    mode.spindlctrl = 1;
    while (sceCdRead(0x19C, 0xA0, p, &mode) != 1 || sceCdSync(0) || sceCdGetError()) {
    }
    pInfo = ((IopmodFileHeader*)p)->_infoTbl;
    pIopTmp = sceSifAllocIopHeap(0x20800);
    for (i = 0; i < ((IopmodFileHeader*)p)->_nModCount; i++, pInfo++) {
        TRACE("[%s]\n", pInfo->_szName);
        sdd.data = pInfo->_nOffset + (u_int)p;
        sdd.addr = (u_int)pIopTmp;
        sdd.size = (pInfo->_nSize + 15) & ~15;
        sdd.mode = 0;
        while (!(q = sceSifSetDma(&sdd, 1))) {
        }
        while (sceSifDmaStat(q) >= 0) {
        }
        while (sceSifLoadModuleBuffer(pIopTmp, 0, __null) < 0) {
        }
    }
    sceSifFreeIopHeap(pIopTmp);
    com_free(p);
}

static void _setupManagers() {
    _nTickCount = 0;
    _nMainCount = 0;
    _bTickCountOverflow = 0;
    CPlaySpeedCtrl::setPlaySpeed(1.f);
    CPadIO::initialize();
    CVramMan::initialize(_nMainThreadID);
    CCoordMan::initialize();
    CModelMan::initialize();
    CTaskMan::initialize();
    _pMasterFader = new KH_AT("(840)") CColorFader;
    CCollObj::collisionInit();
    MainAppSingletons::initCollUtil();
    CRsrcDB::initialize();
    CFrecFont::initialize();
    CFileIO::initialize();
    CRcf::initialize();
    CMemoca::initialize();
    CSound::initialize();
    CMovie::initialize();
    CGameWork::initialize();
    CCtdMan::initialize();
    CCard::initialize();
    CTechnic::initialize();
    CStatus::initialize();
    _bSetupManagers = 1;
}

static void _disposeManagers() {
    _bSetupManagers = 0;
    TRACE("---- CVramMan::halt() ----\n");
    CVramMan::halt(1);
    TRACE("---- CRcf::finalize() ----\n");
    CRcf::finalize();
    TRACE("---- CEffectMan::finalize() ----\n");
    CEffectMan::finalize();
    TRACE("---- CTaskMan::finalize() ----\n");
    CTaskMan::finalize();
    TRACE("---- Delete _pMasterFader ----\n");
    if (_pMasterFader) delete _pMasterFader;
    _pMasterFader = __null;
    TRACE("---- CGameWork::finalize() ----\n");
    CGameWork::finalize();
    TRACE("---- CCollUtil::finalize() ----\n");
    MainAppSingletons::finalizeCollUtil();
    TRACE("---- CCollObj::collisionDelete() ----\n");
    CCollObj::collisionDelete();
    TRACE("---- CFileIO::finalize() ----\n");
    CFileIO::finalize();
    TRACE("---- CSound::finalize() ----\n");
    CSound::finalize();
    TRACE("---- CMovie::finalize() ----\n");
    CMovie::finalize();
    TRACE("---- CRsrcDB::finalize() ----\n");
    CRsrcDB::finalize();
    TRACE("---- CModelMan::finalize() ----\n");
    CModelMan::finalize();
    TRACE("---- CPadIO::finalize() ----\n");
    CPadIO::finalize();
    TRACE("---- CMemoca::finalize() ----\n");
    CMemoca::finalize();
    TRACE("---- CWorldEnv::finalize() ----\n");
    CWorldEnv::finalize();
    TRACE("---- CFrecFont::finalize(); ----\n");
    CFrecFont::finalize();
    TRACE("---- CCtdMan::finalize(); ----\n");
    CCtdMan::finalize();
    TRACE("---- CCard::finalize(); ----\n");
    CCard::finalize();
    TRACE("---- chnic::finalize(); ----\n");
    CTechnic::finalize();
    TRACE("---- atus::finalize(); ----\n");
    CStatus::finalize();
    TRACE("---- CSceneEnv::finalize(); ----\n");
    CSceneEnv::finalize();
    TRACE("---- CCoordMan::finalize(); ----\n");
    CCoordMan::finalize();
    TRACE("---- CPackMan::finalize(); ----\n");
    CPackMan::finalize();
    TRACE("---- CVramMan::finalize(); ----\n");
    CVramMan::finalize();
}

static void _warmReset() {
    sceCdCLOCK rtc;
    INT nSeed;

    _nSysPauseReq = 0;
    _nSysPause = 0;
    _nUsrPauseReq = 0;
    _nUsrPause = 0;
    _nHardError = 0;
    _nHardErrorMsg = 0;
    _initTraceOnScreen();
    CWorldEnv::initialize();
    CPackMan::initialize();
    _initThread();
    _setupManagers();
    *T1_MODE = 0x9B;
    *T0_MODE = 0x83;
    sceGsSyncV(0);
    sceGsSyncPath(0, 0);
    TRACE("%.2f\x81\x93 memory used after warm reset.\n", CHeapMan::getUsedHeapRatio());
    CDebugWindow::enableTopDebugWindow(1);
    _nProcCountMax = 0;
    if (mcdReadClock(&rtc) == 0) {
        nSeed = rtc.hour * 3600 + rtc.minute * 60 + rtc.second;
    } else {
        nSeed = 0x9FBF1;
    }
    RND::initRnd(nSeed);
    srand(nSeed);
    RND::init_genrand(nSeed);
    _nFlag = 2;
    CMainApp::resetPlayTime(0);
}

BOOL CMainApp::isEnableSoftReset() {
    return !(_nFlag & 2);
}

static void _closeWarningMessageCallback() {
    _nSysPause &= ~8;
    CSound::setWarningPause(0, 0);
}

static void _checkHardError() {
    if (_nHardError) {
        if (CMainApp::isWarningPauseDisable()) return;
        if (_nHardError & 1) {
            if (!CFileIO::isTrayOpened() || CFileIO::checkTrayClosed()) {
                _nHardError &= ~1;
                if (!_nHardError) {
                    _closeWarningMessageCallback();
                }
            }
        }
    } else {
        if (CFileIO::isTrayOpened() || CFileIO::checkTrayOpened()) {
            _nHardError |= 1;
        }
        if (_nHardError) {
            if (CMainApp::isWarningPauseDisable()) {
                CSound::setWarningPause(1, 1);
            } else {
                _nSysPause |= 8;
                CSound::setWarningPause(1, 0);
            }
        }
    }
}

static BOOL _checkPause() {
    BOOL bPause;
    BOOL bOn;

    if (_nSysPauseReq & 4) {
        _nSysPause = (_nSysPause & ~1) | (_nSysPauseReq & 1);
        _nSysPauseReq &= ~4;
    }
    if (_nSysPause & 8) return 1;
    if (_nSysPause & 1) return 1;
    if (_nUsrPauseReq & 4) {
        _nUsrPause = _nUsrPauseReq & 3;
        _nUsrPauseReq = 0;
    }
    if (CMainApp::isDbgUsrPauseDisable()) return 0;
    if (CPadIO::isButtonTrg(0x100, 1)) {
        bPause = !CMainApp::isDbgUsrPauseNow();
        if (bPause) {
            _nUsrPause |= 1;
        } else {
            _nUsrPause &= ~1;
        }
        if (bPause) {
            CTaskMan::setDbgSteppingMode(0);
        } else {
            CTaskMan::resetDbgPlaySpeed();
        }
    } else {
        bPause = CMainApp::isDbgUsrPauseNow();
    }
    if (bPause) {
        return CTaskMan::setupDbgPlaySpeed();
    }
    return 0;
}

static void _softReset() {
    TRACE("================== SOFT RESET START ======================\n");
    FlushCache(0);
    sceGsSyncPath(0, 0);
    WaitSema(_VsyncSema);
    TRACE("\x81\xab\x81\xab\x81\xab\x81\xab _disposeManagers() Start \x81\xab\x81\xab\x81\xab\x81\xab\n");
    _disposeManagers();
    TRACE("\x81\xaa\x81\xaa\x81\xaa\x81\xaa _disposeManagers() End \x81\xaa\x81\xaa\x81\xaa\x81\xaa\n");
    TRACE("\x81\xab\x81\xab\x81\xab\x81\xab _disposeThread() Start \x81\xab\x81\xab\x81\xab\x81\xab\n");
    _disposeThread();
    TRACE("\x81\xaa\x81\xaa\x81\xaa\x81\xaa _disposeThread() End \x81\xaa\x81\xaa\x81\xaa\x81\xaa\n");
    TRACE("\x81\xab\x81\xab\x81\xab\x81\xab _disposeTraceOnScreen() Start \x81\xab\x81\xab\x81\xab\x81\xab\n");
    _disposeTraceOnScreen();
    TRACE("\x81\xaa\x81\xaa\x81\xaa\x81\xaa _disposeTraceOnScreen() End \x81\xaa\x81\xaa\x81\xaa\x81\xaa\n");
    TRACE("\x81\xab\x81\xab\x81\xab\x81\xab CHeapMan::resetHeap() Start \x81\xab\x81\xab\x81\xab\x81\xab\n");
    CHeapMan::resetHeap();
    TRACE("\x81\xaa\x81\xaa\x81\xaa\x81\xaa CHeapMan::resetHeap() End \x81\xaa\x81\xaa\x81\xaa\x81\xaa\n");
    TRACE("================== SOFT RESET DONE =======================\n");
}

BOOL CMainApp::reqSoftReset(SOFTRESET_MODE mode) {
    if (_nFlag & 2) {
        return 0;
    }
    _nFlag |= 4;
    _nBootMode = mode == SOFTRESET_SKIP_LOGO;
    return 1;
}

void CMainApp::enableSoftReset(BOOL bEnable) {
    if (!bEnable) {
        _nFlag |= 2;
    } else {
        _nFlag &= ~2;
    }
}

BOOL CMainApp::isSkipLogo() {
    return _nBootMode & 1;
}

static void _setProcCount(USHORT q) {
    _nProcCount = q;
    if (_nProcCountMax < q) {
        _nProcCountMax = q;
    }
}

static void _setDrawCount(USHORT q) {
    _nDrawCount = q;
}

static void _setDraw2Count(USHORT q) {
    _nDraw2Count = q;
}

void CMainApp::resetPlayTime(UINT nTime) {
    CGameWork::setInt(0, 0xC0, nTime);
    _nFlag &= ~1;
    _nPlayTimeCounter = 0;
}

static void _countPlayTime(TIMEQ nTQ) {
    INT nSec;

    _nPlayTimeCounter += nTQ;
    nSec = _nPlayTimeCounter / 3000;
    if (nSec) {
        CGameWork::setInt(0, 0xC0, CGameWork::getInt(0, 0xC0) + nSec);
        _nPlayTimeCounter -= nSec * 3000;
    }
}

UINT CMainApp::getPlayTime() {
    return CGameWork::getInt(0, 0xC0);
}

void CMainApp::startCountPlayTime() {
    _nFlag |= 1;
}

void CMainApp::pauseCountPlayTime() {
    _nFlag |= 1;
}

void CMainApp::setFpsFull() {
    _nVWaitCount = 0;
    _bForceHalfFps = 0;
    TRACE_COL(_TTY_RED, " --- 60fps ---\n");
    KH_DEBUG(DebugWork::g_dw.sys.m_iFps = 0;)
}

void CMainApp::setFpsHalf() {
    _nVWaitCount = 1;
    _bForceHalfFps = 1;
    TRACE_COL(_TTY_RED, " --- 30fps ---\n");
    KH_DEBUG(DebugWork::g_dw.sys.m_iFps = 1;)
}

BOOL CMainApp::isFpsHalf() {
    return _bForceHalfFps;
}

BOOL CMainApp::isNTSC() {
    return !_bPAL;
}

BOOL CMainApp::isPAL() {
    return _bPAL;
}

void CMainApp::setBackToFullFpsLag(INT nLag) {
    _nBackToFullFpsLag = nLag;
    if (nLag > 0) {
        _nVWaitCount = 1;
        _nBackToFullFpsTimer = 1;
    }
}

INT CMainApp::getBackToFullFpsLag() {
    return _nBackToFullFpsLag;
}

static void _initThread() {
    SemaParam sp;
    INT nID;

    sp.maxCount = 1;
    sp.initCount = 0;
    _VsyncSema = CreateSema(&sp);
    _FlipSema = CreateSema(&sp);
    CMainApp::setFpsFull();
    _nDrawEnd = 0;
    nID = GetThreadId();
    _nDrawEnd = -1;
    _nMainThreadID = nID;
    _nVProcCount = 99;
    _startFlipThread();
    WaitSema(_FlipSema);
    _nVBlankHandle = _installIntcHandler(2, _VBlankStartHandler, 0);
}

static void _startFlipThread() {
    ThreadParam tp;

    tp.entry = procFlipThread;
    tp.stack = s_FlipThreadStack;
    tp.gpReg = _gp;
    tp.stackSize = sizeof(s_FlipThreadStack);
    tp.initPriority = 1;
    _nFlipThreadID = CreateThread(&tp);
    StartThread(_nFlipThreadID, 0);
}

static INT _installIntcHandler(INT cause, INT (*handler)(INT), INT next) {
    INT intr;
    INT hid;

    intr = DIntr();
    hid = AddIntcHandler(cause, handler, next);
    if (hid >= 0) {
        EnableIntc(cause);
    }
    if (intr) {
        EIntr();
    }
    return hid;
}

static INT _uninstallIntcHandler(INT cause, INT hid) {
    INT intr;
    INT res;

    intr = DIntr();
    res = RemoveIntcHandler(cause, hid);
    if (res == 0) {
        DisableIntc(cause);
    }
    if (intr) {
        EIntr();
    }
    return res;
}

static void _disposeThread() {
    if (_nDrawEnd) {
        WaitSema(_FlipSema);
    }
    _nDrawEnd = 0;
    _uninstallIntcHandler(2, _nVBlankHandle);
    DeleteSema(_VsyncSema);
    DeleteSema(_FlipSema);
    TerminateThread(_nFlipThreadID);
    DeleteThread(_nFlipThreadID);
}

static TIMEQ _getTimePerFrame(INT nHCount) {
    if (nHCount < 0) {
        nHCount += 0x10000;
    }
    return (nHCount + 1) * 3000 / SCAN_RATE_H;
}

BOOL CMainApp::isSysPauseNow() {
    return _nSysPause & 1;
}

void CMainApp::setSysPause(BOOL bFlag) {
    INT n = _nSysPauseReq;
    n &= ~1;
    INT k = n | 4;
    INT m = n | 5;
    if (!bFlag) m = k; // MATCH: n|4 first keeps the cmov target n|5 in v1 (movz)
    _nSysPauseReq = m;
}

BOOL CMainApp::isWarningPauseNow() {
    return (_nSysPause & 0x28) == 0x28;
}

void CMainApp::enableWarningPause(BOOL bEnable) {
    _nSysPause = bEnable ? _nSysPause | 0x20 : _nSysPause & ~0x20;
    if (bEnable) {
        if (_nHardError) {
            _nSysPause |= 8;
            CSound::pauseAllSE();
        }
    } else {
        if (_nHardError) {
            _nSysPause &= ~8;
            CSound::startAllPausedSE();
        }
    }
}

BOOL CMainApp::isWarningPauseDisable() {
    return !(_nSysPause & 0x20);
}

BOOL CMainApp::isDbgUsrPauseNow() {
    return _nUsrPause & 1;
}

void CMainApp::enableDbgUsrPause(BOOL bFlag, BOOL bPause) {
    _nUsrPauseReq |= 4;
    if (bFlag) {
        _nUsrPauseReq |= 2;
    } else {
        _nUsrPauseReq &= ~2;
    }
    if (bPause) {
        _nUsrPauseReq |= 1;
    } else {
        _nUsrPauseReq &= ~1;
    }
}

BOOL CMainApp::isDbgUsrPauseDisable() {
    return !(_nUsrPause & 2);
}

static INT _VBlankStartHandler(INT nCH) {
    SemaParam sema;
    BOOL bFlip;
    CColorFader* pFader;

    _nVSyncCount++;
    _nVProcCount++;
    iReferSemaStatus(_VsyncSema, &sema);
    if (sema.numWaitThreads) iSignalSema(_VsyncSema);
    bFlip = FALSE;
    _nBackToFullFpsTimer--;
    if (_nDrawEnd) {
        *(volatile u_int*)0x10000000 = 0;
        INT nOverFrame = _nVProcCount - _nVWaitCount;
        if (nOverFrame > 0) {
            bFlip = TRUE;
            if (!_nVWaitCount && nOverFrame >= 2) {
                _nVWaitCount = 1;
                _nBackToFullFpsTimer = _nBackToFullFpsLag;
            }
        } else if (!_bForceHalfFps && _nBackToFullFpsTimer <= 0) {
            if (_nVProcCount == 1) {
                _nVWaitCount = 0;
                bFlip = TRUE;
            }
            _nBackToFullFpsTimer = 0;
        }
    }
    if (bFlip) {
        INT nID = _nFlipThreadID;
        iWakeupThread(nID);
        iCancelWakeupThread(nID);
        _nVProcCount = 0;
    }
    pFader = _pMasterFader;
    if (pFader && (pFader->hasReqFade() || pFader->isFadeDone())) {
        BOOL bRetFSAA = FALSE;
        INT nAlpha = (INT)((1.0f - pFader->getAlpha()) * 255.5f);
        CPackMan::setBGColor(pFader->getColor());
        CPackMan::updateBGBlend(nAlpha);
        if (pFader->isFadeDone()) {
            pFader->clearReq();
            bRetFSAA = nAlpha == 255;
        } else {
            pFader->main(VSYNC_TIMEQ);
        }
        if (bRetFSAA) CPackMan::changePcrtcMode(0);
    }
    __asm__ volatile("sync.l");
    __asm__ volatile("ei");
    return 0;
}

static void procFlipThread(void*) {
    for (;;) {
        if (_nDrawEnd > 0) {
            CPackMan::transFrame();
        }
        _nDrawEnd = 0;
        SignalSema(_FlipSema);
        SleepThread();
    }
}

static void _checkSnapShot() {
}

static void _checkReportMemory() {
}

BOOL CMainApp::reqBlackIn(USHORT nTime) {
    CColorFader* pFader = _pMasterFader;
    if (pFader) {
        if (pFader->reqBlackIn(nTime, 0.f)) {
            CPackMan::changePcrtcMode(1);
            return 1;
        }
        return 0;
    }
    return 0;
}

BOOL CMainApp::reqBlackOut(USHORT nTime) {
    CColorFader* pFader = _pMasterFader;
    if (pFader) {
        if (pFader->reqBlackOut(nTime, 1.f)) {
            CPackMan::changePcrtcMode(1);
            return 1;
        }
        return 0;
    }
    return 0;
}

BOOL CMainApp::reqWhiteIn(USHORT nTime) {
    CColorFader* pFader = _pMasterFader;
    if (pFader) {
        if (pFader->reqWhiteIn(nTime, 1.f)) {
            CPackMan::changePcrtcMode(1);
            return 1;
        }
        return 0;
    }
    return 0;
}

BOOL CMainApp::reqWhiteOut(USHORT nTime) {
    CColorFader* pFader = _pMasterFader;
    if (pFader) {
        if (pFader->reqWhiteOut(nTime, 0.f)) {
            CPackMan::changePcrtcMode(1);
            return 1;
        }
        return 0;
    }
    return 0;
}

void CMainApp::changeDrawModeNormal() {
    CPackMan::changePcrtcMode(0);
}

static void _setVideoMode(BOOL bNTSC) {
    _bPAL = !bNTSC;
    ASPECT_RATE_X = 0.8f;
    ASPECT_RATE_Y = 0.8f;
    if (bNTSC) {
        GRAVITY = -0.0816666707f;
        SCAN_RATE_H = 15734;
        VSYNC_PER_SEC = 60;
        HLINE_PER_FRAME = 263;
        SCREEN_HEIGHT = 384;
        DISP_HEIGHT = 448;
    } else {
        GRAVITY = -0.0980000049f;
        SCAN_RATE_H = 15625;
        VSYNC_PER_SEC = 50;
        HLINE_PER_FRAME = 314;
        SCREEN_HEIGHT = 512;
        DISP_HEIGHT = 512;
        ASPECT_RATE_Y = 1.06666672f;
    }
    VSYNC_TIMEQ = 3000 / VSYNC_PER_SEC;
}

TIMEQ CMainApp::getTickCount() {
    return _nTickCount;
}

TIMEQ CMainApp::getMainCount() {
    return _nMainCount;
}

TIMEQ CMainApp::getDispFrameRate() {
    return _nDispFrameRate;
}

UINT CMainApp::getFrameCount() {
    return _nFrameCount;
}

UINT CMainApp::getVSyncCount() {
    return _nVSyncCount;
}

INT CMainApp::getObjCount() {
    return _nObjCount;
}

void CMainApp::incObjCount() {
    _nObjCount++;
}

void CMainApp::decObjCount() {
    _nObjCount--;
}

INT CMainApp::getProcCount() {
    return _nProcCount;
}

INT CMainApp::getDrawCount() {
    return _nDrawCount;
}

INT CMainApp::getDraw2Count() {
    return _nDraw2Count;
}

INT CMainApp::getProcCountMax() {
    return _nProcCountMax;
}

void CMainApp::resetProcCountMax() {
    _nProcCountMax = 0;
}

void (AssertApp)(char* pszFile, int nLine, char* pszMsg, char* pszExp) {
    CHeapMan::reportMemory();
    CMainApp::halt(pszFile, nLine, pszMsg, pszExp);
}

static void _halt(const CHAR* pszMsg, const CHAR* pszExp) {
    extern CHAR* __BUILD;
    INT nThreadID;

    _bHalt = 1;
    nThreadID = GetThreadId();
    if (nThreadID != _nMainThreadID) {
        SemaParam sema;
        if (nThreadID == _nFlipThreadID) return;
        SuspendThread(_nMainThreadID);
        ChangeThreadPriority(nThreadID, 1);
        ReferSemaStatus(_FlipSema, &sema);
        if (sema.numWaitThreads) SignalSema(_FlipSema);
    }
    _nDrawEnd = 1;
    WaitSema(_FlipSema);
    _nFrameCount++;
    CWorldEnv::setFogColor(0, 0, 0);
    CMainApp::setFpsFull();
    char szBuf[256];
    sprintf(szBuf, "BUILD-VER.: %s", __BUILD);
    CFnt strBuild;
    strBuild.setText(szBuf);
    strBuild.setPosition(-240, 40 - SCREEN_HEIGHT / 2, 0xFFFF28);
    strBuild.setLayerFG(25);
    strBuild.setStyle(0x40);
    CFnt strMsg;
    strMsg.setText(pszMsg);
    strMsg.setPosition(-240, 60 - SCREEN_HEIGHT / 2, 0xFFFF28);
    strMsg.setLayerFG(25);
    strMsg.setStyle(0x40);
    char szEtc[256];
    sprintf(szEtc, "FREE-HEAP: %d  /  MAX-SIZE: %d", CHeapMan::getFreeHeapSize(), CHeapMan::getMaxFreeBlockSize());
    CFnt strEtc;
    strEtc.setText(szEtc);
    strEtc.setPosition(-240, 80 - SCREEN_HEIGHT / 2, 0xFFFF28);
    strEtc.setLayerFG(25);
    strEtc.setStyle(0x40);
    CFnt strExp;
    strExp.setText(pszExp);
    strExp.setPosition(-240, 100 - SCREEN_HEIGHT / 2, 0xFFFF28);
    strExp.setLayerFG(25);
    strExp.setStyle(0x40);
    INT nCount = 2;
    while (nCount--) {
        CPackMan::beginScene();
        CGTexPrim::clearTexCache();
        CModelMan::clearDispList();
        strBuild.disp();
        strMsg.disp();
        strEtc.disp();
        if (pszExp) strExp.disp();
        CModelMan::drawDispList();
        CPackMan::endScene();
        FlushCache(0);
        sceGsSyncPath(0, 0);
        CModelMan::drawDispListFG();
        CPackMan::kickDMAFG(_nFrameCount);
        sceGsSyncPath(0, 0);
        _nDrawEnd = 1;
        WaitSema(_FlipSema);
        _nFrameCount++;
        sceGsSyncPath(0, 0);
        CPackMan::kickDMA(_nFrameCount);
    }
}

void CMainApp::halt(const CHAR* pszFile, INT nLine, const CHAR* pszMsg, const CHAR* pszExp) {
    char szBuf[512];
    const CHAR* pszPath;

    CPackMan::changeDrawMode(0);
    pszPath = strrchr(pszFile, '/');
    if (pszPath) {
        pszPath++;
    } else {
        pszPath = pszFile;
    }
    sprintf(szBuf, "ASSERT: %s(%d) -> \"%s\"\n", pszPath, nLine, pszMsg);
    if (!g_bTraceOnScreen) {
        TRACE(szBuf);
    }
    if (_bSetupManagers && !_bHalt) {
        _halt(szBuf, pszExp);
    }
    __asm__ volatile("break 0");
}

static void _initTraceOnScreen() {
    UINT i;

    s_nTrScrWriteIndex = 0;
    s_nTrScrWriteIndexX = 0;
    s_nTrScrDispIndex = 0;
    s_pTrScrStrTbl = __null;
    s_pTrScrZabu = __null;
    for (i = 0; i < 24; i++) {
        s_szTrScrBuf[i][0] = 0;
    }
}

static void _disposeTraceOnScreen() {
    s_nTrScrWriteIndex = 0;
    s_nTrScrWriteIndexX = 0;
    s_nTrScrDispIndex = 0;
    if (s_pTrScrStrTbl) delete[] s_pTrScrStrTbl;
    s_pTrScrStrTbl = __null;
    if (s_pTrScrZabu) delete s_pTrScrZabu;
    s_pTrScrZabu = __null;
    g_bTraceOnScreen = 0;
}

static void _initFntTbl() {
#line 2334
    s_pTrScrStrTbl = new KH_AT("(2334)") CFnt[24];
    KH_DEBUG(if (!(s_pTrScrStrTbl)) AssertApp(__FILE__, __LINE__, "s_pTrScrStrTbl", 0);)
    for (UINT i = 0; i < 24; i++) {
        CFnt& f = s_pTrScrStrTbl[i];
        f.setSize(13);
        f.setPosition(-220, i * 14 - 156, 0xFFFF28);
        f.setStyle(0x8000);
        f.setColor(0x80, 0x80, 0x80);
        f.setLayerFG(25);
    }
#line 2348
    s_pTrScrZabu = new KH_AT("(2348)") CTile;
    s_pTrScrZabu->m_P0.set(-0xDC0, -0xAA0); // MATCH: the original reloads s_pTrScrZabu for each later statement (volatile reads)
    (*(CTile* volatile*)&s_pTrScrZabu)->m_P1.set(0xDC0, 0xAA0);
    (*(CTile* volatile*)&s_pTrScrZabu)->m_C0.set(1, 1, 1, 0x40);
    (*(CTile* volatile*)&s_pTrScrZabu)->setSemiTrans(ALPHA_BLEND);
    (*(CTile* volatile*)&s_pTrScrZabu)->setLayerFG(25);
}

void addTraceString(CHAR* pszSrc) {
    CHAR* p;
    CHAR c;
    UINT j;
    BOOL bCR;

    if (!g_bTraceOnScreen || !pszSrc) {
        return;
    }
    if (!s_pTrScrStrTbl) {
        _initFntTbl();
    }
    p = s_szTrScrBuf[s_nTrScrWriteIndex % 24];
    bCR = 0;
    j = s_nTrScrWriteIndexX;
    {
        UINT i;
        for (i = j; i < 56; i++) {
            c = *pszSrc++;
            if (!c) break;
            if (c == '\n') {
                bCR = 1;
                break;
            }
            if (c == '\t') {
                c = ' ';
            }
            p[j++] = c;
        }
    }
    p[j] = 0;
    if (!bCR) {
        if (!c) {
            s_nTrScrWriteIndexX = j;
            return;
        }
        while (*pszSrc && *pszSrc != '\n') {
            pszSrc++;
        }
        if (*pszSrc != '\n') {
            return;
        }
    }
    s_nTrScrWriteIndexX = 0;
    if (++s_nTrScrWriteIndex > 24) {
        if (++s_nTrScrDispIndex >= 24) {
            s_nTrScrDispIndex = 0;
        }
    }
    {
        UINT i;
        for (i = 0, j = s_nTrScrDispIndex; i < 24; i++) {
            s_pTrScrStrTbl[i].setText(s_szTrScrBuf[j]);
            if (++j >= 24) {
                j = 0;
            }
        }
    }
}

static void _dispTraceOnScreen() {
    UINT i;

    if (!g_bTraceOnScreen) {
        return;
    }
    if (s_pTrScrZabu) {
        s_pTrScrZabu->disp();
    }
    if (s_pTrScrStrTbl) {
        for (i = 0; i < 24; i++) {
            s_pTrScrStrTbl[i].disp();
        }
    }
}
// SOURCE DATA: .data .rodata .bss
