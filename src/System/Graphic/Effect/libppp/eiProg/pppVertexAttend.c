#include "pppVertexAttend.h"

void pppVertexAttend(pppPObject *pobj, PVertexAttend *p, pppCtrlTable *ctbl) {
    VVertexAttend *v;
    VPoint *pos;
    pppVsfSt *vsf;
    pppSVECTOR *vert;
    sceVu0FVECTOR v0;

    if (p->vsfno < 0) {
        return;
    }
    v = (VVertexAttend *)&pobj->val[ctbl->useVal[0]];
    pos = (VPoint *)&pobj->val[ctbl->useVal[1]];
    vsf = &ppvEnv->lpVsf[p->vsfno];
    vert = &ppvEnv->lpModel[vsf->modelno].vert[vsf->index[v->indexno]];
    v0[0] = vert->x;
    v0[1] = vert->y;
    v0[2] = vert->z;
    v0[3] = 1.0f;
    sceVu0ApplyMatrix(v0, pobj->ppobj->cmat, v0);
    pos->pos.x = v0[0];
    pos->pos.y = v0[1];
    pos->pos.z = v0[2];
}
