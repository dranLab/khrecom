#include "pppRyjDrawShipolyBone.h"
#include "System/Graphic/Effect/libppp/pppAlloc.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"
#include "System/Graphic/Effect/libppp/draw/md.h"
#include "System/Graphic/Effect/libppp/draw/md0_p3f.h"
#include "System/Debug.h"

extern sceVu0FMATRIX ppvSCMatrix;
void sceVu0SubVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);
void sceVu0InterVector(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2, float t);

typedef struct { /* 0x18 */
    u_char r0, g0, b0, a0;
    u_char r1, g1, b1, a1;
    u_char r2, g2, b2, a2;
    u_char r3, g3, b3, a3;
    u_short v0, v1, v2, v3;
} OMD_F4;

typedef struct { /* 0x28 */
    u_char r0, g0, b0, a0;
    u_char r1, g1, b1, a1;
    u_char r2, g2, b2, a2;
    u_char r3, g3, b3, a3;
    u_short v0, v1, v2, v3;
    u_short s0, t0;
    u_short s1, t1;
    u_short s2, t2;
    u_short s3, t3;
} OMD_FT4;

static void init_spline(sceVu0FVECTOR *x, int n, sceVu0FVECTOR *table) {
    sceVu0FVECTOR d[128];
    static sceVu0FVECTOR c = { 4.0f, -1.0f, 0.0f, 0.0f };

    __asm__ volatile(
        ".set noreorder\n"
        "addi $t1, $a1, -1\n"
        "addi $t2, $a1, -2\n"
        "lqc2 $vf1, 0x0(%0)\n"
        "sqc2 $vf0, 0x0($a2)\n"
        "sll $t3, $t1, 4\n"
        "add $t3, $t3, $a2\n"
        "sqc2 $vf0, 0x0($t3)\n"
        "daddu $t0, $t1, $zero\n"
        "daddu $t3, $zero, $zero\n"
        "1:\n"
        "blez $t0, 2f\n"
        "addi $t0, $t0, -1\n"
        "sll $t4, $t3, 4\n"
        "addi $t5, $t4, 0x10\n"
        "add $t6, $t4, $a0\n"
        "add $t7, $t5, $a0\n"
        "add $s0, $t5, $sp\n"
        "lqc2 $vf10, 0x0($t6)\n"
        "lqc2 $vf11, 0x0($t7)\n"
        "vsub.xyzw $vf10, $vf11, $vf10\n"
        "sqc2 $vf10, 0x0($s0)\n"
        "b 1b\n"
        "addi $t3, $t3, 1\n"
        "2:\n"
        "lqc2 $vf10, 0x20($sp)\n"
        "lqc2 $vf11, 0x10($sp)\n"
        "lqc2 $vf12, 0x0($a2)\n"
        "vsub.xyzw $vf10, $vf10, $vf11\n"
        "vsub.xyzw $vf10, $vf10, $vf12\n"
        "sqc2 $vf10, 0x10($a2)\n"
        "vaddx.xyzw $vf11, $vf0, $vf1x\n"
        "sqc2 $vf11, 0x10($sp)\n"
        "daddu $t0, $t2, $zero\n"
        "addi $t0, $t0, -1\n"
        "addi $t3, $zero, 1\n"
        "3:\n"
        "blez $t0, 4f\n"
        "addi $t0, $t0, -1\n"
        "sll $t4, $t3, 4\n"
        "addi $t5, $t4, 0x10\n"
        "addi $t6, $t5, 0x10\n"
        "add $t7, $t4, $sp\n"
        "add $s0, $t5, $sp\n"
        "add $s1, $t6, $sp\n"
        "add $s2, $t5, $a2\n"
        "add $s3, $t4, $a2\n"
        "lqc2 $vf10, 0x0($t7)\n"
        "lqc2 $vf11, 0x0($s0)\n"
        "lqc2 $vf12, 0x0($s1)\n"
        "lqc2 $vf13, 0x0($s3)\n"
        "vdiv $Q, $vf0w, $vf10x\n"
        "vwaitq\n"
        "vaddq.x $vf15, $vf0, $Q\n"
        "vdiv $Q, $vf0w, $vf10y\n"
        "vwaitq\n"
        "vaddq.y $vf15, $vf0, $Q\n"
        "vdiv $Q, $vf0w, $vf10z\n"
        "vwaitq\n"
        "vaddq.z $vf15, $vf0, $Q\n"
        "vsub.xyzw $vf16, $vf12, $vf11\n"
        "vmul.xyzw $vf17, $vf13, $vf15\n"
        "vsub.xyzw $vf17, $vf16, $vf17\n"
        "sqc2 $vf17, 0x0($s2)\n"
        "vmuly.xyzw $vf18, $vf15, $vf1y\n"
        "vaddx.xyzw $vf18, $vf18, $vf1x\n"
        "sqc2 $vf18, 0x0($s0)\n"
        "b 3b\n"
        "addi $t3, $t3, 1\n"
        "4:\n"
        "sll $t3, $t1, 4\n"
        "add $t3, $t3, $a2\n"
        "sll $t4, $t2, 4\n"
        "add $t4, $t4, $a2\n"
        "lqc2 $vf10, 0x0($t3)\n"
        "lqc2 $vf11, 0x0($t4)\n"
        "vsub.xyzw $vf11, $vf11, $vf10\n"
        "sqc2 $vf11, 0x0($t4)\n"
        "daddu $t3, $t2, $zero\n"
        "5:\n"
        "blezl $t3, 6f\n"
        "nop\n"
        "sll $t4, $t3, 4\n"
        "addi $t5, $t4, 0x10\n"
        "add $t7, $t4, $sp\n"
        "add $s0, $t4, $a2\n"
        "add $s1, $t5, $a2\n"
        "lqc2 $vf10, 0x0($t7)\n"
        "lqc2 $vf11, 0x0($s0)\n"
        "lqc2 $vf12, 0x0($s1)\n"
        "vsub.xyzw $vf11, $vf11, $vf12\n"
        "vdiv $Q, $vf11x, $vf10x\n"
        "vwaitq\n"
        "vaddq.x $vf15, $vf0, $Q\n"
        "vdiv $Q, $vf11y, $vf10y\n"
        "vwaitq\n"
        "vaddq.y $vf15, $vf0, $Q\n"
        "vdiv $Q, $vf11z, $vf10z\n"
        "vwaitq\n"
        "vaddq.z $vf15, $vf0, $Q\n"
        "sqc2 $vf15, 0x0($s0)\n"
        "b 5b\n"
        "addi $t3, $t3, -1\n"
        "6:\n"
        ".set reorder\n"
        :
        : "r"(c)
        : "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$16", "$17", "$18", "$19", "memory");
}

