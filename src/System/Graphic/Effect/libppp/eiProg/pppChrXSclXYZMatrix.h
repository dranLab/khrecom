#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCHRXSCLXYZMATRIX_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCHRXSCLXYZMATRIX_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct {
    pppCDT cdt;
    float sclr;
    float maxscl;
} PChrXSclXYZMatrix;

typedef struct {
    int dummy;
} VChrXSclXYZMatrix;

void pppChrXSclXYZMatrix(pppPObject *pobj, PChrXSclXYZMatrix *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCHRXSCLXYZMATRIX_H */
