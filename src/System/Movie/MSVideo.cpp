#include "System/Movie/MSVideo.h"
#include "System/Graphic/StrPrim.h"
#include "System/Graphic/ColorFader.h"
#include "System/Movie/Movie.h"
#include "System/Movie/MSStream.h"
#include "System/Movie/MovieScript.h"
#include "System/Movie/AudioDec.h"
#include "System/TaskMan.h"
#include "System/FileIO.h"
#include "System/Thread.h"
#include "System/VramMan.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include <string.h>
extern "C" int sprintf(char* buf, const char* fmt, ...);
#include "include_asm.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void* operator new[](size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void operator delete[](void* mem) { com_free(mem); }

template <class T> static inline T& _ref(T& v) { return v; }

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

#define UNCACHED(p) ((((u_int)(p)) & 0x0fffffff) | 0x20000000)
#define T0_COUNT (*(volatile u_int*)0x10000000)

/* GS register write (data | address << 64); volatile keeps each qword's computation before its store. */
#define PACK128(hi, lo)                                                                           \
    ({                                                                                            \
        u_long128 out128;                                                                         \
        __asm__ volatile("pcpyld %0, %2, %1" : "=r"(out128) : "r"((u_long)(lo)), "r"((u_long)(hi))); \
        out128;                                                                                   \
    })

extern "C" {
int sceMpegInit();
int sceMpegCreate(sceMpeg* mp, u_char* work_area, int work_area_size);
int sceMpegDelete(sceMpeg* mp);
int sceMpegReset(sceMpeg* mp);
int sceMpegIsEnd(sceMpeg* mp);
int sceMpegGetPicture(sceMpeg* mp, sceIpuRGB32* rgb32, int mbcount);
int sceMpegGetPictureAbort(sceMpeg* mp);
int sceMpegDemuxPss(sceMpeg* mp, u_char* pss, int pss_size);
void* sceMpegAddCallback(sceMpeg* mp, sceMpegCbType type, void* callback, void* anyData);
void* sceMpegAddStrCallback(sceMpeg* mp, int strType, int ch, void* callback, void* data);
int sceGsSyncPath(int mode, u_short timeout);
void FlushCache(int operation); /* void: an int result changes the register choice in nodataCallback */
int DelayThread(int usec);
}

extern INT SCREEN_HEIGHT;

#include "System/Graphic/PackMan.h"









class CPause : public CTask {
private:
    BYTE _pad[0x100 - sizeof(CTask)];

public:
    static UINT m_nResult;
    CPause(CTask* p, INT nPauseKind);
};

class CFadeIOMask : public CColorFader, public CTask {
private:
    BYTE _pad[0x90 - 0x20 - sizeof(CTask)];

public:
    CFadeIOMask(CTask& inParent, USHORT nLayerDepth);
};

/* The original's position setter converts y and z at RTL level (constants loaded as unsigned: lui/ori for -256). */
static inline void _setXYZ(G_XYZ& xyz, FIX4 _x, UINT _y, UINT _z) {
    xyz.x = _x;
    xyz.y = _y;
    xyz.z = _z;
}

static UINT s_nStreamUnderflowCnt = 0;

CMoviePlayer::CMoviePlayer(CTask* pParent, BOOL bForDebug) : CMoviePlayerBase(pParent) {
    m_nStatus = MOVIE_STATUS_NOP;
    m_nFlag = 0;
    m_pStream = NULL;
    m_pRGBA = NULL;
    m_pbyMpegWorkArea = NULL;
    m_pbyMuxBuffer = NULL;
    m_pdwPath3tag = NULL;
    m_pdqPath3tagForTrans = NULL;
    m_pbyDemuxBuffer = NULL;
    m_pbyDemuxBufferPast = NULL;
    m_pbyDemuxBufferPresent = NULL;
    m_pbyDemuxBufferFuture = NULL;
    m_iMode = 0;
    m_pInfoPrim = NULL;
    m_pScriptManager = NULL;
    m_hPauseSeq = 0;
    m_pFadeMask = NULL;
    if (bForDebug) {
        m_nFlag = 0x80000000;
    }
}

CMoviePlayer::~CMoviePlayer() {
    m_iMode = 0;
    close();
    delete m_pStream;
    CAudioDec::dispose();
    delete m_pInfoPrim;
    CPackMan::changeDrawMode(0);
}

