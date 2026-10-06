#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPARMATRIX_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPARMATRIX_H

#include "System/Graphic/Effect/libppp/pppPart.h"

void sceVu0ApplyMatrix(sceVu0FVECTOR v0, sceVu0FMATRIX m0, sceVu0FVECTOR v1);

void pppParMatrix(pppPObject *pobj, void *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPARMATRIX_H */