static void spline(float *out, sceVu0FVECTOR *x, float t, sceVu0FVECTOR *table) {
    int i;
    float d;
    static sceVu0FVECTOR c = { 3.0f, 2.0f, 0.0f, 0.0f };

    i = t;
    d = t - i;
    x += i;
    table += i;
    c[2] = d;
    __asm__ volatile(
        "lqc2 $vf1, 0x0(%1)\n"
        "lqc2 $vf2, 0x10(%1)\n"
        "lqc2 $vf3, 0x0(%2)\n"
        "lqc2 $vf4, 0x10(%2)\n"
        "lqc2 $vf5, 0x0(%3)\n"
        "vsub.xyzw $vf10, $vf4, $vf3\n"
        "vmuly.xyzw $vf11, $vf3, $vf5y\n"
        "vmulx.xyzw $vf12, $vf3, $vf5x\n"
        "vsub.xyzw $vf13, $vf2, $vf1\n"
        "vmulz.xyzw $vf10, $vf10, $vf5z\n"
        "vadd.xyzw $vf11, $vf11, $vf4\n"
        "vadd.xyzw $vf10, $vf10, $vf12\n"
        "vsub.xyzw $vf13, $vf13, $vf11\n"
        "vmulz.xyzw $vf10, $vf10, $vf5z\n"
        "vadd.xyzw $vf10, $vf10, $vf13\n"
        "vmulz.xyzw $vf10, $vf10, $vf5z\n"
        "vadd.xyzw $vf10, $vf10, $vf1\n"
        "sqc2 $vf10, 0x0(%0)\n"
        :
        : "r"(out), "r"(x), "r"(table), "r"(c)
        : "memory");
}

