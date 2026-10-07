#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDDOWNHCV_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDDOWNHCV_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    int pvoff;
    pppHCVECTOR rw;
    unsigned char rct;
} PRandDownHCV;

typedef struct { /* 0x4 */
    float r0;
} VRandDownHCV;

void pppRandDownHCV(pppPObject *pobj, PRandDownHCV *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDDOWNHCV_H */
