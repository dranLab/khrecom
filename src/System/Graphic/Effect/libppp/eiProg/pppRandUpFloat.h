#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDUPFLOAT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDUPFLOAT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    int pvoff;
    float rw;
    unsigned char rct;
} PRandUpFloat;

typedef struct { /* 0x4 */
    float r0;
} VRandUpFloat;

void pppRandUpFloat(pppPObject *pobj, PRandUpFloat *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDUPFLOAT_H */
