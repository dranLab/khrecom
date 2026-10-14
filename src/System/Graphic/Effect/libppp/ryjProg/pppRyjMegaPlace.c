#include "pppRyjMegaPlace.h"
#include "System/Graphic/Effect/libppp/pppAlloc.h"
#include "System/Graphic/Effect/libppp/draw/shape.h"
#include "System/Graphic/Effect/libppp/draw/md.h"
#include "System/Graphic/Effect/libppp/draw/sin_t.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixXYZ.h"

extern sceVu0FMATRIX ppvSCMatrix;
void *memset(void *s, int c, unsigned int n);

typedef struct _MEGA_SHAPE { /* 0x20 */
    sceVu0FVECTOR vecPos;
    PRyjMegaPlaceShape *pMegaPlaceShape;
    void *pvDpp;
    u_short ushAnmWait;
    u_short ushAnmSeq;
} MEGA_SHAPE;

typedef struct _MEGA_MODEL { /* 0x20 */
    sceVu0FVECTOR vecPos;
    PRyjMegaPlaceModel *pMegaPlaceModel;
    void *pvDpp;
} MEGA_MODEL;

typedef struct _MEGA_LAMP { /* 0x30 */
    sceVu0FVECTOR vecPos;
    PRyjMegaPlaceLamp *pMegaPlaceLamp;
    void *pvDpp;
    u_short ushAnmWait;
    u_short ushAnmSeq;
    u_char bFlickerTimeCtA;
    u_char bFlickerTimeCtB;
    u_short ushFlickerDec;
    int nFlicker;
} MEGA_LAMP;

static void *get_dpp(int nDppId) {
    return 0;
}

int entry_megaplace_shape(void *pvMegaPlace, PRyjMegaPlaceShape *pMegaPlaceShape) {
    VRyjMegaPlace *pVal = (VRyjMegaPlace *)pvMegaPlace;
    int i;
    MEGA_SHAPE *pShape;

    if (!pVal->pvShape) {
        return 0;
    }
    for (i = 0; i < pVal->unShapeNum; i++) {
        pShape = &((MEGA_SHAPE *)pVal->pvShape)[i];
        if (!pShape->pMegaPlaceShape) {
            pShape->pMegaPlaceShape = pMegaPlaceShape;
            pShape->pvDpp = get_dpp(pMegaPlaceShape->unDppId);
            pShape->vecPos[0] += pMegaPlaceShape->Pos.x;
            pShape->vecPos[1] += pMegaPlaceShape->Pos.y;
            pShape->vecPos[2] += pMegaPlaceShape->Pos.z;
            pShape->vecPos[3] = 1.0f;
            break;
        }
    }
    return 1;
}

int entry_megaplace_model(void *pvMegaPlace, PRyjMegaPlaceModel *pMegaPlaceModel) {
    VRyjMegaPlace *pVal = (VRyjMegaPlace *)pvMegaPlace;
    int i;
    MEGA_MODEL *pModel;

    if (!pVal->pvModel) {
        return 0;
    }
    for (i = 0; i < pVal->unModelNum; i++) {
        pModel = &((MEGA_MODEL *)pVal->pvModel)[i];
        if (!pModel->pMegaPlaceModel) {
            pModel->pMegaPlaceModel = pMegaPlaceModel;
            pModel->pvDpp = get_dpp(pMegaPlaceModel->unDppId);
            pModel->vecPos[0] += pMegaPlaceModel->Pos.x;
            pModel->vecPos[1] += pMegaPlaceModel->Pos.y;
            pModel->vecPos[2] += pMegaPlaceModel->Pos.z;
            pModel->vecPos[3] = 1.0f;
            break;
        }
    }
    return 1;
}

