#include "partMng.h"
#include "pppAlloc.h"
#include "draw/draw.h"
#include "draw/draw_w.h"
#include "System/MainApp.h"
#include "System/Debug.h"
#include "pppfunctbl.h"

int sprintf(char *buf, const char *fmt, ...);
int sceDmaSync(sceDmaChan *d, int mode, int timeout);
extern u_long128 *ppvPrimp;
extern int ppvPrimpLen;
extern void *ppvVertex;
extern sceVu0FMATRIX ppvSWMatrixWood;
extern sceVu0FMATRIX ppvCameraMatrixOld;
extern sceVu0FMATRIX ppvCameraMatrixInv;
extern sceVu0FMATRIX ppvSCMatrix;
extern sceVu0FMATRIX ppvUnitMatrix;
extern sceVu0FVECTOR ppvZeroVector;
extern sceVu0FVECTOR ppvAt;
extern sceVu0FVECTOR ppvEyeOld;
extern sceVu0FVECTOR ppvAtOld;
extern float ppvFov;

float ppvFrameTime = 0.f;
int ppvDispWidth = 0;
int ppvDispHeight = 0;
int ppvDrawWidth = 0;
int ppvDrawHeight = 0;
int ppvZFrameBase = 0;
int ppvDrawFrameBase = 0;
int ppvDispFrameBase = 0;
u_long ppvGsFrame = 0;
u_long ppvGsZbuf = 0;
u_long ppvGsXyoffset = 0;
u_long ppvGsScissor = 0;
u_long ppvGsTest = 0;
u_long ppvGsDthe = 0;
u_long ppvGsColorClamp = 0;
int ppvMonochrome = 0;
void (*ppvErrorFunc)() = NULL;
int ppvWorkMemSize = 0;
int ppvPacketMemSize = 0;

static pppModelSt RendModel[128] = {{0}};
static pppShapeSt RendShape[128] = {{0}};
static pppVsfSt RendVsf[128] = {{0}};

char *g_pchDpdTexCommon = NULL;

typedef struct _GsLoadImage_TAG {
    sceGsLoadImage sceGsLoadImage;
} _GsLoadImage __attribute__((aligned(64)));

/* .bss: ppvDbgTemp here, the statics of pppGetBbox and pppPrintPerformance, ppvVu1SprDbId and ppvVu1Prog (after
   pppPrintPerformance), then the file statics (gcc emits them at the end); the rest are common symbols. */
long128 ppvDbgTemp[1] __attribute__((section(".bss"))) = {0};
sceVu0FVECTOR *ppvReferencePosList;
int ppvReferencePosNum;
sceVu0FMATRIX *ppvReferenceMatrixList;
int ppvReferenceMatrixNum;

static pppEnvSt g_Env;
CURRENT_EFFDATA g_CurrentEffData;
static _GsLoadImage s_GsLoadImageTex;
static _GsLoadImage s_GsLoadImageClt;
VRAMCACHE_MNG g_VcacheMng;

static const u_long alDmaPkt[8] = {
    0x70000002, 0, 0x1000000000008001, 0xE, 0, 0x3F, 0, 0,
};






void pppSetPrimPacket(void *pvPrimPacketArea, int nLen) {
    ppvPrimp = pvPrimPacketArea;
    ppvPrimpLen = nLen;
}

static void drawStart() {
    ppvVertex = NULL;
}

void *pppDbgAlloc(pppMemMng *mng, u_word size) {
    return pppAlloc(mng, size);
}

static void drawEnd() {
    if (ppvCurPrimpLen != 0) {
        op_ot_draw();
        op_otz_init();
    }
}

static void initGraphicSystem() {
    op_init_draw();
    op_gs2_init();
    op_otz_init();
    ops_init_fog_work();
}

void pppFreeParticle(void *mng) {
    pppMngSt *pMng = mng;
#line 467
    KH_DEBUG(if (!(pMng != ((void *)0))) AssertApp(__FILE__, __LINE__, "pMng != ((void *)0)", 0);)
    pppAllFreePObject(pMng);
}

void pppStopOnOffParticle(void *mng) {
    ((pppMngSt *)mng)->bFlagStop ^= 1;
}

void pppStopParticle(void *mng, int b) {
    ((pppMngSt *)mng)->bFlagStop = b;
}

int pppIsLoop(void *mng) {
    return ((pppMngSt *)mng)->partLoopf;
}

void pppLoopEnd(void *mng) {
    ((pppMngSt *)mng)->partLoopEndf = 1;
}
#define TBP0_OF(l) ((u_short)(l) & 0x3FFF)
#define CBP_OF(h) (((h) >> 5) & 0x3FFF)
#define ASSERTS(e, s) KH_DEBUG(if (!(e)) AssertApp(__FILE__, __LINE__, s, 0))

