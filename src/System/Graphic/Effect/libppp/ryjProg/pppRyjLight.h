#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJLIGHT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJLIGHT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

/* Parameter block (pppCDT, pppFVECTOR Vector, fRange, mode flags); unused by the empty functions. */
typedef struct PRyjLight PRyjLight;

void pppRyjLight(pppPObject *pPobj, PRyjLight *pParam, pppCtrlTable *pCtbl);
void pppRyjLightCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjLightDes(pppPObject *pPobj, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJLIGHT_H */
