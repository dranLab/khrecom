// PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppMatrixLoop
#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPOINTLOOP_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPOINTLOOP_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    pppFVECTOR m_pos;
} PPointLoop;

typedef struct { /* 0x10 */
    pppFVECTOR m_pos;
    void *m_pp;
} VPointLoop;

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPPOINTLOOP_H */