int entry_megaplace_lamp(void *pvMegaPlace, PRyjMegaPlaceLamp *pMegaPlaceLamp) {
    VRyjMegaPlace *pVal = (VRyjMegaPlace *)pvMegaPlace;
    MEGA_LAMP *pLamp = (MEGA_LAMP *)pVal->pvLamp;
    int i;

    if (!pLamp) {
        return 0;
    }
    for (i = 0; i < pVal->unLampNum; i++) {
        if (!pLamp[i].pMegaPlaceLamp) {
            pLamp[i].pMegaPlaceLamp = pMegaPlaceLamp;
            pLamp[i].pvDpp = get_dpp(pMegaPlaceLamp->unDppId);
            pLamp[i].bFlickerTimeCtA = pMegaPlaceLamp->bFlickerTimeA;
            pLamp[i].bFlickerTimeCtB = pMegaPlaceLamp->bFlickerTimeB;
            pLamp[i].ushFlickerDec = 0x7FFF / pMegaPlaceLamp->bFlickerFrame;
            pLamp[i].vecPos[0] += pMegaPlaceLamp->Pos.x;
            pLamp[i].vecPos[1] += pMegaPlaceLamp->Pos.y;
            pLamp[i].vecPos[2] += pMegaPlaceLamp->Pos.z;
            pLamp[i].vecPos[3] = 1.0f;
            break;
        }
    }
    return 1;
}

void pppRyjMegaPlace(pppPObject *pPobj, PRyjMegaPlace *pParam, pppCtrlTable *pCtbl) {
    VRyjMegaPlace *pVal = (VRyjMegaPlace *)&pPobj->val[pCtbl->useVal[0]];

    if (!pVal->pvShape && pVal->unShapeNum) {
        pVal->pvShape = pppAlloc(&ppvEnv->heapMng, pVal->unShapeNum * sizeof(MEGA_SHAPE));
        if (pVal->pvShape) {
            memset(pVal->pvShape, 0, pVal->unShapeNum * sizeof(MEGA_SHAPE));
        }
    }
    if (!pVal->pvModel && pVal->unModelNum) {
        pVal->pvModel = pppAlloc(&ppvEnv->heapMng, pVal->unModelNum * sizeof(MEGA_MODEL));
        if (pVal->pvModel) {
            memset(pVal->pvModel, 0, pVal->unModelNum * sizeof(MEGA_MODEL));
        }
    }
    if (!pVal->pvLamp && pVal->unLampNum) {
        pVal->pvLamp = pppAlloc(&ppvEnv->heapMng, pVal->unLampNum * sizeof(MEGA_LAMP));
        if (pVal->pvLamp) {
            memset(pVal->pvLamp, 0, pVal->unLampNum * sizeof(MEGA_LAMP));
        }
    }
}

