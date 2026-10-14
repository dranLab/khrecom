#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAIL3X_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAIL3X_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IKeShpTail3X_This = 0,
    IKeShpTail3X_Color = 1,
    IKeShpTail3X_DrawMatrix = 2,
    IKeShpTail3X_KeZCrctShp = 3
};

typedef struct { /* 0x58 */
    pppCDT cdt;
    int Shp;
    unsigned int AnmSpd;
    float Scl0;
    float Scl1;
    float RndScl;
    float Sep;
    unsigned char Num;
    unsigned char RtDraw;
    unsigned short RndRot;
    pppHCVECTOR Col0;
    pppHCVECTOR Col1;
    pppHCVECTOR Col0Mov;
    pppHCVECTOR Col0Acc;
    pppHCVECTOR Col1Mov;
    pppHCVECTOR Col1Acc;
    unsigned char fog;
    unsigned char near;
    unsigned char zCrct;
    unsigned char World;
    unsigned char draw_mode;
    unsigned char ztest;
} PKeShpTail3X;

typedef struct { /* 0x200 */
    pppHCVECTOR Col0;
    pppHCVECTOR Col1;
    pppHCVECTOR Col0Mov;
    pppHCVECTOR Col0Acc;
    pppHCVECTOR Col1Mov;
    pppHCVECTOR Col1Acc;
    sceVu0FVECTOR Stk[28];
    unsigned int Cnt;
    unsigned short Seq;
    unsigned short SeqNow;
    unsigned short Seed;
    unsigned char Cur;
} VKeShpTail3X;

void sceVu0SubVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);

void pppKeShpTail3X(pppPObject *pobj, PKeShpTail3X *dat, pppCtrlTable *ctbl);
void pppKeShpTail3XDraw(pppPObject *pobj, PKeShpTail3X *dat, pppCtrlTable *ctbl);
void pppKeShpTail3XCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppKeShpTail3XCon2(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAIL3X_H */
