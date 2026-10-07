#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDDOWNHCV_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDDOWNHCV_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x14 */
    pppCDT cdt;
    int pvoff;
    pppHCVECTOR rw;
    unsigned char rct;
} PSRandDownHCV;

typedef struct { /* 0x10 */
    float r0;
    float r1;
    float r2;
    float r3;
} VSRandDownHCV;

void pppSRandDownHCV(pppPObject *pobj, PSRandDownHCV *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDDOWNHCV_H */