static void draw_shape(VRyjMegaPlace *pVal) {
    OSHAPE_IN Shi;
    OSHAPE_VU1 *pSh;
    OANM_HEADER *pAh;
    OANM_SEQ *pSeq;
    pppShapeSt *pShp;
    sceVu0FMATRIX matZ;
    int i;
    MEGA_SHAPE *pShape;
    PRyjMegaPlaceShape *pParam;
    int nAnmSeq;

    if (!pVal->pvShape) {
        return;
    }
    Shi.pmat = &ppvPObj->dmat;
    *(u_long *)&Shi.toff = 0;
    Shi.pri = 0;
    for (i = 0; i < pVal->unShapeNum; i++) {
        pShape = &((MEGA_SHAPE *)pVal->pvShape)[i];
        pParam = pShape->pMegaPlaceShape;
        if (!pParam) {
            continue;
        }
        if (!pParam->Col.a) {
            continue;
        }
        Shi.flag = 0;
        if (pParam->bZtest == 1) {
            Shi.flag = 0x10;
        }
        if (pParam->bFog) {
            Shi.flag |= 0x800;
        }
        if (pParam->bNear) {
            Shi.flag |= 0x400;
        }
        nAnmSeq = pShape->ushAnmSeq;
        if (pParam->nShape == 0xFFFF) {
            continue;
        }
        pShp = &ppvEnv->lpShape[pParam->nShape];
        Shi.r = pParam->Col.r;
        Shi.g = pParam->Col.g;
        Shi.b = pParam->Col.b;
        Shi.a = pParam->Col.a;
        pAh = (OANM_HEADER *)pShp->shape;
        pSeq = &pAh->seq[nAnmSeq];
        pSh = (OSHAPE_VU1 *)((char *)pAh + pSeq->sha);
        Shi.pzmat = 0;
        sceVu0UnitMatrix(ppvPObj->dmat);
        ((float *)ppvPObj->dmat)[0] = pParam->Scl.x;
        ((float *)ppvPObj->dmat)[5] = pParam->Scl.y;
        ((float *)ppvPObj->dmat)[10] = pParam->Scl.z;
        sceVu0CopyMatrix(ppvPObj->cmat, ppvPObj->dmat);
        sceVu0CopyVector(ppvPObj->cmat[3], pShape->vecPos);
        sceVu0ApplyMatrix(ppvPObj->dmat[3], ppvCameraMatrix, pShape->vecPos);
        if (pParam->bZcrct) {
            sceVu0CopyMatrix(matZ, ppvPObj->dmat);
            matZ[3][0] += pParam->ZcrctPos.x;
            matZ[3][1] += pParam->ZcrctPos.y;
            matZ[3][2] += pParam->ZcrctPos.z;
            sceVu0MulMatrix(matZ, ppvScreenMatrix, matZ);
            Shi.pzmat = &matZ;
        }
        sceVu0MulMatrix(ppvPObj->dmat, ppvScreenMatrix, ppvPObj->dmat);
        Shi.packet = ppvCurPrimp;
        ppvCurPrimp = op_shape_draw(pSh, &Shi, pShp->nPacketSize);
        if (ppvUserStopPartF) {
            continue;
        }
        if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
            continue;
        }
        pShape->ushAnmWait += pParam->unDelta;
        if (pShape->ushAnmWait >= pSeq->wait) {
            pShape->ushAnmWait -= pSeq->wait;
            nAnmSeq = pShape->ushAnmSeq;
            pShape->ushAnmSeq = nAnmSeq + 1;
            if (pShape->ushAnmSeq >= pAh->seqc) {
                if (pSeq->flag & 0x80) {
                    pShape->ushAnmSeq = 0;
                    pShape->ushAnmWait = 0;
                } else {
                    pShape->ushAnmSeq = nAnmSeq;
                    pShape->ushAnmWait = 0;
                }
            }
        }
    }
}

