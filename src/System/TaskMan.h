// PARTIAL: added for Game/Bullet/CreateBullet
#ifndef SYSTEM_TASKMAN_H
#define SYSTEM_TASKMAN_H

#include "khtypes.h"
#include "System/Debug.h"
#include "System/SLContainer.h"
#include "System/FifoFpl.h"

/* CTaskSLCManager comes before CTask (Task.h, included below): TaskMan's vtables follow that order */
class CTaskSLCManager : public ISLCManagerT<CTask>, private CFifoFpl {
public:
    CTaskSLCManager() { CFifoFpl::initialize(sizeof(SLContainerT<CTask>), 0x1000, NULL); }
    virtual SLContainerT<CTask>* createContainer() {
        SLContainerT<CTask>* pContainer = (SLContainerT<CTask>*)allocBlock();
        if (!pContainer) {
            TRACE("\x83\x5e\x83\x58\x83\x4e\x97\x70\x83\x52\x83\x93\x83\x65\x83\x69\x82\xaa MAX \x82\xc9\x93\x9e\x92\x42\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
        }
        return pContainer;
    }
    virtual void releaseContainer(SLContainerT<CTask>& inContainer) { releaseBlock(&inContainer); }
    virtual SLContainerT<CTask>* getContainer(HANDLE hID) { return (SLContainerT<CTask>*)getValidActiveBlock(hID); }
    void debugDump(UINT nStartIndex, UINT nEndIndex) { CFifoFpl::debugDump(nStartIndex, nEndIndex); }
};

#include "System/Task.h"

// PARTIAL: added for System/RootTask
enum MAINTASK_CALLMODE {
    MAINTASK_REPLACE = 0,
    MAINTASK_CALL_REPLACE = 1,
    MAINTASK_CALL_HIDE = 2
};

// PARTIAL: added for System/Task
template <class T> class CVector;
extern INT VSYNC_TIMEQ;
struct TaskMainParam {
    CVector<int>& _SecondaryArray;
    TIMEQ _nSysTickCount;
    TIMEQ _nSysLag;
    TIMEQ _nLag;
    INT _nRnd;
    INT _nProcLevel;
    INT _nFlag;

    // PARTIAL: added for System/TaskMan
    TaskMainParam(CVector<int>& array, TIMEQ nSysTQ, TIMEQ nSysLag, TIMEQ nLag, INT nRnd, INT nFlag)
        : _SecondaryArray(array), _nSysTickCount(nSysTQ), _nSysLag(nSysLag), _nLag(nLag), _nRnd(nRnd), _nProcLevel(4),
          _nFlag(nFlag) {
        if (nLag <= VSYNC_TIMEQ) {
            _nProcLevel = 1;
        } else if (nLag <= VSYNC_TIMEQ * 2) {
            _nProcLevel = 2;
        } else if (nLag <= VSYNC_TIMEQ * 4) {
            _nProcLevel = 3;
        }
    }
};


class CTaskMan {
public:
    static void initialize();
    static void finalize();
    // PARTIAL: added for System/Task
    static ISLCManagerT<CTask>& getSLCManager();
    static void update(TIMEQ nTQ, TIMEQ nDispTQ, BOOL bPause);
    static void postprocess();
    static BOOL registerSysTask(UINT nID, CTask& inTask);
    static BOOL unregisterSysTask(CTask& inTask);
    static void removeMainTask(CTask& inTask);
    static BOOL reqChangeMainTask(INT nID, MAINTASK_CALLMODE nMode); // PARTIAL: added for System/RootTask
    static CTask* getSysTask(UINT nID);
    static CTask* getTask(HANDLE hTask); // PARTIAL: added for Form/Menu/MoogleShop
    static CTask* getRootTask();
    static void enableRnd_TaskProc(BOOL bFlag);
    static BOOL reqSuspendGameTask();
    static BOOL setupDbgPlaySpeed();
    static void setDbgSteppingMode(BOOL bMode);
    static void resetDbgPlaySpeed();
    static void debugDumpContainer(UINT nStartIndex, UINT nEndIndex);

private:
    static void _invokePrimary(TaskMainParam& param);
    static void _invokePrimary_Main(TaskMainParam& param, CTask& task);
    static void _clearTimeLagTo(CTask& task);
    static void _invokeSecondary(TaskMainParam& param);
    static void _invokeDisp();
    static void _launchCurrentMainTask(BOOL bHideMode);
    static void _updateMainTask();
};

#endif /* SYSTEM_TASKMAN_H */
