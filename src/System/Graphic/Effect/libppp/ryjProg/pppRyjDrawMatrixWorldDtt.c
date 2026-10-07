#include "pppRyjDrawMatrixWorldDtt.h"

void pppRyjDrawMatrixWorldDttCon(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjDrawMatrixWorldDtt *pVal = (VRyjDrawMatrixWorldDtt *)&pPobj->val[pCtbl->useVal[RYJDRAWMATRIXWORLDDTT_THIS]];

    sceVu0CopyMatrix(pVal->matPar, ppvParMatrix);
}

void pppRyjDrawDrawMatrixWorldDtt(pppPObject *pPobj, PRyjDrawMatrixWorldDtt *pParam, pppCtrlTable *pCtbl) {
    VRyjDrawMatrixWorldDtt *pVal = (VRyjDrawMatrixWorldDtt *)&pPobj->val[pCtbl->useVal[RYJDRAWMATRIXWORLDDTT_THIS]];

    sceVu0MulMatrix(ppvPObj->dmat, pVal->matPar, ppvPObj->cmat);
    sceVu0MulMatrix(ppvPObj->dmat, ppvCameraMatrix, ppvPObj->dmat);
    sceVu0CopyMatrix(pVal->KeDMat, ppvPObj->dmat);
    sceVu0MulMatrix(ppvPObj->dmat, ppvScreenMatrix, ppvPObj->dmat);
}
