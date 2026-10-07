#include "pppDrawMatrixNoRot.h"

void pppDrawMatrixNoRot(pppPObject *pobj, void *p, pppCtrlTable *ctbl) {
    sceVu0ScaleVector(ppvPObj->dmat[0], ppvPObj->cmat[0], ppvParScl[0]);
    sceVu0ScaleVector(ppvPObj->dmat[1], ppvPObj->cmat[1], ppvParScl[1]);
    sceVu0ScaleVector(ppvPObj->dmat[2], ppvPObj->cmat[2], ppvParScl[2]);
    sceVu0MulVector(ppvPObj->dmat[3], ppvPObj->cmat[3], ppvParScl);
    ((float *)ppvPObj->dmat)[12] += ppvWorldMatrix[3][0];
    ((float *)ppvPObj->dmat)[13] += ppvWorldMatrix[3][1];
    ((float *)ppvPObj->dmat)[14] += ppvWorldMatrix[3][2];
    ((float *)ppvPObj->dmat)[15] = 1.0f;
    sceVu0MulMatrix(ppvPObj->dmat, ppvScreenMatrix, ppvPObj->dmat);
}
