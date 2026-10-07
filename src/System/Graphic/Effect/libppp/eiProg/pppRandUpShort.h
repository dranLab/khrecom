#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDUPSHORT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDUPSHORT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0xC */
    pppCDT cdt;
    int pvoff;
    unsigned short rw;
    unsigned char rct;
} PRandUpShort;

typedef struct { /* 0x4 */
    float r0;
} VRandUpShort;

void pppRandUpShort(pppPObject *pobj, PRandUpShort *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDUPSHORT_H */