BOOL CMoviePlayer::open(const CHAR* pszFile) {
    INT n;
    int iWorkAreaSize;
    INT i;

    close();
#line 174
    m_pbyMuxBuffer = (BYTE*)com_memalign_d(64, 0x4000, __FILE__ "(174)", (UINT)-1);
    if (!m_pbyMuxBuffer) goto error;
    m_dwMuxBuffFullness = 0;
    m_nNowFrame = 0;
#line 184
    m_pbyDemuxBuffer = (BYTE*)com_memalign_d(64, 0x38000, __FILE__ "(184)", (UINT)-1);
    m_pbyDemuxBufferPast = m_pbyDemuxBuffer + 16;
    m_pbyDemuxBufferFuture = m_pbyDemuxBuffer + 16;
    m_pbyDemuxBufferPresent = m_pbyDemuxBuffer;
    if (!m_pbyDemuxBuffer) goto error;
    CAudioDec::create();
#line 201
    m_pRGBA = (sceIpuRGB32*)com_memalign_d(64, NMSVIDEO_MAX_MBX * NMSVIDEO_MAX_MBY * sizeof(sceIpuRGB32),
                                           __FILE__ "(201)", (UINT)-1);
    if (!m_pRGBA) goto error;
    n = NMSVIDEO_MAX_MBX * NMSVIDEO_MAX_MBY * 64;
    {
        u_long128* p = (u_long128*)m_pRGBA;
        for (i = 0; i < n; i++) {
            p[i] = 0;
        }
    }
#line 213
    m_pdwPath3tag = (UINT*)com_memalign_d(64, ((NMSVIDEO_MAX_MBX * NMSVIDEO_MAX_MBY * 6 + 25) / 16) * 256,
                                          __FILE__ "(213)", (UINT)-1);
    if (!m_pdwPath3tag) goto error;
#line 219
    m_pdqPath3tagForTrans = (UINT*)com_memalign_d(64, 0x900, __FILE__ "(219)", (UINT)-1);
    if (!m_pdqPath3tagForTrans) goto error;
#line 226
    m_pScriptManager = new (__FILE__ "(226)") CMovieScript(*this);
    if (!m_pScriptManager) goto error;
    m_pScriptManager->open(pszFile);
    sceMpegInit();
    iWorkAreaSize = NMSVIDEO_MAX_WIDTH * NMSVIDEO_MAX_HEIGHT * 9 / 2 + 0x2800;
#line 239
    m_pbyMpegWorkArea = (BYTE*)com_memalign_d(128, iWorkAreaSize, __FILE__ "(239)", (UINT)-1);
    if (!m_pbyMpegWorkArea) goto error;
    sceMpegCreate(&m_MpegEnv, m_pbyMpegWorkArea, iWorkAreaSize);
    sceMpegReset(&m_MpegEnv);
    sceMpegAddStrCallback(&m_MpegEnv, 0, 0, (void*)videoCallback, this);
    sceMpegAddStrCallback(&m_MpegEnv, 2, 0, (void*)audioCallback, this);
    sceMpegAddStrCallback(&m_MpegEnv, 4, 0, (void*)dataCallback, this);
    sceMpegAddCallback(&m_MpegEnv, sceMpegCbNodata, (void*)nodataCallback, this);
    sceMpegAddCallback(&m_MpegEnv, sceMpegCbBackground, (void*)backgroundCallback, this);
    sceMpegAddCallback(&m_MpegEnv, sceMpegCbError, (void*)errorCallback, this);
    m_nFlag |= 1;
    if (!m_pStream) {
#line 258
        m_pStream = new (__FILE__ "(258)") CMSStream;
        ASSERT(m_pStream);
    }
    if (!m_pStream->Open(pszFile, 0x4000)) {
    error:
        close();
        return FALSE;
    }
    while (fillBuffer(TRUE) > 0) {
    }
    CAudioDec::sendToIOP();
    m_iBgDuration = 0;
    m_iMode = 0;
    m_byTrayStatus = 0;
    m_iPauseStatus = 0;
    m_bKeySkipEnable = 0;
    m_nStatus = MOVIE_STATUS_OPENED;
    return TRUE;
}

void CMoviePlayer::start() {
#line 297
    ASSERT(MOVIE_STATUS_OPENED == m_nStatus);
    m_nStatus = MOVIE_STATUS_PRE_PLAY;
    if (m_nFlag & 0x20000) {
        close();
        m_nStatus = MOVIE_STATUS_END;
    } else {
        m_bKeySkipEnable = m_iMode & 1;
        CPackMan::changeDrawMode(2);
        if (m_pScriptManager) {
            m_pScriptManager->start();
        }
    }
}

