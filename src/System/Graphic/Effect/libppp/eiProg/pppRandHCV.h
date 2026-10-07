#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDHCV_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDHCV_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    int pvoff;
    pppHCVECTOR rw;
    unsigned char rct;
} PRandHCV;

typedef struct { /* 0x4 */
    float r0;
} VRandHCV;

void pppRandHCV(pppPObject *pobj, PRandHCV *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDHCV_H */
