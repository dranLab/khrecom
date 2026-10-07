#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOOPXZY_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOOPXZY_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPointLoop.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngleLoop.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScaleLoop.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixXZY.h"

typedef struct {
    pppCDT cdt;
} PMatrixLoopXZY;

void pppMatrixLoopXZY(pppPObject *pobj, PMatrixLoopXZY *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOOPXZY_H */
