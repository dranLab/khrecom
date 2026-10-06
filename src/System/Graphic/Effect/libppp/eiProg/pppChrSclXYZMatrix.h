#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCHRSCLXYZMATRIX_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCHRSCLXYZMATRIX_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct {
    pppCDT cdt;
    float sclr;
    float maxscl;
} PChrSclXYZMatrix;

typedef struct {
    int dummy;
} VChrSclXYZMatrix;

void pppChrSclXYZMatrix(pppPObject *pobj, PChrSclXYZMatrix *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCHRSCLXYZMATRIX_H */
