#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND_H

#include "System/Graphic/Effect/libppp/pppPart.h"
#include "System/Graphic/Effect/libppp/eiProg/pppMove.h"
#include "System/Graphic/Effect/libppp/keProg/KeBornRnd.h"

enum {
    IKeBornRnd_This = 0,
    IKeBornRnd_WMat = 1
};

typedef struct { /* 0x1c */
    pppCDT cdt;
    short IdxVsf;
    short SpdVsf;
    u_char Num;
    u_char Pstg;
    u_char Mode;
    char dummy0[1];
    pppParamPObj Pos;
    pppParamPObj Mv;
} PKeBornRnd;

typedef struct { /* 0x2 */
    u_short NextIdx;
} VKeBornRnd;

void pppKeBornRnd(pppPObject *pobj, PKeBornRnd *dat, pppCtrlTable *ctbl);
void pppKeBornRndCon(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND_H */
