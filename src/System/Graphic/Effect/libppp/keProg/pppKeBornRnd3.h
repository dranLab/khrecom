#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND3_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND3_H

#include "System/Graphic/Effect/libppp/keProg/KeBornRnd.h"
#include "System/Graphic/Effect/libppp/keProg/pppKeDrct.h"

typedef struct { /* 0x1c */
    pppCDT cdt;
    short IdxVsf;
    u_char Num;
    u_char Pstg;
    u_char Mode;
    char dummy0[3];
    pppParamPObj Pos;
    pppParamPObj Dr;
} PKeBornRnd3;

typedef struct { /* 0x2 */
    u_short NextIdx;
} VKeBornRnd3;

void pppKeBornRnd3_Born(pppPObject *pObj, PKeBornRnd3 *pDat, pppSVECTOR *pVert);
void pppKeBornRnd3(pppPObject *pobj, PKeBornRnd3 *dat, pppCtrlTable *ctbl);
void pppKeBornRnd3Con(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND3_H */
