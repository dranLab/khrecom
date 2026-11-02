#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_SHAPE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_SHAPE_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct tagOSPRITE_VU1 { /* 0x10 */
    short x0;
    short y0;
    short x3;
    short y3;
    short s;
    short t;
    short w;
    short h;
} OSPRITE_VU1;

typedef struct tagOSPRITE_VU1A { /* 0x8 */
    u_char r;
    u_char g;
    u_char b;
    u_char a;
    u_char alpha;
    u_char alphafix;
    u_char alphacode;
    u_char pal;
} OSPRITE_VU1A;

typedef struct tagOSHAPE_VU1 { /* 0x30 */
    short flag;
    short counter;
    word spvu1a;
    u_char timposx;
    char res[7];
    word tex0_l;
    word tex0_h;
    word res2[2];
    OSPRITE_VU1 spvu1[1];
} OSHAPE_VU1;

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


typedef struct { /* 0x70 */
    word flag;
    word dummy0;
    float addz;
    float dummy1;
    word r;
    word g;
    word b;
    word a;
    word tex0_l;
    word tex0_h;
    word shape_flag;
    word counter;
    sceVu0FMATRIX mat;
} OSHAPE_IN_VU1;

void pppInitVu1ShapeProg(void);
void pppStartVu1ShapeProg(int fog);
void pppVu1ShapeDraw(OSHAPE_VU1 *sh, OSHAPE_IN_VU1 *vu1shi);
void *pppGetVu1DrawSpad0(void);
void *pppGetVu1DrawSpad1(void);
void *pppGetVu1DrawSpad(void);
void op_ztest_init(void);
void pppWaitVif1(void);
/* end PARTIAL pppRyjMegaBirthPrize */

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_SHAPE_H */
