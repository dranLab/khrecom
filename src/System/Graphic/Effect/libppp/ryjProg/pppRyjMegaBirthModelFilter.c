#include "pppRyjMegaBirthModelFilter.h"
#include "System/Graphic/Effect/libppp/pppAlloc.h"
#include "System/Graphic/Effect/libppp/draw/md.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"

extern sceVu0FMATRIX ppvCameraMatrixInv;
extern sceVu0FVECTOR ppvAt;
extern sceVu0FMATRIX ppvUnitMatrix;
void *memset(void *s, int c, unsigned int n);
void sceVu0SubVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);
void sceVu0ScaleVectorXYZ(sceVu0FVECTOR v0, sceVu0FVECTOR v1, float s);
void sceVu0RotMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1, sceVu0FVECTOR rot);

typedef struct _PARTICLE_DATA { /* 0xa0 */
    sceVu0FVECTOR vecPos;
    sceVu0FVECTOR vecDir;
    u_short ushLife;
    char chNoiseR;
    char chNoiseG;
    char chNoiseB;
    char chNoiseA;
    int anAng[3];
    int anAngV[3];
    int anAngRnd[3];
    float fAngV;
    float fAngRnd;
    float afScl[3];
    float afSclV[3];
    float afSclRnd[3];
    float fVelocity;
    float fU;
    float fV;
    float fUv;
    float fVv;
    float fUrnd;
    float fVrnd;
    float fAlphaScale;
    u_char bFade;
    struct {
        u_char NoDraw : 1;
        u_char FadeIn : 1;
        u_char FadeOut : 1;
    } bFlag;
} PARTICLE_DATA;

typedef struct _PARTICLE_COLOR { /* 0x20 */
    sceVu0FVECTOR vecCol;
    sceVu0FVECTOR vecColV;
} PARTICLE_COLOR;
/* |v.xyz| */
#define VEC_LENGTH(len, v)                                   \
    __asm__ __volatile__("lqc2 $vf10, 0(%1)\n"               \
                         "\tvmul.xyz $vf10, $vf10, $vf10\n"   \
                         "\tvaddx.y $vf10, $vf10, $vf10x\n"   \
                         "\tvaddy.z $vf10, $vf10, $vf10y\n"   \
                         "\tvsqrt $Q, $vf10z\n"               \
                         "\tvwaitq\n"                         \
                         "\tvaddq.x $vf10, $vf0, $Q\n"        \
                         "\tqmfc2.ni %0, $vf10"               \
                         : "=r"(len) : "r"(v))

static sceVu0FVECTOR g_vecCamDir;


static float get_rand() {
    float r;

    PPP_RAND_F(r);
    return r;
}

static char get_noise(u_char bRange) {
    return bRange * 2 * get_rand() - bRange / 2;
}

static char alloc_check(VRyjMegaBirthModelFilter *pVal, PRyjMegaBirthModelFilter *pParam) {
    if (!pVal->pvParticleData) {
        return 0;
    }
    if (pParam->bColV && !pVal->pvParticleColor) {
        return 0;
    }
    return 1;
}

