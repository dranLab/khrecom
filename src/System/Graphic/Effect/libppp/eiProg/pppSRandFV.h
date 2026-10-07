#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDFV_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDFV_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x18 */
    pppCDT cdt;
    int pvoff;
    pppFVECTOR rw;
    unsigned char rct;
} PSRandFV;

typedef struct { /* 0xc */
    float r0;
    float r1;
    float r2;
} VSRandFV;

void pppSRandFV(pppPObject *pobj, PSRandFV *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDFV_H */
