#include "pppDrawMatrix.h"

void pppDrawMatrix(pppPObject *pobj, void *p, pppCtrlTable *ctbl) {
    sceVu0MulMatrix(ppvPObj->dmat, ppvSWMatrix, ppvPObj->cmat);
}
