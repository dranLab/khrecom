#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDCHAR_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDCHAR_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0xC */
    pppCDT cdt;
    int pvoff;
    unsigned char rw;
    unsigned char rct;
} PRandChar;

typedef struct { /* 0x4 */
    float r0;
} VRandChar;

void pppRandChar(pppPObject *pobj, PRandChar *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDCHAR_H */
