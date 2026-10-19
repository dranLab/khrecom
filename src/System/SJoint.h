// PARTIAL: added for System/Graphic/2d/2DBase
#ifndef SYSTEM_SJOINT_H
#define SYSTEM_SJOINT_H

#include "khtypes.h"

/* Intrusive singly linked list node (original header name unknown). */
template <class T> class CSJointListT; // PARTIAL: added for System/Ctd/CtdMan

template <class T>
class CSJointT {
    friend class CSJointListT<T>; // PARTIAL: added for System/Ctd/CtdMan
private:
    CSJointT<T>* m_pNext;
public:
    CSJointT() : m_pNext(0) {}
    virtual ~CSJointT() {}
    T* getNext() { return (T*)m_pNext; }
protected:
    void setNext(CSJointT<T>* pNext) { m_pNext = pNext; }
};

// PARTIAL: added for System/Ctd/CtdMan
/* Intrusive singly linked list of CSJointT<T> nodes. */
template <class T>
class CSJointListT {
private:
    T* m_pHead;
    T* m_pTail;
    INT m_nListSize;
public:
    CSJointListT() { resetParam(); }
    virtual ~CSJointListT() {}
    T* getHead() { return m_pHead; }
    T* getTail() { return m_pTail; }
    INT getListSize() { return m_nListSize; }
    void pushBack(T& inEntry) {
        if (inEntry.getNext() == NULL) {
            if (m_pHead == NULL) m_pHead = &inEntry;
            if (m_pTail != NULL) m_pTail->setNext(&inEntry);
            m_pTail = &inEntry;
            m_nListSize++;
        }
    }
    // PARTIAL: added for System/VramMan
    void popFront() {
        if (m_pHead != NULL) {
            T* pNext = m_pHead->getNext();
            m_pHead->setNext(NULL);
            m_pHead = pNext;
            if (m_pHead == NULL) m_pTail = NULL;
            m_nListSize--;
        }
    }
    BOOL remove(T& inEntry) {
        T* pPrevEntry = NULL;
        T* pEntry = m_pHead;
        while (pEntry != NULL) {
            T* pNextEntry = pEntry->getNext();
            if (&inEntry == pEntry) {
                if (pPrevEntry != NULL) pPrevEntry->setNext(pNextEntry);
                else m_pHead = pNextEntry;
                if (&inEntry == m_pTail) m_pTail = pPrevEntry;
                inEntry.setNext(NULL);
                m_nListSize--;
                return TRUE;
            }
            pPrevEntry = pEntry;
            pEntry = pNextEntry;
        }
        return FALSE;
    }
    // PARTIAL: added for Game/Collision/CollAttack
    BOOL hasMember(T& inEntry) {
        T* pEntry = m_pHead;
        while (pEntry != NULL) {
            T* pNextEntry = pEntry->getNext();
            if (&inEntry == pEntry) return TRUE;
            pEntry = pNextEntry;
        }
        return FALSE;
    }
    // PARTIAL: added for Game/Collision/CollDisp
    void removeAll() {
        T* pEntry = m_pHead;
        while (pEntry != NULL) {
            T* pNextEntry = pEntry->getNext();
            pEntry->setNext(NULL);
            pEntry = pNextEntry;
        }
        resetParam();
    }
    void resetParam() {
        m_pHead = NULL;
        m_pTail = NULL;
        m_nListSize = 0;
    }
};

#endif /* SYSTEM_SJOINT_H */