static void draw_model(VRyjMegaPlace *pVal) {
    sceVu0FMATRIX matZ;
    OMD_IN Mdi;
    int i;
    MEGA_MODEL *pModel;
    PRyjMegaPlaceModel *pParam;
    pppModelSt *pMdl;
    OMD_H *pOmd;

    if (!pVal->pvModel) {
        return;
    }
    Mdi.lwmat = 0;
    Mdi.lvmat = 0;
    Mdi.lcmat = 0;
    Mdi.pri = 0;
    *(int *)&Mdi.ar = 0;
    Mdi.va = ppvPObj->lpvert;
    Mdi.na = ppvPObj->lpnorm;
    Mdi.tmp_a = (char *)ppvVNWork;
    Mdi.pmat = &ppvPObj->dmat;
    *(int *)&Mdi.toff = 0;
    *(u_long *)&Mdi.u = 0;
    *(int *)&Mdi.ot = 0;
    for (i = 0; i < pVal->unModelNum; i++) {
        pModel = &((MEGA_MODEL *)pVal->pvModel)[i];
        pParam = pModel->pMegaPlaceModel;
        if (!pParam) {
            continue;
        }
        if (!pParam->Col.a) {
            continue;
        }
        if (pParam->nModel == 0xFFFF) {
            continue;
        }
        pppGetRotMatrixXYZ(ppvPObj->cmat, &pParam->Ang);
        SCALE_VECTOR_XYZ(ppvPObj->cmat[0], pParam->Scl.x);
        SCALE_VECTOR_XYZ(ppvPObj->cmat[1], pParam->Scl.y);
        SCALE_VECTOR_XYZ(ppvPObj->cmat[2], pParam->Scl.z);
        sceVu0CopyVector(ppvPObj->cmat[3], pModel->vecPos);
        if (pParam->bZcrct) {
            sceVu0MulMatrix(matZ, ppvCameraMatrix, ppvPObj->cmat);
            matZ[3][0] += pParam->ZcrctPos.x;
            matZ[3][1] += pParam->ZcrctPos.y;
            matZ[3][2] += pParam->ZcrctPos.z;
            sceVu0MulMatrix(matZ, ppvScreenMatrix, matZ);
            Mdi.pzmat = &matZ;
        } else {
            Mdi.pzmat = 0;
        }
        sceVu0MulMatrix(ppvPObj->dmat, ppvSCMatrix, ppvPObj->cmat);
        pMdl = &ppvEnv->lpModel[pParam->nModel];
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
        *(u_short *)&Mdi.alpha = *(u_short *)&pOmd->alpha;
        Mdi.r = pParam->Col.r;
        Mdi.g = pParam->Col.g;
        Mdi.b = pParam->Col.b;
        Mdi.a = pParam->Col.a;
        Mdi.invisible = pParam->bInvisible;
        Mdi.ztest_off = pParam->bZtest;
        Mdi.packet = ppvCurPrimp;
        ppvCurPrimp = op1_md_draw(pOmd, &Mdi, pMdl->nPacketSize);
    }
}

