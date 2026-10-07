#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLDDTTFR_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLDDTTFR_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    RYJDRAWMATRIXWORLDDTTFR_MATRIX = 0,
    RYJDRAWMATRIXWORLDDTTFR_THIS = 1
};

typedef struct { /* 0x4 */
    pppCDT cdt;
} PRyjDrawMatrixWorldDttFr;

typedef struct { /* 0x60 */
    sceVu0FMATRIX KeDMat;
    sceVu0FVECTOR vecPos;
    sceVu0FVECTOR vecScl;
} VRyjDrawMatrixWorldDttFr;

void pppRyjDrawMatrixWorldDttFrCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjDrawDrawMatrixWorldDttFr(pppPObject *pPobj, PRyjDrawMatrixWorldDttFr *pParam, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLDDTTFR_H */
