#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHMODEL_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHMODEL_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    RYJMEGABIRTHMODEL_MATRIX = 0,
    RYJMEGABIRTHMODEL_COLOR = 1,
    RYJMEGABIRTHMODEL_THIS = 2
};

enum {
    MODE_SPHERE = 0,
    MODE_SPHERE_WOODROT = 1,
    MODE_DONUTS = 2,
    MODE_DONUTS_WOODROT = 3,
    MODE_BOX = 4,
    MODE_BOX_WOODROT = 5,
    MODE_VSF = 6,
    MODE_VSF_WOODROT = 7,
    MODE_VSF_EXP = 8,
    MODE_VSF_EXP_WOODROT = 9,
    MODE_MAX = 10
};

enum {
    UVRND_OFF = 0,
    UVRND_U = 1,
    UVRND_V = 2,
    UVRND_UV = 3,
    UVRND_U_V = 4
};

typedef struct {
    pppCDT cdt;
    int nModel;
    u_char bDrawMode;
    u_char bFog;
    u_char bScissor;
    u_char bNear;
    u_char bInvisible;
    u_char bZcrct;
    char dummy0[2];
    pppFVECTOR ZcrctPos;
    u_short ushNum;
    u_short ushNumOneF;
    u_short ushTiming;
    u_short ushLife;
    u_char bFadeInFrame;
    u_char bFadeOutFrame;
    u_char bMode;
    u_char bDeg;
    float fColMovR;
    float fColMovG;
    float fColMovB;
    float fColMovA;
    float fColAccR;
    float fColAccG;
    float fColAccB;
    float fColAccA;
    u_char bColRndRangeR;
    u_char bColRndRangeG;
    u_char bColRndRangeB;
    u_char bColRndRangeA;
    pppIVECTOR Ang;
    pppIVECTOR AngMov;
    pppIVECTOR AngAcc;
    pppIVECTOR AngRnd;
    pppFVECTOR Scl;
    pppFVECTOR SclMov;
    pppFVECTOR SclAcc;
    pppFVECTOR SclRnd;
    float fU;
    float fUMov;
    float fUAcc;
    float fV;
    float fVMov;
    float fVAcc;
    pppFVECTOR Dir;
    pppFVECTOR Force;
    float fMov;
    float fAcc;
    float fMovRndRange;
    float fForce;
    float fForceAcc;
    float fRangeRadius;
    pppFVECTOR RangeScl;
    u_char bRangeMode;
    u_char bColV;
    u_char bSclRnd;
    u_char bAngRnd;
    u_char bUvRnd;
    u_char bYaxis;
    u_char bWorld;
    char dummy1[1];
    short shVsfNo;
    u_char bZtest;
    u_char bLimitOff;
} PRyjMegaBirthModel;

typedef struct {
    sceVu0FVECTOR vecForce;
    void *pvParticleData;
    void *pvParticleWmat;
    void *pvParticleColor;
    u_int nParticle;
    u_short ushTimingCt;
    u_short ushIct;
} VRyjMegaBirthModel;

void pppRyjMegaBirthModel(pppPObject *pPobj, PRyjMegaBirthModel *pParam, pppCtrlTable *pCtbl);
void pppRyjDrawMegaBirthModel(pppPObject *pPobj, PRyjMegaBirthModel *pParam, pppCtrlTable *pCtbl);
void pppRyjMegaBirthModelCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjMegaBirthModelDes(pppPObject *pPobj, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHMODEL_H */
