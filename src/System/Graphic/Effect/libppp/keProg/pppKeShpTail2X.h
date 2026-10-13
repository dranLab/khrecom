#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAIL2X_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAIL2X_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IKeShpTail2X_This = 0,
    IKeShpTail2X_Color = 1,
    IKeShpTail2X_DMatShp = 2,
    IKeShpTail2X_KeZCrctShp = 3
};

typedef struct { /* 0x2c */
    pppCDT cdt;
    int Shp;
    unsigned int AnmSpd;
    float Scl0;
    float Scl1;
    pppCVECTOR Col0;
    pppCVECTOR Col1;
    float Sep;
    unsigned short Num;
    unsigned char RtDraw;
    unsigned char fog;
    unsigned char near;
    unsigned char zCrct;
    unsigned char World;
    unsigned char draw_mode;
    unsigned char ztest;
} PKeShpTail2X;

typedef struct { /* 0x200 */
    unsigned char StkNum;
    unsigned char Cur;
    unsigned short Cnt;
    unsigned short Seq;
    unsigned short SeqNow;
    char dummy0[8];
    sceVu0FVECTOR Stk[31];
} VKeShpTail2X;

void sceVu0SubVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);

void pppKeShpTail2X(pppPObject *pobj, PKeShpTail2X *dat, pppCtrlTable *ctbl);
void pppKeShpTail2XDraw(pppPObject *pobj, PKeShpTail2X *dat, pppCtrlTable *ctbl);
void pppKeShpTail2XCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAIL2X_H */
