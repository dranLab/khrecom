// PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppDrawMdlSea
#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_PPPPART_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_PPPPART_H

#include "khtypes.h"

/* Core particle (ppp) types shared by the libppp units (C). */

typedef int word;
typedef float sceVu0FVECTOR[4];
typedef float sceVu0FMATRIX[4][4];

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

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_PPPPART_H */
