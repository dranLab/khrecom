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

class CDelayedFree {
public:
    static void addFreePtr(CLinkedPtr& inPtr);
    static void addFreeMallocPtr(void* pInPtr); /* name unknown (inline) */
};

#line 130
inline void CDelayedFree::addFreeMallocPtr(void* pInPtr) {
    CMallocPtr* pPtr = new (com_malloc_d(sizeof(CMallocPtr), __FILE__ "(131)", ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; }))) CMallocPtr(pInPtr);
    if (!pPtr) AssertApp(__FILE__, __LINE__, "pPtr", 0);
    addFreePtr(*pPtr);
}

#endif /* SYSTEM_LINKEDBUF_H */
