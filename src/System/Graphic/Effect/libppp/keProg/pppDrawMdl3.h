#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDL3_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDL3_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IDrawMdl3_Color = 0,
    IDrawMdl3_DMat = 1,
    IDrawMdl3_KeZCrct = 2
};

typedef struct { /* 0x10 */
    pppCDT cdt;
    int model;
    unsigned char draw_mode;
    unsigned char fog;
    unsigned char scissor;
    unsigned char near;
    unsigned char zCrct;
    unsigned char invisible;
    unsigned char ztest;
} PDrawMdl3;

void pppDrawMdl3(pppPObject *pobj, PDrawMdl3 *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDL3_H */
