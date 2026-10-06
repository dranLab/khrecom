#include "Spad.h"

namespace SPAD {

static UINT _ALLOC_PTR;

void* allocSpad(UINT nInSize) {
    void* pRet;
    UINT nTail;

    pRet = 0;
    nTail = _ALLOC_PTR + nInSize;
    if (nTail <= 0x70004000) {
        pRet = (void*)_ALLOC_PTR;
        _ALLOC_PTR = nTail;
    }
    return pRet;
}

void freeSpad(UINT nInSize) {
    _ALLOC_PTR -= nInSize;
}

void freeAllSpad() {
    _ALLOC_PTR = 0x70000000;
}

}

// SOURCE DATA: .bss
