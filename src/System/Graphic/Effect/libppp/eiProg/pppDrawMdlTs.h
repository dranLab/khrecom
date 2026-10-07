#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMDLTS_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMDLTS_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x20 */
    pppCDT cdt;
    int model;
    float tu;
    float dtu;
    float ddtu;
    float tv;
    float dtv;
    float ddtv;
} PDrawMdlTs;

typedef struct { /* 0x18 */
    float tu;
    float dtu;
    float ddtu;
    float tv;
    float dtv;
    float ddtv;
} VDrawMdlTs;

void pppDrawMdlTsCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppDrawMdlTs(pppPObject *pobj, PDrawMdlTs *p, pppCtrlTable *ctbl);
void pppDrawDrawMdlTs(pppPObject *pobj, PDrawMdlTs *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMDLTS_H */
