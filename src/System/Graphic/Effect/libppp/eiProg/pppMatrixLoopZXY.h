#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOOPZXY_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOOPZXY_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPointLoop.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngleLoop.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScaleLoop.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixZXY.h"

typedef struct {
    pppCDT cdt;
} PMatrixLoopZXY;

void pppMatrixLoopZXY(pppPObject *pobj, PMatrixLoopZXY *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOOPZXY_H */
