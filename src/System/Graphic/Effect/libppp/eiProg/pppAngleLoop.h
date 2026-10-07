#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPANGLELOOP_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPANGLELOOP_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppIVECTOR m_ang;
} PAngleLoop;

typedef struct { /* 0x10 */
    pppIVECTOR m_ang;
    void *m_pp;
} VAngleLoop;

void pppAngleLoopCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppAngleLoopCon2(pppPObject *pobj, pppCtrlTable *ctbl);
void pppAngleLoop(pppPObject *pobj, PAngleLoop *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPANGLELOOP_H */
