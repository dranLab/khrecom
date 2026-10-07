#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPACCELE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPACCELE_H

#include "System/Graphic/Effect/libppp/eiProg/pppMove.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppFVECTOR vec;
} PAccele;

typedef struct { /* 0xc */
    pppFVECTOR vec;
} VAccele;

void pppAccele(pppPObject *pobj, PAccele *p, pppCtrlTable *ctbl);
void pppAcceleCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPACCELE_H */
