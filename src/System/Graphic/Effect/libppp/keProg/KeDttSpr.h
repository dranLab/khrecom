
#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KEDTTSPR_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KEDTTSPR_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x60 */
    sceVu0FVECTOR Pos;
    sceVu0FVECTOR Col;
    sceVu0FMATRIX *VSMat;
    u_long *Pk;
    sceVu0FMATRIX *LSMat;
    int Z;
    float Dtt[3];
    int _Pad0[1];
    u_long TEX0;
    u_long SCISSOR;
    u_long XYOFFSET;
    float Sz[2];
} KeaDttSpr_MkGifPk;

void KeDttSpr_MkGifPk_Pre(KeaDttSpr_MkGifPk *a);
int KeDttSpr_MkGifPk(KeaDttSpr_MkGifPk *a);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KEDTTSPR_H */
