// PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppMatrixLoop
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

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPANGLELOOP_H */
