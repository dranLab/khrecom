#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_DRAW_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_DRAW_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "global.h"
#include "draw_w.h"

typedef struct _OS_FOG { /* 0x40 */
    float fFA;
    float fFB;
    float fFogMin;
    float fFogMax;
    float fVoidFA;
    float fVoidFB;
    float fVoidFogMin;
    float fVoidFogMax;
    float fFadeRate;
    float fFadeAdjust;
    float fFadeMin;
    float fFadeMax;
    float fVoidFadeRate;
    float fVoidFadeAdjust;
    float fVoidFadeMin;
    float fVoidFadeMax;
} OS_FOG;

typedef struct tagTM2HEADER { /* 0x10 */
    u_char eye[4];
    u_char ver;
    u_char id;
    u_short info_c;
    u_char res[5];
    u_char timposx;
    u_char flag;
    u_char otp;
} TM2HEADER;

typedef struct tagTM2INFO_R { /* 0x30 */
    u_word info_size;
    u_word clut_size;
    u_word image_size;
    u_short info_h_size;
    u_short clut_c;
    u_char id;
    u_char mipmap_c;
    u_char clut;
    u_char image;
    u_short w;
    u_short h;
    u_word tex0_l;
    u_word tex0_h;
    u_word tex1_l;
    u_word tex1_h;
    u_char absw;
    u_char absh;
    u_char absx;
    u_char absy;
    u_char res[2];
    u_short texclut;
} TM2INFO_R;

extern u_char ot_pixel_format_tm2[];
extern OS_FOG OsFog;
extern u_char FogCol[3];

extern OS_GS2_REGISTER os_gs2_register2; /* draw_w */
extern u_long ppvGsFrame;                /* partMng */
extern int ppvZFrameBase;                /* partMng */
sceDmaChan *sceDmaGetChan(int id);
int rand(void);

/* draw_ws */
extern u_long128 ow_ot_before[16];
extern u_long128 ow_ot[256];
extern u_long128 ow_ot_after[16];

void op_init_draw();
void ops_init_draw_work();
void op_gs2_init();
void op_otz_init();
void op_gs2_init_eff();
void op_ztest_init();
void op_random_seed();
int op_beki(int i);
void op_tm2_load(TM2HEADER *tm2);
void op_clut256_load(u_long128 *cadr, int counter, u_short gsadr, int wait);
void op_oef2_texture_load(OEF2_TEXINFO *ti, int wait);
void op_oef2_clut_load(OEF2_CLUTINFO *ci, int wait);
void op_gs_clear(int gsadr, int gssize);
void op_ot_draw();
void op_ot_add_before(u_long128 *packet, int qsize, int otz, int pri);
void op_ot_add_after(u_long128 *packet, int qsize, int otz, int pri);
void op_ot_add(u_long128 *packet, int qsize, int otz, int pri);
u_long128 *op_flash_ot(u_long128 *packet, u_word rgba, u_char alpha, u_char alphafix);
void ops_init_fog_work();
void op_fog_param_set(float near, float far);
void op_fog_color_set(u_char r, u_char g, u_char b);
void op_fog_clamp_set(u_char min, u_char max);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_DRAW_H */
