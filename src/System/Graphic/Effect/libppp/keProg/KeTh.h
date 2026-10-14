/* PARTIAL: added for System/Graphic/Effect/libppp/keProg/pppKeThHitBorn (KeTh thread types, as far as used) */
#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KETH_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KETH_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct _KeListTag { /* 0x8 */
    void *NextItem;
    struct _KeListTag *PrevTag;
} KeListTag;

typedef struct _KeListHold { /* 0x4 */
    void *FirstItem;
} KeListHold;

typedef struct _KeThTP { /* 0x20 */
    sceVu0FVECTOR RPos;
    float ValidR;
    float Pow;
    float HitR;
    struct _KeThTP *Nxt;
} KeThTP;

typedef struct _KeThRP { /* 0x30 */
    sceVu0FVECTOR RPos;
    float _P1[2];
    KeListHold GrpList;
    float Scl;
    sceVu0FVECTOR InVec;
} KeThRP;

enum {
    KeThBP_FlgClr_ = 0,
    KeThBP_FlgStepStop = 2,
    KeThBP_FlgStepLimit = 4,
    KeThBP_FlgParentDel = 8,
    KeThBP_FlgReqDel = 16,
    KeThBP_FlgRootStep = 32,
    KeThBP_SttHitted = 64,
    KeThBP_SttNonStop = 128,
    KeThBP_FlgNoParent = 256,
    KeThBP_SttCpSync = 512,
    KeThBP_SttSpdFix = 1024,
    KeThBP_SttHit = 2048,
    KeThBP_SttRvrs = 4096
};

typedef u_short KetThBp_Stt;

typedef struct _KeThBP { /* 0x70 */
    KeListTag UdtTag;
    KeListTag GrpTag;
    struct _KeThBP *This;
    struct _KeThTP *TpAdr;
    float Step;
    u_short RootStep;
    u_char LowNum;
    char _P0[1];
    u_long128 Send; /* KeData16 */
    float OrgStep;
    u_short RootIdx;
    u_char LastIdx;
    u_char RPNum;
    u_short FromRoot;
    u_char _P1[2];
    u_short FromZero;
    KetThBp_Stt Flg;
    KeThRP RpTbl[1];
} KeThBP;

typedef struct _KeAlowHold { /* 0x4 */
    void *FirstItem;
} KeAlowHold;

typedef struct _KeThResHd { /* 0x80 */
    u_char BPNum;
    u_char RPNum;
    u_short Sz;
    KeAlowHold FreeAlow;
    char _Pad0[8];
    KeThBP BP[1];
} KeThResHd;

enum {
    KeThCp_SttClr_ = 0,
    KeThCp_FlgRootStep = 2,
    KeThCp_SttNonStop = 4,
    KeThCp_SttSpdFix = 8,
    KeThCp_SttCpSync = 16,
    KeThCp_SttRvrs = 32
};

typedef u_char KeThCp_Stt;

typedef struct _KeThCp { /* 0x10 */
    KeListHold UdtList;
    KeThCp_Stt Stt;
    char _P0[11];
} KeThCp;

typedef struct _KetThBp_Pr { /* 0x50 */
    u_char ColCe[4];
    u_char ColLt[4];
    float _P1;
    float Wid;
    signed char AddColCe[4];
    signed char AddColLt[4];
    float _P2;
    float AddWid;
    float Spd1st;
    float Wild;
    float Curv;
    float Corner;
    float CePstg;
    float StepSpd;
    float MvWidSz;
    float MvWidMv;
    u_char LowMax;
    u_char Pstg;
    u_char MaxStep;
    char _P0[13];
} KetThBp_Pr;

typedef struct { /* 0x90 */
    KeThCp Cp;
    KetThBp_Pr Pr;
    struct _KeThResHd *Res;
    struct _KeThTP *Tp;
    float MaxLen;
    float Spd;
    pppHCVECTOR ColCe;
    pppHCVECTOR ColEg;
    pppHCVECTOR AddColCe;
    pppHCVECTOR AddColEg;
} VKeTh;

typedef struct _KeaThCp_Update { /* 0x18 */
    KeThCp *This;
    KeThTP *TP;
    KetThBp_Pr *Pr;
    KeThResHd *Res;
    sceVu0FVECTOR *RPos;
    sceVu0FVECTOR *Vec;
} KeaThCp_Update;

/* PARTIAL: added for System/Graphic/Effect/libppp/keProg/pppKeTh */
void KeThResHd_Init(KeThResHd *_t, u_char bpNum, u_char rpNum);
void KeThCp_AddBp(KeThCp *_t, KeThResHd *pResHd, sceVu0FVECTOR *pRPos, sceVu0FVECTOR *pVec);
void KeThCp_Update(KeaThCp_Update *_a);
void KeThCp_Draw(KeThCp *_t, sceVu0FMATRIX *pMat, float *scl, KetThBp_Pr *pPr, u_long128 **pPkBuf, u_long Alpha, u_long Test);
void KeThCp_Init(KeThCp *_t);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KETH_H */
