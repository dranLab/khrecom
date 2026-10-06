#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHUSERCTRL2_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHUSERCTRL2_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x1c */
    pppCDT cdt;
    int shape;
    pppCVECTOR shape_color;
    unsigned int delta;
    int model;
    pppCVECTOR model_color;
    unsigned char fog;
    unsigned char near;
} PRyjMegaBirthUserCtrl2;

void pppRyjDrawMegaBirthUserCtrl2(pppPObject *pobj, PRyjMegaBirthUserCtrl2 *param, pppCtrlTable *ctbl);
void pppRyjMegaBirthUserCtrl2Con(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHUSERCTRL2_H */
