#include "pppChrXSclXYZMatrix.h"

void pppChrXSclXYZMatrix(pppPObject *pobj, PChrXSclXYZMatrix *p, pppCtrlTable *ctbl) {
    sceVu0FMATRIX mat;

    sceVu0UnitMatrix(mat);
    mat[0][0] = ppvMng->parChrScl[0] + ppvMng->parChrScl[0] * p->sclr;
    if (mat[0][0] > p->maxscl) {
        mat[0][0] = p->maxscl;
    }
    mat[2][2] = mat[0][0];
    mat[1][1] = mat[0][0];
    sceVu0MulMatrix(ppvPObj->cmat, mat, ppvPObj->cmat);
}
