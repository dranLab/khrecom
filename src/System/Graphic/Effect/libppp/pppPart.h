// PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppDrawMdlSea
#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_PPPPART_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_PPPPART_H

#include "khtypes.h"

/* Core particle (ppp) types shared by the libppp units (C). */

typedef int word;
typedef unsigned int u_word; /* PARTIAL: added for System/Graphic/Effect/libppp/pppAlloc */
/* 16-aligned as in the SDK's libvu0.h. This makes pppMngSt 8-aligned, so gcc merges byte tests like
   "ppvMng->bFlagStop == 0 && ppvMng->bBoolSlow == 0" into one ld + mask, as the original does. */
typedef float sceVu0FVECTOR[4] __attribute__((aligned(16)));
typedef float sceVu0FMATRIX[4][4] __attribute__((aligned(16)));

typedef struct {
    short x;
    short y;
    short z;
} pppSVECTOR;

typedef struct { /* 0x28 */
    char *progName;
    void *calcFunc;
    void *deltFunc;
    void *drawFunc;
    void *iniFunc;
    void *staFunc;
    void *endFunc;
    void *conFunc;
    void *con2Func;
    void *desFunc;
} pppProg;

typedef struct _pppCtrlData { /* 0x4 */
    word time;
    char dt[0];
} pppCtrlData;

typedef struct _pppCtrlTable { /* 0x10 */
    pppProg *prog;
    u_short dtbytes;
    u_short ctrldtn;
    pppCtrlData *ctrldt;
    word *useVal;
} pppCtrlTable;

typedef struct _pppPData { /* 0x28 */
    struct _pppPData *next;
    long *cvoff;
    struct {
        word dummy : 1;
    } flag;
    struct {
        word notdrawf : 1;
        word drawf : 1;
    } status;
    word startFrameInit;
    word totalFrame;
    word loopFrame0;
    word loopFrame1;
    word valbytes;
    short prio;
    short ctrlTablen;
    pppCtrlTable ctrlTable[0];
} pppPData;

typedef struct _pppPDataVal { /* 0x10 */
    pppPData *pdt;
    struct _pppPObject *pobj;
    word startFrame;
    u_short pobjn;
    u_char pdtno;
    u_char cdummy[1];
} pppPDataVal;

typedef struct _pppPObject { /* 0xa0 */
    struct _pppPObject *next;
    struct _pppPObject *ppobj;
    pppPDataVal *pdtval;
    word time;
    sceVu0FMATRIX cmat;
    sceVu0FMATRIX dmat;
    pppSVECTOR *lpvert;
    pppSVECTOR *lpnorm;
    pppCtrlData **lpcdt;
    u_char appearf;
    u_char loop_in;
    u_char loop_start0;
    u_char dummy[1];
    char val[0];
} pppPObject;

// PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppDrawMatrix
extern pppPObject *ppvPObj;
extern sceVu0FMATRIX ppvSWMatrix;
void sceVu0MulMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1, sceVu0FMATRIX m2);

/* PARTIAL: added for System/Graphic/Effect/libppp/ryjProg/pppRyjMngFlag */

typedef struct { /* 0x4 */
    word time;
} pppCDT;

typedef struct _pppMngSt { /* 0xd0 */
    union {
        sceVu0FMATRIX parMat;
        struct {
            sceVu0FVECTOR parRot[3];
            sceVu0FVECTOR parLoc;
        };
    };
    sceVu0FVECTOR parScl;
    sceVu0FVECTOR parCol;
    sceVu0FVECTOR parChrScl;
    word startFrame;
    word totalFrame;
    word time;
    word otime;
    word skip_frame;
    word progn;
    word pdtn;
    word newtime;
    pppPData *m_pdata;
    pppPObject *m_pobj;
    pppProg **m_prog;
    pppPData **m_lppdt;
    pppPDataVal *m_pdtval;
    u_char partLoopf;
    u_char partLoopEndf;
    u_short usef;
    char *pchDpd;
    struct _pppPEtcData *pEtc;
    u_char bViewClip;
    u_char bTexVramBlock;
    u_char bClutVramBlock;
    u_char bDpdId;
    float fFrameTimeAccum;
    short shTexpStart;
    short shClutpStart;
    float fWaitCt;
    u_char bDrawPriority;
    u_char bFlagStop;
    u_char bBoolFollowCam;
    u_char bBoolNoViewClip;
    u_char bBoolSlow;
    u_char bEnd;
    u_char Pad[2];
} pppMngSt;

