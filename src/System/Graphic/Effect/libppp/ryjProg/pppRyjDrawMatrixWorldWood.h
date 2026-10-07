#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLDWOOD_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLDWOOD_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    RYJDRAWMATRIXWORLDWOOD_MATRIX = 0,
    RYJDRAWMATRIXWORLDWOOD_THIS = 1
};

typedef struct { /* 0x4 */
    pppCDT cdt;
} PRyjDrawMatrixWorldWood;

typedef struct { /* 0x20 */
    sceVu0FVECTOR vecPos;
    sceVu0FVECTOR vecScl;
} VRyjDrawMatrixWorldWood;

void pppRyjDrawMatrixWorldWoodCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjDrawDrawMatrixWorldWood(pppPObject *pPobj, PRyjDrawMatrixWorldWood *pParam, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLDWOOD_H */
