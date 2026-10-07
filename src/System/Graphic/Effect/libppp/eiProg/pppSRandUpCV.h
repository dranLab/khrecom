#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDUPCV_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDUPCV_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    int pvoff;
    pppCVECTOR rw;
    unsigned char rct;
} PSRandUpCV;

typedef struct { /* 0x10 */
    float r0;
    float r1;
    float r2;
    float r3;
} VSRandUpCV;

void pppSRandUpCV(pppPObject *pobj, PSRandUpCV *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSRANDUPCV_H */