static void setOmdTex0(OMD_H *omd, DPD_MODEL_HEADER *pMh, short shTexpStart, short shClutpStart) {
    OMD_PRIM_H *mph;
    short shTexpDif = 0;
    short shClutpDif = 0;
    short shTexpOld;
    short shClutpOld;
    int set_first = 0;
    int nBoolTex;
    int nSizeAdd;
    int nTexOfs;
    int nBase;

    mph = (OMD_PRIM_H *)((char *)omd + omd->pa);
    if (pMh->shTexpBase == 0) {
        pMh->shTexpBase = 0x2800;
        pMh->shClutpBase = 0x3E40;
        set_first = 1;
    }
    if (shTexpStart == 0 && shClutpStart == 0 && !set_first) {
        return;
    }
    if (shTexpStart != 0) {
        shTexpDif = shTexpStart - pMh->shTexpBase;
    }
    if (shClutpStart != 0) {
        shClutpDif = shClutpStart - pMh->shClutpBase;
    }
    if (shTexpDif == 0 && shClutpDif == 0 && !set_first) {
        return;
    }
    while (mph->ptype != -1) {
        nBoolTex = 1;
        nSizeAdd = 0;
        switch (mph->ptype) {
        case 0:
            nSizeAdd = 0x14;
            nBoolTex = 0;
            break;
        case 1:
            nSizeAdd = 0x1C;
            nBoolTex = 0;
            break;
        case 2:
            nSizeAdd = 0x20;
            break;
        case 4:
            nSizeAdd = 0x18;
            nBoolTex = 0;
            break;
        case 5:
            nSizeAdd = 0x20;
            nBoolTex = 0;
            break;
        case 3:
        case 6:
            nSizeAdd = 0x28;
            break;
        case 7:
            nSizeAdd = 0x30;
            break;
        default:
#line 574
            ASSERTS(!("unknown ptype."), "!(\"unknown ptype.\")");
            break;
        }
        if (nBoolTex) {
            if (set_first) {
                shTexpOld = TBP0_OF(mph->tex0l);
                if (shTexpOld >= 0x3800) {
                    mph->tex0l &= ~0x3FFF;
                    nTexOfs = shTexpOld - 0x1000;
                    if (nTexOfs < 0x400) {
                        TRACE("partMng::Model:\223]\221\227\203e\203N\203X\203`\203\203\210\312\222u\202\311\203}\203C\203i\203X\202\252\224\255\220\266\202\265\202\304\202\242\202\334\202\267!!!!! %d 0x%x\n", nTexOfs, shTexpOld);
                        nTexOfs = 0x400;
                    }
                    mph->tex0l += nTexOfs;
                } else {
                    nBase = shTexpOld < 0x3600 ? 0x2A00 : 0x3600;
                    mph->tex0l &= ~0x3FFF;
                    nTexOfs = shTexpOld - nBase + 0x2600;
                    if (nTexOfs < 0x400) {
                        TRACE("partMng::Model:\213\244\222\312\203e\203N\203X\203`\203\203\210\312\222u\202\311\203}\203C\203i\203X\202\252\224\255\220\266\202\265\202\304\202\242\202\334\202\267!!!!! %d 0x%x\n", nTexOfs, shTexpOld);
                        nTexOfs = 0x400;
                    }
                    mph->tex0l += nTexOfs;
                }
                shClutpOld = CBP_OF(mph->tex0h);
                if (shClutpOld < 0x3D40) {
                    mph->tex0h = (mph->tex0h & 0xFFF8001F) + ((shClutpOld + 0x100) << 5);
                } else {
                    mph->tex0h = (mph->tex0h & 0xFFF8001F) + ((shClutpOld - 0x3D40 + 0x3E40) << 5);
                }
            }
            if (shTexpStart != 0 && shTexpDif != 0) {
                shTexpOld = TBP0_OF(mph->tex0l);
                if (shTexpOld >= 0x2800) {
#line 622
                    ASSERTS(((0x2600) + (((0x100) / 2) * ((2) * (((0x100) / ((0x100) / 2)))))) <= (shTexpOld + shTexpDif), "((0x2600) + (((0x100) / 2) * ((2) * (((0x100) / ((0x100) / 2)))))) <= (shTexpOld + shTexpDif)");
                    ASSERTS((shTexpOld + shTexpDif) < (((0x2600) + (((0x100) / 2) * ((2) * (((0x100) / ((0x100) / 2)))))) + (((0x100) / 2) * (((8) * (((0x100) / ((0x100) / 2)))) - ((2) * (((0x100) / ((0x100) / 2))))))), "(shTexpOld + shTexpDif) < (((0x2600) + (((0x100) / 2) * ((2) * (((0x100) / ((0x100) / 2)))))) + (((0x100) / 2) * (((8) * (((0x100) / ((0x100) / 2)))) - ((2) * (((0x100) / ((0x100) / 2)))))))");
                    mph->tex0l = (mph->tex0l & ~0x3FFF) + (shTexpOld + shTexpDif);
                }
            }
            if (shClutpStart != 0 && shClutpDif != 0) {
                shClutpOld = CBP_OF(mph->tex0h);
                if (shClutpOld >= 0x3E40) {
#line 633
                    ASSERTS(((0x3e00) + (((0x100) / 8) * (2))) <= (shClutpOld + shClutpDif), "((0x3e00) + (((0x100) / 8) * (2))) <= (shClutpOld + shClutpDif)");
                    ASSERTS((shClutpOld + shClutpDif) < (((0x3e00) + (((0x100) / 8) * (2))) + (((0x100) / 8) * (((1)* ((0x100) / ((0x100) / 8))) - (2)))), "(shClutpOld + shClutpDif) < (((0x3e00) + (((0x100) / 8) * (2))) + (((0x100) / 8) * (((1)* ((0x100) / ((0x100) / 8))) - (2))))");
                    mph->tex0h = (mph->tex0h & 0xFFF8001F) + ((shClutpOld + shClutpDif) << 5);
                }
            }
        }
        mph = (OMD_PRIM_H *)((char *)mph + nSizeAdd * mph->pc);
        mph++;
    }
}
void setShapeTex0(OANM_HEADER *Ah, DPD_SHAPE_HEADER *pSh, short shTexpStart, short shClutpStart) {
    OANM_SEQ *Seq;
    OSHAPE_VU1 *Shape;
    short shTexpDif = 0;
    short shClutpDif = 0;
    short shTexpOld;
    short shClutpOld;
    int i;
    int set_first = 0;
    int nTexOfs;
    int nBase;

    if (pSh->shTexpBase == 0) {
        pSh->shTexpBase = 0x2800;
        pSh->shClutpBase = 0x3E40;
        set_first = 1;
    }
    if (shTexpStart == 0 && shClutpStart == 0 && !set_first) {
        return;
    }
    if (shTexpStart != 0) {
        shTexpDif = shTexpStart - pSh->shTexpBase;
    }
    if (shClutpStart != 0) {
        shClutpDif = shClutpStart - pSh->shClutpBase;
    }
    Seq = Ah->seq;
    for (i = Ah->seqc; i > 0; i--, Seq++) {
        Shape = (OSHAPE_VU1 *)((char *)Ah + Seq->sha);
        if (set_first) {
            shTexpOld = TBP0_OF(Shape->tex0_l);
            if (shTexpOld >= 0x3800) {
                Shape->tex0_l &= ~0x3FFF;
                nTexOfs = shTexpOld - 0x1000;
                if (nTexOfs < 0x400) {
                    TRACE("partMng::Shape:\223]\221\227\203e\203N\203X\203`\203\203\210\312\222u\202\311\203}\203C\203i\203X\202\252\224\255\220\266\202\265\202\304\202\242\202\334\202\267!!!!! %d 0x%x\n", nTexOfs, shTexpOld);
                    nTexOfs = 0x400;
                }
                Shape->tex0_l += nTexOfs;
            } else {
                nBase = shTexpOld < 0x3600 ? 0x2A00 : 0x3600;
                Shape->tex0_l &= ~0x3FFF;
                nTexOfs = shTexpOld - nBase + 0x2600;
                if (nTexOfs < 0x400) {
                    TRACE("partMng::Shape:\213\244\222\312\203e\203N\203X\203`\203\203\210\312\222u\202\311\203}\203C\203i\203X\202\252\224\255\220\266\202\265\202\304\202\242\202\334\202\267!!!!! %d 0x%x\n", nTexOfs, shTexpOld);
                    nTexOfs = 0x400;
                }
                Shape->tex0_l += nTexOfs;
            }
            shClutpOld = (Shape->tex0_h & 0x7FFE0) >> 5;
            if (shClutpOld < 0x3D40) {
                Shape->tex0_h = (Shape->tex0_h & 0xFFF8001F) + ((shClutpOld + 0x100) << 5);
            } else {
                Shape->tex0_h = (Shape->tex0_h & 0xFFF8001F) + ((shClutpOld - 0x3D40 + 0x3E40) << 5);
            }
        }
        if (shTexpStart != 0 && shTexpDif != 0) {
            shTexpOld = TBP0_OF(Shape->tex0_l);
            if (shTexpOld >= 0x2800) {
#line 718
                ASSERTS(((0x2600) + (((0x100) / 2) * ((2) * (((0x100) / ((0x100) / 2)))))) <= (shTexpOld + shTexpDif), "((0x2600) + (((0x100) / 2) * ((2) * (((0x100) / ((0x100) / 2)))))) <= (shTexpOld + shTexpDif)");
                ASSERTS((shTexpOld + shTexpDif) < (((0x2600) + (((0x100) / 2) * ((2) * (((0x100) / ((0x100) / 2)))))) + (((0x100) / 2) * (((8) * (((0x100) / ((0x100) / 2)))) - ((2) * (((0x100) / ((0x100) / 2))))))), "(shTexpOld + shTexpDif) < (((0x2600) + (((0x100) / 2) * ((2) * (((0x100) / ((0x100) / 2)))))) + (((0x100) / 2) * (((8) * (((0x100) / ((0x100) / 2)))) - ((2) * (((0x100) / ((0x100) / 2)))))))");
                Shape->tex0_l = (Shape->tex0_l & ~0x3FFF) + (shTexpOld + shTexpDif);
            }
        }
        if (shClutpStart != 0 && shClutpDif != 0) {
            shClutpOld = (Shape->tex0_h & 0x7FFE0) >> 5;
            if (shClutpOld >= 0x3E40) {
#line 729
                ASSERTS(((0x3e00) + (((0x100) / 8) * (2))) <= (shClutpOld + shClutpDif), "((0x3e00) + (((0x100) / 8) * (2))) <= (shClutpOld + shClutpDif)");
                ASSERTS((shClutpOld + shClutpDif) < (((0x3e00) + (((0x100) / 8) * (2))) + (((0x100) / 8) * (((1)* ((0x100) / ((0x100) / 8))) - (2)))), "(shClutpOld + shClutpDif) < (((0x3e00) + (((0x100) / 8) * (2))) + (((0x100) / 8) * (((1)* ((0x100) / ((0x100) / 8))) - (2))))");
                Shape->tex0_h = (Shape->tex0_h & 0xFFF8001F) + ((shClutpOld + shClutpDif) << 5);
            }
        }
    }
}
#define VEC_COPY(d, s) (*(u_long128 *)(d) = *(u_long128 *)(s))

void pppSetUp(void *mng) {
    sceVu0FVECTOR dx;
    sceVu0FVECTOR dy;
    sceVu0FVECTOR dz;

    pppSetMatrix(mng);
    sceVu0MulMatrix(ppvWorldMatrix, ppvCameraMatrix, ppvParMatrix);
    sceVu0MulMatrix(ppvSWMatrix, ppvScreenMatrix, ppvWorldMatrix);
    dy[0] = ppvWorldMatrix[1][0];
    dy[1] = ppvWorldMatrix[1][1];
    dy[2] = ppvWorldMatrix[1][2];
    dy[3] = 0.0f;
    sceVu0Normalize(dy, dy);
    VEC_COPY(ppvWorldMatrixWood[1], dy);
    dx[0] = dy[1];
    dx[1] = -dy[0];
    dx[2] = 0.0f;
    dx[3] = 0.0f;
    sceVu0Normalize(dx, dx);
    VEC_COPY(ppvWorldMatrixWood[0], dx);
    sceVu0OuterProduct(dz, dx, dy);
    sceVu0Normalize(dz, dz);
    VEC_COPY(ppvWorldMatrixWood[3], ppvWorldMatrix[3]);
    VEC_COPY(ppvWorldMatrixWood[2], dz);
    sceVu0MulMatrix(ppvSWMatrixWood, ppvScreenMatrix, ppvWorldMatrixWood);
}

float *pppGetBbox(void *mng) {
    static sceVu0FVECTOR v[8];
    int i;
    sceVu0FVECTOR *bbox = (sceVu0FVECTOR *)((pppMngSt *)mng)->pEtc->afBbox;

    for (i = 0; i < 8; i++) {
        sceVu0ApplyMatrix(v[i], ppvParMatrix, bbox[i]);
    }
    return (float *)v;
}

static const int _DRAW_PRIORITY_FIRST_Tbl[8] = {1, 3, 5, 7, 9, 11, 13, 15};
static const int _DRAW_PRIORITY_LAST_Tbl[8] = {2, 4, 6, 8, 10, 12, 14, 16};

