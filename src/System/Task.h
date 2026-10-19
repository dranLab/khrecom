// PARTIAL: added for Game/Bullet/CreateBullet
#ifndef SYSTEM_TASK_H
#define SYSTEM_TASK_H

#include "khtypes.h"
#include "System/Debug.h"

class CCoords;
class CCoord; // PARTIAL: added for Game/Atel/AtelCamera
union FMATRIX; // PARTIAL: added for System/Graphic/PathCamera
class CTask;
template <class T> class ISLCManagerT;
template <class T> class SLContainerT;
template <class T> class CSLCListT; // PARTIAL: added for System/CoordMan

// PARTIAL: added for System/CoordMan
template <class T>
class ISLCManagerT {
public:
    virtual ~ISLCManagerT() {}
    virtual SLContainerT<T>* createContainer() = 0;
    virtual void releaseContainer(SLContainerT<T>& inContainer) = 0;
    virtual SLContainerT<T>* getContainer(HANDLE hID) = 0;
};

// PARTIAL: added for Game/Bullet/BulletManager
template <class T>
class SLContainerT {
private:
    HANDLE _hID;
    T* _pValue;
    SLContainerT<T>* _pNextContainer;
    int _nGeneral;
    friend class CSLCListT<T>; // PARTIAL: added for System/CoordMan
public:
    HANDLE getHandle() { return _hID; }
    T* getValue() { return _pValue; }
    SLContainerT<T>* getNext() { return _pNextContainer; }
    void setValue(T* pValue) { _pValue = pValue; } // PARTIAL: added for System/Task
    void setGeneral(int nGeneral) { _nGeneral = nGeneral; } // PARTIAL: added for System/Graphic/2d/2DSpr (name unknown)
    int getGeneral() { return _nGeneral; } // PARTIAL: added for System/Graphic/2d/2DMan (name unknown)
};

template <class T>
class CSLCListT {
private:
    ISLCManagerT<T>& m_SLCManager;
    SLContainerT<T>* m_pHeadContainer;
    SLContainerT<T>* m_pTailContainer;
    int m_nListSize;
public:
    virtual ~CSLCListT() { removeAll(); } // PARTIAL: body added for System/CoordMan
    SLContainerT<T>* getHead() { return m_pHeadContainer; } // PARTIAL: added for Game/Bullet/BulletManager
    int getSize() { return m_nListSize; } // PARTIAL: added for System/Graphic/2d/2DSpr
    // PARTIAL: added for System/CoordMan
    CSLCListT(ISLCManagerT<T>& inSLCManager) : m_SLCManager(inSLCManager) { resetParam(); }
    SLContainerT<T>* pushBack(T& inEntry) {
        SLContainerT<T>* pContainer = createContainer(inEntry);
        if (!pContainer) {
            return NULL;
        }
        if (m_pTailContainer) {
            m_pTailContainer->_pNextContainer = pContainer;
        }
        m_pTailContainer = pContainer;
        if (!m_pHeadContainer) {
            m_pHeadContainer = pContainer;
        }
        m_nListSize++;
        return pContainer;
    }
    BOOL remove(HANDLE hContainer) {
        SLContainerT<T>* pPrevContainer = NULL;
        SLContainerT<T>* pContainer = m_pHeadContainer;
        while (pContainer) {
            SLContainerT<T>* pNextContainer = pContainer->getNext();
            if (hContainer == pContainer->getHandle()) {
                if (pPrevContainer) {
                    pPrevContainer->_pNextContainer = pNextContainer;
                } else {
                    m_pHeadContainer = pNextContainer;
                }
                if (pContainer == m_pTailContainer) {
                    m_pTailContainer = pPrevContainer;
                }
                releaseContainer(*pContainer);
                m_nListSize--;
                return TRUE;
            }
            pPrevContainer = pContainer;
            pContainer = pNextContainer;
        }
        return FALSE;
    }
    // PARTIAL: added for System/Task (name unknown): drops the containers whose value was cleared
    void removeNull() {
        SLContainerT<T>* pPrevContainer = NULL;
        SLContainerT<T>* pContainer = m_pHeadContainer;
        while (pContainer) {
            SLContainerT<T>* pNextContainer = pContainer->getNext();
            if (!pContainer->getValue()) {
                if (pPrevContainer) {
                    pPrevContainer->_pNextContainer = pNextContainer;
                } else {
                    m_pHeadContainer = pNextContainer;
                }
                if (pContainer == m_pTailContainer) {
                    m_pTailContainer = pPrevContainer;
                }
                releaseContainer(*pContainer);
                m_nListSize--;
            } else {
                pPrevContainer = pContainer;
            }
            pContainer = pNextContainer;
        }
    }
    // PARTIAL: added for System/Graphic/2d/2DSpr
    SLContainerT<T>* replace(HANDLE hContainer, T& inEntry) {
        SLContainerT<T>* pPrevContainer = NULL;
        SLContainerT<T>* pContainer = m_pHeadContainer;
        while (pContainer) {
            SLContainerT<T>* pNextContainer = pContainer->getNext();
            if (hContainer == pContainer->getHandle()) {
                BOOL bTail = (pContainer == m_pTailContainer);
                releaseContainer(*pContainer);
                SLContainerT<T>* pNewContainer = createContainer(inEntry);
                if (!pNewContainer) {
                    return NULL;
                }
                if (pPrevContainer) {
                    pPrevContainer->_pNextContainer = pNewContainer;
                } else {
                    m_pHeadContainer = pNewContainer;
                }
                if (bTail) {
                    m_pTailContainer = pNewContainer;
                }
                pNewContainer->_pNextContainer = pNextContainer;
                return pNewContainer;
            }
            pPrevContainer = pContainer;
            pContainer = pNextContainer;
        }
        return NULL;
    }
    void removeAll() {
        SLContainerT<T>* pContainer = m_pHeadContainer;
        while (pContainer) {
            SLContainerT<T>* p = pContainer;
            pContainer = pContainer->getNext();
            releaseContainer(*p);
        }
        resetParam();
    }

protected:
    SLContainerT<T>* createContainer(T& inEntry) {
        SLContainerT<T>* pContainer = m_SLCManager.createContainer();
        if (pContainer) {
            pContainer->_pValue = &inEntry;
            pContainer->_pNextContainer = NULL;
        }
        return pContainer;
    }
    void releaseContainer(SLContainerT<T>& inContainer) { m_SLCManager.releaseContainer(inContainer); }
    void resetParam() {
        m_pHeadContainer = NULL;
        m_pTailContainer = NULL;
        m_nListSize = 0;
    }
};

