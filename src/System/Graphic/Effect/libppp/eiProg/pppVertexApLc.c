#include "pppVertexApLc.h"

void pppVertexApLcCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VVertexApLc *v = (VVertexApLc *)&pobj->val[ctbl->useVal[0]];

    v->aptct = v->ict = 0;
}

static void apea(pppPObject *ppobj, PVertexApLc *p, pppSVECTOR *mv0) {
    pppPObject *apobj;
    VPoint *pos;
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
            apobj->ppobj = ppobj;
        }
    }
    if (apobj != NULL) {
        pos = (VPoint *)&apobj->val[p->pos.valoff];
        pos->pos.x = mv0->x;
        pos->pos.y = mv0->y;
        pos->pos.z = mv0->z;
    }
}

void pppVertexApLc(pppPObject *pobj, PVertexApLc *p, pppCtrlTable *ctbl) {
    VVertexApLc *v;
    int ct;
    pppVsfSt *vsf;
    pppSVECTOR *vert;
    pppSVECTOR v0;
    float r;

    v = (VVertexApLc *)&pobj->val[ctbl->useVal[0]];
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
