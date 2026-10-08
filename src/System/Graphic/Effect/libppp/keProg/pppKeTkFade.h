#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKETKFADE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKETKFADE_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    IKeTkFade_This = 0
};

typedef struct { /* 0x1c */
    pppCDT cdt;
    int Mdl;
    float Lv;
    float LvSpd;
    float LvAcc;
    pppCVECTOR ColBs;
    pppCVECTOR ColSs;
} PKeTkFade;

typedef struct { /* 0x10 */
    float Lv;
    float LvSpd;
    float LvAcc;
    u_int VAdr;
} VKeTkFade;

void pppKeTkFadeDraw(pppPObject *pobj, PKeTkFade *dat, pppCtrlTable *ctbl);
void pppKeTkFadeCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKETKFADE_H */
