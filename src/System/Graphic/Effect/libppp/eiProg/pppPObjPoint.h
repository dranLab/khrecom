#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPOBJPOINT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPOBJPOINT_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppParamPObj pobj;
} PPObjPoint;

typedef struct { /* 0x10 */
    pppFVECTOR pos;
    VPoint *lpvp;
} VPObjPoint;

void pppPObjPoint(pppPObject *pobj, PPObjPoint *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPOBJPOINT_H */
