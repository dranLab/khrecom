#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMDL_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMDL_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x8 */
    pppCDT cdt;
    int model;
} PDrawMdl;

void pppDrawMdl(pppPObject *pobj, PDrawMdl *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMDL_H */
