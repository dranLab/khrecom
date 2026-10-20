#include "pppKeBornRnd.h"

void pppKeBornRnd(pppPObject *pobj, PKeBornRnd *dat, pppCtrlTable *ctbl) {
    int r;
    sceVu0FMATRIX mat;
    sceVu0FVECTOR fv0;
    VKeBornRnd *_t;
    pppVsfSt *pVsf;
    pppSVECTOR *pTblVert;
    pppSVECTOR *pTblVertS;
    pppSVECTOR *pVert;
    pppPObject *pObj;
    VMove *pMv;
    int md;
    int i;
    int j;
    int k;

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    r = KeVu0RndI();
    if (dat->Pstg == 0xFF) {
        return;
    }
    if ((r >> 8) & dat->Pstg) {
        return;
    }
    if (dat->Pos.pdtn == 0xFFFFFFFF) {
        return;
    }
    if (dat->Mv.pdtn == 0xFFFFFFFF) {
        return;
    }

    pVsf = &ppvEnv->lpVsf[dat->IdxVsf];
    pTblVertS = ppvEnv->lpModel[ppvEnv->lpVsf[dat->SpdVsf].modelno].vert;
    _t = (VKeBornRnd *)&pobj->val[ctbl->useVal[IKeBornRnd_This]];
    pTblVert = ppvPObj->lpvert;
    if (pVsf->indexn != ppvEnv->lpVsf[dat->SpdVsf].indexn) {
        return;
    }
    if (pTblVert == NULL) {
        pTblVert = ppvEnv->lpModel[pVsf->modelno].vert;
    }
    fv0[3] = 1.0f;
    md = dat->Mode; /* before the call: keeps md in s8 */
    sceVu0CopyMatrix(mat, ppvPObj->cmat);
    if (md) {
        j = r % pVsf->indexn;
    } else {
        j = _t->NextIdx;
    }
    for (i = dat->Num; i > 0; i--) {
        pppPDataVal *pdv;

        k = pVsf->index[j];
        pdv = &ppvMng->m_pdtval[dat->Pos.pdtn];
        if (pdv == NULL) {
            pObj = NULL;
        } else {
            pObj = pppCreatePObject(ppvMng, pdv);
            if (pObj != NULL) {
                pObj->ppobj = pobj;
            }
        }
        if (pObj != NULL) {
            float *pPos;
            pVert = &pTblVert[k];
            fv0[0] = pVert->x;
            fv0[1] = pVert->y;
            fv0[2] = pVert->z;
            /* pPos before pMv: Mv.valoff is then loaded after k's last use and pMv shares k's register */
            pPos = (float *)&pObj->val[dat->Pos.valoff];
            pMv = (VMove *)&pObj->val[dat->Mv.valoff];
            sceVu0ApplyMatrix(pPos, ppvPObj->cmat, fv0);
            pVert = &pTblVertS[k];
            fv0[0] = pVert->x;
            fv0[1] = pVert->y;
            fv0[2] = pVert->z;
            sceVu0ApplyMatrix(fv0, mat, fv0);
            KeVu0SubVecXYZ((float *)pMv, fv0, ppvPObj->cmat[3]);
        }
        if (md) {
            j = KeVu0RndI() % pVsf->indexn;
        } else {
            j++;
            if (j >= pVsf->indexn) {
                j = 0;
            }
        }
    }
    _t->NextIdx = j;
}


void pppKeBornRndCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VKeBornRnd *_t = (VKeBornRnd *)&pobj->val[ctbl->useVal[IKeBornRnd_This]];

    _t->NextIdx = 0;
}
