#include "pppRyjDrawMatrixWorld.h"

void pppRyjDrawMatrixWorldCon(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjDrawMatrixWorld *pVal = (VRyjDrawMatrixWorld *)&pPobj->val[pCtbl->useVal[RYJDRAWMATRIXWORLD_THIS]];

    sceVu0CopyMatrix(pVal->matPar, ppvParMatrix);
}

void pppRyjDrawDrawMatrixWorld(pppPObject *pPobj, PRyjDrawMatrixWorld *pParam, pppCtrlTable *pCtbl) {
    VRyjDrawMatrixWorld *pVal = (VRyjDrawMatrixWorld *)&pPobj->val[pCtbl->useVal[RYJDRAWMATRIXWORLD_THIS]];

    sceVu0MulMatrix(ppvPObj->dmat, ppvCameraMatrix, pVal->matPar);
    sceVu0MulMatrix(ppvPObj->dmat, ppvScreenMatrix, ppvPObj->dmat);
    sceVu0MulMatrix(ppvPObj->dmat, ppvPObj->dmat, ppvPObj->cmat);
}