static void init_param(PRyjMegaBirthModelFilter *pParam, PARTICLE_DATA *pPdat, PARTICLE_COLOR *pPcol) {
    int i;

    pPdat->chNoiseR = get_noise(pParam->bColRndRangeR);
    pPdat->chNoiseG = get_noise(pParam->bColRndRangeG);
    pPdat->chNoiseB = get_noise(pParam->bColRndRangeB);
    pPdat->chNoiseA = get_noise(pParam->bColRndRangeA);
    if (pParam->bFadeInFrame) {
        pPdat->bFade = pParam->bFadeInFrame;
        pPdat->bFlag.FadeIn = 1;
    }
    pPdat->anAng[0] = pParam->Ang.x;
    pPdat->anAng[1] = pParam->Ang.y;
    pPdat->anAng[2] = pParam->Ang.z;
    pPdat->anAngV[0] = pParam->AngMov.x;
    pPdat->anAngV[1] = pParam->AngMov.y;
    pPdat->anAngV[2] = pParam->AngMov.z;
    if (pParam->bAngRnd) {
        if (pParam->bAngRnd & 0x20) {
            pPdat->anAngRnd[0] = pPdat->anAngRnd[1] = pPdat->anAngRnd[2] = get_rand() * pParam->AngRnd.x;
            if (pParam->bAngRnd & 1 && pParam->bAngRnd & 2) {
                if (get_rand() > 0.5) {
                    for (i = 0; i < 3; i++) {
                        pPdat->anAngRnd[i] = -pPdat->anAngRnd[i];
                    }
                }
            } else if (pParam->bAngRnd & 2) {
                for (i = 0; i < 3; i++) {
                    pPdat->anAngRnd[i] = -pPdat->anAngRnd[i];
                }
            }
        } else {
            pPdat->anAngRnd[0] = get_rand() * pParam->AngRnd.x;
            pPdat->anAngRnd[1] = get_rand() * pParam->AngRnd.y;
            pPdat->anAngRnd[2] = get_rand() * pParam->AngRnd.z;
            if (pParam->bAngRnd & 1 && pParam->bAngRnd & 2) {
                for (i = 0; i < 3; i++) {
                    if (get_rand() > 0.5) {
                        pPdat->anAngRnd[i] = -pPdat->anAngRnd[i];
                    }
                }
            } else if (pParam->bAngRnd & 2) {
                for (i = 0; i < 3; i++) {
                    pPdat->anAngRnd[i] = -pPdat->anAngRnd[i];
                }
            }
        }
    }
    if (pParam->bAngRnd & 4) {
        for (i = 0; i < 3; i++) {
            pPdat->anAng[i] += pPdat->anAngRnd[i];
        }
    }
    if (pParam->bAngRnd & 8) {
        for (i = 0; i < 3; i++) {
            pPdat->anAngV[i] += pPdat->anAngRnd[i];
        }
    }
    for (i = 0; i < 3; i++) {
        if (pPdat->anAng[i] > 0x7FFF || pPdat->anAng[i] < -0x8000) {
            pPdat->anAng[i] %= 0x8000;
        }
    }
    pPdat->afScl[0] = pParam->Scl.x;
    pPdat->afScl[1] = pParam->Scl.y;
    pPdat->afScl[2] = pParam->Scl.z;
    pPdat->afSclV[0] = pParam->SclMov.x;
    pPdat->afSclV[1] = pParam->SclMov.y;
    pPdat->afSclV[2] = pParam->SclMov.z;
    if (pParam->bSclRnd) {
        if (pParam->bSclRnd & 0x20) {
            pPdat->afSclRnd[0] = pPdat->afSclRnd[1] = pPdat->afSclRnd[2] = get_rand() * pParam->SclRnd.x;
            if (pParam->bSclRnd & 1 && pParam->bSclRnd & 2) {
                if (get_rand() > 0.5) {
                    for (i = 0; i < 3; i++) {
                        pPdat->afSclRnd[i] = -pPdat->afSclRnd[i];
                    }
                }
            } else if (pParam->bSclRnd & 2) {
                for (i = 0; i < 3; i++) {
                    pPdat->afSclRnd[i] = -pPdat->afSclRnd[i];
                }
            }
        } else {
            pPdat->afSclRnd[0] = get_rand() * pParam->SclRnd.x;
            pPdat->afSclRnd[1] = get_rand() * pParam->SclRnd.y;
            pPdat->afSclRnd[2] = get_rand() * pParam->SclRnd.z;
            if (pParam->bSclRnd & 1 && pParam->bSclRnd & 2) {
                for (i = 0; i < 3; i++) {
                    if (get_rand() > 0.5) {
                        pPdat->afSclRnd[i] = -pPdat->afSclRnd[i];
                    }
                }
            } else if (pParam->bSclRnd & 2) {
                for (i = 0; i < 3; i++) {
                    pPdat->afSclRnd[i] = -pPdat->afSclRnd[i];
                }
            }
        }
    }
    if (pParam->bSclRnd & 4) {
        for (i = 0; i < 3; i++) {
            pPdat->afScl[i] += pPdat->afSclRnd[i];
        }
    }
    if (pParam->bSclRnd & 8) {
        for (i = 0; i < 3; i++) {
            pPdat->afSclV[i] += pPdat->afSclRnd[i];
        }
    }
    pPdat->fVelocity = pParam->fMov * 0.01f;
    if (pParam->fMovRndRange != 0.0f) {
        float fMovRndRange = pParam->fMovRndRange * 0.01f;

        pPdat->fVelocity += fMovRndRange * 2.0f * get_rand() - fMovRndRange;
    }
    pPdat->fU = pParam->fU;
    pPdat->fV = pParam->fV;
    pPdat->fUv = pParam->fUMov;
    pPdat->fVv = pParam->fVMov;
    if (pParam->bUvRnd) {
        if (pParam->bUvRnd & 0x20) {
            pPdat->fUrnd = pPdat->fVrnd = get_rand() * pParam->fURnd;
            if (pParam->bUvRnd & 1 && pParam->bUvRnd & 2) {
                if (get_rand() > 0.5) {
                    pPdat->fUrnd = -pPdat->fUrnd;
                    pPdat->fVrnd = -pPdat->fVrnd;
                }
            } else if (pParam->bUvRnd & 2) {
                pPdat->fVrnd = pPdat->fUrnd = -pPdat->fUrnd;
            }
        } else {
            pPdat->fUrnd = get_rand() * pParam->fURnd;
            pPdat->fVrnd = get_rand() * pParam->fVRnd;
            if (pParam->bUvRnd & 1 && pParam->bUvRnd & 2) {
                if (get_rand() > 0.5) pPdat->fUrnd = -pPdat->fUrnd;
                if (get_rand() > 0.5) pPdat->fVrnd = -pPdat->fVrnd;
            } else if (pParam->bUvRnd & 2) {
                pPdat->fUrnd = -pPdat->fUrnd;
                pPdat->fVrnd = -pPdat->fVrnd;
            }
        }
    }
    if (pParam->bUvRnd & 4) {
        pPdat->fU += pPdat->fUrnd;
        pPdat->fV += pPdat->fVrnd;
    }
    if (pParam->bUvRnd & 8) {
        pPdat->fUv += pPdat->fUrnd;
        pPdat->fVv += pPdat->fVrnd;
    }
    if (pParam->ushLife) {
        pPdat->ushLife = pParam->ushLife;
    } else {
        pPdat->ushLife = 0xFFFF;
    }
    if (pPcol) {
        pPcol->vecColV[0] = pParam->fColMovR;
        pPcol->vecColV[1] = pParam->fColMovG;
        pPcol->vecColV[2] = pParam->fColMovB;
        pPcol->vecColV[3] = pParam->fColMovA;
    }
    pPdat->fAlphaScale = 0.0f;
    switch (pParam->bReverseRnd) {
    case REVERSERND_LR:
        if (get_rand() > 0.5f) pPdat->afScl[0] = -pPdat->afScl[0];
        break;
    case REVERSERND_UD:
        if (get_rand() > 0.5f) pPdat->afScl[1] = -pPdat->afScl[1];
        break;
    case REVERSERND_LRUD:
        if (get_rand() > 0.5f) pPdat->afScl[0] = -pPdat->afScl[0];
        if (get_rand() > 0.5f) pPdat->afScl[1] = -pPdat->afScl[1];
        break;
    }
}

