#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXYZX_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXYZX_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngle.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScale.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixYZX.h"

typedef struct {
    pppCDT cdt;
} PMatrixYZX;

void pppMatrixYZX(pppPObject *pobj, PMatrixYZX *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXYZX_H */