void CMoviePlayer::close() {
    setVisible(FALSE);
    CAudioDec::dispose();
    if (m_pStream) {
        m_pStream->Close();
    }
    if (m_nFlag & 1) {
        sceMpegAddStrCallback(&m_MpegEnv, 0, 0, NULL, NULL);
        sceMpegAddStrCallback(&m_MpegEnv, 2, 0, NULL, NULL);
        sceMpegAddCallback(&m_MpegEnv, sceMpegCbNodata, NULL, NULL);
        sceMpegAddCallback(&m_MpegEnv, sceMpegCbBackground, NULL, NULL);
        sceMpegAddCallback(&m_MpegEnv, sceMpegCbError, NULL, NULL);
        sceMpegReset(&m_MpegEnv);
        sceMpegDelete(&m_MpegEnv);
        sceMpegInit();
    }
    m_nStatus = MOVIE_STATUS_NOP;
    m_nFlag &= 0x80000000;
    m_bKeySkipEnable = 0;
    if (m_pbyMpegWorkArea) {
        com_free(m_pbyMpegWorkArea);
    }
    m_pbyMpegWorkArea = NULL;
    if (m_pbyMuxBuffer) {
        com_free(m_pbyMuxBuffer);
    }
    m_pbyMuxBuffer = NULL;
    if (m_pbyDemuxBuffer) {
        com_free(m_pbyDemuxBuffer);
    }
    m_pbyDemuxBuffer = NULL;
    if (!(m_iMode & 4)) {
        if (m_pdwPath3tag) {
            sceGsSyncPath(0, 0);
            com_free(m_pdwPath3tag);
            m_pdwPath3tag = NULL;
        }
        if (m_pRGBA) {
            com_free(m_pRGBA);
            m_pRGBA = NULL;
        }
        if (m_pdqPath3tagForTrans) {
            com_free(m_pdqPath3tagForTrans);
            m_pdqPath3tagForTrans = NULL;
        }
        if (m_pFadeMask) {
            m_pFadeMask->out();
            m_pFadeMask = NULL;
        }
    }
    if (m_pScriptManager) {
        delete m_pScriptManager;
    }
    m_pScriptManager = NULL;
}

void CMoviePlayer::pause(INT nFlag, BOOL bPause) {
    if (bPause) {
        m_nFlag |= nFlag;
        if (!m_iPauseStatus) {
            m_pStream->Pause();
            CAudioDec::pause();
            m_iPauseStatus = 1;
            if (!m_hPauseSeq && !(m_nFlag & 0x80000000) && nFlag == 0x8000) {
                CTask* p;
#line 405
                p = new (__FILE__ "(405)") CPause(this, 5);
                ASSERT(p);
                m_hPauseSeq = p->getHandle();
            }
        }
    } else if (m_iPauseStatus == 1) {
        m_nFlag &= ~nFlag;
        if (!(m_nFlag & 0xC000)) {
            m_pStream->Resume();
            m_iPauseStatus = 0;
            m_nStatus = MOVIE_STATUS_RESUME;
            Thread::delayThread_1Frame();
        }
    }
}

void CMoviePlayer::main() {
    switch (m_nStatus) {
    case MOVIE_STATUS_RESUME:
        if (CAudioDec::isOK()) {
            CAudioDec::resume();
        }
    case MOVIE_STATUS_PRE_PLAY:
        m_nStatus = MOVIE_STATUS_PLAYING;
        s_nStreamUnderflowCnt = 0;
        break;
    case MOVIE_STATUS_PLAYING:
        if (CPackMan::getDrawMode() == 2) {
            playing();
            if (m_nFlag & 0x30000) {
                m_nDispAlpha = 0xA0;
                m_iMode &= ~4;
                m_nStatus = MOVIE_STATUS_SKIPOUT;
                if (m_iMode & 2) {
                    if (!m_pFadeMask) {
#line 462
                        m_pFadeMask = new (__FILE__ "(462)") CFadeIOMask(*this, 10);
                    }
                } else if (m_pFadeMask) {
                    m_pFadeMask->out();
                    m_pFadeMask = NULL;
                }
            }
        }
        break;
    case MOVIE_STATUS_SKIPOUT:
        if (!CMainApp::isWarningPauseNow()) {
            if (m_nDispAlpha > 0) {
                INT nOld = m_nDispAlpha;
                m_nDispAlpha -= 8;
                if (m_pFadeMask) {
                    if (m_nDispAlpha < 0x80 && nOld >= 0x80) {
                        m_pFadeMask->reqWhiteOut(20, 0.f);
                    }
                } else if (m_nDispAlpha < 0x80) {
                    if (m_nDispAlpha < 0) {
                        m_nDispAlpha = 0;
                    }
                    CPackMan::setFrameEffect(4, 0, 0, m_nDispAlpha);
                }
            } else {
                m_nStatus = (m_nFlag & 0x20000) ? MOVIE_STATUS_END : MOVIE_STATUS_END_SKIP;
            }
        }
        break;
    case MOVIE_STATUS_PRE_END:
        close();
        m_nStatus = MOVIE_STATUS_END;
        break;
    }
}

