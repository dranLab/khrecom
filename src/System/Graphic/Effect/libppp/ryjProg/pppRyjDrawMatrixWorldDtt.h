#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLDDTT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLDDTT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    RYJDRAWMATRIXWORLDDTT_MATRIX = 0,
    RYJDRAWMATRIXWORLDDTT_THIS = 1
};

typedef struct { /* 0x4 */
    pppCDT cdt;
} PRyjDrawMatrixWorldDtt;

typedef struct { /* 0x80 */
    sceVu0FMATRIX KeDMat;
    sceVu0FMATRIX matPar;
} VRyjDrawMatrixWorldDtt;

void pppRyjDrawMatrixWorldDttCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjDrawDrawMatrixWorldDtt(pppPObject *pPobj, PRyjDrawMatrixWorldDtt *pParam, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLDDTT_H */
