// PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppDrawMdlSea
#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_PPPPART_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_PPPPART_H

#include "khtypes.h"

/* Core particle (ppp) types shared by the libppp units (C). */

typedef int word;
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
        } s;
    } u;
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

/* PARTIAL: added for System/Graphic/Effect/libppp/keProg/pppKeDrct */
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

/* PARTIAL: added for System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixXYZ */
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

/* PARTIAL: added for System/Graphic/Effect/libppp/ryjProg/pppRyjMegaBirthUserCtrl */
typedef struct { /* 0x4 */
    u_char r;
    u_char g;
    u_char b;
    u_char a;
} pppCVECTOR;

extern sceVu0FVECTOR *ppvReferencePosList;
extern int ppvReferencePosNum;
/* end PARTIAL pppRyjMegaBirthUserCtrl */

/* PARTIAL: added for System/Graphic/Effect/libppp/ryjProg/pppRyjMegaBirthUserCtrl2 */
extern sceVu0FMATRIX *ppvReferenceMatrixList;
extern int ppvReferenceMatrixNum;
/* end PARTIAL pppRyjMegaBirthUserCtrl2 */

/* PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppDrawHook */
typedef void (*PPPUSRFUNC)();
extern PPPUSRFUNC *ppvHookFuncTbl;
/* end PARTIAL pppDrawHook */

/* PARTIAL: added for System/Graphic/Effect/libppp/keProg/pppKeParMatR */
typedef struct { /* 0x8 */
    word pdtn;
    word valoff;
} pppParamPObj;

void sceVu0Normalize(sceVu0FVECTOR v0, sceVu0FVECTOR v1);
void sceVu0CopyVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1);
/* end PARTIAL pppKeParMatR */

/* PARTIAL: added for System/Graphic/Effect/libppp/ryjProg/pppRyjDrawMatrixWorld */
extern sceVu0FMATRIX ppvCameraMatrix;
extern sceVu0FMATRIX ppvParMatrix;
/* end PARTIAL pppRyjDrawMatrixWorld */

/* PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppAngle */
extern BOOL ppvUserStopPartF;
/* end PARTIAL pppAngle */

/* PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppMatrixXYZ */
/* v.xyz *= s (VU0 macro mode) */
#define SCALE_VECTOR_XYZ(v, s)                  \
    __asm__ __volatile__("qmtc2.ni %1, $vf9\n"  \
                         "\tlqc2 $vf10, 0(%0)\n" \
                         "\tvmulx.xyz $vf10, $vf10, $vf9x\n" \
                         "\tsqc2 $vf10, 0(%0)"   \
                         : : "r"(v), "r"(s) : "memory")
/* end PARTIAL pppMatrixXYZ */

/* PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppColMove */
typedef struct { /* 0x8 */
    short r;
    short g;
    short b;
    short a;
} pppHCVECTOR;
/* end PARTIAL pppColMove */

/* PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppMoveLoop */
extern int ppvEmptyLoop;
extern int ppvMonochrome;
pppPObject *pppCreatePObject(pppMngSt *mng, pppPDataVal *pdv0);
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

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_PPPPART_H */
