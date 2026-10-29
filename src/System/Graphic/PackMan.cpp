#include "khtypes.h"
#include "include_asm.h"
#include "System/VramMan.h"

typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned short u_short;
typedef unsigned char u_char;

struct VTransInfo { /* PARTIAL: stand-in for System/Graphic/TexUtil VTransInfo: gated by SYSTEM_GRAPHIC_GPRIM_CPP; strings: Gprim.h (.rodata still asm) */
    UINT _nDistPos;
    USHORT _nFormat;
    USHORT _nDistBufW;
    ULONGLONG* _pImage;
    UINT _qwImage;
    USHORT _nDistX;
    USHORT _nDistY;
    USHORT _nDistW;
    USHORT _nDistH;
    USHORT _nTw;
    USHORT _nTh;
    UINT _nTransSize;
};

union QWdata { /* PARTIAL: stand-in for System/Graphic/Gprim QWdata: strings: Gprim.h (.rodata still asm) */
    u_long ul64[2];
    u_long128 ul128;
    u_int ui32[4];
};

struct pacVif1Packet {
    u_int* pCurrent;    /* 0x00 */
    u_long128* pBase;   /* 0x04 */
    u_long128* pDmaTag; /* 0x08 */
    u_int* pVifCode;    /* 0x0c */
    u_int numlen;       /* 0x10 */
    u_long* pGifTag;    /* 0x14 */
    u_int pad12;        /* 0x18 */
    u_int iVifCode;     /* 0x1c */
    QWdata lDmaTag;     /* 0x20 */
    QWdata lGifTag;     /* 0x30 */
};

struct tD_CHCR { /* PARTIAL: stand-in for System/Graphic/Effect/libppp/draw/draw_w tD_CHCR: owner not written */
    u_int DIR : 1;
    u_int p0 : 1;
    u_int MOD : 2;
    u_int ASP : 2;
    u_int TTE : 1;
    u_int TIE : 1;
    u_int STR : 1;
    u_int p1 : 7;
    u_int TAG : 16;
};
struct sceDmaChan { /* PARTIAL: stand-in for System/Graphic/Effect/libppp/draw/draw_w sceDmaChan: owner not written (only chcr used here) */
    tD_CHCR chcr;
};
extern "C" sceDmaChan* sceDmaGetChan(int id);
extern "C" void sceDmaReset(int mode);
extern "C" void sceGsResetPath();
extern "C" void sceGsResetGraph(short mode, short inter, short omode, short ffmode);

struct sceGsDispEnv {
    u_long pmode;
    u_long smode2;
    u_long dispfb;
    u_long display;
    u_long bgcolor;
};
struct sceGsDrawEnv12 {
    u_long reg[8][2];
};
struct sceGsClear {
    u_long testa, testaaddr;
    u_long prim, primaddr;
    u_long rgbaq, rgbaqaddr;
    u_long xyz2a, xyz2aaddr;
    u_long xyz2b, xyz2baddr;
    u_long testb, testbaddr;
};
struct sceGsDBuffDc {
    sceGsDispEnv disp[2];
    u_long giftag0[2];
    sceGsDrawEnv12 draw01;
    sceGsDrawEnv12 draw02;
    sceGsClear clear0;
    u_long giftag1[2];
    sceGsDrawEnv12 draw11;
    sceGsDrawEnv12 draw12;
    sceGsClear clear1;
};

struct GpuRegsInfoSep {
    ULONGLONG _nCLAMP;
    ULONGLONG _nSCISSOR;
    ULONGLONG _nALPHA;
    ULONGLONG _nTEST;
    ULONGLONG _nFBA;
    ULONGLONG _nZBUF;
    ULONGLONG _nFRAME;
};

class CDelayedFree { /* PARTIAL: stand-in for System/LinkedBuf CDelayedFree: strings: LinkedBuf.h (.rodata still asm) */
public:
    static void disposeFreeList();
};

class CPackMan {
public:
    static void finalize();
    static void transFrame();
    static void transFrameEffect();
    static void initSemiPacket();
    static void enableZWrite(BOOL inEnable, INT inAddr);
    static void resetGsDma();
    static void setFrameEffect(INT nKind, INT n1, INT n2, INT n3);
    static void terminateSemiPacket();
    static void resetDmaChanVif1();
    static INT getDBIdx();
    static BOOL isPacketForBG();
    static BOOL isStartStillMode();
    static void setMovieEnv(UINT nDrawW, UINT nDrawH, UINT nDispW, UINT nDispH);
    static void updateBGBlend(INT nAlpha);
    static ULONGLONG getDefaultTEST();
    static GpuRegsInfoSep* getSysGpuRegs2();
    static GpuRegsInfoSep* getDefaultGpuRegs2();
    static ULONGLONG getSys_CLAMP_2();
    static ULONGLONG getSys_SCISSOR_2();
    static ULONGLONG getSys_ALPHA_2();
    static ULONGLONG getSys_TEST_2();
    static ULONGLONG getSys_FBA_2();
    static ULONGLONG getSys_ZBUF_2();
    static ULONGLONG getSys_FRAME_2();
    static ULONGLONG getDefault_CLAMP_2();
    static ULONGLONG getDefault_SCISSOR_2();
    static ULONGLONG getDefault_ALPHA_2();
    static ULONGLONG getDefault_TEST_2();
    static ULONGLONG getDefault_FBA_2();
    static ULONGLONG getDefault_ZBUF_2();
    static ULONGLONG getDefault_FRAME_2();
    static ULONGLONG getDefault_TEST_1();
    static ULONGLONG getDefault_ZBUF_1();
    static ULONGLONG getDefault_FRAME_1();
    static void setSys_CLAMP_2(ULONGLONG n);
    static void setSys_SCISSOR_2(ULONGLONG n);
    static void setSys_ALPHA_2(ULONGLONG n);
    static void setSys_TEST_2(ULONGLONG n);
    static void setSys_FBA_2(ULONGLONG n);
    static void setDefault_CLAMP_2(ULONGLONG n);
    static void setDefault_SCISSOR_2(ULONGLONG n);
    static void setDefault_ALPHA_2(ULONGLONG n);
    static void setDefault_TEST_2(ULONGLONG n);
    static void setDefault_FBA_2(ULONGLONG n);
    static void setDefault_ZBUF_2(ULONGLONG n);
    static u_long* getPktVif1Frame();
    static INT getTransFrameKind();
    static void enableDraw(INT nBit);
    static void disableDraw(INT nBit);
    static UINT getEnableDraw();
    static void changeDrawMode(INT nMode);
    static BOOL isSkipPacketDMA();
    static void changePcrtcMode(INT nMode);
    static INT getPcrtcMode();
    static void setBGColor(INT nColor);
    static INT getBGColor();
    static void setDisp1Alpha(INT nAlpha);
    static INT getDisp1Alpha();
    static void startCountTmpSize();
    static UINT getTmpCountSize();
    static void setFGPacketMaxNorml();
    static void extFGPacketMaxForFlex();
    static INT getPkUseSize();
    static INT getPkTotalSize();
    static void pkReset();
    static void pkNext(u_long128* pNext, UINT flag);
    static void pkRet(UINT flag);
    static void pkEnd(UINT flag);
    static void pkRefe(u_long128* pRef, UINT size, UINT opt1, UINT opt2, UINT flag);
    static void pkRefs(u_long128* pRef, UINT size, UINT opt1, UINT opt2, UINT flag);
    static void pkRefLoadImage(const VTransInfo& info);
    static void pkRefLoadImage(u_short bp, const VTransInfo& info);
    static void pkRefLoadImage(u_short bp, u_char psm, u_short bw, u_long128* image, UINT size, UINT x, UINT y, UINT w, UINT h);
    static INT getDrawMode();
    static BOOL isOverFlow(UINT nSize);
    static u_int* pkOpenUpkCodeV4_32(u_short vuaddr, UINT cl, UINT wl);
    static void pkRefMpg(u_short vuaddr, u_long128* pMicro, UINT size, UINT opt1);
    static void pkInit(u_long128* pBase, pacVif1Packet* pPkt, UINT nMax);
    static u_int* pkOpenUpkCode(u_short vuaddr, UINT upkcmd, UINT cl, UINT wl);
    static void saveScreen();
    static void pkOpenDirectCode(INT stall);
    static void pkCloseDirectCode();
    static void pkCloseDirectHLCode();
    static void pkTerminate();
    static void pkCnt(UINT flag);
    static void pkCall(u_long128* pCall, UINT flag);
    static void pkRef(u_long128* pRef, UINT size, UINT opt1, UINT opt2, UINT flag);
    static UINT pkSize();
    static void* pkGetCurrentPointer();
    static void pkSetCurrentPointer(void* p);
    static void pkCloseUpkCode(void* p);
    static void pkCloseUpkCode();
    static void pkCloseUpkCodeV4_32(void* p);
    static void pkCloseUpkCodeV4_32();
    static void pkOpenDirectHLCode(INT stall);
    static u_long128* pkOpenGifTag(u_long128 tag);
    static void pkCloseGifTag(void* p);
    static void pkCloseGifTag();
    static u_long128* pkOpenGifTagManual(u_long128 tag);
    static void pkCloseGifTagManual(UINT* p);
    static u_int* pkReserve(UINT count);
    static void pkAlign(UINT bit, UINT pos);
    static void pkAddCode(u_int data);
    static void pkAddData(u_int data);
    static void pkAddDataN(u_int* pData, UINT count);
    static void pkAddUpkData32(u_int data);
    static void pkAddUpkData64(u_long data);
    static void pkAddUpkData128(u_long128 data);
    static void pkAddUpkData32N(u_int* pData, UINT count);
    static void pkAddUpkData64N(u_long* pData, UINT count);
    static void pkAddUpkData128N(u_long128* pData, UINT count);
    static void pkAddDirectData(u_long128 data);
    static void pkAddDirectDataN(u_long128* pData, UINT count);
    static void pkAddGsData(u_long data);
    static void pkAddGsDataN(u_long* pData, UINT count);
    static void pkAddGsPacked(u_long128 data);
    static void pkAddGsPackedN(u_long128* pData, UINT count);
    static void pkAddGsAD(UINT addr, u_long data);
};

