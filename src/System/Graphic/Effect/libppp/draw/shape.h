// PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppDrawShape
#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_SHAPE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_SHAPE_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct tagOSHAPE_VU1 OSHAPE_VU1;

typedef struct { /* 0x30 */
    u_short id;
    u_short flag;
    u_char r;
    u_char g;
    u_char b;
    u_char a;
    u_short toff;
    u_short coff;
    short ot;
    short z;
    sceVu0FMATRIX *pmat;
    void *packet;
    u_char pri;
    char res[3];
    sceVu0FMATRIX *pzmat;
    u_char fr;
    u_char fg;
    u_char fb;
    u_char fa;
    float ffar;
    float fnear;
    float flimit;
} OSHAPE_IN;

void *op_shape_draw(OSHAPE_VU1 *sh, OSHAPE_IN *shi, int nPacketSize);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_SHAPE_H */
