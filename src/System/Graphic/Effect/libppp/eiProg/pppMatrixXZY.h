#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXXZY_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXXZY_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngle.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScale.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixXZY.h"

typedef struct {
    pppCDT cdt;
} PMatrixXZY;

void pppMatrixXZY(pppPObject *pobj, PMatrixXZY *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXXZY_H */
