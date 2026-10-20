#include "include_asm.h"
#include "System/LinkedBuf.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new[](size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void operator delete[](void* mem) {
    com_free(mem);
}

namespace {
CPtrList* s_pFreeListTbl;
CPtrList* s_pFreeListCurrent;
BOOL s_bEnableFreeList;
}

CLinkedBuf::CLinkedBuf(size_t nInSize) {
    if (nInSize) {
        allocate(nInSize);
    } else {
        m_pBuf = NULL;
    }
}

CLinkedBuf::~CLinkedBuf() {
    release();
}

void* CLinkedBuf::allocate(size_t nInSize) {
    m_pBuf = com_malloc_d(nInSize, __FILE__ "(43)", (UINT)-1);
}

void* CLinkedBuf::allocAligned(size_t nInAlign, size_t nInSize) {
    m_pBuf = com_memalign_d(nInAlign, nInSize, __FILE__ "(59)", (UINT)-1);
}

void CLinkedBuf::release() {
    if (m_pBuf) {
        com_free(m_pBuf);
    }
    m_pBuf = NULL;
}

void CDelayedFree::initFreeList() {
    s_pFreeListTbl = new (__FILE__ "(87)") CPtrList[2];
    s_pFreeListCurrent = NULL;
    s_bEnableFreeList = TRUE;
}

void CDelayedFree::disposeFreeList() {
    flipFreeList(0);
    flipFreeList(1);
    delete[] s_pFreeListTbl;
    s_pFreeListTbl = NULL;
    s_pFreeListCurrent = NULL;
    s_bEnableFreeList = FALSE;
}

#line 106
void CDelayedFree::addFreePtr(CLinkedPtr& inPtr) {
    if (!(s_pFreeListCurrent && s_bEnableFreeList))
        AssertApp(__FILE__, __LINE__, "s_pFreeListCurrent && s_bEnableFreeList", 0);
    s_pFreeListCurrent->pushBack(inPtr);
}

void CDelayedFree::lockFreeList() {
    s_bEnableFreeList = FALSE;
}

void CDelayedFree::unlockFreeList() {
    s_bEnableFreeList = TRUE;
}

BOOL CDelayedFree::isFreeListEmpty() {
    CPtrList* pList;
    INT i;
    for (i = 0; i < 2; i++) {
        pList = &s_pFreeListTbl[i];
        if (pList->getListSize()) return FALSE;
    }
    return TRUE;
}

void CDelayedFree::flipFreeList(INT nID) {
    BOOL bLocked = !s_bEnableFreeList;
    if (!bLocked) lockFreeList();
    CPtrList*& rCurrent = s_pFreeListCurrent;
    rCurrent = &s_pFreeListTbl[nID & 1];
    /* the list iteration macro form (next-next from __pn): one getHead, two getNext inlines in the ref */
    for (CLinkedPtr *__p = s_pFreeListCurrent->getHead(), *__pn = __p ? __p->getNext() : NULL; __p;
         __p = __pn, __pn = __pn ? __pn->getNext() : NULL) {
        delete __p;
    }
    s_pFreeListCurrent->resetParam();
    if (!bLocked) unlockFreeList();
}

// SOURCE DATA: .rodata .bss
