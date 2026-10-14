#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWSHIPOLY_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWSHIPOLY_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    RYJDRAWSHIPOLY_MATRIX = 0,
    RYJDRAWSHIPOLY_COLOR = 1,
    RYJDRAWSHIPOLY_THIS = 2
};

typedef struct { /* 0x1C */
    pppCDT cdt;
    int nModel;
    unsigned char bDrawMode;
    unsigned char bFog;
    unsigned char bScissor;
    unsigned char bNear;
    unsigned char bInvisible;
    unsigned char bStop;
    unsigned char bFrameNum;
    char dummy0[1];
    float fWidth;
    unsigned char bThin;
    unsigned char bWorld;
    unsigned char bTexS;
    unsigned char bTexT;
    unsigned char bTexX;
    unsigned char bZtest;
} PRyjDrawShipoly;

typedef struct { /* 0xC */
    unsigned int unFrameCt;
    unsigned char bSmp;
    unsigned char bStandby;
    char dummy0[2];
    sceVu0FVECTOR *pvecSmp;
} VRyjDrawShipoly;

void pppRyjDrawShipoly(pppPObject *pPobj, PRyjDrawShipoly *pParam, pppCtrlTable *pCtbl);
void pppRyjDrawDrawShipoly(pppPObject *pPobj, PRyjDrawShipoly *pParam, pppCtrlTable *pCtbl);
void pppRyjDrawShipolyCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjDrawShipolyDes(pppPObject *pPobj, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWSHIPOLY_H */
