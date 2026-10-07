#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOOPYZX_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOOPYZX_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPointLoop.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngleLoop.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScaleLoop.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixYZX.h"

typedef struct {
    pppCDT cdt;
} PMatrixLoopYZX;

void pppMatrixLoopYZX(pppPObject *pobj, PMatrixLoopYZX *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXLOOPYZX_H */
