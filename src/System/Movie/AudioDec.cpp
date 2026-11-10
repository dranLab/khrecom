#include "System/Movie/AudioDec.h"
#include "System/Buf.h"
#include "System/HeapMan.h"
#include "System/MainApp.h"
#include "System/Debug.h"
#include <string.h>
#include "include_asm.h"




typedef struct {
    unsigned char* p_at3buf;
    short int* p_pcmbuf;
    unsigned int at3decparam;
    void* p_at3decwork;
    unsigned int minusvol;
} at3decinfo;

struct sceSifDmaData {
    unsigned int data;
    unsigned int addr;
    unsigned int size;
    unsigned int mode;
};

extern "C" {
void hncAt3dec_Init(at3decinfo* info); /* MATCH: void here (lib/hncAt3dec returns int): no $v0 set by the call */
int hncAt3dec_Idle(at3decinfo* info);
int TsSdMpegSetVolume(int vol);
int TsSdCheckDriverSetParam2();
int TsSdMpegTransBlockStatus(int a, int b);
int* TsSdMpegTransBlockStop();
void TsSdMpegTransBlock(int addr, int size, int pos);
int TsSdMpegTransVoice(int addr, int mode, int size);
int FlushCache(int operation);
unsigned int sceSifSetDma(sceSifDmaData* sdd, int len);
int sceSifDmaStat(unsigned int id);
int DelayThread(int usec);
}

extern INT NMSVIDEO_IOP_ADDR_FOR_AUDIO_ZEROBUF;
extern INT NMSVIDEO_IOP_ADDR_FOR_AUDIO_DECODE;

template <class T> static inline T& _ref(T& v) { return v; }

struct SpuStreamHeader {
    char id[4];
    INT size;
    INT type;
    INT rate;
    INT ch;
    INT interSize;
    INT loopStart;
    INT loopEnd;
};

struct SpuStreamBody {
    char id[4];
    INT size;
};

static INT _sendToIOP(INT nDstIopAddr, const BYTE* pSrc, INT nSize);
static INT _sendToIOP2Area(INT pd0, INT nD0, INT pd1, INT nD1, BYTE* ps0, INT nS0, BYTE* ps1, INT nS1);
static int _copy2area(BYTE* pd0, INT nD0, BYTE* pd1, INT nD1, const BYTE* pSrc, INT nSize);

class CAudioDecImpl {
private:
    INT m_nState;
    SpuStreamHeader m_SSHeader;
    SpuStreamBody m_SSBody;
    INT m_nHeaderCount;
    CBuf m_PcmBuf;
    CBuf m_ZeroBuf;
    CBuf m_At3Buf;
    INT m_nPcmPut;
    INT m_nPcmCount;
    INT m_nPcmTotalBytes;
    INT m_nAt3Put;
    INT m_nAt3Count;
    INT m_nAt3TotalBytes;
    at3decinfo m_at3info;
    INT m_nIopBufAddr;
    INT m_nIopBufSize;
    INT m_nIopLastPos;
    INT m_nIopPausePos;
    INT m_nTotalBytesSent;
    INT m_nIopZeroAddr;
    INT m_nIopZeroSize;
    INT m_nNoDataCount;

public:
    CAudioDecImpl();
    virtual ~CAudioDecImpl();

private:
    void beginPutPcm(BYTE*& p0, INT& nLen0, BYTE*& p1, INT& nLen1);
    void endPutPcm(INT nSize);
    void beginPutAt3(BYTE*& p0, INT& nLen0, BYTE*& p1, INT& nLen1);
    void endPutAt3(INT nSize);
    void decodeAt3(BOOL bAll);

public:
    INT putData(const BYTE* pSrc, INT nSize);
    BOOL isPreset() const;
    BOOL isStarted() const;
    BOOL isPause() const;
    INT sendToIOP();
    void reset();
    void pause();
    void resume();

private:
    void getIopEmptyArea(INT& pd0, INT& d0, INT& pd1, INT& d1, INT nPos);
};

static CAudioDecImpl* s_pAudioDec = NULL;

