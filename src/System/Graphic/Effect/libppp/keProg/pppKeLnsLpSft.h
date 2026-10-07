#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKELNSLPSFT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKELNSLPSFT_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/keProg/KeLns.h"

typedef struct { /* 0xc */
    pppCDT cdt;
    float LWid;
    float Pow;
} PKeLnsLpSft;

typedef struct { /* 0x8 */
    float LWid;
    float Pow;
} VKeLnsLpSft;

void pppKeLnsLpSft(pppPObject *pobj, PKeLnsLpSft *dat, pppCtrlTable *ctbl);
void pppKeLnsLpSftCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKELNSLPSFT_H */
