#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPOINT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPOINT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppFVECTOR pos;
} PPoint;

typedef struct { /* 0xc */
    pppFVECTOR pos;
} VPoint;

void pppPoint(pppPObject *pobj, PPoint *p, pppCtrlTable *ctbl);
void pppPointCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPOINT_H */
