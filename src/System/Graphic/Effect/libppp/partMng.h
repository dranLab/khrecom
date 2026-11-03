#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_PARTMNG_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_PARTMNG_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/draw/md.h"
#include "System/Graphic/Effect/libppp/draw/md0_p3f.h"
#include "System/Graphic/Effect/libppp/draw/shape.h"

typedef struct { /* 0x8 */
    u_long TBP0 : 14;
    u_long TBW : 6;
    u_long PSM : 6;
    u_long TW : 4;
    u_long TH : 4;
    u_long TCC : 1;
    u_long TFX : 2;
    u_long CBP : 14;
    u_long CPSM : 4;
    u_long CSM : 1;
    u_long CSA : 5;
    u_long CLD : 3;
} sceGsTex0;

typedef struct _DPD_SHAPE_HEADER { /* 0x10 */
    short shShpNo;
    short shTexpBase;
    short shClutpBase;
    u_char abRes0[2];
    int nPacketSize;
    u_char abRes1[4];
} DPD_SHAPE_HEADER;

typedef struct _DPD_MODEL_HEADER { /* 0x10 */
    short shRsdNo;
    short shTexpBase;
    short shClutpBase;
    u_char abRes0[2];
    int nPacketSize;
    u_char abRes1[4];
} DPD_MODEL_HEADER;

typedef struct _DPD_TEXTURE { /* 0x20 */
    short shTexDbp;
    short shCltDbp;
    short shDbw;
    short shDpsm;
    short shX;
    short shY;
    short shW;
    short shH;
    u_int unTex0L;
    u_int unTex0H;
    u_int unClutStart;
    short shTexVramSize;
    short shCltVramSize;
    u_char abData[0];
} DPD_TEXTURE;

typedef struct _CURRENT_EFFDATA {
    char *pchDpdData;
    int nDpdId;
} CURRENT_EFFDATA;

typedef struct _VRAMCACHE { /* 0x10 */
    char *pchDpd;
    u_long unDpdId;
    char chBlockStart;
    char chBlockNum;
    u_char bBoolUse;
    u_char bRes;
} VRAMCACHE;

typedef struct _VRAMCACHE_MNG {
    VRAMCACHE aVramCacheTex[12];
    VRAMCACHE aVramCacheClut[6];
} VRAMCACHE_MNG;

typedef struct _DPD_HEAD_ETCDATA {
    int nMinTexVram;
    int nMaxTexVram;
    int nMinClutVram;
    int nMaxClutVram;
} DPD_HEAD_ETCDATA;

extern float ppvFrameTime;
extern int ppvDispWidth;
extern int ppvDispHeight;
extern int ppvDrawWidth;
extern int ppvDrawHeight;
extern int ppvZFrameBase;
extern int ppvDrawFrameBase;
extern int ppvDispFrameBase;
extern u_long ppvGsFrame;
extern u_long ppvGsZbuf;
extern u_long ppvGsXyoffset;
extern u_long ppvGsScissor;
extern u_long ppvGsTest;
extern u_long ppvGsDthe;
extern u_long ppvGsColorClamp;
extern void (*ppvErrorFunc)();
extern int ppvWorkMemSize;
extern int ppvPacketMemSize;
extern char *g_pchDpdTexCommon;
extern sceVu0FMATRIX *ppvReferenceMatrixList;
extern int ppvReferenceMatrixNum;
extern u_char ppvVu1SprDbId;
extern int ppvVu1Prog;
extern CURRENT_EFFDATA g_CurrentEffData;
extern VRAMCACHE_MNG g_VcacheMng;

void pppSetPrimPacket(void *pvPrimPacketArea, int nLen);
void *pppDbgAlloc(pppMemMng *mng, u_word size);
void pppFreeParticle(void *mng);
void pppStopOnOffParticle(void *mng);
void pppStopParticle(void *mng, int b);
int pppIsLoop(void *mng);
void pppLoopEnd(void *mng);
void setShapeTex0(OANM_HEADER *Ah, DPD_SHAPE_HEADER *pSh, short shTexpStart, short shClutpStart);
void pppSetUp(void *mng);
float *pppGetBbox(void *mng);
void pppSetDrawFirst(void *mng, int priority);
void pppSetDrawLast(void *mng, int priority);
int pppIsDrawFirst(void *mng);
int pppIsDrawLast(void *mng);
char *pppGetPdata(pppMngSt *pMng);
void debugDpdCheckShapeData(int nShapeNum);
void pppInitDpd(char *pchDpdData);
void pppMoveDpd(char *pchDpdData, int nOffset);
void pppInitTexCommon(char *pchDpdData);
char *pppPrintProf(void *mng);
char *pppPrintPerformance();
void pppGetWorkMemoryInfo(int *memory_use, int *memory_size);
void pppPartCalcReady();
int pppPartCalc(void *mng, int view_clip);
void pppPartCalcEnd();
void pppInitDraw(BOOL bDraw);
void pppPartDrawReady(BOOL bDraw);
int pppPartDraw(void *mng, BOOL bDraw);
void pppPartDrawEnd();
void *pppGetPartMng();
void pppFreeMng(void *mng);
void pppDeleteMng(void *mng);
int pppStartPartMng(void *mng, u_long128 *dpd, u_int dpd_id, int priority, float wait);
sceVu0FMATRIX *pppPartMngGetMatrix(void *mng);
sceVu0FVECTOR *pppPartMngGetPos(void *mng);
sceVu0FVECTOR *pppPartMngGetScl(void *mng);
sceVu0FVECTOR *pppPartMngGetCol(void *mng);
void pppPartMngSetSkipFrame(void *mng, int nFrame);
int pppPartMngGetTotalFrame(void *mng);
float pppPartMngGetNowFrame(void *mng);
BOOL pppPartMngIsEnd(void *mng);
BOOL pppIsViewClip(void *mng);
int pppPartMngIsWaiting(void *mng);
void pppSetPause(int pause);
void pppSetPartMatrix(float *cm[4], float *sm[4], float *eye, float *at);
void pppSetFrameTime(float frame_time);
void pppSetFogParam(float near, float far);
void pppSetFogColor(u_char r, u_char g, u_char b);
void pppSetFogClamp(u_char min, u_char max);
void pppSetReferencePos(void *list, int num);
void pppSetReferenceMatrix(void *list, int num);
void pppSetMonochrome(int b);
void pppRandInit();
void *lmemcpy(void *dis, void *src, int ln);
void *l64memcpy(void *dis, void *src, int ln);
void *l128memcpy(void *dis, void *src, int ln);
void *lmemset(void *dis, word code, int ln);
void *l64memset(void *dis, long code, int ln);
void *l128memset(void *dis, long128 code, int ln);
void pppSetErrorFunc(void (*func)());
void pppWaitVif1();
void pppSendVu1Prog(int vu1prog);
void pppWaitDmaGs();
void pppSendGsPacket(u_long128 *pkt_top);
void pppSendVif1Packet(u_long128 *pkt_top);
void pppSendTagVif1Packet(u_long128 *pkt_top);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_PARTMNG_H */
