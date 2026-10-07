#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_SNOPROG_PPPSNOSCALEALL_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_SNOPROG_PPPSNOSCALEALL_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x34 */
    pppCDT cdt;
    pppFVECTOR scl;
    pppFVECTOR dscl;
    pppFVECTOR ddscl;
    float all;
    float dall;
    float ddall;
} PSnoScaleAll;

typedef struct { /* 0x3c */
    pppFVECTOR scl;
    pppFVECTOR dscl;
    pppFVECTOR ddscl;
    pppFVECTOR scl0;
    float all;
    float dall;
    float ddall;
} VSnoScaleAll;

void pppMemClear(void *dis, int bytes);

void pppSnoScaleAll(pppPObject *pobj, PSnoScaleAll *p, pppCtrlTable *ctbl);
void pppSnoScaleAllCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_SNOPROG_PPPSNOSCALEALL_H */