void CMoviePlayer::disp() {}

void CMoviePlayer::playing() {
    if (m_nFlag & 0x20000) return;
    if (sceMpegIsEnd(&m_MpegEnv)) {
        m_nStatus = MOVIE_STATUS_PRE_END;
        if (!(m_iMode & 4)) {
            CPackMan::changeDrawMode(0);
            INT nMax = NMSVIDEO_MAX_MBX * NMSVIDEO_MAX_MBY * 256;
            INT* p = (INT*)m_pRGBA;
            for (INT i = 0; i < nMax; i++) {
                *p++ = 0;
            }
        }
        CAudioDec::reset();
        TRACE("MoviePlayer End... (%d)\n", CMainApp::getFrameCount());
        if (m_iMode & 4) {
            TRACE_USER((USER)4, "Continue Last Frame...\n");
        }
        return;
    }
    if (m_nNowFrame > 0) {
        procPause();
        if (m_byTrayStatus == 0) {
            if (CMainApp::isWarningPauseNow()) {
                m_byTrayStatus = 1;
                pause(0x4000, TRUE);
                return;
            }
        } else if (m_byTrayStatus == 1) {
            if (!CMainApp::isWarningPauseNow()) {
                pause(0x4000, FALSE);
                m_byTrayStatus = 0;
            }
        }
    }
    if (m_iPauseStatus) return;
    if (sceMpegGetPicture(&m_MpegEnv, m_pRGBA, NMSVIDEO_MAX_MBX * NMSVIDEO_MAX_MBY) < 0) {
        TRACE_COL((TtyColor)0x1F, "sceMpegGetPicture failed\n");
        return;
    }
    m_nNowFrame = m_MpegEnv.frameCount;
    if (m_nNowFrame == 0) {
        INT nWidth = m_MpegEnv.width;
        INT nHeight = m_MpegEnv.height;
        setLoadImageTags(m_pdwPath3tag, m_pRGBA, 0, 0, nWidth, nHeight);
        setTransScr0ImageTags(m_pdqPath3tagForTrans, nWidth, nHeight);
        m_iBgDuration = (FLOAT)nWidth * (FLOAT)nHeight * 0.000151f;
        CPackMan::setMovieEnv(nWidth, nHeight, 0x200, 0x174);
        CPackMan::setFrameEffect(4, 0, 0, 0x80);
        setVisible(TRUE);
    }
    if (m_pScriptManager) {
        m_pScriptManager->update(m_nNowFrame + 1);
    }
}

void CMoviePlayer::procPause() {
    BOOL bPauseEnable = m_bKeySkipEnable;
    if (m_nFlag & 0x80000000) bPauseEnable = TRUE;
    if (!bPauseEnable) return;
    if (m_iPauseStatus == 0) {
        if (CPadHandler::isButtonTrg(0x800)) {
            pause(0x8000, TRUE);
        }
    } else if (m_iPauseStatus == 1) {
        if (m_nFlag & 0x80000000) {
            if (CPadHandler::isButtonTrg(0x800)) {
                pause(0x8000, FALSE);
            }
        } else if (m_nFlag & 0x8000) {
            if (m_hPauseSeq && !CTaskMan::getTask(m_hPauseSeq)) {
                m_hPauseSeq = 0;
                if (_ref(CPause::m_nResult) == 1) {
                    m_nFlag |= 0x10000;
                } else {
                    pause(0x8000, FALSE);
                }
            }
        }
    }
}

BOOL CMoviePlayer::drawMovie() {
    BOOL bDraw = FALSE;
    switch (m_nStatus) {
    case MOVIE_STATUS_PLAYING:
    case MOVIE_STATUS_RESUME:
    case MOVIE_STATUS_SKIPOUT:
        bDraw = updateFrame();
        if (bDraw) {
            CAudioDec::sendToIOP();
            if (!CAudioDec::isStarted() && !m_iPauseStatus) {
                if (!CAudioDec::isOK()) {
                    TRACE_COL((TtyColor)0x1F, "!!! Mpeg-Audio is not ready !!!\n");
                    CAudioDec::start();
                    CAudioDec::pause();
                } else {
                    CAudioDec::start();
                }
            }
        }
        break;
    case MOVIE_STATUS_PRE_END:
    case MOVIE_STATUS_END_SKIP: {
        if (!(m_iMode & 4)) {
            drawMovieSub();
        }
        MOVIE_STATUS nStatus = m_nStatus;
        if (nStatus == MOVIE_STATUS_END_SKIP) {
            close();
            m_nStatus = nStatus;
        }
        if (!(m_iMode & 4)) break;
        m_nStatus = MOVIE_STATUS_END_STILL;
    }
    case MOVIE_STATUS_END_STILL:
        drawMovieSub();
        bDraw = TRUE;
        break;
    }
    return bDraw;
}

