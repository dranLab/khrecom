#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCHRSCLYMATRIX_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCHRSCLYMATRIX_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct {
    pppCDT cdt;
    float sclr;
    float maxscl;
} PChrSclYMatrix;

typedef struct {
    int dummy;
} VChrSclYMatrix;

void sceVu0UnitMatrix(sceVu0FMATRIX m);

void pppChrSclYMatrix(pppPObject *pobj, PChrSclYMatrix *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCHRSCLYMATRIX_H */
