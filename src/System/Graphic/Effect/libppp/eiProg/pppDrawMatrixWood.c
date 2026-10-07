#include "pppDrawMatrixWood.h"

void pppDrawMatrixWoodSta(pppMngSt *mng) {
}

void pppDrawMatrixWood(pppPObject *pobj, void *p, pppCtrlTable *ctbl) {
    sceVu0ScaleVector(ppvPObj->dmat[0], ppvPObj->cmat[0], ppvParScl[0]);
    sceVu0ScaleVector(ppvPObj->dmat[1], ppvPObj->cmat[1], ppvParScl[1]);
    sceVu0ScaleVector(ppvPObj->dmat[2], ppvPObj->cmat[2], ppvParScl[2]);
    *(u_long128 *)ppvPObj->dmat[3] = *(u_long128 *)ppvPObj->cmat[3];
    sceVu0MulMatrix(ppvPObj->dmat, ppvSWMatrixWood, ppvPObj->dmat);
    sceVu0ApplyMatrix(ppvPObj->dmat[3], ppvSWMatrix, ppvPObj->cmat[3]);
}
