#include "pppRyjMegaBirthFilter.h"
#include "System/Graphic/Effect/libppp/pppAlloc.h"
#include "System/Graphic/Effect/libppp/draw/shape.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"

extern sceVu0FMATRIX ppvCameraMatrixInv;
extern sceVu0FVECTOR ppvAt;
extern sceVu0FMATRIX ppvUnitMatrix;
void *memset(void *s, int c, unsigned int n);
void sceVu0SubVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);
void sceVu0ScaleVectorXYZ(sceVu0FVECTOR v0, sceVu0FVECTOR v1, float s);
void sceVu0RotMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1, sceVu0FVECTOR rot);
void sceVu0RotMatrixZ(sceVu0FMATRIX m0, sceVu0FMATRIX m1, float rz);

typedef struct _PARTICLE_DATA { /* 0x60 */
    sceVu0FVECTOR vecPos;
    sceVu0FVECTOR vecDir;
    u_short ushWait;
    u_short ushSeq;
    u_short ushSeqNow;
    u_short ushLife;
    char chNoiseR;
    char chNoiseG;
    char chNoiseB;
    char chNoiseA;
    float fAng;
    float fAngV;
    float fAngRnd;
    float afScl[2];
    float afSclV[2];
    float afSclRnd[2];
    float fV;
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

static char alloc_check(VRyjMegaBirthFilter *pVal, PRyjMegaBirthFilter *pParam) {
    if (!pVal->pvParticleData) {
        return 0;
    }
    if (pParam->bColV && !pVal->pvParticleColor) {
        return 0;
    }
    return 1;
}

static void init_param(PRyjMegaBirthFilter *pParam, PARTICLE_DATA *pPdat, PARTICLE_COLOR *pPcol) {
    pPdat->chNoiseR = get_noise(pParam->bColRndRangeR);
    pPdat->chNoiseG = get_noise(pParam->bColRndRangeG);
    pPdat->chNoiseB = get_noise(pParam->bColRndRangeB);
    pPdat->chNoiseA = get_noise(pParam->bColRndRangeA);
    {
        OANM_HEADER *pAh;
        pppShapeSt *pShape;

        pShape = &ppvEnv->lpShape[pParam->nShape];
        pAh = (OANM_HEADER *)pShape->shape;
        pPdat->ushSeq = pAh->seqc * get_rand();
    }
    if (pParam->bFadeInFrame) {
        pPdat->bFade = pParam->bFadeInFrame;
        pPdat->bFlag.FadeIn = 1;
    }
    pPdat->fAng = pParam->fAng;
    pPdat->fAngV = pParam->fAngMov;
    if (pParam->bAngRnd) {
        pPdat->fAngRnd = get_rand() * pParam->fAngRnd;
        if (pParam->bAngRnd & 1 && pParam->bAngRnd & 2) {
            if (get_rand() > 0.5) {
                pPdat->fAngRnd = -pPdat->fAngRnd;
            }
        } else if (pParam->bAngRnd & 2) {
            pPdat->fAngRnd = -pPdat->fAngRnd;
        }
    }
    if (pParam->bAngRnd & 4) {
        pPdat->fAng += pPdat->fAngRnd;
    }
    if (pParam->bAngRnd & 8) {
        pPdat->fAngV += pPdat->fAngRnd;
    }
    while (pPdat->fAng >= 180.0f) pPdat->fAng -= 360.0f;
    while (pPdat->fAng < -180.0f) pPdat->fAng += 360.0f;
    pPdat->afScl[0] = pParam->Scl.x;
    pPdat->afScl[1] = pParam->Scl.y;
    pPdat->afSclV[0] = pParam->SclMov.x;
    pPdat->afSclV[1] = pParam->SclMov.y;
    if (pParam->bSclRnd) {
        if (pParam->bSclRnd & 0x20) {
            pPdat->afSclRnd[0] = pPdat->afSclRnd[1] = get_rand() * pParam->SclRnd.x;
            if (pParam->bSclRnd & 1 && pParam->bSclRnd & 2) {
                if (get_rand() > 0.5) {
                    pPdat->afSclRnd[0] = -pPdat->afSclRnd[0];
                    pPdat->afSclRnd[1] = -pPdat->afSclRnd[1];
                }
            } else if (pParam->bSclRnd & 2) {
                pPdat->afSclRnd[1] = pPdat->afSclRnd[0] = -pPdat->afSclRnd[0];
            }
        } else {
            pPdat->afSclRnd[0] = get_rand() * pParam->SclRnd.x;
            pPdat->afSclRnd[1] = get_rand() * pParam->SclRnd.y;
            if (pParam->bSclRnd & 1 && pParam->bSclRnd & 2) {
                if (get_rand() > 0.5) pPdat->afSclRnd[0] = -pPdat->afSclRnd[0];
                if (get_rand() > 0.5) pPdat->afSclRnd[1] = -pPdat->afSclRnd[1];
            } else if (pParam->bSclRnd & 2) {
                pPdat->afSclRnd[0] = -pPdat->afSclRnd[0];
                pPdat->afSclRnd[1] = -pPdat->afSclRnd[1];
            }
        }
    }
    if (pParam->bSclRnd & 4) {
        pPdat->afScl[0] += pPdat->afSclRnd[0];
        pPdat->afScl[1] += pPdat->afSclRnd[1];
    }
    if (pParam->bSclRnd & 8) {
        pPdat->afSclV[0] += pPdat->afSclRnd[0];
        pPdat->afSclV[1] += pPdat->afSclRnd[1];
    }
    pPdat->fV = pParam->fMov * 0.01f;
    if (pParam->fMovRndRange != 0.0f) {
        float fMovRndRange = pParam->fMovRndRange * 0.01f;

        pPdat->fV += fMovRndRange * 2.0f * get_rand() - fMovRndRange;
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
}

static void birth(VRyjMegaBirthFilter *pVal, PRyjMegaBirthFilter *pParam, PARTICLE_DATA *pPdat, PARTICLE_COLOR *pPcol) {
    sceVu0FVECTOR vecDir;
    float fFarFadeStart = pParam->fFarFadeStart * 0.01f;
    float fFarFadeLen = pParam->fFarFadeLen * 0.01f;

    memset(pPdat, 0, sizeof(PARTICLE_DATA));
    if (pPcol) {
        memset(pPcol, 0, sizeof(PARTICLE_COLOR));
    }
    {
        float fAppearRadius;
        float fLen;

        vecDir[0] = pParam->Dir.x;
        vecDir[1] = -pParam->Dir.y;
        vecDir[2] = -pParam->Dir.z;
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
    }
    init_param(pParam, pPdat, pPcol);
}

static void calc(VRyjMegaBirthFilter *pVal, PRyjMegaBirthFilter *pParam, PARTICLE_DATA *pPdat, VColor *pCol, PARTICLE_COLOR *pPcol) {
    sceVu0FVECTOR vecDist;
    sceVu0FVECTOR vecDir;
    sceVu0FVECTOR vec;
    float fLen;
    int i;
    /* not in the original debug info: pins the fFarFadeStart load to f2 (gcc picks f3 otherwise) */
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
    pPdat->fAng += pPdat->fAngV;
    if (pParam->bAngRnd & 0x10) {
        pPdat->fAngV += pParam->fAngAcc + pPdat->fAngRnd;
    } else {
        pPdat->fAngV += pParam->fAngAcc;
    }
    while (pPdat->fAng >= 180.0f) pPdat->fAng -= 360.0f;
    while (pPdat->fAng < -180.0f) pPdat->fAng += 360.0f;
    pPdat->afScl[0] += pPdat->afSclV[0];
    pPdat->afScl[1] += pPdat->afSclV[1];
    if (pParam->bSclRnd & 0x10) {
        pPdat->afSclV[0] += pParam->SclAcc.x + pPdat->afSclRnd[0];
        pPdat->afSclV[1] += pParam->SclAcc.y + pPdat->afSclRnd[1];
    } else {
        pPdat->afSclV[0] += pParam->SclAcc.x;
        pPdat->afSclV[1] += pParam->SclAcc.y;
    }
    pPdat->fV += pParam->fAcc * 0.01f;
    sceVu0ScaleVectorXYZ(vecDir, pPdat->vecDir, pPdat->fV);
    sceVu0AddVector(pPdat->vecPos, pPdat->vecPos, vecDir);
    pPdat->vecPos[3] = 1.0f;
    sceVu0SubVector(vecDist, pPdat->vecPos, ppvEye);
    sceVu0Normalize(vecDir, vecDist);
    vecDir[3] = 1.0f;
    VEC_LENGTH(fLen, vecDist);
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

static void calc_particle(VRyjMegaBirthFilter *pVal, PRyjMegaBirthFilter *pParam, VColor *pCol) {
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
    if (pParam->nShape == 0xFFFF) {
        return;
    }
    pVal->ushTimingCt++;
    for (i = 0; i < nNum; i++) {
        if (pPdat->ushLife) {
            OANM_HEADER *pAh;
            OANM_SEQ *pSeq;
            pppShapeSt *pShape;

            calc(pVal, pParam, pPdat, pCol, pPcol);
            pShape = &ppvEnv->lpShape[pParam->nShape];
            pAh = (OANM_HEADER *)pShape->shape;
            pSeq = &pAh->seq[pPdat->ushSeqNow];
            pPdat->ushSeqNow = pPdat->ushSeq;
            pPdat->ushWait += pParam->unDelta;
            if (pPdat->ushWait >= pSeq->wait) {
                pPdat->ushWait -= pSeq->wait;
                pPdat->ushSeq = pPdat->ushSeqNow + 1;
                if (pPdat->ushSeq >= pAh->seqc) {
                    if (pSeq->flag & 0x80) {
                        pPdat->ushSeq = 0;
                        pPdat->ushWait = 0;
                    } else {
                        pPdat->ushSeq = pPdat->ushSeqNow;
                        pPdat->ushWait = 0;
                    }
                }
            }
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

void pppRyjMegaBirthFilter(pppPObject *pPobj, PRyjMegaBirthFilter *pParam, pppCtrlTable *pCtbl) {
    VRyjMegaBirthFilter *pVal = (VRyjMegaBirthFilter *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHFILTER_THIS]];
    VColor *pCol = (VColor *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHFILTER_COLOR]];
    sceVu0FVECTOR vec;

    if (!pVal->pvParticleData) {
        /* not in the original debug info: n and the volatile store keep the store ahead of the multiply,
           which gives the original's registers and order */
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

static void init_matrix(sceVu0FMATRIX matW, PRyjMegaBirthFilter *pParam, VRyjMegaBirthFilter *pVal) {
}

static float set_matrix(sceVu0FMATRIX matW, sceVu0FMATRIX matIn, PRyjMegaBirthFilter *pParam, VRyjMegaBirthFilter *pVal, PARTICLE_DATA *pPdat) {
    float fZ;

    sceVu0UnitMatrix(matIn);
    matIn[0][0] = pPdat->afScl[0] * ppvParScl[0];
    matIn[1][1] = pPdat->afScl[1] * ppvParScl[1];
    matIn[2][2] = pPdat->afScl[0] * ppvParScl[0];
    if (pPdat->fAng != 0.0f) {
        sceVu0RotMatrixZ(matIn, matIn, pPdat->fAng * 3.1415927f / 180.0f);
    }
    sceVu0CopyVector(matIn[3], pPdat->vecPos);
    sceVu0ApplyMatrix(matIn[3], ppvCameraMatrix, matIn[3]);
    fZ = matIn[3][2];
    matIn[3][1] *= pParam->fAppearScaleY;
    matIn[3][1] += pParam->fAppearOffsetY * ppvParScl[1];
    sceVu0MulMatrix(matIn, ppvScreenMatrix, matIn);
    return fZ;
}

void pppRyjDrawMegaBirthFilter(pppPObject *pPobj, PRyjMegaBirthFilter *pParam, pppCtrlTable *pCtbl) {
    VRyjMegaBirthFilter *pVal = (VRyjMegaBirthFilter *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHFILTER_THIS]];
    VColor *pCol = (VColor *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHFILTER_COLOR]];
    PARTICLE_DATA *pPdat = pVal->pvParticleData;
    PARTICLE_COLOR *pPcol = pVal->pvParticleColor;
    int nNum = pVal->nParticle;
    sceVu0FMATRIX matW;
    sceVu0FMATRIX matIn;
    OSHAPE_IN Shi;
    OSHAPE_VU1 *pSh;
    OANM_HEADER *pAh;
    OANM_SEQ *pSeq;
    pppShapeSt *pShape;
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
    if (pParam->nShape == 0xFFFF) {
        return;
    }
    init_matrix(matW, pParam, pVal);
    pShape = &ppvEnv->lpShape[pParam->nShape];
    Shi.flag = 0x10;
    if (pParam->bFog) {
        Shi.flag |= 0x800;
    }
    Shi.pzmat = 0;
    *(u_long *)&Shi.toff = 0;
    Shi.pri = 0;
    Shi.pmat = &matIn;
    pAh = (OANM_HEADER *)pShape->shape;
    nRed = pCol->release_col.r;
    nGre = pCol->release_col.g;
    nBlu = pCol->release_col.b;
    nAlp = pCol->release_col.a;
    for (i = 0; i < nNum; i++) {
        if (pPdat->ushLife && !pPdat->bFlag.NoDraw) {
            float fZ = set_matrix(matW, matIn, pParam, pVal, pPdat);

            if (fZ < fNearFadeStart - fNearFadeLen) {
                pPdat->fAlphaScale = 0.0f;
            } else if (fZ < fNearFadeStart) {
                pPdat->fAlphaScale *= 1.0f - (fNearFadeStart - fZ) / fNearFadeLen;
            }
            pSeq = &pAh->seq[pPdat->ushSeqNow];
            pSh = (OSHAPE_VU1 *)((char *)pAh + pSeq->sha);
            {
                int nR = nRed + pPdat->chNoiseR;
                int nG = nGre + pPdat->chNoiseG;
                int nB = nBlu + pPdat->chNoiseB;
                int nA = nAlp + pPdat->chNoiseA;

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
                Shi.a = nA;
                Shi.r = nR;
                Shi.g = nG;
                Shi.b = nB;
                Shi.a = Shi.a * pPdat->fAlphaScale * pVal->fAlphaScale;
                if (Shi.a) {
                    Shi.packet = ppvCurPrimp;
                    ppvCurPrimp = op_shape_draw(pSh, &Shi, pShape->nPacketSize);
                }
            }
        }
        pPdat++;
        if (pPcol) {
            pPcol++;
        }
    }
}

void pppRyjMegaBirthFilterSta(pppMngSt *pMng) {
    sceVu0SubVector(g_vecCamDir, ppvAt, ppvEye);
    sceVu0Normalize(g_vecCamDir, g_vecCamDir);
    g_vecCamDir[3] = 1.0f;
}

void pppRyjMegaBirthFilterCon(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjMegaBirthFilter *pVal = (VRyjMegaBirthFilter *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHFILTER_THIS]];

    pVal->pvParticleData = 0;
    pVal->pvParticleColor = 0;
    pVal->nParticle = 0;
    pVal->fAlphaScale = 1.0f;
    pVal->ushTimingCt = 0;
    pVal->bActive = 1;
    /* volatile read: as in pppRyjDrawShipolyBoneCon, gives the original's register choice */
    (*(pppMngSt * volatile *)&ppvMng)->bBoolNoViewClip = 1;
}

void pppRyjMegaBirthFilterDes(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjMegaBirthFilter *pVal = (VRyjMegaBirthFilter *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHFILTER_THIS]];

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
