#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDUPIV_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDUPIV_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x18 */
    pppCDT cdt;
    int pvoff;
    pppIVECTOR rw;
    unsigned char rct;
} PRandUpIV;

typedef struct { /* 0x4 */
    float r0;
} VRandUpIV;

void pppRandUpIV(pppPObject *pobj, PRandUpIV *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDUPIV_H */
