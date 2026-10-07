#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMVYPEFF_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMVYPEFF_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppMove.h"
#include "System/Graphic/Effect/libppp/eiProg/pppAngle.h"
#include "System/Graphic/Effect/libppp/keProg/KeBornRnd.h"

enum {
    IKeMvYpEff_Angle = 0,
    IKeMvYpEff_Move = 1
};

typedef struct { /* 0x4 */
    pppCDT cdt;
} PKeMvYpEff;

int __KeGetDeg2(float *xy, float *deg);
void pppKeMvYpEff(pppPObject *pobj, PKeMvYpEff *dat, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMVYPEFF_H */