void pppSetDrawFirst(void *mng, int priority) {
    if ((u_int)priority < 8) {
        ((pppMngSt *)mng)->bDrawPriority = _DRAW_PRIORITY_FIRST_Tbl[priority];
    } else {
        ((pppMngSt *)mng)->bDrawPriority = 0;
    }
}

void pppSetDrawLast(void *mng, int priority) {
    if ((u_int)priority < 8) {
        ((pppMngSt *)mng)->bDrawPriority = _DRAW_PRIORITY_LAST_Tbl[priority];
    } else {
        ((pppMngSt *)mng)->bDrawPriority = 0;
    }
}

int pppIsDrawFirst(void *mng) {
    int i;
    int pri = ((pppMngSt *)mng)->bDrawPriority;

    if (pri == 0) {
        return 0;
    }
    for (i = 0; i < 8; i++) {
        if (_DRAW_PRIORITY_FIRST_Tbl[i] == pri) {
            return i + 1;
        }
    }
    return 0;
}

int pppIsDrawLast(void *mng) {
    int i;
    int pri = ((pppMngSt *)mng)->bDrawPriority;

    if (pri == 0) {
        return 0;
    }
    for (i = 0; i < 8; i++) {
        if (_DRAW_PRIORITY_LAST_Tbl[i] == pri) {
            return i + 1;
        }
    }
    return 0;
}

static int dpd_h_get_version(char *pchDpdData) {
    return *(int *)pchDpdData;
}

static char *dpd_h_skip_version(char *pchDpdData) {
    return pchDpdData + 4;
}

static char *dpd_h_skip_datainfo(char *pchDpdData, int nNum) {
    int nData;
    int i;

    for (i = 0; i < nNum; i++) {
        nData = *(int *)pchDpdData;
        pchDpdData += 4;
        pchDpdData += nData * 4;
    }
    return pchDpdData;
}

char *pppGetPdata(pppMngSt *pMng) {
    char *pchPdataOut = NULL;
    char *pchDpdData;
    int nId = pMng->bDpdId;
    int nData;
    int i;
    char *pchPdata;
    int nTexVramBlock;
    int nClutVramBlock;

    pchDpdData = dpd_h_skip_version(pMng->pchDpd);
    nData = *(int *)pchDpdData;
    pchDpdData += 4;
    for (i = 0; i < nData; i++) {
        if (nId == i) {
            pchPdata = *(char **)pchDpdData;
            pMng->pEtc = (pppPEtcData *)pchPdata;
            pchPdataOut = pchPdata + sizeof(pppPEtcData);
        }
        pchDpdData += 4;
    }
    pchDpdData = dpd_h_skip_datainfo(pchDpdData, 4);
    if (((DPD_HEAD_ETCDATA *)pchDpdData)->nMinTexVram != -1) {
        nTexVramBlock = (((DPD_HEAD_ETCDATA *)pchDpdData)->nMaxTexVram - 0x3781) / 128;
        nClutVramBlock = (((DPD_HEAD_ETCDATA *)pchDpdData)->nMaxClutVram - 0x3D21) / 32;
        pMng->bTexVramBlock = nTexVramBlock;
        pMng->bClutVramBlock = nClutVramBlock;
    } else {
        pMng->bTexVramBlock = 0;
        pMng->bClutVramBlock = 0;
    }
    return pchPdataOut;
}

static char *dpd_h_init_datainfo(char *pchDpdData, int nOffset) {
    int nData;
    int i;
    int j;

    pchDpdData = dpd_h_skip_version(pchDpdData);
    for (i = 0; i < 5; i++) {
        nData = *(int *)pchDpdData;
        pchDpdData += 4;
        for (j = 0; j < nData; j++) {
            *(int *)pchDpdData += nOffset;
            pchDpdData += 4;
        }
    }
    return pchDpdData;
}
static char *dpd_set_shape_tex(char *pchDpdData, u_char *pabUseId, short shTexpStart, short shClutpStart) {
    int nData;
    u_char bUseId = 0;
    int i;
    char *pchBuf;
    int nIdx;
    int nId;

    nData = *(int *)pchDpdData;
    pchDpdData += 4;
    for (i = 0; i < nData; i++) {
        pchBuf = *(char **)pchDpdData;
        pchDpdData += 4;
        nIdx = i >> 3;
        nId = i & 7;
#line 1039
        ASSERTS(0 == (int)pchBuf % 16, "0 == (int)pchBuf % 16");
        ASSERTS(nIdx < (16), "nIdx < (16)"); ASSERTS(0 <= nIdx, "0 <= nIdx");
        if (nId == 0) {
            bUseId = pabUseId[nIdx];
        }
        if (bUseId & 1) {
            setShapeTex0((OANM_HEADER *)(pchBuf + sizeof(DPD_SHAPE_HEADER)), (DPD_SHAPE_HEADER *)pchBuf, shTexpStart, shClutpStart);
            if (shTexpStart != 0) {
                ((DPD_SHAPE_HEADER *)pchBuf)->shTexpBase = shTexpStart;
            }
            if (shClutpStart != 0) {
                ((DPD_SHAPE_HEADER *)pchBuf)->shClutpBase = shClutpStart;
            }
        }
        bUseId >>= 1;
    }
    return pchDpdData;
}

static char *dpd_set_rsd_tex(char *pchDpdData, u_char *pabUseId, short shTexpStart, short shClutpStart) {
    int nData;
    u_char bUseId = 0;
    int i;
    char *pchBuf;
    int nIdx;
    int nId;

    nData = *(int *)pchDpdData;
    pchDpdData += 4;
    for (i = 0; i < nData; i++) {
        pchBuf = *(char **)pchDpdData;
        pchDpdData += 4;
        nIdx = i >> 3;
        nId = i & 7;
#line 1078
        ASSERTS(0 == (int)pchBuf % 16, "0 == (int)pchBuf % 16");
        ASSERTS(nIdx < (16), "nIdx < (16)"); ASSERTS(0 <= nIdx, "0 <= nIdx");
        if (nId == 0) {
            bUseId = pabUseId[nIdx];
        }
        if (bUseId & 1) {
            setOmdTex0((OMD_H *)(pchBuf + sizeof(DPD_MODEL_HEADER)), (DPD_MODEL_HEADER *)pchBuf, shTexpStart, shClutpStart);
            if (shTexpStart != 0) {
                ((DPD_MODEL_HEADER *)pchBuf)->shTexpBase = shTexpStart;
            }
            if (shClutpStart != 0) {
                ((DPD_MODEL_HEADER *)pchBuf)->shClutpBase = shClutpStart;
            }
        }
        bUseId >>= 1;
    }
    return pchDpdData;
}

static void dpd_set_object_tex(pppMngSt *pMng) {
    short shTexpStart = pMng->shTexpStart;
    short shClutpStart = pMng->shClutpStart;
    char *pchDpdData;

    pchDpdData = dpd_h_skip_version(pMng->pchDpd);
    pchDpdData = dpd_h_skip_datainfo(pchDpdData, 2);
    pchDpdData = dpd_set_shape_tex(pchDpdData, pMng->pEtc->abUseShape, shTexpStart, shClutpStart);
    dpd_set_rsd_tex(pchDpdData, pMng->pEtc->abUseRsd, shTexpStart, shClutpStart);
}
static char *dpd_set_shape_data(char *pchDpdData, u_char *pabUseId) {
    int nData;
    u_char bUseId = 0;
    int i;
    char *pchBuf;
    int nIdx;
    int nId;
    int nShpNo;
    pppShapeSt *pShpSt;

    nData = *(int *)pchDpdData;
    pchDpdData += 4;
    for (i = 0; i < nData; i++) {
        pchBuf = *(char **)pchDpdData;
        pchDpdData += 4;
        nIdx = i >> 3;
        nId = i & 7;
#line 1143
        ASSERTS(0 == (int)pchBuf % 16, "0 == (int)pchBuf % 16");
        ASSERTS(nIdx < (16), "nIdx < (16)"); ASSERTS(0 <= nIdx, "0 <= nIdx");
        if (nId == 0) {
            bUseId = pabUseId[nIdx];
        }
        if (bUseId & 1) {
            nShpNo = ((DPD_SHAPE_HEADER *)pchBuf)->shShpNo;
#line 1156
            ASSERTS(nShpNo < (128), "nShpNo < (128)");
            pShpSt = &RendShape[nShpNo];
            pShpSt->shape = (long *)(pchBuf + sizeof(DPD_SHAPE_HEADER));
            pShpSt->nPacketSize = ((DPD_SHAPE_HEADER *)pchBuf)->nPacketSize;
        }
        bUseId >>= 1;
    }
    return pchDpdData;
}

