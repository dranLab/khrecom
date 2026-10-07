#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDSHORT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDSHORT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0xC */
    pppCDT cdt;
    int pvoff;
    unsigned short rw;
    unsigned char rct;
} PRandShort;

typedef struct { /* 0x4 */
    float r0;
} VRandShort;

void pppRandShort(pppPObject *pobj, PRandShort *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDSHORT_H */
