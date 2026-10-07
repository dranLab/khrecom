#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDLTS2_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDLTS2_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x24 */
    pppCDT cdt;
    int model;
    float tu;
    float dtu;
    float ddtu;
    float tv;
    float dtv;
    float ddtv;
    u_char draw_mode;
    u_char ztest;
} PDrawMdlTs2;

typedef struct { /* 0x18 */
    float tu;
    float dtu;
    float ddtu;
    float tv;
    float dtv;
    float ddtv;
} VDrawMdlTs2;

void pppDrawMdlTs2Con(pppPObject *pobj, pppCtrlTable *ctbl);
void pppDrawMdlTs2(pppPObject *pobj, PDrawMdlTs2 *p, pppCtrlTable *ctbl);
void pppDrawDrawMdlTs2(pppPObject *pobj, PDrawMdlTs2 *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWMDLTS2_H */