extern pppMngSt *ppvMng;
/* end PARTIAL pppRyjMngFlag */


typedef struct { /* 0xc */
    float x;
    float y;
    float z;
} pppFVECTOR;
/* end PARTIAL pppKeDrct */

/* PARTIAL: added for System/Graphic/Effect/libppp/keProg/pppKeDMat */
extern sceVu0FMATRIX ppvWorldMatrix;
extern sceVu0FMATRIX ppvScreenMatrix;
void sceVu0CopyMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1);
/* end PARTIAL pppKeDMat */


typedef struct { /* 0xc */
    word x;
    word y;
    word z;
} pppIVECTOR;
/* end PARTIAL pppGetRotMatrixXYZ */

/* PARTIAL: added for System/Graphic/Effect/libppp/keProg/pppKeDMatPht */
extern sceVu0FVECTOR ppvParScl;
extern sceVu0FVECTOR ppvParLoc;
void sceVu0ScaleVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, float t);
void sceVu0ApplyMatrix(sceVu0FVECTOR v0, sceVu0FMATRIX m0, sceVu0FVECTOR v1);
void sceVu0UnitMatrix(sceVu0FMATRIX m);
void sceVu0MulVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);
extern sceVu0FMATRIX *ppvlpCMatrix;
/* end PARTIAL pppKeDMatPht */


typedef union { /* 0x4 */
    struct {
        u_char r;
        u_char g;
        u_char b;
        u_char a;
    };
} pppCVECTOR;

extern sceVu0FVECTOR *ppvReferencePosList;
extern int ppvReferencePosNum;
/* end PARTIAL pppRyjMegaBirthUserCtrl */

/* PARTIAL: added for System/Graphic/Effect/libppp/ryjProg/pppRyjMegaBirthUserCtrl2 */
extern sceVu0FMATRIX *ppvReferenceMatrixList;
extern int ppvReferenceMatrixNum;
/* end PARTIAL pppRyjMegaBirthUserCtrl2 */


typedef void (*PPPUSRFUNC)();
extern PPPUSRFUNC *ppvHookFuncTbl;



typedef struct { /* 0x8 */
    word pdtn;
    word valoff;
} pppParamPObj;

void sceVu0Normalize(sceVu0FVECTOR v0, sceVu0FVECTOR v1);
void sceVu0CopyVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1);
/* end PARTIAL pppKeParMatR */


extern sceVu0FMATRIX ppvCameraMatrix;
extern sceVu0FMATRIX ppvParMatrix;



extern BOOL ppvUserStopPartF;


/* PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppMatrixXYZ */
/* v.xyz *= s (VU0 macro mode) */
#define SCALE_VECTOR_XYZ(v, s)                  \
    __asm__ __volatile__("qmtc2.ni %1, $vf9\n"  \
                         "\tlqc2 $vf10, 0(%0)\n" \
                         "\tvmulx.xyz $vf10, $vf10, $vf9x\n" \
                         "\tsqc2 $vf10, 0(%0)"   \
                         : : "r"(v), "r"(s) : "memory")
/* end PARTIAL pppMatrixXYZ */


typedef union { /* 0x8 */
    struct {
        short r;
        short g;
        short b;
        short a;
    };
} pppHCVECTOR;
/* end PARTIAL pppColMove */

/* PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppMoveLoop */
extern int ppvEmptyLoop;
extern int ppvMonochrome;
pppPObject *pppCreatePObject(pppMngSt *mng, pppPDataVal *pdv0);
void pppMemClear(void *dis, int bytes);
/* end PARTIAL pppMoveLoop */

/* PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppRandFloat */
extern long128 ppvDbgTemp[1];

/* r = random float in [0, 1) from the VU0 random unit (R register): xor R with -1, five rounds of
   vrxor/vrnext, then add -1.0 (vrnext yields [1, 2)). r must be a float lvalue. */
