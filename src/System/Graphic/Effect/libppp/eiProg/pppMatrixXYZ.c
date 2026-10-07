#include "pppMatrixXYZ.h"

void pppMatrixXYZ(pppPObject *pobj, PMatrixXYZ *p, pppCtrlTable *ctbl) {
    VPoint *vp;
    VAngle *va;
    VScale *vs;

    vp = (VPoint *)&pobj->val[ctbl->useVal[0]];
    va = (VAngle *)&pobj->val[ctbl->useVal[1]];
    vs = (VScale *)&pobj->val[ctbl->useVal[2]];
    pppGetRotMatrixXYZ(ppvPObj->cmat, &va->ang);
    SCALE_VECTOR_XYZ(ppvPObj->cmat[0], vs->scl.x);
    SCALE_VECTOR_XYZ(ppvPObj->cmat[1], vs->scl.y);
    SCALE_VECTOR_XYZ(ppvPObj->cmat[2], vs->scl.z);
    ((float *)ppvPObj->cmat)[12] = vp->pos.x;
    ((float *)ppvPObj->cmat)[13] = vp->pos.y;
    ((float *)ppvPObj->cmat)[14] = vp->pos.z;
}
