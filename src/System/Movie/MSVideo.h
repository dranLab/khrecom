#ifndef SYSTEM_MOVIE_MSVIDEO_H
#define SYSTEM_MOVIE_MSVIDEO_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/PadHandler.h"

class CMSStream;
class CStrPrim;
class CMovieScript;
class CFadeIOMask;

enum MOVIE_STATUS {
    MOVIE_STATUS_NOP = 0,
    MOVIE_STATUS_OPENED = 1,
    MOVIE_STATUS_PRE_PLAY = 2,
    MOVIE_STATUS_PLAYING = 3,
    MOVIE_STATUS_RESUME = 4,
    MOVIE_STATUS_SKIPOUT = 5,
    MOVIE_STATUS_PRE_END = 6,
    MOVIE_STATUS_END = 7,
    MOVIE_STATUS_END_STILL = 8,
    MOVIE_STATUS_END_SKIP = 9
};

typedef struct {
    u_int pix[256];
} sceIpuRGB32;

typedef enum {
    sceMpegCbError = 0,
    sceMpegCbNodata = 1,
    sceMpegCbStopDMA = 2,
    sceMpegCbRestartDMA = 3,
    sceMpegCbBackground = 4,
    sceMpegCbTimeStamp = 5,
    sceMpegCbStr = 6
} sceMpegCbType;

typedef struct {
    sceMpegCbType type;
    char* errMessage;
} sceMpegCbDataError;

typedef struct {
    sceMpegCbType type;
    long pts;
    long dts;
} sceMpegCbDataTimeStamp;

typedef struct {
    sceMpegCbType type;
    u_char* header;
    u_char* data;
    u_int len;
    long pts;
    long dts;
} sceMpegCbDataStr;

typedef union {
    sceMpegCbType type;
    sceMpegCbDataError error;
    sceMpegCbDataTimeStamp ts;
    sceMpegCbDataStr str;
} sceMpegCbData;

typedef struct {
    int width;
    int height;
    int frameCount;
    long pts;
    long dts;
    u_long flags;
    long pts2nd;
    long dts2nd;
    u_long flags2nd;
    void* sys;
} sceMpeg;

class CMoviePlayerBase : public CTask, public CPadHandler {
public:
    CMoviePlayerBase(CTask* pParent) : CTask(pParent, PRI_BIT_ALWAYS) {
        ignorePause(TRUE);
        ignorePlaySpeed(TRUE);
        setVisible(FALSE);
    }
    virtual ~CMoviePlayerBase() {}
    virtual BOOL drawMovie() { return FALSE; }
    virtual BOOL open(const CHAR* pszFile) { return FALSE; }
    virtual void start() {}
    virtual MOVIE_STATUS getStatus() const { return MOVIE_STATUS_NOP; }
    virtual INT getNowFrame() const { return -1; }
    virtual void setMode(UINT nFlags) {}
};

class CMoviePlayer : public CMoviePlayerBase {
private:
    MOVIE_STATUS m_nStatus;
    INT m_nFlag;
    INT m_nNowFrame;
    CMSStream* m_pStream;
    sceMpeg m_MpegEnv;
    sceIpuRGB32* m_pRGBA;
    BYTE* m_pbyMpegWorkArea;
    BYTE* m_pbyMuxBuffer;
    UINT* m_pdwPath3tag;
    UINT* m_pdqPath3tagForTrans;
    BYTE* m_pbyDemuxBuffer;
    BYTE* m_pbyDemuxBufferPast;
    BYTE* m_pbyDemuxBufferPresent;
    BYTE* m_pbyDemuxBufferFuture;
    UINT m_dwMuxBuffFullness;
    INT m_iBgDuration;
    BYTE m_iMode;
    BYTE m_bKeySkipEnable;
    BYTE m_byTrayStatus;
    BYTE m_iPauseStatus;
    CStrPrim* m_pInfoPrim;
    CMovieScript* m_pScriptManager;
    HANDLE m_hPauseSeq;
    INT m_nDispAlpha;
    CFadeIOMask* m_pFadeMask;

public:
    CMoviePlayer(CTask* pParent KH_DEBUG(, BOOL bForDebug));
    virtual ~CMoviePlayer();
    virtual void main();
    virtual void disp();
    virtual BOOL drawMovie();
    virtual BOOL open(const CHAR* pszFile);
    virtual void start();
    virtual MOVIE_STATUS getStatus() const { return m_nStatus; }
    virtual INT getNowFrame() const { return m_nNowFrame + 1; }
    virtual void setMode(UINT nFlags) { m_iMode = nFlags; }
    static INT videoCallback(sceMpeg* mp, sceMpegCbDataStr* cbData, void* data);
    static INT audioCallback(sceMpeg* mp, sceMpegCbDataStr* cbData, void* data);
    static INT dataCallback(sceMpeg* mp, sceMpegCbDataStr* cbData, void* data);
    static INT nodataCallback(sceMpeg* mp, sceMpegCbData* cbstr, void* data);
    static INT backgroundCallback(sceMpeg* mp, sceMpegCbData* cbstr, void* data);
    static INT errorCallback(sceMpeg* mp, sceMpegCbData* cbstr, void* data);
    static UINT getStreamUnderflowCnt();

private:
    void close();
    void pause(INT nFlag, BOOL bPause);
    void playing();
    void procPause();
    void drawMovieSub();
    BOOL updateFrame();
    INT fillBuffer(BOOL bBlock);
    void setLoadImageTags(u_int* tags, void* image, int x, int y, int width, int height);
    void setLoadImageTagsTile(u_int* tags, void* image, int x, int y, int width, int height);
    void setLoadImageTagsYX(u_int* tags, void* image, int skip, int x, int y, int width, int height);
    void setDMAscTag(u_int* p, u_int spr, void* addr, u_int irq, u_int id, u_int pce, u_int qwc);
    void setGIFtag(u_int* p, u_long regs, u_int nreg, u_int flg, u_int prim, u_int pre, u_int eop, u_int nloop);
    void setGIFad(u_int* p, u_int reg, u_long value);
    void setBITBLTBUF(u_int* p, u_int dbp, u_int dbw, u_int dpsm);
    void setTRXPOS(u_int* p, u_int dir, u_int dsax, u_int dsay);
    void setTRXREG(u_int* p, u_int rrw, u_int rrh);
    void setTRXDIR(u_int* p, u_int xdir);
    void loadImage(u_int* tags);
    void setTransScr0ImageTags(u_int* tags, int width, int height);
};

class CMoviePlayerDummy : public CMoviePlayerBase {
private:
    char m_szInfo[256];
    MOVIE_STATUS m_nStatus;
    CStrPrim* m_pInfoPrim;

public:
    CMoviePlayerDummy(CTask* pParent);
    virtual ~CMoviePlayerDummy();
    virtual void main();
    virtual void disp();
    virtual BOOL open(const CHAR* pszFile);
    virtual void start();
    virtual MOVIE_STATUS getStatus() const { return m_nStatus; }
};

#endif // SYSTEM_MOVIE_MSVIDEO_H
