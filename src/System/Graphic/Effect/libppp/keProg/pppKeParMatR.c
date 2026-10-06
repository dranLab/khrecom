#include "pppKeParMatR.h"

void pppKeParMatR(pppPObject *pobj, PKeParMatR *p, pppCtrlTable *ctbl) {
    sceVu0FMATRIX fm0;
    sceVu0FVECTOR fv0;
    sceVu0FMATRIX *mat;

    mat = &pobj->ppobj->cmat;
    sceVu0Normalize(fm0[0], (*mat)[0]);
    sceVu0Normalize(fm0[1], (*mat)[1]);
    sceVu0Normalize(fm0[2], (*mat)[2]);
    sceVu0ApplyMatrix(fv0, *mat, ppvPObj->cmat[3]);
    sceVu0MulMatrix(ppvPObj->cmat, fm0, ppvPObj->cmat);
    sceVu0CopyVector(ppvPObj->cmat[3], fv0);
}
