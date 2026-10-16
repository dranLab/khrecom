// PARTIAL: added for System/Buf
#ifndef SYSTEM_LINKEDBUF_H
#define SYSTEM_LINKEDBUF_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/SJoint.h"

inline void* operator new(size_t, void* p) throw() { return p; }

class CLinkedPtr : public CSJointT<CLinkedPtr> {
protected:
    void* m_pPtr; /* 0x8 */

public:
    CLinkedPtr(void* pInPtr) { setPtr(pInPtr); }
    virtual ~CLinkedPtr() {}
    void setPtr(void* pInPtr); /* defined below: not inlined into the constructors */
};

inline void CLinkedPtr::setPtr(void* pInPtr) {
    m_pPtr = pInPtr;
}

class CMallocPtr : public CLinkedPtr {
public:
    CMallocPtr(void* pInPtr) : CLinkedPtr(pInPtr) {}
    virtual ~CMallocPtr() {
        if (m_pPtr) com_free(m_pPtr);
    }
};

typedef CSJointListT<CLinkedPtr> CPtrList;

class CLinkedBuf : public CSJointT<CLinkedBuf> { // PARTIAL: added for System/LinkedBuf
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
public:
    static void initFreeList(); // PARTIAL: added for System/LinkedBuf
    static void disposeFreeList();
    static void addFreePtr(CLinkedPtr& inPtr);
    static void lockFreeList();
    static void unlockFreeList();
    static BOOL isFreeListEmpty();
    static void flipFreeList(INT nID);
    static void addFreeMallocPtr(void* pInPtr); /* name unknown (inline) */
};

#line 130
inline void CDelayedFree::addFreeMallocPtr(void* pInPtr) {
    CMallocPtr* pPtr = new (com_malloc_d(sizeof(CMallocPtr), __FILE__ "(131)", ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; }))) CMallocPtr(pInPtr);
    if (!pPtr) AssertApp(__FILE__, __LINE__, "pPtr", 0);
    addFreePtr(*pPtr);
}

#endif /* SYSTEM_LINKEDBUF_H */
