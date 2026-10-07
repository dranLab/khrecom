#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEDMATPHTFR_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEDMATPHTFR_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x4 */
    pppCDT cdt;
} PKeDMatPhtFr;

void pppKeDMatPhtFrDraw(pppPObject *pobj, PKeDMatPhtFr *dat, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEDMATPHTFR_H */
