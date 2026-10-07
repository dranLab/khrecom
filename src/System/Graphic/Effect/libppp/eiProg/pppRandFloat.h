#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDFLOAT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDFLOAT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    int pvoff;
    float rw;
    unsigned char rct;
} PRandFloat;

typedef struct { /* 0x4 */
    float r0;
} VRandFloat;

void pppRandFloat(pppPObject *pobj, PRandFloat *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDFLOAT_H */
