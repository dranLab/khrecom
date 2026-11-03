#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHUSERCTRL3_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHUSERCTRL3_H

#include "System/Graphic/Effect/libppp/partMng.h"

typedef struct { /* 0x20 */
    pppCDT cdt;
    int shape;
    pppCVECTOR shape_color;
    float scale;
    float length;
    unsigned int num;
    unsigned int delta;
    unsigned char fog;
    unsigned char near;
} PRyjMegaBirthUserCtrl3;

void pppRyjDrawMegaBirthUserCtrl3(pppPObject *pobj, PRyjMegaBirthUserCtrl3 *param, pppCtrlTable *ctbl);
void pppRyjMegaBirthUserCtrl3Con(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHUSERCTRL3_H */