static void birth(VRyjMegaBirthModelFilter *pVal, PRyjMegaBirthModelFilter *pParam, PARTICLE_DATA *pPdat, PARTICLE_COLOR *pPcol) {
    sceVu0FVECTOR vecDir;
    float fFarFadeStart = pParam->fFarFadeStart * 0.01f;
    float fFarFadeLen = pParam->fFarFadeLen * 0.01f;
    float fAppearRadius;
    float fLen;

    memset(pPdat, 0, sizeof(PARTICLE_DATA));
    if (pPcol) {
        memset(pPcol, 0, sizeof(PARTICLE_COLOR));
    }
    vecDir[0] = pParam->Dir.x;
    vecDir[1] = pParam->Dir.y;
    vecDir[2] = pParam->Dir.z;
    vecDir[3] = 1.0f;
    sceVu0Normalize(pPdat->vecDir, vecDir);
    fAppearRadius = fFarFadeStart + fFarFadeLen;
    do {
        pPdat->vecPos[0] = fAppearRadius * 2.0f * get_rand() - fAppearRadius;
        pPdat->vecPos[1] = fAppearRadius * 2.0f * get_rand() - fAppearRadius;
        pPdat->vecPos[2] = fAppearRadius * 2.0f * get_rand() - fAppearRadius;
        VEC_LENGTH(fLen, pPdat->vecPos);
    } while (fLen > fAppearRadius);
    pPdat->vecPos[3] = 1.0f;
    sceVu0ApplyMatrix(pPdat->vecPos, ppvCameraMatrixInv, pPdat->vecPos);
    init_param(pParam, pPdat, pPcol);
}

