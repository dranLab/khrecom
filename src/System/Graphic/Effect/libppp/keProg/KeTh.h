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

typedef u_char KeThCp_Stt;

typedef struct _KeThCp { /* 0x10 */
    KeListHold UdtList;
    KeThCp_Stt Stt;
    char _P0[11];
} KeThCp;

typedef struct { /* 0x90 */
    KeThCp Cp;
    u_char _rest[0x80]; /* KetThBp_Pr Pr, Res, Tp, MaxLen, Spd, colors */
} VKeTh;

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KETH_H */
