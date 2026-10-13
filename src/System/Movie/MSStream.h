#ifndef SYSTEM_MOVIE_MSSTREAM_H
#define SYSTEM_MOVIE_MSSTREAM_H

#include "khtypes.h"

class CMSStream {
private:
    BYTE* m_pbyIopBuffer;
    INT m_iLength;
    INT m_iHost;
    BYTE m_bPauseStatus;

public:
    CMSStream();
    virtual ~CMSStream();
    BOOL IsEOF();
    BOOL Open(const CHAR* pszFile, INT nChunkSize);
    UINT Read(void* pvBuffer);
    void Close();
    INT Pause();
    INT Resume();
    static BOOL IsRunning();
};

#endif // SYSTEM_MOVIE_MSSTREAM_H
