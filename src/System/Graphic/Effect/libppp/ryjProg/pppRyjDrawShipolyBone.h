#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWSHIPOLYBONE_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWSHIPOLYBONE_H

#include "System/Graphic/Effect/libppp/pppPart.h"

enum {
    RYJDRAWSHIPOLYBONE_COLOR = 0,
    RYJDRAWSHIPOLYBONE_THIS = 1
};

typedef struct { /* 0x5 */
    unsigned char bThin;
    unsigned char bTexS;
    unsigned char bTexT;
    unsigned char bTexX;
    unsigned char bZtest;
} PRyjDrawShipolyBoneTex;

typedef struct { /* 0x2C */
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
    unsigned short ushBoneSta;
    unsigned short ushBoneTypeSta;
    float fRatioSta;
    unsigned short ushBoneEnd;
    unsigned short ushBoneTypeEnd;
    float fRatioEnd;
    PRyjDrawShipolyBoneTex sTex;
    char dummy1[1];
    short shBoneDirectSta;
    short shBoneDirectEnd;
} PRyjDrawShipolyBoneOld;

typedef struct { /* 0x14 */
    pppCDT cdt;
    int nModel;
    unsigned char bDrawMode;
    unsigned char bFog;
    unsigned char bScissor;
    unsigned char bNear;
    unsigned char bInvisible;
    unsigned char bStop;
    unsigned char bFrameNum;
    PRyjDrawShipolyBoneTex sTex;
} PRyjDrawShipolyBone;

typedef struct { /* 0x30 */
    unsigned int unFrameCt;
    unsigned char bSmp;
    unsigned char bStandby;
    char dummy0[2];
    sceVu0FVECTOR *pvecSmp;
    char dummy1[4];
    sceVu0FVECTOR avecBonePos[2];
} VRyjDrawShipolyBone;

void pppRyjDrawShipolyBone(pppPObject *pPobj, PRyjDrawShipolyBone *pParam, pppCtrlTable *pCtbl);
void pppRyjDrawDrawShipolyBone(pppPObject *pPobj, PRyjDrawShipolyBone *pParam, pppCtrlTable *pCtbl);
void pppRyjDrawShipolyBoneCon(pppPObject *pPobj, pppCtrlTable *pCtbl);
void pppRyjDrawShipolyBoneDes(pppPObject *pPobj, pppCtrlTable *pCtbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_PPPRYJDRAWSHIPOLYBONE_H */