void sceVif1PkInit(pacVif1Packet* pPacket, u_long128* pBase);
void sceVif1PkReset(pacVif1Packet* pPacket);
u_int sceVif1PkSize(pacVif1Packet* pPacket);
void sceVif1PkCloseUpkCode(pacVif1Packet* pPacket);
void sceVif1PkCloseUpkCodeV4_32(pacVif1Packet* pPacket);
void sceVif1PkOpenDirectHLCode(pacVif1Packet* pPacket, int stall);
void sceVif1PkOpenGifTag(pacVif1Packet* pPacket, u_long128 tag);
void sceVif1PkCloseGifTag(pacVif1Packet* pPacket);
u_int* sceVif1PkReserve(pacVif1Packet* pPacket, u_int count);
void sceVif1PkAlign(pacVif1Packet* pPacket, u_int bit, u_int pos);
void sceVif1PkAddCode(pacVif1Packet* pPacket, u_int code);
void sceVif1PkAddData(pacVif1Packet* pPacket, u_int data);
void sceVif1PkAddDataN(pacVif1Packet* pPacket, u_int* pData, u_int count);
void sceVif1PkAddUpkData32(pacVif1Packet* pPacket, u_int data);
void sceVif1PkAddUpkData32N(pacVif1Packet* pPacket, u_int* pData, u_int count);
void sceVif1PkAddUpkData64(pacVif1Packet* pPacket, u_long data);
void sceVif1PkAddUpkData64N(pacVif1Packet* pPacket, u_long* pData, u_int count);
void sceVif1PkAddUpkData128(pacVif1Packet* pPacket, u_long128 data);
void sceVif1PkAddUpkData128N(pacVif1Packet* pPacket, u_long128* pData, u_int count);
void sceVif1PkAddDirectData(pacVif1Packet* pPacket, u_long128 data);
void sceVif1PkAddDirectDataN(pacVif1Packet* pPacket, u_long128* pData, u_int count);
void sceVif1PkAddGsData(pacVif1Packet* pPacket, u_long data);
void sceVif1PkAddGsDataN(pacVif1Packet* pPacket, u_long* pData, u_int count);
void sceVif1PkAddGsPacked(pacVif1Packet* pPacket, u_long128 data);
void sceVif1PkAddGsPackedN(pacVif1Packet* pPacket, u_long128* pData, u_int count);
void sceVif1PkAddGsAD(pacVif1Packet* pPacket, u_int addr, u_long data);
u_long128* sceVif1PkTerminate(pacVif1Packet* pPacket);
void sceVif1PkCnt(pacVif1Packet* pPacket, u_int flag);
void sceVif1PkCall(pacVif1Packet* pPacket, u_long128* pCall, u_int flag);
void sceVif1PkRef(pacVif1Packet* pPacket, u_long128* pRef, u_int size, u_int opt1, u_int opt2, u_int flag);
void sceVif1PkRefMpg(pacVif1Packet* pPacket, u_short vuaddr, u_long128* pMicro, u_int size, u_int opt1);
void sceVif1PkOpenUpkCode(pacVif1Packet* pPacket, u_short vuaddr, u_int upkcmd, u_int cl, u_int wl);
void sceVif1PkOpenUpkCodeV4_32(pacVif1Packet* pPacket, u_short vuaddr, u_int cl, u_int wl);
void sceVif1PkOpenDirectCode(pacVif1Packet* pPacket, int stall);
void sceVif1PkCloseDirectCode(pacVif1Packet* pPacket);
void sceVif1PkCloseDirectHLCode(pacVif1Packet* pPacket);
void sceVif1PkNext(pacVif1Packet* pPacket, u_long128* pNext, u_int flag);
void sceVif1PkRet(pacVif1Packet* pPacket, u_int flag);
void sceVif1PkEnd(pacVif1Packet* pPacket, u_int flag);
void sceVif1PkRefe(pacVif1Packet* pPacket, u_long128* pRef, u_int size, u_int opt1, u_int opt2, u_int flag);
void sceVif1PkRefs(pacVif1Packet* pPacket, u_long128* pRef, u_int size, u_int opt1, u_int opt2, u_int flag);
void sceVif1PkRefLoadImage(pacVif1Packet* pPacket, u_short bp, u_char psm, u_short bw, u_long128* image, u_int size, u_int x, u_int y, u_int w, u_int h);
void _makePkt_SaveScreen(); /* static in the original (still INCLUDE_ASM) */
extern "C" void FlushCache(int op); /* MATCH: void (the SDK returns int): v0 stays free for the lui after the call in saveScreen */
extern "C" void sceDmaSend(sceDmaChan* d, void* tag);
extern "C" int sceGsSyncPath(int mode, u_short timeout);
extern "C" void TRACE(char* fmt, ...);
typedef struct { /* 0x60 */ /* PARTIAL: stand-in for System/Graphic/Effect/libppp/pppPart sceGsLoadImage: libgraph.h not written */
    u_long giftag0[2];
    u_long bitbltbuf;
    u_long bitbltbufaddr;
    u_long trxpos;
    u_long trxposaddr;
    u_long trxreg;
    u_long trxregaddr;
    u_long trxdir;
    u_long trxdiraddr;
    u_long giftag1[2];
} sceGsLoadImage;
extern "C" int sceGsSetDefLoadImage(sceGsLoadImage* lp, short dbp, short dbw, short dpsm, short x, short y, short w, short h);
extern "C" int sceGsExecLoadImage(sceGsLoadImage* lp, u_long128* srcaddr);
extern "C" char D_00461B70[], D_00461C28[]; /* PARTIAL: assert text "0" and the semi-packet overflow message (data still asm) */
extern u_long128 My_dma_start[];
extern u_long128 My_VU0_start[];
extern INT SCREEN_HEIGHT;
class CMainApp { /* PARTIAL: stand-in for System/MainApp CMainApp: strings: MainApp.h (.rodata still asm) */
public:
    static BOOL isNTSC();
};
class CModel { /* PARTIAL: stand-in for System/Graphic/Model CModel: strings: Model.h (.rodata still asm) */
public:
    static void resetMicroProgram();
};
extern "C" void AssertApp(const char* pFile, int nLine, const char* pExpr, int n);
extern "C" char D_00461BE0[], D_00461C10[]; /* PARTIAL: __FILE__ and the assert text (data still asm) */
static void _setPcrtcMode_BlendBG(sceGsDispEnv& inEnv);

