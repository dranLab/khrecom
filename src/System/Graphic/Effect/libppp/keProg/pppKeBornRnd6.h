#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND6_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND6_H

#include "System/Graphic/Effect/libppp/keProg/KeBornRnd.h"

enum {
    IKeBornRnd6_This = 0,
    IKeBornRnd6_Matrix = 1
};

typedef struct { /* 0x1c */
    pppCDT cdt;
    short IdxVsf;
    u_char Num;
    u_char Wait;
    u_char Mode;
    char dummy0[3];
    pppParamPObj Pos;
    pppParamPObj Ang;
} PKeBornRnd6;

typedef struct { /* 0x4 */
    u_short NextIdx;
    u_char Cnt;
} VKeBornRnd6;

int _KeGetDeg2(float *xy, float *deg);
void pppKeBornRnd6_Born(pppPObject *pObj, PKeBornRnd6 *pDat, pppSVECTOR *pVert);
void pppKeBornRnd6(pppPObject *pobj, PKeBornRnd6 *dat, pppCtrlTable *ctbl);
void pppKeBornRnd6Con(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_PPPKEBORNRND6_H */
