#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAILPHT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAILPHT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

/* dat is a PKeShpTailPht* in the original (pppCDT/pppHCVECTOR not in pppPart.h yet) */
void pppKeShpTailPht(pppPObject *pobj, void *dat, pppCtrlTable *ctbl);
void pppKeShpTailPhtDraw(pppPObject *pobj, void *dat, pppCtrlTable *ctbl);
void pppKeShpTailPhtCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeShpTailPhtCon2(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAILPHT_H */
