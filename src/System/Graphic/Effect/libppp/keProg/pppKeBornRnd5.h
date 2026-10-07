#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND5_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND5_H

#include "System/Graphic/Effect/libppp/keProg/KeBornRnd.h"

typedef struct { /* 0x24 */
    pppCDT cdt;
    short IdxVsf;
    char dummy0[2];
    float Spd;
    u_char Num;
    u_char Wait;
    u_char Mode;
    char dummy1[1];
    pppParamPObj Pos;
    pppParamPObj Mv;
} PKeBornRnd5;

typedef struct { /* 0x4 */
    u_short NextIdx;
    u_char Cnt;
} VKeBornRnd5;

void pppKeBornRnd5_Born(pppPObject *pObj, PKeBornRnd5 *pDat, pppSVECTOR *pVert);
void pppKeBornRnd5(pppPObject *pobj, PKeBornRnd5 *dat, pppCtrlTable *ctbl);
void pppKeBornRnd5Con(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND5_H */
