#include "pppRyjMegaBirthModel.h"
#include "System/Graphic/Effect/libppp/pppAlloc.h"
#include "System/Graphic/Effect/libppp/draw/md.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"

void *memset(void *s, int c, unsigned int n);
void sceVu0ScaleVectorXYZ(sceVu0FVECTOR v0, sceVu0FVECTOR v1, float s);
void sceVu0RotMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1, sceVu0FVECTOR rot);

typedef struct _PARTICLE_DATA { /* 0xb0 */
    sceVu0FMATRIX matMdl;
    u_short ushLife;
    char chNoiseR;
    char chNoiseG;
    char chNoiseB;
    char chNoiseA;
    int anAng[3];
    int anAngV[3];
    int anAngRnd[3];
    float afScl[3];
    float afSclV[3];
    float afSclRnd[3];
    float fVelocity;
    float fGravity;
    float fU;
    float fV;
    float fUv;
    float fVv;
    float fAlphaDec;
    u_char bCt;
    u_char bFadeInFrame;
    u_char bFadeOutFrame;
} PARTICLE_DATA;

typedef struct _PARTICLE_WMAT { /* 0x40 */
    sceVu0FMATRIX matParMatrix;
} PARTICLE_WMAT;

typedef struct _PARTICLE_COLOR { /* 0x20 */
    sceVu0FVECTOR vecCol;
    sceVu0FVECTOR vecColV;
} PARTICLE_COLOR;

static float get_rand() {
    float r;

    PPP_RAND_F(r);
    return r;
}

static char get_noise(u_char bRange) {
    get_rand();
    return bRange * 2 * get_rand() - bRange / 2;
}

static char alloc_check(VRyjMegaBirthModel *pVal, PRyjMegaBirthModel *pParam) {
    if (!pVal->pvParticleData) {
        return 0;
    }
    if (pParam->bWorld && !pVal->pvParticleWmat) {
        return 0;
    }
    if (pParam->bColV && !pVal->pvParticleColor) {
        return 0;
    }
    return 1;
}

