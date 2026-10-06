#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEPARMATR_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEPARMATR_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0xc */
    pppCDT cdt;
    pppParamPObj mat;
} PKeParMatR;

void pppKeParMatR(pppPObject *pobj, PKeParMatR *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEPARMATR_H */
