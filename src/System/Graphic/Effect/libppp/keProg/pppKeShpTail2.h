#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAIL2_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAIL2_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IKeShpTail2_This = 0,
    IKeShpTail2_Color = 1,
    IKeShpTail2_DMatShp = 2
};

typedef struct { /* 0x28 */
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
    unsigned char World;
    unsigned char draw_mode;
    unsigned char ztest;
} PKeShpTail2;

typedef struct { /* 0x200 */
    unsigned char StkNum;
    unsigned char Cur;
    unsigned short Cnt;
    unsigned short Seq;
    unsigned short SeqNow;
    char dummy0[8];
    sceVu0FVECTOR Stk[31];
} VKeShpTail2;

void sceVu0SubVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);

void pppKeShpTail2(pppPObject *pobj, PKeShpTail2 *dat, pppCtrlTable *ctbl);
void pppKeShpTail2Draw(pppPObject *pobj, PKeShpTail2 *dat, pppCtrlTable *ctbl);
void pppKeShpTail2Con(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPTAIL2_H */
