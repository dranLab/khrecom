// PARTIAL: added for System/Graphic/Effect/libppp/eiProg/pppColMove
#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCOLOR_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCOLOR_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0xc */
    pppCDT cdt;
    pppHCVECTOR col;
} PColor;

typedef struct { /* 0xc */
    pppHCVECTOR col;
    pppCVECTOR release_col;
} VColor;

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPCOLOR_H */