CAudioDecImpl::CAudioDecImpl()
    : m_nState(0), m_nHeaderCount(0), m_nPcmPut(0), m_nPcmCount(0), m_nPcmTotalBytes(0), m_nAt3Put(0),
      m_nAt3Count(0), m_nAt3TotalBytes(0), m_nIopLastPos(0), m_nIopPausePos(0), m_nTotalBytesSent(0),
      m_nNoDataCount(0) {
    m_PcmBuf.allocBuf_A64(0xC000);
    m_ZeroBuf.allocBuf_A64(0x800);
    m_At3Buf.allocBuf_A64(0x1200);
    m_at3info.p_pcmbuf = (short int*)com_malloc_d(0x1000, __FILE__ "(165)", -1);
    m_at3info.p_at3decwork = com_malloc_d(0x68CA, __FILE__ "(166)", -1);
    m_at3info.at3decparam = 1;
    m_at3info.minusvol = 0;

    hncAt3dec_Init(&m_at3info);
    _ref(m_nIopBufAddr) = NMSVIDEO_IOP_ADDR_FOR_AUDIO_DECODE;
    _ref(m_nIopBufSize) = 0x6000;
    m_nIopZeroAddr = NMSVIDEO_IOP_ADDR_FOR_AUDIO_ZEROBUF;
    m_nIopZeroSize = 0x800;
    TsSdMpegSetVolume(0);
    TsSdCheckDriverSetParam2();
    memset(m_ZeroBuf.getBufPtr(), 0, 0x800);
    _sendToIOP(NMSVIDEO_IOP_ADDR_FOR_AUDIO_ZEROBUF, m_ZeroBuf.getBufPtr(), 0x800);
}

CAudioDecImpl::~CAudioDecImpl() {
    pause();
    if (m_at3info.p_pcmbuf) {
        delete m_at3info.p_pcmbuf;
    }
    if (m_at3info.p_at3decwork) {
        delete (BYTE*)m_at3info.p_at3decwork;
    }
}

INT CAudioDecImpl::putData(const BYTE* pSrc, INT nSize) {
    BYTE* pd0;
    BYTE* pd1;
    INT nD0;
    INT nD1;
    INT nRet;

    pSrc += 4;
    nSize -= 4;
    if (nSize <= 0) {
        return 0;
    }
    beginPutAt3(pd0, nD0, pd1, nD1);
    nRet = _copy2area(pd0, nD0, pd1, nD1, pSrc, nSize);
    endPutAt3(nRet);
    decodeAt3(FALSE);
    return nRet > 0;
}

void CAudioDecImpl::beginPutPcm(BYTE*& p0, INT& nLen0, BYTE*& p1, INT& nLen1) {
    INT nLen = m_PcmBuf.getBufSize() - m_nPcmCount;
    if (m_PcmBuf.getBufSize() - m_nPcmPut >= nLen) {
        p0 = m_PcmBuf.getBufPtr() + m_nPcmPut;
        nLen0 = nLen;
        p1 = NULL;
        nLen1 = 0;
    } else {
        p0 = m_PcmBuf.getBufPtr() + m_nPcmPut;
        nLen0 = m_PcmBuf.getBufSize() - m_nPcmPut;
        p1 = m_PcmBuf.getBufPtr();
        nLen1 = nLen - nLen0;
    }
}

void CAudioDecImpl::endPutPcm(INT nSize) {
    m_nPcmPut = (m_nPcmPut + nSize) % m_PcmBuf.getBufSize();
    m_nPcmCount += nSize;
    m_nPcmTotalBytes += nSize;
}

void CAudioDecImpl::beginPutAt3(BYTE*& p0, INT& nLen0, BYTE*& p1, INT& nLen1) {
    INT nLen = m_At3Buf.getBufSize() - m_nAt3Count;
    if (m_At3Buf.getBufSize() - m_nAt3Put >= nLen) {
        p0 = m_At3Buf.getBufPtr() + m_nAt3Put;
        nLen0 = nLen;
        p1 = NULL;
        nLen1 = 0;
    } else {
        p0 = m_At3Buf.getBufPtr() + m_nAt3Put;
        nLen0 = m_At3Buf.getBufSize() - m_nAt3Put;
        p1 = m_At3Buf.getBufPtr();
        nLen1 = nLen - nLen0;
    }
}

void CAudioDecImpl::endPutAt3(INT nSize) {
    if (nSize && m_nState == 0) {
        m_nState = 1;
    }
    m_nAt3Put = (m_nAt3Put + nSize) % m_At3Buf.getBufSize();
    m_nAt3Count += nSize;
    m_nAt3TotalBytes += nSize;
}

void CAudioDecImpl::decodeAt3(BOOL bAll) {
    BYTE* pd0;
    BYTE* pd1;
    INT nD0;
    INT nD1;

    while (m_nAt3Count >= 0x180) {
        beginPutPcm(pd0, nD0, pd1, nD1);
        if (nD0 + nD1 < 0x1000) {
            break;
        }
        m_at3info.p_at3buf = m_At3Buf.getBufPtr()
            + (m_nAt3Put - m_nAt3Count + m_At3Buf.getBufSize()) % m_At3Buf.getBufSize();
        if (hncAt3dec_Idle(&m_at3info) == -1) {
            TRACE("Atrac-Decode : Failed...\n");
            return;
        }
        _copy2area(pd0, nD0, pd1, nD1, (BYTE*)m_at3info.p_pcmbuf, 0x1000);
        endPutPcm(0x1000);
        m_nAt3Count -= 0x180;
        if (!bAll) {
            break;
        }
    }
}

