#include "pppChrYSclXYZMatrix.h"

void pppChrYSclXYZMatrix(pppPObject *pobj, PChrYSclXYZMatrix *p, pppCtrlTable *ctbl) {
    sceVu0FMATRIX mat;

    sceVu0UnitMatrix(mat);
    mat[1][1] = ppvMng->parChrScl[1] + ppvMng->parChrScl[1] * p->sclr;
    if (mat[1][1] > p->maxscl) {
        mat[1][1] = p->maxscl;
    }
    /* original bug: the clamped Y scale is overwritten with mat[2][2] */
    mat[1][1] = mat[2][2];
    mat[0][0] = mat[1][1];
    sceVu0MulMatrix(ppvPObj->cmat, mat, ppvPObj->cmat);
}
