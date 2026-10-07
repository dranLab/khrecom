#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDLSEMI3_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDLSEMI3_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IDrawMdlSemi3_Color = 0,
    IDrawMdlSemi3_DMat = 1,
    IDrawMdlSemi3_KeZCrct = 2
};

typedef struct { /* 0x10 */
    pppCDT cdt;
    int model;
    unsigned char acalc;
    unsigned char afix;
    unsigned char draw_mode;
    unsigned char fog;
    unsigned char scissor;
    unsigned char near;
    unsigned char zCrct;
    unsigned char ztest;
} PDrawMdlSemi3;

void pppDrawMdlSemi3(pppPObject *pobj, PDrawMdlSemi3 *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDLSEMI3_H */
