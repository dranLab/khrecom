#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDXYZ_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDXYZ_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngle.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScale.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixXYZ.h"

enum {
    RYJMATRIXWORLDXYZ_POINT = 0,
    RYJMATRIXWORLDXYZ_ANGLE = 1,
    RYJMATRIXWORLDXYZ_SCALE = 2,
    RYJMATRIXWORLDXYZ_THIS = 3
};

typedef struct { /* 0x4 */
    pppCDT cdt;
} PRyjMatrixWorldXYZ;

typedef struct { /* 0x40 */
    sceVu0FMATRIX matPar;
} VRyjMatrixWorldXYZ;

void pppRyjMatrixWorldXYZCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjMatrixWorldXYZ(pppPObject *pPobj, PRyjMatrixWorldXYZ *pParam, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDXYZ_H */
