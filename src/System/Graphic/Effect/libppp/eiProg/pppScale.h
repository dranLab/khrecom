#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSCALE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSCALE_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppFVECTOR scl;
} PScale;

typedef struct { /* 0xc */
    pppFVECTOR scl;
} VScale;

void pppScale(pppPObject *pobj, PScale *p, pppCtrlTable *ctbl);
void pppScaleCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSCALE_H */
