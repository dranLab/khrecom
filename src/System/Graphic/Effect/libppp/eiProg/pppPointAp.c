#include "pppPointAp.h"

pppPObject *pppCreatePObject(pppMngSt *mng, pppPDataVal *pdv0);

void pppPointApCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VPointAp *v = (VPointAp *)&pobj->val[ctbl->useVal[1]];

    v->aptct = 0;
}

void pppPointAp(pppPObject *pobj, PPointAp *p, pppCtrlTable *ctbl) {
    VPoint *vp;
    VPointAp *v;
    pppPObject *apobj;
    VPoint *pos;
    int apct;
    pppPDataVal *pdv;

    vp = (VPoint *)&pobj->val[ctbl->useVal[0]];
    v = (VPointAp *)&pobj->val[ctbl->useVal[1]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (v->aptct == 0) {
        if ((u_char)*ppvMng->pchDpd >= 150) {
            apct = p->apsu;
        } else {
            if (p->pos.pdtn == 0xFFFFFFFF) {
                return;
            }
            apct = 1;
        }
        while (apct-- != 0) {
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
                pos = (VPoint *)&apobj->val[p->pos.valoff];
                pos->pos.x = vp->pos.x;
                pos->pos.y = vp->pos.y;
                pos->pos.z = vp->pos.z;
            }
        }
        v->aptct = p->faps;
    }
    v->aptct--;
}
