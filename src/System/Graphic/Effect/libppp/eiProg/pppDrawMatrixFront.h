#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMATRIXFRONT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMATRIXFRONT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct {
    pppCDT cdt;
} PDrawMatrixFront;

void pppDrawMatrixFront(pppPObject *pobj, PDrawMatrixFront *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMATRIXFRONT_H */
