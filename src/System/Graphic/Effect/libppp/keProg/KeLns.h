#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KELNS_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KELNS_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/draw/shape.h"

typedef struct _KeLnsFlr { /* 0x4 */
    short Cnt;
    u_char SeqIdx;
    char _P0[1];
} KeLnsFlr;

typedef struct _KeWkLnsShp_Draw { /* 0x40 */
    pppShapeSt *Shp;
    KeLnsFlr *Flr;
    OSHAPE_IN In __attribute__((aligned(16))); /* at 0x10 in the original */
} KeWkLnsShp_Draw;

typedef struct _KeLnsLp { /* 0xb0 */
    sceVu0FMATRIX Mat;
    sceVu0FVECTOR ZMat[3];
    float Vec[2];
    float Pers;
    float VPow;
    float LcPos[3];
    u_char Idx;
    u_char Mbk;
    u_char R;
    u_char Md;
    sceVu0FVECTOR LVec;
    float _P2[1];
    float Len2;
    float Pow;
    float PreHidePow;
} KeLnsLp;

typedef struct _KeWkLnsLp_Update { /* 0x10 */
    KeLnsLp *This;
    sceVu0FVECTOR *RPos;
    sceVu0FMATRIX *WvMat;
    sceVu0FMATRIX *VsMat;
} KeWkLnsLp_Update;

typedef struct _KeLnsArnd { /* 0xc */
    KeLnsLp *Lp;
    pppShapeSt *Shp;
    KeLnsFlr Flr;
} KeLnsArnd;

typedef struct _KeWkLnsArnd_Draw { /* 0x14 */
    KeLnsArnd *This;
    u_char Col[4];
    sceVu0FMATRIX *VsMat;
    float Scl;
    sceVu0FVECTOR *SclVec;
} KeWkLnsArnd_Draw;

typedef struct _KeLnsClmFlr { /* 0x8 */
    KeLnsFlr Flr;
    float RdPr;
} KeLnsClmFlr;

typedef struct _KeLnsClm { /* 0x18 */
    KeLnsLp *Lp;
    pppShapeSt *Shp;
    u_char Num;
    u_char Flg;
    short Betw;
    short StOfs;
    u_char FlgType;
    char _P0[1];
    KeLnsClmFlr Flr[1];
} KeLnsClm;

typedef struct _KeWkLnsClm_Draw { /* 0x18 */
    KeLnsClm *This;
    u_char Col[4];
    sceVu0FMATRIX *VsMat;
    float Scl;
    float R;
    sceVu0FVECTOR *SclVec;
} KeWkLnsClm_Draw;

typedef struct _KeLnsCrn { /* 0xc */
    KeLnsLp *Lp;
    pppShapeSt *Shp;
    KeLnsFlr Flr;
} KeLnsCrn;

typedef struct _KeWkLnsCrn_Draw { /* 0x18 */
    KeLnsCrn *This;
    u_char Col[4];
    sceVu0FMATRIX *VsMat;
    float Scl;
    u_short PLen;
    u_short DLen;
    sceVu0FVECTOR *SclVec;
} KeWkLnsCrn_Draw;

typedef struct _KeLnsFls { /* 0xc */
    KeLnsLp *Lp;
    pppShapeSt *Shp;
    KeLnsFlr Flr;
} KeLnsFls;

typedef struct _KeWkLnsFls_Draw { /* 0x18 */
    KeLnsFls *This;
    u_char Col[4];
    sceVu0FMATRIX *VsMat;
    float Scl;
    u_short PLen;
    u_char Typ[2];
    sceVu0FVECTOR *SclVec;
} KeWkLnsFls_Draw;

void KeLnsFlr_Init(KeLnsFlr *_p, pppShapeSt *pSS);
void KeLnsShp_Update(KeWkLnsShp_Draw *_w);
void KeLnsShp_Draw(KeWkLnsShp_Draw *_w);
void KeLnsLp_Update(KeWkLnsLp_Update *_w);
void KeLnsLp_Init(KeLnsLp *_p);
void KeLnsClm_Draw(KeWkLnsClm_Draw *_w, int Flg);
void KeLnsClm_SetShp(KeLnsClm *_p, pppShapeSt *pSS, int seed);
void KeLnsClm_Init(KeLnsClm *_p);
void KeLnsArnd_Draw(KeWkLnsArnd_Draw *_w, int Flg);
void KeLnsArnd_Init(KeLnsArnd *_p);
void KeLnsCrn_Draw(KeWkLnsCrn_Draw *_w, int Flg);
void KeLnsCrn_SetShp(KeLnsCrn *_p, pppShapeSt *pSS);
void KeLnsCrn_Init(KeLnsCrn *_p);
void KeLnsFls_Draw(KeWkLnsFls_Draw *_w, int Flg);
void KeLnsFls_Init(KeLnsFls *_p);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KELNS_H */
