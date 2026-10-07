#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXNOROT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXNOROT_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScale.h"

typedef struct {
    pppCDT cdt;
} PRyjMatrixNoRot;

void pppRyjMatrixNoRot(pppPObject *pPobj, PRyjMatrixNoRot *pParam, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXNOROT_H */
