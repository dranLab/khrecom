// PARTIAL: added for Game/Bullet/CreateBullet
#ifndef SYSTEM_TASK_H
#define SYSTEM_TASK_H

#include "khtypes.h"
#include "System/Debug.h"

class CCoords;
class CTask;
template <class T> class ISLCManagerT;
template <class T> class SLContainerT;

template <class T>
class CSLCListT {
private:
    ISLCManagerT<T>& m_SLCManager;
    SLContainerT<T>* m_pHeadContainer;
    SLContainerT<T>* m_pTailContainer;
    int m_nListSize;
public:
    virtual ~CSLCListT();
};

typedef CSLCListT<CTask> CTaskSLCList;

/* Placeholder for the original header's inline function that reports a full task container pool: its
 * message ("タスク用コンテナが MAX に到達しました\n", Shift-JIS) heads .rodata of every includer. */
inline void _taskSLCOverflow() {
    TRACE("\x83\x5e\x83\x58\x83\x4e\x97\x70\x83\x52\x83\x93\x83\x65\x83\x69\x82\xaa MAX \x82\xc9\x93\x9e\x92\x42\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
}

class CTask {
private:
    union {
        u_long128 _nZeroMember128;
        struct {
            CCoords* m_pCrds;
            TIMEQ m_nLagStock;
            TIMEQ m_nTimeLag;
            SHORT m_nInfo;
            BYTE m_cFrameSkip;
            SBYTE m_cLastClip;
        };
    };
    INT m_nProcPrio;
    HANDLE m_hThis;
    CTask* m_pParent;
    CTaskSLCList m_listChildren;
    CHAR* m_pszDebugInfo;

public:
    CTask(CTask* pParent, INT nProcPrio);
    virtual ~CTask();
    virtual void main();
    virtual void disp();
    virtual void after();
    virtual void onOut();
    virtual INT getTypeID();

    HANDLE getHandle() { return m_hThis; }
    BOOL isOut() { return m_nInfo & 0x8000; }
    void out() {
        if (!isOut()) {
            m_nInfo |= 0x8000;
            onOut();
        }
    }
};

#endif /* SYSTEM_TASK_H */
