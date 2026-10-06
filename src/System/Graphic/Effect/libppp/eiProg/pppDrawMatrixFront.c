#include "pppDrawMatrixFront.h"

void pppDrawMatrixFront(pppPObject *pobj, PDrawMatrixFront *p, pppCtrlTable *ctbl) {
    sceVu0ScaleVector(ppvPObj->dmat[0], ppvPObj->cmat[0], ppvParScl[0]);
    sceVu0ScaleVector(ppvPObj->dmat[1], ppvPObj->cmat[1], ppvParScl[1]);
    sceVu0ScaleVector(ppvPObj->dmat[2], ppvPObj->cmat[2], ppvParScl[2]);
    sceVu0ApplyMatrix(ppvPObj->dmat[3], ppvWorldMatrix, ppvPObj->cmat[3]);
    sceVu0MulMatrix(ppvPObj->dmat, ppvScreenMatrix, ppvPObj->dmat);
}
