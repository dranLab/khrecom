#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCHRYSCLXYZMATRIX_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCHRYSCLXYZMATRIX_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct {
    pppCDT cdt;
    float sclr;
    float maxscl;
} PChrYSclXYZMatrix;

typedef struct {
    int dummy;
} VChrYSclXYZMatrix;

void pppChrYSclXYZMatrix(pppPObject *pobj, PChrYSclXYZMatrix *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCHRYSCLXYZMATRIX_H */
