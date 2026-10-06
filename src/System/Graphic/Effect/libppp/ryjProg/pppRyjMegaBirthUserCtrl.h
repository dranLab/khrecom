#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHUSERCTRL_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHUSERCTRL_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x14 */
    pppCDT cdt;
    int shape;
    pppCVECTOR shape_color;
    unsigned int delta;
    unsigned char fog;
    unsigned char near;
} PRyjMegaBirthUserCtrl;

void pppRyjDrawMegaBirthUserCtrl(pppPObject *pobj, PRyjMegaBirthUserCtrl *param, pppCtrlTable *ctbl);
void pppRyjMegaBirthUserCtrlCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHUSERCTRL_H */
