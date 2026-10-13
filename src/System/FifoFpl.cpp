#include "System/FifoFpl.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"
#include "System/Debug.h"

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

CFifoFpl::CFifoFpl() {
    m_pPoolBuf = NULL;
    m_nFlag = 0;
    m_nBlockCountMax = 0;
    m_nBlockSizeShift = 0;
}

CFifoFpl::~CFifoFpl() {
    void* pv = m_pPoolBuf;
    if (pv) {
        if (m_nFlag & 1) {
            com_free(pv);
        }
    }
}

void CFifoFpl::initialize(UINT nBlockSize, UINT nBlockCount, void* pMem) {
    UINT n;
    UINT nShift;
    BYTE* p;
    _Tag* pTag;

#line 40
    ASSERT(__null == m_pPoolBuf);
    m_nFlag = 0;

    n = nBlockSize;
    nShift = 0;
    while (n >>= 1) {
        nShift++;
    }
#line 48
    ASSERT((3 <= nShift) && ((UINT)(1<<nShift) == nBlockSize));
    m_nBlockSizeShift = nShift;

    n = nBlockCount;
    nShift = 0;
    while (n >>= 1) {
        nShift++;
    }
#line 54
    ASSERT((UINT)(1<<nShift) == nBlockCount);

    if (pMem) {
        m_pPoolBuf = pMem;
    } else {
#line 60
        m_pPoolBuf = com_malloc_d(nBlockSize * nBlockCount, __FILE__ "(60)", -1);
        m_nFlag |= 1;
    }
#line 63
    ASSERT(m_pPoolBuf);

    pTag = (_Tag*)m_pPoolBuf;
    m_nBlockCountMax = nBlockCount;
    m_nAvailableBlockCount = nBlockCount;
    p = (BYTE*)pTag;
    for (n = 0; n < nBlockCount; n++) {
        pTag = (_Tag*)p;
        pTag->_nID = n | _Tag::_FREE_FLAG;
        pTag->_nLink = n + 1;
        p += nBlockSize;
    }
    pTag->_nLink = _Tag::_LINK_TERMINATOR;
    m_nAvailableBlockTail = n - 1;
    m_nAvailableBlockHead = 0;
}

void* CFifoFpl::allocBlock() {
    INT nAvailableBlock;
    BYTE* p;

    if (m_nAvailableBlockCount == 0) {
        return NULL;
    }
    nAvailableBlock = m_nAvailableBlockHead;
#line 92
    ASSERT(m_pPoolBuf && (0 <= nAvailableBlock));
    p = (BYTE*)m_pPoolBuf;
    p += nAvailableBlock << m_nBlockSizeShift;
    _Tag* pTag = (_Tag*)p;
#line 99
    ASSERT(pTag->_nID & _Tag::_FREE_FLAG);
    m_nAvailableBlockHead = pTag->_nLink;
    pTag->_nID &= ~_Tag::_FREE_FLAG;
    m_nAvailableBlockCount--;
    if (m_nAvailableBlockHead == _Tag::_LINK_TERMINATOR) {
#line 110
        ASSERT((0==m_nAvailableBlockCount) && ((UINT)m_nAvailableBlockTail==(pTag->_nID&(m_nBlockCountMax-1))));
        m_nAvailableBlockTail = _Tag::_LINK_TERMINATOR;
    }
    return p;
}