static char *dpd_set_rsd_data(char *pchDpdData, u_char *pabUseId) {
    int nData;
    u_char bUseId = 0;
    int i;
    char *pchBuf;
    int nIdx;
    int nId;
    OMD_H *pOmd;
    short shRsdNo;
    pppModelSt *pMdlSt;

    nData = *(int *)pchDpdData;
    pchDpdData += 4;
    for (i = 0; i < nData; i++) {
        pchBuf = *(char **)pchDpdData;
        pchDpdData += 4;
        nIdx = i >> 3;
        nId = i & 7;
#line 1186
        ASSERTS(0 == (int)pchBuf % 16, "0 == (int)pchBuf % 16");
        ASSERTS(nIdx < (16), "nIdx < (16)"); ASSERTS(0 <= nIdx, "0 <= nIdx");
        if (nId == 0) {
            bUseId = pabUseId[nIdx];
        }
        if (bUseId & 1) {
            pOmd = (OMD_H *)(pchBuf + sizeof(DPD_MODEL_HEADER));
            shRsdNo = ((DPD_MODEL_HEADER *)pchBuf)->shRsdNo;
#line 1201
            ASSERTS(shRsdNo < (128), "shRsdNo < (128)");
            pMdlSt = &RendModel[shRsdNo];
            pMdlSt->omd = pOmd;
            pMdlSt->nvert = pOmd->vc;
            pMdlSt->nnorm = pOmd->nc;
            pMdlSt->npoly = pOmd->pc;
            pMdlSt->vert = (pppSVECTOR *)((char *)pOmd + pOmd->va);
            pMdlSt->norm = (pppSVECTOR *)((char *)pOmd + pOmd->na);
            pMdlSt->nPacketSize = ((DPD_MODEL_HEADER *)pchBuf)->nPacketSize;
        }
        bUseId >>= 1;
    }
    return pchDpdData;
}
static char *dpd_set_vsf_data(char *pchDpdData, u_char *pabUseId) {
    int nData;
    u_char bUseId = 0;
    int i;
    char *pchBuf;
    int nIdx;
    int nId;
    int vsfno;
    pppVsfSt *vsf;

    nData = *(int *)pchDpdData;
    pchDpdData += 4;
    for (i = 0; i < nData; i++) {
        pchBuf = *(char **)pchDpdData;
        pchDpdData += 4;
        nIdx = i >> 3;
        nId = i & 7;
#line 1233
        ASSERTS(0 == (int)pchBuf % 16, "0 == (int)pchBuf % 16");
        ASSERTS(nIdx < (16), "nIdx < (16)"); ASSERTS(0 <= nIdx, "0 <= nIdx");
        if (nId == 0) {
            bUseId = pabUseId[nIdx];
        }
        if (bUseId & 1) {
            vsfno = ((int *)pchBuf)[0];
#line 1244
            ASSERTS(vsfno < (128), "vsfno < (128)");
            vsf = &RendVsf[vsfno];
            vsf->modelno = ((int *)pchBuf)[1];
            vsf->indexn = ((int *)pchBuf)[2];
            vsf->index = (u_short *)(pchBuf + 12);
        }
        bUseId >>= 1;
    }
    return pchDpdData;
}

static void dpd_set_object_data(pppMngSt *pMng) {
    char *pchDpdData;

    pchDpdData = dpd_h_skip_version(pMng->pchDpd);
    pchDpdData = dpd_h_skip_datainfo(pchDpdData, 2);
    pchDpdData = dpd_set_shape_data(pchDpdData, pMng->pEtc->abUseShape);
    pchDpdData = dpd_set_rsd_data(pchDpdData, pMng->pEtc->abUseRsd);
    dpd_set_vsf_data(pchDpdData, pMng->pEtc->abUseVsf);
}

KH_DEBUG(void debugDpdCheckShapeData(int nShapeNum) {)
    KH_DEBUG(u_char *pabUseId;)
    KH_DEBUG(int nIdx;)
    KH_DEBUG(int nId;)

#line 1296
    KH_DEBUG(ASSERTS((ppvMng), "(ppvMng)");)
    KH_DEBUG(pabUseId = ppvMng->pEtc->abUseShape;)
    KH_DEBUG(nIdx = nShapeNum >> 3;)
    KH_DEBUG(nId = nShapeNum & 7;)
#line 1300
    KH_DEBUG(ASSERTS(nIdx < (16), "nIdx < (16)"); ASSERTS(0 <= nIdx, "0 <= nIdx");)
    KH_DEBUG(ASSERTS(pabUseId[nIdx] & (1<<nId), "pabUseId[nIdx] & (1<<nId)");)
KH_DEBUG(})
static char *dpd_set_monochrome_tex_data(char *data) {
    int num;
    int i;
    int j;
    DPD_TEXTURE *tex;
    u_char *p;
    int len;
    float y;

    num = *(int *)data;
    data += 4;
    for (i = 0; i < num; i++) {
        tex = *(DPD_TEXTURE **)data;
        data += 4;
        len = 0;
        p = tex->abData + tex->unClutStart;
        if (tex->shCltDbp == 0) {
            continue;
        }
        switch (tex->shDpsm) {
        case 0x13:
            len = 0x100;
            break;
        case 0x14:
            len = 0x10;
            break;
        }
        for (j = 0; j < len; j++) {
            y = p[0] * 0.3f + p[1] * 0.59f + p[2] * 0.11f;
            p[0] = p[1] = p[2] = y;
            p += 4;
        }
    }
    return data;
}
#define MONO_RGB(p)                                     \
    y = (p)[0] * 0.3f + (p)[1] * 0.59f + (p)[2] * 0.11f; \
    (p)[2] = (p)[1] = (p)[0] = y

static char *dpd_set_monochrome_rsd_data(char *data) {
    int num;
    int i;
    int j;
    DPD_MODEL_HEADER *model;
    OMD_H *omd;
    OMD_PRIM_H *mph;
    int size;
    char *prim;
    int pc;

    num = *(int *)data;
    data += 4;
    for (i = 0; i < num; i++) {
        model = *(DPD_MODEL_HEADER **)data;
        data += 4;
        size = 0;
        omd = (OMD_H *)(model + 1);
        mph = (OMD_PRIM_H *)((char *)omd + omd->pa);
#line 1353
        ASSERTS(0 == (int)model % 16, "0 == (int)model % 16");
        while (mph->ptype != -1) {
            u_char *p;
            p = (u_char *)(mph + 1);
            pc = mph->pc;
            prim = (char *)(mph + 1);
            switch (mph->ptype) {
            case 0:
            {
                float y;
                size = 0x14;
                for (j = 0; j < pc; j++) {
                    MONO_RGB(p);
                    MONO_RGB(p + 4);
                    MONO_RGB(p + 8);
                    p += size;
                }
                break;
            }
            case 1:
            {
                float y;
                size = 0x1C;
                for (j = 0; j < pc; j++) {
                    MONO_RGB(p);
                    MONO_RGB(p + 4);
                    MONO_RGB(p + 8);
                    p += size;
                }
                break;
            }
            case 2:
            {
                float y;
                size = 0x20;
                for (j = 0; j < pc; j++) {
                    MONO_RGB(p);
                    MONO_RGB(p + 4);
                    MONO_RGB(p + 8);
                    p += size;
                }
                break;
            }
            case 3:
            {
                float y;
                size = 0x28;
                for (j = 0; j < pc; j++) {
                    MONO_RGB(p);
                    MONO_RGB(p + 4);
                    MONO_RGB(p + 8);
                    p += size;
                }
                break;
            }
            case 4:
            {
                float y;
                size = 0x18;
                for (j = 0; j < pc; j++) {
                    MONO_RGB(p);
                    MONO_RGB(p + 4);
                    MONO_RGB(p + 8);
                    MONO_RGB(p + 12);
                    p += size;
                }
                break;
            }
            case 5:
            {
                float y;
                size = 0x20;
                for (j = 0; j < pc; j++) {
                    MONO_RGB(p);
                    MONO_RGB(p + 4);
                    MONO_RGB(p + 8);
                    MONO_RGB(p + 12);
                    p += size;
                }
                break;
            }
            case 6:
            {
                float y;
                size = 0x28;
                for (j = 0; j < pc; j++) {
                    MONO_RGB(p);
                    MONO_RGB(p + 4);
                    MONO_RGB(p + 8);
                    MONO_RGB(p + 12);
                    p += size;
                }
                break;
            }
            case 7:
            {
                float y;
                size = 0x30;
                for (j = 0; j < pc; j++) {
                    MONO_RGB(p);
                    MONO_RGB(p + 4);
                    MONO_RGB(p + 8);
                    MONO_RGB(p + 12);
                    p += size;
                }
                break;
            }
            default:
#line 1489
                ASSERTS(!("unknown ptype."), "!(\"unknown ptype.\")");
                break;
            }
            mph = (OMD_PRIM_H *)(prim + size * pc);
        }
    }
    return data;
}

static char *dpd_set_monochrome_shape(char *data) {
    int num;
    int i;
    int j;
    int k;
    DPD_SHAPE_HEADER *shape;
    OANM_HEADER *ah;
    int seqc;
    OANM_SEQ *seq;
    OSHAPE_VU1 *sh;
    OSPRITE_VU1A *sp;

    num = *(int *)data;
    data += 4;
    for (i = 0; i < num; i++) {
        shape = *(DPD_SHAPE_HEADER **)data;
        data += 4;
        ah = (OANM_HEADER *)(shape + 1);
        seqc = ah->seqc;
        for (j = 0; j < seqc; j++) {
            seq = &ah->seq[j];
            sh = (OSHAPE_VU1 *)((char *)ah + seq->sha);
            sp = (OSPRITE_VU1A *)((char *)sh + sh->spvu1a);
            for (k = 0; k < sh->counter; k++) {
                float y = sp->r * 0.3f + sp->g * 0.59f + sp->b * 0.11f;
                sp->b = sp->g = sp->r = y;
                sp++;
            }
        }
    }
    return data;
}

