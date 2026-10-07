#include "pppMatrixScl.h"

void pppMatrixScl(pppPObject *pobj, PMatrixScl *p, pppCtrlTable *ctbl) {
    VPoint *vp;
    VScale *vs;

    vp = (VPoint *)&pobj->val[ctbl->useVal[0]];
    vs = (VScale *)&pobj->val[ctbl->useVal[1]];
    sceVu0UnitMatrix(ppvPObj->cmat);
    ((float *)ppvPObj->cmat)[0] = vs->scl.x;
    ((float *)ppvPObj->cmat)[5] = vs->scl.y;
    ((float *)ppvPObj->cmat)[10] = vs->scl.z;
    ((float *)ppvPObj->cmat)[12] = vp->pos.x;
    ((float *)ppvPObj->cmat)[13] = vp->pos.y;
    ((float *)ppvPObj->cmat)[14] = vp->pos.z;
}