static void calc(VRyjMegaBirthModelFilter *pVal, PRyjMegaBirthModelFilter *pParam, PARTICLE_DATA *pPdat, VColor *pCol, PARTICLE_COLOR *pPcol) {
    sceVu0FVECTOR vecDist;
    sceVu0FVECTOR vecDir;
    sceVu0FVECTOR vec;
    float fLen;
    int i;
    /* not in the original debug info: pins the fFarFadeStart load to f2 (as in pppRyjMegaBirthFilter) */
    register float fTmp asm("$f2") = pParam->fFarFadeStart;
    float fFarFadeStart = fTmp * 0.01f;
    float fFarFadeLen = pParam->fFarFadeLen * 0.01f;
    float fNearFadeLen = pParam->fNearFadeLen * 0.01f;

    if (pPcol) {
        pPcol->vecCol[0] += pPcol->vecColV[0];
        pPcol->vecCol[1] += pPcol->vecColV[1];
        pPcol->vecCol[2] += pPcol->vecColV[2];
        pPcol->vecCol[3] += pPcol->vecColV[3];
        pPcol->vecColV[0] += pParam->fColAccR;
        pPcol->vecColV[1] += pParam->fColAccG;
        pPcol->vecColV[2] += pParam->fColAccB;
        pPcol->vecColV[3] += pParam->fColAccA;
    }
    for (i = 0; i < 3; i++) {
        pPdat->anAng[i] += pPdat->anAngV[i];
    }
    if (pParam->bAngRnd & 0x10) {
        pPdat->anAngV[0] += pParam->AngAcc.x + pPdat->anAngRnd[0];
        pPdat->anAngV[1] += pParam->AngAcc.y + pPdat->anAngRnd[1];
        pPdat->anAngV[2] += pParam->AngAcc.z + pPdat->anAngRnd[2];
    } else {
        pPdat->anAngV[0] += pParam->AngAcc.x;
        pPdat->anAngV[1] += pParam->AngAcc.y;
        pPdat->anAngV[2] += pParam->AngAcc.z;
    }
    for (i = 0; i < 3; i++) {
        while (pPdat->anAng[i] > 0x7FFF) pPdat->anAng[i] -= 0x10000;
    }
    for (i = 0; i < 3; i++) {
        pPdat->afScl[i] += pPdat->afSclV[i];
    }
    if (pParam->bSclRnd & 0x10) {
        pPdat->afSclV[0] += pParam->SclAcc.x + pPdat->afSclRnd[0];
        pPdat->afSclV[1] += pParam->SclAcc.y + pPdat->afSclRnd[1];
        pPdat->afSclV[2] += pParam->SclAcc.z + pPdat->afSclRnd[2];
    } else {
        pPdat->afSclV[0] += pParam->SclAcc.x;
        pPdat->afSclV[1] += pParam->SclAcc.y;
        pPdat->afSclV[2] += pParam->SclAcc.z;
    }
    pPdat->fVelocity += pParam->fAcc * 0.01f;
    sceVu0ScaleVectorXYZ(vecDir, pPdat->vecDir, pPdat->fVelocity);
    sceVu0AddVector(pPdat->vecPos, pPdat->vecPos, vecDir);
    pPdat->vecPos[3] = 1.0f;
    sceVu0SubVector(vecDist, pPdat->vecPos, ppvEye);
    sceVu0Normalize(vecDir, vecDist);
    vecDir[3] = 1.0f;
    VEC_LENGTH(fLen, vecDist);
    pPdat->fU += pPdat->fUv;
    pPdat->fV += pPdat->fVv;
    pPdat->fUv += pParam->fUAcc;
    pPdat->fVv += pParam->fVAcc;
    while (pPdat->fU > 2048.0f) pPdat->fU -= 2048.0f;
    while (pPdat->fV > 2048.0f) pPdat->fV -= 2048.0f;
    if (pPdat->fUv > 2048.0f) pPdat->fUv = 2048.0f;
    if (pPdat->fVv > 2048.0f) pPdat->fVv = 2048.0f;
    if (pParam->ushLife != 0 && pParam->ushLife != 0xFFFF) {
        pPdat->ushLife--;
    }
    if (pPdat->bFlag.FadeIn) {
        if (pPdat->bFade) {
            pPdat->fAlphaScale = 1.0f - (float)pPdat->bFade / pParam->bFadeInFrame;
            pPdat->bFade--;
        } else {
            pPdat->fAlphaScale = 1.0f;
            pPdat->bFlag.FadeIn = 0;
        }
    } else if (pPdat->bFlag.FadeOut) {
        if (pPdat->bFade) {
            pPdat->fAlphaScale = (float)pPdat->bFade / pParam->bFadeInFrame; /* sic: bFadeInFrame */
            pPdat->bFade--;
        } else {
            pPdat->fAlphaScale = 0.0f;
            pPdat->bFlag.FadeOut = 0;
        }
    } else {
        if (pParam->bFadeOutFrame && pPdat->ushLife <= pParam->bFadeOutFrame) {
            pPdat->bFade = pParam->bFadeOutFrame;
            pPdat->bFlag.FadeOut = 1;
        }
        pPdat->fAlphaScale = 1.0f;
    }
    if (fLen > fFarFadeStart + fFarFadeLen) {
        pPdat->fAlphaScale = 0.0f;
    } else if (fLen > fFarFadeStart) {
        pPdat->fAlphaScale *= 1.0f - (fLen - fFarFadeStart) / fNearFadeLen;
    }
    if (sceVu0InnerProduct(vecDir, g_vecCamDir) < 0.0f) {
        pPdat->bFlag.NoDraw = 1;
    } else {
        pPdat->bFlag.NoDraw = 0;
    }
    if (fLen > fFarFadeStart + fFarFadeLen) {
        sceVu0FMATRIX matRot;

        fFarFadeStart = -(fFarFadeStart + fFarFadeLen);
        for (i = 0; i < 3; i++) {
            vec[i] = get_rand() * 1.2566371f - 0.62831855f;
        }
        sceVu0RotMatrix(matRot, ppvUnitMatrix, vec);
        sceVu0ApplyMatrix(vec, matRot, vecDir);
        sceVu0ScaleVector(vec, vec, fFarFadeStart);
        sceVu0AddVector(pPdat->vecPos, ppvEye, vec);
        pPdat->vecPos[3] = 1.0f;
        init_param(pParam, pPdat, pPcol);
    }
}