void CMoviePlayer::drawMovieSub() {
    loadImage(m_pdwPath3tag);
    sceGsSyncPath(0, 0);
    loadImage(m_pdqPath3tagForTrans);
}

BOOL CMoviePlayer::updateFrame() {
    BOOL bDraw = FALSE;
    if (isVisible()) {
        drawMovieSub();
        bDraw = TRUE;
    }
    if (m_iPauseStatus) return bDraw;
    while (T0_COUNT < (UINT)NMSVIDEO_FILLBUFFMAXTIME) {
        fillBuffer(FALSE);
    }
    return bDraw;
}

INT CMoviePlayer::fillBuffer(BOOL bBlock) {
    BYTE* pbyStartAddress;
    INT iProcessBytes;
    UINT nLimiter;

    if (!m_pbyMuxBuffer) return 0;
    nLimiter = 0;
    if (m_dwMuxBuffFullness) {
        pbyStartAddress = m_pbyMuxBuffer - (m_dwMuxBuffFullness - 0x4000);
    } else {
        do {
            INT nTimeLine;
            INT nTime = T0_COUNT;
            m_dwMuxBuffFullness = m_pStream->Read(m_pbyMuxBuffer);
            if (m_dwMuxBuffFullness == 0) {
                if (!bBlock) {
                    if (m_nStatus == MOVIE_STATUS_PLAYING && m_nNowFrame >= 2) {
                        s_nStreamUnderflowCnt++;
                        TRACE_USER((USER)4, "*");
                    }
                    return 0;
                }
                if (CFileIO::isTrayOpened() && CFileIO::checkTrayClosed()) {
                    m_pStream->Resume();
                }
                DelayThread(2000);
            }
            nTimeLine = T0_COUNT - nTime;
            if (nTimeLine < 0) nTimeLine += 0x10000;
            nLimiter += nTimeLine;
            if (nLimiter > 0x13000) {
                TRACE_COL((TtyColor)0x1F, "CMoviePlayer::fillBuffer ... TimeOut\n");
                m_nFlag |= 0x20000;
                pause(0x4000, TRUE);
                return -1;
            }
        } while (m_dwMuxBuffFullness == 0);
        pbyStartAddress = m_pbyMuxBuffer;
    }
    iProcessBytes = sceMpegDemuxPss(&m_MpegEnv, pbyStartAddress, m_dwMuxBuffFullness);
    m_dwMuxBuffFullness -= iProcessBytes;
    return iProcessBytes;
}

INT CMoviePlayer::videoCallback(sceMpeg* mp, sceMpegCbDataStr* cbData, void* data) {
    CMoviePlayer* pMSVideo = (CMoviePlayer*)data;
    INT availSpace;
    INT spill;

    if (!pMSVideo->m_pbyMuxBuffer) return 0;
    availSpace = pMSVideo->m_pbyDemuxBufferPresent - pMSVideo->m_pbyDemuxBufferPast;
    if (availSpace < 0) availSpace += 0x38000;
    if (availSpace == 0) {
        availSpace = (pMSVideo->m_pbyDemuxBufferPresent == pMSVideo->m_pbyDemuxBufferFuture) ? 0x38000 : 0;
    }
    if ((UINT)availSpace < cbData->len) return 0;
    spill = pMSVideo->m_pbyDemuxBufferPast - pMSVideo->m_pbyDemuxBuffer + cbData->len - 0x38000;
    if (spill > 0) {
        memcpy((void*)UNCACHED(pMSVideo->m_pbyDemuxBufferPast), cbData->data, cbData->len - spill);
        memcpy((void*)UNCACHED(pMSVideo->m_pbyDemuxBuffer), cbData->data + (cbData->len - spill), spill);
        pMSVideo->m_pbyDemuxBufferPast = pMSVideo->m_pbyDemuxBuffer + spill;
    } else {
        memcpy((void*)UNCACHED(pMSVideo->m_pbyDemuxBufferPast), cbData->data, cbData->len);
        pMSVideo->m_pbyDemuxBufferPast += cbData->len;
    }
    return 1;
}