BOOL CAudioDecImpl::isPreset() const {
    return m_nTotalBytesSent >= m_nIopBufSize;
}

BOOL CAudioDecImpl::isStarted() const {
    return (UINT)m_nState >= 2;
}

BOOL CAudioDecImpl::isPause() const {
    return m_nState == 3;
}

INT CAudioDecImpl::sendToIOP() {
    INT pd0;
    INT pd1;
    INT nD0;
    INT nD1;
    BYTE* ps0;
    INT nCountAdj;
    INT nS0;
    INT nS1;
    INT nCountSent;

    switch (m_nState) {
    case 0:
    case 3:
        return 0;
    case 1:
        decodeAt3(TRUE);
        pd0 = m_nIopBufAddr + m_nTotalBytesSent % m_nIopBufSize;
        nD0 = m_nIopBufSize - m_nTotalBytesSent;
        pd1 = 0;
        nD1 = 0;
        break;
    case 2:
        decodeAt3(FALSE);
        {
            INT nPos = (TsSdMpegTransBlockStatus(0, 0) & 0xFFFFFF) - m_nIopBufAddr;
            getIopEmptyArea(pd0, nD0, pd1, nD1, nPos);
        }
        break;
    }

    nCountSent = 0;
    ps0 = m_PcmBuf.getBufPtr() + (m_nPcmPut - m_nPcmCount + m_PcmBuf.getBufSize()) % m_PcmBuf.getBufSize();
    nCountAdj = (m_nPcmCount / 0x800) * 0x800;
    nS0 = m_PcmBuf.getBufSize() - (ps0 - m_PcmBuf.getBufPtr());
    nS0 = (nCountAdj > nS0) ? nS0 : nCountAdj;
    nS1 = nCountAdj - nS0;
    if (nD0 + nD1 >= 0x800 && nS0 + nS1 >= 0x800) {
        nCountSent = _sendToIOP2Area(pd0, nD0, pd1, nD1, ps0, nS0, m_PcmBuf.getBufPtr(), nS1);
    }
    m_nPcmCount -= nCountSent;
    m_nTotalBytesSent += nCountSent;
    m_nIopLastPos = (m_nIopLastPos + nCountSent) % m_nIopBufSize;
    if (nCountSent != 0 || m_nState == 1) {
        m_nNoDataCount = 0;
    } else {
        m_nNoDataCount++;
    }
    return nCountSent;
}

void CAudioDecImpl::reset() {
    pause();
    m_nState = 0;
    m_nHeaderCount = 0;
    m_nPcmPut = 0;
    m_nPcmCount = 0;
    m_nPcmTotalBytes = 0;
    m_nTotalBytesSent = 0;
    m_nIopLastPos = 0;
    m_nIopPausePos = 0;
    m_nAt3Put = 0;
    m_nAt3Count = 0;
    m_nAt3TotalBytes = 0;
}

void CAudioDecImpl::pause() {
    if ((UINT)m_nState < 2 || m_nState == 3) {
        return;
    }
    m_nState = 3;
    TsSdMpegSetVolume(0);
    TsSdCheckDriverSetParam2();
    m_nIopPausePos = (*TsSdMpegTransBlockStop() & 0xFFFFFF) - m_nIopBufAddr;
    TRACE_USER(TGOTO, "*** Movie-Audio IOP pause-pos : %X ***\n", m_nIopPausePos);
    TsSdMpegTransVoice(m_nIopZeroAddr, 0x4000, m_nIopZeroSize);
}

void CAudioDecImpl::resume() {
    TsSdMpegSetVolume(0x7FFF);
    TsSdMpegTransBlock(m_nIopBufAddr, m_nIopBufSize / 0x800 * 0x800, m_nIopBufAddr + m_nIopPausePos);
    m_nState = 2;
}

void CAudioDecImpl::getIopEmptyArea(INT& pd0, INT& d0, INT& pd1, INT& d1, INT nPos) {
    INT nLen = (nPos + m_nIopBufSize - m_nIopLastPos - 0x800) % m_nIopBufSize;
    nLen = nLen / 0x800 * 0x800;
    if (m_nIopBufSize - m_nIopLastPos >= nLen) {
        pd0 = m_nIopBufAddr + m_nIopLastPos;
        d0 = nLen;
        pd1 = 0;
        d1 = 0;
    } else {
        pd0 = m_nIopBufAddr + m_nIopLastPos;
        d0 = m_nIopBufSize - m_nIopLastPos;
        pd1 = m_nIopBufAddr;
        d1 = nLen - d0;
    }
}

