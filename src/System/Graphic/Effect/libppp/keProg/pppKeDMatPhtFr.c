#include "pppKeDMatPhtFr.h"
#include "System/Graphic/Effect/libppp/keProg/KeDMat.h"

void pppKeDMatPhtFrDraw(pppPObject *pobj, PKeDMatPhtFr *dat, pppCtrlTable *ctbl) {
    sceVu0ScaleVector(KeDMat[0], KeMatPht[0], ppvParScl[0]);
    sceVu0ScaleVector(KeDMat[1], KeMatPht[1], ppvParScl[1]);
    sceVu0ScaleVector(KeDMat[2], KeMatPht[2], ppvParScl[2]);
    sceVu0ApplyMatrix(KeDMat[3], ppvWorldMatrix, KeMatPht[3]);
    sceVu0ScaleVector(ppvPObj->dmat[0], ppvPObj->cmat[0], ppvParScl[0]);
    sceVu0ScaleVector(ppvPObj->dmat[1], ppvPObj->cmat[1], ppvParScl[1]);
    sceVu0ScaleVector(ppvPObj->dmat[2], ppvPObj->cmat[2], ppvParScl[2]);
    sceVu0ApplyMatrix(ppvPObj->dmat[3], ppvWorldMatrix, ppvPObj->cmat[3]);
    sceVu0MulMatrix(ppvPObj->dmat, ppvScreenMatrix, ppvPObj->dmat);
}