static void get_st(PRyjDrawShipolyBoneTex *pParam, int *pnSMin, int *pnSMax, int *pnTMin, int *pnTMax) {
    int nSMin;
    int nSMax;
    int nTMax;

    nSMin = 0;
    nSMax = 0x1000;
    nTMax = 0x1000;
    switch (pParam->bTexS) {
    case 1:
        nSMax = 0x800;
        break;
    case 2:
        nSMax = 0x400;
        break;
    }
    switch (pParam->bTexT) {
    case 1:
        nTMax >>= 1;
        break;
    case 2:
        nTMax >>= 2;
        break;
    }
    switch (pParam->bTexX) {
    case 1:
        nSMin = 0x400;
        nSMax += 0x400;
        break;
    case 2:
        nSMin = 0x800;
        nSMax += 0x800;
        break;
    case 3:
        nSMin = 0xC00;
        nSMax += 0xC00;
        break;
    }
    *pnSMin = nSMin + 0x10;
    *pnSMax = nSMax - 0x10;
    *pnTMin = 0x10;
    *pnTMax = nTMax - 0x10;
}

static void shipoly(pppModelSt *pMdl, PRyjDrawShipolyBone *pParam, VRyjDrawShipolyBone *pVal) {
    sceVu0FVECTOR avecSpline[128];
    sceVu0FVECTOR avecSplineTbl[128];
    u_int unFrameCt;
    u_int unSmp;
    sceVu0FVECTOR *pvecSmp;
    int nPoly;
    int nStandbyNum;
    int i;
    int j;
    int k;
    int n;
    PRyjDrawShipolyBoneTex *pParamTex;

    unFrameCt = pVal->unFrameCt;
    unSmp = pVal->bSmp;
    nStandbyNum = 0;
    nPoly = pMdl->npoly;
    pvecSmp = pVal->pvecSmp;
    if (!pvecSmp) {
        return;
    }
    pParamTex = &((PRyjDrawShipolyBoneOld *)pParam)->sTex;
    if ((u_char)*ppvMng->pchDpd >= 150) {
        pParamTex = &pParam->sTex;
    }
    sceVu0CopyVector(pvecSmp[unFrameCt * 2], ppvReferencePosList[0]);
    sceVu0CopyVector(pvecSmp[unFrameCt * 2 + 1], ppvReferencePosList[1]);
    {
        pppSVECTOR *pVt = pMdl->vert;

        for (j = 0; j < 2; j++) {
            for (k = 0; k < unSmp; k++) {
                n = unFrameCt - k;
                if (n < 0) {
                    n += unSmp;
                }
                sceVu0SubVector(avecSpline[k], pvecSmp[n * 2 + j], ppvReferencePosList[0]);
            }
            init_spline(avecSpline, unSmp, avecSplineTbl);
            for (k = 0; k < nPoly + 1; k++) {
                sceVu0FVECTOR vecOut;

                spline(vecOut, avecSpline, (float)k / (float)nPoly * (float)(unSmp - 1), avecSplineTbl);
                pVt[k * 2 + j].x = vecOut[0] * 100.0f;
                pVt[k * 2 + j].y = vecOut[1] * 100.0f;
                pVt[k * 2 + j].z = vecOut[2] * 100.0f;
            }
        }
    }
    if (pVal->bStandby) {
        pppSVECTOR *pVt = pMdl->vert;

        for (i = nPoly; i > 0; i--) {
            if (pVt[i * 2].x != pVt[i * 2 - 2].x || pVt[i * 2].y != pVt[i * 2 - 2].y ||
                pVt[i * 2].z != pVt[i * 2 - 2].z) {
                break;
            }
            nStandbyNum++;
        }
    }
    if (pParamTex->bThin) {
        sceVu0FVECTOR avecP[2];
        pppSVECTOR *pVt;
        float fR;
        float fThinR;

        pVt = pMdl->vert;
        fR = 1.0f;
        if (nPoly - nStandbyNum <= 0) {
            fThinR = fR;
        } else {
            fThinR = fR / (nStandbyNum > 0 ? (float)(nPoly - nStandbyNum) : (float)nPoly);
        }
        for (i = 0; i < nPoly + 1; i++) {
            for (j = 0; j < 2; j++) {
                avecP[j][0] = pVt[i * 2 + j].x;
                avecP[j][1] = pVt[i * 2 + j].y;
                avecP[j][2] = pVt[i * 2 + j].z;
            }
            sceVu0InterVector(avecP[0], avecP[0], avecP[1], fR);
            pVt[i * 2].x = avecP[0][0];
            pVt[i * 2].y = avecP[0][1];
            pVt[i * 2].z = avecP[0][2];
            fR -= fThinR;
            if (fR <= 0.0f) {
                fR = 0.0f;
            }
        }
    }
    if (pVal->bStandby) {
        OMD_H *pOmd;
        OMD_PRIM_H *pMph;
        int nSAdd;
        int nS;
        int nSMin;
        int nSMax;
        int nTMin;
        int nTMax;
        float fAlpha;
        float fAlphaDel;
        int nCt;
        int nSizeAdd;

        pOmd = pMdl->omd;
        pMph = (OMD_PRIM_H *)((char *)pOmd + pOmd->pa);
        nCt = nSAdd = 0;
        fAlpha = 128.0f;
        fAlphaDel = 0.0f;
        get_st(pParamTex, &nSMin, &nSMax, &nTMin, &nTMax);
        if (nPoly - nStandbyNum <= 0) {
            fAlpha = fAlphaDel;
            nS = 0;
            nSAdd = 0;
        } else {
            nS = nSMin;
            fAlphaDel = fAlpha / (nPoly - nStandbyNum);
            nSAdd = (nSMax - nSMin) / (nPoly - nStandbyNum);
        }
        while (pMph->ptype != -1) {
            nSizeAdd = 0;
            switch (pMph->ptype) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 5:
            case 7:
                return;
            case 4: {
                OMD_F4 *pPoly = (OMD_F4 *)(pMph + 1);

                for (i = 0; i < pMph->pc; i++) {
                    if (nCt >= nPoly - nStandbyNum) {
                        pPoly->a0 = pPoly->a1 = pPoly->a2 = pPoly->a3 = 0;
                    } else {
                        pPoly->a0 = pPoly->a1 = fAlpha;
                        pPoly->a2 = pPoly->a3 = (u_char)fAlpha - fAlphaDel;
                    }
                    fAlpha -= fAlphaDel;
                    nS += nSAdd;
                    if (nS > nSMax) {
                        nS = nSMax;
                    }
                    if (fAlpha < 0.0f) {
                        fAlpha = 0.0f;
                    }
                    pPoly++;
                    nCt++;
                }
                nSizeAdd = sizeof(OMD_F4);
                break;
            }
            case 6: {
                OMD_FT4 *pPoly = (OMD_FT4 *)(pMph + 1);

                for (i = 0; i < pMph->pc; i++) {
                    if (nCt >= nPoly - nStandbyNum) {
                        pPoly->a0 = pPoly->a1 = pPoly->a2 = pPoly->a3 = 0;
                    } else {
                        pPoly->a0 = pPoly->a1 = fAlpha;
                        pPoly->a2 = pPoly->a3 = (u_char)fAlpha - fAlphaDel;
                    }
                    pPoly->s0 = pPoly->s1 = nS;
                    pPoly->b0 = pPoly->b1 = pPoly->b2 = pPoly->b3 = 0xFF;
                    pPoly->g0 = pPoly->g1 = pPoly->g2 = pPoly->g3 = 0xFF;
                    pPoly->r0 = pPoly->r1 = pPoly->r2 = pPoly->r3 = 0xFF;
                    pPoly->s2 = pPoly->s3 = nS + nSAdd - 1;
                    pPoly->t0 = pPoly->t2 = nTMax;
                    pPoly->t1 = pPoly->t3 = nTMin;
                    fAlpha -= fAlphaDel;
                    nS += nSAdd;
                    if (nS > nSMax) {
                        nS = nSMax;
                    }
                    if (fAlpha < 0.0f) {
                        fAlpha = 0.0f;
                    }
                    pPoly++;
                    nCt++;
                }
                nSizeAdd = sizeof(OMD_FT4);
                break;
            }
            }
            pMph = (OMD_PRIM_H *)((char *)pMph + nSizeAdd * pMph->pc);
            pMph++;
        }
    }
    if (!pParam->bStop) {
        if (!ppvUserStopPartF && !(ppvMng->bFlagStop || ppvMng->bBoolSlow)) {
            pVal->unFrameCt++;
            if (pVal->unFrameCt >= pVal->bSmp) {
                pVal->bStandby = 0;
                pVal->unFrameCt = 0;
            }
        }
        sceVu0CopyVector(ppvMng->parLoc, ppvReferencePosList[0]);
    }
}

