#include "pppVertexApAt.h"

void pppVertexApAtCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VVertexApAt *v = (VVertexApAt *)&pobj->val[ctbl->useVal[0]];

    v->aptct = v->ict = 0;
}

static void apea(pppPObject *ppobj, PVertexApAt *p, u_short indexno) {
    pppPObject *apobj;
    VVertexAttend *att;
    pppPDataVal *pdv;

    if (p->att.pdtn == 0xFFFFFFFF) {
        return;
    }
    pdv = &ppvMng->m_pdtval[p->att.pdtn];
    if (pdv == NULL) {
        apobj = NULL;
    } else {
        apobj = pppCreatePObject(ppvMng, pdv);
        if (apobj != NULL) {
            apobj->ppobj = ppobj;
        }
    }
    if (apobj != NULL) {
        att = (VVertexAttend *)&apobj->val[p->att.valoff];
        att->indexno = indexno;
    }
}

void pppVertexApAt(pppPObject *pobj, PVertexApAt *p, pppCtrlTable *ctbl) {
    VVertexApAt *v;
    int ct;
    pppVsfSt *vsf;
    float r;

    v = (VVertexApAt *)&pobj->val[ctbl->useVal[0]];
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
        ct = p->fapn;
        switch (p->aptype) {
        case 0:
            while (ct-- != 0) {
                if (v->ict >= vsf->indexn) {
                    v->ict = 0;
                }
                apea(pobj, p, v->ict++);
            }
            break;
        case 1:
            while (ct-- != 0) {
                PPP_RAND_F(r);
                apea(pobj, p, (int)(r * vsf->indexn));
            }
            break;
        }
        v->aptct = p->faps;
    }
    v->aptct--;
}
