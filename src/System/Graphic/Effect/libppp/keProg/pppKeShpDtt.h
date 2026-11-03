#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPDTT_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPDTT_H

#include "System/Graphic/Effect/libppp/partMng.h"

enum {
    IKeShpDtt_This = 0,
    IKeShpDtt_Color = 1,
    IKeShpDtt_KeDMat = 2,
    IKeShpDtt_Point = 3
};

typedef struct { /* 0x1c */
    pppCDT cdt;
    float DttX;
    float SpdX;
    float AccX;
    float SzX;
    float SzY;
    u_char Page;
    u_char FEdg;
} PKeShpDtt;

typedef struct { /* 0xc */
    float DttX;
    float SpdX;
    float AccX;
} VKeShpDtt;

void pppKeShpDtt(pppPObject *pobj, PKeShpDtt *dat, pppCtrlTable *ctbl);
void pppKeShpDttDraw(pppPObject *pobj, PKeShpDtt *dat, pppCtrlTable *ctbl);
void pppKeShpDttCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKESHPDTT_H */
