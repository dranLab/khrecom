#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHMODELFILTER_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHMODELFILTER_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    RYJMEGABIRTHMODELFILTER_MATRIX = 0,
    RYJMEGABIRTHMODELFILTER_COLOR = 1,
    RYJMEGABIRTHMODELFILTER_THIS = 2
};

enum {
    REVERSERND_OFF = 0,
    REVERSERND_LR = 1,
    REVERSERND_UD = 2,
    REVERSERND_LRUD = 3
};

typedef struct {
    pppCDT cdt;
    int nModel;
    u_char bFog;
    u_char bScissor;
    u_short ushNum;
    u_short ushNumOneF;
    u_short ushTiming;
    u_short ushLife;
    u_char bFadeInFrame;
    u_char bFadeOutFrame;
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
    float fURnd;
    float fV;
    float fVMov;
    float fVAcc;
    float fVRnd;
    pppFVECTOR Dir;
    float fMov;
    float fAcc;
    float fMovRndRange;
    float fNearFadeStart;
    float fNearFadeLen;
    float fFarFadeStart;
    float fFarFadeLen;
    float fAppearScaleY;
    float fAppearOffsetY;
    float fCollisionRadius;
    float fCollisionFadeLen;
    u_char bCollision;
    u_char bAppearMode;
    u_char bColV;
    u_char bSclRnd;
    u_char bAngRnd;
    u_char bUvRnd;
    u_char bReverseRnd;
} PRyjMegaBirthModelFilter;

typedef struct {
    void *pvParticleData;
    void *pvParticleColor;
    u_int nParticle;
    float fAlphaScale;
    u_short ushTimingCt;
    u_char bActive;
} VRyjMegaBirthModelFilter;

void pppRyjMegaBirthModelFilter(pppPObject *pPobj, PRyjMegaBirthModelFilter *pParam, pppCtrlTable *pCtbl);
void pppRyjDrawMegaBirthModelFilter(pppPObject *pPobj, PRyjMegaBirthModelFilter *pParam, pppCtrlTable *pCtbl);
void pppRyjMegaBirthModelFilterSta(pppMngSt *pMng);
void pppRyjMegaBirthModelFilterCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjMegaBirthModelFilterDes(pppPObject *pPobj, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHMODELFILTER_H */
