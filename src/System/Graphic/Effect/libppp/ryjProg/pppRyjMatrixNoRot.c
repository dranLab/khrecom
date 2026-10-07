#include "pppRyjMatrixNoRot.h"

#define SCALE_VECTOR_XYZ_TO(d, v, s)            \
    __asm__ __volatile__("qmtc2.ni %2, $vf9\n"  \
                         "\tlqc2 $vf10, 0(%1)\n" \
                         "\tvmulx.xyz $vf10, $vf10, $vf9x\n" \
                         "\tsqc2 $vf10, 0(%0)"   \
                         : : "r"(d), "r"(v), "r"(s) : "memory")

void pppRyjMatrixNoRot(pppPObject *pPobj, PRyjMatrixNoRot *pParam, pppCtrlTable *pCtbl) {
    VPoint *vp;
    VScale *vs;

    vp = (VPoint *)&pPobj->val[pCtbl->useVal[0]];
    vs = (VScale *)&pPobj->val[pCtbl->useVal[2]];
    SCALE_VECTOR_XYZ_TO(ppvPObj->cmat[0], ppvCameraMatrix[0], 1.0f);
    SCALE_VECTOR_XYZ_TO(ppvPObj->cmat[1], ppvCameraMatrix[1], -1.0f);
    SCALE_VECTOR_XYZ_TO(ppvPObj->cmat[2], ppvCameraMatrix[2], -1.0f);
    SCALE_VECTOR_XYZ_TO(ppvPObj->cmat[3], ppvCameraMatrix[3], 1.0f);
    sceVu0InversMatrix(ppvPObj->cmat, ppvPObj->cmat);
    SCALE_VECTOR_XYZ(ppvPObj->cmat[0], vs->scl.x);
    SCALE_VECTOR_XYZ(ppvPObj->cmat[1], vs->scl.y);
    SCALE_VECTOR_XYZ(ppvPObj->cmat[2], vs->scl.z);
    ((float *)ppvPObj->cmat)[12] = vp->pos.x;
    ((float *)ppvPObj->cmat)[13] = vp->pos.y;
    ((float *)ppvPObj->cmat)[14] = vp->pos.z;
}
