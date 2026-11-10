// PARTIAL: added for Game/MessageManager (line numbers are the original ones where they are known)
#ifndef GAME_POOLLIST_H
#define GAME_POOLLIST_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"



/* Fixed-size pool of list entries T (with pPrev/pNext links), kept in a doubly linked list between two sentinels. */
template <class T>
class CPoolList {
protected:
    INT m_iSize;          /* 0x00 */
    T* m_pListTbl;        /* 0x04 */
    T** m_ppListPtrTbl;   /* 0x08 */
    T m_Top;              /* 0x0c */
    T m_Tail;
    INT m_iReadPtr;
    INT m_iWritePtr;
    INT m_iNowCount;

public:
    CPoolList(INT inSize) {
        m_iSize = 0;
        m_pListTbl = NULL;
        m_ppListPtrTbl = NULL;
        m_iNowCount = 0;
        m_iWritePtr = 0;
        m_iReadPtr = 0;
        init(inSize);
    }
    virtual ~CPoolList() { release(); }
    T* getList() {
        if (m_iNowCount >= m_iSize) return NULL;
        T* p = m_ppListPtrTbl[m_iReadPtr];
        if (p == NULL) return NULL;
        m_ppListPtrTbl[m_iReadPtr] = NULL;
        if (++m_iReadPtr >= m_iSize) m_iReadPtr = 0;
        m_iNowCount++;
        m_Tail.pPrev->pNext = p;
        p->pPrev = m_Tail.pPrev;
        p->pNext = &m_Tail;
        m_Tail.pPrev = p;
        return p;
    }

protected:
#line 59 "src/Game/PoolList.h"
    void init(INT inSize) {
        INT i;
        m_iSize = inSize;
        m_pListTbl = new KH_AT("(62)") T[inSize];
        m_ppListPtrTbl = new KH_AT("(63)") T*[inSize];
        KH_DEBUG(if (!m_pListTbl) AssertApp(__FILE__, __LINE__, "m_pListTbl", 0);)
        KH_DEBUG(if (!m_ppListPtrTbl) AssertApp(__FILE__, __LINE__, "m_ppListPtrTbl", 0);)
        for (i = 0; i < inSize; i++) {
            m_ppListPtrTbl[i] = &m_pListTbl[i];
        }
        erase();
    }
    void release() {
        if (m_pListTbl) com_free(m_pListTbl);
        m_pListTbl = NULL;
        if (m_ppListPtrTbl) com_free(m_ppListPtrTbl);
        m_ppListPtrTbl = NULL;
        m_iNowCount = 0;
        m_iWritePtr = 0;
        m_iReadPtr = 0;
        m_iSize = 0;
    }
    void erase() {
        m_Top.pNext = &m_Tail;
        m_Tail.pPrev = &m_Top;
        m_iNowCount = 0;
        m_iWritePtr = 0;
        m_iReadPtr = 0;
        m_Top.pPrev = NULL;
        m_Tail.pNext = NULL;
    }
#line 88
    void releaseList(T* inList) {
        inList->pPrev->pNext = inList->pNext;
        inList->pNext->pPrev = inList->pPrev;
#line 93
        KH_DEBUG(if (!(m_ppListPtrTbl[m_iWritePtr] == __null)) AssertApp(__FILE__, __LINE__, "m_ppListPtrTbl[m_iWritePtr] == __null", 0);)
        m_ppListPtrTbl[m_iWritePtr] = inList;
        if (++m_iWritePtr >= m_iSize) m_iWritePtr = 0;
        m_iNowCount--;
    }
};

#endif /* GAME_POOLLIST_H */
