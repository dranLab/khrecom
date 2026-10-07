#include "pppRyjDrawMatrixWorldNoRot.h"

void pppRyjDrawMatrixWorldNoRotCon(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjDrawMatrixWorldNoRot *pVal = (VRyjDrawMatrixWorldNoRot *)&pPobj->val[pCtbl->useVal[RYJDRAWMATRIXWORLDNOROT_THIS]];

    sceVu0CopyVector(pVal->vecPos, ppvParLoc);
    sceVu0CopyVector(pVal->vecScl, ppvParScl);
}

void pppRyjDrawDrawMatrixWorldNoRot(pppPObject *pPobj, PRyjDrawMatrixWorldNoRot *pParam, pppCtrlTable *pCtbl) {
    VRyjDrawMatrixWorldNoRot *pVal = (VRyjDrawMatrixWorldNoRot *)&pPobj->val[pCtbl->useVal[RYJDRAWMATRIXWORLDNOROT_THIS]];
    sceVu0FMATRIX matW;

    sceVu0UnitMatrix(matW);
    matW[3][0] = pVal->vecPos[0];
    matW[3][1] = pVal->vecPos[1];
    matW[3][2] = pVal->vecPos[2];
    sceVu0MulMatrix(matW, ppvCameraMatrix, matW);
    sceVu0ScaleVector(ppvPObj->dmat[0], ppvPObj->cmat[0], pVal->vecScl[0]);
    sceVu0ScaleVector(ppvPObj->dmat[1], ppvPObj->cmat[1], pVal->vecScl[1]);
    sceVu0ScaleVector(ppvPObj->dmat[2], ppvPObj->cmat[2], pVal->vecScl[2]);
    sceVu0MulVector(ppvPObj->dmat[3], ppvPObj->cmat[3], pVal->vecScl);
    ((float *)ppvPObj->dmat)[12] += matW[3][0];
    ((float *)ppvPObj->dmat)[13] += matW[3][1];
    ((float *)ppvPObj->dmat)[14] += matW[3][2];
    sceVu0MulMatrix(ppvPObj->dmat, ppvScreenMatrix, ppvPObj->dmat);
}
