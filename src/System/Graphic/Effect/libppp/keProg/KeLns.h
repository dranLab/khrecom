// PARTIAL: added for System/Graphic/Effect/libppp/keProg/pppKeLnsLpSft
#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KELNS_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KELNS_H

#include "System/Graphic/Effect/libppp/pppPart.h"

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

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KELNS_H */
