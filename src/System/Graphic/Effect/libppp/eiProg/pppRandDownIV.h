#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDDOWNIV_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDDOWNIV_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x18 */
    pppCDT cdt;
    int pvoff;
    pppIVECTOR rw;
    unsigned char rct;
} PRandDownIV;

typedef struct { /* 0x4 */
    float r0;
} VRandDownIV;

void pppRandDownIV(pppPObject *pobj, PRandDownIV *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDDOWNIV_H */
