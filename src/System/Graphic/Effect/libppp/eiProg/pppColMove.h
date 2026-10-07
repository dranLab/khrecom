#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCOLMOVE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCOLMOVE_H

#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"

typedef struct { /* 0xc */
    pppCDT cdt;
    pppHCVECTOR dcol;
} PColMove;

typedef struct { /* 0x8 */
    pppHCVECTOR dcol;
} VColMove;

void pppColMove(pppPObject *pobj, PColMove *p, pppCtrlTable *ctbl);
void pppColMoveCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCOLMOVE_H */
