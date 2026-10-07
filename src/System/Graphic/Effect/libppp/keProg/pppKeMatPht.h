#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMATPHT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMATPHT_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngle.h"
#include "System/Graphic/Effect/libppp/eiProg/pppScale.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixZYX.h"
#include "System/Graphic/Effect/libppp/keProg/KeDMat.h"

typedef struct { /* 0x4 */
    pppCDT cdt;
} PKeMatPht;

void pppKeMatPht(pppPObject *pobj, PKeMatPht *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMATPHT_H */
