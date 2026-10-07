#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDDOWNFV_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDDOWNFV_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x18 */
    pppCDT cdt;
    int pvoff;
    pppFVECTOR rw;
    unsigned char rct;
} PSRandDownFV;

typedef struct { /* 0xc */
    float r0;
    float r1;
    float r2;
} VSRandDownFV;

void pppSRandDownFV(pppPObject *pobj, PSRandDownFV *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDDOWNFV_H */
