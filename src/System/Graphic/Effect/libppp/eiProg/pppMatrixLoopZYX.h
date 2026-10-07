#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOOPZYX_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOOPZYX_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPointLoop.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngleLoop.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScaleLoop.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixZYX.h"

typedef struct {
    pppCDT cdt;
} PMatrixLoopZYX;

void pppMatrixLoopZYX(pppPObject *pobj, PMatrixLoopZYX *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOOPZYX_H */
