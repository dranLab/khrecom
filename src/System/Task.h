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
    CTask* getParent() { return m_pParent; }           // PARTIAL: added for System/Component
    BOOL isInactive() { return m_nInfo & 0x8008; }     // PARTIAL: added for System/Component
    BOOL isInvisible() { return m_nInfo & 0x8010; }    // PARTIAL: added for System/Component
    void out() {
        if (!isOut()) {
            m_nInfo |= 0x8000;
            onOut();
        }
    }
    void setPriority(INT pri) { m_nProcPrio = pri; } // PARTIAL: added for System/Graphic/TexCtrl
    CCoord& Crd(UINT nIndex); // PARTIAL: added for Game/Atel/AtelCamera
    const FMATRIX& getWorld(UINT nIndex) const; // PARTIAL: added for System/Graphic/PathCamera
    void addCoord(UINT nNum);                               // PARTIAL: added for Game/ModelObj
    INT getCoordNum() const;                                // PARTIAL: added for Game/ModelObj
    BOOL isClippedLastTime() { return m_cLastClip < 0; }    // PARTIAL: added for Game/ModelObj
    // PARTIAL: added for Game/Atel/AtelCamera
    void ignorePause(BOOL bFlag) {
        if (bFlag) {
            m_nInfo |= 0x80;
        } else {
            m_nInfo &= ~0x80;
        }
    }
    void ignorePlaySpeed(BOOL bFlag) {
        if (bFlag) {
            m_nInfo |= 0x100;
        } else {
            m_nInfo &= ~0x100;
        }
    }

protected: // PARTIAL: added for Game/Bullet/BulletManager
    BOOL declareSysTask(UINT nID);
    void adjustSelfCoord(); // PARTIAL: added for System/Graphic/Camera
    friend class CTaskSecondary; // PARTIAL: added for System/Graphic/Camera (m_nInfo)
    CTaskSLCList& getChildren() { return m_listChildren; }
};

// PARTIAL: added for Game/Atel/AtelCamera
class CTaskSecondary : public CTask {
public:
    // PARTIAL: inline body added for System/Graphic/Camera
    CTaskSecondary(CTask* pParent, INT nProcPrio = 0x4000) : CTask(pParent, nProcPrio) { m_nInfo |= 2; }
};

#endif /* SYSTEM_TASK_H */
