#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLDNOROT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLDNOROT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

void sceVu0MulVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);

enum {
    RYJDRAWMATRIXWORLDNOROT_MATRIX = 0,
    RYJDRAWMATRIXWORLDNOROT_THIS = 1
};

typedef struct { /* 0x4 */
    pppCDT cdt;
} PRyjDrawMatrixWorldNoRot;

typedef struct { /* 0x20 */
    sceVu0FVECTOR vecPos;
    sceVu0FVECTOR vecScl;
} VRyjDrawMatrixWorldNoRot;

void pppRyjDrawMatrixWorldNoRotCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjDrawDrawMatrixWorldNoRot(pppPObject *pPobj, PRyjDrawMatrixWorldNoRot *pParam, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLDNOROT_H */
