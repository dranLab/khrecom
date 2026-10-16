// PARTIAL: added for System/Movie/AudioDec
#ifndef SYSTEM_BUF_H
#define SYSTEM_BUF_H

#include "khtypes.h"
#include "System/HeapMan.h"

class CBuf {
private:
    BYTE* m_pBuf;
    INT m_nBufSize;
    BOOL m_bManaged;

public:
    CBuf() : m_pBuf(0), m_nBufSize(0), m_bManaged(0) {}
    virtual ~CBuf() { releaseBuf(0); }
    BYTE* _allocBuf(INT nSize, UINT nAlign); // PARTIAL: added for System/Buf
    BYTE* allocBuf(INT nSize);
    BYTE* allocBuf_A64(INT nSize);
    BYTE* allocBuf_A128(INT nSize);
    BYTE* allocBuf_A256(INT nSize);
    void attachBuf(BYTE* pInBuf, INT nInSize); // PARTIAL: added for System/Buf
    void releaseBuf(BOOL bDelay);
    void clear();
    void copyTo(CBuf& outBuf); // PARTIAL: added for System/VramMan
    void copyTo(CBuf& outBuf) const;
    BYTE* getBufPtr() { return m_pBuf; }
    const BYTE* getBufPtr() const { return m_pBuf; }
    INT getBufSize() const { return m_nBufSize; }
    BOOL isManaged() const { return m_bManaged; }
};

#endif // SYSTEM_BUF_H
