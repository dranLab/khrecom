#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMATSN_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMATSN_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixXYZ.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngle.h"

typedef struct { /* 0x4 */
    pppCDT cdt;
} PKeMatSN;

typedef struct { /* 0x90 */
    sceVu0FMATRIX mat;
    sceVu0FMATRIX Nega;
    pppIVECTOR PreRot;
} VKeMatSN;

extern sceVu0FVECTOR KeVec0001;

void pppKeMatSN(pppPObject *pobj, PKeMatSN *dat, pppCtrlTable *ctbl);
void pppKeMatSNCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMATSN_H */
