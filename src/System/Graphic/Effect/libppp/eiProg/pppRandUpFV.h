#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDUPFV_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDUPFV_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x18 */
    pppCDT cdt;
    int pvoff;
    pppFVECTOR rw;
    unsigned char rct;
} PRandUpFV;

typedef struct { /* 0x4 */
    float r0;
} VRandUpFV;

void pppRandUpFV(pppPObject *pobj, PRandUpFV *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDUPFV_H */
