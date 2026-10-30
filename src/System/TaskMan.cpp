#define SYSTEM_TASKMAN_CPP
#include "System/HeapMan.h"
#define VECTOR_H_PATH "c:/KHCOM/MainApp/System/Vector.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Str.h"
#include "System/TaskMan.h"
#include "System/Vector.h"
#include "System/FifoFpl.h"
#include "System/RootTask.h"
#include "System/MainTaskFactory.h"
#include "System/PlaySpeedCtrl.h"
#include "System/DebugProcInfo.h"
#include "System/CoordMan.h"
#include "System/Coord.h"
#include "System/PadIO.h"
#include "System/Debug.h"
#include "System/HeapMan.h"
#include "System/Graphic/ModelMan.h"
#include "System/Graphic/WorldEnv.h"
#include "Game/Game.h"
#include "Form/DebugWindow.h"
#include "include_asm.h"

extern "C" int rand();

#include "System/Graphic/PackMan.h"




#include "System/RsrcDB.h"





#include "System/Rcf.h"





template <class T> static inline T& _ref(T& v) { return v; }

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}

/* The unit's .text ends with an unnamed copy of CSLCListT<CTask>'s inline destructor (and .rodata has one of its
 * vtable): our compiled copies are only kept with these lines in config/SLUS_217.99/symbol_addrs.txt
 *     _$_t9CSLCListT1Z5CTask_001CA290 = 0x001CA290; // type:func size:0x90
 *     _vt$t9CSLCListT1Z5CTask_00470FC8 = 0x00470FC8; // size:0x18 */

class CMainTaskRoot : public CTask {
public:
    CMainTaskRoot(CTask* pParent) : CTask(pParent, 0x8000) {
        ignorePause(TRUE);
        ignorePlaySpeed(TRUE);
    }
    virtual ~CMainTaskRoot() {}
    virtual void main() {}
};

#define SYS_TASK_MAX 64
#define _MAINTASK_INFO_STACK_SIZE 4

namespace {
struct _MainTaskInfo {
    MAINTASK_ID _nID;
    MAINTASK_CALLMODE _nMode;
    INT _nDllRsrc;
};

enum _MAINTASK_STATE {
    _MAINTASK_STATE_NONE = 0,
    _MAINTASK_STATE_RUNNING = 1,
    _MAINTASK_STATE_REMOVE = 2,
    _MAINTASK_STATE_REMOVE_DLL = 3,
    _MAINTASK_STATE_LOAD_DLL = 4,
    _MAINTASK_STATE_LOAD_WAIT = 5,
    _MAINTASK_STATE_LAUNCH_MAINTASK = 6
};

CTaskSLCManager* _pSLCManager = NULL;
CTaskSLCList* _pRootList;
CRootTask* _pRootTask;
CMainTaskRoot* _pMainTaskRoot;
CVector<int>* _pSecondaryTaskArray;
int _arraySysTask[SYS_TASK_MAX];
int _arrayMainTask[28];
_MainTaskInfo _stackMainTaskInfo[_MAINTASK_INFO_STACK_SIZE];
INT _nMainTaskStackLevel;
_MAINTASK_STATE _nMainTaskState;
HANDLE _hActiveMainTask;
MAINTASK_ID _nNextMainTask;
MAINTASK_CALLMODE _nNextMainTaskMode;
INT _nRemoveDllRsrc;
INT _nRnd;
SHORT _nTaskFlag;
SHORT _nPauseReq;
INT _nDebugPlaySpeed;
BOOL _bSteppingMode;
}

static void _initForMainTask();

