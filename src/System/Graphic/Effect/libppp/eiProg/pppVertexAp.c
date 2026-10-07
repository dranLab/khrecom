#include "pppVertexAp.h"

void pppVertexApCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VVertexAp *v = (VVertexAp *)&pobj->val[ctbl->useVal[0]];

    v->aptct = v->ict = 0;
}

static void apea(pppPObject *pobj, PVertexAp *p, pppSVECTOR *mv0) {
    pppPObject *apobj;
    VPoint *pos;
    sceVu0FVECTOR v0;
    pppPDataVal *pdv;

    if (p->pos.pdtn == 0xFFFFFFFF) {
        return;
    }
    pdv = &ppvMng->m_pdtval[p->pos.pdtn];
    if (pdv == NULL) {
        apobj = NULL;
    } else {
        apobj = pppCreatePObject(ppvMng, pdv);
        if (apobj != NULL) {
            apobj->ppobj = pobj;
        }
    }
    if (apobj != NULL) {
        v0[0] = mv0->x;
        v0[1] = mv0->y;
        v0[2] = mv0->z;
        v0[3] = 1.0f;
        sceVu0ApplyMatrix(v0, ppvPObj->cmat, v0);
        pos = (VPoint *)&apobj->val[p->pos.valoff];
        pos->pos.x = v0[0];
        pos->pos.y = v0[1];
        pos->pos.z = v0[2];
    }
}

void pppVertexAp(pppPObject *pobj, PVertexAp *p, pppCtrlTable *ctbl) {
    VVertexAp *v;
    int ct;
    pppVsfSt *vsf;
    pppSVECTOR *vert;
    pppSVECTOR v0;
    float r;

    v = (VVertexAp *)&pobj->val[ctbl->useVal[0]];
    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    if (ppvEmptyLoop != 0) {
        return;
    }
    if (p->vsfno < 0) {
        return;
    }

    if (v->aptct == 0) {
        vsf = &ppvEnv->lpVsf[p->vsfno];
        vert = ppvPObj->lpvert;
        if (vert == NULL) {
            vert = ppvEnv->lpModel[vsf->modelno].vert;
        }
        ct = p->fapn;
        switch (p->aptype) {
        case 0:
            while (ct-- != 0) {
                if (v->ict >= vsf->indexn) {
                    v->ict = 0;
                }
                v0 = vert[vsf->index[v->ict++]];
                apea(pobj, p, &v0);
            }
            break;
        case 1:
            while (ct-- != 0) {
                PPP_RAND_F(r);
                v0 = vert[vsf->index[(int)(r * vsf->indexn)]];
                apea(pobj, p, &v0);
            }
            break;
        }
        v->aptct = p->faps;
    }
    v->aptct--;
}
