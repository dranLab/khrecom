#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMOVELOOP_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMOVELOOP_H

#include "System/Graphic/Effect/libppp/eiProg/pppPointLoop.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppFVECTOR m_vec;
} PMoveLoop;

typedef struct { /* 0xc */
    pppFVECTOR m_vec;
} VMoveLoop;

void pppMoveLoopCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppMoveLoopCon2(pppPObject *pobj, pppCtrlTable *ctbl);
void pppMoveLoop(pppPObject *pobj, PMoveLoop *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPMOVELOOP_H */
