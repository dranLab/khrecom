#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDL2_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDL2_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0xC */
    pppCDT cdt;
    int model;
    unsigned char draw_mode;
    unsigned char invisible;
    unsigned char ztest;
} PDrawMdl2;

void pppDrawMdl2(pppPObject *pobj, PDrawMdl2 *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDL2_H */
