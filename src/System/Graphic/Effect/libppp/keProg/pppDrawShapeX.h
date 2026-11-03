#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWSHAPEX_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWSHAPEX_H

#include "System/Graphic/Effect/libppp/partMng.h"

enum {
    IDrawShapeX_This = 0,
    IDrawShapeX_Color = 1,
    IDrawShapeX_DMatShp = 2,
    IDrawShapeX_KeZCrctShp = 3
};

typedef struct { /* 0x14 */
    pppCDT cdt;
    int shape;
    unsigned int delta;
    unsigned char fog;
    unsigned char near;
    unsigned char zCrct;
    unsigned char draw_mode;
    unsigned char ztest;
} PDrawShapeX;

typedef struct { /* 0x6 */
    unsigned short wait;
    unsigned short seq;
    unsigned short seq_now;
} VDrawShapeX;

void pppDrawShapeXConstruct(pppPObject *pobj, pppCtrlTable *ctbl);
void pppDrawShapeX(pppPObject *pobj, PDrawShapeX *p, pppCtrlTable *ctbl);
void pppDrawDrawShapeX(pppPObject *pobj, PDrawShapeX *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPDRAWSHAPEX_H */
