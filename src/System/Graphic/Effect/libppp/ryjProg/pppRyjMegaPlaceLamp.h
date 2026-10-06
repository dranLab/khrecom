#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGAPLACELAMP_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGAPLACELAMP_H

#include "pppRyjMegaPlace.h"

typedef struct { /* 0x1 */
    unsigned char bEntry;
} VRyjMegaPlaceLamp;

void pppRyjMegaPlaceLamp(pppPObject *pPobj, PRyjMegaPlaceLamp *pParam, pppCtrlTable *pCtbl);
void pppRyjMegaPlaceLampCon(pppPObject *pPobj, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGAPLACELAMP_H */
