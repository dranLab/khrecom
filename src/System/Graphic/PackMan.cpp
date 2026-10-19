#include "khtypes.h"
#include "include_asm.h"

typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned short u_short;
typedef unsigned char u_char;

struct VTransInfo { /* System/Graphic/modeldata.h */
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

union QWdata { /* System/Graphic/Gprim.h */
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

struct tD_CHCR { /* libdma.h */
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
struct sceDmaChan { /* libdma.h (only chcr used here) */
    tD_CHCR chcr;
};
extern "C" sceDmaChan* sceDmaGetChan(int id);

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

class CDelayedFree {
public:
    static void disposeFreeList();
};

class CPackMan {
public:
    static void finalize();
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
extern "C" int FlushCache(int op);
extern "C" void sceDmaSend(sceDmaChan* d, void* tag);
extern "C" void AssertApp(const char* pFile, int nLine, const char* pExpr, int n);
extern "C" char D_00461BE0[], D_00461C10[]; /* PARTIAL: __FILE__ and the assert text (data still asm) */
void _setPcrtcMode_BlendBG(sceGsDispEnv& inEnv); /* static in the original (still INCLUDE_ASM) */

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
extern u_long s_pPktBufVif1Frame[320];
extern pacVif1Packet s_pktVif1;
extern pacVif1Packet s_pktVif1BG;
extern sceDmaChan* s_pDmaChanVif1;
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
extern INT s_nBGColor;
extern INT s_nDisp1Alpha;
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

void CPackMan::pkInit(u_long128* pBase, pacVif1Packet* pPkt, UINT nMax) {
    UINT nSize = nMax ? nMax : 0x8000;
    s_pPktVif1 = pPkt ? pPkt : &s_pktVif1;
    s_nMaxPktSizeVif1 = nSize;
    sceVif1PkInit(s_pPktVif1, pBase);
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
u_long128* CPackMan::pkOpenGifTag(u_long128 tag) {
    pacVif1Packet* pPkt = s_pPktVif1;
    sceVif1PkOpenGifTag(pPkt, tag);
    return (u_long128*)pPkt->pCurrent;
}
void CPackMan::pkCloseGifTag(void* p) {
    s_pPktVif1->pCurrent = (u_int*)p;
    sceVif1PkCloseGifTag(s_pPktVif1);
}
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
void CPackMan::pkRefLoadImage(u_short bp, u_char psm, u_short bw, u_long128* image, UINT size, UINT x, UINT y, UINT w, UINT h) {
    sceVif1PkRefLoadImage(s_pPktVif1, bp, psm, bw, image, size, x, y, w, h);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", pkRefLoadImage__8CPackManRC10VTransInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", pkRefLoadImage__8CPackManUsRC10VTransInfo);
void CPackMan::pkRefMpg(u_short vuaddr, u_long128* pMicro, UINT size, UINT opt1) { sceVif1PkRefMpg(s_pPktVif1, vuaddr, pMicro, size, opt1); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", initialize__8CPackMan);
void CPackMan::finalize() { CDelayedFree::disposeFreeList(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _initGsDefDrawEnv__FR12sceGsDBuffDc12_GS_BUF_SIDEsssssUiUii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _initDBuf__Fv);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", resetGsDma__8CPackMan);
void CPackMan::resetDmaChanVif1() {
    sceDmaChan* pChan = sceDmaGetChan(1);
    s_pDmaChanVif1 = pChan;
    *(volatile u_int*)&pChan->chcr |= 0x40;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _clearVram__Fv);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _setupSysContext__Fv);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", beginScene__8CPackMan);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", endScene__8CPackMan);
INT CPackMan::getDBIdx() { return s_nDB & 1; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", transFrameEffect__8CPackMan);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", transFrame__8CPackMan);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", initBGPacket__8CPackMani);
BOOL CPackMan::isPacketForBG() { return s_pPktVif1 == &s_pktVif1BG; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", terminateBGPacket__8CPackMan);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", changeLayer__8CPackMani);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", initSemiPacket__8CPackMan);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", terminateSemiPacket__8CPackMan);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", initFGPacket__8CPackMan);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", terminateFGPacket__8CPackMan);
BOOL CPackMan::isStartStillMode() { return s_nReqDrawMode == 1 && s_nDrawMode == 0; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", saveScreen__8CPackMan);
void CPackMan::setMovieEnv(UINT nDrawW, UINT nDrawH, UINT nDispW, UINT nDispH) {
    s_nMovieDrawW = nDrawW;
    s_nMovieDrawH = nDrawH;
    s_nMovieDispW = nDispW;
    s_nMovieDispH = nDispH;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _makePkt_SaveScreen__Fv);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _makePkt_StillScreen__FPUx);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _setPcrtcMode_FSAA__FR12sceGsDispEnv);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _setPcrtcMode_BlendBG__FR12sceGsDispEnv);
void CPackMan::updateBGBlend(INT nAlpha) {
    s_nDisp1Alpha = nAlpha;
    _setPcrtcMode_BlendBG(s_DBuf.disp[0]);
}
ULONGLONG CPackMan::getDefaultTEST() { return s_DBuf.clear0.testb; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", swapDBuf__8CPackMani);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", kickDMA__8CPackMani);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", kickDMAFG__8CPackMani);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _loadMicroProgram__Fv);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", setFrameEffect__8CPackManiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _initFramePacket__Fi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _initFramePacketForMovie__Fi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _initFrameEffPacket__Fiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", enableZWrite__8CPackManii);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", sceVif1PkRefe__FP13pacVif1PacketPUxUiUiUiUi);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", sceVif1PkRef__FP13pacVif1PacketPUxUiUiUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", sceVif1PkRefs__FP13pacVif1PacketPUxUiUiUiUi);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", sceVif1PkCloseUpkCode__FP13pacVif1Packet);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", sceVif1PkCloseGifTag__FP13pacVif1Packet);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", sceVif1PkRefMpg__FP13pacVif1PacketUsPUxUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", __static_initialization_and_destruction_0_0013E4E0);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PackMan", _GLOBAL_$I$c__KHCOM_MainApp_System_Graphic_PackMan.cppcWbaaa);
