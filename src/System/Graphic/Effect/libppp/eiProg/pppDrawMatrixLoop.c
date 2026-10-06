#include "pppDrawMatrixLoop.h"

void pppDrawMatrixLoop(pppPObject *pobj, PDrawMatrixLoop *p, pppCtrlTable *ctbl) {
    sceVu0MulMatrix(ppvPObj->dmat, ppvSWMatrix, ppvPObj->cmat);
}
