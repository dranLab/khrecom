#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGAPLACESHAPE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGAPLACESHAPE_H

#include "pppRyjMegaPlace.h"

typedef struct { /* 0x1 */
    unsigned char bEntry;
} VRyjMegaPlaceShape;

void pppRyjMegaPlaceShape(pppPObject *pPobj, PRyjMegaPlaceShape *pParam, pppCtrlTable *pCtbl);
void pppRyjMegaPlaceShapeCon(pppPObject *pPobj, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGAPLACESHAPE_H */
