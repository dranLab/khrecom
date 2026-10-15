#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPEIDRAWSHIPOLY_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPEIDRAWSHIPOLY_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0xa4 */
    pppCDT cdt;
    int Model;
    unsigned char DistMode;
    unsigned char Type;
    unsigned char Stop;
    unsigned char FrameNum;
    float Width;
    unsigned char Thin;
    unsigned char Coordinate;
    unsigned char PolyNumH;
    unsigned char PolyNumV;
    unsigned char ZTest;
    unsigned char AlphaMode;
    unsigned char Fade;
    unsigned char Gradation;
    float GradationCenter;
    unsigned char Centrifugal;
    unsigned char WindMode;
    unsigned char GravityMode;
    char dummy0[1];
    float Dist;
    float DistV;
    float DistA;
    float CentrifugalForce;
    float CentrifugalForceV;
    float CentrifugalForceA;
    pppFVECTOR Wind;
    pppFVECTOR WindV;
    pppFVECTOR WindA;
    pppFVECTOR GPoint;
    pppFVECTOR GPointV;
    pppFVECTOR GPointA;
    float Gravity;
    float GravityV;
    float GravityA;
    float GRange;
    float TexRepeat;
    float TexScrollS;
    float TexScrollT;
    unsigned char TexS;
    unsigned char TexT;
    unsigned char TexX;
    unsigned char UvRandom;
    unsigned char TexDir;
    unsigned char TexMapping;
} PEiDrawShipoly;

typedef struct { /* 0xe0 */
    unsigned int FrameCt;
    unsigned char SmpNum;
    unsigned char Standby;
    unsigned short GetPosCt;
    sceVu0FVECTOR *Smp;
    float CentrifugalForce;
    float CentrifugalForceV;
    float CentrifugalForceA;
    float Dist;
    float DistV;
    float DistA;
    float TexScrollS;
    float TexScrollT;
    char dummy0[4];
    sceVu0FMATRIX CMat;
    sceVu0FVECTOR Wind;
    sceVu0FVECTOR WindV;
    sceVu0FVECTOR WindA;
    sceVu0FVECTOR GPoint;
    sceVu0FVECTOR GPointV;
    sceVu0FVECTOR GPointA;
    float Gravity;
    float GravityV;
    float GravityA;
    void *StartParam;
} VEiDrawShipoly;

void pppEiDrawDrawShipoly(pppPObject *pobj, PEiDrawShipoly *param, pppCtrlTable *ctbl);
void pppEiDrawShipoly(pppPObject *pobj, PEiDrawShipoly *param, pppCtrlTable *ctbl);
void pppEiDrawShipolyCon(pppPObject *pobj, pppCtrlTable *ctbl);
void pppEiDrawShipolyCon2(pppPObject *pobj, pppCtrlTable *ctbl);
void pppEiDrawShipolyDes(pppPObject *pobj, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPEIDRAWSHIPOLY_H */
