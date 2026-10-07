#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEZCRCT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEZCRCT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IKeZCrct_This = 0
};

typedef struct { /* 0x20 */
    pppCDT cdt;
    pppFVECTOR Pos;
    pppFVECTOR Scl;
    u_char PosMd;
} PKeZCrct;

typedef struct { /* 0x40 */
    sceVu0FMATRIX Mat;
} VKeZCrct;

void pppKeZCrctDraw(pppPObject *pobj, PKeZCrct *dat, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEZCRCT_H */