static inline u_long _upper(u_long128 q) {
    u_long r;
    __asm__ volatile("pcpyud %0, %1, $0" : "=r"(r) : "r"(q));
    return r;
}

static inline u_long128 _pack(u_long lo, u_long hi) {
    u_long128 r;
    __asm__ volatile("pcpyld %0, %2, %1" : "=r"(r) : "r"(lo), "r"(hi));
    return r;
}

namespace {
extern pacVif1Packet* s_pPktVif1;
}

#line 179
static inline void _checkSize(UINT iSize) {
    if (!(!(iSize >= 0x10000))) AssertApp(D_00461BE0, __LINE__, D_00461C10, 0);
}
static inline void _checkDmaTag() {
    pacVif1Packet* pPkt = s_pPktVif1;
    if (pPkt->pDmaTag) _checkSize((u_long128*)(((UINT)pPkt->pCurrent + 15) & ~15) - pPkt->pDmaTag - 1);
}

/* The file's statics; the data still comes from the original (no SOURCE DATA yet) */
namespace {
extern u_long s_pPktBufVif1[2][36992];
extern u_long s_pPktBufVif1Frame[320];
extern pacVif1Packet s_pktVif1;
extern pacVif1Packet s_pktVif1BG;
extern INT s_nMaxPktSizeVif1Semi;
extern u_long128* s_pktVif1SemiBase;
extern pacVif1Packet s_pktVif1Semi;
extern pacVif1Packet s_pktVif1FrameEff;
extern pacVif1Packet s_pktVif1Frame;
extern sceDmaChan* s_pDmaChanVif1;
extern sceDmaChan* s_pDmaChanVif0;
extern sceDmaChan* s_pDmaChanGif;
extern pacVif1Packet* s_pPktVif1;
extern UINT s_nMaxPktSizeVif1;
extern UINT s_nMaxPktSizeVif1Normal;
extern INT s_nMaxPktSizeVif1FG;
extern UINT s_nTmpPacketSize;
extern INT s_nDB;
extern sceGsDBuffDc s_DBuf;
extern SHORT s_nReqDrawMode;
extern SHORT s_nDrawMode;
extern SHORT s_nReqPcrtcMode;
extern SHORT s_nPcrtcMode;
extern INT s_nDispOffsetX;
extern INT s_nDispOffsetY;
extern INT s_nBGColor;
extern INT s_nDisp1Alpha;
extern INT s_nFSAA_Sharpness;
extern INT s_transFrameKind;
extern GpuRegsInfoSep s_SysGpuRegs2;
extern GpuRegsInfoSep s_DefaultGpuRegs2;
extern GpuRegsInfoSep s_DefaultGpuRegs1;
extern UINT s_nDawEnableBit;
extern UINT s_nMovieDrawW;
extern UINT s_nMovieDrawH;
extern UINT s_nMovieDispW;
extern UINT s_nMovieDispH;
extern INT s_nPkUseSize;
}