static void birth(VRyjMegaBirthModel *pVal, PRyjMegaBirthModel *pParam, VColor *pCol, PARTICLE_DATA *pPdat, PARTICLE_WMAT *pWmat, PARTICLE_COLOR *pPcol) {
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

    memset(pPdat, 0, sizeof(PARTICLE_DATA));
    if (pWmat) {
        memset(pWmat, 0, sizeof(PARTICLE_WMAT));
    }
    if (pPcol) {
        memset(pPcol, 0, sizeof(PARTICLE_COLOR));
    }
    sceVu0UnitMatrix(pPdat->matMdl);
    switch (pParam->bMode) {
    case MODE_SPHERE:
    case MODE_SPHERE_WOODROT:
    case MODE_DONUTS:
    case MODE_DONUTS_WOODROT:
    case MODE_BOX:
    case MODE_BOX_WOODROT:
    case MODE_VSF:
    case MODE_VSF_WOODROT:
        vecDir[0] = pParam->Dir.x;
        vecDir[1] = pParam->Dir.y;
        vecDir[2] = pParam->Dir.z;
        vecDir[3] = 1.0f;
        for (i = 0; i < 3; i++) {
            vecRot[i] = fDeg * get_rand() - fDegHalf;
            vecRot[i] = vecRot[i] * 3.1415927f / 180.0f;
        }
        if (pParam->bMode == MODE_DONUTS || pParam->bMode == MODE_DONUTS_WOODROT) {
            vecRot[0] = 0.0f;
            vecRot[1] = 0.0f;
        }
        sceVu0UnitMatrix(matRot);
        sceVu0RotMatrix(matRot, matRot, vecRot);
        sceVu0ApplyMatrix(pPdat->matMdl[1], matRot, vecDir);
        pPdat->matMdl[1][0] *= pParam->RangeScl.x;
        pPdat->matMdl[1][1] *= pParam->RangeScl.y;
        pPdat->matMdl[1][2] *= pParam->RangeScl.z;
        sceVu0Normalize(pPdat->matMdl[1], pPdat->matMdl[1]);
        sceVu0OuterProduct(pPdat->matMdl[2], pPdat->matMdl[1], pPdat->matMdl[0]);
        sceVu0Normalize(pPdat->matMdl[2], pPdat->matMdl[2]);
        sceVu0OuterProduct(pPdat->matMdl[0], pPdat->matMdl[2], pPdat->matMdl[1]);
        sceVu0Normalize(pPdat->matMdl[0], pPdat->matMdl[0]);
        break;
    }
    switch (pParam->bMode) {
    case MODE_SPHERE:
    case MODE_SPHERE_WOODROT:
    case MODE_DONUTS:
    case MODE_DONUTS_WOODROT:
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
            sceVu0ScaleVectorXYZ(pPdat->matMdl[3], pPdat->matMdl[1], fR);
            pPdat->matMdl[3][3] = 1.0f;
        }
        break;
    case MODE_BOX:
    case MODE_BOX_WOODROT:
        if (pParam->fRangeRadius != 0.0f) {
            fRangeRadiusHalf = pParam->fRangeRadius * 0.5f;
            switch (pParam->bRangeMode) {
            default:
            case 0:
                for (i = 0; i < 3; i++) {
                    pPdat->matMdl[3][i] = pParam->fRangeRadius * get_rand() - fRangeRadiusHalf;
                }
                break;
            case 1:
                get_rand();
                for (i = 0; i < 3; i++) {
                    pPdat->matMdl[3][i] = pParam->fRangeRadius * get_rand() - fRangeRadiusHalf;
                }
                break;
            case 2:
                for (i = 0; i < 3; i++) {
                    fRnd = get_rand();
                    pPdat->matMdl[3][i] = pParam->fRangeRadius * fRnd * get_rand() - fRangeRadiusHalf;
                }
                break;
            case 3:
                for (i = 0; i < 3; i++) {
                    float r1 = get_rand();

                    pPdat->matMdl[3][i] = pParam->fRangeRadius - pParam->fRangeRadius * r1 * get_rand() * 0.7f - fRangeRadiusHalf;
                }
                break;
            case 4:
                for (i = 0; i < 3; i++) {
                    float r1 = get_rand();
                    float r2 = get_rand();
                    float r3 = get_rand();

                    pPdat->matMdl[3][i] = pParam->fRangeRadius * r1 * r2 * r3 * get_rand() - fRangeRadiusHalf;
                }
                break;
            case 5:
                for (i = 0; i < 3; i++) {
                    float r1 = get_rand();
                    float r2 = get_rand();

                    pPdat->matMdl[3][i] = pParam->fRangeRadius - pParam->fRangeRadius * r1 * r2 * get_rand() * 0.5f - fRangeRadiusHalf;
                }
                break;
            }
            pPdat->matMdl[3][0] *= pParam->RangeScl.x;
            pPdat->matMdl[3][1] *= pParam->RangeScl.y;
            pPdat->matMdl[3][2] *= pParam->RangeScl.z;
            pPdat->matMdl[3][3] = 1.0f;
        }
        break;
    case MODE_VSF:
    case MODE_VSF_WOODROT:
    case MODE_VSF_EXP:
    case MODE_VSF_EXP_WOODROT:
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
            pPdat->matMdl[3][0] = Vtx.x * pParam->RangeScl.x;
            pPdat->matMdl[3][1] = Vtx.y * pParam->RangeScl.y;
            pPdat->matMdl[3][2] = Vtx.z * pParam->RangeScl.z;
            pPdat->matMdl[3][3] = 1.0f;
            if (pParam->bMode == MODE_VSF_EXP || pParam->bMode == MODE_VSF_EXP_WOODROT) {
                sceVu0Normalize(pPdat->matMdl[1], pPdat->matMdl[3]);
                pPdat->matMdl[1][0] *= pParam->RangeScl.x;
                pPdat->matMdl[1][1] *= pParam->RangeScl.y;
                pPdat->matMdl[1][2] *= pParam->RangeScl.z;
                sceVu0Normalize(pPdat->matMdl[1], pPdat->matMdl[1]);
                sceVu0OuterProduct(pPdat->matMdl[2], pPdat->matMdl[1], pPdat->matMdl[0]);
                sceVu0Normalize(pPdat->matMdl[2], pPdat->matMdl[2]);
                sceVu0OuterProduct(pPdat->matMdl[0], pPdat->matMdl[2], pPdat->matMdl[1]);
                sceVu0Normalize(pPdat->matMdl[0], pPdat->matMdl[0]);
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
    pPdat->fVelocity = pParam->fMov;
    pPdat->fGravity = pParam->fForce;
    if (pParam->fMovRndRange != 0.0f) {
        pPdat->fVelocity += pParam->fMovRndRange * 2.0f * get_rand() - pParam->fMovRndRange;
    }
    pPdat->fU = pParam->fU;
    pPdat->fV = pParam->fV;
    pPdat->fUv = pParam->fUMov;
    pPdat->fVv = pParam->fVMov;
    switch (pParam->bUvRnd) {
    case UVRND_U:
        pPdat->fU *= get_rand();
        break;
    case UVRND_U_V:
        pPdat->fU *= get_rand();
    case UVRND_V:
        pPdat->fV *= get_rand();
        break;
    case UVRND_UV: {
        float fRnd = get_rand();

        pPdat->fU *= fRnd;
        pPdat->fV *= fRnd;
    }
        break;
    }
    if (pParam->ushLife) {
        pPdat->ushLife = pParam->ushLife;
    } else {
        pPdat->ushLife = 0xFFFF;
    }
    pPdat->bCt = 0;
    if (pWmat) {
        sceVu0CopyMatrix(pWmat->matParMatrix, ppvParMatrix);
    }
    if (pPcol) {
        pPcol->vecColV[0] = pParam->fColMovR;
        pPcol->vecColV[1] = pParam->fColMovG;
        pPcol->vecColV[2] = pParam->fColMovB;
        pPcol->vecColV[3] = pParam->fColMovA;
    }
}

static void calc(VRyjMegaBirthModel *pVal, PRyjMegaBirthModel *pParam, PARTICLE_DATA *pPdat, VColor *pCol, PARTICLE_COLOR *pPcol) {
    int nA = pCol->release_col.a;
    int i;

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
    pPdat->fVelocity += pParam->fAcc;
    if (!pParam->bLimitOff) {
        if (pParam->fMov > 0.0f && pParam->fAcc < 0.0f) {
            if (pPdat->fVelocity < 0.0f) {
                pPdat->fVelocity = 0.0f;
            }
        } else if (pParam->fMov < 0.0f && pParam->fAcc > 0.0f) {
            if (pPdat->fVelocity > 0.0f) {
                pPdat->fVelocity = 0.0f;
            }
        }
    }
    pPdat->fGravity += pParam->fForceAcc;
    /* matMdl[3] += matMdl[1] * fVelocity + vecForce * fGravity; the unused last operand
       (&fVelocity again) reproduces the original's register allocation */
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
                         : "r"(pPdat->matMdl[3]), "r"(pPdat->matMdl[1]), "r"(&pPdat->fVelocity), "r"(pVal->vecForce), "r"(&pPdat->fGravity), "r"(&pPdat->fVelocity)
                         : "$8", "$9", "memory");
    pPdat->fU += pPdat->fUv;
    pPdat->fV += pPdat->fVv;
    pPdat->fUv += pParam->fUAcc;
    pPdat->fVv += pParam->fVAcc;
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
}

