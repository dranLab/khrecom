#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDZYX_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDZYX_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngle.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScale.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixYZX.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixZYX.h"

enum {
    RYJMATRIXWORLDZYX_POINT = 0,
    RYJMATRIXWORLDZYX_ANGLE = 1,
    RYJMATRIXWORLDZYX_SCALE = 2,
    RYJMATRIXWORLDZYX_THIS = 3
};

typedef struct { /* 0x4 */
    pppCDT cdt;
} PRyjMatrixWorldZYX;

typedef struct { /* 0x40 */
    sceVu0FMATRIX matPar;
} VRyjMatrixWorldZYX;

void pppRyjMatrixWorldZYXCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjMatrixWorldZYX(pppPObject *pPobj, PRyjMatrixWorldZYX *pParam, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDZYX_H */
