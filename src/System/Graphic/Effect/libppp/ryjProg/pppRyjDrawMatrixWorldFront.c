#include "pppRyjDrawMatrixWorldFront.h"

void pppRyjDrawMatrixWorldFrontCon(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjDrawMatrixWorldFront *pVal = (VRyjDrawMatrixWorldFront *)&pPobj->val[pCtbl->useVal[RYJDRAWMATRIXWORLDFRONT_THIS]];

    sceVu0CopyVector(pVal->vecPos, ppvParLoc);
    sceVu0CopyVector(pVal->vecScl, ppvParScl);
}

void pppRyjDrawDrawMatrixWorldFront(pppPObject *pPobj, PRyjDrawMatrixWorldFront *pParam, pppCtrlTable *pCtbl) {
    VRyjDrawMatrixWorldFront *pVal = (VRyjDrawMatrixWorldFront *)&pPobj->val[pCtbl->useVal[RYJDRAWMATRIXWORLDFRONT_THIS]];
    sceVu0FMATRIX matW;

    sceVu0UnitMatrix(matW);
    sceVu0ScaleVector(matW[0], matW[0], pVal->vecScl[0]);
    sceVu0ScaleVector(matW[1], matW[1], -pVal->vecScl[1]);
    sceVu0ScaleVector(matW[2], matW[2], -pVal->vecScl[2]);
    matW[3][0] = pVal->vecPos[0];
    matW[3][1] = pVal->vecPos[1];
    matW[3][2] = pVal->vecPos[2];
    sceVu0MulMatrix(matW, ppvCameraMatrix, matW);
    sceVu0ScaleVector(ppvPObj->dmat[0], ppvPObj->cmat[0], pVal->vecScl[0]);
    sceVu0ScaleVector(ppvPObj->dmat[1], ppvPObj->cmat[1], pVal->vecScl[1]);
    sceVu0ScaleVector(ppvPObj->dmat[2], ppvPObj->cmat[2], pVal->vecScl[2]);
    sceVu0ApplyMatrix(ppvPObj->dmat[3], matW, ppvPObj->cmat[3]);
    sceVu0MulMatrix(ppvPObj->dmat, ppvScreenMatrix, ppvPObj->dmat);
}