static void calc_particle(VRyjMegaBirthModelFilter *pVal, PRyjMegaBirthModelFilter *pParam, VColor *pCol) {
    PARTICLE_DATA *pPdat = pVal->pvParticleData;
    PARTICLE_COLOR *pPcol = pVal->pvParticleColor;
    int nNum = pVal->nParticle;
    int nBirthNum = 0;
    int i;

    if (ppvUserStopPartF) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    if (pParam->nModel == 0xFFFF) {
        return;
    }
    pVal->ushTimingCt++;
    for (i = 0; i < nNum; i++) {
        if (pPdat->ushLife) {
            calc(pVal, pParam, pPdat, pCol, pPcol);
        } else if (pParam->ushTiming <= pVal->ushTimingCt && nBirthNum < pParam->ushNumOneF) {
            birth(pVal, pParam, pPdat, pPcol);
            nBirthNum++;
        }
        pPdat++;
        if (pPcol) {
            pPcol++;
        }
    }
    if (nBirthNum > 0) {
        pVal->ushTimingCt = 0;
    }
}

void pppRyjMegaBirthModelFilter(pppPObject *pPobj, PRyjMegaBirthModelFilter *pParam, pppCtrlTable *pCtbl) {
    VRyjMegaBirthModelFilter *pVal = (VRyjMegaBirthModelFilter *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHMODELFILTER_THIS]];
    VColor *pCol = (VColor *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHMODELFILTER_COLOR]];
    sceVu0FVECTOR vec;

    if (!pVal->pvParticleData) {
        /* not in the original debug info: see pppRyjMegaBirthFilter */
        u_int n = pParam->ushNum;

        *(volatile u_int *)&pVal->nParticle = n;
        pVal->pvParticleData = pppAlloc(&ppvEnv->heapMng, n * sizeof(PARTICLE_DATA));
        if (pVal->pvParticleData) {
            memset(pVal->pvParticleData, 0, pVal->nParticle * sizeof(PARTICLE_DATA));
        }
        if (pParam->bColV) {
            pVal->pvParticleColor = pppAlloc(&ppvEnv->heapMng, pVal->nParticle * sizeof(PARTICLE_COLOR));
            if (pVal->pvParticleColor) {
                memset(pVal->pvParticleColor, 0, pVal->nParticle * sizeof(PARTICLE_COLOR));
            }
        }
    }
    if (!alloc_check(pVal, pParam)) {
        return;
    }
    if (pParam->bCollision) {
        float fLen;
        float fCollisionRadius = pParam->fCollisionRadius * 0.01f;
        float fCollisionFadeLen = pParam->fCollisionFadeLen * 0.01f;

        sceVu0SubVector(vec, ppvEye, ppvParLoc);
        VEC_LENGTH(fLen, vec);
        if (fLen > fCollisionRadius) {
            pVal->bActive = 0;
        } else {
            pVal->bActive = 1;
            fLen = fCollisionRadius - fLen;
            if (fLen < fCollisionFadeLen) {
                pVal->fAlphaScale = fLen / fCollisionFadeLen;
            } else {
                pVal->fAlphaScale = 1.0f;
            }
        }
    }
    if (pVal->bActive) {
        calc_particle(pVal, pParam, pCol);
    }
}

