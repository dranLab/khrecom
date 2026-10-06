#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEDMAT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEDMAT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x4 */
    pppCDT cdt;
} PKeDMat;

typedef struct { /* 0x40 */
    sceVu0FMATRIX KeDMat;
} VKeDMat;

void pppKeDMatDraw(pppPObject *pobj, PKeDMat *dat, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEDMAT_H */
