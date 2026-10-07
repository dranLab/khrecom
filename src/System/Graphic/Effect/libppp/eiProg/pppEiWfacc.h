#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPEIWFACC_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPEIWFACC_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppMove.h"
#include "System/Graphic/Effect/libppp/eiProg/pppEiWindFun.h"

typedef struct { /* 0x18 */
    pppCDT cdt;
    pppParamPObj fun;
    pppParamPObj pos;
    float mass;
} PEiWfacc;

float sceVu0InnerProduct(sceVu0FVECTOR v0, sceVu0FVECTOR v1);

void pppEiWfacc(pppPObject *pobj, PEiWfacc *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPEIWFACC_H */
