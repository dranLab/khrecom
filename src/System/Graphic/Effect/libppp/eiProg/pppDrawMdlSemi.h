#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMDLSEMI_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMDLSEMI_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0xc */
    pppCDT cdt;
    int model;
    unsigned char acalc;
    unsigned char afix;
} PDrawMdlSemi;

void pppDrawMdlSemi(pppPObject *pobj, PDrawMdlSemi *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMDLSEMI_H */
