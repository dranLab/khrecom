#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAIL_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAIL_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IKeShpTail_This = 0,
    IKeShpTail_Color = 1,
    IKeShpTail_DMatShp = 2
};

typedef struct { /* 0x24 */
    pppCDT cdt;
    int Shp;
    unsigned int AnmSpd;
    pppHCVECTOR LastCol;
    float LastScl;
    float Sep;
    unsigned short Num;
    unsigned char World;
    unsigned char draw_mode;
    unsigned char ztest;
} PKeShpTail;

typedef struct { /* 0x200 */
    unsigned char StkNum;
    unsigned char Cur;
    unsigned short Cnt;
    unsigned short Seq;
    unsigned short SeqNow;
    char dummy0[8];
    sceVu0FVECTOR Stk[31];
} VKeShpTail;

void sceVu0SubVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);

void pppKeShpTail(pppPObject *pobj, PKeShpTail *dat, pppCtrlTable *ctbl);
void pppKeShpTailDraw(pppPObject *pobj, PKeShpTail *dat, pppCtrlTable *ctbl);
void pppKeShpTailCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAIL_H */