static BOOL calc(pppPObject *pPobj, PRyjDrawShipolyBone *pParam, pppCtrlTable *pCtbl) {
    VRyjDrawShipolyBone *pVal;
    int i;
    pppModelSt *pMdl;

    pVal = (VRyjDrawShipolyBone *)&pPobj->val[pCtbl->useVal[RYJDRAWSHIPOLYBONE_THIS]];
    if (pParam->nModel == 0xFFFF) {
        return FALSE;
    }
    if (!ppvReferencePosList || ppvReferencePosNum <= 0) {
        return FALSE;
    }
    if (!pVal->pvecSmp) {
        pVal->bSmp = pParam->bFrameNum;
        pVal->pvecSmp = pppAlloc(&ppvEnv->heapMng, pVal->bSmp * 32);
        if (!pVal->pvecSmp) {
            TRACE("!!!!!!!!!!!!!!!!!!!!!!!!! Effect pppRyjDrawDrawShipolyBone calc Malloc FALSE !!!!!!!!!!!!!!!!!!!!!!!\n");
            return FALSE;
        }
        for (i = 0; i < pVal->bSmp; i++) {
            sceVu0CopyVector(pVal->pvecSmp[i * 2], ppvReferencePosList[0]);
            sceVu0CopyVector(pVal->pvecSmp[i * 2 + 1], ppvReferencePosList[1]);
        }
    }
    pMdl = &ppvEnv->lpModel[pParam->nModel];
    {
        OMD_H *pOmd;
        OMD_PRIM_H *pMph;
        int nCt;
        int i;
        int nSizeAdd;

        pOmd = pMdl->omd;
        pMph = (OMD_PRIM_H *)((char *)pOmd + pOmd->pa);
        nCt = 0;
        while (pMph->ptype != -1) {
            nSizeAdd = 0;
            switch (pMph->ptype) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 5:
            case 7:
                return FALSE;
            case 4: {
                OMD_F4 *pPoly = (OMD_F4 *)(pMph + 1);

                for (i = 0; i < pMph->pc; i++) {
                    pPoly->v0 = nCt * 2;
                    pPoly->v1 = nCt * 2 + 1;
                    pPoly->v2 = nCt * 2 + 2;
                    pPoly->v3 = nCt * 2 + 3;
                    pPoly->b0 = pPoly->b1 = pPoly->b2 = pPoly->b3 = 0xFF;
                    pPoly->g0 = pPoly->g1 = pPoly->g2 = pPoly->g3 = 0xFF;
                    pPoly->r0 = pPoly->r1 = pPoly->r2 = pPoly->r3 = 0xFF;
                    pPoly++;
                    nCt++;
                }
                nSizeAdd = sizeof(OMD_F4);
                break;
            }
            case 6: {
                OMD_FT4 *pPoly = (OMD_FT4 *)(pMph + 1);

                for (i = 0; i < pMph->pc; i++) {
                    pPoly->v0 = nCt * 2;
                    pPoly->v1 = nCt * 2 + 1;
                    pPoly->v2 = nCt * 2 + 2;
                    pPoly->v3 = nCt * 2 + 3;
                    pPoly->b0 = pPoly->b1 = pPoly->b2 = pPoly->b3 = 0xFF;
                    pPoly->g0 = pPoly->g1 = pPoly->g2 = pPoly->g3 = 0xFF;
                    pPoly->r0 = pPoly->r1 = pPoly->r2 = pPoly->r3 = 0xFF;
                    pPoly++;
                    nCt++;
                }
                nSizeAdd = sizeof(OMD_FT4);
                break;
            }
            }
            pMph = (OMD_PRIM_H *)((char *)pMph + nSizeAdd * pMph->pc);
            pMph++;
        }
    }
    if (pMdl->nvert != pMdl->npoly * 2 + 2) {
        return FALSE;
    }
    shipoly(pMdl, pParam, pVal);
    return TRUE;
}

