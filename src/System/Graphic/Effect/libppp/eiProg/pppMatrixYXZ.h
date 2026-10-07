#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXYXZ_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXYXZ_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngle.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScale.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixYXZ.h"

typedef struct {
    pppCDT cdt;
} PMatrixYXZ;

void pppMatrixYXZ(pppPObject *pobj, PMatrixYXZ *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMATRIXYXZ_H */
