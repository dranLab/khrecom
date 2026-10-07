#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSCLACCELE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSCLACCELE_H

#include "System/Graphic/Effect/libppp/eiProg/pppSclMove.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppFVECTOR ddscl;
} PSclAccele;

typedef struct { /* 0xc */
    pppFVECTOR ddscl;
} VSclAccele;

void pppSclAccele(pppPObject *pobj, PSclAccele *p, pppCtrlTable *ctbl);
void pppSclAcceleCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSCLACCELE_H */
