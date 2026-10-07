#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSCLMOVE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSCLMOVE_H

#include "System/Graphic/Effect/libppp/eiProg/pppScale.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppFVECTOR dscl;
} PSclMove;

typedef struct { /* 0xc */
    pppFVECTOR dscl;
} VSclMove;

void pppSclMove(pppPObject *pobj, PSclMove *p, pppCtrlTable *ctbl);
void pppSclMoveCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSCLMOVE_H */
