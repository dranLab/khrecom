#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDZXY_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDZXY_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngle.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScale.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixYZX.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixZXY.h"

enum {
    RYJMATRIXWORLDZXY_POINT = 0,
    RYJMATRIXWORLDZXY_ANGLE = 1,
    RYJMATRIXWORLDZXY_SCALE = 2,
    RYJMATRIXWORLDZXY_THIS = 3
};

typedef struct { /* 0x4 */
    pppCDT cdt;
} PRyjMatrixWorldZXY;

typedef struct { /* 0x40 */
    sceVu0FMATRIX matPar;
} VRyjMatrixWorldZXY;

void pppRyjMatrixWorldZXYCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjMatrixWorldZXY(pppPObject *pPobj, PRyjMatrixWorldZXY *pParam, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDZXY_H */
