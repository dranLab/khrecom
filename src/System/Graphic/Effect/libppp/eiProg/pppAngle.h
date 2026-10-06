#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPANGLE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPANGLE_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppIVECTOR ang;
} PAngle;

typedef struct { /* 0xc */
    pppIVECTOR ang;
} VAngle;

void pppAngle(pppPObject *pobj, PAngle *p, pppCtrlTable *ctbl);
void pppAngleCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPANGLE_H */
