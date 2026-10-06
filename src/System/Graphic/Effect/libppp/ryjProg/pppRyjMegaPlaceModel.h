#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGAPLACEMODEL_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGAPLACEMODEL_H

#include "pppRyjMegaPlace.h"

typedef struct { /* 0x1 */
    unsigned char bEntry;
} VRyjMegaPlaceModel;

void pppRyjMegaPlaceModel(pppPObject *pPobj, PRyjMegaPlaceModel *pParam, pppCtrlTable *pCtbl);
void pppRyjMegaPlaceModelCon(pppPObject *pPobj, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGAPLACEMODEL_H */