typedef CSLCListT<CTask> CTaskSLCList;

/* Placeholder for the original header's inline function that reports a full task container pool: its
 * message ("タスク用コンテナが MAX に到達しました\n", Shift-JIS) heads .rodata of every includer. */
/* PARTIAL: added for Game/Gimmick/GimmickHitMore - not every includer has the string (the Gimmick units
 * don't): define TASK_H_NO_SLC_OVERFLOW_MSG before including to leave it out. */
#if defined(SYSTEM_TASKMAN_CPP)
/* PARTIAL: added for System/TaskMan - the class the placeholder below stands for (its createContainer's message);
 * defined here, before CTask, as in the original (its vtable comes before CSLCListT<CTask>'s in .rodata) */
#include "System/FifoFpl.h"
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
#elif !defined(TASK_H_NO_SLC_OVERFLOW_MSG)
inline void _taskSLCOverflow() {
    TRACE("\x83\x5e\x83\x58\x83\x4e\x97\x70\x83\x52\x83\x93\x83\x65\x83\x69\x82\xaa MAX \x82\xc9\x93\x9e\x92\x42\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
}
#endif

enum TASK_PRIORITY {
    PRI_BIT_ALWAYS = 32768,
    PRI_BIT_DEFAULT = 16384,
    PRI_BIT_SOMETIMES = 8192,
    PRI_SOMETIMES_SHIFT = 0,
    PRI_SOMETIMES_LV1 = 8192,
    PRI_SOMETIMES_LV2 = 8193,
    PRI_SOMETIMES_LV3 = 8194,
    PRI_SOMETIMES_LV4 = 8196,
    PRI_SOMETIMES_LV5 = 8202,
    PRI_SOMETIMES_MASK = 3,
    PRI_DEFAULT = 16384,
    PRI_PLAYER = 32768,
    PRI_PSHOT = 32768,
    PRI_ENEMY = 8194,
    PRI_ESHOT = 32768,
    PRI_EFFECT = 8194,
    PRI_FURNITURE = 8202,
    PRI_EBOSS = 32768,
    PRI_NPC = 8196,
    PRI_SYSTEM = 32768,
    PRI_SLEEP = 8202,
    PRI_ENEMY_LV1 = 8192,
    PRI_ENEMY_LV2 = 8194,
    PRI_EFFECT_LV1 = 8192,
    PRI_EFFECT_LV2 = 8194,
    PRI_EFFECT_LV3 = 8196,
    PRI_ENEMY_LV3 = 8202,
    PRI_EFFECT_D = 16384
};

struct TaskMainParam;

/* m_nInfo bits */
#define TASK_INFO_SYSTASK 0x0001
#define TASK_INFO_SECONDARY 0x0002
#define TASK_INFO_COORDMANAGED 0x0004
#define TASK_INFO_INACTIVE 0x0008
#define TASK_INFO_INVISIBLE 0x0010
#define TASK_INFO_INACTIVETRG 0x0020
#define TASK_INFO_IGNOREPAUSE 0x0080
#define TASK_INFO_IGNOREPLAYSPEED 0x0100
#define TASK_INFO_IGNORECLIPOUT 0x0200
#define TASK_INFO_FORCEPROC 0x1000
#define TASK_INFO_NOCOORDCALC 0x2000
#define TASK_INFO_MAINTASK 0x4000
#define TASK_INFO_OUT 0x8000

/* All inline members are emitted (weak) in Task.cpp, in declaration order: keep the order. */
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
    virtual void main() {}
    virtual void disp() {}
    virtual void after() {}
    virtual void onOut() {}
    virtual INT getTypeID() const { return 0; }
    CTask* getParent() { return m_pParent; }
    const CTask* getParent() const { return m_pParent; }
    HANDLE getHandle() { return m_hThis; }
    HANDLE addChild(CTask& inTask);
    BOOL removeChild(CTask& inTask);
    BOOL removeChild(HANDLE hTask);
    BOOL deleteChild(CTask& inTask);
    BOOL deleteChild(HANDLE hTask);
    void deleteAllChildren();
    TIMEQ getFrameRate() const { return m_nTimeLag; }
    FLOAT getFrameRateF() const { return (FLOAT)getFrameRate() * 0.020000001f; }
    TIMEQ getDispFrameRate() const;
    void addCoord(UINT nNum);
    void addCoord(UINT nNum, CCoord* pParent);
    void setCoord(CCoords& crds);
    INT getCoordNum() const;
    void delCoord();
    void createCoords();
    CCoord& Crd(UINT nIndex);
    const CCoord& Crd(UINT nIndex) const;
    CCoords* getCoords() { return m_pCrds; }
    const CCoords* getCoords() const { return m_pCrds; }
    void adjustCoord();
    void adjustSelfCoord();
    FMATRIX& getLocal(UINT nIndex);
    const FMATRIX& getWorld(UINT nIndex) const;
    void setPriority(TASK_PRIORITY pri) { m_nProcPrio = pri; }
    /* PARTIAL: callers pass plain ints (System/Graphic/PathCamera, TexCtrl, Camera); a member template is
     * not emitted here. */
    template <class T> void setPriority(T pri) { setPriority((TASK_PRIORITY)pri); }
    TASK_PRIORITY getPriority() const { return (TASK_PRIORITY)m_nProcPrio; }
    void out() {
        if (!(m_nInfo & TASK_INFO_OUT)) {
            m_nInfo |= TASK_INFO_OUT;
            onOut();
        }
    }
    BOOL isClippedLastTime() const { return m_cLastClip < 0; }
    void setLastClip(INT c);
    BOOL isOut() const { return (m_nInfo & TASK_INFO_OUT) != 0; }
    BOOL isSysTask() const { return (m_nInfo & TASK_INFO_SYSTASK) != 0; }
    BOOL isMainTask() const { return (m_nInfo & TASK_INFO_MAINTASK) != 0; }
    BOOL isSecondaryObj() const { return (m_nInfo & TASK_INFO_SECONDARY) != 0; }
    BOOL isCoordManaged() const { return (m_nInfo & TASK_INFO_COORDMANAGED) != 0; }
    BOOL isIgnorePause() const { return (m_nInfo & TASK_INFO_IGNOREPAUSE) != 0; }
    BOOL isIgnorePlaySpeed() const { return (m_nInfo & TASK_INFO_IGNOREPLAYSPEED) != 0; }
    BOOL isIgnoreClipOut() const { return (m_nInfo & TASK_INFO_IGNORECLIPOUT) != 0; }
    void dontSkipMainOnce() { m_nInfo |= TASK_INFO_FORCEPROC; }
    BOOL isForceProcNextMain() const { return (USHORT)m_nInfo & TASK_INFO_FORCEPROC; }
    void setActive(BOOL bFlag) {
        if (!bFlag) {
            m_nInfo |= TASK_INFO_INACTIVE | TASK_INFO_INACTIVETRG;
        } else {
            m_nInfo &= ~(TASK_INFO_INACTIVE | TASK_INFO_INACTIVETRG);
        }
    }
    BOOL isActive() const { return !(m_nInfo & (TASK_INFO_OUT | TASK_INFO_INACTIVE)); }
    BOOL isInactive() const { return (m_nInfo & (TASK_INFO_OUT | TASK_INFO_INACTIVE)) != 0; }
    BOOL isInactiveTrg() const { return (m_nInfo & TASK_INFO_INACTIVETRG) != 0; }
    void setVisible(BOOL bFlag) {
        if (!bFlag) {
            m_nInfo |= TASK_INFO_INVISIBLE;
        } else {
            m_nInfo &= ~TASK_INFO_INVISIBLE;
        }
    }
    BOOL isVisible() const { return !(m_nInfo & (TASK_INFO_OUT | TASK_INFO_INVISIBLE)); }
    BOOL isInvisible() const { return (m_nInfo & (TASK_INFO_OUT | TASK_INFO_INVISIBLE)) != 0; }
    void enableCoordCalc(BOOL bFlag);
    BOOL isEnableCoordCalc() const { return !(m_nInfo & TASK_INFO_NOCOORDCALC); }
    void ignorePause(BOOL bFlag) {
        if (bFlag) {
            m_nInfo |= TASK_INFO_IGNOREPAUSE;
        } else {
            m_nInfo &= ~TASK_INFO_IGNOREPAUSE;
        }
    }
    void ignorePlaySpeed(BOOL bFlag) {
        if (bFlag) {
            m_nInfo |= TASK_INFO_IGNOREPLAYSPEED;
        } else {
            m_nInfo &= ~TASK_INFO_IGNOREPLAYSPEED;
        }
    }
    CTask* getSysTask(UINT nID); // PARTIAL: public for Game/Gimmick/GimmickHitMore (protected in the original)

protected:
    BOOL addToParent(CTask& inParent);
    BOOL removeFromParent();
    BOOL declareSysTask(UINT nID);
    BOOL removeFromSystem();
    void removeMainTask();
    void manageCoord(BOOL bFlag) {
        if (bFlag) {
            m_nInfo |= TASK_INFO_COORDMANAGED;
        } else {
            m_nInfo &= ~TASK_INFO_COORDMANAGED;
        }
    }
    void ignoreClipOut(BOOL bFlag) {
        if (bFlag) {
            m_nInfo |= TASK_INFO_IGNORECLIPOUT;
        } else {
            m_nInfo &= ~TASK_INFO_IGNORECLIPOUT;
        }
    }
    CTaskSLCList& getChildren() { return m_listChildren; }
    const CTaskSLCList& getChildren() const { return m_listChildren; }
    friend class CTaskSecondary; // PARTIAL: added for System/Graphic/Camera (m_nInfo)
    friend class CFadeIOMask;    // PARTIAL: added for System/Graphic/ScreenMask (m_nInfo)
    friend class CTaskMan;       // PARTIAL: added for System/TaskMan

private:
    void invokeMain(TaskMainParam& param);
    void invokeDispAll(INT nMode);
    void invokeSweepChildren();
    BOOL canProcMain(TaskMainParam& param);
    void calcCoord(TIMEQ nTQ);
    void calcCoordAll(TIMEQ nTQ);
    void clearLastClip() { m_cLastClip = 0; }
    void clearTimeLag() { m_nTimeLag = 0; }
    void clearInactiveTrg() { m_nInfo &= ~TASK_INFO_INACTIVETRG; }
    void setParent(CTask* pTask) { m_pParent = pTask; }
    void setHandle(HANDLE hTask) { m_hThis = hTask; }
    SHORT getInfo() { return m_nInfo; }
    void setInfo(SHORT nInfo) { m_nInfo = nInfo; }

public:
    CHAR* getDebugInfo() const { return m_pszDebugInfo; }
};

// PARTIAL: added for Game/Atel/AtelCamera
class CTaskSecondary : public CTask {
public:
    // PARTIAL: inline body added for System/Graphic/Camera
    CTaskSecondary(CTask* pParent, INT nProcPrio = 0x4000) : CTask(pParent, nProcPrio) { m_nInfo |= 2; }
};

#endif /* SYSTEM_TASK_H */
