#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDCV_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDCV_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    int pvoff;
    pppCVECTOR rw;
    unsigned char rct;
} PSRandCV;

typedef struct { /* 0x10 */
    float r0;
    float r1;
    float r2;
    float r3;
} VSRandCV;

void pppSRandCV(pppPObject *pobj, PSRandCV *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDCV_H */
