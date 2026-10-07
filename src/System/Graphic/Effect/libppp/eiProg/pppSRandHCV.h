#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDHCV_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDHCV_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x14 */
    pppCDT cdt;
    int pvoff;
    pppHCVECTOR rw;
    unsigned char rct;
} PSRandHCV;

typedef struct { /* 0x10 */
    float r0;
    float r1;
    float r2;
    float r3;
} VSRandHCV;

void pppSRandHCV(pppPObject *pobj, PSRandHCV *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDHCV_H */