INT CMoviePlayer::audioCallback(sceMpeg* mp, sceMpegCbDataStr* cbData, void* data) {
    return CAudioDec::putData(cbData->data, cbData->len);
}

INT CMoviePlayer::dataCallback(sceMpeg* mp, sceMpegCbDataStr* cbData, void* data) {
    return CAudioDec::putData(cbData->data, cbData->len);
}

INT CMoviePlayer::nodataCallback(sceMpeg* mp, sceMpegCbData* cbstr, void* data) {
    int dmaSize;
    int availData;
    CMoviePlayer* pMSVideo = (CMoviePlayer*)data;
    INT nRes;

    if (!pMSVideo->m_pbyMuxBuffer) return 0;
    pMSVideo->m_pbyDemuxBufferPresent = pMSVideo->m_pbyDemuxBufferFuture;
    availData = pMSVideo->m_pbyDemuxBufferPast - pMSVideo->m_pbyDemuxBufferFuture;
    if (availData < 0) availData += 0x38000;
    while (availData < 0x1000) {
        if (pMSVideo->m_nFlag & 0x20000) {
            sceMpegGetPictureAbort(mp);
            break;
        }
        nRes = pMSVideo->fillBuffer(TRUE);
        if (nRes == 0) {
            availData += 15;
            TRACE("End of stream presumed\n");
            break;
        }
        if (nRes < 0) {
            sceMpegGetPictureAbort(mp);
            break;
        }
        availData = pMSVideo->m_pbyDemuxBufferPast - pMSVideo->m_pbyDemuxBufferFuture;
        if (availData < 0) availData += 0x38000;
    }
    dmaSize = availData & ~15;
    if (dmaSize > 0x1000) dmaSize = 0x1000;
    if (pMSVideo->m_pbyDemuxBufferPresent + dmaSize > pMSVideo->m_pbyDemuxBuffer + 0x38000) {
        dmaSize = pMSVideo->m_pbyDemuxBuffer + 0x38000 - pMSVideo->m_pbyDemuxBufferPresent;
    }
    FlushCache(0);
    *(volatile u_int*)0x1000B420 = dmaSize / 16;
    *(volatile u_int*)0x1000B410 = (u_int)pMSVideo->m_pbyDemuxBufferPresent;
    *(volatile u_int*)0x1000B400 = 0x101;
    pMSVideo->m_pbyDemuxBufferFuture = pMSVideo->m_pbyDemuxBufferPresent + dmaSize;
    if (pMSVideo->m_pbyDemuxBufferFuture >= pMSVideo->m_pbyDemuxBuffer + 0x38000) {
        pMSVideo->m_pbyDemuxBufferFuture -= 0x38000;
    }
    return 1;
}

INT CMoviePlayer::backgroundCallback(sceMpeg* mp, sceMpegCbData* cbstr, void* data) {
    CMoviePlayer* pMSVideo = (CMoviePlayer*)data;
    INT iDueTime = T0_COUNT + pMSVideo->m_iBgDuration;
    while (T0_COUNT < (UINT)iDueTime) {
        pMSVideo->fillBuffer(FALSE);
    }
    return 1;
}

INT CMoviePlayer::errorCallback(sceMpeg* mp, sceMpegCbData* cbstr, void* data) {
    if (cbstr) {
        TRACE("errorCallback: MPEG decoding error '%s'\n", cbstr->error.errMessage);
    }
    return 1;
}

void CMoviePlayer::setLoadImageTags(u_int* tags, void* image, int x, int y, int width, int height) {
    setLoadImageTagsYX(tags, image, 0x400, x, y, width, height);
}

