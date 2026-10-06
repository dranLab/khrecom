// PARTIAL: added for System/Graphic/Effect/libppp/ryjProg/pppRyjMegaPlaceLamp
#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGAPLACE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGAPLACE_H

#include "System/Graphic/Effect/libppp/pppPart.h"

/* Shared by pppRyjMegaPlace and its Shape/Model/Lamp programs. */

typedef struct { /* 0x4 */
    pppCDT cdt;
} PRyjMegaPlace;

typedef struct { /* 0x18 */
    unsigned int unShapeNum;
    unsigned int unModelNum;
    unsigned int unLampNum;
    void *pvShape;
    void *pvModel;
    void *pvLamp;
} VRyjMegaPlace;

typedef struct { /* 0x40 */
    pppCDT cdt;
    unsigned int unDppId;
    int nShape;
    unsigned int unDelta;
    unsigned char bFog;
    unsigned char bNear;
    unsigned char bDrawMode;
    unsigned char bZcrct;
    pppFVECTOR ZcrctPos;
    pppFVECTOR Pos;
    pppFVECTOR Scl;
    pppCVECTOR Col;
    unsigned char bZtest;
} PRyjMegaPlaceShape;

typedef struct { /* 0x4c */
    pppCDT cdt;
    unsigned int unDppId;
    int nModel;
    unsigned char bDrawMode;
    unsigned char bFog;
    unsigned char bScissor;
    unsigned char bNear;
    unsigned char bInvisible;
    unsigned char bZcrct;
    char dummy0[2];
    pppFVECTOR ZcrctPos;
    pppFVECTOR Pos;
    pppIVECTOR Ang;
    pppFVECTOR Scl;
    pppCVECTOR Col;
    unsigned char bZtest;
} PRyjMegaPlaceModel;

typedef struct { /* 0x48 */
    pppCDT cdt;
    unsigned int unDppId;
    int nShape;
    unsigned int unDelta;
    unsigned char bFog;
    unsigned char bNear;
    unsigned char bZcrct;
    char dummy0[1];
    pppFVECTOR ZcrctPos;
    unsigned char bDrawMode;
    char dummy1[3];
    pppFVECTOR Pos;
    pppFVECTOR Scl;
    pppCVECTOR Col;
    unsigned char bFlickerTimeA;
    unsigned char bFlickerTimeB;
    unsigned char bFlickerFrame;
    unsigned char bFlickerRange;
    unsigned char bZtest;
} PRyjMegaPlaceLamp;

int entry_megaplace_shape(void *pvMegaPlace, PRyjMegaPlaceShape *pMegaPlaceShape);
int entry_megaplace_model(void *pvMegaPlace, PRyjMegaPlaceModel *pMegaPlaceModel);
int entry_megaplace_lamp(void *pvMegaPlace, PRyjMegaPlaceLamp *pMegaPlaceLamp);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGAPLACE_H */
