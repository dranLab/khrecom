#include "pppChrSclXYZMatrix.h"

void pppChrSclXYZMatrix(pppPObject *pobj, PChrSclXYZMatrix *p, pppCtrlTable *ctbl) {
    sceVu0FMATRIX mat;

    sceVu0UnitMatrix(mat);
    mat[0][0] = ppvMng->parChrScl[0] + ppvMng->parChrScl[0] * p->sclr;
    if (mat[0][0] > p->maxscl) {
        mat[0][0] = p->maxscl;
    }
    mat[2][2] = mat[0][0];
    mat[1][1] = ppvMng->parChrScl[1] + ppvMng->parChrScl[1] * p->sclr;
    if (mat[1][1] > p->maxscl) {
        mat[1][1] = p->maxscl;
    }
    sceVu0MulMatrix(ppvPObj->cmat, mat, ppvPObj->cmat);
}
