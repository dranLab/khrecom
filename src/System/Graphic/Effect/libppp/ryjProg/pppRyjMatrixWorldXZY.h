#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDXZY_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDXZY_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngle.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScale.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixXZY.h"

enum {
    RYJMATRIXWORLDXZY_POINT = 0,
    RYJMATRIXWORLDXZY_ANGLE = 1,
    RYJMATRIXWORLDXZY_SCALE = 2,
    RYJMATRIXWORLDXZY_THIS = 3
};

typedef struct { /* 0x4 */
    pppCDT cdt;
} PRyjMatrixWorldXZY;

typedef struct { /* 0x40 */
    sceVu0FMATRIX matPar;
} VRyjMatrixWorldXZY;

void pppRyjMatrixWorldXZYCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjMatrixWorldXZY(pppPObject *pPobj, PRyjMatrixWorldXZY *pParam, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMATRIXWORLDXZY_H */
