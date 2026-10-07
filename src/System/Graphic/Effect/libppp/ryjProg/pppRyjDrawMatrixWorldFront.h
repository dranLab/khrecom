#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLDFRONT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLDFRONT_H

#include "System/Graphic/Effect/libppp/pppPart.h"

/* declared in the original pppPart.h; not in the project's pppPart.h yet */
extern sceVu0FVECTOR ppvParLoc;

enum {
    RYJDRAWMATRIXWORLDFRONT_MATRIX = 0,
    RYJDRAWMATRIXWORLDFRONT_THIS = 1
};

typedef struct { /* 0x4 */
    pppCDT cdt;
} PRyjDrawMatrixWorldFront;

typedef struct { /* 0x20 */
    sceVu0FVECTOR vecPos;
    sceVu0FVECTOR vecScl;
} VRyjDrawMatrixWorldFront;

void pppRyjDrawMatrixWorldFrontCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjDrawDrawMatrixWorldFront(pppPObject *pPobj, PRyjDrawMatrixWorldFront *pParam, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWMATRIXWORLDFRONT_H */
