#include "pppParMatrix.h"

void pppParMatrix(pppPObject *pobj, void *p, pppCtrlTable *ctbl) {
    sceVu0ApplyMatrix(ppvPObj->cmat[3], pobj->ppobj->cmat, ppvPObj->cmat[3]);
}
