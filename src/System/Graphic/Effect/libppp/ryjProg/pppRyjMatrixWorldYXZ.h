#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDYXZ_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDYXZ_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngle.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScale.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixYXZ.h"

enum {
    RYJMATRIXWORLDYXZ_POINT = 0,
    RYJMATRIXWORLDYXZ_ANGLE = 1,
    RYJMATRIXWORLDYXZ_SCALE = 2,
    RYJMATRIXWORLDYXZ_THIS = 3
};

typedef struct { /* 0x4 */
    pppCDT cdt;
} PRyjMatrixWorldYXZ;

typedef struct { /* 0x40 */
    sceVu0FMATRIX matPar;
} VRyjMatrixWorldYXZ;

void pppRyjMatrixWorldYXZCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjMatrixWorldYXZ(pppPObject *pPobj, PRyjMatrixWorldYXZ *pParam, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDYXZ_H */
