#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWSHAPE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWSHAPE_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x10 */
    pppCDT cdt;
    int shape;
    unsigned int delta;
    unsigned char draw_mode;
    unsigned char ztest;
} PDrawShape;

typedef struct { /* 0x6 */
    unsigned short wait;
    unsigned short seq;
    unsigned short seq_now;
} VDrawShape;

void pppDrawShapeConstruct(pppPObject *pobj, pppCtrlTable *ctbl);
void pppDrawShape(pppPObject *pobj, PDrawShape *p, pppCtrlTable *ctbl);
void pppDrawDrawShape(pppPObject *pobj, PDrawShape *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWSHAPE_H */
