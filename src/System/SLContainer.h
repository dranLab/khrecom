// ISLCManagerT, SLContainerT and CSLCListT (single-linked containers of a pool manager)
#ifndef SYSTEM_SLCONTAINER_H
#define SYSTEM_SLCONTAINER_H

#include "khtypes.h"

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
    // PARTIAL: added for Game/Enemy/BattleEnemyManager
    SLContainerT<T>* getHead() const { return m_pHeadContainer; }
    int getSize() const { return m_nListSize; }
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

#endif /* SYSTEM_SLCONTAINER_H */
