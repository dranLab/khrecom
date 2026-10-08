#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND4_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND4_H

#include "System/Graphic/Effect/libppp/keProg/KeBornRnd.h"
#include "System/Graphic/Effect/libppp/keProg/pppKeDrct.h"

enum {
    IKeBornRnd4_This = 0,
    IKeBornRnd4_WMat = 1
};

typedef struct { /* 0x28 */
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
    pppParamPObj Dr;
} PKeBornRnd4;

typedef struct { /* 0x2 */
    u_short NextIdx;
} VKeBornRnd4;

void pppKeBornRnd4_Born(pppPObject *pObj, PKeBornRnd4 *pDat, pppSVECTOR *pVert);
void pppKeBornRnd4(pppPObject *pobj, PKeBornRnd4 *dat, pppCtrlTable *ctbl);
void pppKeBornRnd4Con(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND4_H */
