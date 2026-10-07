#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSCLMOVELOOP_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSCLMOVELOOP_H

#include "System/Graphic/Effect/libppp/eiProg/pppScaleLoop.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppFVECTOR m_dscl;
} PSclMoveLoop;

typedef struct { /* 0xc */
    pppFVECTOR m_dscl;
} VSclMoveLoop;

void pppSclMoveLoopCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppSclMoveLoopCon2(pppPObject *pobj, pppCtrlTable *ctbl);
void pppSclMoveLoop(pppPObject *pobj, PSclMoveLoop *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSCLMOVELOOP_H */
