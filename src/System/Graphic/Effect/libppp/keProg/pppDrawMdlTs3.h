#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDLTS3_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDLTS3_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IDrawMdlTs3_Color = 0,
    IDrawMdlTs3_DMat = 1,
    IDrawMdlTs3_This = 2,
    IDrawMdlTs3_KeZCrct = 3
};

typedef struct { /* 0x28 */
    pppCDT cdt;
    int model;
    float tu;
    float dtu;
    float ddtu;
    float tv;
    float dtv;
    float ddtv;
    u_char draw_mode;
    u_char fog;
    u_char scissor;
    u_char near;
    u_char zCrct;
    u_char ztest;
} PDrawMdlTs3;

typedef struct { /* 0x18 */
    float tu;
    float dtu;
    float ddtu;
    float tv;
    float dtv;
    float ddtv;
} VDrawMdlTs3;

void pppDrawMdlTs3Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppDrawMdlTs3(pppPObject *pobj, PDrawMdlTs3 *p, pppCtrlTable *ctbl);
void pppDrawDrawMdlTs3(pppPObject *pobj, PDrawMdlTs3 *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDLTS3_H */
