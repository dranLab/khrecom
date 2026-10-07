#include "pppKeZCrctShp.h"

void pppKeZCrctShpDraw(pppPObject *pobj, PKeZCrctShp *dat, pppCtrlTable *ctbl) {
    VKeZCrctShp *Self = (VKeZCrctShp *)&pobj->val[ctbl->useVal[0]];
    sceVu0FMATRIX fm0;
    sceVu0FVECTOR fv0;

    sceVu0ScaleVector(fm0[0], ppvPObj->cmat[0], ppvParScl[0]);
    sceVu0ScaleVector(fm0[1], ppvPObj->cmat[1], ppvParScl[1]);
    sceVu0ScaleVector(fm0[2], ppvPObj->cmat[2], ppvParScl[2]);
    sceVu0CopyVector(fv0, ppvPObj->cmat[3]);
    fv0[0] *= dat->Scl.x;
    fv0[1] *= dat->Scl.y;
    fv0[2] *= dat->Scl.z;

    switch (dat->PosMd) {
    case 0:
        fv0[0] += dat->Pos.x * ppvParScl[0];
        fv0[1] += dat->Pos.y * ppvParScl[1];
        fv0[2] += dat->Pos.z * ppvParScl[2];
        sceVu0ApplyMatrix(fm0[3], ppvWorldMatrix, fv0);
        break;
    case 1:
        sceVu0ApplyMatrix(fm0[3], ppvWorldMatrix, fv0);
        fm0[3][0] += dat->Pos.x * ppvParScl[0];
        fm0[3][1] += dat->Pos.y * ppvParScl[1];
        fm0[3][2] += dat->Pos.z * ppvParScl[2];
        break;
    case 2:
        sceVu0ApplyMatrix(fm0[3], ppvParMatrix, fv0);
        fm0[3][0] += dat->Pos.x * ppvParScl[0];
        fm0[3][1] += -dat->Pos.y * ppvParScl[1];
        fm0[3][2] += -dat->Pos.z * ppvParScl[2];
        sceVu0ApplyMatrix(fm0[3], ppvCameraMatrix, fm0[3]);
        break;
    }
    sceVu0MulMatrix(Self->Mat, ppvScreenMatrix, fm0);
}
