#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEZCRCTSHP_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEZCRCTSHP_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x20 */
    pppCDT cdt;
    pppFVECTOR Pos;
    pppFVECTOR Scl;
    u_char PosMd;
} PKeZCrctShp;

typedef struct { /* 0x40 */
    sceVu0FMATRIX Mat;
} VKeZCrctShp;

void pppKeZCrctShpDraw(pppPObject *pobj, PKeZCrctShp *dat, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEZCRCTSHP_H */
