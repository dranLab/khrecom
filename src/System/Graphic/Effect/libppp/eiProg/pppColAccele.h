#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCOLACCELE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCOLACCELE_H

#include "System/Graphic/Effect/libppp/eiProg/pppColMove.h"

typedef struct { /* 0xc */
    pppCDT cdt;
    pppHCVECTOR ddcol;
} PColAccele;

typedef struct { /* 0x8 */
    pppHCVECTOR ddcol;
} VColAccele;

void pppColAccele(pppPObject *pobj, PColAccele *p, pppCtrlTable *ctbl);
void pppColAcceleCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCOLACCELE_H */
