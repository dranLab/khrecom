#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPEIWINDFUN_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPEIWINDFUN_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x24 */
    pppCDT cdt;
    pppFVECTOR vec;
    float len;
    float r;
    float wv;
    float wdv;
    float wddv;
} PEiWindFun;

typedef struct { /* 0x24 */
    pppFVECTOR vec;
    float len;
    float r;
    float cosa;
    float wv;
    float wdv;
    float wddv;
} VEiWindFun;

void pppEiWindFunCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppEiWindFun(pppPObject *pobj, PEiWindFun *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPEIWINDFUN_H */
