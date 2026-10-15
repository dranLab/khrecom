#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_MD0_P3F_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_MD0_P3F_H

#include "md.h"

/* Primitive group header of an OMD model (belongs with OMD_H in md.h). */
typedef struct { /* 0x10 */
    char flag;
    char ptype;
    short pc;
    u_char tno;
    u_char timposx;
    u_char res[2];
    u_word tex0l;
    u_word tex0h;
} OMD_PRIM_H;

void *op_md_draw_path3_fog(OMD_H *mdh, OMD_IN *mdi);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_MD0_P3F_H */
