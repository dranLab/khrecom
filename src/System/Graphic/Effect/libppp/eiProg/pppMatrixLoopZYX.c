#include "pppMatrixLoopZYX.h"

void pppMatrixLoopZYX(pppPObject *pobj, PMatrixLoopZYX *p, pppCtrlTable *ctbl) {
    VPointLoop *vp;
    VAngleLoop *va;
    VScaleLoop *vs;

    vp = (VPointLoop *)&pobj->val[ctbl->useVal[0]];
    va = (VAngleLoop *)&pobj->val[ctbl->useVal[1]];
    vs = (VScaleLoop *)&pobj->val[ctbl->useVal[2]];
    pppGetRotMatrixZYX(ppvPObj->cmat, &va->m_ang);
    SCALE_VECTOR_XYZ(ppvPObj->cmat[0], vs->m_scl.x);
    SCALE_VECTOR_XYZ(ppvPObj->cmat[1], vs->m_scl.y);
    SCALE_VECTOR_XYZ(ppvPObj->cmat[2], vs->m_scl.z);
    ((float *)ppvPObj->cmat)[12] = vp->m_pos.x;
    ((float *)ppvPObj->cmat)[13] = vp->m_pos.y;
    ((float *)ppvPObj->cmat)[14] = vp->m_pos.z;
}
