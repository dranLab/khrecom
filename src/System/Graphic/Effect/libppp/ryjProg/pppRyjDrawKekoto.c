#include "pppRyjDrawKekoto.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"
#include "System/Graphic/Effect/libppp/draw/md.h"
#include "System/Graphic/Effect/libppp/draw/draw_w.h"
#include "System/Graphic/Effect/libppp/draw/global_w.h"


float sqrtf(float x);
void sceVu0ScaleVectorXYZ(sceVu0FVECTOR v0, sceVu0FVECTOR v1, float s);
void sceVu0SubVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);
void sceVu0FTOI4Vector(sceVu0IVECTOR v0, sceVu0FVECTOR v1);
int sceVu0ClipScreen(sceVu0FVECTOR v0);
sceDmaChan *sceDmaGetChan(int id);

typedef struct { /* 0x10 */
    char flag;
    char ptype;
    short pc;
    u_char tno;
    u_char timposx;
    u_char res[2];
    u_word tex0l;
    u_word tex0h;
} OMD_PRIM_H;

typedef union {
    char *pch;
    u_char *pb;
    short *psh;
    u_short *push;
    int *pn;
    u_int *pun;
    float *pf;
    long *pl;
    u_long *pul;
    long128 *pq;
    u_long128 *puq;
} RYJ_POINTER;

static void get_st(PRyjDrawKekoto *pParam, VRyjDrawKekoto *pVal) {
    float fS;
    float fT;
    float fX;

    switch (pParam->bTexS) {
    case 0:
    default:
        fS = 0.99f;
        break;
    case 1:
        fS = 0.49f;
        break;
    case 2:
        fS = 0.24f;
        break;
    }
    switch (pParam->bTexT) {
    case 0:
    default:
        fT = 0.99f;
        break;
    case 1:
        fT = 0.49f;
        break;
    case 2:
        fT = 0.24f;
        break;
    }
    switch (pParam->bTexX) {
    case 0:
    default:
        fX = 0.0f;
        break;
    case 1:
        fX = 0.25f;
        break;
    case 2:
        fX = 0.5f;
        break;
    case 3:
        fX = 0.75f;
        break;
    }
    pVal->vecStq[0][0] = pVal->vecStq[1][0] = pVal->vecStq[6][0] = pVal->vecStq[7][0] = fX + 0.01f;
    pVal->vecStq[2][0] = pVal->vecStq[3][0] = pVal->vecStq[4][0] = pVal->vecStq[5][0] = fX + fS;
    pVal->vecStq[1][1] = pVal->vecStq[3][1] = pVal->vecStq[5][1] = pVal->vecStq[7][1] = fT;
    pVal->vecStq[0][1] = pVal->vecStq[2][1] = pVal->vecStq[4][1] = pVal->vecStq[6][1] = 0.01f;
    pVal->vecStq[0][2] = pVal->vecStq[1][2] = pVal->vecStq[2][2] = pVal->vecStq[3][2] = pVal->vecStq[4][2] =
        pVal->vecStq[5][2] = pVal->vecStq[6][2] = pVal->vecStq[7][2] = 1.0f;
}

void pppRyjDrawKekoto(pppPObject *pPobj, PRyjDrawKekoto *pParam, pppCtrlTable *pCtbl) {
    VRyjDrawKekoto *pVal = (VRyjDrawKekoto *)&pPobj->val[pCtbl->useVal[RYJDRAWKEKOTO_THIS]];

    if (pParam->cdt.time == pPobj->time) {
        get_st(pParam, pVal);
    }
}