static void dpd_set_monochrome(char *dpd) {
    dpd = dpd_h_skip_version(dpd);
    dpd = dpd_h_skip_datainfo(dpd, 1);
    dpd = dpd_set_monochrome_tex_data(dpd);
    dpd = dpd_set_monochrome_shape(dpd);
    dpd_set_monochrome_rsd_data(dpd);
}
void pppInitDpd(char *pchDpdData) {
    char *pchDpdData0 = pchDpdData;
    int nData;
    int i;
    char *pchPdata;

#line 1546
    ASSERTS(((void *)0) != pchDpdData, "((void *)0) != pchDpdData");
    ASSERTS(0 == (int)pchDpdData % 16, "0 == (int)pchDpdData % 16");
    dpd_h_init_datainfo(pchDpdData, (int)pchDpdData);
    pchDpdData = dpd_h_skip_version(pchDpdData0);
    nData = *(int *)pchDpdData;
    pchDpdData += 4;
    for (i = 0; i < nData; i++) {
        pchPdata = *(char **)pchDpdData;
        pchDpdData += 4;
#line 1586
        ASSERTS(0 == (int)pchPdata % 16, "0 == (int)pchPdata % 16");
        pppInitPdt((long *)pchPdata, pppProgTbl);
    }
    if (ppvMonochrome) {
        dpd_set_monochrome(pchDpdData0);
    }
}

void pppMoveDpd(char *pchDpdData, int nOffset) {
    int nData;
    int i;
    char *pchPdata;

#line 1605
    ASSERTS(((void *)0) != pchDpdData, "((void *)0) != pchDpdData");
    ASSERTS(0 == (int)pchDpdData % 16, "0 == (int)pchDpdData % 16");
    dpd_h_init_datainfo(pchDpdData, nOffset);
    pchDpdData = dpd_h_skip_version(pchDpdData);
    nData = *(int *)pchDpdData;
    pchDpdData += 4;
    for (i = 0; i < nData; i++) {
        pchPdata = *(char **)pchDpdData;
        pchDpdData += 4;
#line 1624
        ASSERTS(0 == (int)pchPdata % 16, "0 == (int)pchPdata % 16");
        pppMovePdt((long *)pchPdata, nOffset);
    }
}

void pppInitTexCommon(char *pchDpdData) {
    pppInitDpd(pchDpdData);
    g_pchDpdTexCommon = pchDpdData;
}

int sceGsSetDefLoadImage(sceGsLoadImage *lp, short dbp, short dbw, short dpsm, short x, short y, short w, short h) {
    int size = 0;

    switch (dpsm) {
    case 0x00:
    case 0x30:
        size = w * h >> 2;
        break;
    case 0x01:
    case 0x31:
        size = w * h * 3 >> 4;
        break;
    case 0x02:
    case 0x0A:
    case 0x32:
    case 0x3A:
        size = w * h >> 3;
        break;
    case 0x13:
    case 0x1B:
        size = w * h >> 4;
        break;
    case 0x14:
    case 0x24:
    case 0x2C:
        size = w * h >> 5;
        break;
    }
    if (size >= 0x8000) {
        TRACE("sceGsSetDefLoadImage: too big size\r\n");
        return 0;
    }
    lp->giftag0[0] = (u_long)4 | ((u_long)1 << 60);
    lp->giftag0[1] = 0xE;
    lp->bitbltbuf = ((u_long)dbp << 32) | ((u_long)dbw << 48) | ((u_long)dpsm << 56);
    lp->bitbltbufaddr = 0x50;
    lp->trxpos = ((u_long)x << 32) | ((u_long)y << 48);
    lp->trxposaddr = 0x51;
    lp->trxreg = (u_long)w | ((u_long)h << 32);
    lp->trxregaddr = 0x52;
    lp->trxdir = 0;
    lp->trxdiraddr = 0x53;
    lp->giftag1[0] = (u_long)size | ((u_long)1 << 15) | ((u_long)2 << 58);
    lp->giftag1[1] = 0;
    __asm__ volatile("sync.l");
    return 6;
}
static void set_texture(DPD_TEXTURE *pTex, short shTexDbpStart) {
    sceGsLoadImage *gs_load_image = (sceGsLoadImage *)(((u_int)&s_GsLoadImageTex & 0x0FFFFFFF) | 0x30000000);
    short shTexDbp = pTex->shTexDbp;

#line 1726
    ASSERTS(((void *)0) != pTex, "((void *)0) != pTex");
    if (shTexDbp >= 0x3800) {
        shTexDbp = shTexDbp - 0x3800 + shTexDbpStart;
    KH_DEBUG(} else if (shTexDbpStart != 0x2600) {)
        KH_DEBUG(TRACE("set_texture::\213\244\227LTEXTURE\202\252\223]\221\227\210\265\202\242\202\276\202\274 %08x\n", shTexDbp);)
        KH_DEBUG(shTexDbp = shTexDbp - 0x3000;)
    } else {
        shTexDbp = KH_VER(shTexDbp - 0x400, (short)(shTexDbp - 0x2A00) + shTexDbpStart); /* retail: one formula */
    }
    sceGsSetDefLoadImage(gs_load_image, shTexDbp, pTex->shDbw, pTex->shDpsm, pTex->shX, pTex->shY, pTex->shW, pTex->shH);
    sceGsExecLoadImage(gs_load_image, (u_long128 *)pTex->abData);
}

static void set_clut(DPD_TEXTURE *pTex, short shCltDbpStart) {
    sceGsLoadImage *gs_load_image = (sceGsLoadImage *)(((u_int)&s_GsLoadImageClt & 0x0FFFFFFF) | 0x30000000);
    short shCltDbp = pTex->shCltDbp;

#line 1763
    ASSERTS(((void *)0) != pTex, "((void *)0) != pTex");
    if (shCltDbp == 0) {
        return;
    }
    if (shCltDbp >= 0x3D40) {
        shCltDbp = shCltDbp - 0x3D40 + shCltDbpStart;
    } else KH_DEBUG(if (shCltDbpStart == 0x3E00)) {
        shCltDbp = KH_VER(shCltDbp + 0x100, shCltDbp - 0x3D00 + shCltDbpStart); /* retail: one formula */
    KH_DEBUG(} else {)
        KH_DEBUG(TRACE("set_clut::\213\244\227LCLUT\202\252\223]\221\227\210\265\202\242\202\276\202\274 %08x\n", shCltDbp);)
        KH_DEBUG(return;)
    }
#line 1793
    ASSERTS((0x3e00) <= shCltDbp, "(0x3e00) <= shCltDbp");
    ASSERTS(shCltDbp+pTex->shCltVramSize <= (((0x3e00) + (((0x100) / 8) * (2))) + (((0x100) / 8) * (((1)* ((0x100) / ((0x100) / 8))) - (2)))), "shCltDbp+pTex->shCltVramSize <= (((0x3e00) + (((0x100) / 8) * (2))) + (((0x100) / 8) * (((1)* ((0x100) / ((0x100) / 8))) - (2))))");
    switch (pTex->shDpsm) {
    case 0x13:
        sceGsSetDefLoadImage(gs_load_image, shCltDbp, 1, 0, 0, 0, 16, 16);
        break;
    case 0x14:
        sceGsSetDefLoadImage(gs_load_image, shCltDbp, 1, 0, 0, 0, 8, 2);
        break;
    default:
        return;
    }
    sceGsExecLoadImage(gs_load_image, (u_long128 *)(pTex->abData + pTex->unClutStart));
}

static void dpd_set_texture(char *pchDpdData, u_char *pabUseId, short shTexpStart) {
    int nData;
    u_char bUseId = 0;
    int i;
    char *pchBuf;
    int nIdx;
    int nId;

#line 1826
    ASSERTS(((void *)0) != pchDpdData, "((void *)0) != pchDpdData");
    pchDpdData = dpd_h_skip_version(pchDpdData);
    pchDpdData = dpd_h_skip_datainfo(pchDpdData, 1);
    nData = *(int *)pchDpdData;
    pchDpdData += 4;
    for (i = 0; i < nData; i++) {
        pchBuf = *(char **)pchDpdData;
        pchDpdData += 4;
        nIdx = i >> 3;
        nId = i & 7;
#line 1839
        ASSERTS(0 == (int)pchBuf % 16, "0 == (int)pchBuf % 16");
        ASSERTS(nIdx < (16), "nIdx < (16)"); ASSERTS(0 <= nIdx, "0 <= nIdx");
        if (nId == 0) {
            bUseId = pabUseId[nIdx];
        }
        if (bUseId & 1) {
            KH_DEBUG(if (shTexpStart < 0x2800) {)
                KH_DEBUG(shTexpStart = 0x2800;)
            KH_DEBUG(})
            set_texture((DPD_TEXTURE *)pchBuf, shTexpStart);
        }
        bUseId >>= 1;
    }
}

