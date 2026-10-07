// PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppDrawMdl
#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_MD_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_MD_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct _OMD_H { /* 0x20 */
    word flag;
    word pa;
    word va;
    word na;
    short pc;
    short vc;
    short nc;
    u_char alpha;
    u_char alphafix;
    short res[2];
    word vna;
} OMD_H;

typedef struct { /* 0x60 */
    word flag;
    char *tmp_a;
    u_char alpha;
    u_char alphafix;
    u_char pri;
    char res;
    u_short toff;
    u_short coff;
    word u;
    word v;
    u_char r;
    u_char g;
    u_char b;
    u_char a;
    char ar;
    char ag;
    char ab;
    char aa;
    sceVu0FMATRIX *pmat;
    sceVu0FMATRIX *lcmat;
    sceVu0FMATRIX *lvmat;
    sceVu0FMATRIX *lwmat;
    sceVu0FMATRIX *lviewmat;
    void *packet;
    void *va;
    void *na;
    short ot;
    short z;
    sceVu0FMATRIX *pzmat;
    void *packet_end;
    char invisible;
    char ztest_off;
    char res2[2];
    u_char fr;
    u_char fg;
    u_char fb;
    u_char fa;
    float ffar;
    float fnear;
    float flimit;
} OMD_IN;

extern int ot_omd_size[0];

void *op1_md_draw(OMD_H *mdh, OMD_IN *mdi, int nPacketSize);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_MD_H */
