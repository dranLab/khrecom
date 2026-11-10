#include "pppRyjMegaBirth.h"
#include "System/Graphic/Effect/libppp/pppAlloc.h"
#include "System/Graphic/Effect/libppp/draw/shape.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"
#include "System/Graphic/Effect/libppp/draw/sin_t.h"


void *lmemset(void *dis, word code, int ln);
void sceVu0ScaleVectorXYZ(sceVu0FVECTOR v0, sceVu0FVECTOR v1, float s);
void sceVu0RotMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1, sceVu0FVECTOR rot);
void sceVu0ClampVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, float min, float max);
void sceVu0FTOI0Vector(sceVu0IVECTOR v0, sceVu0FVECTOR v1);

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
    u_char bCt;
    u_char cdummy0;
    short fAng;
    short fAngV;
    short fAngRnd;
    float afScl[2];
    float afSclV[2];
    float afSclRnd[2];
    float fV;
    float fG;
    float fAlphaDec;
    float fScaleRate;
    u_char bFadeInFrame;
    u_char bFadeOutFrame;
    u_char bScaleFadeInFrame;
    u_char bScaleFadeOutFrame;
} PARTICLE_DATA;

typedef struct _PARTICLE_WMAT { /* 0x40 */
    sceVu0FMATRIX matKeepMatrix;
} PARTICLE_WMAT;

typedef struct _PARTICLE_COLOR { /* 0x20 */
    sceVu0FVECTOR vecCol;
    sceVu0FVECTOR vecColV;
} PARTICLE_COLOR;

typedef struct {
    sceVu0FMATRIX matPC;
    sceVu0FMATRIX matW;
    sceVu0FMATRIX camMat;
    sceVu0FMATRIX scrMat;
    sceVu0FMATRIX calcMat;
    sceVu0FMATRIX matWk;
    sceVu0FVECTOR fvColor;
    sceVu0FVECTOR fvRate;
    sceVu0FVECTOR parScl;
    sceVu0FVECTOR wloc;
} pwst;

typedef union {
    sceVu0FVECTOR f;
    sceVu0IVECTOR i;
} sceVu0FIVECTOR;

static sceVu0FMATRIX g_matUnit;

static float get_rand() {
    float r;

    PPP_RAND_F(r);
    return r;
}

static char get_noise(u_char bRange) {
    return bRange * 2 * get_rand() - bRange / 2;
}

static char alloc_check(VRyjMegaBirth *pVal, PRyjMegaBirth *pParam) {
    if (!pVal->pvParticleData) {
        return 0;
    }
    if ((pParam->bCoord == COORD_WORLD || pParam->bCoord == COORD_WOKIZARI || pParam->bCoord == COORD_OKIZARI) && !pVal->pvParticleWmat) {
        return 0;
    }
    if (pParam->bColV && !pVal->pvParticleColor) {
        return 0;
    }
    return 1;
}