#define PPP_RAND_F(r)                                    \
    __asm__ __volatile__("addi $8, $0, -1\n"             \
                         "\tqmtc2.ni $8, $vf2\n"         \
                         "\tvrxor $R, $vf2x\n"           \
                         "\tvrnext.x $vf1, $R\n"         \
                         "\tvrxor $R, $vf2x\n"           \
                         "\tvrnext.x $vf1, $R\n"         \
                         "\tvrxor $R, $vf2x\n"           \
                         "\tvrnext.x $vf1, $R\n"         \
                         "\tvrxor $R, $vf2x\n"           \
                         "\tvrnext.x $vf1, $R\n"         \
                         "\tvrxor $R, $vf2x\n"           \
                         "\tvrnext.x $vf1, $R\n"         \
                         "\tvitof0.x $vf2, $vf2\n"       \
                         "\tvadd.x $vf1, $vf1, $vf2\n"   \
                         "\tqmfc2.ni %0, $vf1"           \
                         : "=r"(r) : : "$8")
/* end PARTIAL pppRandFloat */

/* PARTIAL: added for System/Graphic/Effect/libppp/ryjProg/pppRyjMatrixWorldXYZ */
extern sceVu0FVECTOR ppvParAng;
void sceVu0InversMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1);
/* end PARTIAL pppRyjMatrixWorldXYZ */

/* PARTIAL: added for System/Graphic/Effect/libppp/pppAlloc */
typedef struct _pppEMCB { /* 0x10 */
    struct _pppEMCB *next;
    struct _pppEMCB *before;
    int owner;
    int wdummy;
} pppEMCB;

extern void (*ppvErrorFunc)();
/* end PARTIAL pppAlloc */


typedef struct { /* 0x10 */
    struct _pppEMCB *last_free_heap;
    struct _pppEMCB *effect_heap_top;
    struct _pppEMCB *effect_heap_bottom;
    int allocSize;
} pppMemMng;

typedef struct { /* 0x24 */
    word nvert;
    word nnorm;
    word npoly;
    word tex0l;
    word tex0h;
    pppSVECTOR *vert;
    pppSVECTOR *norm;
    struct _OMD_H *omd;
    word nPacketSize;
} pppModelSt;

typedef struct _pppVsfSt { /* 0x8 */
    short modelno;
    short indexn;
    u_short *index;
} pppVsfSt;

typedef struct { /* 0x1c */
    pppMemMng heapMng;
    pppModelSt *lpModel;
    struct _pppShapeSt *lpShape;
    struct _pppVsfSt *lpVsf;
} pppEnvSt;

extern pppEnvSt *ppvEnv;
extern u_long128 *ppvCurPrimp;
extern int ppvCurPrimpLen; /* PARTIAL: added for System/Graphic/Effect/libppp/draw/md */
extern int ppvPrimpLen;    /* PARTIAL: added for System/Graphic/Effect/libppp/draw/md */

/* libgraph (GIF tags and GS registers as plain 64-bit words) */
typedef struct { /* 0x60 */
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

int sceGsSetDefLoadImage(sceGsLoadImage *lp, short dbp, short dbw, short dpsm, short x, short y, short w,
                         short h);
void op_ot_add_before(u_long128 *packet, int qsize, int otz, int pri);
void op_ot_add(u_long128 *packet, int qsize, int otz, int pri); /* PARTIAL: added for System/Graphic/Effect/libppp/keProg/pppKeShpDtt */


typedef struct {
    u_long SCAX0 : 11;
    u_long pad11 : 5;
    u_long SCAX1 : 11;
    u_long pad27 : 5;
    u_long SCAY0 : 11;
    u_long pad43 : 5;
    u_long SCAY1 : 11;
    u_long pad59 : 5;
} sceGsScissor;

typedef struct {
    u_long OFX : 16;
    u_long pad16 : 16;
    u_long OFY : 16;
    u_long pad48 : 16;
} sceGsXyoffset;

typedef struct {
    u_long ATE : 1;
    u_long ATST : 3;
    u_long AREF : 8;
    u_long AFAIL : 2;
    u_long DATE : 1;
    u_long DATM : 1;
    u_long ZTE : 1;
    u_long ZTST : 2;
    u_long pad19 : 45;
} sceGsTest;
/* end PARTIAL KeDttSpr */

/* PARTIAL: added for System/Graphic/Effect/libppp/keProg/pppKeTkFade */
void *op_spr_alloc(word mem);
void op_spr_free(word mem);
int sceGsExecLoadImage(sceGsLoadImage *lp, u_long128 *srcaddr);
void FlushCache(int operation);
void sceVu0DivVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, float q);
void sceVu0AddVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);
/* end PARTIAL pppKeTkFade */