static void calc_particle(VRyjMegaBirthModel *pVal, PRyjMegaBirthModel *pParam, VColor *pCol) {
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
    pVal->ushTimingCt++;
    for (i = 0; i < nNum; i++) {
        if (pPdat->ushLife) {
            calc(pVal, pParam, pPdat, pCol, pPcol);
        } else if (pParam->ushTiming <= pVal->ushTimingCt && nBirthNum < pParam->ushNumOneF) {
            birth(pVal, pParam, pCol, pPdat, pWmat, pPcol);
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

void pppRyjMegaBirthModel(pppPObject *pPobj, PRyjMegaBirthModel *pParam, pppCtrlTable *pCtbl) {
    VRyjMegaBirthModel *pVal = (VRyjMegaBirthModel *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHMODEL_THIS]];
    VColor *pCol = (VColor *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHMODEL_COLOR]];

    if (!pVal->pvParticleData) {
        /* not in the original debug info: see pppRyjMegaBirthFilter */
        u_int n = pParam->ushNum;

        *(volatile u_int *)&pVal->nParticle = n;
        pVal->pvParticleData = pppAlloc(&ppvEnv->heapMng, n * sizeof(PARTICLE_DATA));
        if (pVal->pvParticleData) {
            memset(pVal->pvParticleData, 0, pVal->nParticle * sizeof(PARTICLE_DATA));
        }
        if (pParam->bWorld) {
            pVal->pvParticleWmat = pppAlloc(&ppvEnv->heapMng, pVal->nParticle * sizeof(PARTICLE_WMAT));
            if (pVal->pvParticleWmat) {
                memset(pVal->pvParticleWmat, 0, pVal->nParticle * sizeof(PARTICLE_WMAT));
            }
        }
        if (pParam->bColV) {
            pVal->pvParticleColor = pppAlloc(&ppvEnv->heapMng, pVal->nParticle * sizeof(PARTICLE_COLOR));
            if (pVal->pvParticleColor) {
                memset(pVal->pvParticleColor, 0, pVal->nParticle * sizeof(PARTICLE_COLOR));
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
    calc_particle(pVal, pParam, pCol);
}

static void init_matrix(sceVu0FMATRIX matCore, PRyjMegaBirthModel *pParam, VRyjMegaBirthModel *pVal) {
    PARTICLE_WMAT *pWmat = pVal->pvParticleWmat;

    sceVu0UnitMatrix(matCore);
    switch (pParam->bMode) {
    case MODE_SPHERE:
    case MODE_DONUTS:
    case MODE_BOX:
    case MODE_VSF:
    case MODE_VSF_EXP:
    default:
        if (!pWmat) {
            sceVu0MulMatrix(matCore, ppvWorldMatrix, ppvPObj->cmat);
        }
        break;
    case MODE_SPHERE_WOODROT:
    case MODE_DONUTS_WOODROT:
    case MODE_BOX_WOODROT:
    case MODE_VSF_WOODROT:
    case MODE_VSF_EXP_WOODROT:
        sceVu0MulMatrix(matCore, ppvWorldMatrix, ppvPObj->cmat);
        break;
    }
}

static void set_matrix(sceVu0FMATRIX matCore, sceVu0FMATRIX matIn, PRyjMegaBirthModel *pParam, PARTICLE_DATA *pPdat, PARTICLE_WMAT *pWmat, sceVu0FMATRIX matZ, u_char bZcrct) {
    sceVu0FVECTOR vec;
    sceVu0FMATRIX mat;
    int i;

    if (pParam->bYaxis) {
        sceVu0CopyMatrix(matIn, pPdat->matMdl);
    } else {
        sceVu0UnitMatrix(matIn);
        sceVu0CopyVector(matIn[3], pPdat->matMdl[3]);
    }
    if (*(u_long *)pPdat->anAng || pPdat->anAng[2]) {
        sceVu0FMATRIX matRot;
        sceVu0FVECTOR vecRot;

        for (i = 0; i < 3; i++) {
            vecRot[i] = pPdat->anAng[i] * 3.1415927f / 32768.0f;
        }
        sceVu0UnitMatrix(matRot);
        sceVu0RotMatrix(matRot, matRot, vecRot);
        sceVu0MulMatrix(matIn, matIn, matRot);
    }
    switch (pParam->bMode) {
    case MODE_SPHERE:
    case MODE_DONUTS:
    case MODE_BOX:
    case MODE_VSF:
    case MODE_VSF_EXP:
    default:
        for (i = 0; i < 3; i++) {
            sceVu0ScaleVector(matIn[i], matIn[i], pPdat->afScl[i]);
        }
        if (!pWmat) {
            sceVu0CopyVector(vec, matIn[3]);
            sceVu0MulMatrix(matIn, ppvWorldMatrix, matIn);
            sceVu0ApplyMatrix(matIn[3], matCore, vec);
            if (bZcrct) {
                sceVu0CopyMatrix(matZ, matIn);
            }
            sceVu0MulMatrix(matIn, ppvScreenMatrix, matIn);
        } else {
            sceVu0MulMatrix(mat, pWmat->matParMatrix, ppvPObj->cmat);
            sceVu0MulMatrix(mat, ppvCameraMatrix, mat);
            sceVu0CopyVector(vec, matIn[3]);
            sceVu0MulMatrix(matIn, ppvWorldMatrix, matIn);
            sceVu0ApplyMatrix(matIn[3], mat, vec);
            if (bZcrct) {
                sceVu0CopyMatrix(matZ, matIn);
            }
            sceVu0MulMatrix(matIn, ppvScreenMatrix, matIn);
        }
        break;
    case MODE_SPHERE_WOODROT:
    case MODE_DONUTS_WOODROT:
    case MODE_BOX_WOODROT:
    case MODE_VSF_WOODROT:
    case MODE_VSF_EXP_WOODROT:
        for (i = 0; i < 3; i++) {
            sceVu0ScaleVector(matIn[i], matIn[i], pPdat->afScl[i] * ppvParScl[i]);
        }
        sceVu0ApplyMatrix(vec, matCore, matIn[3]);
        sceVu0MulMatrix(matIn, ppvWorldMatrixWood, matIn);
        sceVu0CopyVector(matIn[3], vec);
        if (bZcrct) {
            sceVu0CopyMatrix(matZ, matIn);
        }
        sceVu0MulMatrix(matIn, ppvScreenMatrix, matIn);
        break;
    }
}

void pppRyjDrawMegaBirthModel(pppPObject *pPobj, PRyjMegaBirthModel *pParam, pppCtrlTable *pCtbl) {
    VRyjMegaBirthModel *pVal = (VRyjMegaBirthModel *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHMODEL_THIS]];
    VColor *pCol = (VColor *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHMODEL_COLOR]];
    PARTICLE_DATA *pPdat = pVal->pvParticleData;
    PARTICLE_WMAT *pWmat = pVal->pvParticleWmat;
    PARTICLE_COLOR *pPcol = pVal->pvParticleColor;
    int nNum = pVal->nParticle;
    sceVu0FMATRIX matCore;
    sceVu0FMATRIX matIn;
    sceVu0FMATRIX matZ;
    sceVu0FVECTOR vecZPos;
    OMD_IN Mdi;
    pppModelSt *pMdl;
    OMD_H *pOmd;
    int nRed;
    int nGre;
    int nBlu;
    int nAlp;
    int i;

    if (!alloc_check(pVal, pParam)) {
        return;
    }
    if (pParam->nModel == 0xFFFF) {
        return;
    }
    init_matrix(matCore, pParam, pVal);
    pMdl = &ppvEnv->lpModel[pParam->nModel];
    pOmd = pMdl->omd;
    if (pParam->bZcrct) {
        vecZPos[0] = pParam->ZcrctPos.x;
        vecZPos[1] = pParam->ZcrctPos.y;
        vecZPos[2] = pParam->ZcrctPos.z;
        vecZPos[3] = 0.0f;
        vecZPos[0] *= 0.01f;
        vecZPos[1] *= -0.01f;
        vecZPos[2] *= -0.01f;
        Mdi.pzmat = &matZ;
    } else {
        Mdi.pzmat = 0;
    }
    Mdi.lwmat = 0;
    Mdi.lvmat = 0;
    Mdi.lcmat = 0;
    Mdi.pri = 0;
    *(int *)&Mdi.ar = 0;
    Mdi.va = ppvPObj->lpvert;
    Mdi.na = ppvPObj->lpnorm;
    Mdi.tmp_a = (char *)ppvVNWork;
    Mdi.flag = 0x20;
    Mdi.pmat = &matIn;
    *(int *)&Mdi.toff = 0;
    *(u_long *)&Mdi.u = 0;
    *(int *)&Mdi.ot = 0;
    if (pParam->bDrawMode & 2) {
        Mdi.flag = 0x24;
        if (!(pOmd->flag & 1)) {
            Mdi.flag |= 8;
        }
    } else {
        if (pParam->bDrawMode & 1) {
            Mdi.flag = 0xA0;
        }
        if (!(pOmd->flag & 1)) {
            Mdi.flag |= 8;
        }
    }
    if (!(pOmd->flag & 2)) {
        Mdi.flag |= 0x10;
    }
    if (pParam->bFog) {
        Mdi.flag |= 0x200;
    }
    if (pParam->bNear) {
        Mdi.flag |= 0x100;
    }
    *(u_short *)&Mdi.alpha = *(u_short *)&pOmd->alpha;
    Mdi.invisible = pParam->bInvisible;
    Mdi.ztest_off = pParam->bZtest;
    nRed = pCol->release_col.r;
    nGre = pCol->release_col.g;
    nBlu = pCol->release_col.b;
    nAlp = pCol->release_col.a;
    for (i = 0; i < nNum; i++) {
        if (pPdat->ushLife) {
            int nR;
            int nG;
            int nB;
            int nA;

            set_matrix(matCore, matIn, pParam, pPdat, pWmat, matZ, pParam->bZcrct);
            if (pParam->bZcrct) {
                sceVu0AddVector(matZ[3], matZ[3], vecZPos);
                sceVu0MulMatrix(matZ, ppvScreenMatrix, matZ);
            }
            nR = nRed + pPdat->chNoiseR;
            nG = nGre + pPdat->chNoiseG;
            nB = nBlu + pPdat->chNoiseB;
            nA = (float)nAlp + pPdat->chNoiseA - pPdat->fAlphaDec;
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
            Mdi.r = nR;
            Mdi.g = nG;
            Mdi.b = nB;
            Mdi.a = nA;
            Mdi.u = pPdat->fU * 65536.0f;
            Mdi.v = pPdat->fV * 65536.0f;
            Mdi.packet = ppvCurPrimp;
            ppvCurPrimp = op1_md_draw(pOmd, &Mdi, pMdl->nPacketSize);
        }
        pPdat++;
        if (pWmat) {
            pWmat++;
        }
        if (pPcol) {
            pPcol++;
        }
    }
}


void pppRyjMegaBirthModelCon(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjMegaBirthModel *pVal = (VRyjMegaBirthModel *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHMODEL_THIS]];

    *(u_long128 *)pVal->vecForce = 0;
    pVal->pvParticleData = 0;
    pVal->pvParticleWmat = 0;
    pVal->pvParticleColor = 0;
    pVal->nParticle = 0;
    pVal->ushTimingCt = 0;
    pVal->ushIct = 0;
}

void pppRyjMegaBirthModelDes(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjMegaBirthModel *pVal = (VRyjMegaBirthModel *)&pPobj->val[pCtbl->useVal[RYJMEGABIRTHMODEL_THIS]];

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
// SOURCE DATA: .rodata
