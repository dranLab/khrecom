#ifndef SYSTEM_FIFOFPL_H
#define SYSTEM_FIFOFPL_H

#include "khtypes.h"

class CFifoFpl {
public:
    struct _Tag {
        enum {
            _FREE_FLAG = 0x80000000,
            _LINK_TERMINATOR = -1
        };
        UINT _nID;
        UINT _nLink;
    };

private:
    void* m_pPoolBuf;
    INT m_nFlag;
    UINT m_nBlockCountMax;
    UINT m_nBlockSizeShift;
    INT m_nAvailableBlockHead;
    INT m_nAvailableBlockTail;
    UINT m_nAvailableBlockCount;

public:
    CFifoFpl();
    virtual ~CFifoFpl();
    void initialize(UINT nBlockSize, UINT nBlockCount, void* pMem);
    void* allocBlock();
    void releaseBlock(void* pBlock);
    void* getValidActiveBlock(UINT nID);
    BOOL isValidActiveBlockID(UINT nID) const;
    void debugDump(UINT nStartIndex, UINT nEndIndex);
};

#endif /* SYSTEM_FIFOFPL_H */
