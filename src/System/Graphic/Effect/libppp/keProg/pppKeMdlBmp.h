#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLBMP_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLBMP_H

#include "System/Graphic/Effect/libppp/pppPart.h"

/* dat is a PKeMdlBmp* in the original (pppCDT/pppParamPObj not in pppPart.h yet) */
void pppKeMdlBmpDraw(pppPObject *pobj, void *dat, pppCtrlTable *ctbl);
void pppKeMdlBmpCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeMdlBmpCon2(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEMDLBMP_H */