static void dpd_set_common_texture(char *pchDpdData) {
    int nData;
    int i;
    char *pchBuf;

#line 1862
    ASSERTS(((void *)0) != pchDpdData, "((void *)0) != pchDpdData");
    pchDpdData = dpd_h_skip_version(pchDpdData);
    pchDpdData = dpd_h_skip_datainfo(pchDpdData, 1);
    nData = *(int *)pchDpdData;
    pchDpdData += 4;
    for (i = 0; i < nData; i++) {
        pchBuf = *(char **)pchDpdData;
        pchDpdData += 4;
#line 1873
        ASSERTS(0 == (int)pchBuf % 16, "0 == (int)pchBuf % 16");
        set_texture((DPD_TEXTURE *)pchBuf, 0x2600);
    }
}

static void dpd_set_clut(char *pchDpdData, u_char *pabUseId, short shClutpStart) {
    int nData;
    u_char bUseId = 0;
    int i;
    char *pchBuf;
    int nIdx;
    int nId;

    if (pchDpdData != NULL) {
        pchDpdData = dpd_h_skip_version(pchDpdData);
        pchDpdData = dpd_h_skip_datainfo(pchDpdData, 1);
        nData = *(int *)pchDpdData;
        pchDpdData += 4;
        for (i = 0; i < nData; i++) {
            pchBuf = *(char **)pchDpdData;
            pchDpdData += 4;
            nIdx = i >> 3;
            nId = i & 7;
#line 1904
            ASSERTS(0 == (int)pchBuf % 16, "0 == (int)pchBuf % 16");
            ASSERTS(nIdx < (16), "nIdx < (16)"); ASSERTS(0 <= nIdx, "0 <= nIdx");
            if (nId == 0) {
                bUseId = pabUseId[nIdx];
            }
            if (bUseId & 1) {
                set_clut((DPD_TEXTURE *)pchBuf, shClutpStart);
            }
            bUseId >>= 1;
        }
    }
}

static void dpd_set_common_clut(char *pchDpdData) {
    int nData;
    int i;
    char *pchBuf;

    if (pchDpdData != NULL) {
        pchDpdData = dpd_h_skip_version(pchDpdData);
        pchDpdData = dpd_h_skip_datainfo(pchDpdData, 1);
        nData = *(int *)pchDpdData;
        pchDpdData += 4;
        for (i = 0; i < nData; i++) {
            pchBuf = *(char **)pchDpdData;
            pchDpdData += 4;
#line 1935
            ASSERTS(0 == (int)pchBuf % 16, "0 == (int)pchBuf % 16");
            set_clut((DPD_TEXTURE *)pchBuf, 0x3E00);
        }
    }
}

static void vramcache_clear(VRAMCACHE *pVcache) {
    pVcache->pchDpd = NULL;
    pVcache->unDpdId = 0;
    pVcache->chBlockStart = -1;
    pVcache->chBlockNum = 0;
    pVcache->bBoolUse = 0;
}

static void vramcachemng_clear(VRAMCACHE_MNG *pVcacheMng) {
    int i;

    for (i = 0; i < 12; i++) {
        vramcache_clear(&pVcacheMng->aVramCacheTex[i]);
    }
    for (i = 0; i < 6; i++) {
        vramcache_clear(&pVcacheMng->aVramCacheClut[i]);
    }
}

/* abBlock is a VLA in the original (.mdebug: its address in a stack slot) of a constant size: a const int local, which
   gcc leaves out of .mdebug once its reads are folded. The loops' pVcache locals that loop.c strength-reduces away are
   the blocks without variables in the original's .mdebug block tree. */
static int vramcache(VRAMCACHE *paVcache, int nBlockMax, pppMngSt *pMng, u_char bUseBlockNum) {
    const int BLOCK_MAX = 40;
    u_char abBlock[BLOCK_MAX];
    char *pchDpd = pMng->pchDpd;
    int nParId = pMng->bDpdId;
    int i;
    int j;

#line 2031
    ASSERTS(nBlockMax <= BLOCK_MAX, "nBlockMax <= BLOCK_MAX");
    ASSERTS(((void *)0) != pchDpd, "((void *)0) != pchDpd");
    ASSERTS(0 <= nParId && nParId < 64, "0 <= nParId && nParId < 64");
    for (i = 0; i < nBlockMax; i++) {
        VRAMCACHE *pVcache = &paVcache[i];

        if (pVcache->bBoolUse
            && pchDpd == pVcache->pchDpd) {
            if ((pVcache->unDpdId >> nParId) & 1) {
                return -1;
            }
            pVcache->unDpdId |= (u_long)1 << nParId;
            return pVcache->chBlockStart;
        }
    }
    for (i = 0; i < BLOCK_MAX; i++) {
        abBlock[i] = 0;
    }
    for (i = 0; i < nBlockMax; i++) {
        VRAMCACHE *pVcache = &paVcache[i];

        if (pVcache->bBoolUse) {
            for (j = 0; j < pVcache->chBlockNum; j++) {
#line 2061
                ASSERTS((pVcache->chBlockStart+j) < BLOCK_MAX, "(pVcache->chBlockStart+j) < BLOCK_MAX");
                abBlock[pVcache->chBlockStart + j] = 1;
            }
        }
    }
    {
        int nEmptyCt = 0;
        int nStartBlock = -1;

        for (i = 0; i < nBlockMax; i++) {
            if (abBlock[i]) {
                nEmptyCt = 0;
            } else {
                nEmptyCt++;
                if (nEmptyCt == bUseBlockNum) {
                    nStartBlock = i + 1 - bUseBlockNum;
#line 2078
                    ASSERTS(0 <= nStartBlock, "0 <= nStartBlock");
                    break;
                }
            }
        }
        if (nStartBlock > 0) {
            for (i = 0; i < nBlockMax; i++) {
                VRAMCACHE *pVcache = &paVcache[i];

                if (!pVcache->bBoolUse) {
                    pVcache->pchDpd = pchDpd;
                    pVcache->unDpdId = (u_long)1 << nParId;
                    pVcache->chBlockStart = nStartBlock;
                    pVcache->chBlockNum = bUseBlockNum;
                    pVcache->bBoolUse = 1;
                    return nStartBlock;
                }
            }
#line 2101
            ASSERTS(!("Not find VRAMCACHE!"), "!(\"Not find VRAMCACHE!\")");
        }
    }
    {
        VRAMCACHE *pVcacheTarget = NULL;
        int nDif = -1;

        for (i = 0; i < nBlockMax; i++) {
            VRAMCACHE *pVcache = &paVcache[i];

            if (pVcache->bBoolUse) {
                if (bUseBlockNum == pVcache->chBlockNum) {
                    pVcache->pchDpd = pchDpd;
                    pVcache->unDpdId = (u_long)1 << nParId;
                    return pVcache->chBlockStart;
                }
                if (pVcache->chBlockNum > bUseBlockNum) {
                    if (pVcacheTarget == NULL || pVcache->chBlockNum - bUseBlockNum < nDif) {
                        pVcacheTarget = pVcache;
                        nDif = pVcache->chBlockNum - bUseBlockNum;
                    }
                }
            }
        }
        if (pVcacheTarget != NULL) {
            pVcacheTarget->pchDpd = pchDpd;
            pVcacheTarget->unDpdId = (u_long)1 << nParId;
            pVcacheTarget->chBlockNum = bUseBlockNum;
            return pVcacheTarget->chBlockStart;
        }
    }
    for (i = 0; i < nBlockMax; i++) {
        VRAMCACHE *pVcache = &paVcache[i];

        if (pVcache->bBoolUse && pVcache->chBlockStart < bUseBlockNum) {
            vramcache_clear(pVcache);
        }
    }
    for (i = 0; i < nBlockMax; i++) {
        VRAMCACHE *pVcache = &paVcache[i];

        if (!pVcache->bBoolUse) {
            pVcache->pchDpd = pchDpd;
            pVcache->unDpdId = (u_long)1 << nParId;
            pVcache->chBlockStart = 0;
            pVcache->chBlockNum = bUseBlockNum;
            pVcache->bBoolUse = 1;
            return 0;
        }
    }
#line 2169
    ASSERTS(0, "0");
    return -1;
}

static void vramcache_texture(VRAMCACHE_MNG *pVcacheMng, pppMngSt *pMng) {
    int nStartBlock = vramcache(pVcacheMng->aVramCacheTex, 12, pMng, pMng->bTexVramBlock);

    if (nStartBlock < 0) {
        pMng->shTexpStart = 0;
    } else {
        pMng->shTexpStart = nStartBlock * 128 + 0x2800;
    }
}

static void vramcache_clut(VRAMCACHE_MNG *pVcacheMng, pppMngSt *pMng) {
    int nStartBlock = vramcache(pVcacheMng->aVramCacheClut, 6, pMng, pMng->bClutVramBlock);

    if (nStartBlock < 0) {
        pMng->shClutpStart = 0;
    } else {
        pMng->shClutpStart = nStartBlock * 32 + 0x3E40;
    }
}
char *pppPrintProf(void *mng) {
    return "NO PROFILE.\n";
}