void pppRyjDrawShipolyBone(pppPObject *pPobj, PRyjDrawShipolyBone *pParam, pppCtrlTable *pCtbl) {
}

void pppRyjDrawDrawShipolyBone(pppPObject *pPobj, PRyjDrawShipolyBone *pParam, pppCtrlTable *pCtbl) {
    VColor *pCol;
    OMD_IN Mdi;
    sceVu0FMATRIX matLvw;
    pppModelSt *pMdl;
    OMD_H *pOmd;
    PRyjDrawShipolyBoneTex *pParamTex;

    if (pParam->nModel == 0xFFFF) {
        return;
    }
    if (!ppvReferencePosList || ppvReferencePosNum <= 0) {
        return;
    }
    ppvReferencePosList[0][3] = 1.0f;
    ppvReferencePosList[1][3] = 1.0f;
    if (!calc(pPobj, pParam, pCtbl)) {
        return;
    }
    pCol = (VColor *)&pPobj->val[pCtbl->useVal[RYJDRAWSHIPOLYBONE_COLOR]];
    Mdi.a = pCol->release_col.a;
    if (Mdi.a == 0) {
        return;
    }
    pMdl = &ppvEnv->lpModel[pParam->nModel];
    pParamTex = &((PRyjDrawShipolyBoneOld *)pParam)->sTex;
    if ((u_char)*ppvMng->pchDpd >= 150) {
        pParamTex = &pParam->sTex;
    }
    pOmd = pMdl->omd;
    Mdi.flag = 0;
    if (pParam->bDrawMode & 2) {
        Mdi.flag = 4;
        if (!(pOmd->flag & 1)) {
            Mdi.flag |= 8;
        }
    } else {
        if (pParam->bDrawMode & 1) {
            Mdi.flag = 0x80;
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
    if (pParam->bScissor) {
        Mdi.flag |= 0x2000;
        Mdi.lviewmat = &matLvw;
        sceVu0MulMatrix(matLvw, ppvWorldMatrix, ppvPObj->cmat);
    }
    Mdi.pzmat = 0;
    *(u_short *)&Mdi.alpha = *(u_short *)&pOmd->alpha;
    Mdi.pri = 0;
    *(int *)&Mdi.ar = 0;
    Mdi.va = ppvPObj->lpvert;
    Mdi.na = ppvPObj->lpnorm;
    Mdi.tmp_a = (char *)ppvVNWork;
    sceVu0UnitMatrix(ppvPObj->dmat);
    sceVu0ScaleVector(ppvPObj->dmat[0], ppvPObj->dmat[0], 0.01f);
    sceVu0ScaleVector(ppvPObj->dmat[1], ppvPObj->dmat[1], 0.01f);
    sceVu0ScaleVector(ppvPObj->dmat[2], ppvPObj->dmat[2], 0.01f);
    sceVu0CopyVector(ppvPObj->dmat[3], ppvReferencePosList[0]);
    sceVu0MulMatrix(ppvPObj->dmat, ppvSCMatrix, ppvPObj->dmat);
    Mdi.pmat = &ppvPObj->dmat;
    Mdi.r = pCol->release_col.r;
    Mdi.g = pCol->release_col.g;
    Mdi.b = pCol->release_col.b;
    Mdi.lcmat = &ppvColorMatrix;
    Mdi.lvmat = &ppvLightMatrix;
    *(int *)&Mdi.toff = 0;
    *(u_long *)&Mdi.u = 0;
    *(int *)&Mdi.ot = 0;
    Mdi.lwmat = &ppvPObj->cmat;
    Mdi.packet = ppvCurPrimp;
    Mdi.invisible = pParam->bInvisible;
    Mdi.ztest_off = pParamTex->bZtest;
    ppvCurPrimp = op1_md_draw(pOmd, &Mdi, pMdl->nPacketSize);
}

void pppRyjDrawShipolyBoneCon(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjDrawShipolyBone *pVal = (VRyjDrawShipolyBone *)&pPobj->val[pCtbl->useVal[RYJDRAWSHIPOLYBONE_THIS]];

    pVal->unFrameCt = 0;
    pVal->bSmp = 0;
    pVal->bStandby = 1;
    pVal->pvecSmp = 0;
    /* volatile read: keeps the ppvMng load after the stores as in the original */
    (*(pppMngSt * volatile *)&ppvMng)->bBoolNoViewClip = 1;
}

void pppRyjDrawShipolyBoneDes(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjDrawShipolyBone *pVal = (VRyjDrawShipolyBone *)&pPobj->val[pCtbl->useVal[RYJDRAWSHIPOLYBONE_THIS]];

    if (pVal->pvecSmp) {
        pppFree(&ppvEnv->heapMng, pVal->pvecSmp);
        pVal->pvecSmp = 0;
    }
}
// SOURCE DATA: .data .rodata
