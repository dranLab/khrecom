#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHFILTER_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHFILTER_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    RYJMEGABIRTHFILTER_MATRIX = 0,
    RYJMEGABIRTHFILTER_COLOR = 1,
    RYJMEGABIRTHFILTER_THIS = 2
};

typedef struct { /* 0xbc */
    pppCDT cdt;
    int nShape;
    u_int unDelta;
    u_char bFog;
    char dummy0[1];
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
    pppFVECTOR Scl;
    pppFVECTOR SclMov;
    pppFVECTOR SclAcc;
    pppFVECTOR SclRnd;
    float fAng;
    float fAngMov;
    float fAngAcc;
    float fAngRnd;
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
} PRyjMegaBirthFilter;

typedef struct { /* 0x14 */
    void *pvParticleData;
    void *pvParticleColor;
    u_int nParticle;
    float fAlphaScale;
    u_short ushTimingCt;
    u_char bActive;
} VRyjMegaBirthFilter;

void pppRyjMegaBirthFilter(pppPObject *pPobj, PRyjMegaBirthFilter *pParam, pppCtrlTable *pCtbl);
void pppRyjDrawMegaBirthFilter(pppPObject *pPobj, PRyjMegaBirthFilter *pParam, pppCtrlTable *pCtbl);
void pppRyjMegaBirthFilterSta(pppMngSt *pMng);
void pppRyjMegaBirthFilterCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjMegaBirthFilterDes(pppPObject *pPobj, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJMEGABIRTHFILTER_H */
