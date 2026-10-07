#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPANGMOVE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPANGMOVE_H

#include "System/Graphic/Effect/libppp/eiProg/pppAngle.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppIVECTOR dang;
} PAngMove;

typedef struct { /* 0xc */
    pppIVECTOR dang;
} VAngMove;

void pppAngMove(pppPObject *pobj, PAngMove *p, pppCtrlTable *ctbl);
void pppAngMoveCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPANGMOVE_H */
