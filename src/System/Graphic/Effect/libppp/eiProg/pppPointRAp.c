#include "pppPointRAp.h"
#include "System/Graphic/Effect/libppp/draw/sin_t.h"

void pppPointRApCon(pppPObject *pobj, pppCtrlTable *ctbl) {
    VPointRAp *v = (VPointRAp *)&pobj->val[ctbl->useVal[1]];

    v->aptct = 0;
}

void pppPointRAp(pppPObject *pobj, PPointRAp *p, pppCtrlTable *ctbl) {
    VPointRAp *v;
    pppPObject *apobj;
    VPoint *pos;
    VMove *mov;
    VPoint *vp;
    pppPDataVal *pdv;
    float fr;
    word wr;
    float x;
    float y;
    float z;

    v = (VPointRAp *)&pobj->val[ctbl->useVal[1]];

    if (ppvUserStopPartF != 0) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }

    if (v->aptct == 0) {
        vp = (VPoint *)&pobj->val[ctbl->useVal[0]];
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
            PPP_RAND_F(fr);
            wr = fr * 32768.0f - 16384.0f;
            y = eff_sin_t[((u_int)wr >> 4) & 0xfff] * p->apr;
            z = eff_sin_t[((u_int)(wr + 0x4000) >> 4) & 0xfff] * p->apr;
            PPP_RAND_F(fr);
            wr = fr * 32768.0f * 2.0f;
            x = eff_sin_t[((u_int)wr >> 4) & 0xfff] * z;
            z = z * eff_sin_t[((u_int)(wr + 0x4000) >> 4) & 0xfff];

            pos = (VPoint *)&apobj->val[p->pos.valoff];
            mov = (VMove *)&apobj->val[p->mov.valoff];
            pos->pos.x = vp->pos.x + x;
            pos->pos.y = vp->pos.y + y;
            pos->pos.z = vp->pos.z + z;
            mov->vec.x = x * p->speedr;
            mov->vec.y = y * p->speedr;
            mov->vec.z = z * p->speedr;
        }
        v->aptct = p->faps;
    }
    v->aptct--;
}