static void init_matrix(sceVu0FMATRIX matW, PRyjMegaBirthModelFilter *pParam, VRyjMegaBirthModelFilter *pVal) {
}

static float set_matrix(sceVu0FMATRIX matW, sceVu0FMATRIX matIn, PRyjMegaBirthModelFilter *pParam, VRyjMegaBirthModelFilter *pVal, PARTICLE_DATA *pPdat) {
    float fZ;
    int i;

    sceVu0UnitMatrix(matIn);
    if (*(u_long *)pPdat->anAng || pPdat->anAng[2]) {
        sceVu0FMATRIX matRot;
        sceVu0FVECTOR vecRot;

        for (i = 0; i < 3; i++) {
            vecRot[i] = pPdat->anAng[i] * 3.1415927f / 32768.0f;
        }
        sceVu0RotMatrix(matRot, ppvUnitMatrix, vecRot);
        sceVu0MulMatrix(matIn, matIn, matRot);
    }
    for (i = 0; i < 3; i++) {
        sceVu0ScaleVector(matIn[i], matIn[i], pPdat->afScl[i] * ppvParScl[i]);
    }
    sceVu0CopyVector(matIn[3], pPdat->vecPos);
    sceVu0ApplyMatrix(matIn[3], ppvCameraMatrix, matIn[3]);
    fZ = matIn[3][2];
    matIn[3][1] *= pParam->fAppearScaleY;
    matIn[3][1] += pParam->fAppearOffsetY * ppvParScl[1];
    sceVu0MulMatrix(matIn, ppvScreenMatrix, matIn);
    return fZ;
}

