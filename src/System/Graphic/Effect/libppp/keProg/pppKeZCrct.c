#include "pppKeZCrct.h"

void pppKeZCrctDraw(pppPObject *pobj, PKeZCrct *dat, pppCtrlTable *ctbl) {
    VKeZCrct *Self = (VKeZCrct *)&pobj->val[ctbl->useVal[IKeZCrct_This]];
    sceVu0FMATRIX fm0;

    sceVu0ScaleVector(fm0[0], ppvPObj->cmat[0], dat->Scl.x);
    sceVu0ScaleVector(fm0[1], ppvPObj->cmat[1], dat->Scl.y);
    sceVu0ScaleVector(fm0[2], ppvPObj->cmat[2], dat->Scl.z);
    sceVu0CopyVector(fm0[3], ppvPObj->cmat[3]);

    switch (dat->PosMd) {
    case 0:
        fm0[3][0] += dat->Pos.x * ppvParScl[0];
        fm0[3][1] += dat->Pos.y * ppvParScl[1];
        fm0[3][2] += dat->Pos.z * ppvParScl[2];
        sceVu0MulMatrix(fm0, ppvWorldMatrix, fm0);
        break;
    case 1:
        if (dat->Pos.x != 0.0f || dat->Pos.y != 0.0f || dat->Pos.z != 0.0f) {
            sceVu0MulMatrix(fm0, ppvWorldMatrix, fm0);
        } else {
            sceVu0ScaleVector(fm0[0], fm0[0], ppvParScl[0]);
            sceVu0ScaleVector(fm0[1], fm0[1], ppvParScl[1]);
            sceVu0ScaleVector(fm0[2], fm0[2], ppvParScl[2]);
            sceVu0ApplyMatrix(fm0[3], ppvWorldMatrix, fm0[3]);
        }
        fm0[3][0] += dat->Pos.x * ppvParScl[0];
        fm0[3][1] += dat->Pos.y * ppvParScl[1];
        fm0[3][2] += dat->Pos.z * ppvParScl[2];
        break;
    case 2:
        sceVu0MulMatrix(fm0, ppvParMatrix, fm0);
        fm0[3][0] += dat->Pos.x * ppvParScl[0];
        fm0[3][1] += -dat->Pos.y * ppvParScl[1];
        fm0[3][2] += -dat->Pos.z * ppvParScl[2];
        sceVu0MulMatrix(fm0, ppvCameraMatrix, fm0);
        break;
    case 3:
        sceVu0ScaleVector(fm0[0], fm0[0], ppvParScl[0]);
        sceVu0ScaleVector(fm0[1], fm0[1], ppvParScl[1]);
        sceVu0ScaleVector(fm0[2], fm0[2], ppvParScl[2]);
        sceVu0MulMatrix(fm0, ppvWorldMatrixWood, fm0);
        fm0[3][0] += dat->Pos.x * ppvParScl[0];
        fm0[3][1] += dat->Pos.y * ppvParScl[1];
        fm0[3][2] += dat->Pos.z * ppvParScl[2];
        break;
    }
    sceVu0MulMatrix(Self->Mat, ppvScreenMatrix, fm0);
}