#line 126
void CTaskMan::initialize() {
    _pSLCManager = new (__FILE__ "(127)") CTaskSLCManager;
    ASSERT(_pSLCManager);
    for (INT i = 0; i < SYS_TASK_MAX; i++) {
        _arraySysTask[i] = -1;
    }
    for (INT i = 0; i < 28; i++) {
        _arrayMainTask[i] = -1;
    }
#line 137
    _pRootList = new (__FILE__ "(137)") CTaskSLCList(*_pSLCManager);
    ASSERT(_pRootList);
#line 140
    _pRootTask = new (__FILE__ "(140)") CRootTask;
    ASSERT(_pRootTask);
    _pRootList->pushBack(*_pRootTask);
    _pRootTask->setHandle(0);
#line 147
    CTask* pTop = new (__FILE__ "(147)") CTask(_pRootTask, 0x4000);
    ASSERT(pTop);
    pTop->declareSysTask(14);
    _initForMainTask();
#line 155
    _pSecondaryTaskArray = new (__FILE__ "(155)") CVector<int>;
    ASSERT(_pSecondaryTaskArray);
    _pSecondaryTaskArray->reserve(8);
    _nRnd = rand() & 0xF;
    _nTaskFlag = 0;
    _nPauseReq = 0;
    _nDebugPlaySpeed = 0;
    _bSteppingMode = FALSE;
    CDebugProcInfo::initialize();
}

void CTaskMan::finalize() {
    TRACE("CTaskMan::finalize() ==== START ====\n");
    CDebugProcInfo::finalize();
    TRACE("CTaskMan::finalize() ==== DllTaskDelete ====\n");
    CTask* p;
    p = getSysTask(0x1F);
    delete p;
    p = getSysTask(3);
    delete p;
    p = getSysTask(0x19);
    delete p;
    p = getSysTask(0x18);
    delete p;
    p = getSysTask(0x11);
    delete p;
    TRACE("CTaskMan::finalize() ==== SecondaryTaskDelete ====\n");
    delete _pSecondaryTaskArray;
    _pSecondaryTaskArray = NULL;
    TRACE("CTaskMan::finalize() ==== TaskDelete ====\n");
    delete _pRootTask;
    _pRootTask = NULL;
    TRACE("CTaskMan::finalize() ==== ListDelete ====\n");
    delete _pRootList;
    _pRootList = NULL;
    delete _pSLCManager;
    _pSLCManager = NULL;
    _pMainTaskRoot = NULL;
    TRACE("CTaskMan::finalize() ==== END ====\n");
}

ISLCManagerT<CTask>& CTaskMan::getSLCManager() {
    return *_pSLCManager;
}

