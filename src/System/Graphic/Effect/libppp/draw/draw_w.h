/* PARTIAL: added for System/Graphic/Effect/libppp/draw/md */
#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_DRAW_W_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_DRAW_W_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct _sceDmaChan sceDmaChan;
void sceDmaSend(sceDmaChan *d, void *tag);
int sceGsSyncPath(int mode, u_short timeout);

typedef struct { /* 0x90 */
    u_long128 dma_code;
    u_long gif_code;
    u_long giftag;
    u_long frame;
    u_long frame_code;
    u_long zbuf;
    u_long zbuf_code;
    u_long test;
    u_long test_code;
    u_long tex1;
    u_long tex1_code;
    u_long texa;
    u_long texa_code;
    u_long clamp;
    u_long clamp_code;
    u_long fogcol;
    u_long fogcol_code;
} OS_GS2_REGISTER;

extern OS_GS2_REGISTER os_gs2_register;
extern sceDmaChan *oa_dmagif;

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_DRAW_W_H */