void pppRyjDrawDrawKekoto(pppPObject *pPobj, PRyjDrawKekoto *pParam, pppCtrlTable *pCtbl) {
    VRyjDrawKekoto *pVal;
    VColor *pCol;
    u_long128 *puqPacketTop;
    u_long *pulPacketGif;
    RYJ_POINTER Pt;
    sceVu0FVECTOR vecA;
    sceVu0FVECTOR vecB;
    sceVu0FVECTOR avecP[8];
    sceVu0IVECTOR aivecP[8];
    sceVu0IVECTOR ivecColA;
    sceVu0IVECTOR ivecColB;
    float afKeepQ[2];
    float fSizeA;
    float fSizeB;
    sceVu0FVECTOR vecV;
    sceVu0FVECTOR vecN;
    sceVu0FVECTOR vecT;
    pppModelSt *pMdl;
    OMD_H *pOmd;
    OMD_PRIM_H *pMph;
    OS_GS2_REGISTER *pGs2;
    int i;
    int j;

    pVal = (VRyjDrawKekoto *)&pPobj->val[pCtbl->useVal[RYJDRAWKEKOTO_THIS]];
    pCol = (VColor *)&pPobj->val[pCtbl->useVal[RYJDRAWKEKOTO_COLOR]];
    if (pParam->nModel == 0xFFFF) {
        return;
    }

    vecA[0] = -pParam->fLength;
    vecA[1] = 0.0f;
    vecA[2] = 0.0f;
    vecA[3] = 1.0f;
    vecB[0] = pParam->fLength;
    vecB[1] = 0.0f;
    vecB[2] = 0.0f;
    vecB[3] = 1.0f;

    if (pParam->bPers) {
        sceVu0FVECTOR vec0;
        sceVu0FVECTOR vec1;
        sceVu0FMATRIX mat;

        sceVu0MulMatrix(mat, ppvWorldMatrix, ppvPObj->cmat);
        sceVu0ApplyMatrix(vec0, mat, vecA);
        sceVu0CopyVector(vec1, vec0);
        vec1[0] += pParam->fWidth * 0.01f;
        sceVu0ApplyMatrix(vec0, ppvScreenMatrix, vec0);
        sceVu0ApplyMatrix(vec1, ppvScreenMatrix, vec1);
        if (vec0[3] > 0.0f && vec1[3] > 0.0f) {
            vec0[3] = 1.0f / vec0[3];
            sceVu0ScaleVectorXYZ(vec0, vec0, vec0[3]);
            vec1[3] = 1.0f / vec1[3];
            sceVu0ScaleVectorXYZ(vec1, vec1, vec1[3]);
            sceVu0SubVector(vec0, vec0, vec1);
            fSizeA = sqrtf(vec0[0] * vec0[0] + vec0[1] * vec0[1]);
        } else {
            fSizeA = 0.0f;
        }

        sceVu0ApplyMatrix(vec0, mat, vecB);
        sceVu0CopyVector(vec1, vec0);
        vec1[0] += pParam->fWidth * 0.01f;
        sceVu0ApplyMatrix(vec0, ppvScreenMatrix, vec0);
        sceVu0ApplyMatrix(vec1, ppvScreenMatrix, vec1);
        if (vec0[3] > 0.0f && vec1[3] > 0.0f) {
            vec0[3] = 1.0f / vec0[3];
            sceVu0ScaleVectorXYZ(vec0, vec0, vec0[3]);
            vec1[3] = 1.0f / vec1[3];
            sceVu0ScaleVectorXYZ(vec1, vec1, vec1[3]);
            sceVu0SubVector(vec0, vec0, vec1);
            fSizeB = sqrtf(vec0[0] * vec0[0] + vec0[1] * vec0[1]);
        } else {
            fSizeB = 0.0f;
        }

        if (fSizeA == 0.0f && fSizeB == 0.0f) {
            return;
        }
    } else {
        sceVu0FVECTOR vec0;
        sceVu0FVECTOR vec1;
        sceVu0FMATRIX mat;

        sceVu0MulMatrix(mat, ppvWorldMatrix, ppvPObj->cmat);
        vec0[0] = vec0[1] = vec0[2] = 0.0f;
        vec0[3] = 1.0f;
        sceVu0ApplyMatrix(vec0, mat, vec0);
        sceVu0CopyVector(vec1, vec0);
        vec1[0] += pParam->fWidth * 0.01f;
        sceVu0ApplyMatrix(vec0, ppvScreenMatrix, vec0);
        sceVu0ApplyMatrix(vec1, ppvScreenMatrix, vec1);
        if (vec0[3] > 0.0f && vec1[3] > 0.0f) {
            vec0[3] = 1.0f / vec0[3];
            sceVu0ScaleVectorXYZ(vec0, vec0, vec0[3]);
            vec1[3] = 1.0f / vec1[3];
            sceVu0ScaleVectorXYZ(vec1, vec1, vec1[3]);
            sceVu0SubVector(vec0, vec0, vec1);
            fSizeA = fSizeB = sqrtf(vec0[0] * vec0[0] + vec0[1] * vec0[1]);
        } else {
            fSizeA = fSizeB = 0.0f;
        }
    }

    sceVu0ApplyMatrix(avecP[2], ppvPObj->dmat, vecA);
    sceVu0ApplyMatrix(avecP[4], ppvPObj->dmat, vecB);
    if (avecP[2][3] <= 0.0f && avecP[4][3] <= 0.0f) {
        return;
    }
    if (avecP[2][3] > 0.0f) {
        afKeepQ[0] = avecP[2][3] = 1.0f / avecP[2][3];
        sceVu0ScaleVectorXYZ(avecP[2], avecP[2], afKeepQ[0]);
    }
    if (avecP[4][3] > 0.0f) {
        afKeepQ[1] = avecP[4][3] = 1.0f / avecP[4][3];
        sceVu0ScaleVectorXYZ(avecP[4], avecP[4], afKeepQ[1]);
    }

    vecV[0] = avecP[4][0] - avecP[2][0];
    vecV[1] = avecP[4][1] - avecP[2][1];
    vecV[2] = 0.0f;
    vecV[3] = 0.0f;
    sceVu0Normalize(vecV, vecV);
    vecN[0] = -vecV[1];
    vecN[1] = vecV[0];
    vecN[2] = 0.0f;
    vecN[3] = 0.0f;

    sceVu0ScaleVector(vecT, vecN, fSizeA);
    sceVu0AddVector(avecP[3], avecP[2], vecT);
    sceVu0ScaleVector(vecT, vecN, fSizeB);
    sceVu0AddVector(avecP[5], avecP[4], vecT);
    vecN[0] = -vecN[0];
    vecN[1] = -vecN[1];
    sceVu0ScaleVector(vecT, vecN, fSizeA);
    sceVu0AddVector(avecP[2], avecP[2], vecT);
    sceVu0ScaleVector(vecT, vecN, fSizeB);
    sceVu0AddVector(avecP[4], avecP[4], vecT);

    sceVu0ScaleVector(vecT, vecV, fSizeA);
    sceVu0SubVector(avecP[0], avecP[2], vecT);
    sceVu0SubVector(avecP[1], avecP[3], vecT);
    sceVu0ScaleVector(vecT, vecV, fSizeB);
    sceVu0AddVector(avecP[6], avecP[4], vecT);
    sceVu0AddVector(avecP[7], avecP[5], vecT);

    for (i = 0; i < 8; i++) {
        sceVu0FTOI4Vector(aivecP[i], avecP[i]);
    }

    ivecColA[0] = pCol->release_col.r * pParam->fColSclA_R;
    ivecColA[1] = pCol->release_col.g * pParam->fColSclA_G;
    ivecColA[2] = pCol->release_col.b * pParam->fColSclA_B;
    ivecColA[3] = pCol->release_col.a * pParam->fColSclA_A;
    ivecColB[0] = pCol->release_col.r * pParam->fColSclB_R;
    ivecColB[1] = pCol->release_col.g * pParam->fColSclB_G;
    ivecColB[2] = pCol->release_col.b * pParam->fColSclB_B;
    ivecColB[3] = pCol->release_col.a * pParam->fColSclB_A;
    for (i = 0; i < 4; i++) {
        if (ivecColA[i] < 0) {
            ivecColA[i] = 0;
        } else if (ivecColA[i] > 255) {
            ivecColA[i] = 255;
        }
        if (ivecColB[i] < 0) {
            ivecColB[i] = 0;
        } else if (ivecColB[i] > 255) {
            ivecColB[i] = 255;
        }
    }

    pMdl = &ppvEnv->lpModel[pParam->nModel];
    pOmd = pMdl->omd;
    pMph = (OMD_PRIM_H *)((char *)pOmd + pOmd->pa);
    {
        word spr2;
        word spr;
        word w;
        int nCt;

        spr = spr_a[spr_index++];
        spr_a[spr_index] = spr + 0x800;
        pulPacketGif = (u_long *)(spr + 0x50);
        puqPacketTop = (u_long128 *)((spr & 0x0FFFFFFF) | 0x80000000);
        pGs2 = &os_gs2_register;

        ((u_long *)spr)[2] = 0x3000000000000001;
        ((u_long *)spr)[3] = 0xEEE;
        ((u_int *)spr)[8] = pMph->tex0l;
        ((u_int *)spr)[9] = pMph->tex0h;
        ((u_long *)spr)[5] = 6;
        ((u_int *)spr)[12] = pOmd->alpha;
        ((u_int *)spr)[13] = pOmd->alphafix;
        ((u_long *)spr)[7] = 0x42;
        ((u_long128 *)spr)[4] = *(u_long128 *)&pGs2->zbuf;

        nCt = 0;
        Pt.puq = (u_long128 *)spr + 6;
        for (i = 0; i < 8; i++) {
            if (sceVu0ClipScreen(avecP[i]) == 0) {
                if (pParam->bPers) {
                    if (i < 4) {
                        sceVu0ScaleVector(vecT, pVal->vecStq[i], afKeepQ[0]);
                    } else {
                        sceVu0ScaleVector(vecT, pVal->vecStq[i], afKeepQ[1]);
                    }
                    *Pt.pf++ = vecT[0];
                    *Pt.pf++ = vecT[1];
                    *Pt.pf++ = vecT[2];
                } else {
                    *Pt.pf++ = pVal->vecStq[i][0];
                    *Pt.pf++ = pVal->vecStq[i][1];
                    *Pt.pf++ = pVal->vecStq[i][2];
                }
                *Pt.pn++ = 0;
                if (i < 4) {
                    for (j = 0; j < 4; j++) {
                        *Pt.pn++ = ivecColA[j];
                    }
                } else {
                    for (j = 0; j < 4; j++) {
                        *Pt.pn++ = ivecColB[j];
                    }
                }
                *Pt.pn++ = aivecP[i][0];
                *Pt.pn++ = aivecP[i][1];
                *Pt.pn++ = aivecP[i][2];
                *Pt.pn++ = 0;
                nCt++;
            }
        }
        pulPacketGif[0] = nCt | 0x8000 | ((u_long)0xC0B9 << 46);
        pulPacketGif[1] = 0x412;
        {
            word w;

            __asm__ volatile("sync.l");
            *(u_long128 *)spr = 0;
            *(u_int *)spr = (Pt.puq - (u_long128 *)spr - 1) | 0x70000000;
            FlushCache(0);
            sceDmaSend(sceDmaGetChan(2), puqPacketTop);
            sceGsSyncPath(0, 0);
            spr_index--;
        }
    }
}

void pppRyjDrawKekotoCon(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjDrawKekoto *pVal = (VRyjDrawKekoto *)&pPobj->val[pCtbl->useVal[RYJDRAWKEKOTO_THIS]];
    int i;

    for (i = 7; i >= 0; i--) {
        *(u_long128 *)pVal->vecStq[i] = 0;
    }
}
