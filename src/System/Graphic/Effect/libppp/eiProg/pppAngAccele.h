#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPANGACCELE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPANGACCELE_H

#include "System/Graphic/Effect/libppp/eiProg/pppAngMove.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppIVECTOR ddang;
} PAngAccele;

typedef struct { /* 0xc */
    pppIVECTOR ddang;
} VAngAccele;

void pppAngAccele(pppPObject *pobj, PAngAccele *p, pppCtrlTable *ctbl);
void pppAngAcceleCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPANGACCELE_H */