char *pppPrintPerformance() {
    static char buff[256];

    sprintf(buff, "P4:%d/%d  P3:%d/%d  SHP:%d/%d  WORK:%.2f%%\n", os_debug_draw.poly4d_c, os_debug_draw.poly4_c,
            os_debug_draw.poly3d_c, os_debug_draw.poly3_c, os_debug_draw.shaped_c, os_debug_draw.shape_c,
            (float)g_Env.heapMng.allocSize / (float)ppvWorkMemSize * 100.0f);
    return buff;
}

u_char ppvVu1SprDbId __attribute__((section(".bss"))) = 0;
int ppvVu1Prog __attribute__((section(".bss"))) = 0;

void pppGetWorkMemoryInfo(int *memory_use, int *memory_size) {
    *memory_use = g_Env.heapMng.allocSize;
    *memory_size = ppvWorkMemSize;
}

void pppPartCalcReady() {
    g_CurrentEffData.pchDpdData = NULL;
    g_CurrentEffData.nDpdId = -1;
}

static inline int pppIsCurrentEffData(pppMngSt *pMng) {
    return g_CurrentEffData.pchDpdData == pMng->pchDpd && g_CurrentEffData.nDpdId == pMng->bDpdId;
}

#define pMng ((pppMngSt *)mng)
int pppPartCalc(void *mng, int view_clip) {
    int result = 1;

#line 2407
    ASSERTS(pMng != ((void *)0), "pMng != ((void *)0)");
    ppvMng = pMng;
    if (!ppvUserStopPartF && pMng->bFlagStop == 0 && pMng->bBoolSlow == 0 && !(pMng->usef & 8)) {
        if (pMng->fWaitCt > 0.0f) {
            pMng->fWaitCt -= ppvFrameTime;
        }
    }
    if (pMng->fWaitCt > 0.0f) {
        return 1;
    }
    if (pMng->time != 0 && !pMng->bBoolNoViewClip && view_clip) {
        pMng->bViewClip = 1;
    } else {
        pMng->bViewClip = 0;
    }
    if (!pMng->bViewClip || !pMng->pEtc->bViewClipStop || !pMng->partLoopf || pMng->partLoopEndf) {
        if (!pppIsCurrentEffData(pMng)) {
            dpd_set_object_data(pMng);
        }
        pMng->fFrameTimeAccum += ppvFrameTime;
        if (pMng->fFrameTimeAccum >= 2.0f) {
            pMng->bBoolSlow = 0;
            for (; pMng->skip_frame > 0; pMng->skip_frame--) {
                if (!pppRunPartExec(pMng) || pppRunPart(pMng, FALSE)) {
                    goto error;
                }
            }
            for (pMng->fFrameTimeAccum -= 2.0f; pMng->fFrameTimeAccum >= 2.0f; pMng->fFrameTimeAccum -= 2.0f) {
                if (!pppRunPartExec(pMng) || pppRunPart(pMng, FALSE)) {
                    goto error;
                }
            }
            if (!pppRunPartExec(pMng)) {
                goto error;
            }
        } else {
            pMng->bBoolSlow = 1;
            if (!pppRunPartExec(pMng)) {
                goto error;
            }
        }
        goto end;
    error:
        pppAllFreePObject(pMng);
        result = 0;
    end:
        g_CurrentEffData.pchDpdData = pMng->pchDpd;
        g_CurrentEffData.nDpdId = pMng->bDpdId;
    }
    ppvMng = NULL;
    return result;
}
#undef pMng

void pppPartCalcEnd() {
}
static void pppInitVram() {
    vramcachemng_clear(&g_VcacheMng);
    dpd_set_common_texture(g_pchDpdTexCommon);
    dpd_set_common_clut(g_pchDpdTexCommon);
    sceGsSyncPath(0, 0);
    sceDmaSend(oa_dmagif, (void *)alDmaPkt);
}

static void pppInitGsReg() {
    op_gs2_init_eff();
}

void pppInitDraw(BOOL bDraw) {
    if (bDraw) {
        pppInitVram();
        pppInitGsReg();
    }
    ppvVu1Prog = 0;
    ppvVu1SprDbId = 0;
}

void pppPartDrawReady(BOOL bDraw) {
    g_CurrentEffData.pchDpdData = NULL;
    g_CurrentEffData.nDpdId = -1;
    pppInitDraw(bDraw);
    pppInitVu1ShapeProg();
    drawStart();
}

#define pMng ((pppMngSt *)mng)
int pppPartDraw(void *mng, BOOL bDraw) {
    int result = 1;

#line 2592
    ASSERTS(pMng != ((void *)0), "pMng != ((void *)0)");
    ppvMng = pMng;
    /* through a pointer: no struct member to gcc's alias analysis, so the load stays after the ppvMng store */
    if (*&pMng->fWaitCt > 0.0f) {
        pMng->usef &= ~8;
        return 1;
    }
    if (!pMng->bViewClip || !pMng->pEtc->bViewClipStop || !pMng->partLoopf || pMng->partLoopEndf) {
        if (pMng->bViewClip) {
            if (!pppIsCurrentEffData(pMng)) {
                dpd_set_object_data(pMng);
            }
            if (pppRunPart(pMng, FALSE)) {
                pppAllFreePObject(pMng);
                result = 0;
            }
            g_CurrentEffData.pchDpdData = pMng->pchDpd;
            g_CurrentEffData.nDpdId = pMng->bDpdId;
        } else {
            if (!pppIsCurrentEffData(pMng)) {
                dpd_set_object_data(pMng);
            }
            if (pMng->bTexVramBlock && bDraw) {
                vramcache_texture(&g_VcacheMng, pMng);
                vramcache_clut(&g_VcacheMng, pMng);
                if (pMng->shTexpStart != 0) {
                    dpd_set_texture(pMng->pchDpd, pMng->pEtc->abUseTexture, pMng->shTexpStart);
                }
                if (pMng->shClutpStart != 0) {
                    dpd_set_clut(pMng->pchDpd, pMng->pEtc->abUseTexture, pMng->shClutpStart);
                }
                if (pMng->shTexpStart != 0 || pMng->shClutpStart != 0) {
                    sceGsSyncPath(0, 0);
                    sceDmaSend(oa_dmagif, (void *)alDmaPkt);
                }
            } else {
                pMng->shTexpStart = 0;
                pMng->shClutpStart = 0;
            }
            dpd_set_object_tex(pMng);
            ppvCurPrimp = ppvPrimp;
            ppvCurPrimpLen = 0;
            if (pppRunPart(pMng, bDraw)) {
                pppAllFreePObject(pMng);
                result = 0;
            }
            drawEnd();
            g_CurrentEffData.pchDpdData = pMng->pchDpd;
            g_CurrentEffData.nDpdId = pMng->bDpdId;
        }
    }
    pMng->usef &= ~8;
    ppvMng = NULL;
    return result;
}
#undef pMng

void pppPartDrawEnd() {
}
static void init_part_mng(pppMngSt *pMng) {
    memset(pMng, 0, sizeof(pppMngSt));
    sceVu0UnitMatrix(pMng->parMat);
    pMng->usef = 8;
    pMng->parChrScl[3] = pMng->parChrScl[2] = pMng->parChrScl[1] = pMng->parChrScl[0] = pMng->parCol[3] =
        pMng->parCol[2] = pMng->parCol[1] = pMng->parCol[0] = pMng->parScl[3] = pMng->parScl[2] = pMng->parScl[1] =
            pMng->parScl[0] = 1.0f;
}

void *pppGetPartMng() {
    pppMngSt *mng = pppAlloc(&ppvEnv->heapMng, sizeof(pppMngSt));

    if (mng != NULL) {
        init_part_mng(mng);
    } else {
        TRACE("!!!!!!!!!!!!!!!!!!!!!!!!! Effect pppGetPartMng Malloc FALSE !!!!!!!!!!!!!!!!!!!!!!!\n");
    }
    return mng;
}

void pppFreeMng(void *mng) {
    if (((pppMngSt *)mng)->usef & 1) {
        ((pppMngSt *)mng)->usef = (((pppMngSt *)mng)->usef & ~1) | 2;
    }
}

void pppDeleteMng(void *mng) {
    pppFree(&ppvEnv->heapMng, mng);
}

int pppStartPartMng(void *mng, u_long128 *dpd, u_int dpd_id, int priority, float wait) {
    void *pdata;

    ((pppMngSt *)mng)->pchDpd = (char *)dpd;
    ((pppMngSt *)mng)->bDpdId = dpd_id;
    ((pppMngSt *)mng)->fWaitCt = wait;
    pdata = pppGetPdata(mng);
    if (pdata == NULL) {
#line 2848
        ASSERTS(((void *)0) != pdata, "((void *)0) != pdata");
        return 0;
    }
    return pppStartPart(mng, pdata, TRUE) != 0;
}

sceVu0FMATRIX *pppPartMngGetMatrix(void *mng) {
    return &((pppMngSt *)mng)->parMat;
}

sceVu0FVECTOR *pppPartMngGetPos(void *mng) {
    return &((pppMngSt *)mng)->parLoc;
}

sceVu0FVECTOR *pppPartMngGetScl(void *mng) {
    return &((pppMngSt *)mng)->parScl;
}

