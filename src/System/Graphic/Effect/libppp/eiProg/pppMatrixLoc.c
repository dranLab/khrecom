#include "pppMatrixLoc.h"

void pppMatrixLoc(pppPObject *pobj, PMatrixLoc *p, pppCtrlTable *ctbl) {
    VPoint *vp;

    vp = (VPoint *)&pobj->val[ctbl->useVal[0]];
    sceVu0UnitMatrix(ppvPObj->cmat);
    ((float *)ppvPObj->cmat)[12] = vp->pos.x;
    ((float *)ppvPObj->cmat)[13] = vp->pos.y;
    ((float *)ppvPObj->cmat)[14] = vp->pos.z;
}
