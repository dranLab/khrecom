#include "pppKeDMatPht.h"
#include "System/Graphic/Effect/libppp/keProg/KeDMat.h"

void pppKeDMatPhtDraw(pppPObject *pobj, PKeDMatPht *dat, pppCtrlTable *ctbl) {
    sceVu0MulMatrix(KeDMat, ppvWorldMatrix, KeMatPht);
    sceVu0ScaleVector(ppvPObj->dmat[0], ppvPObj->cmat[0], ppvParScl[0]);
    sceVu0ScaleVector(ppvPObj->dmat[1], ppvPObj->cmat[1], ppvParScl[1]);
    sceVu0ScaleVector(ppvPObj->dmat[2], ppvPObj->cmat[2], ppvParScl[2]);
    sceVu0ApplyMatrix(ppvPObj->dmat[3], ppvWorldMatrix, ppvPObj->cmat[3]);
    sceVu0MulMatrix(ppvPObj->dmat, ppvScreenMatrix, ppvPObj->dmat);
}