sceVu0FVECTOR *pppPartMngGetCol(void *mng) {
    return &((pppMngSt *)mng)->parCol;
}

void pppPartMngSetSkipFrame(void *mng, int nFrame) {
    ((pppMngSt *)mng)->skip_frame = nFrame;
}

int pppPartMngGetTotalFrame(void *mng) {
    return ((pppMngSt *)mng)->totalFrame / 2048;
}

float pppPartMngGetNowFrame(void *mng) {
    return ((pppMngSt *)mng)->time / 2048.0f;
}

BOOL pppPartMngIsEnd(void *mng) {
    return ((pppMngSt *)mng)->bEnd;
}

BOOL pppIsViewClip(void *mng) {
    return ((pppMngSt *)mng)->bViewClip;
}

int pppPartMngIsWaiting(void *mng) {
    return ((pppMngSt *)mng)->fWaitCt > 0.0f;
}

void pppSetPause(int pause) {
    ppvUserStopPartF = pause;
}

void pppInitPartRender(void *work_memory, int work_memory_size, void *packet_memory, int packet_memory_size,
                       int disp_width, int disp_height, int draw_width, int draw_height, int z_frame_base,
                       int draw_frame_base, int disp_frame_base, u_long gs_frame, u_long gs_zbuf, u_long gs_xyoffset,
                       u_long gs_scissor, u_long gs_test, u_long gs_dthe, u_long gs_color_clamp) {
    ppvDispWidth = disp_width;
    ppvDispHeight = disp_height;
    ppvDrawWidth = draw_width;
    ppvDrawHeight = draw_height;
    ppvZFrameBase = z_frame_base;
    ppvDrawFrameBase = draw_frame_base;
    ppvDispFrameBase = disp_frame_base;
    ppvGsFrame = gs_frame;
    ppvGsZbuf = gs_zbuf;
    ppvGsXyoffset = gs_xyoffset;
    ppvGsScissor = gs_scissor;
    ppvGsTest = gs_test;
    ppvGsDthe = gs_dthe;
    ppvGsColorClamp = gs_color_clamp;
    ((u_int *)&ppvGsFrame)[1] = 0xFF000000;
    initGraphicSystem();
    ppvUserStopPartF = 0;
    ppvWorkMemSize = work_memory_size;
    ppvPacketMemSize = packet_memory_size;
    pppCreateHeap(&g_Env.heapMng, work_memory, work_memory_size);
    g_Env.lpModel = RendModel; /* MATCH: the g_Env stores before ppvEnv's (decomp-permuter) */
    g_Env.lpShape = RendShape;
    g_Env.lpVsf = RendVsf;
    ppvEnv = &g_Env;
    ppvHookFuncTbl = NULL;
    sceVu0UnitMatrix(ppvUnitMatrix);
    *(u_long128 *)ppvZeroVector = 0;
    ppvZeroVector[3] = 1.0f;
    pppSetPrimPacket(packet_memory, packet_memory_size);
}

typedef struct {
    u_long128 m[4];
} MAT128;
#define MAT_COPY(d, s) (*(MAT128 *)(d) = *(MAT128 *)(s))

void pppSetPartMatrix(float *cm[4], float *sm[4], float *eye, float *at) {
    sceVu0CopyMatrix(ppvCameraMatrixOld, ppvCameraMatrix);
    MAT_COPY(ppvCameraMatrix, cm);
    MAT_COPY(ppvScreenMatrix, sm);
    VEC_COPY(ppvEyeOld, ppvEye);
    VEC_COPY(ppvAtOld, ppvAt);
    VEC_COPY(ppvEye, eye);
    VEC_COPY(ppvAt, at);
    ppvFov = 53.0f;
    sceVu0InversMatrix(ppvCameraMatrixInv, ppvCameraMatrix);
    sceVu0MulMatrix(ppvSCMatrix, ppvScreenMatrix, ppvCameraMatrix);
}

void pppSetFrameTime(float frame_time) {
    ppvFrameTime = frame_time;
}

void pppSetFogParam(float near, float far) {
    op_fog_param_set(near, far);
}

void pppSetFogColor(u_char r, u_char g, u_char b) {
    op_fog_color_set(r, g, b);
}

void pppSetFogClamp(u_char min, u_char max) {
    op_fog_clamp_set(min, max);
}

void pppSetReferencePos(void *list, int num) {
    ppvReferencePosList = list;
    ppvReferencePosNum = num;
}

void pppSetReferenceMatrix(void *list, int num) {
    ppvReferenceMatrixList = list;
    ppvReferenceMatrixNum = num;
}

void pppSetMonochrome(int b) {
    ppvMonochrome = b;
}

void pppRandInit() {
    float num = 165.86864f;
    register int xori asm("$8");

    __asm__ volatile("qmtc2.ni %1, $vf1\n"
                     "\tvrinit $R, $vf1x\n"
                     "\tqmtc2.ni %2, $vf2\n"
                     "\tvrxor $R, $vf2x\n"
                     "\tvrget.x $vf1, $R\n"
                     "\tqmfc2.ni %0, $vf1"
                     : "=&r"(xori)
                     : "r"(num), "r"(-1));
}
void *lmemcpy(void *dis, void *src, int ln) {
    word *dlp = dis;
    word *slp = src;

    while (ln--) {
        *dlp++ = *slp++;
    }
    return dlp;
}

void *l64memcpy(void *dis, void *src, int ln) {
    long *dlp = dis;
    long *slp = src;

    while (ln--) {
        *dlp++ = *slp++;
    }
    return dlp;
}

void *l128memcpy(void *dis, void *src, int ln) {
    long128 *dlp = dis;
    long128 *slp = src;

    while (ln--) {
        *dlp++ = *slp++;
    }
    return dlp;
}

void *lmemset(void *dis, word code, int ln) {
    word *dlp = dis;

    while (ln--) {
        *dlp++ = code;
    }
    return dlp;
}

void *l64memset(void *dis, long code, int ln) {
    long *dlp = dis;

    while (ln--) {
        *dlp++ = code;
    }
    return dlp;
}

void *l128memset(void *dis, long128 code, int ln) {
    long128 *dlp = dis;

    while (ln--) {
        *dlp++ = code;
    }
    return dlp;
}

void pppMemClear(void *dis, int bytes) {
    int ct;
    word *wp = dis;
    char *cp;

    ct = bytes >> 2;
    while (ct--) {
        *wp++ = 0;
    }
    cp = (char *)wp;
    ct = bytes & 3;
    while (ct--) {
        *cp++ = 0;
    }
}

void pppSetErrorFunc(void (*func)()) {
    ppvErrorFunc = func;
}
#define D_REG(a) (*(volatile u_int *)(a))
#define D1_CHCR 0x10009000
#define D1_QWC 0x10009020
#define D1_TADR 0x10009030
#define D2_CHCR 0x1000A000
#define D2_QWC 0x1000A020
#define D2_TADR 0x1000A030
#define D_STAT 0x1000E010
#define VIF1_STAT 0x10003C00

void pppWaitVif1() {
    int wait;
    int reg;

    do {
        wait = 0;
        if (D_REG(D1_CHCR) & 0x100) {
            wait |= 1;
        }
        if (D_REG(D2_CHCR) & 0x100) {
            wait |= 2;
        }
        if (D_REG(VIF1_STAT) & 0x1F000003) {
            wait |= 4;
        }
        __asm__ volatile("cfc2.ni %0, $vi29" : "=r"(reg));
        if (reg & 0x100) {
            wait |= 8;
        }
    } while (wait);
}

void pppSendVu1Prog(int vu1prog) {
    sceGsSyncPath(0, 0);
    if (ppvVu1Prog != vu1prog) {
        pppWaitVif1();
        D_REG(D1_QWC) = 0;
        D_REG(D1_TADR) = vu1prog;
        D_REG(D_STAT) = 2;
        D_REG(D1_CHCR) = 0x145;
        ppvVu1Prog = vu1prog;
    } else {
        pppWaitVif1();
    }
}

void pppWaitDmaGs() {
    do {
    } while (D_REG(D2_CHCR) & 0x100);
}

void pppSendGsPacket(u_long128 *pkt_top) {
    do {
    } while (D_REG(D2_CHCR) & 0x100);
    D_REG(D2_QWC) = 0;
    D_REG(D2_TADR) = (u_int)pkt_top | 0x80000000;
    D_REG(D_STAT) = 2;
    D_REG(D2_CHCR) = 0x145;
}

void pppSendVif1Packet(u_long128 *pkt_top) {
    do {
    } while (D_REG(VIF1_STAT) & 3);
    sceDmaSync((sceDmaChan *)D1_CHCR, 0, 0);
    sceDmaSend((sceDmaChan *)D1_CHCR, pkt_top);
}

void pppSendTagVif1Packet(u_long128 *pkt_top) {
    pppWaitVif1();
    D_REG(D1_QWC) = 0;
    D_REG(D1_TADR) = (u_int)pkt_top;
    D_REG(D_STAT) = 2;
    D_REG(D1_CHCR) = 0x145;
}
// SOURCE DATA: .data .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss .data .rodata
