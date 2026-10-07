#include "pppMatrixXYZ.h"

/* v.xyz *= s (VU0 macro mode) */
#define SCALE_VECTOR_XYZ(v, s)                  \
    __asm__ __volatile__("qmtc2.ni %1, $vf9\n"  \
                         "\tlqc2 $vf10, 0(%0)\n" \
                         "\tvmulx.xyz $vf10, $vf10, $vf9x\n" \
                         "\tsqc2 $vf10, 0(%0)"   \
                         : : "r"(v), "r"(s) : "memory")

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
