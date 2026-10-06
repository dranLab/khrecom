#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEDMATFR_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEDMATFR_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x4 */
    pppCDT cdt;
} PKeDMatFr;

typedef struct { /* 0x40 */
    sceVu0FMATRIX KeDMat;
} VKeDMatFr;

void pppKeDMatFrDraw(pppPObject *pobj, PKeDMatFr *dat, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEDMATFR_H */