void CFifoFpl::releaseBlock(void* pBlock) {
    UINT nID;
    UINT nIDMask;
    UINT nIndex;
    UINT nCheckIndex;
    BYTE* p;

#line 124
    ASSERT(m_pPoolBuf);
    if (!pBlock) {
        TRACE("NULL \x83" "A\x83h\x83\x8c\x83X\x82\xf0\x8aJ\x95\xfa\x82\xb5\x82\xe6\x82\xa4\x82\xc6\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb7\n");
        return;
    }
    p = (BYTE*)pBlock;
    nIDMask = m_nBlockCountMax - 1;
    nID = ((_Tag*)p)->_nID;
    nIndex = nID & nIDMask;
    if (nID & _Tag::_FREE_FLAG) {
        TRACE("\x82\xb7\x82\xc5\x82\xc9\x8aJ\x95\xfa\x8d\xcf\x82\xdd\x82\xcc\x83u\x83\x8d\x83" "b\x83N\x82\xf0\x8aJ\x95\xfa\x82\xb5\x82\xe6\x82\xa4\x82\xc6\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb7\n");
        return;
    }
    nCheckIndex = (UINT)(p - (BYTE*)m_pPoolBuf) >> m_nBlockSizeShift;
    if (nCheckIndex != nIndex) {
        if (nCheckIndex >= m_nBlockCountMax) {
            TRACE("\x8a\xc7\x97\x9d\x8aO\x82\xcc\x83u\x83\x8d\x83" "b\x83N\x82\xf0\x8aJ\x95\xfa\x82\xb5\x82\xe6\x82\xa4\x82\xc6\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb7\n");
        } else {
            TRACE("\x83u\x83\x8d\x83" "b\x83N\x82\xcc\x82h\x82" "c\x8f\xee\x95\xf1\x82\xaa\x89\xf3\x82\xb3\x82\xea\x82\xc4\x82\xa2\x82\xe9\x82\xa9\x82\xe0\x81`\n");
        }
        return;
    }
    nID += m_nBlockCountMax;
    nID |= _Tag::_FREE_FLAG;
    ((_Tag*)p)->_nID = nID;
    if (m_nAvailableBlockTail != _Tag::_LINK_TERMINATOR) {
        ((_Tag*)((BYTE*)m_pPoolBuf + (m_nAvailableBlockTail << m_nBlockSizeShift)))->_nLink = nIndex;
        m_nAvailableBlockTail = nIndex;
        ((_Tag*)p)->_nLink = _Tag::_LINK_TERMINATOR;
    } else {
#line 174
        ASSERT((0==m_nAvailableBlockCount) && (_Tag::_LINK_TERMINATOR==m_nAvailableBlockHead));
        m_nAvailableBlockTail = m_nAvailableBlockHead = nIndex;
    }
    m_nAvailableBlockCount++;
}

void* CFifoFpl::getValidActiveBlock(UINT nID) {
    BYTE* p;
    UINT nIDMask;

    if (nID & _Tag::_FREE_FLAG) {
        return NULL;
    }
#line 191
    ASSERT(m_pPoolBuf);
    nIDMask = m_nBlockCountMax - 1;
    p = (BYTE*)m_pPoolBuf + ((nID & nIDMask) << m_nBlockSizeShift);
    if (((_Tag*)p)->_nID != nID) {
        return NULL;
    }
    return p;
}

BOOL CFifoFpl::isValidActiveBlockID(UINT nID) const {
    BYTE* p;
    UINT nIDMask;

#line 208
    ASSERT(m_pPoolBuf);
    nIDMask = m_nBlockCountMax - 1;
    p = (BYTE*)m_pPoolBuf;
    p += (nID & nIDMask) << m_nBlockSizeShift;
    return !(nID & _Tag::_FREE_FLAG) && ((_Tag*)p)->_nID == nID;
}

void CFifoFpl::debugDump(UINT nStartIndex, UINT nEndIndex) {
    BYTE* p;
    INT nBlockSize;
    UINT i;
    UINT n;

    if (nEndIndex == 0 || m_nBlockCountMax < nEndIndex) {
        nEndIndex = m_nBlockCountMax;
    }
    TRACE("\n*** CFifoFpl Debug Dump Start ***\n");
    TRACE("    Buffer-Address : [%07X]\n", m_pPoolBuf);
    TRACE("    Block-Head     : [#%d]\n", m_nAvailableBlockHead);
    TRACE("    Block-Tail     : [#%d]\n", m_nAvailableBlockTail);
    TRACE("    Available      : [#%d]", m_nAvailableBlockCount);
    p = (BYTE*)m_pPoolBuf;
    if (p) {
        nBlockSize = 1 << m_nBlockSizeShift;
        for (i = nStartIndex, n = 0; i < nEndIndex; i++, n++, p += nBlockSize) {
            if ((n & 1) == 0) {
                TRACE("\n   ");
            }
            TRACE(" [%2d]:[ID=%8X][L=%-3d]", i, ((_Tag*)p)->_nID, ((_Tag*)p)->_nLink);
        }
        TRACE("\n**** CFifoFpl Debug Dump End ****\n\n");
    }
}
// SOURCE DATA: .rodata