static void draw_lamp(VRyjMegaPlace *pVal) {
    OSHAPE_IN Shi;
    OSHAPE_VU1 *pSh;
    OANM_HEADER *pAh;
    OANM_SEQ *pSeq;
    pppShapeSt *pShp;
    sceVu0FMATRIX matZ;
    int i;
    MEGA_LAMP *pLamp;
    PRyjMegaPlaceLamp *pParam;
    int nAnmSeq;
    float fSin;
    int n;

    if (!pVal->pvLamp) {
        return;
    }
    Shi.pmat = &ppvPObj->dmat;
    *(u_long *)&Shi.toff = 0;
    Shi.pri = 0;
    for (i = 0; i < pVal->unLampNum; i++) {
        pLamp = &((MEGA_LAMP *)pVal->pvLamp)[i];
        pParam = pLamp->pMegaPlaceLamp;
        if (!pParam) {
            continue;
        }
        if (--pLamp->bFlickerTimeCtA == 0xFF) {
            pLamp->bFlickerTimeCtA = pParam->bFlickerTimeA;
            if (pLamp->nFlicker == 0) {
                pLamp->nFlicker = 0x7FFF;
            }
        }
        if (--pLamp->bFlickerTimeCtB == 0xFF) {
            pLamp->bFlickerTimeCtB = pParam->bFlickerTimeB;
            if (pLamp->nFlicker == 0) {
                pLamp->nFlicker = 0x7FFF;
            }
        }
        if (pLamp->nFlicker) {
            pLamp->nFlicker -= pLamp->ushFlickerDec;
            if (pLamp->nFlicker < 0) {
                pLamp->nFlicker = 0;
            }
        }
        Shi.flag = 0;
        if (pParam->bZtest == 1) {
            Shi.flag = 0x10;
        }
        if (pParam->bFog) {
            Shi.flag |= 0x800;
        }
        if (pParam->bNear) {
            Shi.flag |= 0x400;
        }
        nAnmSeq = pLamp->ushAnmSeq;
        if (pParam->nShape == 0xFFFF) {
            continue;
        }
        pShp = &ppvEnv->lpShape[pParam->nShape];
        Shi.r = pParam->Col.r;
        Shi.g = pParam->Col.g;
        Shi.b = pParam->Col.b;
        fSin = eff_sin_t[(pLamp->nFlicker & 0xFFFF) >> 4];
        if (fSin < 0.0f) {
            fSin = -fSin;
        }
        n = pParam->Col.a - (u_char)(pParam->bFlickerRange * fSin);
        if (n < 0) {
            n = 0;
        }
        Shi.a = n;
        pAh = (OANM_HEADER *)pShp->shape;
        pSeq = &pAh->seq[nAnmSeq];
        pSh = (OSHAPE_VU1 *)((char *)pAh + pSeq->sha);
        Shi.pzmat = 0;
        sceVu0UnitMatrix(ppvPObj->dmat);
        ((float *)ppvPObj->dmat)[0] = pParam->Scl.x;
        ((float *)ppvPObj->dmat)[5] = pParam->Scl.y;
        ((float *)ppvPObj->dmat)[10] = pParam->Scl.z;
        sceVu0CopyMatrix(ppvPObj->cmat, ppvPObj->dmat);
        sceVu0CopyVector(ppvPObj->cmat[3], pLamp->vecPos);
        sceVu0ApplyMatrix(ppvPObj->dmat[3], ppvCameraMatrix, pLamp->vecPos);
        if (pParam->bZcrct) {
            sceVu0CopyMatrix(matZ, ppvPObj->dmat);
            matZ[3][0] += pParam->ZcrctPos.x;
            matZ[3][1] += pParam->ZcrctPos.y;
            matZ[3][2] += pParam->ZcrctPos.z;
            sceVu0MulMatrix(matZ, ppvScreenMatrix, matZ);
            Shi.pzmat = &matZ;
        }
        sceVu0MulMatrix(ppvPObj->dmat, ppvScreenMatrix, ppvPObj->dmat);
        Shi.packet = ppvCurPrimp;
        ppvCurPrimp = op_shape_draw(pSh, &Shi, pShp->nPacketSize);
        if (ppvUserStopPartF) {
            continue;
        }
        if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
            continue;
        }
        pLamp->ushAnmWait += pParam->unDelta;
        if (pLamp->ushAnmWait >= pSeq->wait) {
            /* not in the original debug info: nAnmSeq (a3) or n here changes the register allocation */
            int nSeq;

            pLamp->ushAnmWait -= pSeq->wait;
            nSeq = pLamp->ushAnmSeq + 1;
            pLamp->ushAnmSeq = nSeq;
            if (pLamp->ushAnmSeq >= pAh->seqc) {
                if (pSeq->flag & 0x80) {
                    pLamp->ushAnmSeq = 0;
                    pLamp->ushAnmWait = 0;
                } else {
                    pLamp->ushAnmSeq = nSeq - 1;
                    pLamp->ushAnmWait = 0;
                }
            }
        }
    }
}

void pppRyjDrawMegaPlace(pppPObject *pPobj, PRyjMegaPlace *pParam, pppCtrlTable *pCtbl) {
    VRyjMegaPlace *pVal = (VRyjMegaPlace *)&pPobj->val[pCtbl->useVal[0]];

    draw_shape(pVal);
    draw_model(pVal);
    draw_lamp(pVal);
}

void pppRyjMegaPlaceCon(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjMegaPlace *pVal = (VRyjMegaPlace *)&pPobj->val[pCtbl->useVal[0]];

    pVal->unShapeNum = pVal->unModelNum = pVal->unLampNum = 0;
    pVal->pvShape = pVal->pvModel = pVal->pvLamp = 0;
}

void pppRyjMegaPlaceDes(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjMegaPlace *pVal = (VRyjMegaPlace *)&pPobj->val[pCtbl->useVal[0]];

    if (pVal->pvShape) {
        pppFree(&ppvEnv->heapMng, pVal->pvShape);
        pVal->pvShape = 0;
    }
    if (pVal->pvModel) {
        pppFree(&ppvEnv->heapMng, pVal->pvModel);
        pVal->pvModel = 0;
    }
    if (pVal->pvLamp) {
        pppFree(&ppvEnv->heapMng, pVal->pvLamp);
        pVal->pvLamp = 0;
    }
}