void CMoviePlayer::setLoadImageTagsTile(u_int* tags, void* image, int x, int y, int width, int height) {
    setLoadImageTagsYX(tags, image, 0, x, y, width, height);
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Movie/MSVideo", setLoadImageTagsYX__12CMoviePlayerPUiPviiiii);
// Not matching: mby - 1 is not hoisted into a register for the inner loop (the original keeps it in s8 and
// spills i * 16), so the j++ / dsay increments are scheduled differently.
#if 0
void CMoviePlayer::setLoadImageTagsYX(u_int* tags, void* image, int skip, int x, int y, int width, int height) {
    u_int* p = (u_int*)UNCACHED(tags);
    u_int dbp = VRAM::_TEX_PREFIX_START;
    u_int dbw = (NMSVIDEO_MAX_WIDTH + 63) / 64;
    int mbx = width >> 4;
    int mby = height >> 4;
    int i;
    int j;
    int eop;

    setDMAscTag(p, 0, 0, 0, 1, 0, 3);
    p += 4;
    setGIFtag(p, 0xE, 1, 0, 0, 0, 0, 2);
    p += 4;
    setBITBLTBUF(p, dbp, dbw, 0);
    p += 4;
    setTRXREG(p, 16, 16);
    p += 4;
    for (i = 0; i < mbx; i++) {
        for (j = 0; j < mby; j++) {
            eop = 0;
            if (i == mbx - 1) eop = 1;
            if (j != mby - 1) eop = 0;
            setDMAscTag(p, 0, 0, 0, 1, 0, 4);
            p += 4;
            setGIFtag(p, 0xE, 1, 0, 0, 0, 0, 2);
            p += 4;
            setTRXPOS(p, 0, x + i * 16, y + j * 16);
            p += 4;
            setTRXDIR(p, 0);
            p += 4;
            setGIFtag(p, 0, 0, 2, 0, 0, eop, 64);
            p += 4;
            setDMAscTag(p, 0, (void*)((u_int)image & 0x0fffffff), 0, 3, 0, 64);
            p += 4;
            image = (char*)image + skip;
        }
    }
    setDMAscTag(p, 0, 0, 0, 7, 0, 0);
}
#endif

void CMoviePlayer::setDMAscTag(u_int* p, u_int spr, void* addr, u_int irq, u_int id, u_int pce, u_int qwc) {
    *(u_long*)p = ((u_long)spr << 63) | ((u_long)((u_int)addr & 0xfffffff0) << 32) | ((u_long)irq << 31) |
                  ((u_long)id << 28) | ((u_long)pce << 26) | (u_long)qwc;
}

void CMoviePlayer::setGIFtag(u_int* p, u_long regs, u_int nreg, u_int flg, u_int prim, u_int pre, u_int eop,
                             u_int nloop) {
    p[0] = (eop << 15) | nloop;
    p[1] = (pre << 14) | (prim << 15) | (flg << 26) | (nreg << 28);
    p[2] = regs & 0xffffffff;
    p[3] = regs >> 32;
}

void CMoviePlayer::setGIFad(u_int* p, u_int reg, u_long value) {
    p[0] = value & 0xffffffff;
    p[1] = value >> 32;
    p[2] = reg;
    p[3] = 0;
}

void CMoviePlayer::setBITBLTBUF(u_int* p, u_int dbp, u_int dbw, u_int dpsm) {
    setGIFad(p, 0x50, ((u_long)dpsm << 56) | ((u_long)dbw << 48) | ((u_long)dbp << 32));
}

void CMoviePlayer::setTRXPOS(u_int* p, u_int dir, u_int dsax, u_int dsay) {
    setGIFad(p, 0x51, ((u_long)dir << 59) | ((u_long)dsay << 48) | ((u_long)dsax << 32));
}

void CMoviePlayer::setTRXREG(u_int* p, u_int rrw, u_int rrh) {
    setGIFad(p, 0x52, ((u_long)rrh << 32) | (u_long)rrw);
}

void CMoviePlayer::setTRXDIR(u_int* p, u_int xdir) {
    setGIFad(p, 0x53, (u_long)xdir);
}

void CMoviePlayer::loadImage(u_int* tags) {
    *(volatile u_int*)0x1000A030 = (u_int)tags & 0x0fffffff;
    *(volatile u_int*)0x1000A020 = 0;
    *(volatile u_int*)0x1000A000 = 0x105;
}

void CMoviePlayer::setTransScr0ImageTags(u_int* tags, int width, int height) {
    u_int* p = (u_int*)UNCACHED(tags);
    INT i;
    INT eop;

    setDMAscTag(p, 0, 0, 0, 1, 0, 8);
    p += 4;
    setGIFtag(p, 0xE, 1, 0, 0, 0, 0, 7);
    p += 4;
    *(u_long128*)&p[0] = PACK128(0x47, 0x30000);
    *(u_long128*)&p[4] = PACK128(0x4E, (u_long)(VRAM::_ZBUF_START / 32) | 0x31000000 | ((u_long)1 << 32));
    *(u_long128*)&p[8] = PACK128(0x4C, (u_long)(VRAM::_FRAME0_START / 32) | 0x1080000);
    *(u_long128*)&p[12] = PACK128(0x42, 0x44);
    *(u_long128*)&p[16] = PACK128(0x14, 0x61);
    *(u_long128*)&p[20] = PACK128(0x08, 0x05);
    *(u_long128*)&p[24] = PACK128(0x4A, 0);
    p += 28;
    setDMAscTag(p, 0, 0, 0, 1, 0, 4);
    p += 4;
    setGIFtag(p, 0xE, 1, 0, 0, 0, 0, 3);
    p += 4;
    *(u_long128*)&p[0] = PACK128(0x06, (u_long)VRAM::_TEX_PREFIX_START |
                                           ((u_long)(NMSVIDEO_MAX_WIDTH / 64) << 14) | 0x20000006A8000000);
    *(u_long128*)&p[4] = PACK128(0x01, 0x80808080);
    *(u_long128*)&p[8] = PACK128(0x00, 0x116);
    p += 12;
    for (i = 0; i < 16; i++) {
        eop = (i + 1 >= 16);
        setDMAscTag(p, 0, 0, 0, 1, 0, 6);
        p += 4;
        setGIFtag(p, 0xE, 1, 0, 0, 0, eop, 5);
        p += 4;
        *(u_long128*)&p[0] = PACK128(0x40, (u_long)(i * 32) | ((u_long)((i + 1) * 32 - 1) << 16) |
                                               ((u_long)(SCREEN_HEIGHT - 1) << 48));
        *(u_long128*)&p[4] = PACK128(0x03, 0x80008);
        *(u_long128*)&p[8] = PACK128(0x05, 0xFFFFF76107000);
        {
            INT u = (width - 1) * 16 + 8;
            INT v = height * 16 + 8;
            *(u_long128*)&p[12] = PACK128(0x03, (u_long)(u_int)u | ((u_long)(u_int)v << 16));
        }
        *(u_long128*)&p[16] = PACK128(0x05, 0xFFFFF89F09000);
        p += 20;
    }
    setDMAscTag(p, 0, 0, 0, 7, 0, 0);
}

UINT CMoviePlayer::getStreamUnderflowCnt() {
    return s_nStreamUnderflowCnt;
}

CMoviePlayerDummy::CMoviePlayerDummy(CTask* pParent) : CMoviePlayerBase(pParent) {
    m_nStatus = MOVIE_STATUS_NOP;
    m_pInfoPrim = NULL;
}

CMoviePlayerDummy::~CMoviePlayerDummy() {
    delete[] m_pInfoPrim;
    CPackMan::enableDraw(15);
}

void CMoviePlayerDummy::main() {
    if (m_nStatus == MOVIE_STATUS_PLAYING && CPadHandler::isButtonTrg(0x20)) {
        m_nStatus = MOVIE_STATUS_END;
    }
}

void CMoviePlayerDummy::disp() {
    if (m_pInfoPrim) {
        for (INT i = 0; i < 2; i++) {
            m_pInfoPrim[i].disp();
        }
    }
}

BOOL CMoviePlayerDummy::open(const CHAR* pszFile) {
    char szTmp[128];

    if (!m_pInfoPrim) {
#line 1593
        m_pInfoPrim = new (__FILE__ "(1593)") CStrPrim[2];
    }
    CStrPrim* pPrim = m_pInfoPrim;
    _setXYZ(pPrim->m_P0, 0, (UINT)-256, 0xFFFF280);
    pPrim->m_C0.set(0x80, 0x80, 0x40);
    pPrim->m_Size = 0x100;
    pPrim->setText(m_szInfo);
    pPrim->setLayerFG(10);
    pPrim->m_Style = 0x8042;
    if (!pszFile) {
        pszFile = "NONAME";
    } else {
        const CHAR* p = strrchr(pszFile, '/');
        if (!p) p = strrchr(pszFile, '\\');
        strcpy(szTmp, p ? p + 1 : pszFile);
        CHAR* q = strrchr(szTmp, '.');
        if (q) *q = 0;
        pszFile = szTmp;
    }
    sprintf(m_szInfo, "MOVIE \"%s\" IS PLAYING...", pszFile);
    pPrim = m_pInfoPrim;
    _setXYZ(pPrim[1].m_P0, 0, 0x80, 0xFFFF280);
    pPrim[1].m_C0.set(0x80, 0x80, 0x40);
    pPrim++;
    pPrim->m_Size = 0x100;
    pPrim->allocText("PUSH \"\x81\x9b\" BUTTON TO EXIT");
    pPrim->setLayerFG(10);
    pPrim->m_Style = 0x8042;
    m_nStatus = MOVIE_STATUS_OPENED;
    return TRUE;
}

void CMoviePlayerDummy::start() {
    CPackMan::disableDraw(6);
    m_nStatus = MOVIE_STATUS_PLAYING;
    setVisible(TRUE);
}
// SOURCE DATA: .data .rodata