static void birth(pppPObject *pobj, VRyjMegaBirth *pVal, PRyjMegaBirth *pParam, VColor *pCol, PARTICLE_DATA *pPdat, PARTICLE_WMAT *pWmat, PARTICLE_COLOR *pPcol) {
    sceVu0FVECTOR vecDir;
    float fDegHalf = pParam->bDeg;
    float fDeg = fDegHalf * 2.0f;
    sceVu0FVECTOR vecRot;
    sceVu0FMATRIX matRot;
    int i;
    float fR;
    float fRangeRadiusHalf;
    pppVsfSt *pVsf;
    pppSVECTOR *pVert;
    pppSVECTOR Vtx;
    float fRnd;

    lmemset(pPdat, 0, sizeof(PARTICLE_DATA) / 4);
    if (pWmat) {
        lmemset(pWmat, 0, sizeof(PARTICLE_WMAT) / 4);
    }
    if (pPcol) {
        lmemset(pPcol, 0, sizeof(PARTICLE_COLOR) / 4);
    }
    switch (pParam->bMode) {
    case MODE_SPHERE:
    case MODE_SPHERE_NOROT:
    case MODE_DONUTS:
    case MODE_DONUTS_NOROT:
    case MODE_BOX:
    case MODE_BOX_NOROT:
    case MODE_VSF:
    case MODE_VSF_NOROT:
        vecDir[0] = pParam->Dir.x;
        vecDir[1] = pParam->Dir.y;
        vecDir[2] = pParam->Dir.z;
        vecDir[3] = 0.0f;
        for (i = 0; i < 3; i++) {
            vecRot[i] = fDeg * get_rand() - fDegHalf;
            vecRot[i] = vecRot[i] * 3.1415927f / 180.0f;
        }
        if (pParam->bMode == MODE_DONUTS || pParam->bMode == MODE_DONUTS_NOROT) {
            vecRot[0] = 0.0f;
            vecRot[1] = 0.0f;
        }
        sceVu0UnitMatrix(matRot);
        sceVu0RotMatrix(matRot, matRot, vecRot);
        sceVu0ApplyMatrix(pPdat->vecDir, matRot, vecDir);
        pPdat->vecDir[0] *= pParam->RangeScl.x;
        pPdat->vecDir[1] *= pParam->RangeScl.y;
        pPdat->vecDir[2] *= pParam->RangeScl.z;
        sceVu0Normalize(pPdat->vecDir, pPdat->vecDir);
        pPdat->vecDir[3] = 0.0f;
        break;
    }
    switch (pParam->bMode) {
    case MODE_SPHERE:
    case MODE_SPHERE_NOROT:
    case MODE_DONUTS:
    case MODE_DONUTS_NOROT:
    default:
        if (pParam->fRangeRadius != 0.0f) {
            switch (pParam->bRangeMode) {
            default:
            case 0:
                fR = pParam->fRangeRadius;
                break;
            case 1:
                fRnd = get_rand();
                fR = pParam->fRangeRadius * get_rand();
                break;
            case 2:
                fRnd = get_rand();
                fR = pParam->fRangeRadius * fRnd * get_rand();
                break;
            case 3:
                {
                    float r1 = get_rand();

                    fR = pParam->fRangeRadius - pParam->fRangeRadius * r1 * get_rand() * 0.7f;
                }
                break;
            case 4:
                {
                    float r1 = get_rand();
                    float r2 = get_rand();
                    float r3 = get_rand();

                    fR = pParam->fRangeRadius * r1 * r2 * r3 * get_rand();
                }
                break;
            case 5:
                {
                    float r1 = get_rand();
                    float r2 = get_rand();

                    fR = pParam->fRangeRadius - pParam->fRangeRadius * r1 * r2 * get_rand() * 0.5f;
                }
                break;
            }
            sceVu0ScaleVectorXYZ(pPdat->vecPos, pPdat->vecDir, fR);
        }
        break;
    case MODE_BOX:
    case MODE_BOX_NOROT:
        if (pParam->fRangeRadius != 0.0f) {
            fRangeRadiusHalf = pParam->fRangeRadius * 0.5f;
            switch (pParam->bRangeMode) {
            default:
            case 0:
                for (i = 0; i < 3; i++) {
                    pPdat->vecPos[i] = pParam->fRangeRadius * get_rand() - fRangeRadiusHalf;
                }
                break;
            case 1:
                get_rand();
                for (i = 0; i < 3; i++) {
                    pPdat->vecPos[i] = pParam->fRangeRadius * get_rand() - fRangeRadiusHalf;
                }
                break;
            case 2:
                for (i = 0; i < 3; i++) {
                    fRnd = get_rand();
                    pPdat->vecPos[i] = pParam->fRangeRadius * fRnd * get_rand() - fRangeRadiusHalf;
                }
                break;
            case 3:
                for (i = 0; i < 3; i++) {
                    float r1 = get_rand();

                    pPdat->vecPos[i] = pParam->fRangeRadius - pParam->fRangeRadius * r1 * get_rand() * 0.7f - fRangeRadiusHalf;
                }
                break;
            case 4:
                for (i = 0; i < 3; i++) {
                    float r1 = get_rand();
                    float r2 = get_rand();
                    float r3 = get_rand();

                    pPdat->vecPos[i] = pParam->fRangeRadius * r1 * r2 * r3 * get_rand() - fRangeRadiusHalf;
                }
                break;
            case 5:
                for (i = 0; i < 3; i++) {
                    float r1 = get_rand();
                    float r2 = get_rand();

                    pPdat->vecPos[i] = pParam->fRangeRadius - pParam->fRangeRadius * r1 * r2 * get_rand() * 0.5f - fRangeRadiusHalf;
                }
                break;
            }
            pPdat->vecPos[0] *= pParam->RangeScl.x;
            pPdat->vecPos[1] *= pParam->RangeScl.y;
            pPdat->vecPos[2] *= pParam->RangeScl.z;
        }
        break;
    case MODE_VSF:
    case MODE_VSF_NOROT:
    case MODE_VSF_EXP:
    case MODE_VSF_EXP_NOROT:
        pVert = ppvPObj->lpvert;
        if (pParam->shVsfNo >= 0) {
            pVsf = &ppvEnv->lpVsf[pParam->shVsfNo];
            if (!pVert) {
                pVert = ppvEnv->lpModel[pVsf->modelno].vert;
            }
            switch (pParam->bRangeMode) {
            default:
            case 0:
                if (pVal->ushIct >= pVsf->indexn) {
                    pVal->ushIct = 0;
                }
                Vtx = pVert[pVsf->index[pVal->ushIct++]];
                break;
            case 1:
                get_rand();
                fRnd = get_rand();
                goto rnd;
            case 2:
                {
                    float r1 = get_rand();
                    float r2 = get_rand();

                    fRnd = r1 * r2 * get_rand();
                }
                goto rnd;
            case 3:
                {
                    float r1 = get_rand();
                    float r2 = get_rand();

                    fRnd = 1.0 - r1 * r2 * get_rand();
                }
                goto rnd;
            case 4:
                {
                    float r1 = get_rand();
                    float r2 = get_rand();
                    float r3 = get_rand();

                    fRnd = r1 * r2 * r3 * get_rand();
                }
                goto rnd;
            case 5:
                {
                    float r1 = get_rand();
                    float r2 = get_rand();
                    float r3 = get_rand();
                    float r4 = get_rand();

                    fRnd = 1.0 - r1 * r2 * r3 * r4 * get_rand();
                }
            rnd:
                if (pVal->ushIct >= pVsf->indexn) {
                    pVal->ushIct = 0;
                }
                Vtx = pVert[pVsf->index[(int)(fRnd * pVsf->indexn)]];
                break;
            }
            pPdat->vecPos[0] = Vtx.x * pParam->RangeScl.x;
            pPdat->vecPos[1] = Vtx.y * pParam->RangeScl.y;
            pPdat->vecPos[2] = Vtx.z * pParam->RangeScl.z;
            if (pParam->bMode == MODE_VSF_EXP || pParam->bMode == MODE_VSF_EXP_NOROT) {
                sceVu0Normalize(pPdat->vecDir, pPdat->vecPos);
                pPdat->vecDir[3] = 0.0f;
            }
        }
        break;
    }
    pPdat->chNoiseR = get_noise(pParam->bColRndRangeR);
    pPdat->chNoiseG = get_noise(pParam->bColRndRangeG);
    pPdat->chNoiseB = get_noise(pParam->bColRndRangeB);
    pPdat->chNoiseA = get_noise(pParam->bColRndRangeA);
    if (pParam->bFadeInFrame) {
        pPdat->fAlphaDec = pCol->release_col.a;
        pPdat->bFadeInFrame = pParam->bFadeInFrame;
    }
    if (pParam->bFadeOutFrame) {
        pPdat->bFadeOutFrame = pParam->bFadeOutFrame;
    }
    pPdat->fScaleRate = 1.0f;
    if (pParam->bScaleFadeInFrame) {
        pPdat->fScaleRate = 0.0f;
        pPdat->bScaleFadeInFrame = pParam->bScaleFadeInFrame;
    }
    if (pParam->bScaleFadeOutFrame) {
        pPdat->bScaleFadeOutFrame = pParam->bScaleFadeOutFrame;
    }
    pPdat->fAng = pParam->fAng * 32768.0f / 180.0f;
    pPdat->fAngV = pParam->fAngMov * 32768.0f / 180.0f;
    if (pParam->bAngRnd) {
        pPdat->fAngRnd = get_rand() * pParam->fAngRnd * 32768.0f / 180.0f;
        if (pParam->bAngRnd & 1 && pParam->bAngRnd & 2) {
            if (get_rand() > 0.5f) {
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
    pPdat->afScl[0] = pParam->Scl.x;
    pPdat->afScl[1] = pParam->Scl.y;
    pPdat->afSclV[0] = pParam->SclMov.x;
    pPdat->afSclV[1] = pParam->SclMov.y;
    if (pParam->bSclRnd) {
        if (pParam->bSclRnd & 0x20) {
            pPdat->afSclRnd[0] = pPdat->afSclRnd[1] = get_rand() * pParam->SclRnd.x;
            if (pParam->bSclRnd & 1) {
                if (pParam->bSclRnd & 2 && get_rand() > 0.5f) {
                    pPdat->afSclRnd[0] = -pPdat->afSclRnd[0];
                    pPdat->afSclRnd[1] = -pPdat->afSclRnd[1];
                }
            } else if (pParam->bSclRnd & 2) {
                pPdat->afSclRnd[1] = pPdat->afSclRnd[0] = -pPdat->afSclRnd[0];
            }
        } else {
            pPdat->afSclRnd[0] = get_rand() * pParam->SclRnd.x;
            pPdat->afSclRnd[1] = get_rand() * pParam->SclRnd.y;
            if (pParam->bSclRnd & 1) {
                if (pParam->bSclRnd & 2) {
                    if (get_rand() > 0.5f) {
                        pPdat->afSclRnd[0] = -pPdat->afSclRnd[0];
                    }
                    if (get_rand() > 0.5f) {
                        pPdat->afSclRnd[1] = -pPdat->afSclRnd[1];
                    }
                }
            } else if (pParam->bSclRnd & 2) {
                pPdat->afSclRnd[1] = -pPdat->afSclRnd[1];
                pPdat->afSclRnd[0] = -pPdat->afSclRnd[0];
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
    switch (pParam->bReverseRnd) {
    case REVERSE_OFF:
        break;
    case REVERSE_LR:
        if (get_rand() > 0.5f) {
            pPdat->afScl[0] = -pPdat->afScl[0];
        }
        break;
    case REVERSE_UD:
        if (get_rand() > 0.5f) {
            pPdat->afScl[1] = -pPdat->afScl[1];
        }
        break;
    case REVERSE_LRUD:
        if (get_rand() > 0.5f) {
            pPdat->afScl[0] = -pPdat->afScl[0];
        }
        if (get_rand() > 0.5f) {
            pPdat->afScl[1] = -pPdat->afScl[1];
        }
        break;
    }
    pPdat->fV = pParam->fMov;
    pPdat->fG = pParam->fForce;
    if (pParam->fMovRndRange != 0.0f) {
        pPdat->fV += pParam->fMovRndRange * 2.0f * get_rand() - pParam->fMovRndRange;
    }
    if (pParam->fForceRndRange != 0.0f) {
        pPdat->fG += pParam->fForceRndRange * 2.0f * get_rand() - pParam->fForceRndRange;
    }
    if (pParam->ushLife) {
        pPdat->ushLife = pParam->ushLife;
    } else {
        pPdat->ushLife = 0xFFFF;
    }
    pPdat->bCt = 0;
    switch (pParam->bCoord) {
    case COORD_WORLD:
        sceVu0CopyMatrix(pWmat->matKeepMatrix, pVal->matPar);
        break;
    case COORD_OKIZARI:
        sceVu0CopyMatrix(pWmat->matKeepMatrix, ppvPObj->cmat);
        break;
    case COORD_WOKIZARI:
        sceVu0MulMatrix(pWmat->matKeepMatrix, pVal->matPar, ppvPObj->cmat);
        break;
    }
    if (pPcol) {
        pPcol->vecColV[0] = pParam->fColMovR;
        pPcol->vecColV[1] = pParam->fColMovG;
        pPcol->vecColV[2] = pParam->fColMovB;
        pPcol->vecColV[3] = pParam->fColMovA;
    }
}

static void calc(VRyjMegaBirth *pVal, PRyjMegaBirth *pParam, PARTICLE_DATA *pPdat, VColor *pCol, PARTICLE_COLOR *pPcol) {
    int nA = pCol->release_col.a;

    if (pPcol) {
        pPcol->vecCol[0] += pPcol->vecColV[0];
        pPcol->vecCol[1] += pPcol->vecColV[1];
        pPcol->vecCol[2] += pPcol->vecColV[2];
        pPcol->vecCol[3] += pPcol->vecColV[3];
        pPcol->vecColV[0] += pParam->fColAccR;
        pPcol->vecColV[1] += pParam->fColAccG;
        pPcol->vecColV[2] += pParam->fColAccB;
        pPcol->vecColV[3] += pParam->fColAccA;
        nA = pCol->release_col.a + (int)pPcol->vecCol[3];
        if (nA > 255) {
            nA = 255;
        }
    }
    pPdat->fAng += pPdat->fAngV;
    if (pParam->bAngRnd & 0x10) {
        pPdat->fAngV += pParam->fAngAcc + pPdat->fAngRnd;
    } else {
        pPdat->fAngV += pParam->fAngAcc;
    }
    pPdat->afScl[0] += pPdat->afSclV[0];
    pPdat->afScl[1] += pPdat->afSclV[1];
    if (pParam->bSclRnd & 0x10) {
        pPdat->afSclV[0] += pParam->SclAcc.x + pPdat->afSclRnd[0];
        pPdat->afSclV[1] += pParam->SclAcc.y + pPdat->afSclRnd[1];
    } else {
        pPdat->afSclV[0] += pParam->SclAcc.x;
        pPdat->afSclV[1] += pParam->SclAcc.y;
    }
    pPdat->fV += pParam->fAcc;
    if (!pParam->bLimitOff) {
        if (pParam->fMov > 0.0f && pParam->fAcc < 0.0f) {
            if (pPdat->fV < 0.0f) {
                pPdat->fV = 0.0f;
            }
        } else if (pParam->fMov < 0.0f && pParam->fAcc > 0.0f) {
            if (pPdat->fV > 0.0f) {
                pPdat->fV = 0.0f;
            }
        }
    }
    pPdat->fG += pParam->fForceAcc;
    /* vecPos += vecDir * fV + vecForce * fG */
    __asm__ __volatile__("lqc2 $vf1, 0(%0)\n"
                         "\tlqc2 $vf2, 0(%1)\n"
                         "\tlw $8, 0(%2)\n"
                         "\tqmtc2.ni $8, $vf3\n"
                         "\tlqc2 $vf4, 0(%3)\n"
                         "\tlw $9, 0(%4)\n"
                         "\tqmtc2.ni $9, $vf5\n"
                         "\tvmulx.xyz $vf2, $vf2, $vf3x\n"
                         "\tvmulx.xyz $vf4, $vf4, $vf5x\n"
                         "\tvadd.xyz $vf1, $vf1, $vf2\n"
                         "\tvadd.xyz $vf1, $vf1, $vf4\n"
                         "\tsqc2 $vf1, 0(%0)"
                         :
                         : "r"(pPdat->vecPos), "r"(pPdat->vecDir), "r"(&pPdat->fV), "r"(pVal->vecForce), "r"(&pPdat->fG)
                         : "$8", "$9", "memory");
    if (pParam->ushLife) {
        pPdat->ushLife--;
    }
    pPdat->bCt++;
    if (pPdat->bFadeInFrame && pPdat->bCt <= pPdat->bFadeInFrame) {
        pPdat->fAlphaDec -= (float)nA / pPdat->bFadeInFrame;
    }
    if (pPdat->bFadeOutFrame && pPdat->ushLife <= pPdat->bFadeOutFrame) {
        pPdat->fAlphaDec += (float)nA / pParam->bFadeOutFrame;
    }
    if (pPdat->bScaleFadeInFrame && pPdat->bCt <= pPdat->bScaleFadeInFrame) {
        pPdat->fScaleRate = (float)pPdat->bCt / pPdat->bScaleFadeInFrame;
    }
    if (pPdat->bScaleFadeOutFrame && pPdat->ushLife <= pPdat->bScaleFadeOutFrame) {
        pPdat->fScaleRate = (float)pPdat->ushLife / pPdat->bScaleFadeOutFrame;
    }
}


static void calc_particle(pppPObject *pobj, VRyjMegaBirth *pVal, PRyjMegaBirth *pParam, VColor *pCol) {
    PARTICLE_DATA *pPdat = pVal->pvParticleData;
    PARTICLE_WMAT *pWmat = pVal->pvParticleWmat;
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
            birth(pobj, pVal, pParam, pCol, pPdat, pWmat, pPcol);
            nBirthNum++;
        }
        pPdat++;
        if (pWmat) {
            pWmat++;
        }
        if (pPcol) {
            pPcol++;
        }
    }
    if (nBirthNum > 0) {
        pVal->ushTimingCt = 0;
    }
}

void pppRyjMegaBirth(pppPObject *pobj, PRyjMegaBirth *pParam, pppCtrlTable *pCtbl) {
    VRyjMegaBirth *pVal = (VRyjMegaBirth *)&pobj->val[pCtbl->useVal[RYJMEGABIRTH_THIS]];
    VColor *pCol = (VColor *)&pobj->val[pCtbl->useVal[RYJMEGABIRTH_COLOR]];

    if (!pVal->pvParticleData) {
        /* not in the original debug info: see pppRyjMegaBirthFilter */
        u_int n = pParam->ushNum;

        *(volatile u_int *)&pVal->nParticle = n;
        pVal->pvParticleData = pppAlloc(&ppvEnv->heapMng, n * sizeof(PARTICLE_DATA));
        if (pVal->pvParticleData) {
            lmemset(pVal->pvParticleData, 0, pVal->nParticle * sizeof(PARTICLE_DATA) / 4);
        }
        if ((pParam->bCoord == COORD_WORLD || pParam->bCoord == COORD_OKIZARI || pParam->bCoord == COORD_WOKIZARI) && !pVal->pvParticleWmat) {
            pVal->pvParticleWmat = pppAlloc(&ppvEnv->heapMng, pVal->nParticle * sizeof(PARTICLE_WMAT));
            if (pVal->pvParticleWmat) {
                lmemset(pVal->pvParticleWmat, 0, pVal->nParticle * sizeof(PARTICLE_WMAT) / 4);
            }
        }
        if (pParam->bColV && !pVal->pvParticleColor) {
            pVal->pvParticleColor = pppAlloc(&ppvEnv->heapMng, pVal->nParticle * sizeof(PARTICLE_COLOR));
            if (pVal->pvParticleColor) {
                lmemset(pVal->pvParticleColor, 0, pVal->nParticle * sizeof(PARTICLE_COLOR) / 4);
            }
        }
        pVal->vecForce[0] = pParam->Force.x;
        pVal->vecForce[1] = pParam->Force.y;
        pVal->vecForce[2] = pParam->Force.z;
        pVal->vecForce[3] = 0.0f;
        sceVu0Normalize(pVal->vecForce, pVal->vecForce);
    }
    if (!alloc_check(pVal, pParam)) {
        return;
    }
    switch (pParam->bMode) {
    case MODE_SPHERE_NOROT:
    case MODE_DONUTS_NOROT:
    case MODE_BOX_NOROT:
    case MODE_VSF_NOROT:
    case MODE_VSF_EXP_NOROT:
        sceVu0UnitMatrix(pVal->matPar);
        sceVu0ScaleVectorXYZ(pVal->matPar[0], pVal->matPar[0], ppvParScl[0]);
        sceVu0ScaleVectorXYZ(pVal->matPar[1], pVal->matPar[1], ppvParScl[1]);
        sceVu0ScaleVectorXYZ(pVal->matPar[2], pVal->matPar[2], ppvParScl[2]);
        sceVu0CopyVector(pVal->matPar[3], ppvParMatrix[3]);
        pVal->matPar[3][3] = 1.0f;
        break;
    /* MODE_SPHERE: default */
    case MODE_DONUTS:
    case MODE_BOX:
    case MODE_VSF:
    case MODE_VSF_EXP:
    default:
        sceVu0CopyMatrix(pVal->matPar, ppvParMatrix);
        break;
    }
    calc_particle(pobj, pVal, pParam, pCol);
}


static void init_matrix(pppPObject *pobj, pwst *pw, PRyjMegaBirth *pParam, VRyjMegaBirth *pVal) {
    if (!pParam->bCoord) {
        sceVu0MulMatrix(pw->matPC, pVal->matPar, pw->calcMat);
        sceVu0MulMatrix(pw->matW, pw->camMat, pw->matPC);
    }
}









static void qsort_particle(int *idx_list, float *z_list, int first, int last) {
    int i;
    int j;
    float mid;
    float ftmp;
    int tmp;

    mid = z_list[(first + last) / 2];
    i = first;
    j = last;
    for (;;) {
        while (mid < z_list[i]) {
            i++;
        }
        while (z_list[j] < mid) {
            j--;
        }
        if (i >= j) {
            break;
        }
        ftmp = z_list[i];
        z_list[i] = z_list[j];
        z_list[j] = ftmp;
        tmp = idx_list[i];
        idx_list[i] = idx_list[j];
        idx_list[j] = tmp;
        i++;
        j--;
    }
    if (first < i - 1) {
        qsort_particle(idx_list, z_list, first, i - 1);
    }
    if (j + 1 < last) {
        qsort_particle(idx_list, z_list, j + 1, last);
    }
}


/* not in the debug info: as an inline function, the address operand lives long enough for the loop optimizer
   to hoist pw->wloc out of the draw loop, as in the original */
static inline void scale_vector(float *v, float s) {
    __asm__ __volatile__(
        "qmtc2.ni %1, $vf9\n"
        "lqc2 $vf10, 0x0(%0)\n"
        "vmulx.xyz $vf10, $vf10, $vf9x\n"
        "sqc2 $vf10, 0x0(%0)"
        :
        : "r"(v), "r"(s)
        : "memory");
}

static inline void set_matrix(pppPObject *pobj, pwst *pw, sceVu0FMATRIX matIn, PRyjMegaBirth *pParam, VRyjMegaBirth *pVal, PARTICLE_DATA *pPdat, PARTICLE_WMAT *pWmat) {
    ((u_long128 *)matIn)[3] = 0;
    ((u_long128 *)matIn)[2] = 0;
    ((u_long128 *)matIn)[1] = 0;
    ((u_long128 *)matIn)[0] = 0;
    pw->wloc[0] = pPdat->afScl[0] * pw->parScl[0];
    pw->wloc[2] = pPdat->afScl[1] * pw->parScl[1];
    if (pPdat->fScaleRate < 1.0f) {
        scale_vector(pw->wloc, pPdat->fScaleRate);
    }
    matIn[3][3] = 1.0f;
    matIn[0][0] = pw->wloc[0];
    matIn[2][2] = pw->wloc[0];
    matIn[1][1] = pw->wloc[2];
    if (pPdat->fAng != 0.0f) {
        float c;

        pw->matWk[0][1] = eff_sin_t[(pPdat->fAng & 0xFFFF) >> 4];
        c = eff_sin_t[((pPdat->fAng + 0x4000) & 0xFFFF) >> 4];
        pw->matWk[1][0] = -pw->matWk[0][1];
        pw->matWk[1][1] = c;
        pw->matWk[0][0] = c;
        pw->matWk[0][2] = 0.0f;
        pw->matWk[0][3] = 0.0f;
        pw->matWk[1][2] = 0.0f;
        pw->matWk[1][3] = 0.0f;
        __asm__ __volatile__(
            "lqc2 $vf1, 0x0(%0)\n"
            "lqc2 $vf2, 0x10(%0)\n"
            "lqc2 $vf3, 0x20(%0)\n"
            "lqc2 $vf4, 0x30(%0)\n"
            "lqc2 $vf5, 0x0(%1)\n"
            "lqc2 $vf6, 0x10(%1)\n"
            "lqc2 $vf7, 0x20(%1)\n"
            "lqc2 $vf8, 0x30(%1)\n"
            "vmulax.xyzw $ACC, $vf5, $vf1x\n"
            "vmaddy.xyzw $vf1, $vf6, $vf1y\n"
            "vmulax.xyzw $ACC, $vf5, $vf2x\n"
            "vmaddy.xyzw $vf2, $vf6, $vf2y\n"
            "sqc2 $vf1, 0x0(%1)\n"
            "sqc2 $vf2, 0x10(%1)\n"
            "sqc2 $vf7, 0x20(%1)\n"
            "sqc2 $vf8, 0x30(%1)"
            :
            : "r"(pw->matWk), "r"(matIn)
            : "memory");
    }
    __asm__ __volatile__(
        "lqc2 $vf10, 0x0(%0)\n"
        "lqc2 $vf11, 0x0(%1)\n"
        "vadd.xyzw $vf10, $vf10, $vf11\n"
        "sqc2 $vf10, 0x0(%0)"
        :
        : "r"(matIn[3]), "r"(pPdat->vecPos)
        : "memory");
    switch (pParam->bCoord) {
    case COORD_LOCAL:
        __asm__ __volatile__(
            "lqc2 $vf1, 0x0(%0)\n"
            "lqc2 $vf2, 0x10(%0)\n"
            "lqc2 $vf3, 0x20(%0)\n"
            "lqc2 $vf4, 0x30(%0)\n"
            "lqc2 $vf9, 0x0(%1)\n"
            "lqc2 $vf10, 0x10(%1)\n"
            "lqc2 $vf11, 0x20(%1)\n"
            "lqc2 $vf12, 0x30(%1)\n"
            "lqc2 $vf5, 0x0(%2)\n"
            "lqc2 $vf6, 0x10(%2)\n"
            "lqc2 $vf7, 0x20(%2)\n"
            "lqc2 $vf8, 0x30(%2)\n"
            "vmulax.xyzw $ACC, $vf9, $vf4x\n"
            "vmadday.xyzw $ACC, $vf10, $vf4y\n"
            "vmaddaz.xyzw $ACC, $vf11, $vf4z\n"
            "vmaddw.xyzw $vf4, $vf12, $vf4w\n"
            "vmulax.xyzw $ACC, $vf5, $vf1x\n"
            "vmadday.xyzw $ACC, $vf6, $vf1y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf1z\n"
            "vmaddw.xyzw $vf1, $vf8, $vf1w\n"
            "vmulax.xyzw $ACC, $vf5, $vf2x\n"
            "vmadday.xyzw $ACC, $vf6, $vf2y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf2z\n"
            "vmaddw.xyzw $vf2, $vf8, $vf2w\n"
            "vmulax.xyzw $ACC, $vf5, $vf3x\n"
            "vmadday.xyzw $ACC, $vf6, $vf3y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf3z\n"
            "vmaddw.xyzw $vf3, $vf8, $vf3w\n"
            "vmulax.xyzw $ACC, $vf5, $vf4x\n"
            "vmadday.xyzw $ACC, $vf6, $vf4y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf4z\n"
            "vmaddw.xyzw $vf4, $vf8, $vf4w\n"
            "sqc2 $vf1, 0x0(%0)\n"
            "sqc2 $vf2, 0x10(%0)\n"
            "sqc2 $vf3, 0x20(%0)\n"
            "sqc2 $vf4, 0x30(%0)"
            :
            : "r"(matIn), "r"(pw->matW), "r"(pw->scrMat)
            : "memory");
        break;
    case COORD_WORLD:
        __asm__ __volatile__(
            "lqc2 $vf1, 0x0(%0)\n"
            "lqc2 $vf2, 0x10(%0)\n"
            "lqc2 $vf3, 0x20(%0)\n"
            "lqc2 $vf4, 0x30(%0)\n"
            "lqc2 $vf9, 0x0(%1)\n"
            "lqc2 $vf10, 0x10(%1)\n"
            "lqc2 $vf11, 0x20(%1)\n"
            "lqc2 $vf12, 0x30(%1)\n"
            "lqc2 $vf13, 0x0(%2)\n"
            "lqc2 $vf14, 0x10(%2)\n"
            "lqc2 $vf15, 0x20(%2)\n"
            "lqc2 $vf16, 0x30(%2)\n"
            "lqc2 $vf17, 0x0(%3)\n"
            "lqc2 $vf18, 0x10(%3)\n"
            "lqc2 $vf19, 0x20(%3)\n"
            "lqc2 $vf20, 0x30(%3)\n"
            "lqc2 $vf5, 0x0(%4)\n"
            "lqc2 $vf6, 0x10(%4)\n"
            "lqc2 $vf7, 0x20(%4)\n"
            "lqc2 $vf8, 0x30(%4)\n"
            "vmulax.xyzw $ACC, $vf9, $vf17x\n"
            "vmadday.xyzw $ACC, $vf10, $vf17y\n"
            "vmaddaz.xyzw $ACC, $vf11, $vf17z\n"
            "vmaddw.xyzw $vf17, $vf12, $vf17w\n"
            "vmulax.xyzw $ACC, $vf9, $vf18x\n"
            "vmadday.xyzw $ACC, $vf10, $vf18y\n"
            "vmaddaz.xyzw $ACC, $vf11, $vf18z\n"
            "vmaddw.xyzw $vf18, $vf12, $vf18w\n"
            "vmulax.xyzw $ACC, $vf9, $vf19x\n"
            "vmadday.xyzw $ACC, $vf10, $vf19y\n"
            "vmaddaz.xyzw $ACC, $vf11, $vf19z\n"
            "vmaddw.xyzw $vf19, $vf12, $vf19w\n"
            "vmulax.xyzw $ACC, $vf9, $vf20x\n"
            "vmadday.xyzw $ACC, $vf10, $vf20y\n"
            "vmaddaz.xyzw $ACC, $vf11, $vf20z\n"
            "vmaddw.xyzw $vf20, $vf12, $vf20w\n"
            "vmulax.xyzw $ACC, $vf13, $vf17x\n"
            "vmadday.xyzw $ACC, $vf14, $vf17y\n"
            "vmaddaz.xyzw $ACC, $vf15, $vf17z\n"
            "vmaddw.xyzw $vf9, $vf16, $vf17w\n"
            "vmulax.xyzw $ACC, $vf13, $vf18x\n"
            "vmadday.xyzw $ACC, $vf14, $vf18y\n"
            "vmaddaz.xyzw $ACC, $vf15, $vf18z\n"
            "vmaddw.xyzw $vf10, $vf16, $vf18w\n"
            "vmulax.xyzw $ACC, $vf13, $vf19x\n"
            "vmadday.xyzw $ACC, $vf14, $vf19y\n"
            "vmaddaz.xyzw $ACC, $vf15, $vf19z\n"
            "vmaddw.xyzw $vf11, $vf16, $vf19w\n"
            "vmulax.xyzw $ACC, $vf13, $vf20x\n"
            "vmadday.xyzw $ACC, $vf14, $vf20y\n"
            "vmaddaz.xyzw $ACC, $vf15, $vf20z\n"
            "vmaddw.xyzw $vf12, $vf16, $vf20w\n"
            "vmulax.xyzw $ACC, $vf9, $vf4x\n"
            "vmadday.xyzw $ACC, $vf10, $vf4y\n"
            "vmaddaz.xyzw $ACC, $vf11, $vf4z\n"
            "vmaddw.xyzw $vf4, $vf12, $vf4w\n"
            "vmulax.xyzw $ACC, $vf5, $vf1x\n"
            "vmadday.xyzw $ACC, $vf6, $vf1y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf1z\n"
            "vmaddw.xyzw $vf1, $vf8, $vf1w\n"
            "vmulax.xyzw $ACC, $vf5, $vf2x\n"
            "vmadday.xyzw $ACC, $vf6, $vf2y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf2z\n"
            "vmaddw.xyzw $vf2, $vf8, $vf2w\n"
            "vmulax.xyzw $ACC, $vf5, $vf3x\n"
            "vmadday.xyzw $ACC, $vf6, $vf3y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf3z\n"
            "vmaddw.xyzw $vf3, $vf8, $vf3w\n"
            "vmulax.xyzw $ACC, $vf5, $vf4x\n"
            "vmadday.xyzw $ACC, $vf6, $vf4y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf4z\n"
            "vmaddw.xyzw $vf4, $vf8, $vf4w\n"
            "sqc2 $vf1, 0x0(%0)\n"
            "sqc2 $vf2, 0x10(%0)\n"
            "sqc2 $vf3, 0x20(%0)\n"
            "sqc2 $vf4, 0x30(%0)"
            :
            : "r"(matIn), "r"(pWmat->matKeepMatrix), "r"(pw->camMat), "r"(pw->calcMat), "r"(pw->scrMat)
            : "memory");
        break;
    case COORD_WOKIZARI:
        __asm__ __volatile__(
            "lqc2 $vf9, 0x0(%1)\n"
            "lqc2 $vf10, 0x10(%1)\n"
            "lqc2 $vf11, 0x20(%1)\n"
            "lqc2 $vf12, 0x30(%1)\n"
            "lqc2 $vf13, 0x0(%2)\n"
            "lqc2 $vf14, 0x10(%2)\n"
            "lqc2 $vf15, 0x20(%2)\n"
            "lqc2 $vf16, 0x30(%2)\n"
            "vmulax.xyzw $ACC, $vf13, $vf9x\n"
            "lqc2 $vf1, 0x0(%0)\n"
            "vmadday.xyzw $ACC, $vf14, $vf9y\n"
            "lqc2 $vf2, 0x10(%0)\n"
            "vmaddaz.xyzw $ACC, $vf15, $vf9z\n"
            "lqc2 $vf3, 0x20(%0)\n"
            "vmaddw.xyzw $vf9, $vf16, $vf9w\n"
            "lqc2 $vf4, 0x30(%0)\n"
            "vmulax.xyzw $ACC, $vf13, $vf10x\n"
            "lqc2 $vf5, 0x0(%3)\n"
            "vmadday.xyzw $ACC, $vf14, $vf10y\n"
            "lqc2 $vf6, 0x10(%3)\n"
            "vmaddaz.xyzw $ACC, $vf15, $vf10z\n"
            "lqc2 $vf7, 0x20(%3)\n"
            "vmaddw.xyzw $vf10, $vf16, $vf10w\n"
            "lqc2 $vf8, 0x30(%3)\n"
            "vmulax.xyzw $ACC, $vf13, $vf11x\n"
            "vmadday.xyzw $ACC, $vf14, $vf11y\n"
            "vmaddaz.xyzw $ACC, $vf15, $vf11z\n"
            "vmaddw.xyzw $vf11, $vf16, $vf11w\n"
            "vmulax.xyzw $ACC, $vf13, $vf12x\n"
            "vmadday.xyzw $ACC, $vf14, $vf12y\n"
            "vmaddaz.xyzw $ACC, $vf15, $vf12z\n"
            "vmaddw.xyzw $vf12, $vf16, $vf12w\n"
            "vmulax.xyzw $ACC, $vf9, $vf4x\n"
            "vmadday.xyzw $ACC, $vf10, $vf4y\n"
            "vmaddaz.xyzw $ACC, $vf11, $vf4z\n"
            "vmaddw.xyzw $vf4, $vf12, $vf4w\n"
            "vmulax.xyzw $ACC, $vf5, $vf1x\n"
            "vmadday.xyzw $ACC, $vf6, $vf1y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf1z\n"
            "vmaddw.xyzw $vf1, $vf8, $vf1w\n"
            "vmulax.xyzw $ACC, $vf5, $vf2x\n"
            "sqc2 $vf1, 0x0(%0)\n"
            "vmadday.xyzw $ACC, $vf6, $vf2y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf2z\n"
            "vmaddw.xyzw $vf2, $vf8, $vf2w\n"
            "vmulax.xyzw $ACC, $vf5, $vf3x\n"
            "sqc2 $vf2, 0x10(%0)\n"
            "vmadday.xyzw $ACC, $vf6, $vf3y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf3z\n"
            "vmaddw.xyzw $vf3, $vf8, $vf3w\n"
            "vmulax.xyzw $ACC, $vf5, $vf4x\n"
            "sqc2 $vf3, 0x20(%0)\n"
            "vmadday.xyzw $ACC, $vf6, $vf4y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf4z\n"
            "vmaddw.xyzw $vf4, $vf8, $vf4w\n"
            "sqc2 $vf4, 0x30(%0)"
            :
            : "r"(matIn), "r"(pWmat->matKeepMatrix), "r"(pw->camMat), "r"(pw->scrMat)
            : "memory");
        break;
    case COORD_OKIZARI:
        __asm__ __volatile__(
            "lqc2 $vf1, 0x0(%0)\n"
            "lqc2 $vf2, 0x10(%0)\n"
            "lqc2 $vf3, 0x20(%0)\n"
            "lqc2 $vf4, 0x30(%0)\n"
            "lqc2 $vf9, 0x0(%1)\n"
            "lqc2 $vf10, 0x10(%1)\n"
            "lqc2 $vf11, 0x20(%1)\n"
            "lqc2 $vf12, 0x30(%1)\n"
            "lqc2 $vf13, 0x0(%2)\n"
            "lqc2 $vf14, 0x10(%2)\n"
            "lqc2 $vf15, 0x20(%2)\n"
            "lqc2 $vf16, 0x30(%2)\n"
            "lqc2 $vf17, 0x0(%3)\n"
            "lqc2 $vf18, 0x10(%3)\n"
            "lqc2 $vf19, 0x20(%3)\n"
            "lqc2 $vf20, 0x30(%3)\n"
            "lqc2 $vf5, 0x0(%4)\n"
            "lqc2 $vf6, 0x10(%4)\n"
            "lqc2 $vf7, 0x20(%4)\n"
            "lqc2 $vf8, 0x30(%4)\n"
            "vmulax.xyzw $ACC, $vf17, $vf9x\n"
            "vmadday.xyzw $ACC, $vf18, $vf9y\n"
            "vmaddaz.xyzw $ACC, $vf19, $vf9z\n"
            "vmaddw.xyzw $vf9, $vf20, $vf9w\n"
            "vmulax.xyzw $ACC, $vf17, $vf10x\n"
            "vmadday.xyzw $ACC, $vf18, $vf10y\n"
            "vmaddaz.xyzw $ACC, $vf19, $vf10z\n"
            "vmaddw.xyzw $vf10, $vf20, $vf10w\n"
            "vmulax.xyzw $ACC, $vf17, $vf11x\n"
            "vmadday.xyzw $ACC, $vf18, $vf11y\n"
            "vmaddaz.xyzw $ACC, $vf19, $vf11z\n"
            "vmaddw.xyzw $vf11, $vf20, $vf11w\n"
            "vmulax.xyzw $ACC, $vf17, $vf12x\n"
            "vmadday.xyzw $ACC, $vf18, $vf12y\n"
            "vmaddaz.xyzw $ACC, $vf19, $vf12z\n"
            "vmaddw.xyzw $vf12, $vf20, $vf12w\n"
            "vmulax.xyzw $ACC, $vf13, $vf9x\n"
            "vmadday.xyzw $ACC, $vf14, $vf9y\n"
            "vmaddaz.xyzw $ACC, $vf15, $vf9z\n"
            "vmaddw.xyzw $vf9, $vf16, $vf9w\n"
            "vmulax.xyzw $ACC, $vf13, $vf10x\n"
            "vmadday.xyzw $ACC, $vf14, $vf10y\n"
            "vmaddaz.xyzw $ACC, $vf15, $vf10z\n"
            "vmaddw.xyzw $vf10, $vf16, $vf10w\n"
            "vmulax.xyzw $ACC, $vf13, $vf11x\n"
            "vmadday.xyzw $ACC, $vf14, $vf11y\n"
            "vmaddaz.xyzw $ACC, $vf15, $vf11z\n"
            "vmaddw.xyzw $vf11, $vf16, $vf11w\n"
            "vmulax.xyzw $ACC, $vf13, $vf12x\n"
            "vmadday.xyzw $ACC, $vf14, $vf12y\n"
            "vmaddaz.xyzw $ACC, $vf15, $vf12z\n"
            "vmaddw.xyzw $vf12, $vf16, $vf12w\n"
            "vmulax.xyzw $ACC, $vf9, $vf4x\n"
            "vmadday.xyzw $ACC, $vf10, $vf4y\n"
            "vmaddaz.xyzw $ACC, $vf11, $vf4z\n"
            "vmaddw.xyzw $vf4, $vf12, $vf4w\n"
            "vmulax.xyzw $ACC, $vf5, $vf1x\n"
            "vmadday.xyzw $ACC, $vf6, $vf1y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf1z\n"
            "vmaddw.xyzw $vf1, $vf8, $vf1w\n"
            "vmulax.xyzw $ACC, $vf5, $vf2x\n"
            "vmadday.xyzw $ACC, $vf6, $vf2y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf2z\n"
            "vmaddw.xyzw $vf2, $vf8, $vf2w\n"
            "vmulax.xyzw $ACC, $vf5, $vf3x\n"
            "vmadday.xyzw $ACC, $vf6, $vf3y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf3z\n"
            "vmaddw.xyzw $vf3, $vf8, $vf3w\n"
            "vmulax.xyzw $ACC, $vf5, $vf4x\n"
            "vmadday.xyzw $ACC, $vf6, $vf4y\n"
            "vmaddaz.xyzw $ACC, $vf7, $vf4z\n"
            "vmaddw.xyzw $vf4, $vf8, $vf4w\n"
            "sqc2 $vf1, 0x0(%0)\n"
            "sqc2 $vf2, 0x10(%0)\n"
            "sqc2 $vf3, 0x20(%0)\n"
            "sqc2 $vf4, 0x30(%0)"
            :
            : "r"(matIn), "r"(pWmat->matKeepMatrix), "r"(pw->camMat), "r"(pVal->matPar), "r"(pw->scrMat)
            : "memory");
        break;
    }
}

void pppRyjDrawMegaBirth(pppPObject *pobj, PRyjMegaBirth *pParam, pppCtrlTable *pCtbl) {
    VRyjMegaBirth *pVal = (VRyjMegaBirth *)&pobj->val[pCtbl->useVal[RYJMEGABIRTH_THIS]];
    VColor *pCol = (VColor *)&pobj->val[pCtbl->useVal[RYJMEGABIRTH_COLOR]];
    PARTICLE_DATA *pPdat = pVal->pvParticleData;
    PARTICLE_WMAT *pWmat = pVal->pvParticleWmat;
    PARTICLE_COLOR *pPcol = pVal->pvParticleColor;
    OSHAPE_IN_VU1 *pShi0;
    OSHAPE_IN_VU1 *pShi1;
    OSHAPE_VU1 *pSh;
    OANM_HEADER *pAh;
    OANM_SEQ *pSeq;
    pppShapeSt *pShape;
    pwst *pw;
    int *idx_list = 0;
    int nNum;
    float *z_list;
    int i;

    if (!alloc_check(pVal, pParam)) {
        return;
    }
    if (pParam->nShape == 0xFFFF) {
        return;
    }
    if (!pParam->bDrawMode) {
        idx_list = pppAlloc(&ppvEnv->heapMng, pVal->nParticle * sizeof(int));
        z_list = pppAlloc(&ppvEnv->heapMng, pVal->nParticle * sizeof(float));
        if (idx_list) {
            if (z_list) {
                for (i = 0; i < pVal->nParticle; i++) {
                    idx_list[i] = i;
                    z_list[i] = pPdat[i].vecDir[3];
                }
                qsort_particle(idx_list, z_list, 0, pVal->nParticle - 1);
            } else {
                pppFree(&ppvEnv->heapMng, idx_list);
                idx_list = 0;
            }
        }
        if (z_list) {
            pppFree(&ppvEnv->heapMng, z_list);
        }
    }
    pppStartVu1ShapeProg(pParam->bFog);
    pShi0 = pppGetVu1DrawSpad0();
    pShi1 = pppGetVu1DrawSpad1();
    pw = op_spr_alloc(sizeof(pwst));
    nNum = pVal->nParticle;
    sceVu0CopyMatrix(pw->camMat, ppvCameraMatrix);
    sceVu0CopyMatrix(pw->scrMat, ppvScreenMatrix);
    sceVu0CopyMatrix(pw->calcMat, ppvPObj->cmat);
    sceVu0CopyVector(pw->parScl, ppvParScl);
    init_matrix(pobj, pw, pParam, pVal);
    pShape = &ppvEnv->lpShape[pParam->nShape];
    if (pParam->bZtest == 1) {
        pShi0->flag = 1;
    } else {
        pShi0->flag = 0;
    }
    if (pParam->bZcrct) {
        pShi0->addz = pParam->ZcrctPos.z * 0.01f;
    } else {
        pShi0->addz = 0.0f;
    }
    pAh = (OANM_HEADER *)pShape->shape;
    pw->fvColor[0] = pCol->release_col.r;
    pw->fvColor[1] = pCol->release_col.g;
    pw->fvColor[2] = pCol->release_col.b;
    pw->fvColor[3] = pCol->release_col.a;
    /* volatile read: keeps the ppvMng load after the fvColor stores, as in the original */
    sceVu0CopyVector(pw->fvRate, (*(pppMngSt * volatile *)&ppvMng)->parCol);
    pShi1->flag = pShi0->flag;
    pShi1->addz = pShi0->addz;
    while (nNum-- > 0) {
        PARTICLE_DATA *pdat;
        PARTICLE_WMAT *wmat = 0;
        PARTICLE_COLOR *pcol = 0;
        sceVu0FIVECTOR vCol;

        if (!idx_list) {
            pdat = &pPdat[nNum];
            if (pWmat) {
                wmat = &pWmat[nNum];
            }
            if (pPcol) {
                pcol = &pPcol[nNum];
            }
        } else {
            int n = idx_list[nNum];

            pdat = &pPdat[n];
            if (pWmat) {
                wmat = &pWmat[n];
            }
            if (pPcol) {
                pcol = &pPcol[n];
            }
        }
        if (!pdat->ushLife) {
            continue;
        }
        pShi0 = pppGetVu1DrawSpad();
        set_matrix(pobj, pw, pShi0->mat, pParam, pVal, pdat, wmat);
        if (!pParam->bDrawMode) {
            pdat->vecDir[3] = pShi0->mat[3][2];
        }
        pSeq = &pAh->seq[pdat->ushSeqNow];
        pSh = (OSHAPE_VU1 *)((char *)pAh + pSeq->sha);
        vCol.f[0] = pdat->chNoiseR;
        vCol.f[1] = pdat->chNoiseG;
        vCol.f[2] = pdat->chNoiseB;
        vCol.f[3] = pdat->chNoiseA - pdat->fAlphaDec;
        if (pcol) {
            sceVu0AddVector(vCol.f, vCol.f, pcol->vecCol);
        }
        sceVu0MulVector(vCol.f, vCol.f, pw->fvRate);
        sceVu0AddVector(vCol.f, vCol.f, pw->fvColor);
        sceVu0ClampVector(vCol.f, vCol.f, 0.0f, 255.0f);
        sceVu0FTOI0Vector(vCol.i, vCol.f);
        if (vCol.i[3] > 127) {
            vCol.i[3] = 127;
        }
        pShi0->r = (u_char)vCol.i[0];
        pShi0->g = (u_char)vCol.i[1];
        pShi0->b = (u_char)vCol.i[2];
        pShi0->a = (u_char)vCol.i[3];
        pppVu1ShapeDraw(pSh, pShi0);
    }
    pppWaitVif1();
    op_spr_free(sizeof(pwst));
    if (pParam->bZtest == 1) {
        op_ztest_init();
    }
    if (idx_list) {
        pppFree(&ppvEnv->heapMng, idx_list);
    }
}

void pppRyjMegaBirthCon(pppPObject *pobj, pppCtrlTable *pCtbl) {
    VRyjMegaBirth *pVal = (VRyjMegaBirth *)&pobj->val[pCtbl->useVal[RYJMEGABIRTH_THIS]];

    sceVu0UnitMatrix(pVal->matPar);
    *(u_long128 *)pVal->vecForce = 0;
    pVal->pvParticleData = 0;
    pVal->pvParticleWmat = 0;
    pVal->pvParticleColor = 0;
    pVal->nParticle = 0;
    pVal->ushTimingCt = 0;
    pVal->ushIct = 0;
    sceVu0UnitMatrix(g_matUnit);
}

void pppRyjMegaBirthDes(pppPObject *pobj, pppCtrlTable *pCtbl) {
    VRyjMegaBirth *pVal = (VRyjMegaBirth *)&pobj->val[pCtbl->useVal[RYJMEGABIRTH_THIS]];

    if (pVal->pvParticleData) {
        pppFree(&ppvEnv->heapMng, pVal->pvParticleData);
        pVal->pvParticleData = 0;
    }
    if (pVal->pvParticleWmat) {
        pppFree(&ppvEnv->heapMng, pVal->pvParticleWmat);
        pVal->pvParticleWmat = 0;
    }
    if (pVal->pvParticleColor) {
        pppFree(&ppvEnv->heapMng, pVal->pvParticleColor);
        pVal->pvParticleColor = 0;
    }
}
// SOURCE DATA: .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss .rodata
