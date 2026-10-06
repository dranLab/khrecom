#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOC_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOC_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct {
    pppCDT cdt;
} PMatrixLoc;

typedef struct { /* 0xc */
    pppFVECTOR pos;
} VMatrixLocPoint;

void sceVu0UnitMatrix(sceVu0FMATRIX m);

void pppMatrixLoc(pppPObject *pobj, PMatrixLoc *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOC_H */
