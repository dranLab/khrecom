#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND2_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND2_H

#include "System/Graphic/Effect/libppp/keProg/KeBornRnd.h"

typedef struct { /* 0x24 */
    pppCDT cdt;
    short IdxVsf;
    char dummy0[2];
    float Spd;
    u_char Num;
    u_char Pstg;
    u_char Mode;
    char dummy1[1];
    pppParamPObj Pos;
    pppParamPObj Mv;
} PKeBornRnd2;

typedef struct { /* 0x2 */
    u_short NextIdx;
} VKeBornRnd2;

void pppKeBornRnd2_Born(pppPObject *pObj, PKeBornRnd2 *pDat, pppSVECTOR *pVert);
void pppKeBornRnd2(pppPObject *pobj, PKeBornRnd2 *dat, pppCtrlTable *ctbl);
void pppKeBornRnd2Con(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND2_H */
