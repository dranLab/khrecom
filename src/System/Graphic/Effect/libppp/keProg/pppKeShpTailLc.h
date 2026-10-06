#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAILLC_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAILLC_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct {
    unsigned char StkNum;
    unsigned char Cur;
    unsigned short Cnt;
    unsigned short Seq;
    unsigned short SeqNow;
    unsigned char Use;
    char dummy0[7];
    sceVu0FVECTOR Stk[31];
    sceVu0FMATRIX PreMat;
} VKeShpTailLc;

/* dat is a PKeShpTailLc* in the original (pppCVECTOR/pppParamPObj not in pppPart.h yet) */
void pppKeShpTailLc(pppPObject *pobj, void *dat, pppCtrlTable *ctbl);
void pppKeShpTailLcDraw(pppPObject *pobj, void *dat, pppCtrlTable *ctbl);
void pppKeShpTailLcCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAILLC_H */
