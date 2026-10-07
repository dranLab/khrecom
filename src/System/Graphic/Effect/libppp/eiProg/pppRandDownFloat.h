#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDDOWNFLOAT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDDOWNFLOAT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    int pvoff;
    float rw;
    unsigned char rct;
} PRandDownFloat;

typedef struct { /* 0x4 */
    float r0;
} VRandDownFloat;

void pppRandDownFloat(pppPObject *pobj, PRandDownFloat *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDDOWNFLOAT_H */
