#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEDRCT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEDRCT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppFVECTOR Drct;
} PKeDrct;

typedef struct { /* 0x10 */
    sceVu0FVECTOR Drct;
} VKeDrct;

void pppKeDrct(pppPObject *pobj, PKeDrct *dat, pppCtrlTable *ctbl);
void pppKeDrctCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEDRCT_H */
