#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMATRIXLOOP_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMATRIXLOOP_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct {
    pppCDT cdt;
} PDrawMatrixLoop;

void pppDrawMatrixLoop(pppPObject *pobj, PDrawMatrixLoop *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMATRIXLOOP_H */
