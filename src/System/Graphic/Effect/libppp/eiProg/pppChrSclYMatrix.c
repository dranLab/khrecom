#include "pppChrSclYMatrix.h"

void pppChrSclYMatrix(pppPObject *pobj, PChrSclYMatrix *p, pppCtrlTable *ctbl) {
    sceVu0FMATRIX mat;

    sceVu0UnitMatrix(mat);
    mat[1][1] = ppvMng->parChrScl[1] + ppvMng->parChrScl[1] * p->sclr;
    if (mat[1][1] > p->maxscl) {
        mat[1][1] = p->maxscl;
    }
    sceVu0MulMatrix(ppvPObj->cmat, mat, ppvPObj->cmat);
}
