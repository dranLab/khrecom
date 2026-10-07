#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPANGMOVELOOP_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPANGMOVELOOP_H

#include "System/Graphic/Effect/libppp/eiProg/pppAngleLoop.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppIVECTOR m_dang;
} PAngMoveLoop;

typedef struct { /* 0xc */
    pppIVECTOR m_dang;
} VAngMoveLoop;

void pppAngMoveLoopCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppAngMoveLoopCon2(pppPObject *pobj, pppCtrlTable *ctbl);
void pppAngMoveLoop(pppPObject *pobj, PAngMoveLoop *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPANGMOVELOOP_H */
