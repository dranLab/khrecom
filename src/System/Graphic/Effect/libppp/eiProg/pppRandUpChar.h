#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDUPCHAR_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDUPCHAR_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0xC */
    pppCDT cdt;
    int pvoff;
    unsigned char rw;
    unsigned char rct;
} PRandUpChar;

typedef struct { /* 0x4 */
    float r0;
} VRandUpChar;

void pppRandUpChar(pppPObject *pobj, PRandUpChar *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPRANDUPCHAR_H */
