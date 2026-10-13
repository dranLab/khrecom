#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAILX_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAILX_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IKeShpTailX_This = 0,
    IKeShpTailX_Color = 1,
    IKeShpTailX_DMatShp = 2,
    IKeShpTailX_KeZCrctShp = 3
};

typedef struct { /* 0x24 */
    pppCDT cdt;
    int Shp;
    unsigned int AnmSpd;
    pppHCVECTOR LastCol;
    float LastScl;
    float Sep;
    unsigned short Num;
    unsigned char fog;
    unsigned char near;
    unsigned char zCrct;
    unsigned char World;
    unsigned char draw_mode;
    unsigned char ztest;
} PKeShpTailX;

typedef struct { /* 0x200 */
    unsigned char StkNum;
    unsigned char Cur;
    unsigned short Cnt;
    unsigned short Seq;
    unsigned short SeqNow;
    char dummy0[8];
    sceVu0FVECTOR Stk[31];
} VKeShpTailX;

void sceVu0SubVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);

void pppKeShpTailX(pppPObject *pobj, PKeShpTailX *dat, pppCtrlTable *ctbl);
void pppKeShpTailXDraw(pppPObject *pobj, PKeShpTailX *dat, pppCtrlTable *ctbl);
void pppKeShpTailXCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAILX_H */
