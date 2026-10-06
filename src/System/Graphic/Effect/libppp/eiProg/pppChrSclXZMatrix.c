#include "pppChrSclXZMatrix.h"

void pppChrSclXZMatrix(pppPObject *pobj, PChrSclXZMatrix *p, pppCtrlTable *ctbl) {
    sceVu0FMATRIX mat;

    sceVu0UnitMatrix(mat);
    mat[0][0] = ppvMng->parChrScl[0] + ppvMng->parChrScl[0] * p->sclr;
    if (mat[0][0] > p->maxscl) {
        mat[0][0] = p->maxscl;
    }
    mat[2][2] = mat[0][0];
    sceVu0MulMatrix(ppvPObj->cmat, mat, ppvPObj->cmat);
}
