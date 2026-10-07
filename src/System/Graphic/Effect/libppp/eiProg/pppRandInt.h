#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDINT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDINT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0xC */
    pppCDT cdt;
    int pvoff;
    unsigned int rw;
    unsigned char rct;
} PRandInt;

typedef struct { /* 0x4 */
    float r0;
} VRandInt;

void pppRandInt(pppPObject *pobj, PRandInt *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDINT_H */
