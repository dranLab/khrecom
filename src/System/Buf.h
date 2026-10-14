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
    BYTE* allocBuf(INT nSize);
    BYTE* allocBuf_A64(INT nSize);
    BYTE* allocBuf_A128(INT nSize);
    BYTE* allocBuf_A256(INT nSize);
    void releaseBuf(INT n);
    void copyTo(CBuf& dst); // PARTIAL: added for System/VramMan
    BYTE* getBufPtr() { return m_pBuf; }
    const BYTE* getBufPtr() const { return m_pBuf; }
    INT getBufSize() const { return m_nBufSize; }
    BOOL isManaged() const { return m_bManaged; }
};

#endif // SYSTEM_BUF_H
