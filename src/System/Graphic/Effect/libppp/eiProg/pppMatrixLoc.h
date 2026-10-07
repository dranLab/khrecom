#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOC_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOC_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"

typedef struct {
    pppCDT cdt;
} PMatrixLoc;


void pppMatrixLoc(pppPObject *pobj, PMatrixLoc *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOC_H */