void CAudioDec::create() {
    s_pAudioDec = new KH_AT("(631)") CAudioDecImpl;
    if (!s_pAudioDec) {
        AssertApp(__FILE__, 632, "s_pAudioDec", 0);
    }
}

void CAudioDec::dispose() {
    if (s_pAudioDec) {
        delete s_pAudioDec;
    }
    s_pAudioDec = NULL;
}

INT CAudioDec::putData(const BYTE* pSrc, INT nSize) {
    if (s_pAudioDec) {
        return s_pAudioDec->putData(pSrc, nSize);
    }
    return 0;
}

BOOL CAudioDec::isOK() {
    if (s_pAudioDec) {
        return s_pAudioDec->isPreset();
    }
    return FALSE;
}

BOOL CAudioDec::isStarted() {
    if (s_pAudioDec) {
        return s_pAudioDec->isStarted();
    }
    return FALSE;
}

BOOL CAudioDec::isPause() {
    if (s_pAudioDec) {
        return s_pAudioDec->isPause();
    }
    return FALSE;
}

void CAudioDec::start() {
    if (s_pAudioDec) {
        s_pAudioDec->resume();
        TRACE_USER(TGOTO, "*** Movie-Audio Start !! ***\n");
    }
}

INT CAudioDec::sendToIOP() {
    if (s_pAudioDec) {
        return s_pAudioDec->sendToIOP();
    }
    return 0;
}

void CAudioDec::reset() {
    if (s_pAudioDec) {
        s_pAudioDec->reset();
    } else {
        s_pAudioDec = new KH_AT("(701)") CAudioDecImpl;
    }
}

void CAudioDec::pause() {
    if (s_pAudioDec) {
        s_pAudioDec->pause();
    }
}

void CAudioDec::resume() {
    if (s_pAudioDec) {
        s_pAudioDec->resume();
    }
}

static INT _sendToIOP(INT nDstIopAddr, const BYTE* pSrc, INT nSize) {
    sceSifDmaData sdd;
    INT did;

    if (nSize <= 0) {
        return 0;
    }
    sdd.data = (unsigned int)pSrc;
    sdd.addr = nDstIopAddr;
    sdd.size = nSize;
    sdd.mode = 0;
    FlushCache(0);
    did = sceSifSetDma(&sdd, 1);
    while (sceSifDmaStat(did) >= 0) {
        DelayThread(1000);
    }
    return nSize;
}

static INT _sendToIOP2Area(INT pd0, INT nD0, INT pd1, INT nD1, BYTE* ps0, INT nS0, BYTE* ps1, INT nS1) {
    INT nDiff;

    if (nD0 + nD1 < nS0 + nS1) {
        nDiff = nS0 + nS1 - (nD0 + nD1);
        if (nDiff >= nS1) {
            nS0 -= nDiff - nS1;
            nS1 = 0;
        } else {
            nS1 -= nDiff;
        }
    }
    if (nS0 >= nD0) {
        _sendToIOP(pd0, ps0, nD0);
        _sendToIOP(pd1, ps0 + nD0, nS0 - nD0);
        _sendToIOP(pd1 + nS0 - nD0, ps1, nS1);
    } else if (nS1 >= nD0 - nS0) {
        _sendToIOP(pd0, ps0, nS0);
        _sendToIOP(pd0 + nS0, ps1, nD0 - nS0);
        _sendToIOP(pd1, ps1 + nD0 - nS0, nS1 - (nD0 - nS0));
    } else {
        _sendToIOP(pd0, ps0, nS0);
        _sendToIOP(pd0 + nS0, ps1, nS1);
    }
    return nS0 + nS1;
}

static int _copy2area(BYTE* pd0, INT nD0, BYTE* pd1, INT nD1, const BYTE* pSrc, INT nSize) {
    INT nRest;

    if (nD0 + nD1 < nSize) {
        return 0;
    }
    if (nSize < nD0) {
        memcpy(pd0, pSrc, nSize);
    } else {
        memcpy(pd0, pSrc, nD0);
        nRest = nSize - nD0;
        if (nRest > nD1) {
            nSize -= nRest - nD1;
            nRest = nD1;
        }
        memcpy(pd1, pSrc + nD0, nRest);
    }
    return nSize;
}

// SOURCE DATA: .data .rodata
