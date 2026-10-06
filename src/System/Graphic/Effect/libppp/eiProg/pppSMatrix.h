#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSMATRIX_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSMATRIX_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct {
    pppCDT cdt;
} PSMatrix;

typedef struct {
    sceVu0FMATRIX mat;
} VSMatrix;

extern sceVu0FMATRIX *ppvlpCMatrix;

void pppSMatrix(pppPObject *pobj, PSMatrix *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSMATRIX_H */
