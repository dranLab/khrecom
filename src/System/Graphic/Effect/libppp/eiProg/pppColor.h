#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCOLOR_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCOLOR_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0xc */
    pppCDT cdt;
    pppHCVECTOR col;
} PColor;

typedef struct { /* 0xc */
    pppHCVECTOR col;
    pppCVECTOR release_col;
} VColor;

void pppColor(pppPObject *pobj, PColor *p, pppCtrlTable *ctbl);
void pppColorCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCOLOR_H */
