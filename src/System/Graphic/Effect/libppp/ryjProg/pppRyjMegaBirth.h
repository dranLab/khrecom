#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTH_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTH_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    RYJMEGABIRTH_MATRIX = 0,
    RYJMEGABIRTH_COLOR = 1,
    RYJMEGABIRTH_THIS = 2
};

enum {
    MODE_SPHERE = 0,
    MODE_SPHERE_NOROT = 1,
    MODE_DONUTS = 2,
    MODE_DONUTS_NOROT = 3,
    MODE_BOX = 4,
    MODE_BOX_NOROT = 5,
    MODE_VSF = 6,
    MODE_VSF_NOROT = 7,
    MODE_VSF_EXP = 8,
    MODE_VSF_EXP_NOROT = 9,
    MODE_MAX = 10
};

enum {
    COORD_LOCAL = 0,
    COORD_WORLD = 1,
    COORD_OKIZARI = 2,
    COORD_WOKIZARI = 3
};

enum {
    REVERSE_OFF = 0,
    REVERSE_LR = 1,
    REVERSE_UD = 2,
    REVERSE_LRUD = 3
};

typedef struct {
    pppCDT cdt;
    int nShape;
    u_int unDelta;
    u_char bFog;
    u_char bNear;
    u_char bZcrct;
    u_char bDrawMode;
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
    pppFVECTOR Scl;
    pppFVECTOR SclMov;
    pppFVECTOR SclAcc;
    pppFVECTOR SclRnd;
    float fAng;
    float fAngMov;
    float fAngAcc;
    float fAngRnd;
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
    u_char bCoord;
    u_char bZtest;
    u_char bLimitOff;
    char dummy0[1];
    short shVsfNo;
    u_char bReverseRnd;
    u_char bScaleFadeInFrame;
    u_char bScaleFadeOutFrame;
    char dummy1[3];
    float fForceRndRange;
} PRyjMegaBirth;

typedef struct {
    sceVu0FMATRIX matPar;
    sceVu0FVECTOR vecForce;
    void *pvParticleData;
    void *pvParticleWmat;
    void *pvParticleColor;
    u_int nParticle;
    u_short ushTimingCt;
    u_short ushIct;
} VRyjMegaBirth;

void pppRyjMegaBirth(pppPObject *pobj, PRyjMegaBirth *pParam, pppCtrlTable *pCtbl);
void pppRyjDrawMegaBirth(pppPObject *pobj, PRyjMegaBirth *pParam, pppCtrlTable *pCtbl);
void pppRyjMegaBirthCon(pppPObject *pobj, pppCtrlTable *pCtbl);
void pppRyjMegaBirthDes(pppPObject *pobj, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTH_H */
