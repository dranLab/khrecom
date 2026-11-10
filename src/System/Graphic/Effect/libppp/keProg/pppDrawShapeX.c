#include "pppDrawShapeX.h"
#include "System/Graphic/Effect/libppp/eiProg/pppColor.h"
#include "System/Graphic/Effect/libppp/keProg/pppKeZCrctShp.h"
#include "System/Graphic/Effect/libppp/draw/shape.h"
#include "System/MainApp.h"

void pppDrawShapeXConstruct(pppPObject *pobj, pppCtrlTable *ctbl) {
    VDrawShapeX *vds = (VDrawShapeX *)&pobj->val[ctbl->useVal[IDrawShapeX_This]];

    vds->seq_now = 0;
    vds->seq = 0;
    vds->wait = 0;
}

void pppDrawShapeX(pppPObject *pobj, PDrawShapeX *p, pppCtrlTable *ctbl) {
    VDrawShapeX *vds;
    OANM_HEADER *ah;
    OANM_SEQ *seq;
    pppShapeSt *shp;

    if (ppvUserStopPartF) {
        return;
    }
    if (ppvMng->bFlagStop || ppvMng->bBoolSlow) {
        return;
    }
    vds = (VDrawShapeX *)&pobj->val[ctbl->useVal[IDrawShapeX_This]];
    if (p->shape == 0xFFFF) {
        return;
    }
    debugDpdCheckShapeData(p->shape);
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
            } else {
                vds->seq = vds->seq_now;
            }
            vds->wait = 0;
#line 48
            KH_DEBUG(if (!(vds->seq < ah->seqc)) AssertApp(__FILE__, __LINE__, "vds->seq<ah->seqc", 0);)
        }
#line 49
        KH_DEBUG(if (!(vds->seq_now < ah->seqc)) AssertApp(__FILE__, __LINE__, "vds->seq_now<ah->seqc", 0);)
    }
}

void pppDrawDrawShapeX(pppPObject *pobj, PDrawShapeX *p, pppCtrlTable *ctbl) {
    VDrawShapeX *vds;
    VColor *pv;
    OANM_HEADER *ah;
    OANM_SEQ *seq;
    pppShapeSt *shp;
    OSHAPE_IN shi;
    VKeZCrctShp *pZ;

    vds = (VDrawShapeX *)&pobj->val[ctbl->useVal[IDrawShapeX_This]];
    pv = (VColor *)&pobj->val[ctbl->useVal[IDrawShapeX_Color]];
    if (p->shape == 0xFFFF) {
        return;
    }
    shp = &ppvEnv->lpShape[p->shape];
    if (p->draw_mode == 0) {
        shi.flag = 0x80;
    } else {
        shi.flag = 0;
    }
    if (p->ztest == 1) {
        shi.flag |= 0x10;
    }
    if (p->fog) {
        shi.flag |= 0x800;
    }
    if (p->near) {
        shi.flag |= 0x400;
    }
    if (p->zCrct) {
        pZ = (VKeZCrctShp *)&pobj->val[ctbl->useVal[IDrawShapeX_KeZCrctShp]];
        shi.pzmat = &pZ->Mat;
    } else {
        shi.pzmat = 0;
    }
    shi.pmat = &ppvPObj->dmat;
    shi.r = pv->release_col.r;
    shi.g = pv->release_col.g;
    shi.b = pv->release_col.b;
    shi.a = pv->release_col.a;
    *(u_long *)&shi.toff = 0;
    shi.pri = 0;
    ah = (OANM_HEADER *)shp->shape;
#line 96
    KH_DEBUG(if (!(ah->id == 'F')) AssertApp(__FILE__, __LINE__, "ah->id=='F'", 0);)
    seq = &ah->seq[vds->seq_now];
#line 99
    KH_DEBUG(if (!(vds->seq_now < ah->seqc)) AssertApp(__FILE__, __LINE__, "vds->seq_now<ah->seqc", 0);)
    {
        OSHAPE_VU1 *sh = (OSHAPE_VU1 *)((char *)ah + seq->sha);
        shi.packet = ppvCurPrimp;
        ppvCurPrimp = op_shape_draw(sh, &shi, shp->nPacketSize);
    }
}
// SOURCE DATA: .rodata
