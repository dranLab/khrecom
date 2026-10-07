// PARTIAL: added for Game/Bullet/CreateBullet
#ifndef SYSTEM_TASK_H
#define SYSTEM_TASK_H

#include "khtypes.h"
#include "System/Debug.h"

class CCoords;
class CTask;
template <class T> class ISLCManagerT;
template <class T> class SLContainerT;

// PARTIAL: added for Game/Bullet/BulletManager
template <class T>
class SLContainerT {
private:
    HANDLE _hID;
    T* _pValue;
    SLContainerT<T>* _pNextContainer;
    int _nGeneral;
public:
    HANDLE getHandle() { return _hID; }
    T* getValue() { return _pValue; }
    SLContainerT<T>* getNext() { return _pNextContainer; }
};

template <class T>
class CSLCListT {
private:
    ISLCManagerT<T>& m_SLCManager;
    SLContainerT<T>* m_pHeadContainer;
    SLContainerT<T>* m_pTailContainer;
    int m_nListSize;
public:
    virtual ~CSLCListT();
    SLContainerT<T>* getHead() { return m_pHeadContainer; } // PARTIAL: added for Game/Bullet/BulletManager
};

typedef CSLCListT<CTask> CTaskSLCList;

/* Placeholder for the original header's inline function that reports a full task container pool: its
 * message ("タスク用コンテナが MAX に到達しました\n", Shift-JIS) heads .rodata of every includer. */
/* PARTIAL: added for Game/Gimmick/GimmickHitMore - not every includer has the string (the Gimmick units
 * don't): define TASK_H_NO_SLC_OVERFLOW_MSG before including to leave it out. */
#ifndef TASK_H_NO_SLC_OVERFLOW_MSG
inline void _taskSLCOverflow() {
    TRACE("\x83\x5e\x83\x58\x83\x4e\x97\x70\x83\x52\x83\x93\x83\x65\x83\x69\x82\xaa MAX \x82\xc9\x93\x9e\x92\x42\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
}
#endif

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
    virtual INT getTypeID() const;

    CTask* getSysTask(UINT nID); // PARTIAL: added for Game/Gimmick/GimmickHitMore
    HANDLE getHandle() { return m_hThis; }
    TIMEQ getFrameRate() { return m_nTimeLag; }                   // PARTIAL: added for Game/Gimmick/GimmickPlantChild
    FLOAT getFrameRateF() { return (FLOAT)getFrameRate() * 0.020000001f; } // PARTIAL: added for Game/Gimmick/GimmickPlantChild
    BOOL isOut() { return m_nInfo & 0x8000; }
    void out() {
        if (!isOut()) {
            m_nInfo |= 0x8000;
            onOut();
        }
    }

protected: // PARTIAL: added for Game/Bullet/BulletManager
    BOOL declareSysTask(UINT nID);
    CTaskSLCList& getChildren() { return m_listChildren; }
};

#endif /* SYSTEM_TASK_H */
