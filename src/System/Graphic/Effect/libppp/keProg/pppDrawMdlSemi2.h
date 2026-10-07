#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDLSEMI2_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDLSEMI2_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0xC */
    pppCDT cdt;
    int model;
    unsigned char acalc;
    unsigned char afix;
    unsigned char draw_mode;
    unsigned char ztest;
} PDrawMdlSemi2;

void pppDrawMdlSemi2(pppPObject *pobj, PDrawMdlSemi2 *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDLSEMI2_H */
