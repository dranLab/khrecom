#include "KeBornRnd.h"

void KeBornRnd2(pppPObject *pObj, void *pDat, void *fnc, u_short *pIdxNxt, short idxVsf, u_char md, u_char num) {
    pppVsfSt *pVsf;
    pppSVECTOR *pTblVert;
    int j;

    pVsf = &ppvEnv->lpVsf[idxVsf];
    pTblVert = ppvPObj->lpvert;
    if (pTblVert == 0) {
        pTblVert = ppvEnv->lpModel[pVsf->modelno].vert;
    }
    if (md) {
        j = KeVu0RndI() % pVsf->indexn;
    } else {
        j = *pIdxNxt;
    }
    for (; num; num--) {
        ((KetBornRnd_Born)fnc)(pObj, pDat, &pTblVert[pVsf->index[j]]);
        if (md) {
            j = KeVu0RndI() % pVsf->indexn;
        } else {
            j++;
            if (j >= pVsf->indexn) {
                j = 0;
            }
        }
    }
    *pIdxNxt = j;
}
