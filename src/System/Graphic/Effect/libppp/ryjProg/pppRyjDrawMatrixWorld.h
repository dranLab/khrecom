#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLD_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLD_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    RYJDRAWMATRIXWORLD_MATRIX = 0,
    RYJDRAWMATRIXWORLD_THIS = 1
};

typedef struct { /* 0x4 */
    pppCDT cdt;
} PRyjDrawMatrixWorld;

typedef struct { /* 0x40 */
    sceVu0FMATRIX matPar;
} VRyjDrawMatrixWorld;

void pppRyjDrawMatrixWorldCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjDrawDrawMatrixWorld(pppPObject *pPobj, PRyjDrawMatrixWorld *pParam, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLD_H */
