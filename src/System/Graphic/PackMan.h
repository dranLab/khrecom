#ifndef SYSTEM_GRAPHIC_PACKMAN_H
#define SYSTEM_GRAPHIC_PACKMAN_H

#include "khtypes.h"
#include "System/Graphic/Gprim.h"
#include "sdk/libgraph.h"

struct VTransInfo;

typedef struct {
    tGS_PMODE pmode;
    tGS_SMODE2 smode2;
    tGS_DISPFB2 dispfb;
    tGS_DISPLAY2 display;
    tGS_BGCOLOR bgcolor;
} sceGsDispEnv;

typedef struct {
    sceGsFrame frame2;
    u_long frame2addr;
    sceGsZbuf zbuf2;
    long zbuf2addr;
    sceGsXyoffset xyoffset2;
    long xyoffset2addr;
    sceGsScissor scissor2;
    long scissor2addr;
    sceGsPrmodecont prmodecont;
    long prmodecontaddr;
    sceGsColclamp colclamp;
    long colclampaddr;
    sceGsDthe dthe;
    long dtheaddr;
    sceGsTest test2;
    long test2addr;
} sceGsDrawEnv2;

typedef struct {
    sceGsTest testa;
    long testaaddr;
    sceGsPrim prim;
    long primaddr;
    sceGsRgbaq rgbaq;
    long rgbaqaddr;
    sceGsXyz xyz2a;
    long xyz2aaddr;
    sceGsXyz xyz2b;
    long xyz2baddr;
    sceGsTest testb;
    long testbaddr;
} sceGsClear;

typedef struct {
    sceGsDispEnv disp[2];
    sceGifTag giftag0;
    sceGsDrawEnv1 draw01;
    sceGsDrawEnv2 draw02;
    sceGsClear clear0;
    sceGifTag giftag1;
    sceGsDrawEnv1 draw11;
    sceGsDrawEnv2 draw12;
    sceGsClear clear1;
} sceGsDBuffDc;

typedef struct {
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
} pacVif1Packet;

struct GpuRegsInfoSep {
    ULONGLONG _nCLAMP;
    ULONGLONG _nSCISSOR;
    ULONGLONG _nALPHA;
    ULONGLONG _nTEST;
    ULONGLONG _nFBA;
    ULONGLONG _nZBUF;
    ULONGLONG _nFRAME;
};