void pppRyjDrawMegaBirthModelFilter(pppPObject *pPobj, PRyjMegaBirthModelFilter *pParam, pppCtrlTable *pCtbl) {
    VRyjMegaBirthModelFilter *pVal = (VRyjMegaBirthModelFilter *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHMODELFILTER_THIS]];
    VColor *pCol = (VColor *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHMODELFILTER_COLOR]];
    PARTICLE_DATA *pPdat = pVal->pvParticleData;
    PARTICLE_COLOR *pPcol = pVal->pvParticleColor;
    int nNum = pVal->nParticle;
    sceVu0FMATRIX matW;
    sceVu0FMATRIX matIn;
    OMD_IN Mdi;
    pppModelSt *pMdl;
    OMD_H *pOmd;
    int nRed;
    int nGre;
    int nBlu;
    int nAlp;
    int i;
    float fNearFadeStart = pParam->fNearFadeStart * 0.01f;
    float fNearFadeLen = pParam->fNearFadeLen * 0.01f;

    if (!alloc_check(pVal, pParam)) {
        return;
    }
    if (!pVal->bActive) {
        return;
    }
    if (pParam->nModel == 0xFFFF) {
        return;
    }
    init_matrix(matW, pParam, pVal);
    pMdl = &ppvEnv->lpModel[pParam->nModel];
    pOmd = pMdl->omd;
    Mdi.pzmat = 0;
    Mdi.lwmat = 0;
    Mdi.lvmat = 0;
    Mdi.lcmat = 0;
    Mdi.pri = 0;
    *(int *)&Mdi.ar = 0;
    Mdi.va = ppvPObj->lpvert;
    Mdi.na = ppvPObj->lpnorm;
    Mdi.tmp_a = (char *)ppvVNWork;
    Mdi.flag = 4;
    Mdi.pmat = &matIn;
    *(int *)&Mdi.toff = 0;
    *(u_long *)&Mdi.u = 0;
    *(int *)&Mdi.ot = 0;
    if (pParam->bFog) {
        Mdi.flag |= 0x200;
    }
    Mdi.ztest_off = 1;
    *(u_short *)&Mdi.alpha = *(u_short *)&pOmd->alpha;
    nRed = pCol->release_col.r;
    nGre = pCol->release_col.g;
    nBlu = pCol->release_col.b;
    nAlp = pCol->release_col.a;
    for (i = 0; i < nNum; i++) {
        if (pPdat->ushLife && !pPdat->bFlag.NoDraw) {
            float fZ = set_matrix(matW, matIn, pParam, pVal, pPdat);
            int nR;
            int nG;
            int nB;
            int nA;

            if (fZ < fNearFadeStart - fNearFadeLen) {
                pPdat->fAlphaScale = 0.0f;
            } else if (fZ < fNearFadeStart) {
                pPdat->fAlphaScale *= 1.0f - (fNearFadeStart - fZ) / fNearFadeLen;
            }
            nR = nRed + pPdat->chNoiseR;
            nG = nGre + pPdat->chNoiseG;
            nB = nBlu + pPdat->chNoiseB;
            nA = nAlp + pPdat->chNoiseA;
            if (pPcol) {
                nR += (int)pPcol->vecCol[0];
                nG += (int)pPcol->vecCol[1];
                nB += (int)pPcol->vecCol[2];
                nA += (int)pPcol->vecCol[3];
            }
            if (nR < 0) {
                nR = 0;
            } else if (nR > 255) {
                nR = 255;
            }
            if (nG < 0) {
                nG = 0;
            } else if (nG > 255) {
                nG = 255;
            }
            if (nB < 0) {
                nB = 0;
            } else if (nB > 255) {
                nB = 255;
            }
            if (nA < 0) {
                nA = 0;
            } else if (nA > 127) {
                nA = 127;
            }
            Mdi.a = nA;
            Mdi.r = nR;
            Mdi.g = nG;
            Mdi.b = nB;
            Mdi.a = Mdi.a * pPdat->fAlphaScale * pVal->fAlphaScale;
            Mdi.u = pPdat->fU * 65536.0f;
            Mdi.v = pPdat->fV * 65536.0f;
            if (Mdi.a) {
                Mdi.packet = ppvCurPrimp;
                ppvCurPrimp = op1_md_draw(pOmd, &Mdi, pMdl->nPacketSize);
            }
        }
        pPdat++;
        if (pPcol) {
            pPcol++;
        }
    }
}

void pppRyjMegaBirthModelFilterSta(pppMngSt *pMng) {
    sceVu0SubVector(g_vecCamDir, ppvAt, ppvEye);
    sceVu0Normalize(g_vecCamDir, g_vecCamDir);
    g_vecCamDir[3] = 1.0f;
}

void pppRyjMegaBirthModelFilterCon(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjMegaBirthModelFilter *pVal = (VRyjMegaBirthModelFilter *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHMODELFILTER_THIS]];

    pVal->pvParticleData = 0;
    pVal->pvParticleColor = 0;
    pVal->nParticle = 0;
    pVal->fAlphaScale = 1.0f;
    pVal->ushTimingCt = 0;
    pVal->bActive = 1;
    (*(pppMngSt * volatile *)&ppvMng)->bBoolNoViewClip = 1;
}

void pppRyjMegaBirthModelFilterDes(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjMegaBirthModelFilter *pVal = (VRyjMegaBirthModelFilter *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHMODELFILTER_THIS]];

    if (pVal->pvParticleData) {
        pppFree(&ppvEnv->heapMng, pVal->pvParticleData);
        pVal->pvParticleData = 0;
    }
    if (pVal->pvParticleColor) {
        pppFree(&ppvEnv->heapMng, pVal->pvParticleColor);
        pVal->pvParticleColor = 0;
    }
}

// SOURCE DATA: .bss
