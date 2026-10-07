#include "pppRyjDrawMatrixWorldWood.h"

void pppRyjDrawMatrixWorldWoodCon(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjDrawMatrixWorldWood *pVal = (VRyjDrawMatrixWorldWood *)&pPobj->val[pCtbl->useVal[RYJDRAWMATRIXWORLDWOOD_THIS]];
    sceVu0FVECTOR vecX;
    sceVu0FVECTOR vecY;
    sceVu0FVECTOR vecZ;

    sceVu0CopyVector(pVal->vecPos, ppvParLoc);
    sceVu0CopyVector(pVal->vecScl, ppvParScl);

    vecY[0] = ppvWorldMatrix[1][0];
    vecY[1] = ppvWorldMatrix[1][1];
    vecY[2] = ppvWorldMatrix[1][2];
    vecY[3] = 0.0f;
    sceVu0Normalize(vecY, vecY);
    *(u_long128 *)ppvWorldMatrixWood[1] = *(u_long128 *)vecY;

    vecX[0] = vecY[1];
    vecX[1] = -vecY[0];
    vecX[2] = 0.0f;
    vecX[3] = 0.0f;
    sceVu0Normalize(vecX, vecX);
    *(u_long128 *)ppvWorldMatrixWood[0] = *(u_long128 *)vecX;

    sceVu0OuterProduct(vecZ, vecX, vecY);
    sceVu0Normalize(vecZ, vecZ);
    *(u_long128 *)ppvWorldMatrixWood[2] = *(u_long128 *)vecZ;
    *(u_long128 *)ppvWorldMatrixWood[3] = *(u_long128 *)ppvWorldMatrix[3];

    sceVu0MulMatrix(ppvSWMatrixWood, ppvScreenMatrix, ppvWorldMatrixWood);
}

void pppRyjDrawDrawMatrixWorldWood(pppPObject *pPobj, PRyjDrawMatrixWorldWood *pParam, pppCtrlTable *pCtbl) {
    VRyjDrawMatrixWorldWood *pVal = (VRyjDrawMatrixWorldWood *)&pPobj->val[pCtbl->useVal[RYJDRAWMATRIXWORLDWOOD_THIS]];
    sceVu0FMATRIX matSW;

    sceVu0UnitMatrix(matSW);
    matSW[3][0] = pVal->vecPos[0];
    matSW[3][1] = pVal->vecPos[1];
    matSW[3][2] = pVal->vecPos[2];
    sceVu0MulMatrix(matSW, ppvCameraMatrix, matSW);
    sceVu0MulMatrix(matSW, ppvScreenMatrix, matSW);

    sceVu0ScaleVector(ppvPObj->dmat[0], ppvPObj->cmat[0], pVal->vecScl[0]);
    sceVu0ScaleVector(ppvPObj->dmat[1], ppvPObj->cmat[1], pVal->vecScl[1]);
    sceVu0ScaleVector(ppvPObj->dmat[2], ppvPObj->cmat[2], pVal->vecScl[2]);
    *(u_long128 *)ppvPObj->dmat[3] = *(u_long128 *)ppvPObj->cmat[3];
    sceVu0MulMatrix(ppvPObj->dmat, ppvSWMatrixWood, ppvPObj->dmat);
    sceVu0ApplyMatrix(ppvPObj->dmat[3], matSW, ppvPObj->cmat[3]);
}
