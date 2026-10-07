#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDYZX_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDYZX_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngle.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScale.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixYZX.h"

enum {
    RYJMATRIXWORLDYZX_POINT = 0,
    RYJMATRIXWORLDYZX_ANGLE = 1,
    RYJMATRIXWORLDYZX_SCALE = 2,
    RYJMATRIXWORLDYZX_THIS = 3
};

typedef struct { /* 0x4 */
    pppCDT cdt;
} PRyjMatrixWorldYZX;

typedef struct { /* 0x40 */
    sceVu0FMATRIX matPar;
} VRyjMatrixWorldYZX;

void pppRyjMatrixWorldYZXCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjMatrixWorldYZX(pppPObject *pPobj, PRyjMatrixWorldYZX *pParam, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDYZX_H */