class CPackMan {
public:
    static void initialize();
    static void finalize();
    static void resetGsDma();
    static void resetDmaChanVif1();
    static void beginScene();
    static void endScene();
    static void kickDMA(INT nInFrame);
    static void kickDMAFG(INT nInFrame);
    static void swapDBuf(INT nInFrame);
    static void transFrame();
    static void setFrameEffect(INT transKind, INT in_Xofs, INT in_Yofs, INT in_Alpha);
    static void transFrameEffect();
    static INT getDBIdx();
    static ULONGLONG getDefaultTEST();
    static void enableZWrite(BOOL inEnable, INT inAddr);
    static u_long128* getPktVif1Frame();
    static INT getTransFrameKind();
    static void enableDraw(INT iDrawBit);
    static void disableDraw(INT iDrawBit);
    static INT getEnableDraw();
    static void changeDrawMode(INT nMode);
    static INT getDrawMode();
    static BOOL isSkipPacketDMA();
    static void saveScreen();
    static BOOL isStartStillMode();
    static void setMovieEnv(UINT nDrawW, UINT nDrawH, UINT nDispW, UINT nDispH);
    static void changePcrtcMode(INT nMode);
    static INT getPcrtcMode();
    static void setBGColor(INT nColor);
    static INT getBGColor();
    static void setDisp1Alpha(INT nAlpha);
    static INT getDisp1Alpha();
    static void updateBGBlend(INT nAlpha);
    static u_long128* initBGPacket(INT nInIdx);
    static BOOL isPacketForBG();
    static void terminateBGPacket();
    static void changeLayer(INT nInLayer);
    static void initSemiPacket();
    static void terminateSemiPacket();
    static void initFGPacket();
    static void terminateFGPacket();
    static BOOL isOverFlow(UINT nSize);
    static void startCountTmpSize();
    static UINT getTmpCountSize();
    static void setFGPacketMaxNorml();
    static void extFGPacketMaxForFlex();
    static INT getPkUseSize();
    static INT getPkTotalSize();
    static GpuRegsInfoSep& getSysGpuRegs2();
    static GpuRegsInfoSep& getDefaultGpuRegs2();
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
    static void pkInit(u_long128* pBase, pacVif1Packet* pPkt, UINT nMaxPkt);
    static void pkReset();
    static UINT pkSize();
    static u_long128* pkTerminate();
    static void pkCnt(UINT nFlag);
    static void pkRet(UINT nFlag);
    static void pkEnd(UINT nFlag);
    static void pkNext(u_long128* pNext, UINT nFlag);
    static void pkCall(u_long128* pCall, UINT nFlag);
    static void pkRefe(u_long128* pRef, UINT nSize, UINT nOpt1, UINT nOpt2, UINT nFlag);
    static void pkRef(u_long128* pRef, UINT nSize, UINT nOpt1, UINT nOpt2, UINT nFlag);
    static void pkRefs(u_long128* pRef, UINT nSize, UINT nOpt1, UINT nOpt2, UINT nFlag);
    static void pkSetCurrentPointer(void* p);
    static UINT* pkGetCurrentPointer();
    static u_long128* pkOpenUpkCode(USHORT nVUaddr, UINT nUpkCmd, UINT nCL, UINT nWL);
    static void pkCloseUpkCode(void* p);
    static void pkCloseUpkCode();
    static u_long128* pkOpenUpkCodeV4_32(USHORT nVUaddr, UINT nCL, UINT nWL);
    static void pkCloseUpkCodeV4_32(void* p);
    static void pkCloseUpkCodeV4_32();
    static void pkOpenDirectCode(INT nStall);
    static void pkCloseDirectCode();
    static void pkOpenDirectHLCode(INT nStall);
    static void pkCloseDirectHLCode();
    static u_long128* pkOpenGifTag(u_long128 nInGifTag);
    static void pkCloseGifTag(void* p);
    static void pkCloseGifTag();
    static u_long128* pkOpenGifTagManual(u_long128 nGifTag);
    static void pkCloseGifTagManual(UINT* p);
    static UINT* pkReserve(UINT nCount);
    static void pkAlign(UINT nBit, UINT nPos);
    static void pkAddCode(UINT nCode);
    static void pkAddData(UINT nData);
    static void pkAddDataN(UINT* pData, UINT nCount);
    static void pkAddUpkData32(UINT nData);
    static void pkAddUpkData64(ULONGLONG nData);
    static void pkAddUpkData128(u_long128 nData);
    static void pkAddUpkData32N(UINT* pData, UINT nCount);
    static void pkAddUpkData64N(ULONGLONG* pData, UINT nCount);
    static void pkAddUpkData128N(u_long128* pData, UINT nCount);
    static void pkAddDirectData(u_long128 nData);
    static void pkAddDirectDataN(u_long128* pData, UINT nCount);
    static void pkAddGsData(ULONGLONG nData);
    static void pkAddGsDataN(ULONGLONG* pData, UINT nCount);
    static void pkAddGsPacked(u_long128 nData);
    static void pkAddGsPackedN(u_long128* pData, UINT nCount);
    static void pkAddGsAD(UINT nAddr, ULONGLONG nData);
    static void pkRefLoadImage(USHORT nInBp, BYTE cInPsm, USHORT nInBw, u_long128* pInImage, UINT nInSize, UINT nInX, UINT nInY, UINT nInW, UINT nInH);
    static void pkRefLoadImage(const VTransInfo& info);
    static void pkRefLoadImage(USHORT nBP, const VTransInfo& info);
    static void pkRefMpg(USHORT nInVuAddr, u_long128* pInMicro, UINT nInSize, UINT nInOpt1);
};

#endif /* SYSTEM_GRAPHIC_PACKMAN_H */
