#ifndef SYSTEM_LINKEDBUF_H
#define SYSTEM_LINKEDBUF_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/SJoint.h"






class CLinkedPtr : public CSJointT<CLinkedPtr> {
private:
    void* m_pBuf; /* 0x8 */

public:
    CLinkedPtr(void* pInPtr) { setPtr(pInPtr); }
    virtual ~CLinkedPtr() {}
    void setPtr(void* pInPtr); /* MATCH: defined below: not inlined into the constructors */
    void* getPtr() { return m_pBuf; }
};

inline void CLinkedPtr::setPtr(void* pInPtr) {
    m_pBuf = pInPtr;
}

class CMallocPtr : public CLinkedPtr {
public:
    CMallocPtr(void* pInPtr) : CLinkedPtr(pInPtr) {}
    virtual ~CMallocPtr() {
        if (getPtr()) com_free(getPtr());
    }
};

typedef CSJointListT<CLinkedPtr> CPtrList;

class CLinkedBuf : public CSJointT<CLinkedBuf> {
private:
    void* m_pBuf; /* 0x8 */

public:
    CLinkedBuf(size_t nInSize);
    virtual ~CLinkedBuf();
    void* allocate(size_t nInSize);
    void* allocAligned(size_t nInAlign, size_t nInSize);
    void release();
    void* getBuf() { return m_pBuf; }
    BOOL isEmpty() const { return m_pBuf == NULL; }
};

class CDelayedFree {
    friend class CPackMan; /* MATCH: System/Graphic/PackMan sets up, flips and disposes the free lists (private) */

public:
    static void addFreePtr(CLinkedPtr& inPtr);
    static void lockFreeList();
    static void unlockFreeList();
    static BOOL isFreeListEmpty();

private:
    static void initFreeList();
    static void disposeFreeList();
    static void flipFreeList(INT nID);
};

/* MATCH: path override for Game/AttachCard and Game/Prize ("System/LinkedBuf.h") */
#ifdef LINKEDBUF_H_PATH
#line 1 LINKEDBUF_H_PATH
#endif
/* MATCH: name unknown (inlined everywhere; the ref's CDelayedFree has no such member) */
#line 130
inline void DelayedFree(void* pInPtr) {
    CMallocPtr* pPtr = new KH_AT("(131)") CMallocPtr(pInPtr);
    KH_DEBUG(if (!pPtr) AssertApp(__FILE__, __LINE__, "pPtr", 0);)
    CDelayedFree::addFreePtr(*pPtr);
}

template <class T>
class CNewPtrT : public CLinkedPtr {
private:
    BOOL m_bArray; /* 0xc */

public:
    CNewPtrT(T* pInBuf, BOOL bInArray) : CLinkedPtr(pInBuf) { m_bArray = bInArray; }
    virtual ~CNewPtrT() {
        T* p = (T*)getPtr();
        if (p) {
            if (m_bArray) delete[] p;
            else delete p;
        }
    }
};

#line 140
template <class T>
void DelayedDelete(T* pInBuf, BOOL bInArray) {
    if (pInBuf) {
        CNewPtrT<T>* pPtr = new KH_AT("(141)") CNewPtrT<T>(pInBuf, bInArray);
#line 142
        KH_DEBUG(if (!pPtr) AssertApp(__FILE__, __LINE__, "pPtr", 0);)
        CDelayedFree::addFreePtr(*pPtr);
    }
}

#endif /* SYSTEM_LINKEDBUF_H */
