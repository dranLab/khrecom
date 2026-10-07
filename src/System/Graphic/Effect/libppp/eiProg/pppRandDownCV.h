#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDDOWNCV_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDDOWNCV_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    int pvoff;
    pppCVECTOR rw;
    unsigned char rct;
} PRandDownCV;

typedef struct { /* 0x4 */
    float r0;
} VRandDownCV;

void pppRandDownCV(pppPObject *pobj, PRandDownCV *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDDOWNCV_H */