/* PARTIAL: added for System/Graphic/Effect/libppp/keProg/pppKeShpDtt (partMng.h) */
extern int ppvDispWidth;
extern int ppvDispHeight;
extern int ppvDrawWidth;
extern int ppvDrawHeight;
extern int ppvDrawFrameBase;
extern int ppvDispFrameBase;
extern u_long ppvGsScissor;
/* end PARTIAL pppKeShpDtt */
/* end PARTIAL pppKeMdlTfd */


extern sceVu0FMATRIX ppvLightMatrix;
extern sceVu0FMATRIX ppvColorMatrix;
extern long128 ppvVNWork[2048];


/* PARTIAL: added for System/Graphic/Effect/libppp/ryjProg/pppRyjDrawMatrixWorldWood */
extern sceVu0FMATRIX ppvWorldMatrixWood;
extern sceVu0FMATRIX ppvSWMatrixWood;
void sceVu0OuterProduct(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);
/* end PARTIAL pppRyjDrawMatrixWorldWood */

/* PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppDrawShape */
typedef struct _pppShapeSt { /* 0x8 */
    long *shape;
    word nPacketSize;
} pppShapeSt;

typedef struct tagOANM_SEQ { /* 0x8 */
    short sha;
    short wait;
    char flag;
    char res[3];
} OANM_SEQ;

typedef struct tagOANM_HEADER { /* 0x18 */
    char id;
    char ver;
    short flag;
    short shc;
    short seqc;
    short x;
    short y;
    short res;
    u_short size;
    OANM_SEQ seq[1];
} OANM_HEADER;

void debugDpdCheckShapeData(int nShapeNum); /* PARTIAL: added for System/Graphic/Effect/libppp/keProg/pppDrawShapeX (partMng.h) */
/* end PARTIAL pppDrawShape */

/* PARTIAL: added for System/Graphic/Effect/libppp/pppPart */
typedef int sceVu0IVECTOR[4] __attribute__((aligned(16)));

typedef struct _pppPEtcData { /* 0x110 */
    float afBbox[32];
    sceVu0FVECTOR vecOffset;
    sceVu0IVECTOR ivecAngle;
    sceVu0FVECTOR vecScale;
    u_char abUseTexture[16];
    u_char abUseShape[16];
    u_char abUseRsd[16];
    u_char abUseVsf[16];
    int nWorkMemSize;
    int nPacketMemSize;
    u_char bViewClipStop;
    u_char bChrScale;
    u_char bBindPosOnly;
    u_char bNonAmbient;
    u_short ushSeNo;
    u_short ushSeVolAdd;
    float fSeWait;
    int nBoneNo;
    float fWait;
    int nRes[1];
} pppPEtcData;

typedef void (*PPPCALC)();
typedef void (*PPPDELT)();
typedef void (*PPPDRAW)();
typedef void (*PPPINI)();
typedef void (*PPPSTA)();
typedef void (*PPPEND)();
typedef void (*PPPCON)();
typedef void (*PPPCON2)();
typedef void (*PPPDES)();

extern sceVu0FVECTOR ppvEye;
extern BOOL ppvSysStopPartF;
float sceVu0InnerProduct(sceVu0FVECTOR v0, sceVu0FVECTOR v1);
BOOL pppStartPart(pppMngSt *mng, long *lpdt, BOOL callInitFuncf);
void pppInitPdt(long *lpdt, pppProg *progtbl);
void pppMovePdt(long *lpdt, int offset);
void pppDeletePObject(pppPObject *pobj);
void pppAllFreePObject(pppMngSt *mng);
void pppNormalize(float *v0, float *v1);
void pppSetMatrix(pppMngSt *mng);
BOOL pppRunPartExec(pppMngSt *mng);
BOOL pppRunPart(pppMngSt *mng, BOOL drawf);
BOOL pppRunPartImpotent(pppMngSt *mng);
/* end PARTIAL pppPart */

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_PPPPART_H */
