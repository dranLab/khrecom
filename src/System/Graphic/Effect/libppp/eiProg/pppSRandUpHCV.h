#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDUPHCV_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDUPHCV_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x14 */
    pppCDT cdt;
    int pvoff;
    pppHCVECTOR rw;
    unsigned char rct;
} PSRandUpHCV;

typedef struct { /* 0x10 */
    float r0;
    float r1;
    float r2;
    float r3;
} VSRandUpHCV;

void pppSRandUpHCV(pppPObject *pobj, PSRandUpHCV *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDUPHCV_H */