/* MATCH: pkInit and the init*Packet functions inline one helper (the ref lists its parameters as pkInit's locals) */
static inline void _pkInit(u_long128* pInBase, pacVif1Packet* pPkt, UINT nInMaxPkt) {
    s_pPktVif1 = pPkt ? pPkt : &s_pktVif1;
    s_nMaxPktSizeVif1 = nInMaxPkt;
    sceVif1PkInit(s_pPktVif1, pInBase);
}
void CPackMan::pkInit(u_long128* pBase, pacVif1Packet* pPkt, UINT nMax) {
    UINT nSize = nMax ? nMax : 0x8000;
    _pkInit(pBase, pPkt, nSize);
}
void CPackMan::pkReset() { sceVif1PkReset(s_pPktVif1); }
UINT CPackMan::pkSize() { return sceVif1PkSize(s_pPktVif1); }
void CPackMan::pkTerminate() {
    _checkDmaTag();
    sceVif1PkTerminate(s_pPktVif1);
}
void CPackMan::pkCnt(UINT flag) {
    _checkDmaTag();
    sceVif1PkCnt(s_pPktVif1, flag);
}
void CPackMan::pkRet(UINT flag) {
    _checkDmaTag();
    sceVif1PkRet(s_pPktVif1, flag);
}
void CPackMan::pkEnd(UINT flag) {
    _checkDmaTag();
    sceVif1PkEnd(s_pPktVif1, flag);
}
void CPackMan::pkNext(u_long128* pNext, UINT flag) {
    _checkDmaTag();
    sceVif1PkNext(s_pPktVif1, pNext, flag);
}
void CPackMan::pkCall(u_long128* pCall, UINT flag) {
    _checkDmaTag();
    sceVif1PkCall(s_pPktVif1, pCall, flag);
}
void CPackMan::pkRefe(u_long128* pRef, UINT size, UINT opt1, UINT opt2, UINT flag) {
    _checkDmaTag();
    _checkSize(size);
    sceVif1PkRefe(s_pPktVif1, pRef, size, opt1, opt2, flag);
}
void CPackMan::pkRef(u_long128* pRef, UINT size, UINT opt1, UINT opt2, UINT flag) {
    _checkDmaTag();
    _checkSize(size);
    sceVif1PkRef(s_pPktVif1, pRef, size, opt1, opt2, flag);
}
void CPackMan::pkRefs(u_long128* pRef, UINT size, UINT opt1, UINT opt2, UINT flag) {
    _checkDmaTag();
    _checkSize(size);
    sceVif1PkRefs(s_pPktVif1, pRef, size, opt1, opt2, flag);
}
void* CPackMan::pkGetCurrentPointer() { return s_pPktVif1->pCurrent; }
void CPackMan::pkSetCurrentPointer(void* p) { s_pPktVif1->pCurrent = (u_int*)p; }
u_int* CPackMan::pkOpenUpkCode(u_short vuaddr, UINT upkcmd, UINT cl, UINT wl) {
    pacVif1Packet* pPkt = s_pPktVif1;
    sceVif1PkOpenUpkCode(pPkt, vuaddr, upkcmd, cl, wl);
    return pPkt->pCurrent;
}
void CPackMan::pkCloseUpkCode(void* p) {
    s_pPktVif1->pCurrent = (u_int*)p;
    sceVif1PkCloseUpkCode(s_pPktVif1);
}
void CPackMan::pkCloseUpkCode() { sceVif1PkCloseUpkCode(s_pPktVif1); }
u_int* CPackMan::pkOpenUpkCodeV4_32(u_short vuaddr, UINT cl, UINT wl) {
    pacVif1Packet* pPkt = s_pPktVif1;
    sceVif1PkOpenUpkCodeV4_32(pPkt, vuaddr, cl, wl);
    return pPkt->pCurrent;
}
void CPackMan::pkCloseUpkCodeV4_32(void* p) {
    s_pPktVif1->pCurrent = (u_int*)p;
    sceVif1PkCloseUpkCodeV4_32(s_pPktVif1);
}
void CPackMan::pkCloseUpkCodeV4_32() { sceVif1PkCloseUpkCodeV4_32(s_pPktVif1); }
void CPackMan::pkOpenDirectCode(INT stall) {
    sceVif1PkOpenDirectCode(s_pPktVif1, stall);
    *(u_long*)s_pPktVif1->pCurrent = 0x8000;
    s_pPktVif1->pCurrent += 4;
}
void CPackMan::pkCloseDirectCode() {
    u_int* pEnd = s_pPktVif1->pCurrent - 1;
    _checkSize((u_long128*)pEnd - (u_long128*)s_pPktVif1->pVifCode);
    sceVif1PkCloseDirectCode(s_pPktVif1);
}
void CPackMan::pkOpenDirectHLCode(INT stall) { sceVif1PkOpenDirectHLCode(s_pPktVif1, stall); }
void CPackMan::pkCloseDirectHLCode() {
    u_int* pEnd = s_pPktVif1->pCurrent - 1;
    _checkSize((u_long128*)pEnd - (u_long128*)s_pPktVif1->pVifCode);
    sceVif1PkCloseDirectHLCode(s_pPktVif1);
}
/* MATCH: pkOpenGifTag/pkCloseGifTag(void*) are inlined by enableZWrite (the ref lists pPkt and p as its locals) */
static inline u_long128* _pkOpenGifTag(u_long128 tag) {
    pacVif1Packet* pPkt = s_pPktVif1;
    sceVif1PkOpenGifTag(pPkt, tag);
    return (u_long128*)pPkt->pCurrent;
}
static inline void _pkCloseGifTag(void* p) {
    s_pPktVif1->pCurrent = (u_int*)p;
    sceVif1PkCloseGifTag(s_pPktVif1);
}
u_long128* CPackMan::pkOpenGifTag(u_long128 tag) { return _pkOpenGifTag(tag); }
void CPackMan::pkCloseGifTag(void* p) { _pkCloseGifTag(p); }
void CPackMan::pkCloseGifTag() { sceVif1PkCloseGifTag(s_pPktVif1); }
u_long128* CPackMan::pkOpenGifTagManual(u_long128 tag) {
    u_long128* p = (u_long128*)s_pPktVif1->pCurrent;
    *p = tag;
    return p + 1;
}
void CPackMan::pkCloseGifTagManual(UINT* p) { s_pPktVif1->pCurrent = (u_int*)((UINT)p + ((UINT)p & 8)); }
u_int* CPackMan::pkReserve(UINT count) { return sceVif1PkReserve(s_pPktVif1, count); }
void CPackMan::pkAlign(UINT bit, UINT pos) { sceVif1PkAlign(s_pPktVif1, bit, pos); }
void CPackMan::pkAddCode(u_int data) { sceVif1PkAddCode(s_pPktVif1, data); }
void CPackMan::pkAddData(u_int data) { sceVif1PkAddData(s_pPktVif1, data); }
void CPackMan::pkAddDataN(u_int* pData, UINT count) { sceVif1PkAddDataN(s_pPktVif1, pData, count); }
void CPackMan::pkAddUpkData32(u_int data) { sceVif1PkAddUpkData32(s_pPktVif1, data); }
void CPackMan::pkAddUpkData64(u_long data) { sceVif1PkAddUpkData64(s_pPktVif1, data); }
void CPackMan::pkAddUpkData128(u_long128 data) { sceVif1PkAddUpkData128(s_pPktVif1, data); }
void CPackMan::pkAddUpkData32N(u_int* pData, UINT count) { sceVif1PkAddUpkData32N(s_pPktVif1, pData, count); }
void CPackMan::pkAddUpkData64N(u_long* pData, UINT count) { sceVif1PkAddUpkData64N(s_pPktVif1, pData, count); }
void CPackMan::pkAddUpkData128N(u_long128* pData, UINT count) { sceVif1PkAddUpkData128N(s_pPktVif1, pData, count); }
void CPackMan::pkAddDirectData(u_long128 data) { sceVif1PkAddDirectData(s_pPktVif1, data); }
void CPackMan::pkAddDirectDataN(u_long128* pData, UINT count) { sceVif1PkAddDirectDataN(s_pPktVif1, pData, count); }
void CPackMan::pkAddGsData(u_long data) { sceVif1PkAddGsData(s_pPktVif1, data); }
void CPackMan::pkAddGsDataN(u_long* pData, UINT count) { sceVif1PkAddGsDataN(s_pPktVif1, pData, count); }
void CPackMan::pkAddGsPacked(u_long128 data) { sceVif1PkAddGsPacked(s_pPktVif1, data); }
void CPackMan::pkAddGsPackedN(u_long128* pData, UINT count) { sceVif1PkAddGsPackedN(s_pPktVif1, pData, count); }
void CPackMan::pkAddGsAD(UINT addr, u_long data) { sceVif1PkAddGsAD(s_pPktVif1, addr, data); }
/* MATCH: the three pkRefLoadImage overloads inline one helper (the ref lists its parameters as their locals) */
static inline void _pkRefLoadImage(USHORT nInBp, BYTE cInPsm, USHORT nInBw, u_long128* pInImage, UINT nInSize, UINT nInX, UINT nInY,
                                   UINT nInW, UINT nInH) {
    sceVif1PkRefLoadImage(s_pPktVif1, nInBp, cInPsm, nInBw, pInImage, nInSize, nInX, nInY, nInW, nInH);
}
void CPackMan::pkRefLoadImage(u_short bp, u_char psm, u_short bw, u_long128* image, UINT size, UINT x, UINT y, UINT w, UINT h) {
    _pkRefLoadImage(bp, psm, bw, image, size, x, y, w, h);
}
void CPackMan::pkRefLoadImage(const VTransInfo& info) {
    _pkRefLoadImage(info._nDistPos, info._nFormat, info._nDistBufW, (u_long128*)info._pImage, info._qwImage, info._nDistX,
                   info._nDistY, info._nDistW, info._nDistH);
}
void CPackMan::pkRefLoadImage(u_short nBP, const VTransInfo& info) {
    _pkRefLoadImage(nBP, info._nFormat, info._nDistBufW, (u_long128*)info._pImage, info._qwImage, info._nDistX, info._nDistY,
                   info._nDistW, info._nDistH);
}
void CPackMan::pkRefMpg(u_short vuaddr, u_long128* pMicro, UINT size, UINT opt1) { sceVif1PkRefMpg(s_pPktVif1, vuaddr, pMicro, size, opt1); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", initialize__8CPackMan);
void CPackMan::finalize() { CDelayedFree::disposeFreeList(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _initGsDefDrawEnv__FR12sceGsDBuffDc12_GS_BUF_SIDEsssssUiUii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _initDBuf__Fv);
void CPackMan::resetGsDma() {
    sceGsResetPath();
    sceDmaReset(1);
    if (CMainApp::isNTSC()) sceGsResetGraph(0, 1, 2, 0);
    else sceGsResetGraph(0, 1, 3, 0);
    s_pDmaChanVif0 = sceDmaGetChan(0);
    *(u_int*)&s_pDmaChanVif0->chcr |= 0x40;
    s_pDmaChanVif1 = sceDmaGetChan(1);
    *(u_int*)&s_pDmaChanVif1->chcr |= 0x40;
    s_pDmaChanGif = sceDmaGetChan(2);
    *(int*)&s_pDmaChanGif->chcr &= ~0x40;
    *(volatile u_int*)0x10003010 = 4;
}
void CPackMan::resetDmaChanVif1() {
    sceDmaChan* pChan = sceDmaGetChan(1);
    s_pDmaChanVif1 = pChan;
    *(volatile u_int*)&pChan->chcr |= 0x40;
}
static void _clearVram() {
    sceGsLoadImage li;
    u_long128* p = (u_long128*)s_pPktBufVif1;
    UINT i;
    for (i = 0; i < 0x800; i++) p[i] = 0;
    for (i = 0; i < 0x80; i++) {
        sceGsSetDefLoadImage(&li, i * 128, 2, 0, 0, 0, 128, 64);
        FlushCache(0);
        sceGsExecLoadImage(&li, p);
        sceGsSyncPath(0, 0);
    }
}
static void _setupSysContext() {
    s_SysGpuRegs2._nCLAMP = 5;
    s_SysGpuRegs2._nSCISSOR = ((u_long)(SCREEN_HEIGHT - 1) << 48) | (511 << 16);
    s_SysGpuRegs2._nALPHA = 0x44;
    s_SysGpuRegs2._nTEST = *(u_long*)&s_DBuf.draw02.reg[7];
    s_SysGpuRegs2._nFBA = 0;
    s_SysGpuRegs2._nZBUF = *(u_long*)&s_DBuf.draw02.reg[1];
    s_SysGpuRegs2._nFRAME = *(u_long*)&s_DBuf.draw02.reg[0];
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", beginScene__8CPackMan);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", endScene__8CPackMan);
INT CPackMan::getDBIdx() { return s_nDB & 1; }
void CPackMan::transFrameEffect() {
    if (s_nDrawMode != 0) return;
    if (s_transFrameKind >= 4) return;
    if (s_transFrameKind > 0) {
        *(u_int*)&s_pDmaChanVif1->chcr |= 0x40;
        FlushCache(0);
        sceDmaSend(s_pDmaChanVif1, (void*)((u_int)s_pktVif1FrameEff.pBase & 0x0fffffff));
        sceGsSyncPath(0, 0);
    }
}
void CPackMan::transFrame() {
    if (s_nDrawMode == 3) return;
    *(u_int*)&s_pDmaChanVif1->chcr |= 0x40;
    FlushCache(0);
    sceDmaSend(s_pDmaChanVif1, (void*)((u_int)s_pktVif1Frame.pBase & 0x0fffffff));
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", initBGPacket__8CPackMani);
BOOL CPackMan::isPacketForBG() { return s_pPktVif1 == &s_pktVif1BG; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", terminateBGPacket__8CPackMan);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", changeLayer__8CPackMani);
void CPackMan::initSemiPacket() {
    if (!s_pktVif1SemiBase) return;
    _pkInit(s_pktVif1SemiBase, &s_pktVif1Semi, s_nMaxPktSizeVif1Semi);
    sceVif1PkReset(s_pPktVif1);
}
void CPackMan::terminateSemiPacket() {
    INT nSize = sceVif1PkSize(s_pPktVif1);
    s_nPkUseSize += nSize;
    if (s_nMaxPktSizeVif1Semi < nSize) {
        TRACE(D_00461C28, nSize);
        AssertApp(D_00461BE0, 1682, D_00461B70, 0);
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", initFGPacket__8CPackMan);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", terminateFGPacket__8CPackMan);
BOOL CPackMan::isStartStillMode() { return s_nReqDrawMode == 1 && s_nDrawMode == 0; }
void CPackMan::saveScreen() {
    _makePkt_SaveScreen();
    FlushCache(0);
    sceDmaSend(s_pDmaChanVif1, (void*)((u_int)s_pktVif1.pBase & 0x0fffffff));
}
void CPackMan::setMovieEnv(UINT nDrawW, UINT nDrawH, UINT nDispW, UINT nDispH) {
    s_nMovieDrawW = nDrawW;
    s_nMovieDrawH = nDrawH;
    s_nMovieDispW = nDispW;
    s_nMovieDispH = nDispH;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _makePkt_SaveScreen__Fv);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _makePkt_StillScreen__FPUx);
static void _setPcrtcMode_FSAA(sceGsDispEnv& inEnv) {
    ULONGLONG nPmode = ((ULONGLONG)(s_nFSAA_Sharpness + 0xc0) << 8) | 0x63;
    *(volatile u_long*)0x12000000 = nPmode;
    *(volatile u_long*)0x12000070 = inEnv.dispfb;
    *(volatile u_long*)0x12000090 = inEnv.dispfb;
    *(volatile INT*)&s_nBGColor = 0; /* MATCH: volatile store keeps its lui first (original lui order) */
    ULONGLONG nDisplay2 = inEnv.display + s_nDispOffsetX + ((ULONGLONG)s_nDispOffsetY << 12);
    *(volatile u_long*)0x120000a0 = nDisplay2;
    *(volatile u_long*)0x12000080 = nDisplay2 - 0x1000;
}
static void _setPcrtcMode_BlendBG(sceGsDispEnv& inEnv) {
    ULONGLONG nPmode = ((ULONGLONG)s_nDisp1Alpha << 8) | 0xa1;
    *(volatile u_long*)0x12000000 = nPmode;
    *(volatile u_long*)0x12000070 = inEnv.dispfb;
    *(volatile u_long*)0x12000090 = inEnv.dispfb;
    ULONGLONG nDisplay1 = inEnv.display;
    nDisplay1 += s_nDispOffsetX;
    nDisplay1 += (ULONGLONG)(s_nDispOffsetY - 1) << 12;
    *(volatile u_long*)0x12000080 = nDisplay1;
}
void CPackMan::updateBGBlend(INT nAlpha) {
    s_nDisp1Alpha = nAlpha;
    _setPcrtcMode_BlendBG(s_DBuf.disp[0]);
}
ULONGLONG CPackMan::getDefaultTEST() { return s_DBuf.clear0.testb; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", swapDBuf__8CPackMani);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", kickDMA__8CPackMani);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", kickDMAFG__8CPackMani);
static void _loadMicroProgram() {
    FlushCache(0);
    sceGsSyncPath(0, 0);
    sceDmaSend(s_pDmaChanVif1, My_dma_start);
    sceDmaSend(s_pDmaChanVif0, My_VU0_start);
    CModel::resetMicroProgram();
}
void _initFramePacket(INT nKind);                          /* static in the original (still INCLUDE_ASM) */
void _initFramePacketForMovie(INT n);                       /* static in the original (still INCLUDE_ASM) */
void _initFrameEffPacket(INT nKind, INT n1, INT n2, INT n3); /* static in the original (still INCLUDE_ASM) */
void CPackMan::setFrameEffect(INT nKind, INT n1, INT n2, INT n3) {
    if (nKind >= 6) return;
    INT nMode = getDrawMode();
    if (nKind == 5) {
        if (nMode != 0 && nMode != 2) return;
        nKind = 0;
    } else if (nMode == 2 && nKind != 4) {
        return;
    }
    s_transFrameKind = nKind;
    if (nKind == 4) _initFramePacketForMovie(n3);
    else _initFramePacket(nKind);
    _initFrameEffPacket(nKind, n1, n2, n3);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _initFramePacket__Fi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _initFramePacketForMovie__Fi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _initFrameEffPacket__Fiiii);
void CPackMan::enableZWrite(BOOL inEnable, INT inAddr) {
    QWdata* qp = (QWdata*)_pkOpenGifTag(_pack(0x1000800000008000, 0xe));
    u_long nZbuf = (VRAM::_ZBUF_START / 32) | 0x31000000;
    if (inEnable != 1) nZbuf |= (u_long)1 << 32;
    *(u_long128*)qp = _pack(nZbuf, inAddr);
    qp++;
    _pkCloseGifTag(qp);
}
GpuRegsInfoSep* CPackMan::getSysGpuRegs2() { return &s_SysGpuRegs2; }
GpuRegsInfoSep* CPackMan::getDefaultGpuRegs2() { return &s_DefaultGpuRegs2; }
ULONGLONG CPackMan::getSys_CLAMP_2() { return s_SysGpuRegs2._nCLAMP; }
ULONGLONG CPackMan::getSys_SCISSOR_2() { return s_SysGpuRegs2._nSCISSOR; }
ULONGLONG CPackMan::getSys_ALPHA_2() { return s_SysGpuRegs2._nALPHA; }
ULONGLONG CPackMan::getSys_TEST_2() { return s_SysGpuRegs2._nTEST; }
ULONGLONG CPackMan::getSys_FBA_2() { return s_SysGpuRegs2._nFBA; }
ULONGLONG CPackMan::getSys_ZBUF_2() { return s_SysGpuRegs2._nZBUF; }
ULONGLONG CPackMan::getSys_FRAME_2() { return s_SysGpuRegs2._nFRAME; }
ULONGLONG CPackMan::getDefault_CLAMP_2() { return s_DefaultGpuRegs2._nCLAMP; }
ULONGLONG CPackMan::getDefault_SCISSOR_2() { return s_DefaultGpuRegs2._nSCISSOR; }
ULONGLONG CPackMan::getDefault_ALPHA_2() { return s_DefaultGpuRegs2._nALPHA; }
ULONGLONG CPackMan::getDefault_TEST_2() { return s_DefaultGpuRegs2._nTEST; }
ULONGLONG CPackMan::getDefault_FBA_2() { return s_DefaultGpuRegs2._nFBA; }
ULONGLONG CPackMan::getDefault_ZBUF_2() { return s_DefaultGpuRegs2._nZBUF; }
ULONGLONG CPackMan::getDefault_FRAME_2() { return s_DefaultGpuRegs2._nFRAME; }
ULONGLONG CPackMan::getDefault_TEST_1() { return s_DefaultGpuRegs1._nTEST; }
ULONGLONG CPackMan::getDefault_ZBUF_1() { return s_DefaultGpuRegs1._nZBUF; }
ULONGLONG CPackMan::getDefault_FRAME_1() { return s_DefaultGpuRegs1._nFRAME; }
void CPackMan::setSys_CLAMP_2(ULONGLONG n) { s_SysGpuRegs2._nCLAMP = n; }
void CPackMan::setSys_SCISSOR_2(ULONGLONG n) { s_SysGpuRegs2._nSCISSOR = n; }
void CPackMan::setSys_ALPHA_2(ULONGLONG n) { s_SysGpuRegs2._nALPHA = n; }
void CPackMan::setSys_TEST_2(ULONGLONG n) { s_SysGpuRegs2._nTEST = n; }
void CPackMan::setSys_FBA_2(ULONGLONG n) { s_SysGpuRegs2._nFBA = n; }
void CPackMan::setDefault_CLAMP_2(ULONGLONG n) { s_DefaultGpuRegs2._nCLAMP = n; }
void CPackMan::setDefault_SCISSOR_2(ULONGLONG n) { s_DefaultGpuRegs2._nSCISSOR = n; }
void CPackMan::setDefault_ALPHA_2(ULONGLONG n) { s_DefaultGpuRegs2._nALPHA = n; }
void CPackMan::setDefault_TEST_2(ULONGLONG n) { s_DefaultGpuRegs2._nTEST = n; }
void CPackMan::setDefault_FBA_2(ULONGLONG n) { s_DefaultGpuRegs2._nFBA = n; }
void CPackMan::setDefault_ZBUF_2(ULONGLONG n) { s_DefaultGpuRegs2._nZBUF = n; }
u_long* CPackMan::getPktVif1Frame() { return s_pPktBufVif1Frame; }
INT CPackMan::getTransFrameKind() { return s_transFrameKind; }
void CPackMan::enableDraw(INT nBit) { s_nDawEnableBit |= nBit; }
void CPackMan::disableDraw(INT nBit) { s_nDawEnableBit &= ~nBit; }
UINT CPackMan::getEnableDraw() { return s_nDawEnableBit; }
void CPackMan::changeDrawMode(INT nMode) { s_nReqDrawMode = nMode; }
INT CPackMan::getDrawMode() {
    INT nMode = s_nDrawMode;
    switch (nMode) {
    case 3:
    case 4:
        nMode = 1;
        break;
    case 5:
        nMode = 2;
        break;
    }
    return nMode;
}
BOOL CPackMan::isSkipPacketDMA() { return s_nDrawMode == 3; }
void CPackMan::changePcrtcMode(INT nMode) { s_nReqPcrtcMode = nMode; }
INT CPackMan::getPcrtcMode() { return s_nPcrtcMode; }
void CPackMan::setBGColor(INT nColor) { s_nBGColor = nColor; }
INT CPackMan::getBGColor() { return s_nBGColor; }
void CPackMan::setDisp1Alpha(INT nAlpha) { s_nDisp1Alpha = nAlpha; }
INT CPackMan::getDisp1Alpha() { return s_nDisp1Alpha; }
BOOL CPackMan::isOverFlow(UINT nSize) { return sceVif1PkSize(s_pPktVif1) + nSize >= s_nMaxPktSizeVif1; }
void CPackMan::startCountTmpSize() { s_nTmpPacketSize = sceVif1PkSize(s_pPktVif1); }
UINT CPackMan::getTmpCountSize() { return sceVif1PkSize(s_pPktVif1) - s_nTmpPacketSize; }
void CPackMan::setFGPacketMaxNorml() {
    s_nMaxPktSizeVif1 = s_nMaxPktSizeVif1FG;
    s_nMaxPktSizeVif1Normal = s_nMaxPktSizeVif1FG;
}
void CPackMan::extFGPacketMaxForFlex() { s_nMaxPktSizeVif1Normal = s_nMaxPktSizeVif1 = s_nMaxPktSizeVif1 + 0x800; }
INT CPackMan::getPkUseSize() { return s_nPkUseSize; }
INT CPackMan::getPkTotalSize() { return 0x9080; }
void sceVif1PkInit(pacVif1Packet* pPacket, u_long128* pBase) {
    pPacket->pDmaTag = NULL;
    pPacket->pCurrent = (u_int*)(pPacket->pBase = (u_long128*)(((u_int)pBase & 0x0fffffff) | 0x30000000));
}
void sceVif1PkReset(pacVif1Packet* pPacket) {
    pPacket->pDmaTag = NULL;
    pPacket->pCurrent = (u_int*)pPacket->pBase;
}
u_int sceVif1PkSize(pacVif1Packet* pPacket) { return ((u_int)pPacket->pCurrent - (u_int)pPacket->pBase) >> 4; }
u_long128* sceVif1PkTerminate(pacVif1Packet* pPacket) {
    u_int* pc = pPacket->pCurrent;
    while ((u_int)pc & 0xc) *pc++ = 0;
    if (pPacket->pDmaTag) {
        *(u_long*)pPacket->pDmaTag = pPacket->lDmaTag.ul64[0] + (u_int)((u_long128*)pc - pPacket->pDmaTag - 1);
    }
    pPacket->pCurrent = pc;
    pPacket->pDmaTag = NULL;
    return (u_long128*)pc;
}
void sceVif1PkRefe(pacVif1Packet* pPacket, u_long128* pRef, u_int size, u_int opt1, u_int opt2, u_int flag) {
    sceVif1PkTerminate(pPacket);
    u_int tag = flag | size;
    u_int addr = (u_int)pRef & 0x8fffffff;
    u_long128 q, t;
    __asm__ volatile("pextlw %0, %2, %3\n\tpextlw %1, %4, %5\n\tpextlw %0, %1, %0" : "=&r"(q), "=&r"(t) : "r"(opt1), "r"(tag), "r"(opt2), "r"(addr));
    *(u_long128*)pPacket->pCurrent = q;
    pPacket->pCurrent += 4;
}
void sceVif1PkCnt(pacVif1Packet* pPacket, u_int flag) {
    pPacket->pDmaTag = sceVif1PkTerminate(pPacket);
    pPacket->lDmaTag.ul64[0] = (u_int)(flag | 0x10000000);
    pPacket->pCurrent += 2;
    pPacket->pVifCode = NULL;
}
void sceVif1PkNext(pacVif1Packet* pPacket, u_long128* pNext, u_int flag) {
    pPacket->pDmaTag = sceVif1PkTerminate(pPacket);
    pPacket->lDmaTag.ul64[0] = (u_int)(flag | 0x20000000) | ((u_long)((u_int)pNext & 0x8fffffff) << 32);
    pPacket->pCurrent += 2;
    pPacket->pVifCode = NULL;
}
void sceVif1PkRef(pacVif1Packet* pPacket, u_long128* pRef, u_int size, u_int opt1, u_int opt2, u_int flag) {
    sceVif1PkTerminate(pPacket);
    size |= 0x30000000;
    u_int tag = flag | size;
    u_int addr = (u_int)pRef & 0x8fffffff;
    u_long128 q, t;
    __asm__ volatile("pextlw %0, %2, %3\n\tpextlw %1, %4, %5\n\tpextlw %0, %1, %0" : "=&r"(q), "=&r"(t) : "r"(opt1), "r"(tag), "r"(opt2), "r"(addr));
    *(u_long128*)pPacket->pCurrent = q;
    pPacket->pCurrent += 4;
}
void sceVif1PkRefs(pacVif1Packet* pPacket, u_long128* pRef, u_int size, u_int opt1, u_int opt2, u_int flag) {
    sceVif1PkTerminate(pPacket);
    size |= 0x40000000;
    u_int tag = flag | size;
    u_int addr = (u_int)pRef & 0x8fffffff;
    u_long128 q, t;
    __asm__ volatile("pextlw %0, %2, %3\n\tpextlw %1, %4, %5\n\tpextlw %0, %1, %0" : "=&r"(q), "=&r"(t) : "r"(opt1), "r"(tag), "r"(opt2), "r"(addr));
    *(u_long128*)pPacket->pCurrent = q;
    pPacket->pCurrent += 4;
}
void sceVif1PkCall(pacVif1Packet* pPacket, u_long128* pCall, u_int flag) {
    pPacket->pDmaTag = sceVif1PkTerminate(pPacket);
    pPacket->lDmaTag.ul64[0] = (u_int)(flag | 0x50000000) | ((u_long)((u_int)pCall & 0x8fffffff) << 32);
    pPacket->pCurrent += 2;
    pPacket->pVifCode = NULL;
}
void sceVif1PkRet(pacVif1Packet* pPacket, u_int flag) {
    pPacket->pDmaTag = sceVif1PkTerminate(pPacket);
    pPacket->lDmaTag.ul64[0] = (u_int)(flag | 0x60000000);
    pPacket->pCurrent += 2;
    pPacket->pVifCode = NULL;
}
void sceVif1PkEnd(pacVif1Packet* pPacket, u_int flag) {
    pPacket->pDmaTag = sceVif1PkTerminate(pPacket);
    pPacket->lDmaTag.ul64[0] = (u_int)(flag | 0x70000000);
    pPacket->pCurrent += 2;
    pPacket->pVifCode = NULL;
}
void sceVif1PkOpenUpkCode(pacVif1Packet* pPacket, u_short vuaddr, u_int upkcmd, u_int cl, u_int wl) {
    u_int* pc = pPacket->pCurrent;
    while (((u_int)pc & 0xc) != 8) *pc++ = 0;
    cl |= wl << 8;
    pc[0] = cl | 0x01000000;
    pPacket->numlen = cl;
    pPacket->pVifCode = pc + 1;
    pPacket->iVifCode = (upkcmd << 24) | vuaddr;
    pPacket->pCurrent = pc + 2;
}
/* Parked: ~25 lines - register allocation: the original keeps wl (numlen >> 8) in t0 and cl in numlen's register a1, ours swaps them (and the t2/t3 numbering follows); the vn != 3 branch layout, the code copy and the shared divide tail match. cl/wl declaration order, signed locals, ternaries and 5 min of permuter (9k candidates, 25 -> 24) did not get it. */
#if 0
void sceVif1PkCloseUpkCode(pacVif1Packet* pPacket) {
    u_int* pEnd = pPacket->pCurrent - 1;
    u_int code = pPacket->iVifCode;
    u_int bits = (pEnd - pPacket->pVifCode) << 5;
    u_int vn = ((code >> 26) & 3) + 1;
    u_int vl = (code >> 24) & 3;
    u_int wl = pPacket->numlen >> 8;
    u_int cl = pPacket->numlen & 0xff;
    u_int num;
    if (!wl) wl = 256;
    if (!cl) cl = 256;
    if (vn != 3) {
        u_int sh = (vn >> 1) + 5 - vl;
        if (cl >= wl) num = bits >> sh;
        else num = bits / (cl << sh) * wl;
    } else {
        u_int n = vn * (32 >> vl);
        if (cl >= wl) num = bits / n;
        else num = bits / (cl * n) * wl;
    }
    *pPacket->pVifCode = code + (num << 16);
    pPacket->pVifCode = NULL;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", sceVif1PkCloseUpkCode__FP13pacVif1Packet);
#endif
void sceVif1PkOpenUpkCodeV4_32(pacVif1Packet* pPacket, u_short vuaddr, u_int cl, u_int wl) {
    u_int* pc = pPacket->pCurrent;
    while (((u_int)pc & 0xc) != 8) *pc++ = 0;
    cl |= wl << 8;
    pc[0] = cl | 0x01000000;
    pPacket->pVifCode = pc + 1;
    pPacket->iVifCode = vuaddr | 0x6c000000;
    pPacket->pCurrent = pc + 2;
}
void sceVif1PkCloseUpkCodeV4_32(pacVif1Packet* pPacket) {
    u_int* pEnd = pPacket->pCurrent - 1;
    *pPacket->pVifCode = pPacket->iVifCode + (((u_int)(pEnd - pPacket->pVifCode) >> 2) << 16);
    pPacket->pVifCode = NULL;
}
void sceVif1PkOpenDirectCode(pacVif1Packet* pPacket, int stall) {
    u_int* pc = pPacket->pCurrent;
    while (((u_int)pc & 0xc) != 0xc) *pc++ = 0;
    pPacket->iVifCode = stall ? 0xd0000000 : 0x50000000;
    pPacket->pVifCode = pc;
    pPacket->pCurrent = pc + 1;
}
void sceVif1PkCloseDirectCode(pacVif1Packet* pPacket) {
    u_int* pEnd = pPacket->pCurrent - 1;
    *pPacket->pVifCode = pPacket->iVifCode + ((u_int)(pEnd - pPacket->pVifCode) >> 2);
    pPacket->pVifCode = NULL;
}
void sceVif1PkOpenDirectHLCode(pacVif1Packet* pPacket, int stall) {
    u_int* pc = pPacket->pCurrent;
    while (((u_int)pc & 0xc) != 0xc) *pc++ = 0;
    pPacket->iVifCode = stall ? 0xd1000000 : 0x51000000;
    pPacket->pVifCode = pc;
    pPacket->pCurrent = pc + 1;
}
void sceVif1PkCloseDirectHLCode(pacVif1Packet* pPacket) {
    u_int* pEnd = pPacket->pCurrent - 1;
    *pPacket->pVifCode = pPacket->iVifCode + ((u_int)(pEnd - pPacket->pVifCode) >> 2);
    pPacket->pVifCode = NULL;
}
void sceVif1PkOpenGifTag(pacVif1Packet* pPacket, u_long128 tag) {
    pPacket->lGifTag.ul128 = tag;
    pPacket->pGifTag = (u_long*)pPacket->pCurrent;
    pPacket->pCurrent += 4;
}
void sceVif1PkCloseGifTag(pacVif1Packet* pPacket) {
    u_long128 tag = pPacket->lGifTag.ul128;
    u_long hi = _upper(tag);
    u_long lo = (u_long)tag;
    u_long flg = (lo >> 58) & 3;
    u_int n = (u_long*)pPacket->pCurrent - pPacket->pGifTag - 2;
    if (flg != 1) n >>= 1;
    if (flg != 2) {
        u_int nreg = lo >> 60;
        if (!nreg) nreg = 16;
        n = (n + nreg - 1) / nreg;
    }
    *(u_long128*)pPacket->pGifTag = _pack(lo + n, hi);
    pPacket->pGifTag = NULL;
    if ((u_int)pPacket->pCurrent & 8) pPacket->pCurrent += 2;
}
u_int* sceVif1PkReserve(pacVif1Packet* pPacket, u_int count) {
    u_int* p = pPacket->pCurrent;
    pPacket->pCurrent = p + count;
    return p;
}
void sceVif1PkAlign(pacVif1Packet* pPacket, u_int bit, u_int pos) {
    u_int mask = (u_int)0xffffffff >> (32 - ((bit + 2) & 0x1f));
    u_int* pc = pPacket->pCurrent;
    u_int* p = (u_int*)(((u_int)pc & ~mask) + pos * 4);
    if (p < pc) p = (u_int*)((u_int)p + (mask + 1));
    while (pPacket->pCurrent < p) *pPacket->pCurrent++ = 0;
}
void sceVif1PkAddCode(pacVif1Packet* pPacket, u_int code) {
    u_int* pc = pPacket->pCurrent;
    if (((code >> 24) & 0x7f) == 0x13) {
        *pc++ = 0x11000000;
        while (((u_int)pc & 0xc) != 0xc) *pc++ = 0;
        pc[0] = 0x50000001;
        *(u_long*)(pc + 1) = 0x8000;
        pc[5] = code;
        pc += 6;
    } else {
        *pc++ = code;
    }
    pPacket->pCurrent = pc;
}
void sceVif1PkAddData(pacVif1Packet* pPacket, u_int data) {
    u_int* p = (u_int*)pPacket->pCurrent;
    *p = data;
    pPacket->pCurrent = (u_int*)(p + 1);
}
void sceVif1PkAddDataN(pacVif1Packet* pPacket, u_int* pData, u_int count) {
    u_int* p = (u_int*)pPacket->pCurrent;
    while (count--) *p++ = *pData++;
    pPacket->pCurrent = (u_int*)p;
}
void sceVif1PkAddUpkData32(pacVif1Packet* pPacket, u_int data) {
    u_int* p = (u_int*)pPacket->pCurrent;
    *p = data;
    pPacket->pCurrent = (u_int*)(p + 1);
}
void sceVif1PkAddUpkData32N(pacVif1Packet* pPacket, u_int* pData, u_int count) {
    u_int* p = (u_int*)pPacket->pCurrent;
    while (count--) *p++ = *pData++;
    pPacket->pCurrent = (u_int*)p;
}
void sceVif1PkAddUpkData64(pacVif1Packet* pPacket, u_long data) {
    u_int* p = pPacket->pCurrent;
    p[0] = data;
    p[1] = data >> 32;
    p += 2;
    pPacket->pCurrent = p;
}
void sceVif1PkAddUpkData64N(pacVif1Packet* pPacket, u_long* pData, u_int count) {
    u_int* p = pPacket->pCurrent;
    while (count--) {
        u_long d = *pData++;
        p[0] = d;
        p[1] = d >> 32;
        p += 2;
    }
    pPacket->pCurrent = p;
}
void sceVif1PkAddUpkData128(pacVif1Packet* pPacket, u_long128 data) {
    u_int* p = pPacket->pCurrent;
    u_long lo = (u_long)data;
    u_long hi = _upper(data);
    p[0] = lo;
    p[1] = lo >> 32;
    p[2] = hi;
    p[3] = hi >> 32;
    p += 4;
    pPacket->pCurrent = p;
}
void sceVif1PkAddUpkData128N(pacVif1Packet* pPacket, u_long128* pData, u_int count) {
    u_int* p = pPacket->pCurrent;
    while (count--) {
        u_long128 d = *pData++;
        u_long lo = (u_long)d;
        u_long hi = _upper(d);
        p[0] = lo;
        p[1] = lo >> 32;
        p[2] = hi;
        p[3] = hi >> 32;
        p += 4;
    }
    pPacket->pCurrent = p;
}
void sceVif1PkAddDirectData(pacVif1Packet* pPacket, u_long128 data) {
    u_long128* p = (u_long128*)pPacket->pCurrent;
    *p = data;
    pPacket->pCurrent = (u_int*)(p + 1);
}
void sceVif1PkAddDirectDataN(pacVif1Packet* pPacket, u_long128* pData, u_int count) {
    u_long128* p = (u_long128*)pPacket->pCurrent;
    while (count--) *p++ = *pData++;
    pPacket->pCurrent = (u_int*)p;
}
void sceVif1PkAddGsData(pacVif1Packet* pPacket, u_long data) {
    u_long* p = (u_long*)pPacket->pCurrent;
    *p = data;
    pPacket->pCurrent = (u_int*)(p + 1);
}
void sceVif1PkAddGsDataN(pacVif1Packet* pPacket, u_long* pData, u_int count) {
    u_long* p = (u_long*)pPacket->pCurrent;
    while (count--) *p++ = *pData++;
    pPacket->pCurrent = (u_int*)p;
}
void sceVif1PkAddGsPacked(pacVif1Packet* pPacket, u_long128 data) {
    u_long128* p = (u_long128*)pPacket->pCurrent;
    *p = data;
    pPacket->pCurrent = (u_int*)(p + 1);
}
void sceVif1PkAddGsPackedN(pacVif1Packet* pPacket, u_long128* pData, u_int count) {
    u_long128* p = (u_long128*)pPacket->pCurrent;
    while (count--) *p++ = *pData++;
    pPacket->pCurrent = (u_int*)p;
}
void sceVif1PkAddGsAD(pacVif1Packet* pPacket, u_int addr, u_long data) {
    u_long128* p = (u_long128*)pPacket->pCurrent;
    *p = _pack(data, addr);
    pPacket->pCurrent = (u_int*)(p + 1);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", sceVif1PkRefLoadImage__FP13pacVif1PacketUsUcUsPUxUiUiUiUiUi);
void sceVif1PkRefMpg(pacVif1Packet* pPacket, u_short vuaddr, u_long128* pMicro, u_int size, u_int opt1) {
    while (size) {
        u_int n = size <= 128 ? size : 128;
        size -= n;
        sceVif1PkRef(pPacket, pMicro, n, opt1, vuaddr | ((n * 2 & 0xff) << 16) | 0x4a000000, 0);
        pMicro += n;
        vuaddr += n * 2;
        opt1 = 0;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", __static_initialization_and_destruction_0_0013E4E0);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _GLOBAL_$I$c__KHCOM_MainApp_System_Graphic_PackMan.cppcWbaaa);