/* Matches except that nTQ and bPause swap registers (s5/s6). */
#if 0
#line 223
void CTaskMan::update(TIMEQ nTQ, TIMEQ nDispTQ, BOOL bPause) {
    ASSERT(_pRootTask && _pRootList);
    TIMEQ nSysLag = CPlaySpeedCtrl::getSysProgressTime(nDispTQ);
    TIMEQ nLag = CPlaySpeedCtrl::getProgressTime(nSysLag);
    INT nFlag = bPause ? (SHORT)(_nTaskFlag | 1) : (SHORT)(_nTaskFlag & ~1);
    _pSecondaryTaskArray->clear();
    TaskMainParam param(*_ref(_pSecondaryTaskArray), nTQ, nSysLag, nLag, _nRnd, nFlag);
    if (_nPauseReq) {
        if (_nPauseReq == 1) {
            CGame::suspendGameTask();
        }
        _nPauseReq = 0;
    }
    _invokePrimary(param);
    _invokeSecondary(param);
    if (!bPause) {
        CGame::checkPause();
    }
    _nRnd = param._nRnd;
    if (dbgButtonTrg((USER)0, 0x20, 1)) {
        CDebugWindow::openTopDebugWindow(_pRootTask);
    }
    _invokeDisp();
    _updateMainTask();
}
#else
/* placeholder for update's string */
__asm__(".section .sdata\n");
#line 223
static void __attribute__((section(".sdata"))) _updateStrings() {
    ASSERT(_pRootTask && _pRootList);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/TaskMan", update__8CTaskManiii);
#endif

void CTaskMan::postprocess() {
    _pRootTask->invokeSweepChildren();
}

void CTaskMan::_invokePrimary(TaskMainParam& param) {
    _invokePrimary_Main(param, *_pRootTask);
    CCoordMan::update(param._nSysTickCount, CCoords::_PP_PRIMARY);
}

/* Matches except for one srl that the original has as dsrl (isSecondaryObj) and a delay slot. */
#if 0
void CTaskMan::_invokePrimary_Main(TaskMainParam& param, CTask& task) {
    if (!task.isInactive()) {
        if (task.isSecondaryObj()) {
            if (!param._SecondaryArray.push_back(task.getHandle())) {
                TRACE("<error> CTaskMan : SecondaryArray.push_back\n");
            }
            goto children;
        }
        task.invokeMain(param);
        if (!task.isInactive()) {
            goto children;
        }
    }
    if (task.isInactiveTrg()) {
        task.clearInactiveTrg();
        _clearTimeLagTo(task);
    }
    return;
children:
    for (SLContainerT<CTask>* pContainer = task.getChildren().getHead(); pContainer;
         pContainer = pContainer->getNext()) {
        if (pContainer->getValue()) {
            _invokePrimary_Main(param, *pContainer->getValue());
        }
    }
}
#else
/* placeholder for _invokePrimary_Main's string and its use of CVector<int>::push_back */
__asm__(".section .sdata\n");
static void __attribute__((section(".sdata"))) _invokePrimaryMainData(TaskMainParam& param, CTask& task) {
    if (!param._SecondaryArray.push_back(task.getHandle())) {
        TRACE("<error> CTaskMan : SecondaryArray.push_back\n");
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/TaskMan", _invokePrimary_Main__8CTaskManR13TaskMainParamR5CTask);
#endif

void CTaskMan::_clearTimeLagTo(CTask& task) {
    task.clearTimeLag();
    for (SLContainerT<CTask>* pContainer = task.getChildren().getHead(); pContainer;
         pContainer = pContainer->getNext()) {
        if (pContainer->getValue()) {
            _clearTimeLagTo(*pContainer->getValue());
        }
    }
}

void CTaskMan::_invokeSecondary(TaskMainParam& param) {
    if (param._SecondaryArray.size() > 0) {
        for (HANDLE* it = param._SecondaryArray.begin(); it != param._SecondaryArray.end(); it++) {
            CTask* pTask = getTask(*it);
            if (pTask) {
                if (!pTask->isInactive()) {
                    pTask->invokeMain(param);
                } else if (pTask->isInactiveTrg()) {
                    pTask->clearInactiveTrg();
                    pTask->clearTimeLag();
                }
            }
        }
        CCoordMan::update(param._nSysTickCount, CCoords::_PP_SECONDARY);
    }
}

void CTaskMan::_invokeDisp() {
    CModelMan::clearDispList();
    CWorldEnv::mainProc();
    CModelMan::setupDrawEnv();
    _pRootTask->invokeDispAll(CPackMan::getDrawMode());
    CDebugProcInfo::dispProcInfo();
    CDebugProcInfo::dispPadInfo();
    CModelMan::setupDrawMode(1);
    CModelMan::drawDispList();
}

#line 414
BOOL CTaskMan::registerSysTask(UINT nID, CTask& task) {
    ASSERT(SYS_TASK_MAX > nID);
    if (_arraySysTask[nID] != -1) {
        return FALSE;
    }
    _arraySysTask[nID] = task.getHandle();
    task.setInfo(task.getInfo() | TASK_INFO_SYSTASK);
    return TRUE;
}

BOOL CTaskMan::unregisterSysTask(CTask& task) {
    BOOL bResult = FALSE;
    HANDLE* pTbl = _arraySysTask;
    HANDLE handle = task.getHandle();
    for (int i = 0; i < SYS_TASK_MAX; i++, pTbl++) {
        if (handle == *pTbl) {
            *pTbl = -1;
            bResult = TRUE;
            task.setInfo(task.getInfo() & ~TASK_INFO_SYSTASK);
            break;
        }
    }
    return bResult;
}

#line 450
static void _initForMainTask() {
    _pMainTaskRoot = new (__FILE__ "(451)") CMainTaskRoot(_pRootTask);
    ASSERT(_pMainTaskRoot);
    _nMainTaskStackLevel = -1;
    _nMainTaskState = _MAINTASK_STATE_NONE;
    _hActiveMainTask = 0;
    _nNextMainTask = (MAINTASK_ID)-1;
}

static BOOL _getCurrentMainTaskInfo(_MainTaskInfo& outInfo);

#line 480
void CTaskMan::_launchCurrentMainTask(BOOL bHideMode) {
    if ((UINT)_nMainTaskStackLevel >= _MAINTASK_INFO_STACK_SIZE) {
        TRACE("<error> CTaskMan : _launchCurrentMainTask() : MainTask-Stack is invalid !!\n");
        return;
    }
    _MainTaskInfo& info = _stackMainTaskInfo[_nMainTaskStackLevel];
    if (_nNextMainTask == -1) {
        TRACE("*** [MainTask] : Back To Prev MainTask ***\n");
    } else {
        CHAR* pszMode = "UNKONWN";
        switch (info._nMode) {
        case MAINTASK_REPLACE:
            pszMode = "REPLACE";
            break;
        case MAINTASK_CALL_REPLACE:
            pszMode = "CALL_REPLACE";
            break;
        case MAINTASK_CALL_HIDE:
            pszMode = "CALL_HIDE";
            break;
        }
        TRACE("*** [MainTask] <Stack:%d> : Changed. Mode : - %s -\n", _nMainTaskStackLevel, pszMode);
    }
    CTask* pMainTask = getTask(_arrayMainTask[info._nID]);
    if (bHideMode && pMainTask) {
        pMainTask->setActive(TRUE);
        pMainTask->setVisible(TRUE);
    } else {
#line 498
        ASSERT(__null == pMainTask);
        pMainTask = CMainTaskFactory::createMainTask(info._nID, _pMainTaskRoot);
#line 502
        ASSERT(pMainTask);
        pMainTask->setInfo(pMainTask->getInfo() | TASK_INFO_MAINTASK);
    }
    _hActiveMainTask = pMainTask->getHandle();
    _arrayMainTask[info._nID] = _hActiveMainTask;
}

#line 516
static BOOL _endCurrentMainTask(MAINTASK_CALLMODE nMode) {
    if (_nMainTaskStackLevel < 0) {
        return FALSE;
    }
    ASSERT(_MAINTASK_INFO_STACK_SIZE > _nMainTaskStackLevel);
    ASSERT(0 < _hActiveMainTask);
    _MainTaskInfo& info = _stackMainTaskInfo[_nMainTaskStackLevel];
    CTask* pMainTask = CTaskMan::getTask(_hActiveMainTask);
    if (pMainTask) {
        if (nMode == MAINTASK_CALL_HIDE) {
            pMainTask->setActive(FALSE);
            pMainTask->setVisible(FALSE);
        } else {
            pMainTask->out();
            _arrayMainTask[info._nID] = 0;
        }
    }
    _hActiveMainTask = 0;
    return TRUE;
}

static BOOL _getCurrentMainTaskInfo(_MainTaskInfo& outInfo) {
    if ((UINT)_nMainTaskStackLevel >= _MAINTASK_INFO_STACK_SIZE) {
        return FALSE;
    }
    _MainTaskInfo& info = _stackMainTaskInfo[_nMainTaskStackLevel];
    outInfo._nID = info._nID;
    outInfo._nMode = info._nMode;
    outInfo._nDllRsrc = info._nDllRsrc;
    return TRUE;
}

#line 563
static void _pushMainTaskStack(MAINTASK_ID nID, MAINTASK_CALLMODE nMode, INT nDllRsrc) {
    if (_nMainTaskStackLevel >= _MAINTASK_INFO_STACK_SIZE - 1) {
        TRACE("<error> CTaskMan : _pushMainTaskStack() : MainTask-Stack over flow !!\n");
        return;
    }
    _nMainTaskStackLevel++;
#line 570
    ASSERT(0 <= _nMainTaskStackLevel);
    _MainTaskInfo& info = _stackMainTaskInfo[_nMainTaskStackLevel];
    info._nID = nID;
    info._nMode = nMode;
    info._nDllRsrc = nDllRsrc;
}

#line 581
static void _popMainTaskStack(_MainTaskInfo* pOutInfo) {
    if (_nMainTaskStackLevel < 0) {
        TRACE("<error> CTaskMan : _popMainTaskStack() : MainTask-Stack under flow !!\n");
        return;
    }
#line 588
    ASSERT(_MAINTASK_INFO_STACK_SIZE > _nMainTaskStackLevel);
    if (pOutInfo) {
        _getCurrentMainTaskInfo(*pOutInfo);
    }
    _nMainTaskStackLevel--;
}

#line 600
void CTaskMan::removeMainTask(CTask& task) {
    if (task.getHandle() == _hActiveMainTask) {
        ASSERT((0) == ((_MAINTASK_STATE_REMOVE <= _nMainTaskState) && (_MAINTASK_STATE_LAUNCH_MAINTASK >= _nMainTaskState)));
        _nNextMainTask = (MAINTASK_ID)-1;
        _nMainTaskState = _MAINTASK_STATE_REMOVE;
    }
}

static BOOL _isAcceptChangeMainTask(INT nID) {
    if ((_MAINTASK_STATE_REMOVE <= _nMainTaskState && _MAINTASK_STATE_LAUNCH_MAINTASK >= _nMainTaskState) ||
        (UINT)nID >= 28) {
        return FALSE;
    }
    if (_nMainTaskStackLevel >= 0 && _arrayMainTask[nID] == _hActiveMainTask) {
        return FALSE;
    }
    return TRUE;
}

BOOL CTaskMan::reqChangeMainTask(INT nID, MAINTASK_CALLMODE nMode) {
    if (!_isAcceptChangeMainTask(nID)) {
        return FALSE;
    }
    _nNextMainTask = (MAINTASK_ID)nID;
    _nNextMainTaskMode = nMode;
    _nMainTaskState = _nMainTaskStackLevel < 0 ? _MAINTASK_STATE_LOAD_DLL : _MAINTASK_STATE_REMOVE;
    return TRUE;
}

void CTaskMan::_updateMainTask() {
    switch (_nMainTaskState) {
    case _MAINTASK_STATE_REMOVE:
        _nRemoveDllRsrc = 0;
        if (_endCurrentMainTask(_nNextMainTask != -1 ? _nNextMainTaskMode : MAINTASK_REPLACE)) {
            if (_nNextMainTask == -1 ||
                (_nNextMainTaskMode != MAINTASK_CALL_REPLACE && _nNextMainTaskMode != MAINTASK_CALL_HIDE)) {
                _MainTaskInfo info;
                _popMainTaskStack(&info);
                _nRemoveDllRsrc = info._nDllRsrc;
                if (_nNextMainTask == -1) {
                    _nNextMainTaskMode = info._nMode;
                }
            } else if (_nNextMainTaskMode == MAINTASK_CALL_REPLACE) {
                _MainTaskInfo info;
                _getCurrentMainTaskInfo(info);
                _nRemoveDllRsrc = info._nDllRsrc;
            }
        }
        _nMainTaskState = _nRemoveDllRsrc ? _MAINTASK_STATE_REMOVE_DLL : _MAINTASK_STATE_LOAD_DLL;
        break;
    case _MAINTASK_STATE_REMOVE_DLL:
        CRsrcDB::removeRsrcWithType(_nRemoveDllRsrc, 0x1F);
        _nMainTaskState = _MAINTASK_STATE_LOAD_DLL;
        break;
    case _MAINTASK_STATE_LOAD_DLL:
        if (_nNextMainTask != -1) {
            INT nDllRsrc = CMainTaskFactory::getMainTaskDllRsrcID(_nNextMainTask);
            if (nDllRsrc && CRsrcDB::getFirstRsrc(0x1F, nDllRsrc, 1)) {
                nDllRsrc = 0;
            }
            _pushMainTaskStack(_nNextMainTask, _nNextMainTaskMode, nDllRsrc);
            if (nDllRsrc) {
                UINT nDataID = nDllRsrc;
                CRcf::reqLoadRCF(nDataID / 10000, nDataID % 10000, 0x100, NULL, 0);
                _nMainTaskState = _MAINTASK_STATE_LOAD_WAIT;
                break;
            }
        }
        _nMainTaskState = _MAINTASK_STATE_LAUNCH_MAINTASK;
    case _MAINTASK_STATE_LAUNCH_MAINTASK:
        if (_nMainTaskStackLevel >= 0) {
            BOOL bHide = _nNextMainTask == -1 && _nNextMainTaskMode == MAINTASK_CALL_HIDE;
            _launchCurrentMainTask(bHide);
            _nMainTaskState = _MAINTASK_STATE_RUNNING;
        } else {
            _nMainTaskState = _MAINTASK_STATE_NONE;
        }
        break;
    case _MAINTASK_STATE_LOAD_WAIT: {
        _MainTaskInfo info;
        _getCurrentMainTaskInfo(info);
        UINT nDataID = info._nDllRsrc;
        if (CRcf::isCompleteLoadRCF(nDataID / 10000, nDataID % 10000)) {
            _nMainTaskState = _MAINTASK_STATE_LAUNCH_MAINTASK;
        }
        break;
    }
    }
}

#line 733
CTask* CTaskMan::getSysTask(UINT nID) {
    ASSERT(SYS_TASK_MAX > nID);
    if (nID) {
        return getTask(_arraySysTask[nID]);
    }
    return _pRootTask;
}

CTask* CTaskMan::getTask(HANDLE hTask) {
    if (_pSLCManager && hTask) {
        SLContainerT<CTask>* pContainer = _pSLCManager->getContainer(hTask);
        if (pContainer) {
            return pContainer->getValue();
        }
    }
    return NULL;
}

CTask* CTaskMan::getRootTask() {
    return _pRootTask;
}

void CTaskMan::enableRnd_TaskProc(BOOL bFlag) {
    if (bFlag) {
        _nTaskFlag |= 2;
    } else {
        _nTaskFlag &= ~2;
    }
}

BOOL CTaskMan::reqSuspendGameTask() {
    _nPauseReq = 1;
    return TRUE;
}

BOOL CTaskMan::setupDbgPlaySpeed() {
    if (CPadIO::isButtonTrg(2, 1)) {
        _bSteppingMode = TRUE;
    }
    if (_bSteppingMode && CPadIO::isButtonRpt(2, 1)) {
        return FALSE;
    }
    if (CPadIO::isButtonDown(0x400, 1)) {
        _bSteppingMode = FALSE;
        FLOAT fValue = CPadIO::getAnalogRY(1) * 0.00024414062f;
        if (fValue < -0.1f) {
            CPlaySpeedCtrl::setDebugPlaySpeed(fValue * -2.5f + 1.0f);
            CDebugProcInfo::changeProcCounterColor(0x60, 0x20, 0);
        } else if (!(fValue > 0.1f)) {
            resetDbgPlaySpeed();
            return TRUE;
        } else {
            CPlaySpeedCtrl::setDebugPlaySpeed(1.0f - fValue * 0.8f);
            CDebugProcInfo::changeProcCounterColor(0x20, 0x60, 0);
        }
    }
    return _bSteppingMode || CPlaySpeedCtrl::getSysProgressTime(50) == 50;
}

void CTaskMan::setDbgSteppingMode(BOOL bMode) {
    _bSteppingMode = bMode;
}

void CTaskMan::resetDbgPlaySpeed() {
    CPlaySpeedCtrl::setDebugPlaySpeed(1.0f);
    CDebugProcInfo::changeProcCounterColor(0x60, 0x60, 0);
}

void CTaskMan::debugDumpContainer(UINT nStartIndex, UINT nEndIndex) {
    if (_pSLCManager) {
        _pSLCManager->debugDump(nStartIndex, nEndIndex);
    }
}

// SOURCE DATA: .data .rodata .bss
