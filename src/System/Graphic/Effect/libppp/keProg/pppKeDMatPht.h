#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEDMATPHT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEDMATPHT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x4 */
    pppCDT cdt;
} PKeDMatPht;

void pppKeDMatPhtDraw(pppPObject *pobj, PKeDMatPht *dat, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEDMATPHT_H */
