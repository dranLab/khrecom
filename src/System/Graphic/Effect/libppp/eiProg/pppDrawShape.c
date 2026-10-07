#include "pppDrawShape.h"
#include "pppColor.h"
#include "System/Graphic/Effect/libppp/draw/shape.h"

void pppDrawShapeConstruct(pppPObject *pobj, pppCtrlTable *ctbl) {
    VDrawShape *vds = (VDrawShape *)&pobj->val[ctbl->useVal[0]];

    vds->seq_now = 0;
    vds->seq = 0;
    vds->wait = 0;
}

void pppDrawShape(pppPObject *pobj, PDrawShape *p, pppCtrlTable *ctbl) {
    VDrawShape *vds;
    OANM_HEADER *ah;
    OANM_SEQ *seq;
    pppShapeSt *shp;

    if (ppvUserStopPartF) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    vds = (VDrawShape *)&pobj->val[ctbl->useVal[0]];
    if (p->shape == 0xFFFF) {
        return;
    }
    shp = &ppvEnv->lpShape[p->shape];
    ah = (OANM_HEADER *)shp->shape;
    seq = &ah->seq[vds->seq_now];
    vds->seq_now = vds->seq;
    vds->wait += p->delta;
    if (vds->wait >= seq->wait) {
        vds->wait -= seq->wait;
        vds->seq++;
        if (vds->seq >= ah->seqc) {
            if (seq->flag & 0x80) {
                vds->seq = 0;
                vds->wait = 0;
            } else {
                vds->seq = vds->seq_now;
                vds->wait = 0;
            }
        }
    }
}

void pppDrawDrawShape(pppPObject *pobj, PDrawShape *p, pppCtrlTable *ctbl) {
    VDrawShape *vds;
    VColor *pv;
    OSHAPE_IN shi;
    OANM_HEADER *ah;
    OANM_SEQ *seq;
    pppShapeSt *shp;

    vds = (VDrawShape *)&pobj->val[ctbl->useVal[0]];
    pv = (VColor *)&pobj->val[ctbl->useVal[1]];
    if (p->shape == 0xFFFF) {
        return;
    }
    shp = &ppvEnv->lpShape[p->shape];
    shi.pmat = &ppvPObj->dmat;
    *(u_long *)&shi.toff = 0;
    shi.pri = 0;
    shi.pzmat = 0;
    if (p->draw_mode == 0) {
        shi.flag = 0x80;
    } else {
        shi.flag = 0;
    }
    if (p->ztest == 1) {
        shi.flag |= 0x10;
    }
    shi.r = pv->release_col.r;
    shi.g = pv->release_col.g;
    shi.b = pv->release_col.b;
    shi.a = pv->release_col.a;
    ah = (OANM_HEADER *)shp->shape;
    seq = &ah->seq[vds->seq_now];
    {
        OSHAPE_VU1 *sh = (OSHAPE_VU1 *)((char *)ah + seq->sha);
        shi.packet = ppvCurPrimp;
        ppvCurPrimp = op_shape_draw(sh, &shi, shp->nPacketSize);
    }
}
