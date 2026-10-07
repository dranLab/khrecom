#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMOVE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMOVE_H

#include "System/Graphic/Effect/libppp/eiProg/pppPoint.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppFVECTOR vec;
} PMove;

typedef struct { /* 0xc */
    pppFVECTOR vec;
} VMove;

void pppMove(pppPObject *pobj, PMove *p, pppCtrlTable *ctbl);
void pppMoveCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMOVE_H */
