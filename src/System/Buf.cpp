#include "System/LinkedBuf.h"
#include "System/Buf.h"

#line 22
BYTE* CBuf::_allocBuf(INT nSize, UINT nAlign) {
    BYTE* pBuf;
    if (!(__null == m_pBuf)) AssertApp(__FILE__, __LINE__, "__null == m_pBuf", 0);
    pBuf = nAlign ? (BYTE*)com_memalign_d(nAlign, nSize, __FILE__ "(25)", (UINT)-1) : (BYTE*)com_malloc_d(nSize, __FILE__ "(25)", (UINT)-1);
    m_nBufSize = nSize;
    m_bManaged = pBuf != NULL;
    m_pBuf = pBuf;
    return pBuf;
}

BYTE* CBuf::allocBuf(INT nSize) {
    return _allocBuf(nSize, 0);
}

BYTE* CBuf::allocBuf_A64(INT nSize) {
    return _allocBuf(nSize, 64);
}

BYTE* CBuf::allocBuf_A128(INT nSize) {
    return _allocBuf(nSize, 128);
}

BYTE* CBuf::allocBuf_A256(INT nSize) {
    return _allocBuf(nSize, 256);
}

#line 67
void CBuf::attachBuf(BYTE* pInBuf, INT nInSize) {
    if (!((0) == m_bManaged)) AssertApp(__FILE__, __LINE__, "(0) == m_bManaged", 0);
    m_pBuf = pInBuf;
    m_nBufSize = nInSize;
}

void CBuf::releaseBuf(BOOL bDelay) {
    if (m_bManaged && m_pBuf) {
        if (bDelay) {
            DelayedFree(m_pBuf);
        } else {
            com_free(m_pBuf);
        }
    }
    m_pBuf = NULL;
    m_nBufSize = 0;
    m_bManaged = FALSE;
}

void CBuf::clear() {
    m_pBuf = NULL;
    m_nBufSize = 0;
    m_bManaged = FALSE;
}

#line 103
void CBuf::copyTo(CBuf& outBuf) {
    if (!((0) == outBuf.m_bManaged)) AssertApp(__FILE__, __LINE__, "(0) == outBuf.m_bManaged", 0);
    if (this != &outBuf) {
        outBuf.m_pBuf = m_pBuf;
        outBuf.m_nBufSize = m_nBufSize;
        outBuf.m_bManaged = m_bManaged;
        m_bManaged = FALSE;
    }
}

#line 117
void CBuf::copyTo(CBuf& outBuf) const {
    if (!(((0) == outBuf.m_bManaged) && ((0) == m_bManaged)))
        AssertApp(__FILE__, __LINE__, "((0) == outBuf.m_bManaged) && ((0) == m_bManaged)", 0);
    if (this != &outBuf) {
        outBuf.m_pBuf = m_pBuf;
        outBuf.m_nBufSize = m_nBufSize;
        outBuf.m_bManaged = m_bManaged;
    }
}

// SOURCE DATA: .rodata
