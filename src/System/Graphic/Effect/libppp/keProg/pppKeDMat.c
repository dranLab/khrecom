#include "pppKeDMat.h"

void pppKeDMatDraw(pppPObject *pobj, PKeDMat *dat, pppCtrlTable *ctbl) {
    VKeDMat *pThis = (VKeDMat *)&pobj->val[ctbl->useVal[1]];

    sceVu0MulMatrix(ppvPObj->dmat, ppvWorldMatrix, ppvPObj->cmat);
    sceVu0CopyMatrix(pThis->KeDMat, ppvPObj->dmat);
    sceVu0MulMatrix(ppvPObj->dmat, ppvScreenMatrix, ppvPObj->dmat);
}
