#include "pppRyjMatrixWorldYXZ.h"

/* Angle in radians -> ppp angle units (0x8000 = PI) */
#define PPP_RAD2ANG(r) ((r) * 32768.0f / 3.1415927f)

void pppRyjMatrixWorldYXZCon(pppPObject *pPobj, pppCtrlTable *pCtbl) {
    VRyjMatrixWorldYXZ *pVal = (VRyjMatrixWorldYXZ *)&pPobj->val[pCtbl->useVal[RYJMATRIXWORLDYXZ_THIS]];
    pppIVECTOR iv;

    iv.x = PPP_RAD2ANG(ppvParAng[0]);
    iv.y = PPP_RAD2ANG(ppvParAng[1]);
    iv.z = PPP_RAD2ANG(ppvParAng[2]);
    pppGetRotMatrixYXZ(pVal->matPar, &iv);
    pVal->matPar[3][0] = ppvParLoc[0];
    pVal->matPar[3][1] = ppvParLoc[1];
    pVal->matPar[3][2] = ppvParLoc[2];
}

void pppRyjMatrixWorldYXZ(pppPObject *pPobj, PRyjMatrixWorldYXZ *pParam, pppCtrlTable *pCtbl) {
    VRyjMatrixWorldYXZ *pVal;
    VPoint *pVp;
    VAngle *pVa;
    VScale *pVs;
    sceVu0FMATRIX matParNow;

    pVal = (VRyjMatrixWorldYXZ *)&pPobj->val[pCtbl->useVal[RYJMATRIXWORLDYXZ_THIS]];
    pVp = (VPoint *)&pPobj->val[pCtbl->useVal[RYJMATRIXWORLDYXZ_POINT]];
    pVa = (VAngle *)&pPobj->val[pCtbl->useVal[RYJMATRIXWORLDYXZ_ANGLE]];
    pVs = (VScale *)&pPobj->val[pCtbl->useVal[RYJMATRIXWORLDYXZ_SCALE]];
    pppGetRotMatrixYXZ(ppvPObj->cmat, &pVa->ang);
    ((float *)ppvPObj->cmat)[12] = pVp->pos.x;
    ((float *)ppvPObj->cmat)[13] = pVp->pos.y;
    ((float *)ppvPObj->cmat)[14] = pVp->pos.z;
    /* the original passes the float angles unconverted here (unlike the Con) */
    pppGetRotMatrixYXZ(matParNow, (pppIVECTOR *)ppvParAng);
    matParNow[3][0] = ppvParLoc[0];
    matParNow[3][1] = ppvParLoc[1];
    matParNow[3][2] = ppvParLoc[2];
    sceVu0InversMatrix(matParNow, matParNow);
    sceVu0MulMatrix(ppvPObj->cmat, ppvPObj->cmat, pVal->matPar);
    sceVu0MulMatrix(ppvPObj->cmat, matParNow, ppvPObj->cmat);
    SCALE_VECTOR_XYZ(ppvPObj->cmat[0], pVs->scl.x);
    SCALE_VECTOR_XYZ(ppvPObj->cmat[1], pVs->scl.y);
    SCALE_VECTOR_XYZ(ppvPObj->cmat[2], pVs->scl.z);
}
