#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDDOWNFV_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDDOWNFV_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x18 */
    pppCDT cdt;
    int pvoff;
    pppFVECTOR rw;
    unsigned char rct;
} PRandDownFV;

typedef struct { /* 0x4 */
    float r0;
} VRandDownFV;

void pppRandDownFV(pppPObject *pobj, PRandDownFV *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDDOWNFV_H */
