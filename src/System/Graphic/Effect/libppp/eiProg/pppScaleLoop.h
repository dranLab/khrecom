#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSCALELOOP_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSCALELOOP_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppFVECTOR m_scl;
} PScaleLoop;

typedef struct { /* 0x10 */
    pppFVECTOR m_scl;
    void *m_pp;
} VScaleLoop;

void pppScaleLoopCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppScaleLoopCon2(pppPObject *pobj, pppCtrlTable *ctbl);
void pppScaleLoop(pppPObject *pobj, PScaleLoop *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPSCALELOOP_H */
