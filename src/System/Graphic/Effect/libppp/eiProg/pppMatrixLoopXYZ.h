#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOOPXYZ_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOOPXYZ_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPointLoop.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngleLoop.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScaleLoop.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixXYZ.h"

typedef struct {
    pppCDT cdt;
} PMatrixLoopXYZ;

void pppMatrixLoopXYZ(pppPObject *pobj, PMatrixLoopXYZ *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOOPXYZ_H */
