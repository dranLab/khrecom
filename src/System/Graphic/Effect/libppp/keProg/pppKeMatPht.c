#include "pppKeMatPht.h"

void pppKeMatPht(pppPObject *pobj, PKeMatPht *p, pppCtrlTable *ctbl) {
    VPoint *vp;
    VAngle *va;
    VScale *vs;

    vp = (VPoint *)&pobj->val[ctbl->useVal[0]];
    va = (VAngle *)&pobj->val[ctbl->useVal[1]];
    vs = (VScale *)&pobj->val[ctbl->useVal[2]];
    pppGetRotMatrixZYX(KeMatPht, &va->ang);
    SCALE_VECTOR_XYZ(KeMatPht[0], vs->scl.x);
    SCALE_VECTOR_XYZ(KeMatPht[1], vs->scl.y);
    SCALE_VECTOR_XYZ(KeMatPht[2], vs->scl.z);
    KeMatPht[3][0] = vp->pos.x;
    KeMatPht[3][1] = vp->pos.y;
    KeMatPht[3][2] = vp->pos.z;
    sceVu0UnitMatrix(ppvPObj->cmat);
    ((float *)ppvPObj->cmat)[0] = vs->scl.x;
    ((float *)ppvPObj->cmat)[5] = vs->scl.y;
    ((float *)ppvPObj->cmat)[10] = vs->scl.z;
    sceVu0CopyVector(ppvPObj->cmat[3], KeMatPht[3]);
}
